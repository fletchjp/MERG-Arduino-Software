///////////////////////////////////////////////////////////////////////////////////
// CANTOTEM
// Version 1b beta 1
// Modification to start to use IoAbstraction and TaskManagerIO
// as has been done in CANCMDDC in CANCMDDC2
// Version 1b beta 2 Adding code to send text change messages.
// Version 1b beta 3 Adding code for framehandler.
// Version 1b beta 4 Correct bugs inherited from CANmINmOUT event code.
// Version 1b beta 5 Add Sven's modifications to CANmINmOUT event code.
///////////////////////////////////////////////////////////////////////////////////
// This is to run on the TOTEM Minilab with a CAN interface.
// working from
// TOTEMmINnOUT
// Copied from
// CANmINnOUT


/*
  Copyright (C) 2021 Martin Da Costa
  Including copyrights from CBUS_1in1out and Arduino CBUS Libraries


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
      3rd party libraries needed for compilation:

      Streaming   -- C++ stream style output, v5, (http://arduiniana.org/libraries/streaming/)
      ACAN2515    -- library to support the MCP2515/25625 CAN controller IC
      CBUSSwitch  -- library access required by CBUS and CBUS Config
      CBUSLED     -- library access required by CBUS and CBUS Config
*/
///////////////////////////////////////////////////////////////////////////////////
// Pin Use map UNO:
// Digital pin 2          Interupt CAN
// Digital pin 3 (PWM)    Not Used
// Digital pin 4          Not Used
// Digital pin 5 (PWM)    Not Used
// Digital pin 6 (PWM)    Module Switch 2
// Digital pin 7          Module LED 2
// Digital pin 8          Module LED 1
// Digital pin 9 (PWM)    Module Switch 1
// Digital pin 10 (SS)    CS    CAN
// Digital pin 11 (MOSI)  SI    CAN
// Digital pin 12 (MISO)  SO    CAN
// Digital pin 13 (SCK)   Sck   CAN

// Digital / Analog pin 0     Not Used
// Digital / Analog pin 1     Not Used
// Digital / Analog pin 2     Not Used
// Digital / Analog pin 3     Not Used
// Digital / Analog pin 4     Not Used
// Digital / Analog pin 5     Not Used
//////////////////////////////////////////////////////////////////////////

#define DEBUG 1       // set to 0 for no serial debug

// IoAbstraction libraries
#include <IoAbstraction.h>
#include <TaskManagerIO.h>
// IoAbstraction reference to the arduino pins.
IoAbstractionRef arduinoPins = ioUsingArduino();

// 3rd party libraries
#include <Streaming.h>
#include <Bounce2.h>

// CBUS library header files
#include <CBUS2515.h>            // CAN controller and CBUS class
#include "LEDControl.h"          // CBUS LEDs
#include <CBUSconfig.h>          // module configuration
#include <cbusdefs.h>            // MERG CBUS constants
#include <CBUSParams.h>

////////////DEFINE MODULE/////////////////////////////////////////////////

// module name
unsigned char mname[7] = { 'T', 'O', 'T', 'E', 'M', ' ', ' ' };

// forward function declarations
void eventhandler(byte index, byte opc);
void framehandler(CANFrame *msg);

// Set opcodes for polling events
byte nopcodes = 9;
byte opcodes[] = {OPC_ACON, OPC_ACOF, OPC_ARON, OPC_AROF, OPC_ASON, OPC_ASOF, OPC_AREQ, OPC_ASRQ, OPC_CANID }; 

// constants
const byte VER_MAJ = 1;         // code major version
const char VER_MIN = 'b';       // code minor version
const byte VER_BETA = 5;        // code beta sub-version
const byte MODULE_ID = 99;      // CBUS module type

const unsigned long CAN_OSC_FREQ = 8000000;     // Oscillator frequency on the CAN2515 board

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

//CBUS pins
const byte CAN_INT_PIN = 2;  // Only pin 2 and 3 support interrupts
const byte CAN_CS_PIN = 10;
//const byte CAN_SI_PIN = 11;  // Cannot be changed
//const byte CAN_SO_PIN = 12;  // Cannot be changed
//const byte CAN_SCK_PIN = 13;  // Cannot be changed

// CBUS objects
CBUS2515 CBUS;                      // CBUS object
CBUSConfig config;                  // configuration object

// Event Nos for different events to be sent
// enum base changed to avoid other events.
// These are ideas at the moment.
enum eventNos {
  noEvent = 100,  // not used
  testEvent,
  emergencyEvent,
  errorEvent,
  dataEvent,
  requestEvent,
  invalidEvent
};

//
///  setup CBUS - runs once at power on called from setup()
//
void setupCBUS()
{
  // set config layout parameters
  config.EE_NVS_START = 10;
  config.EE_NUM_NVS = NUM_SWITCHES;
  config.EE_EVENTS_START = 50;
  config.EE_MAX_EVENTS = 64;
  config.EE_NUM_EVS = NUM_LEDS;
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

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);
  // This will only process the defined opcodes.
  CBUS.setFrameHandler(framehandler, opcodes, nopcodes);

  // configure and start CAN bus and CBUS message processing
  CBUS.setNumBuffers(2);         // more buffers = more memory used, fewer = less
  CBUS.setOscFreq(CAN_OSC_FREQ);   // select the crystal frequency of the CAN module
  CBUS.setPins(CAN_CS_PIN, CAN_INT_PIN);           // select pins for CAN bus CE and interrupt connections
  CBUS.begin();
}


void runLEDs(){
  // Run the LED code
  for (int i = 0; i < NUM_LEDS; i++) {
    moduleLED[i].run();
  }
}
//
///  setup Module - runs once at power on called from setup()
//

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


void setup()
{
  Serial.begin (115200);
  Serial << endl << endl << F("> ** CANTOTEM ** ") << __FILE__ << endl;

  setupCBUS();
  setupModule();

  // Schedule tasks to run every 250 milliseconds.
  taskManager.scheduleFixedRate(250, runLEDs);
  taskManager.scheduleFixedRate(250, processSwitches);

  // end of setup
#if DEBUG
  Serial << F("> ready") << endl << endl;
#endif
}


void loop()
{
  // do CBUS message, switch and LED processing
  CBUS.process();

  // process console commands
  processSerialInput();

  // Run IO_Abstraction tasks.
  // This replaces actions taken here in the previous version.
  taskManager.runLoop();


 }

void processSwitches(void)
{
   for (int i = 0; i < NUM_SWITCHES; i++)
  {
    moduleSwitch[i].update();
    if (moduleSwitch[i].changed())
    {
     byte nv = i + 1;
     byte nvval = config.readNV(nv);

     byte opCode;

#if DEBUG
     Serial << F("Switch ") << i << F(" changed") << endl; 
#endif   
     Serial << F (" NV = ") << nv << F(" NV Value = ") << nvval << endl;

     switch (nvval)
     {
      case 0:
#if DEBUG
        Serial << F("> Button ") << i << F(" state change detected") << endl;
        if (moduleSwitch[i].fell()) {
          Serial << F("> Button ") << i << F(" pressed, send 0x") << _HEX(OPC_ACON) << endl;
        } else {
          Serial << F("> Button ") << i << F(" released, send 0x") << _HEX(OPC_ACOF) << endl;
        }
#endif

        opCode = (moduleSwitch[i].fell() ? OPC_ACON : OPC_ACOF);
        sendEvent(opCode, (i + 1));
        break;

      case 1:
#if DEBUG
        Serial << F("> Button ") << i << F(" state change detected") << endl;
        if (moduleSwitch[i].fell()) 
        {
          Serial << F("> Button ") << i << F(" pressed, send 0x") << _HEX(OPC_ACON) << endl;
        }
#endif

        if (moduleSwitch[i].fell()) 
        {
          sendEvent(opCode, (i + 1));
          opCode = OPC_ACON;
        }
        break;

      case 2:
#if DEBUG
        Serial << F("> Button ") << i << F(" state change detected") << endl;
        if (moduleSwitch[i].rose()) 
        {
          Serial << F("> Button ") << i << F(" pressed, send 0x") << _HEX(OPC_ACOF) << endl;
        }
#endif

        if (moduleSwitch[i].rose())
        {
          opCode = OPC_ACOF;
          sendEvent(opCode, (i + 1));
        }
        break;

      case 3:

        if (moduleSwitch[i].fell())
        {
          switchState[i] = !switchState[i];
        }
#if DEBUG
        Serial << F("> Button ") << i << F(" state change detected") << endl;
        if (switchState[i]) {
          Serial << F("> Button ") << i << F(" pressed, send 0x") << _HEX(OPC_ACON) << endl;
        } else {
          Serial << F("> Button ") << i << F(" pressed, send 0x") << _HEX(OPC_ACOF) << endl;
        }
#endif

        opCode = (switchState[i] ? OPC_ACON : OPC_ACOF);
        sendEvent(opCode, (i + 1));

        break;
        // Send event to test display on CAN1602BUT.
      case 99:
        opCode = (switchState[i] ? OPC_ACON : OPC_ACOF);
        sendEvent(opCode,testEvent); // Test of new code.

	      break;
		default:
#if DEBUG
        Serial << F("> Invalid NV value ") << nvval << endl;
#endif
        break;
     }
    }
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

#if DEBUG
  if (CBUS.sendMessage(&msg)) {
    Serial << F("> sent CBUS message with Event Number ") << eventNo << endl;
  } else {
    Serial << F("> error sending CBUS message") << endl;
  }
#endif
}

//
/// called from the CBUS library when a learned event is received
//
void eventhandler(byte index, CANFrame *msg)
{
  byte opc = msg->data[0];

#if DEBUG
  Serial << F("> event handler: index = ") << index << F(", opcode = 0x") << _HEX(msg->data[0]) << endl;
  byte len = msg->len;
  Serial << F("> event handler: length = ") << len << endl;
#endif

  unsigned int node_number = (msg->data[1] << 8 ) + msg->data[2];
  unsigned int event_number = (msg->data[3] << 8 ) + msg->data[4];
#if DEBUG
  Serial << F("> NN = ") << node_number << F(", EN = ") << event_number << endl;
  Serial << F("> op_code = ") << opc << endl;
#endif

  switch (opc) {

    case OPC_ACON:
    case OPC_ASON:
      for (int i = 0; i < NUM_LEDS; i++)
      {
        byte ev = i + 1;
        byte evval = config.getEventEVval(index, ev);

        switch (evval) {
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
}

void framehandler(CANFrame *msg) {

  byte op_code = msg->data[0];
  
  // as an example, format and display the received frame

  Serial << "[ " << (msg->id & 0x7f) << "] [" << msg->len << "] [";

  for (byte d = 0; d < msg->len; d++) {
    Serial << " 0x" << _HEX(msg->data[d]);
  }

  Serial << " ]" << endl;

  if (  op_code ==  OPC_CANID ) {
  unsigned int node_number = (msg->data[1] << 8 ) + msg->data[2];
  unsigned int new_CANId = msg->data[3];
      Serial << F("> Frame to change CANId") << endl;
      Serial << F("> Node No   ") << node_number << endl;
      Serial << F("> new CANId ")  << new_CANId << endl;
  }

}


void printConfig(void)
{
  // code version
  Serial << F("> code version = ") << VER_MAJ << VER_MIN << F(" beta ") << VER_BETA << endl;
  Serial << F("> compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(", compiler ver = ") << __cplusplus << endl;

  // copyright
  Serial << F("> © Martin Da Costa (MERG M6223) 2020") << endl;
}

//
/// command interpreter for serial console input
//

void processSerialInput(void)
{
  byte uev = 0;
  char msgstr[32];

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
          sprintf(msgstr, "EV%03d | ", j + 1);
          Serial << msgstr;
        }

        Serial << F("Hash |") << endl;

        Serial << F(" --------------------------------------------------------------") << endl;

        // for each event data line
        for (byte j = 0; j < config.EE_MAX_EVENTS; j++) {
          if (config.getEvTableEntry(j) != 0) {
            sprintf(msgstr, "  %03d  | ", j);
            Serial << msgstr;

            // for each data byte of this event
            for (byte e = 0; e < (config.EE_NUM_EVS + 4); e++) {
              sprintf(msgstr, " 0x%02hx | ", config.readEEPROM(config.EE_EVENTS_START + (j * config.EE_BYTES_PER_EVENT) + e));
              Serial << msgstr;
            }

            sprintf(msgstr, "%4d |", config.getEvTableEntry(j));
            Serial << msgstr << endl;
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

      default:
        // Serial << F("> unknown command ") << c << endl;
        break;
    }
  }
}
