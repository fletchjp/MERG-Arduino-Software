//
/// ESP32 CAN WiFi Bridge
/// (c) Duncan Greenwood, 2019, 2020
//

/*

  Copyright (C) Duncan Greenwood, 2019

  This work is licensed under the:
      Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
   To view a copy of this license, visit:
      http://creativecommons.org/licenses/by-nc-sa/4.0/
   or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

   License summary:
    You are free to:
      Share, copy and redistribute the material in any medium or format
      Adapt, remix, transform, and build upon the material

    The licensor cannot revoke these freedoms as long as you follow the license terms.

    Attribution : You must give appropriate credit, provide a link to the license,
                   and indicate if changes were made. You may do so in any reasonable manner,
                   but not in any way that suggests the licensor endorses you or your use.

    NonCommercial : You may not use the material for commercial purposes. **(see note below)

    ShareAlike : If you remix, transform, or build upon the material, you must distribute
                  your contributions under the same license as the original.

    No additional restrictions : You may not apply legal terms or technological measures that
                                  legally restrict others from doing anything the license permits.

   ** For commercial use, please contact the original copyright holder(s) to agree licensing terms

    This software is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

*/

#include <WiFi.h>
#include "defs.h"

// variables defined in other files
extern QueueHandle_t gc_out_queue, CAN_out_from_GC_queue, logger_in_queue, \
CAN_out_queue, net_out_queue, led_cmd_queue, gc_to_gc_queue, wsserver_out_queue, \
withrottle_queue, cmdproxy_queue, cbus_in_queue;
extern config_t config_data;
extern stats_t stats, errors;
extern byte num_peers, num_wi_clients;

// global variables
gcclient_t gc_clients[MAX_GC_CLIENTS + 1];
byte num_gc_clients;
const byte max_retries = 5;

// forward function declarations
void process_input_data(const byte i);
bool send_message_to_client(const byte i, const char *buffer,  const size_t s);

//
/// task to implement a Gridconnect server
//

void gc_task(void *params) {

  WiFiServer server;
  twai_message_t cf;
  wrapped_gc_t gc;
  char buffer[GC_INP_SIZE];
  byte i;
  unsigned long stimer = millis();

  LOG("gc_task: task starting");

  // don't continue if GC server is not configured
  if (config_data.role == ROLE_SLAVE || !config_data.gc_server_on) {
    LOG("gc_task: GC server not configured to run, suspending task");
    vTaskSuspend(NULL);
  }

  // initialise client records
  for (i = 0; i < MAX_GC_CLIENTS + 1; i++) {
    gc_clients[i].client = NULL;                // client object
    gc_clients[i].input[0] = 0;                 // input data working buffer
    gc_clients[i].buffer[0] = 0;                // in progress GC string buffer
    gc_clients[i].idx = 0;                      // index into GC string buffer, where next char is stored
    gc_clients[i].addr[0] = 0;                  // peer IP address
    gc_clients[i].port = 0;                     // peer remote port
  }

  // hardcode serial client identity if configured
  if (config_data.gc_serial_on) {
    strcpy(gc_clients[SERIAL_CLIENT].addr, "SERIAL");
    gc_clients[SERIAL_CLIENT].port = 99;
    LOG("gc_task: serial client is enabled");
  }

  // set initial number of connected clients
  // serial client is always connected if configured

  num_gc_clients = (config_data.gc_serial_on) ? 1 : 0;

  // start TCP socket server on configured port number
  server.begin(config_data.gc_server_port);
  VLOG("gc_task: started GC server on port = %d", config_data.gc_server_port);

  // infinite loop
  for (;;) {

    //
    /// check for new GC client connections
    //

    WiFiClient client = server.available();

    if (client) {
      for (i = 0; i < MAX_GC_CLIENTS; i++) {
        if (gc_clients[i].client == NULL) {
          gc_clients[i].client = new WiFiClient(client);
          gc_clients[i].idx = 0;
          strcpy(gc_clients[i].addr, gc_clients[i].client->remoteIP().toString().c_str());
          gc_clients[i].port = gc_clients[i].client->remotePort();
          ++num_gc_clients;
          break;
        }
      }

      if (i == MAX_GC_CLIENTS) {
        LOG("gc_task: too many clients, new connection rejected");
        client.stop();
        PULSE_LED(ERR_IND_LED);
      } else {
        VLOG("gc_task: accepted new incoming connection, index = %d, total =  %d, ip = %s, port = %d", i, num_gc_clients, gc_clients[i].addr, gc_clients[i].port);
        PULSE_LED(NET_ACT_LED);
      }
    }

    //
    /// check for serial input (if configured)
    //

    if (config_data.gc_serial_on && Serial.available()) {
      process_input_data(SERIAL_CLIENT);
    }

    //
    /// check for active network clients sending us data
    //

    for (i = 0; i < MAX_GC_CLIENTS; i++) {

      if (gc_clients[i].client != NULL) {
        if (gc_clients[i].client->connected()) {
          if (gc_clients[i].client->available()) {

            // VLOG("gc_task: client %d is available for reading", i);
            process_input_data(i);

          }  // is available
        } else {

          // remote client has disconnected
          gc_clients[i].client->stop();
          delete gc_clients[i].client;
          gc_clients[i].client = NULL;
          gc_clients[i].buffer[0] = 0;
          gc_clients[i].idx = 0;
          gc_clients[i].addr[0] = 0;
          gc_clients[i].port = 0;
          --num_gc_clients;
          VLOG("gc_task: reaped disconnected client at index = %d, new count = %d", i, num_gc_clients);

        }  // is connected
      }  // client is not null
    }  // for each client

    //
    /// process GC-to-GC queue
    /// reflect incoming GC messages to all GC clients except the originator
    //

    if (xQueueReceive(gc_to_gc_queue, &gc, QUEUE_OP_TIMEOUT_SHORT) == pdTRUE) {
      // VLOG("gc_task: received message from GC-to-GC queue");
      // VLOG("gc_task: message IP = %s, port = %d", gc.addr, gc.port);
      size_t s = strlen(gc.msg);
      byte cc = 0;

      for (i = 0; i <= MAX_GC_CLIENTS; i++) {
        // match on port number as there may be multiple clients from the same IP address
        if (gc_clients[i].port > 0 && gc_clients[i].port != gc.port) {

          // VLOG("gc_task: client ip = %s, port = %d", gc_clients[i].addr, gc_clients[i].port);

          if (!send_message_to_client(i, gc.msg, s)) {
            LOG("gc_task: error sending message to GC client");
            PULSE_LED(ERR_IND_LED);
            ++errors.gc_tx;
          } else {
            // VLOG("gc_task: reflected GC-GC msg to client = %d", i);
            ++stats.gc_tx;
            ++cc;
          }
        }  // if client *does not* match
      }  // for each client

      // VLOG("gc_task: reflected message to %d clients", cc);
      PULSE_LED(NET_ACT_LED);
    }

    //
    /// process CAN frames from output queue, convert to GC string & send to active GC clients
    //

    if (xQueueReceive(gc_out_queue, &cf, QUEUE_OP_TIMEOUT_SHORT) == pdTRUE) {

      // VLOG("gc_task: got new frame from output queue: %s", format_CAN_frame(&cf));

      if (num_gc_clients > 0) {
        if (CANtoGC(&cf, buffer)) {

          // VLOG("gc_task: CANtoGC returns %s", buffer);
          size_t s = strlen(buffer);
          byte cc = 0;

          for (i = 0; i <= MAX_GC_CLIENTS; i++) {

            if (gc_clients[i].port != 0) {
              if (!send_message_to_client(i, buffer, s)) {
                LOG("gc_task: error sending incoming CAN frame to GC client");
                PULSE_LED(ERR_IND_LED);
                ++errors.gc_tx;
              } else {
                // VLOG("gc_task: sent GC msg to client = %d, bytes = %d", i, s);
                ++stats.gc_tx;
                ++cc;
              }
            }
          }  // for each client

          // VLOG("gc_task: sent to %d clients", cc);
          PULSE_LED(NET_ACT_LED);
        }  // if converted
      }  // have active clients
    }  // if message dequeued

    //
    /// periodically log connected clients
    //

    if (millis() - stimer >= 10000UL) {
      stimer = millis();
      VLOG("gc_task: [%d] clients = %d", config_data.CANID, num_gc_clients);

      if (num_gc_clients > 0) {
        for (byte i = 0; i < MAX_GC_CLIENTS + 1; i++) {
          if (gc_clients[i].port != 0) {
            VLOG("gc_task: client %d: %s/%d", i, gc_clients[i].addr, gc_clients[i].port);
          }
        }
      }
    }
  }  // for (;;)
}

//
/// convert from CAN frame to Gridconnect string
/// caller must allocate the correct buffer size (32 bytes is more than safe)
//

const char hex_chars[] = "0123456789ABCDEF";

bool CANtoGC(twai_message_t *cf, char buffer[GC_INP_SIZE]) {

  char t[8];
  uint8_t i, idx;
  uint32_t tid;

  // VLOG("CANtoGC: CAN header = %d, CANID = %d", cf->identifier, cf->identifier & 0x7f);

  bzero(buffer, GC_INP_SIZE);
  buffer[0] = ':';
  buffer[1] = (cf->flags & TWAI_MSG_FLAG_EXTD) ? 'X' : 'S';

  // shift header to left justify per CBUS spec
  tid = cf->identifier;
  tid = tid << 5;

  snprintf(t, 7, "%04X", tid);    // is there a cheaper way ?
  idx = strlen(t) + 2;
  strcat(buffer, t);

  // VLOG("CANtoGC: header string = %s, idx = %d, buffer = %s", t, idx, buffer);

  buffer[idx++] = (cf->flags & TWAI_MSG_FLAG_RTR) ? 'R' : 'N';

  // data payload
  // cheaper than using sprintf
  for (i = 0; i < cf->data_length_code; i++) {
    buffer[idx++] = hex_chars[cf->data[i] >> 4];          // upper nibble
    buffer[idx++] = hex_chars[cf->data[i] & 0x0f];        // lower nibble
  }

  // terminate
  buffer[idx++] = ';';
  buffer[idx] = 0;
  // VLOG("CANtoGC: GC string = %s", buffer);

  return true;
}

//
/// convert from Gridconnect string to CAN frame
//

bool GCtoCAN(char buffer[GC_INP_SIZE], twai_message_t *cf) {

  unsigned long header = 0UL;
  uint8_t i, j;
  ssize_t slen = strlen(buffer);
  char hbuf[9];

  // VLOG("GCtoCAN: starting with string = %s", buffer);

  // zero the frame contents
  bzero((void *)cf, sizeof(twai_message_t));

  // check string begins with a colon
  if (buffer[0] != ':') {
    VLOG("GCtoCAN: invalid GC string |%s| does not begin with :", buffer);
    return false;
  }

  // next byte indicates standard or extended frame type
  switch (buffer[1]) {
    case 'S':
      break;
    case 'X':
      cf->flags |= TWAI_MSG_FLAG_EXTD;
      break;
    default:
      VLOG("GCtoCAN: invalid frame type = %c", buffer[1]);
      return false;
  }

  // find the N or R indicator
  for (i = 2; i < slen; i++) {
    if (buffer[i] == 'N' || buffer[i] == 'R') {
      // VLOG("GCtoCAN: found indicator %c at index = %d", buffer[i], i);
      break;
    }
  }

  // i now points to the N or R indicator

  if (i == slen) {
    LOG("GCtoCAN: string does not contain frame type N or R");
    return false;
  }

  if (i == 2) {
    LOG("GCtoCAN: string has no header part");
    return false;
  }

  // set the remote request flag
  if (buffer[i] == 'R') {
    cf->flags |= TWAI_MSG_FLAG_RTR;
  }

  // copy the frame header part - the variable number of bytes between :S/X and N/R
  // check for non-hex characters
  for (j = 2; j < i; j++) {
    if (!isxdigit(buffer[j])) {
      VLOG("GCtoCAN: error: non-hex digit '%c' in frame header at index = %d", buffer[j], j);
      return false;
    }

    hbuf[j - 2] = toupper(buffer[j]);
  }

  hbuf[j - 2] = 0;

  // VLOG("GCtoCAN: GC header id = %s, j = %d, i = %d, len = %d", hbuf, j, i, strlen(hbuf));

  // convert to 32 bit integer value
  sscanf(hbuf, "%lX", &header);

  // shift the header right per CBUS spec
  cf->identifier = header >> 5;

  // VLOG("GCtoCAN: id = %s, frame id = %ld, CAN id = %d", hbuf, cf->identifier, cf->identifier & 0x7f);

  // extract up to eight bytes of message data
  cf->data_length_code = sscanf(&buffer[i + 1], "%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
                                &cf->data[0], &cf->data[1], &cf->data[2], &cf->data[3],
                                &cf->data[4], &cf->data[5], &cf->data[6], &cf->data[7]);

  // VLOG("GCtoCAN: payload scan returns %d matches", cf->data_length_code);

  return true;
}

//
/// read and process data from an available source client
//

void process_input_data(byte i) {

  char c;
  ssize_t num_read = 0;
  twai_message_t cf;
  wrapped_gc_t gc;
  byte retries;

  //
  /// read a chunk of data into the input buffer
  /// NB serial input uses the client data structure [MAX_GC_CLIENTS + 1]
  //

  // LOG("gc_task: process_input_data");

  // network clients
  if (i < MAX_GC_CLIENTS) {
    retries = 0;

    // read a chunk of data into the buffer, retrying temp errors

    do {
      num_read = gc_clients[i].client->read((uint8_t *)gc_clients[i].input, GC_INP_SIZE);

      if (num_read < 0) {

        // retry if error is temporary
        if (errno == EAGAIN && retries < max_retries) {
          LOG("gc_task: process_input_data: retrying read");
          vTaskDelay(2);
          ++retries;
          continue;
        }

        VLOG("gc_task: process_input_data: read error from client = %d, errno = %d", i, errno);

        // clear buffer and bail out if any other error
        gc_clients[i].buffer[0] = 0;
        gc_clients[i].idx = 0;
        ++errors.gc_rx;
        PULSE_LED(ERR_IND_LED);
        return;
      }

    } while (num_read < 0);

  } else {

    // serial client
    while (Serial.available() && num_read < GC_INP_SIZE) {
      gc_clients[i].input[num_read] = Serial.read();
      num_read = (num_read + 1);
    }
  }

  //
  /// process input buffer, char by char, then dispatch completed messages
  //

  if (num_read > 0) {
    // VLOG("gc_task: read %d chars, input = %s", num_read, gc_clients[i].input);

    for (ssize_t j = 0; j < num_read; j++) {

      c = gc_clients[i].input[j];

      // VLOG("gc_task: processing char = %c", c);

      switch (c) {
        case ':':
          // VLOG("gc_task: start of new GC message from client = %d", i);
          gc_clients[i].idx = 0;
          gc_clients[i].buffer[gc_clients[i].idx++] = c;
          break;

        case ';':
          // we now have a complete GC message string
          gc_clients[i].buffer[gc_clients[i].idx] = c;
          gc_clients[i].buffer[gc_clients[i].idx + 1] = 0;
          gc_clients[i].idx = 0;

          // VLOG("gc_task: end of GC string due to semi-colon, complete buffer = %s", gc_clients[i].buffer);
          // VLOG("gc_task: input processing complete for client %d, GC string = %s", i, gc_clients[i].buffer);

          // convert GC string to CAN frame and place on output queues
          if (GCtoCAN(gc_clients[i].buffer, &cf)) {

            // VLOG("gc_task: GCtoCAN returns CANID = %d, opcode = 0x%02x", cf.identifier & 0x7f, cf.data[0]);

            uint16_t queues = QUEUE_CAN_OUT_FROM_GC | QUEUE_NET_OUT;

            if (!send_message_to_queues(queues, &cf, "gc_task", QUEUE_OP_TIMEOUT_SHORT)) {
              LOG("gc_task: process_input_data: error queuing message");
              PULSE_LED(ERR_IND_LED);
            }

            if (num_gc_clients > 1) {
              strcpy(gc.msg, gc_clients[i].buffer);
              strcpy(gc.addr, gc_clients[i].addr);
              gc.port = gc_clients[i].port;

              if (!send_message_to_queues(QUEUE_GC_TO_GC, &gc, "gc_task", QUEUE_OP_TIMEOUT_SHORT)) {
                LOG("gc_task: process_input_data: error queuing message");
                PULSE_LED(ERR_IND_LED);
              }
            }

            queues = QUEUE_WITHROTTLE_IN | QUEUE_CMDPROXY_IN | QUEUE_CBUS_EXTERNAL;

            if (!send_message_to_queues(queues, &cf, "gc_task", QUEUE_OP_TIMEOUT_NONE)) {
              LOG("gc_task: process_input_data: error queuing message");
              PULSE_LED(ERR_IND_LED);
            }

          } else {
            LOG("gc_task: process_input_data: GCtoCAN returns error");
            PULSE_LED(ERR_IND_LED);
          }

          PULSE_LED(NET_ACT_LED);  // once complete message has been despatched
          ++stats.gc_rx;
          break;

        case '\r':
        case '\n':
        case ' ':
          // ignore these
          break;

        default:
          // just store this char
          gc_clients[i].buffer[gc_clients[i].idx] = c;
          gc_clients[i].idx = (gc_clients[i].idx + 1) % GC_INP_SIZE;
          break;

      }  // switch c
    }  // for each char read
  } else {

    // should never be true, but ...
    VLOG("gc_task: process_input_data: unexpectedly read 0 bytes from client = %d", i);
  }

  return;
}

//
/// send GC message buffer to connected GC client, either network or serial
//

bool send_message_to_client(byte i, const char *buffer, const size_t s) {

  ssize_t b;
  byte retries = 0;
  bool ret = true;

  if (i == SERIAL_CLIENT && config_data.gc_serial_on) {
    // send to serial client if configured

    if (Serial.availableForWrite() >= s) {
      Serial.write((const uint8_t *)buffer, s);
    } else {
      LOG("gc_task: send_message_to_client: insufficient serial write buffer space");
      PULSE_LED(ERR_IND_LED);
      ret =  false;
    }

  } else {
    // send to network client
    do {
      b = gc_clients[i].client->write((const char *)buffer, s);

      if (b == -1) {
        if (errno == EAGAIN && retries < max_retries) {
          LOG("gc_task: send_message_to_client: retrying write to client");
          vTaskDelay(1);
          ++retries;
          continue;
        } else {
          VLOG("gc_task: send_message_to_client: error sending to client = %d, expected = %d, sent = %d, errno = %d %s", i, s, b, errno, strerror(errno));
          ++errors.gc_tx;
          PULSE_LED(ERR_IND_LED);
          ret = false;
        }
      }
    } while (b < 0);
  }

  return ret;
}
