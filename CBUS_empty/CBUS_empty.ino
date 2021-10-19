
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
const byte MODULE_ID = 99;          // CBUS module type


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
CBUSConfig myconfig;                // configuration object
CBUS2515 CBUS(&myconfig);           // CBUS object
#ifndef ARDUINO_ARCH_RP2040
CBUSLED ledGrn, ledYlw;             // two LED objects
CBUSSwitch pb_switch;               // switch object
#endif
// module name, must be 7 characters, space padded.
unsigned char mname[7] = { 'E', 'M', 'P', 'T', 'Y', ' ', ' ' };

// forward function declarations
void eventhandler(byte index, byte opc);
void framehandler(CANFrame *msg);

//
/// setup CBUS - runs once at power on from setup()
//
void setupCBUS() {

  // set config layout parameters
  myconfig.EE_NVS_START = 10;
  myconfig.EE_NUM_NVS = 10;
  myconfig.EE_EVENTS_START = 50;
  myconfig.EE_MAX_EVENTS = 32;
  myconfig.EE_NUM_EVS = 1;
  myconfig.EE_BYTES_PER_EVENT = (myconfig.EE_NUM_EVS + 4);

  // initialise and load configuration
  myconfig.setEEPROMtype(EEPROM_INTERNAL);
  myconfig.begin();

  Serial << F("> mode = ") << ((myconfig.FLiM) ? "FLiM" : "SLiM") << F(", CANID = ") << myconfig.CANID;
  Serial << F(", NN = ") << myconfig.nodeNum << endl;

  // show code version and copyright notice
  printConfig();

  // set module parameters
  CBUSParams params(myconfig);
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
  if (pb_switch.isPressed() && !myconfig.FLiM) {
    Serial << F("> switch was pressed at startup in SLiM mode") << endl;
    myconfig.resetModule(ledGrn, ledYlw, pb_switch);
  }
#endif

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);

  // register our CAN frame handler, to receive *every* CAN frame
  CBUS.setFrameHandler(framehandler);

#ifndef ARDUINO_ARCH_RP2040
  // set CBUS LEDs to indicate the current mode
  CBUS.indicateMode(myconfig.FLiM);
#endif

  // configure and start CAN bus and CBUS message processing
  CBUS.setNumBuffers(2, 2);      // more buffers = more memory used, fewer = less
  CBUS.setOscFreq(16000000UL);   // select the crystal frequency of the CAN module
#ifdef ARDUINO_ARCH_RP2040
  // configure SPI pins
  SPI.setSCK(PICO_SCK);   // SCK
  SPI.setTX(PICO_MOSI);   // MOSI
  SPI.setRX(PICO_MISO);   // MISO
  SPI.setCS(PICO_CS);     // CS
  CBUS.setPins(PICO_CS, PICO_INT, PICO_MOSI, PICO_MISO, PICO_SCK);
#else
  CBUS.setPins(10, 2);           // select pins for CAN bus CE and interrupt connections
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
  Serial << endl << endl << F("> ** CBUS Arduino basic example module ** ") << __FILE__ << endl;

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
  /// process console commands
  //

  processSerialInput();

  //
  /// check CAN message buffers
  //

  if (CBUS.canp->receiveBufferPeakCount() > CBUS.canp->receiveBufferSize()) {
    // Serial << F("> receive buffer overflow") << endl;
  }

  if (CBUS.canp->transmitBufferPeakCount(0) > CBUS.canp->transmitBufferSize(0)) {
    // Serial << F("> transmit buffer overflow") << endl;
  }

  //
  /// check CAN bus state
  //

  byte s = CBUS.canp->errorFlagRegister();
  if (s != 0) {
    // Serial << F("> error flag register is non-zero") << endl;
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
  Serial << F("> EV1 = ") << myconfig.getEventEVval(index, 1) << endl;
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
        Serial << F("> mode = ") << (myconfig.FLiM ? "FLiM" : "SLiM") << F(", CANID = ") << myconfig.CANID << F(", node number = ") << myconfig.nodeNum << endl;
        Serial << endl;
        break;

      case 'e':

        // EEPROM learned event data table
        Serial << F("> stored events ") << endl;
        Serial << F("  max events = ") << myconfig.EE_MAX_EVENTS << F(" EVs per event = ") << myconfig.EE_NUM_EVS << F(" bytes per event = ") << myconfig.EE_BYTES_PER_EVENT << endl;

        for (byte j = 0; j < myconfig.EE_MAX_EVENTS; j++) {
          if (myconfig.getEvTableEntry(j) != 0) {
            ++uev;
          }
        }

        Serial << F("  stored events = ") << uev << F(", free = ") << (myconfig.EE_MAX_EVENTS - uev) << endl;
        Serial << F("  using ") << (uev * myconfig.EE_BYTES_PER_EVENT) << F(" of ") << (myconfig.EE_MAX_EVENTS * myconfig.EE_BYTES_PER_EVENT) << F(" bytes") << endl << endl;

        Serial << F("  Ev#  |  NNhi |  NNlo |  ENhi |  ENlo | ");

        for (byte j = 0; j < (myconfig.EE_NUM_EVS); j++) {
          sprintf(dstr, "EV%03d | ", j + 1);
          Serial << dstr;
        }

        Serial << F("Hash |") << endl;

        Serial << F(" --------------------------------------------------------------") << endl;

        // for each event data line
        for (byte j = 0; j < myconfig.EE_MAX_EVENTS; j++) {

          if (myconfig.getEvTableEntry(j) != 0) {
            sprintf(dstr, "  %03d  | ", j);
            Serial << dstr;

            // for each data byte of this event
            for (byte e = 0; e < (myconfig.EE_NUM_EVS + 4); e++) {
              sprintf(dstr, " 0x%02hx | ", myconfig.readEEPROM(myconfig.EE_EVENTS_START + (j * myconfig.EE_BYTES_PER_EVENT) + e));
              Serial << dstr;
            }

            sprintf(dstr, "%4d |", myconfig.getEvTableEntry(j));
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

        for (byte j = 1; j <= myconfig.EE_NUM_NVS; j++) {
          byte v = myconfig.readNV(j);
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
        myconfig.printEvHashTable(false);
        break;

      case 'y':
        // reset CAN bus and CBUS message processing
        CBUS.reset();
        break;

      case '*':
        // reboot
        myconfig.reboot();
        break;

      case 'm':
        // free memory
        Serial << F("> free SRAM = ") << myconfig.freeSRAM() << F(" bytes") << endl;
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
