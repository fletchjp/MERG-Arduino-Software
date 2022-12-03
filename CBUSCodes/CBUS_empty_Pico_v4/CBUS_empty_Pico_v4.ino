// CBUS_empty_Pico_v4
// A copy of CBUS_empty_Pico_v3 to explore the problems.
// Modifications for the RS485 CAN Hat from the Arduino IDE.
// This now has external EEPROM
// It uses Wire1 for the I2C as Wire does not work.
// Crashing out is now much reduced.
// DO NOT USE while(!Serial) in an RP2040!!!
///
//
/// This activates the long message code.
//#define CBUS_LONG_MESSAGE
#define DEBUG       0   // set to 0 for no serial debug
#if DEBUG
#define DEBUG_PRINT(S) Serial << S << endl
#else
#define DEBUG_PRINT(S)
#endif

#define USE_EXTERNAL_EEPROM
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

// Arduino core libraries
#include <Wire.h>
#include <SPI.h>

// 3rd party libraries
#include <Streaming.h>

// CBUS library header files
#include <CBUS2515.h>               // CAN controller and CBUS class
#include <CBUSswitch.h>             // pushbutton switch
#include <CBUSLED.h>                // CBUS LEDs
#include <CBUSconfig.h>             // module configuration
#include <CBUSParams.h>             // CBUS parameters
#include <cbusdefs.h>               // MERG CBUS constants

// constants
const byte VER_MAJ = 1;                  // code major version
const char VER_MIN = 'a';                // code minor version
const byte VER_BETA = 0;                 // code beta sub-version
const byte MODULE_ID = 99;               // CBUS module type

// pins for Pico CAN shield
static const byte CAN_INT = 22;          // INT output of MCP2515
static const byte MCP2515_SCK  = 2;      // SCK input of MCP2515
static const byte MCP2515_MOSI = 3;      // SDI input of MCP2515
static const byte MCP2515_MISO = 4;      // SDO output of MCP2515
static const byte CAN_CS  = 5;           // CS input of MCP2515

#define WIRE Wire1
static const byte WIRE_SDA = 6;
static const byte WIRE_SCL = 7;

const byte LED_YLW = 20;                 // CBUS yellow FLiM LED pin
const byte LED_GRN = 21;                 // CBUS green SLiM LED pin
const byte SWITCH0 = 22;                 // CBUS push button switch pin

// CBUS objects
CBUSConfig module_config;           // configuration object
CBUS2515 CBUS(&module_config);      // CBUS object
CBUSLED ledGrn, ledYlw;             // two LED objects
CBUSSwitch pb_switch;               // switch object
#ifdef CBUS_LONG_MESSAGE
CBUSLongMessageEx lmsg(&CBUS);      // long message object
#endif

// module name, must be 7 characters, space padded.
unsigned char mname[7] = { 'R', 'P', 'I', 'P', 'I', 'C', 'O' };
#ifdef CBUS_LONG_MESSAGE
byte streamids[] = { 1, 2, 3};
#endif

// forward function declarations
void eventhandler(byte index, byte opc);
void framehandler(CANFrame *msg);
#ifdef CBUS_LONG_MESSAGE
void longmessagehandler(void *msg, unsigned int msg_len, byte stream_id, byte status);
#endif

//
/// setup CBUS - runs once at power on from setup()
//
void setupCBUS() {

  Serial << "Starting setupCBUS" << endl;
  // set config layout parameters
  module_config.EE_NVS_START = 10;
  module_config.EE_NUM_NVS = 10;
  module_config.EE_EVENTS_START = 50; 
  module_config.EE_MAX_EVENTS = 64;
  module_config.EE_NUM_EVS = 1;
  module_config.EE_BYTES_PER_EVENT = (module_config.EE_NUM_EVS + 4);
  
  // initialise and load configuration
#ifdef USE_EXTERNAL_EEPROM
  module_config.setExtEEPROMAddress(0x50, &WIRE);
#ifdef ARDUINO_ARCH_RP2040
  module_config.setExtEEPROMPins(WIRE_SDA, WIRE_SCL);
#endif
  module_config.setEEPROMtype(EEPROM_EXTERNAL);
#else
  module_config.setEEPROMtype(EEPROM_INTERNAL);
#endif

  module_config.begin();

  Serial << F("> mode = ") << ((module_config.FLiM) ? "FLiM" : "SLiM") << F(", CANID = ") << module_config.CANID;
  Serial << F(", NN = ") << module_config.nodeNum << endl;

  // show code version and copyright notice
  printConfig();

  // set module parameters
  CBUSParams params(module_config);
  params.setVersion(VER_MAJ, VER_MIN, VER_BETA);
  params.setModuleId(103);
  params.setFlags(PF_FLiM | PF_COMBI);
#ifdef USE_EXTERNAL_EEPROM
  module_config.writeBytesEEPROM(module_config.getEEPROMsize()+1,params.getParams(),params.size());
#endif
  // assign to CBUS
  CBUS.setParams(params.getParams());
  CBUS.setName(mname);

  // set CBUS LED pins and assign to CBUS
  ledGrn.setPin(LED_GRN);
  ledYlw.setPin(LED_YLW);
  CBUS.setLEDs(ledGrn, ledYlw);

  // initialise CBUS switch and assign to CBUS
  pb_switch.setPin(SWITCH0, LOW);
  pb_switch.run();
  CBUS.setSwitch(pb_switch);

  // module reset - if switch is depressed at startup and module is in SLiM mode
  if (pb_switch.isPressed() && !module_config.FLiM) {
    Serial << F("> switch was pressed at startup in SLiM mode") << endl;
    module_config.resetModule(ledGrn, ledYlw, pb_switch);
  }

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);

  // register our CAN frame handler, to receive *every* CAN frame
  CBUS.setFrameHandler(framehandler);

#ifdef CBUS_LONG_MESSAGE
  // register long message handler
  lmsg.allocateContexts(4, 256, 2);
  lmsg.subscribe(streamids, 3, longmessagehandler);
#endif

  // set CBUS LEDs to indicate the current mode
  CBUS.indicateMode(module_config.FLiM);

  // configure and start CAN bus and CBUS message processing
  CBUS.setNumBuffers(8, 4);          // more buffers = more memory used, fewer = less
  CBUS.setOscFreq(12000000UL);       // select the crystal frequency of the CAN module

#ifdef ARDUINO_ARCH_RP2040
  // Pico SPI peripherals have no default pins so all values must be provided
  // Pico SPI peripherals have no default pins so all values must be provided
  CBUS.setPins(CAN_CS, CAN_INT, MCP2515_MOSI, MCP2515_MISO, MCP2515_SCK);
  Serial << "CS pin is " << CBUS.getCSpin() << endl;
  Serial << "INT pin is " << CBUS.getINTpin() << endl;
#endif
  Serial << F("> starting CAN") << endl;
  Serial.flush();

  if (!CBUS.begin()) {
    Serial << F("> error initialising CAN bus") << endl;
  } else {
    Serial << F("> CAN init ok") << endl;
  }
}

//
/// setup - runs once at power on
//

void setup() {

  unsigned long t1 = millis();
  delay(5000);
  Serial.begin (115200);
  //while (!Serial && millis() - t1 < 5000);
  delay(5000);
  t1 = millis();

  Serial << endl << endl << F("> ** CBUS Arduino basic example module ** ") << __FILE__ << endl;
  Serial << F("> Running on Raspberry Pi Pico, startup ms = ") << t1 << endl;

  // I2C bus scan
  WIRE.setSDA(WIRE_SDA);
  WIRE.setSCL(WIRE_SCL);
  WIRE.setClock(400000);
  WIRE.begin(0x30);

  for (byte addr = 1; addr < 128; addr++) {
    WIRE.beginTransmission(addr);
    byte r = WIRE.endTransmission();

    if (r == 0) {
      Serial << F("> detected I2C device at address = 0x") << _HEX(addr) << endl;
    }
  }

  // setup CBUS
  setupCBUS();

  // set initial application default configuration
  if (module_config.isResetFlagSet()) {
   Serial << F("> module has been reset - setting initial application defaults") << endl;
    module_config.clearResetFlag();
  }

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
#ifdef CBUS_LONG_MESSAGE
  lmsg.process();
#endif

  //
  /// process console commands
  //

  processSerialInput();

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
  Serial << F("> EV1 = ") << module_config.getEventEVval(index, 1) << endl;
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
void longmessagehandler(void *msg, unsigned int msg_len, byte stream_id, byte status) {

  char *ptr = (char *)msg;
  ptr[msg_len] = 0;

  Serial << F("> status = ") << status << F(", stream = ") << stream_id << F(", msg = |") << ptr << F("|") << endl;
  return;
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
#ifdef CBUS_LONG_MESSAGE
  Serial << "> using long messages" << endl;
#endif
#ifdef ARDUINO_ARCH_RP2040
  Serial << "> running on ARDUINO_ARCH_RP2040" << endl;
#endif
#ifdef USE_EXTERNAL_EEPROM
  Serial << "> using EXTERNAL EEPROM size " << module_config.getEEPROMsize() << endl;
#endif
  return;
}

//
/// command interpreter for serial console input
//

void processSerialInput(void) {

  byte uev = 0;
  char msgstr[32], dstr[32];
  CANFrame cf;

  if (Serial.available()) {

    char c = Serial.read();

    switch (c) {

      case 'n':

        // node config
        printConfig();

        // node identity
        Serial << F("> CBUS node configuration") << endl;
        Serial << F("> mode = ") << (module_config.FLiM ? "FLiM" : "SLiM") << F(", CANID = ") << module_config.CANID << F(", node number = ") << module_config.nodeNum << endl;
        Serial << endl;
        break;

      // New option to see some raw EEPROM
      case 'd':
        Serial << F("> stored EEPROM ") << endl;
        for (byte j = 0; j < 20; j++)
        {
          Serial << j << " " << module_config.readEEPROM(j) << endl;
        }
        break;

      case 'e':

        // EEPROM learned event data table
        Serial << F("> stored events ") << endl;
        Serial << F("  max events = ") << module_config.EE_MAX_EVENTS << F(" EVs per event = ") << module_config.EE_NUM_EVS << F(" bytes per event = ") << module_config.EE_BYTES_PER_EVENT << endl;

        for (byte j = 0; j < module_config.EE_MAX_EVENTS; j++) {
          if (module_config.getEvTableEntry(j) != 0) {
            ++uev;
          }
        }

        Serial << F("  stored events = ") << uev << F(", free = ") << (module_config.EE_MAX_EVENTS - uev) << endl;
        Serial << F("  using ") << (uev * module_config.EE_BYTES_PER_EVENT) << F(" of ") << (module_config.EE_MAX_EVENTS * module_config.EE_BYTES_PER_EVENT) << F(" bytes") << endl << endl;

        Serial << F("  Ev#  |  NNhi |  NNlo |  ENhi |  ENlo | ");

        for (byte j = 0; j < (module_config.EE_NUM_EVS); j++) {
          sprintf(dstr, "EV%03d | ", j + 1);
          Serial << dstr;
        }

        Serial << F("Hash |") << endl;

        Serial << F(" --------------------------------------------------------------") << endl;

        // for each event data line
        for (byte j = 0; j < module_config.EE_MAX_EVENTS; j++) {

          if (module_config.getEvTableEntry(j) != 0) {
            sprintf(dstr, "  %03d  | ", j);
            Serial << dstr;

            // for each data byte of this event
            for (byte e = 0; e < (module_config.EE_NUM_EVS + 4); e++) {
              sprintf(dstr, " 0x%02hx | ", module_config.readEEPROM(module_config.EE_EVENTS_START + (j * module_config.EE_BYTES_PER_EVENT) + e));
              Serial << dstr;
            }

            sprintf(dstr, "%4d |", module_config.getEvTableEntry(j));
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

        for (byte j = 1; j <= module_config.EE_NUM_NVS; j++) {
          byte v = module_config.readNV(j);
          sprintf(msgstr, " - %02d : %3hd | 0x%02hx", j, v, v);
          Serial << msgstr << endl;
        }

        Serial << endl << endl;

        break;

      // CAN bus status
      case 'c':
        CBUS.printStatus();
        Serial << F("> tx buffer = ") << CBUS.canp->transmitBufferSize(0) << F(", ") << CBUS.canp->transmitBufferCount(0) << F(", ") << CBUS.canp->transmitBufferPeakCount(0) << endl;
        Serial << F("> rx buffer = ") << CBUS.canp->receiveBufferSize() << F(", ") << CBUS.canp->receiveBufferCount() << F(", ") << CBUS.canp->receiveBufferPeakCount() << endl;
        Serial << F("> error flag register = ") << CBUS.canp->errorFlagRegister() << endl;
        break;

      case 'h':
        // event hash table
        module_config.printEvHashTable(false);
        break;

      case 'y':
        // reset CAN bus and CBUS message processing
        CBUS.reset();
        break;

      case '*':
        // reboot
        module_config.reboot();
        break;

      case 'm':
        // free memory
        Serial << F("> free SRAM = ") << module_config.freeSRAM() << F(" bytes") << endl;
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
            module_config.resetModule();
            ResetRq = false;
          }
        }
        break;

      case 'x':
        // send a CBUS message
        cf.id = 0;
        cf.len = 8;
        CBUS.sendMessage(&cf);
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
