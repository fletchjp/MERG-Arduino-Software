// CBUSACAN2040_v1
///////////////////////////////////////////////////////////////////////////////////////////
// This is a new code developed from CBUS_1in1out_ACAN2040_v3
// which was renamed from example CBUS_1in1out with the CBUSACAN2040 library
// Adding external EEPROM
// and starting to add more things from CANDUE3EEPROM
// processSerialInput is now a task.
// This example is running on the Waveshare Pico-Servo-Driver with an external EEPROM.
// There are no LEDs and no button.
////////////////////////////////////////////////////////////////////////////////////////////
// This code is going to have more of the structure of CANDUE3EEPROM so that events can be explored.
////////////////////////////////////////////////////////////////////////////////////////////
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
      ACAN2040    -- library wrapper for can2040 RP2040 CAN driver
*/

// IoAbstraction libraries
#include <IoAbstraction.h>
#include <AnalogDeviceAbstraction.h>
#include <TaskManagerIO.h>
#include <DeviceEvents.h>

// 3rd party libraries
#include <Streaming.h>
#ifdef USE_EXTERNAL_EEPROM
#include <Wire.h>
#define WIRE Wire1
#ifdef ARDUINO_ARCH_RP2040
static const byte WIRE_SDA = 6;  
static const byte WIRE_SCL = 7;
#endif
#endif

// CBUS pins for external device
static const byte CAN_TX = 1;
static const byte CAN_RX = 2;

// CBUS library header files
#include <CBUSACAN2040.h>           // CAN controller and CBUS class
#include <CBUSswitch.h>             // pushbutton switch
#include <CBUSLED.h>                // CBUS LEDs
#include <CBUSconfig.h>             // module configuration
#include <CBUSParams.h>             // CBUS parameters
#include <cbusdefs.h>               // MERG CBUS constants

// Note that on the Pico Breadboard the pins go HIGH when pressed.
#define PICO_BREADBOARD
//#define USE_EXTERNAL_EEPROM
#define DEBUG         1 // set to 0 for no serial debug

#if DEBUG
#define DEBUG_PRINT(S) Serial << S << endl
#else
#define DEBUG_PRINT(S)
#endif

// constants
const byte VER_MAJ = 1;             // code major version
const char VER_MIN = 'a';           // code minor version
const byte VER_BETA = 1;            // code beta sub-version
const byte MODULE_ID = 99;          // CBUS module type

const byte LED_GRN = 8;             // CBUS green SLiM LED pin
const byte LED_YLW = 9;             // CBUS yellow FLiM LED pin
const byte SWITCH0 = 10;            // CBUS push button switch pin


#define NUM_LEDS 2              // How many LEDs are there?
#define NUM_SWITCHES 2          // How many switchs are there?

// CBUS objects
CBUSConfig module_config;           // configuration object
CBUSACAN2040 CBUS(&module_config);  // CBUS object
CBUSLED ledGrn, ledYlw;             // two LED objects
CBUSSwitch pb_switch;               // switch object

// module objects
CBUSSwitch moduleSwitch;            // an example switch as input
CBUSLED moduleLED;                  // an example LED as output

// Pico pins for the button and LED
static const byte MODULE_BUTTON = 11;  // Breadboard button 2
static const byte MODULE_LED    = 12;  // Breadboard LED 3


// module name, must be 7 characters, space padded.
unsigned char mname[7] = { 'C', 'A', 'N', '2', '0', '4', '0' };

// forward function declarations
void eventhandler(byte index, byte opc);

//
/// setup CBUS - runs once at power on from setup()
//

void setupCBUS() {

  // set config layout parameters
  module_config.EE_NVS_START = 10;
  module_config.EE_NUM_NVS = 10;
  module_config.EE_EVENTS_START = 20;
  module_config.EE_MAX_EVENTS = 32;
  module_config.EE_NUM_EVS = 1;
  module_config.EE_BYTES_PER_EVENT = (module_config.EE_NUM_EVS + 4);

  // initialise and load configuration
#ifdef USE_EXTERNAL_EEPROM
  module_config.setExtEEPROMAddress(0x50, &WIRE);
#ifdef ARDUINO_ARCH_RP2040
  module_config.setExtEEPROMPins(WIRE_SDA, WIRE_SCL);
#endif
  // This can fail.
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
  params.setModuleId(MODULE_ID);
  params.setFlags(PF_FLiM | PF_COMBI);

  // assign to CBUS
  CBUS.setParams(params.getParams());
  CBUS.setName(mname);

  // set CBUS LED pins and assign to CBUS
  ledGrn.setPin(LED_GRN);
  ledYlw.setPin(LED_YLW);
  CBUS.setLEDs(ledGrn, ledYlw);

  // initialise CBUS switch and assign to CBUS
#ifdef PICO_BREADBOARD
  pb_switch.setPin(SWITCH0, HIGH); // was LOW
#else
  pb_switch.setPin(SWITCH0, LOW);
#endif
  pb_switch.run();
  CBUS.setSwitch(pb_switch);

  // module reset - if switch is depressed at startup and module is in SLiM mode
  if (pb_switch.isPressed() && !module_config.FLiM) {
    Serial << F("> switch was pressed at startup in SLiM mode") << endl;
    module_config.resetModule(ledGrn, ledYlw, pb_switch);
  }

  // opportunity to set default NVs after module reset
  if (module_config.isResetFlagSet()) {
    Serial << F("> module has been reset") << endl;
    module_config.clearResetFlag();
  }

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);

  // set CBUS LEDs to indicate mode
  CBUS.indicateMode(module_config.FLiM);

  // configure and start CAN bus and CBUS message processing
  CBUS.setNumBuffers(16, 4);     // more buffers = more memory used, fewer = less
  CBUS.setPins(CAN_TX, CAN_RX);  // select pins for CAN tx and rx

  if (!CBUS.begin()) {
    Serial << F("> can2040 init fail") << endl;
  } else {
    Serial << F("> can2040 init ok") << endl;
  }

}

//
/// setup - runs once at power on
//

void setup() {

  delay(5000);
  Serial.begin (115200);
  //while (!Serial);
  delay(5000);
  Serial << endl << endl << F("> ** CBUS 1 in 1 out v3 ** ") << endl;
  Serial << __FILE__ << endl;

  setupCBUS();

  // configure the module switch, attached to pin 11, active low
#ifdef PICO_BREADBOARD
  moduleSwitch.setPin(MODULE_BUTTON, HIGH); // was LOW
#else
  moduleSwitch.setPin(MODULE_BUTTON, LOW); 
#endif

  // configure the module LED, attached to pin MODULE_LED via a 1K resistor
  moduleLED.setPin(MODULE_LED);

  // Schedule tasks to run every 250 milliseconds.
  //taskManager.scheduleFixedRate(250, processModuleSwitchChange);
  taskManager.scheduleFixedRate(250, processSerialInput);

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

  //processSerialInput(); Replaced with task.

  //
  /// give the switch and LED code some time to run
  //

  moduleSwitch.run();
  moduleLED.run();

  //
  /// Check if switch changed and do any processing for this change is now a task.
  //  Task now disabled as it is not working properly.
  processModuleSwitchChange();

// Run IO_Abstraction tasks.
  // This replaces actions taken here in the previous version.
  taskManager.runLoop();

  // bottom of loop()
}

// Send an event routine with the given opCode and eventNo.
bool sendEvent(byte opCode, unsigned int eventNo)
{
  CANFrame msg;
  msg.id = module_config.CANID;
  msg.len = 5;
  msg.data[0] = opCode;
  msg.data[1] = highByte(module_config.nodeNum);
  msg.data[2] = lowByte(module_config.nodeNum);
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

//
/// test for switch input
/// as an example, it must be have been pressed or released for at least half a second
/// then send a long CBUS event with opcode ACON for on and ACOF for off
/// event number (EN) is 1

/// you can just watch for this event in FCU or JMRI, or teach it to another CBUS consumer module
//
void processModuleSwitchChange() {
  byte opcode;
  unsigned int event_no = 1;
  if (moduleSwitch.stateChanged()) {

    if (moduleSwitch.isPressed() ) {
        moduleLED.on();
        DEBUG_PRINT(F("> moduleLED ON ") );
        opcode = OPC_ACON;
    } else {
        moduleLED.off();
        DEBUG_PRINT(F("> moduleLED OFF ") );
        opcode = OPC_ACOF;
    }
    // Send the event
    sendEvent(opcode, event_no);
  }
}

//
/// user-defined event processing function
/// called from the CBUS library when a learned event is received
/// it receives the event table index and the CAN frame
//

void eventhandler(byte index, CANFrame *msg) {

  // as an example, control an LED

  Serial << F("> event handler: index = ") << index << F(", opcode = 0x") << _HEX(msg->data[0]) << endl;

  // read the value of the first event variable (EV) associated with this learned event
  byte evval = module_config.getEventEVval(index, 1);
  Serial << F("> EV1 = ") << evval << endl;

  // set the LED according to the opcode of the received event, if the first EV equals 0
  // we turn on the LED and if the first EV equals 1 we use the blink() method of the LED object as an example

  if (msg->data[0] == OPC_ACON) {
    if (evval == 0) {
      Serial << F("> switching the LED on") << endl;
      moduleLED.on();
    } else if (evval == 1) {
      Serial << F("> switching the LED to blink") << endl;
      moduleLED.blink();
    }
  } else if (msg->data[0] == OPC_ACOF) {
    Serial << F("> switching the LED off") << endl;
    moduleLED.off();
  }
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
#ifdef ARDUINO_ARCH_RP2040
  Serial << "> running on ARDUINO_ARCH_RP2040" << endl;
#endif
#ifdef USE_EXTERNAL_EEPROM
  if (module_config.getEEPROMtype() == EEPROM_EXTERNAL) {
    Serial << "> using EXTERNAL EEPROM size " << module_config.getEEPROMsize() << endl;    
  } else {
    Serial << "> using INTERNAL EEPROM size " << module_config.getEEPROMsize() << endl;
  }
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
