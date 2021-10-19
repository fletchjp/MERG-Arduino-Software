
//
///
//

/*
  Copyright (C) Duncan Greenwood 2017 (duncan_greenwood@hotmail.com)

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

/*
      3rd party libraries needed for compilation: (not for binary-only distributions)

      Streaming   -- C++ stream style output, v5, (http://arduiniana.org/libraries/streaming/)
      ACAN2515    -- library to support the MCP2515/25625 CAN controller IC
*/

// 3rd party libraries
#include <Streaming.h>

// CBUS library header files
#include <CBUS2515.h>               // CAN controller and CBUS class
#ifndef ARDUINO_ARCH_RP2040
#include <CBUSswitch.h>             // pushbutton switch
#include <CBUSLED.h>                // CBUS LEDs
#endif
#include <CBUSconfig.h>             // module configuration
#include <CBUSParams.h>             // CBUS parameters
#include <cbusdefs.h>               // MERG CBUS constants

// constants
const byte VER_MAJ = 1;             // code major version
const char VER_MIN = 'a';           // code minor version
const byte VER_BETA = 0;            // code beta sub-version
const byte MODULE_ID = 97;          // CBUS module type

#ifdef ARDUINO_ARCH_RP2040
// Pico SPI pins
static const byte PICO_INT = 1;
static const byte PICO_SCK = 2;
static const byte PICO_MOSI = 3;
static const byte PICO_MISO = 4;
static const byte PICO_CS = 5;
#else
const byte LED_GRN = 4;             // CBUS green SLiM LED pin
const byte LED_YLW = 5;             // CBUS yellow FLiM LED pin
const byte SWITCH0 = 6;             // CBUS push button switch pin
#endif

// CBUS objects
CBUS2515 CBUS;                      // CBUS object
CBUSConfig config;                  // configuration object
#ifndef ARDUINO_ARCH_RP2040
CBUSLED ledGrn, ledYlw;             // two LED objects
CBUSSwitch pb_switch;               // switch object
#endif
CBUSLongMessage lmsg(&CBUS);        // CBUS RFC0005 long message object

// module name, must be 7 characters, space padded.
unsigned char mname[7] = { 'L', 'M', 'S', 'G', 'E', 'X', ' ' };

// forward function declarations
void eventhandler(byte index, byte opc);
void framehandler(CANFrame *msg);
void longmessagehandler(void *fragment, const unsigned int fragment_len, const byte stream_id, const byte status);

// long message variables
byte streams[] = {1, 2, 3};         // streams to subscribe to
char lmsg_out[32], lmsg_in[32];     // message buffers

//
/// setup CBUS - runs once at power on from setup()
//

void setupCBUS() {

  // set config layout parameters
  config.EE_NVS_START = 10;
  config.EE_NUM_NVS = 10;
  config.EE_EVENTS_START = 50;
  config.EE_MAX_EVENTS = 32;
  config.EE_NUM_EVS = 1;
  config.EE_BYTES_PER_EVENT = (config.EE_NUM_EVS + 4);

  // initialise and load configuration
  config.setEEPROMtype(EEPROM_INTERNAL);
  config.begin();

  Serial << F("> mode = ") << ((config.FLiM) ? "FLiM" : "SLiM") << F(", CANID = ") << config.CANID;
  Serial << F(", NN = ") << config.nodeNum << endl;

  // show code version and copyright notice
  printConfig();

  // set module parameters
  CBUSParams params(config);
  params.setVersion(VER_MAJ, VER_MIN, VER_BETA);
  params.setModuleId(MODULE_ID);
  params.setFlags(PF_FLiM | PF_COMBI);

  // assign to CBUS
  CBUS.setParams(params.getParams());
  CBUS.setName(mname);

#ifndef ARDUINO_ARCH_RP2040
  // set CBUS LED pins and assign to CBUS
  ledGrn.setPin(LED_GRN);
  ledYlw.setPin(LED_YLW);
  CBUS.setLEDs(ledGrn, ledYlw);

  // initialise CBUS switch and assign to CBUS
  pb_switch.setPin(SWITCH0, LOW);
  pb_switch.run();
  CBUS.setSwitch(pb_switch);

  // module reset - if switch is depressed at startup and module is in SLiM mode
  if (pb_switch.isPressed() && !config.FLiM) {
    Serial << F("> switch was pressed at startup in SLiM mode") << endl;
    config.resetModule(ledGrn, ledYlw, pb_switch);
  }
#endif

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);

  // register our CAN frame handler, to receive *every* CAN frame
  CBUS.setFrameHandler(framehandler);

  // subscribe to long message streams and register our handler function
  lmsg.subscribe(streams, sizeof(streams) / sizeof(byte), lmsg_in, 32, longmessagehandler);

#ifndef ARDUINO_ARCH_RP2040
  // set CBUS LEDs to indicate the current mode
  CBUS.indicateMode(config.FLiM);
#endif

  // configure and start CAN bus and CBUS message processing
  CBUS.setNumBuffers(4, 2);         // more buffers = more memory used, fewer = less
  CBUS.setOscFreq(16000000UL);      // select the crystal frequency of the CAN module
#ifdef ARDUINO_ARCH_RP2040
  // configure SPI pins
  SPI.setSCK(PICO_SCK);   // SCK
  SPI.setTX(PICO_MOSI);   // MOSI
  SPI.setRX(PICO_MISO);   // MISO
  SPI.setCS(PICO_CS);     // CS
  CBUS.setPins(PICO_CS, PICO_INT, PICO_MOSI, PICO_MISO, PICO_SCK);
#else
  CBUS.setPins(10, 2);              // select pins for CAN bus CE and interrupt connections
#endif
  if (!CBUS.begin()) {
    Serial << F("> error starting CBUS") << endl;
  }
}

//
/// setup - runs once at power on
//

void setup() {

  Serial.begin (115200);
  while(!Serial) { }
  Serial << endl << endl << F("> ** CBUS Arduino long message example module ** ") << __FILE__ << endl;

  setupCBUS();

  // end of setup
  Serial << F("> ready") << endl << endl;
}

//
/// loop - runs forever
//

void loop() {

  //
  /// do CBUS message, switch and LED processing
  //

  CBUS.process();

  //
  /// do RFC0005 CBUS long message processing
  //

  if (!lmsg.process()) {
    Serial << F("> error in long message processing") << endl;
  }

  //
  /// process console commands
  //

  processSerialInput();

  //
  /// check CAN message buffers
  //

  if (CBUS.canp->receiveBufferPeakCount() > CBUS.canp->receiveBufferSize()) {
    Serial << F("> receive buffer overflow") << endl;
  }

  if (CBUS.canp->transmitBufferPeakCount(0) > CBUS.canp->transmitBufferSize(0)) {
    Serial << F("> transmit buffer overflow") << endl;
  }

  //
  /// check CAN bus state
  //

  byte err;

  if ((err = CBUS.canp->errorFlagRegister()) != 0) {
    Serial << F("> error flag register = ") << err << endl;
  }

  //
  /// bottom of loop()
  //
}

//
/// user-defined event processing function
/// called from the CBUS library when a learned event is received
/// it receives the event table index and the CAN frame
//

void eventhandler(byte index, CANFrame *msg) {

  // as an example, display the opcode and the first EV of this event

  Serial << F("> event handler: index = ") << index << F(", opcode = 0x") << _HEX(msg->data[0]) << endl;
  Serial << F("> EV1 = ") << config.getEventEVval(index, 1) << endl;
  return;
}

//
/// user-defined frame processing function
/// called from the CBUS library for *every* CAN frame received
/// it receives a pointer to the received CAN frame
//

void framehandler(CANFrame *msg) {

  // as an example, format and display the received frame

  Serial << "[ " << (msg->id & 0x7f) << "] [" << msg->len << "] [";

  for (byte d = 0; d < msg->len; d++) {
    Serial << " 0x" << _HEX(msg->data[d]);
  }

  Serial << " ]" << endl;
  return;
}

//
/// long message receive handler function
/// called once the user buffer is full, or the message has been completely received
//

void longmessagehandler(void *fragment, const unsigned int fragment_len, const byte stream_id, const byte status) {

  // display the message
  // this will be the complete message if shorter than the provided buffer, or the final fragment if longer

  if (status == CBUS_LONG_MESSAGE_COMPLETE) {
    Serial << F("> received long message, stream = ") << stream_id << F(", len = ") << fragment_len << F(", msg = |") << (char *) fragment << F("|") << endl;
  }

  return;
}

//
/// print code version config details and copyright notice
//

void printConfig(void) {

  // code version
  Serial << F("> code version = ") << VER_MAJ << VER_MIN << F(" beta ") << VER_BETA << endl;
  Serial << F("> compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(", compiler ver = ") << __cplusplus << endl;

  // copyright
  Serial << F("> © Duncan Greenwood (MERG M5767) 2019") << endl;
  return;
}

//
/// command interpreter for serial console input
//

void processSerialInput(void) {

  byte uev = 0;
  char msgstr[32], dstr[32];

  if (Serial.available()) {

    char c = Serial.read();

    switch (c) {

    case 'n':

      // node config
      printConfig();

      // node identity
      Serial << F("> CBUS node configuration") << endl;
      Serial << F("> mode = ") << (config.FLiM ? "FLiM" : "SLiM") << F(", CANID = ") << config.CANID << F(", node number = ") << config.nodeNum << endl;
      Serial << endl;
      break;

    case 'e':

      // EEPROM learned event data table
      Serial << F("> stored events ") << endl;
      Serial << F("  max events = ") << config.EE_MAX_EVENTS << F(" EVs per event = ") << config.EE_NUM_EVS << F(" bytes per event = ") << config.EE_BYTES_PER_EVENT << endl;

      for (byte j = 0; j < config.EE_MAX_EVENTS; j++) {
        if (config.getEvTableEntry(j) != 0) {
          ++uev;
        }
      }

      Serial << F("  stored events = ") << uev << F(", free = ") << (config.EE_MAX_EVENTS - uev) << endl;
      Serial << F("  using ") << (uev * config.EE_BYTES_PER_EVENT) << F(" of ") << (config.EE_MAX_EVENTS * config.EE_BYTES_PER_EVENT) << F(" bytes") << endl << endl;

      Serial << F("  Ev#  |  NNhi |  NNlo |  ENhi |  ENlo | ");

      for (byte j = 0; j < (config.EE_NUM_EVS); j++) {
        sprintf(dstr, "EV%03d | ", j + 1);
        Serial << dstr;
      }

      Serial << F("Hash |") << endl;

      Serial << F(" --------------------------------------------------------------") << endl;

      // for each event data line
      for (byte j = 0; j < config.EE_MAX_EVENTS; j++) {

        if (config.getEvTableEntry(j) != 0) {
          sprintf(dstr, "  %03d  | ", j);
          Serial << dstr;

          // for each data byte of this event
          for (byte e = 0; e < (config.EE_NUM_EVS + 4); e++) {
            sprintf(dstr, " 0x%02hx | ", config.readEEPROM(config.EE_EVENTS_START + (j * config.EE_BYTES_PER_EVENT) + e));
            Serial << dstr;
          }

          sprintf(dstr, "%4d |", config.getEvTableEntry(j));
          Serial << dstr << endl;
        }
      }

      Serial << endl;

      break;

    // NVs
    case 'v':

      // note NVs number from 1, not 0
      Serial << "> Node variables" << endl;
      Serial << F("   NV   Val") << endl;
      Serial << F("  --------------------") << endl;

      for (byte j = 1; j <= config.EE_NUM_NVS; j++) {
        byte v = config.readNV(j);
        sprintf(msgstr, " - %02d : %3hd | 0x%02hx", j, v, v);
        Serial << msgstr << endl;
      }

      Serial << endl << endl;

      break;

    // CAN bus status
    case 'c':

      CBUS.printStatus();
      break;

    case 'h':
      // event hash table
      config.printEvHashTable(false);
      break;

    case 'y':
      // reset CAN bus and CBUS message processing
      CBUS.reset();
      break;

    case '*':
      // reboot
      config.reboot();
      break;

    case 'm':
      // free memory
      Serial << F("> free SRAM = ") << config.freeSRAM() << F(" bytes") << endl;
      break;

    case 'l':
      // send a long message with stream ID = 2
      uev = Serial.readBytesUntil('\n', lmsg_out, 32);
      lmsg.sendLongMessage(lmsg_out, uev, 2);
      break;

    case 'd':
      Serial << F("> tx buffer = ") << CBUS.canp->transmitBufferSize(0) << F(", ") <<  CBUS.canp->transmitBufferCount(0) << F(", ") << CBUS.canp->transmitBufferPeakCount(0) << endl;
      Serial << F("> rx buffer = ") << CBUS.canp->receiveBufferSize() << F(", ") << CBUS.canp->receiveBufferCount() << F(", ") << CBUS.canp->receiveBufferPeakCount() << endl;
      break;

    case '\r':
    case '\n':
      Serial << endl;
      break;

    default:
      // Serial << F("> unknown command ") << c << endl;
      break;
    }
  }
}
