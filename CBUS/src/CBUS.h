
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

#pragma once

#include <SPI.h>

#include <CBUSLED.h>
#include <CBUSswitch.h>
#include <CBUSconfig.h>
#include <cbusdefs.h>

#define SW_TR_HOLD 6000U                   // CBUS push button hold time for SLiM/FLiM transition in millis = 6 seconds
#define DEFAULT_PRIORITY 0xB               // default CBUS messages priority. 1011 = 2|3 = normal/low
#define LONG_MESSAGE_DEFAULT_DELAY 20      // delay in milliseconds between sending successive long message fragments
#define LONG_MESSAGE_RECEIVE_TIMEOUT 5000  // timeout waiting for next long message packet
#define OPC_LMSG 0xe9                      // temp opcode for CBUS long message

//
/// CBUS modes
//

enum {
  MODE_SLIM = 0,
  MODE_FLIM = 1,
  MODE_CHANGING = 2
};

//
/// CBUS long message status codes
//

enum {
  CBUS_LONG_MESSAGE_INCOMPLETE = 0,
  CBUS_LONG_MESSAGE_COMPLETE,
  CBUS_LONG_MESSAGE_SEQUENCE_ERROR,
  CBUS_LONG_MESSAGE_TIMEOUT_ERROR,
  CBUS_LONG_MESSAGE_CRC_ERROR
};

//
/// CAN/CBUS message type
//

class CANFrame {

public:
  uint32_t id;
  bool ext;
  bool rtr;
  uint8_t len;
  uint8_t data[8] = {};
};

//
/// an abstract class to encapsulate CAN bus and CBUS processing
/// it must be implemented by a derived subclass
//

class CBUSLongMessage;      // forward reference

class CBUSbase {

public:

  // these methods are pure virtual and must be implemented by the derived class
  // as a consequence, it is not possible to create an instance of this class

  virtual bool begin(bool poll = false, SPIClass spi = SPI) = 0;
  virtual bool available(void) = 0;
  virtual CANFrame getNextMessage(void) = 0;
  virtual bool sendMessage(CANFrame *msg, bool rtr = false, bool ext = false, byte priority = DEFAULT_PRIORITY) = 0;
  virtual void reset(void) = 0;

  // implementations of these methods are provided in the base class

  bool sendWRACK(void);
  bool sendCMDERR(byte cerrno);
  void CANenumeration(void);
  byte getCANID(unsigned long header);
  bool isExt(CANFrame *msg);
  bool isRTR(CANFrame *msg);
  void process(byte num_messages = 3);
  void initFLiM(void);
  void revertSLiM(void);
  void setSLiM(void);
  void renegotiate(void);
  void setLEDs(CBUSLED ledGrn, CBUSLED ledYlw);
  void setSwitch(CBUSSwitch sw);
  void setParams(unsigned char *mparams);
  void setName(unsigned char *mname);
  void checkCANenum(void);
  void indicateMode(byte mode);
  void setEventHandler(void (*fptr)(byte index, CANFrame *msg));
  void setEventHandler(void (*fptr)(byte index, CANFrame *msg, bool ison, byte evval));
  void setFrameHandler(void (*fptr)(CANFrame *msg), byte *opcodes = NULL, byte num_opcodes = 0);
  void makeHeader(CANFrame *msg, byte priority = DEFAULT_PRIORITY);
  void processAccessoryEvent(unsigned int nn, unsigned int en, bool is_on_event);

  void setLongMessageHandler(CBUSLongMessage *handler);

protected:                                          // protected members become private in derived classes
  CANFrame _msg;
  unsigned int _numMsgsSent, _numMsgsRcvd;
  CBUSLED _ledGrn, _ledYlw;
  CBUSSwitch _sw;
  unsigned char *_mparams;
  unsigned char *_mname;
  void (*eventhandler)(byte index, CANFrame *msg);
  void (*eventhandlerex)(byte index, CANFrame *msg, bool evOn, byte evVal);
  void (*framehandler)(CANFrame *msg);
  byte *_opcodes;
  byte _num_opcodes;
  byte enum_responses[16];                          // 128 bits for storing CAN ID enumeration results
  bool bModeChanging, bCANenum, bLearn;
  unsigned long timeOutTimer, CANenumTime;
  bool enumeration_required;
  bool UI = false;

  CBUSLongMessage *longMessageHandler = NULL;       // CBUS long message object to receive relevant frames
};

//
/// a class to send and receive CBUS long messages per MERG RFC 0005
//

class CBUSLongMessage {

public:

  CBUSLongMessage(CBUSbase *cbus_object_ptr);
  bool sendLongMessage(const void *msg, const unsigned int msg_len, const byte stream_id, const byte priority = DEFAULT_PRIORITY);
#ifdef CBUS_LONG_MESSAGE_MULTIPLE_LISTEN
  void subscribe(byte *stream_ids, const byte num_stream_ids,bool *receiving, 
     void *receive_buffer, const unsigned int receive_buff_len, void (*messagehandler)(void *msg, unsigned int msg_len, byte stream_id, byte status));
#else
  void subscribe(byte *stream_ids, const byte num_stream_ids, void *receive_buffer, const unsigned int receive_buffer_len, void (*messagehandler)(void *fragment, const unsigned int fragment_len, const byte stream_id, const byte status));
#endif
  bool process(void);
  void processReceivedMessageFragment(const CANFrame *frame);
  bool is_sending(void);
  void setDelay(byte delay_in_millis);
  void setTimeout(unsigned int timeout_in_millis);

private:

  bool sendMessageFragment(CANFrame *frame, const byte priority);

#ifdef CBUS_LONG_MESSAGE_MULTIPLE_LISTEN
  bool *_receiving;
  bool is_receiving(byte id); // reverse lookup for receiving given a stream id
  byte index(byte id);        // reverse lookup for index given a stream id
#endif
  bool _is_receiving;
  byte *_send_buffer, *_receive_buffer;
  byte _send_stream_id, _receive_stream_id, *_stream_ids, _num_stream_ids, _send_priority;
  byte _msg_delay;
  unsigned int _send_buffer_len, _incoming_message_length, _receive_buff_len, _receive_buffer_index, _send_buffer_index, _incoming_message_crc, \
  _incoming_bytes_received, _receive_timeout, _send_sequence_num, _expected_next_receive_sequence_num;
  unsigned long _last_fragment_sent, _last_fragment_received;

  void (*_messagehandler)(void *fragment, const unsigned int fragment_len, const byte stream_id, const byte status);        // user callback function to receive long message fragments
  CBUSbase *_cbus_object_ptr;
};
