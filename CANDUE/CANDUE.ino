/////////////////////////////////////////////////////////////////////////////
// CANDUE 
// Version 1a beta 1 Initial operational test
// Version 1a beta 2 Add some more code.
// Version 1a beta 3 Add code for events and take CBUS button/LEDs out of use.
// Version 1a beta 4 Add changes to code suggested by Sven Rosvall for CANmINnOUT.
//                   and eventhandler code for LEDs which was missing.
// Version 1a beta 5 Add missing return value from sendEvent
// Version 1a beta 6 Change to use DEBUG_PRINT and add failure message from processSwitches
// Version 1a beta 7 Change processSerialInput into a task
///////////////////////////////////////////////////////////////////////////////////
// Version 2a beta 1 Bring in code for long messages from CANTEXTL and CANTOTEM.
#define CBUS_LONG_MESSAGE
///////////////////////////////////////////////////////////////////////////////////
// My working name for changes to the example from Duncan.
// Note that the library DueFlashStorage is accessed from CBUSconfig
// Note that the previously reported problem with the Streaming library has been fixed.
// Thoughts. I want to add some more events to explore working e.g. with a display.
//           I am waiting to have the CBUS library updated so that I can use long messages.
//           There are two CAN interfaces and I could explore using both of them.
// John Fletcher

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

///////////////////////////////////////////////////////////////////////////////////
// Pin Use map DUE:
// Note that the pin layout is similar to an Arduino MEGA.
// There are a lot of other uses defined on a full pinout diagram.
// Pins used by the CAN attachment are documented.
// A lot more pins are inaccessible when it is in place.
/*
 Digital pin 0 RX0            
 Digital pin 1 TX0
 Digital pin 2 (PWM)               
 Digital pin 3 (PWM)
 Digital pin 4 (PWM)
 Digital pin 5 (PWM)
 Digital pin 6 (PWM)
 Digital pin 7 (PWM)
 
 Digital pin 8 (PWM)
 Digital pin 9 (PWM)
 Digital pin 10 (PWM)
 Digital pin 11 (PWM)
 Digital pin 12 (PWM)
 Digital pin 13 (PWM)
 
 Digital pin 14 (TX3) | CAN attachment
 Digital pin 15 (RX3) | CAN attachment
 Digital pin 16 (TX2)
 Digital pin 17 (RX2)
 Digital pin 18 (TX1)
 Digital pin 19 (RX1)
 Digital pin 20 (SDA) | CAN attachment
 Digital pin 21 (SCL) | CAN attachment
 
 Digital pin 22 | CAN attachment
 Digital pin 23 | CAN attachment
 Digital pin 24
 Digital pin 25
 Digital pin 26
 Digital pin 27
 Digital pin 28
 Digital pin 29
 Digital pin 30
 Digital pin 31
 Digital pin 32
 Digital pin 33
 Digital pin 34 (PWM 34 - 45)
 Digital pin 35 |
 Digital pin 36 |
 Digital pin 37 |
 Digital pin 38 |
 Digital pin 39 |
 Digital pin 40 |
 Digital pin 41 |
 Digital pin 42 |
 Digital pin 43 |
 Digital pin 44 |
 Digital pin 45 (PWM)
 Digital pin 46 
 Digital pin 47 
 Digital pin 48 
 Digital pin 49
 Digital pin 50
 Digital pin 51
 Digital pin 52 | CAN attachment
 Digital pin 53 | CAN attachment
 
 Digital pin 54 / Analog pin 0
 Digital pin 55 / Analog pin 1
 Digital pin 56 / Analog pin 2
 Digital pin 57 / Analog pin 3
 Digital pin 58 / Analog pin 4
 Digital pin 59 / Analog pin 5
 Digital pin 60 / Analog pin 6
 Digital pin 61 / Analog pin 7
 
 Digital pin 62 / Analog pin 8  | These pins are all
 Digital pin 63 / Analog pin 9  | used by the CAN attachment.
 Digital pin 64 / Analog pin 10 |
 Digital pin 65 / Analog pin 11 |
 Digital pin 66 / DAC0          |
 Digital pin 67 / DAC1          |
 Digital pin 68 / CANRX         |
 Digital pin 69 / CANTX         |
*/

/////////////////////////////////////////////////////////////////////////////////// 

#define DEBUG 1       // set to 0 for no serial debug

#if DEBUG
#define DEBUG_PRINT(S) Serial << S << endl
#else
#define DEBUG_PRINT(S)
#endif

// IoAbstraction libraries
#include <IoAbstraction.h>
#include <AnalogDeviceAbstraction.h>
#include <TaskManagerIO.h>
#include <DeviceEvents.h>

// 3rd party libraries
#include <Streaming.h>
#include <Bounce2.h>

// CBUS library header files
#include <CBUSSAM3X8E.h>            // CAN controller and CBUS class
#include <CBUSswitch.h>             // pushbutton switch
#include "LEDControl.h"          // CBUS LEDs
//#include <CBUSLED.h>                // CBUS LEDs
#include <CBUSconfig.h>             // module configuration
#include <CBUSParams.h>             // CBUS parameters
#include <cbusdefs.h>               // MERG CBUS constants

// constants
const byte VER_MAJ = 2;                  // code major version
const char VER_MIN = 'a';                // code minor version
const byte VER_BETA = 1;                 // code beta sub-version
const byte MODULE_ID = 99;               // CBUS module type

// These are not being used - not installed.
//const byte LED_GRN = 4;                  // CBUS green SLiM LED pin
//const byte LED_YLW = 5;                  // CBUS yellow FLiM LED pin
//const byte SWITCH0 = 6;                  // CBUS push button switch pin

#define NUM_LEDS 2              // How many LEDs are there?
#define NUM_SWITCHES 2          // How many switchs are there?

//Module pins available for use are Pins 3 - 9 and A0 - A5
const byte LED[NUM_LEDS] = {8, 7};            // LED pin connections through typ. 1K8 resistor
const byte SWITCH[NUM_SWITCHES] = {9, 6};     // Module Switch takes input to 0V.

// module objects
Bounce moduleSwitch[NUM_SWITCHES];  //  switch as input
LEDControl moduleLED[NUM_LEDS];     //  LED as output
byte switchState[NUM_SWITCHES];

//////////////////////////////////////////////////////////////////////////

// CBUS objects
CBUSSAM3X8E CBUS;                   // CBUS object
CBUSConfig config;                  // configuration object
//CBUSLED ledGrn, ledYlw;             // two LED objects
//CBUSSwitch pb_switch;               // switch object
#ifdef CBUS_LONG_MESSAGE
// The Ardunio CBUS library does now support this.
// create an additional object at the top of the sketch:
CBUSLongMessage cbus_long_message(&CBUS);   // CBUS long message object
#endif

// module name, must be 7 characters, space padded.
unsigned char mname[7] = { 'D', 'U', 'E', ' ', ' ', ' ', ' ' };

// forward function declarations
void eventhandler(byte index, byte opc);
void framehandler(CANFrame *msg);

#ifdef CBUS_LONG_MESSAGE
///////////////////////////////////////////////////////////////////////////////////////////////
// Long message setting up.
///////////////////////////////////////////////////////////////////////////////////////////////
 // a list of stream IDs to subscribe to (this ID is defined by the sender):
byte stream_ids[] = {1, 2, 3}; // These are the ones which this module will read.
 // a buffer for the message fragments to be assembled into
// either sized to the maximum message length, or as much as you can afford
const unsigned int buffer_size = 128;
byte long_message_data[buffer_size];
 // create a handler function to receive completed long messages:
void longmessagehandler(byte *fragment, unsigned int fragment_len, byte stream_id, byte status);
const byte delay_in_ms_between_messages = 50;
#endif

//
/// setup CBUS - runs once at power on from setup()
//
void setupCBUS()
{
  // set config layout parameters
  config.EE_NVS_START = 10;
  config.EE_NUM_NVS = 10;
  config.EE_EVENTS_START = 50;
  config.EE_MAX_EVENTS = 64;
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

  // set CBUS LED pins and assign to CBUS
  //ledGrn.setPin(LED_GRN);
  //ledYlw.setPin(LED_YLW);
  //CBUS.setLEDs(ledGrn, ledYlw);

  // initialise CBUS switch and assign to CBUS
  //pb_switch.setPin(SWITCH0, LOW);
  //pb_switch.run();
  //CBUS.setSwitch(pb_switch);

  // module reset - if switch is depressed at startup and module is in SLiM mode
  //if (pb_switch.isPressed() && !config.FLiM) {
  //  Serial << F("> switch was pressed at startup in SLiM mode") << endl;
  //  config.resetModule(ledGrn, ledYlw, pb_switch);
  //}

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);

  // register our CAN frame handler, to receive *every* CAN frame
  CBUS.setFrameHandler(framehandler);

#ifdef CBUS_LONG_MESSAGE
  // subscribe to long messages and register handler
  cbus_long_message.subscribe(stream_ids, (sizeof(stream_ids) / sizeof(byte)), long_message_data, buffer_size, longmessagehandler);
  // this method throttles the transmission so that it doesn't overwhelm the bus:
  cbus_long_message.setDelay(delay_in_ms_between_messages);
  cbus_long_message.setTimeout(1000);
#endif

  // set CBUS LEDs to indicate the current mode
  CBUS.indicateMode(config.FLiM);

  // configure and start CAN bus and CBUS message processing
  // CBUS.setNumBuffers(2);         // more buffers = more memory used, fewer = less
  // CBUS.setOscFreq(16000000UL);   // select the crystal frequency of the CAN module
  // CBUS.setPins(10, 2);           // select pins for CAN bus CE and interrupt connections
  CBUS.setControllerInstance(0);    // only actually required for instance 1, instance 0 is the default
  CBUS.begin();
}

void runLEDs(){
  // Run the LED code
  for (int i = 0; i < NUM_LEDS; i++) {
    moduleLED[i].run();
  }
}

void setupModule()
{
  // configure the module switches, active low
  for (int i = 0; i < NUM_SWITCHES; i++)
  {
    moduleSwitch[i].attach(SWITCH[i], INPUT_PULLUP);
    moduleSwitch[i].interval(5);
    switchState[i] = false;
  }

  // configure the module LEDs
  for (int i = 0; i < NUM_LEDS; i++) {
    moduleLED[i].setPin(LED[i]);
  } 
}

//
/// setup - runs once at power on
//

void setup()
{
  Serial.begin (115200);
  Serial << endl << endl << F("> CANDUE ** ") << __FILE__ << endl;

  setupCBUS();
  setupModule();

  // Schedule tasks to run every 250 milliseconds.
  taskManager.scheduleFixedRate(250, runLEDs);
  taskManager.scheduleFixedRate(250, processSwitches);
  taskManager.scheduleFixedRate(250, processSerialInput);

  // end of setup
  DEBUG_PRINT(F("> ready"));
}

//
/// loop - runs forever
//

void loop() {

  //
  /// do CBUS message, switch and LED processing
  //

  CBUS.process();

#ifdef CBUS_LONG_MESSAGE
  cbus_long_message.process();
#endif

  //
  /// process console commands is now a task
  //
  //  processSerialInput();

  // Run IO_Abstraction tasks.
  // This replaces actions taken here in the previous version.
  taskManager.runLoop();

  //
  /// bottom of loop()
  //
}

void processSwitches(void)
{
  bool is_success = true;
  for (int i = 0; i < NUM_SWITCHES; i++)
  {
    moduleSwitch[i].update();
    if (moduleSwitch[i].changed())
    {
     byte nv = i + 1;
     byte nvval = config.readNV(nv);
     byte opCode;

     DEBUG_PRINT(F("> Button ") << i << F(" state change detected"));
     Serial << F (" NV = ") << nv << F(" NV Value = ") << nvval << endl;

     switch (nvval)
     {
      case 0:
          opCode = (moduleSwitch[i].fell() ? OPC_ACON : OPC_ACOF);
          DEBUG_PRINT(F("> Button ") << i
              << (moduleSwitch[i].fell() ? F(" pressed, send 0x") : F(" released, send 0x")) << _HEX(opCode));
          is_success = sendEvent(opCode, (i + 1));
          break;

      case 1:
          if (moduleSwitch[i].fell())
          {
            opCode = OPC_ACON;
            DEBUG_PRINT(F("> Button ") << i << F(" pressed, send 0x") << _HEX(OPC_ACON));
            is_success = sendEvent(opCode, (i + 1));
          }
          break;

      case 2:

          if (moduleSwitch[i].fell())
          {
            opCode = OPC_ACOF;
            DEBUG_PRINT(F("> Button ") << i << F(" pressed, send 0x") << _HEX(OPC_ACOF));
            is_success = sendEvent(opCode, (i + 1));
          }
          break;

      case 3:

          if (moduleSwitch[i].fell())
          {
            switchState[i] = !switchState[i];
            opCode = (switchState[i] ? OPC_ACON : OPC_ACOF);
            DEBUG_PRINT(F("> Button ") << i
                << (moduleSwitch[i].fell() ? F(" pressed, send 0x") : F(" released, send 0x")) << _HEX(opCode));
            is_success = sendEvent(opCode, (i + 1));
          }

          break;

        /*
        // Send event to test display on CAN1602BUT.
      case 99:
        opCode = (switchState[i] ? OPC_ACON : OPC_ACOF);
        sendEvent(opCode,testEvent); // Test of new code.

        break; */
    default:
        DEBUG_PRINT(F("> Invalid NV value."));
        break;
     }
    }
  } 
  if (!is_success) 
  {
    DEBUG_PRINT(F("> One of the send message events failed"));
  }
}

// Send an event routine according to Module Switch
bool sendEvent(byte opCode, unsigned int eventNo)
{
  CANFrame msg;
  msg.id = config.CANID;
  msg.len = 5;
  msg.data[0] = opCode;
  msg.data[1] = highByte(config.nodeNum);
  msg.data[2] = lowByte(config.nodeNum);
  msg.data[3] = highByte(eventNo);
  msg.data[4] = lowByte(eventNo);

  bool success = CBUS.sendMessage(&msg);
  if (success) {
    DEBUG_PRINT(F("> sent CBUS message with Event Number ") << eventNo);
  } else {
    DEBUG_PRINT(F("> error sending CBUS message"));
  }
  return success;
}

#ifdef CBUS_LONG_MESSAGE
// Example code not yet being used.
void send_a_long_message() {
   char msg[16];
   byte stream_id = 1;
// Somewhere to send the long message.
   while(cbus_long_message.is_sending()) { } //wait for previous message to finish.
// bool cbus_long_message.sendLongMessage(char *msg, const unsigned int msg_len, 
//                        const byte stream_id, const byte priority = DEFAULT_PRIORITY);
    strcpy(msg, "Hello world!");
    if (cbus_long_message.sendLongMessage((const byte *)msg, strlen(msg), stream_id) ) {
      Serial << F("long message ") << msg << F(" sent to ") << stream_id << endl;
    }

}
#endif

//
/// user-defined event processing function
/// called from the CBUS library when a learned event is received
/// it receives the event table index and the CAN frame
//

void eventhandler(byte index, CANFrame *msg) 
{
  byte opc = msg->data[0];

  // as an example, display the opcode and the first EV of this event
  DEBUG_PRINT(F("> event handler: index = ") << index << F(", opcode = 0x") << _HEX(msg->data[0]));
  DEBUG_PRINT(F("> event handler: length = ") << msg->len);
  
  //Serial << F("> EV1 = ") << config.getEventEVval(index, 1) << endl;
  unsigned int node_number = (msg->data[1] << 8 ) + msg->data[2];
  unsigned int event_number = (msg->data[3] << 8 ) + msg->data[4];
  DEBUG_PRINT(F("> NN = ") << node_number << F(", EN = ") << event_number);
  DEBUG_PRINT(F("> op_code = ") << opc);
 
  switch (opc) {

    case OPC_ACON:
    case OPC_ASON:
      for (int i = 0; i < NUM_LEDS; i++)
      {
        byte ev = i + 1;
        byte evval = config.getEventEVval(index, ev);

        switch (evval)
        {
          case 1:
            moduleLED[i].on();
            break;

          case 2:
            moduleLED[i].flash(500);
            break;

          case 3:
            moduleLED[i].flash(250);
            break;

          default:
            break;
        }
      }
      break;

    case OPC_ACOF:
    case OPC_ASOF:
      for (int i = 0; i < NUM_LEDS; i++)
      {
        byte ev = i + 1;
        byte evval = config.getEventEVval(index, ev);

        if (evval > 0) {
          moduleLED[i].off();
        }
      }
      break;
  }

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

#ifdef CBUS_LONG_MESSAGE
   byte new_message = true;
//
// Handler to receive a long message 
// 
void longmessagehandler(byte *fragment, unsigned int fragment_len, byte stream_id, byte status){
// I need an example for what goes in here.
     fragment[fragment_len] = 0;
// If the message is complete it will be in fragment and I can do something with it.
     if( new_message) { // Print this only for the start of a message.
        Serial << F("> user long message handler: stream = ") << stream_id << F(", fragment length = ") 
               << fragment_len << F(", fragment = |");
        new_message = false;
     }
     if ( CBUS_LONG_MESSAGE_INCOMPLETE ) {
     // handle incomplete message
        Serial.write(fragment, fragment_len);
     } else if (CBUS_LONG_MESSAGE_COMPLETE) {
     // handle complete message
        Serial.write(fragment, fragment_len);
        Serial << F("|, status = ") << status << endl;
        new_message = true;  // reset for the next message
     } else {  // CBUS_LONG_MESSAGE_SEQUENCE_ERROR
               // CBUS_LONG_MESSAGE_TIMEOUT_ERROR,
               // CBUS_LONG_MESSAGE_CRC_ERROR
               // raise an error?
        Serial << F("| Message error with  status = ") << status << endl;
        new_message = true;  // reset for the next message
     } 
 }
  
#endif

//
/// print code version config details and copyright notice
//

void printConfig(void) {

  // code version
  Serial << F("> code version = ") << VER_MAJ << VER_MIN << F(" beta ") << VER_BETA << endl;
  Serial << F("> compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(", compiler ver = ") << __cplusplus << endl;

  // copyright
  Serial << F("> © Duncan Greenwood (MERG M5767) 2019") << endl;
  Serial << F("> © Martin Da Costa (MERG M6223) 2021") << endl;
  Serial << F("> © John Fletcher (MERG M6777) 2021") << endl;
  Serial << F("> © Sven Rosvall (MERG M3777) 2021") << endl;
#ifdef CBUS_LONG_MESSAGE
  Serial << F("> CBUS Long message handling available") << endl;
#endif
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

      case 'r':
        // renegotiate
        CBUS.renegotiate();
        break;

      case 'z':
        // Reset module, clear EEPROM
        static bool ResetRq = false;
        static unsigned long ResWaitTime;
        if (!ResetRq) {
          // start timeout timer
          Serial << F(">Reset & EEPROM wipe requested. Press 'z' again within 2 secs to confirm") << endl;
          ResWaitTime = millis();
          ResetRq = true;
        }
        else {
          // This is a confirmed request
          // 2 sec timeout
          if (ResetRq && ((millis() - ResWaitTime) > 2000)) {
            Serial << F(">timeout expired, reset not performed") << endl;
            ResetRq = false;
          }
          else {
            //Request confirmed within timeout
            Serial << F(">RESETTING AND WIPING EEPROM") << endl;
            config.resetModule();
            ResetRq = false;
          }
        }
        break;

      case '\r':
      case '\n':
        Serial << endl;
        break;

      default:
        Serial << F("> unknown command ") << c << endl;
        break;
    }
  }
}
