////////////////////////////////////////////////////////////////////////////////////
// CAN1602BUT
// Take code from CANALCDBUT to make a new code on the CANmINnOUT base.
// Version 1.0b beta2 starting to add the code for input events setting the display.
// Version 1.0b beta3 Adding more code from CANTEXT 
// and decoding of node number and event number of incoming events.
// Also change the display in response to an incoming event.
// Version 1.0b beta4 Changes as the previous version did not update the display.
// Add some more opcodes.
// Version 1.0b beta5 Correct bugs inherited from CANmINmOUT event code.
// Version 1.0b beta6 Add Sven's modifications to CANmINmOUT event code.
// Version 1.0b beta7 Correct lack of return value from sendEvent.
// Version 1.0b beta8 Use DEBUG_PRINT and process send fail in processSwitches.
// Version 1.0b beta9 Change event no of incoming error.
// Version 1.0b beta10 Changes to code processing the incoming error.
// Version 1.0b beta11 Make processSerialInput a task.
////////////////////////////////////////////////////////////////////////////////////
// CANTOTEM
// Modification to start to use IoAbstraction and TaskManagerIO
// as has been done in CANCMDDC in CANCMDDC2
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
// Digital pin 3 (PWM)    LED 0
// Digital pin 4          LCD pin_d4
// Digital pin 5 (PWM)    LCD pin_d5
// Digital pin 6 (PWM)    LCD pin_d6
// Digital pin 7          LCD pin_d7
// Digital pin 8          LCD pin_RS
// Digital pin 9 (PWM)    LCD pin_EN
// Digital pin 10         LCD backlight pin
// Digital pin 11 (MOSI)  SI    CAN
// Digital pin 12 (MISO)  SO    CAN
// Digital pin 13 (SCK)   Sck   CAN

// Digital pin 14 / Analog pin 0  Analog input from buttons
// Digital pin 15 / Analog pin 1 (SS)    CS    CAN    
// Digital pin 16 / Analog pin 2     Switch 0
// Digital pin 17 / Analog pin 3     Bell/buzzer use.
// Digital / Analog pin 4     Not Used - reserved for I2C
// Digital / Analog pin 5     Not Used - reserved for I2C
//////////////////////////////////////////////////////////////////////////

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

// IoAbstraction reference to the arduino pins.
IoAbstractionRef arduinoPins = ioUsingArduino();

#define ANALOG_IN_PIN A0
// here we create the abstraction over the standard arduino analog IO capabilities
ArduinoAnalogDevice analog; // by default it assumes 10 bit read, 8 bit write

int previous_analog = 0;
int analog_value;

// 3rd party libraries
#include <Streaming.h>
#include <Bounce2.h>

#include <LiquidCrystal.h>
//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

// Variables for buttons
int x;
int prevx = 0;
int range;
int prevrange = 0;
// Use these for the CBUS outputs
int button;
int prevbutton = 0;

// CBUS library header files
#include <CBUS2515.h>            // CAN controller and CBUS class
#include "LEDControl.h"          // CBUS LEDs
#include <CBUSconfig.h>          // module configuration
#include <cbusdefs.h>            // MERG CBUS constants
#include <CBUSParams.h>

////////////DEFINE MODULE/////////////////////////////////////////////////

// module name
unsigned char mname[7] = { '1', '6', '0', '2', 'B', 'U', 'T' };

// constants
const byte VER_MAJ = 1;         // code major version
const char VER_MIN = 'b';       // code minor version
const byte VER_BETA = 11;        // code beta sub-version
const byte MODULE_ID = 99;      // CBUS module type

const unsigned long CAN_OSC_FREQ = 8000000;     // Oscillator frequency on the CAN2515 board

#define NUM_LEDS 1              // How many LEDs are there?
#define NUM_SWITCHES 1          // How many switchs are there?

//Module pins available for use are Pins 3 and A2 - A5
const byte LED[NUM_LEDS] = {3};            // LED pin connections through typ. 1K8 resistor
const byte SWITCH[NUM_SWITCHES] = {16};     // Module Switch takes input to 0V.

// module objects
Bounce moduleSwitch[NUM_SWITCHES];  //  switch as input
LEDControl moduleLED[NUM_LEDS];     //  LED as output
byte switchState[NUM_SWITCHES];

//////////////////////////////////////////////////////////////////////////

//CBUS pins
const byte CAN_INT_PIN = 2;  // Only pin 2 and 3 support interrupts
const byte CAN_CS_PIN = 15;  // Changed from 10 which is used for the display.
//const byte CAN_SI_PIN = 11;  // Cannot be changed
//const byte CAN_SO_PIN = 12;  // Cannot be changed
//const byte CAN_SCK_PIN = 13;  // Cannot be changed

// CBUS objects
CBUS2515 CBUS;                      // CBUS object
CBUSConfig config;                  // configuration object

////////////////////////////////////////////////////////////////////////////////////
// Adapted from CANTEXT
// forward function declarations
////////////////////////////////////////////////////////////////////////////////////
void eventhandler(byte index, CANFrame *msg);
void framehandler(CANFrame *msg);
// Opcodes to be recognised by frame handler.
byte nopcodes = 11;
byte opcodes[] = {OPC_ACON, OPC_ACOF, OPC_ACON1, OPC_ACOF1, OPC_ACON2, OPC_ACOF2, OPC_ACON3, OPC_ACOF3, 
                  OPC_ASON, OPC_ASOF, OPC_CANID };

// Buzzer pin
#define SOUNDER  17

// Adding a buzzer output for taught event
int buzzer = SOUNDER;
#define TONE 1000    // Set the tone for the buzzer

// Index values for errors
enum errorStates {
  blankError,
  noError,
  emergencyStop,
  CANbusError,
  invalidError
};

// Index values for incoming event processing
// enum base changed to avoid other events.
// These are ideas at the moment.
enum eventTypes {
  nonEvent = 100,  // not used
  testEvent,
  emergencyEvent,
  sendFailureEvent,
  dataEvent,
  requestEvent,
  invalidEvent
};

// This is following the methods in EzyBus_master to provide error messages.
// These have been limited to 16 chars to go on an LCD 2 by 16 display.
// blank_string is used to cancel an error message.
const char blank_string[]   PROGMEM  = "                ";
const char error_string_0[] PROGMEM  = "no error";
const char error_string_1[] PROGMEM  = "Test message";
const char error_string_2[] PROGMEM  = "Emergency Stop";
const char error_string_3[] PROGMEM  = "CANbus error";
const char error_string_4[] PROGMEM  = "invalid error";

const char* const error_string_table[] PROGMEM = {
  blank_string, error_string_0, error_string_1, error_string_2, error_string_3, error_string_4
};

#define MAX_ERROR_NO 5

// Buffer for string output.
// This has been made safe for line termination.
#define MAX_LENGTH_OF_STRING 16
#define LENGTH_OF_BUFFER (MAX_LENGTH_OF_STRING + 1)
char error_buffer[LENGTH_OF_BUFFER];

// Add check for invalid error
void getErrorMessage(int i)
{
  if (i >= 0 && i <= MAX_ERROR_NO) {
     strncpy_P(error_buffer, (char*)pgm_read_word(&(error_string_table[i])),MAX_LENGTH_OF_STRING); 
  } else {
     strncpy_P(error_buffer, (char*)pgm_read_word(&(error_string_table[MAX_ERROR_NO])),MAX_LENGTH_OF_STRING); 
  }
}

void serialPrintError(int i)
{
 getErrorMessage(i);Serial.print(error_buffer); 
}
void serialPrintErrorln(int i)
{
 getErrorMessage(i);Serial.println(error_buffer);
}


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
  CBUS.setFrameHandler(framehandler, opcodes, nopcodes);

  // configure and start CAN bus and CBUS message processing
  CBUS.setNumBuffers(2);         // more buffers = more memory used, fewer = less
  CBUS.setOscFreq(CAN_OSC_FREQ);   // select the crystal frequency of the CAN module
  CBUS.setPins(CAN_CS_PIN, CAN_INT_PIN);           // select pins for CAN bus CE and interrupt connections
  CBUS.begin();
}

void checkA0()
{
 x = analogRead (0);
 if (x < 175){         // was 50
  range = 1;
 } else if (x < 350){ // was 250
  range = 2;
 } else if (x < 500){ // unchanged
  range = 3;
 } else if (x < 800){ // was 650
  range = 4;
 } else if (x < 850){ // unchanged
  range = 5;
 } //else { range = 0; }

 if (range != prevrange) {
 Serial.print(range);
 Serial.print(" ");
 Serial.print(x);
 lcd.setCursor(10,1);
 switch (range) {
  case 1:
  {
   lcd.print ("Right ");
   DEBUG_PRINT(F(" Right"));
   break;
  }
  case 2:
  {
   lcd.print ("Up    ");
   DEBUG_PRINT(F(" Up"));
   break;
  }
  case 3:
  {
   lcd.print ("Down  ");
   DEBUG_PRINT(F(" Down"));
   break;
  }
  case 4:
  {
   lcd.print ("Left  ");
   DEBUG_PRINT(F(" Left"));
   break;
  }
  case 5:
  {
   lcd.print ("Select");
   DEBUG_PRINT(F(" Select"));
   break;
  }
  default:
  break;
 }
 prevrange = range;
 // For CBUS output
 button = range;
 }
  
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

void setup1602() {
 lcd.begin(16, 2);
 lcd.setCursor(0,0);
 lcd.print("CAN1602BUT");
 lcd.setCursor(0,1);
 lcd.print("Press Key:");
}

void setup()
{
  Serial.begin (115200);
  Serial << endl << endl << F("> ** CAN1602BUT ** ") << __FILE__ << endl;

  setup1602();
  setupCBUS();
  setupModule();

  // Schedule tasks to run every 250 milliseconds.
  taskManager.scheduleFixedRate(250, runLEDs);
  taskManager.scheduleFixedRate(250, processSwitches);
  taskManager.scheduleFixedRate(250, checkA0);
  taskManager.scheduleFixedRate(250, processSerialInput);
  taskManager.scheduleFixedRate(250, processButtons);

  // end of setup
  DEBUG_PRINT(F("> Using buzzer on pin ") << SOUNDER
         << F(" with tone set to ") << TONE );
  DEBUG_PRINT(F("> ready") << endl);
}


void loop()
{
  // do CBUS message, switch and LED processing
  CBUS.process();

  // process console commands is now a task.
  // processSerialInput();

  // Run IO_Abstraction tasks.
  // This replaces actions taken here in the previous version.
  taskManager.runLoop();


 }

void processButtons(void)
{
   // Send an event corresponding to the button, add NUM_SWITCHES to avoid switch events.
   byte opCode;
   if (button != prevbutton) {
      DEBUG_PRINT(F("Button ") << button << F(" changed")); 
      opCode = OPC_ACON;
      sendEvent(opCode, button + NUM_SWITCHES);
      prevbutton = button;
   }
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
     Serial << F (" NV = ") << nv << F(" NV Value = ") << nvval;

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

void displayError(int i,byte x,byte y)
{
  getErrorMessage(i);
  lcd.setCursor(x, y);
  lcd.write("E: ");
  lcd.write(error_buffer);
}

//
/// called from the CBUS library when a learned event is received
//
void eventhandler(byte index, CANFrame *msg)
{
  byte opc = msg->data[0];

  DEBUG_PRINT(F("> event handler: index = ") << index << F(", opcode = 0x") << _HEX(msg->data[0]));
  DEBUG_PRINT(F("> event handler: length = ") << msg->len);

  unsigned int node_number = (msg->data[1] << 8 ) + msg->data[2];
  unsigned int event_number = (msg->data[3] << 8 ) + msg->data[4];
  DEBUG_PRINT(F("> NN = ") << node_number << F(", EN = ") << event_number);
  DEBUG_PRINT(F("> op_code = ") << opc);

   // Experimental code to display a message index on the event_number.
   if (event_number > nonEvent) {
     switch (opc) {

      case OPC_ACON:
      case OPC_ASON:
     
      displayError(event_number-nonEvent,0,0);
      break;

      case OPC_ACOF:
      case OPC_ASOF:
      
      displayError(blankError,0,0);
      break;
      
     }   
   } else {
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
    } // switch(opc)
  }

}

// Getting an opcode array is done when calling SetFrameHandler.
void framehandler(CANFrame *msg) {

  // as an example, format and display the received frame

#if DEBUG
  Serial << F("[ ") << (msg->id & 0x7f) << F("] [") << msg->len << F("] [");
  if ( msg->len > 0) {
    for (byte d = 0; d < msg->len; d++) {
      Serial << F(" 0x") << _HEX(msg->data[d]);
    }
  Serial << F(" ]") << endl;
  }

  if (nopcodes > 0) {
    Serial << F("Opcodes [ ");
    for(byte i = 0;  i < nopcodes; i++)
    {
       Serial << F(" 0x") << _HEX(opcodes[i]);
    }
    Serial << F(" ]") << endl;
  }
#endif

  if (nopcodes > 0) {
      DEBUG_PRINT(F("Message received with Opcode [ 0x") << _HEX(msg->data[0]) << F(" ]") );
    for(byte i = 0;  i < nopcodes; i++)
    {
       if ( msg->data[0] == opcodes[i]) {
           DEBUG_PRINT(F("Message recognised with Opcode [ 0x") << _HEX(opcodes[i]) << F(" ]") );
     // This will be executed if the code matches.
           //messagehandler(msg); Not implemented yet
           break;       
        }
    }
  }
  return;

}

void printConfig(void)
{
  // code version
  Serial << F("> code version = ") << VER_MAJ << VER_MIN << F(" beta ") << VER_BETA << endl;
  Serial << F("> compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(", compiler ver = ") << __cplusplus << endl;

  // copyright
  Serial << F("> © Martin Da Costa (MERG M6223) 2021") << endl;
  Serial << F("> © Duncan Greenwood (MERG M5767) 2021") << endl;
  Serial << F("> © John Fletcher (MERG M6777) 2021") << endl;
  Serial << F("> © Sven Rosvall (MERG M3777) 2021") << endl;
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
