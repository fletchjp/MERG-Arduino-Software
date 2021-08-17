//////////////////////////////////////////////////////////////////////////////////
// CANTEXTL
// This is a test file for sending and receiving long messages.
// I have still to figure out how to do this.
/**************************************************************************************
  Version 1a beta 1
  Initial ideas.
*************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////
// CANTEXT2
// I am going to build CANTEXT again on top of the version changed by
// Martin Da Costa.
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CANTEXT - CBUS Text transfer
// Developing CAN1in1out for the support of a text display with exchange of messages over CBUS.
//////////////////////////////////////////////////////////////////////////////////
// CBUS_1IN1OUT_2c

// This version uses the IO ABstraction library to handle input output and task 
// scheduling. This now controls the input and output for events.
// The LED is now toggled when the incoming event is detected.
//
// It does not control the button and LEDs for the SLim/FLiM change for CBUS.
//
// https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/
//
// The module button and LED have been changed to use IO_Abstraction structures.
// These use the same pin numbers.
// The operation to test the module button has been moved into a function checkSwitch
// which is not called directly. Instead it is called by the taskmanager to set it up 
// and specify how often it will be called.
//
// John Fletcher (M6777) 
//
//////////////////////////////////////////////////////////////////////////////////

///
//
// Modified by Martin Da Costa (M6223) and John Fletcher (M6777)

/*
  Copyright (C) Duncan Greenwood 2017 (duncan_greenwood@hotmail.com)
  Copyright (C) Martin Da Costa 2020 John Fletcher 2021

  
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
// Pin Use map UNO:
// Digital pin 2 (PWM)    Interupt CAN
// Digital pin 3 (PWM)    Module Switch
// Digital pin 4 (PWM)    Green SLiM LED and Module LED
// Digital pin 5 (PWM)    Yellow FLiM LED
// Digital pin 6 (PWM)    CBUS Switch
// Digital pin 7 (PWM)    Module Sounder (now implemented in code - adds 750 bytes of code)
// Digital pin 8 (PWM)    Not Used
// Digital pin 9 (PWM)    Not Used
// Digital pin 10 (SS)    CS    CAN
// Digital pin 11 (MOSI)  SI    CAN
// Digital pin 12 (MISO)  SO    CAN
// Digital pin 13 (SCK)   Sck   CAN

// Digital / Analog pin 0     Not Used
// Digital / Analog pin 1     Not Used
// Digital / Analog pin 2     Not Used
// Digital / Analog pin 3     Not Used
// Digital pin 18 / Analog pin 4     I2C SDA 
// Digital pin 19 / Analog pin 5     I2C SCL
// Digital pin 20 / Analog pin 6  NANO only
// Digital pin 21 / Analog pin 7  NANO only
// Note that pins 18 and 19 are duplicated on a UNO but not on a NANO
//////////////////////////////////////////////////////////////////////////

// IoAbstraction libraries
#include <IoAbstraction.h>
#include <TaskManagerIO.h>
// IoAbstraction reference to the arduino pins.
IoAbstractionRef arduinoPins = ioUsingArduino();

//////////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANT - This depends on the choice of hardware used to connect to CBUS.
//////////////////////////////////////////////////////////////////////////////////////////////
// Set this to 1 for CANBUS modules with 8 Mhz Crystal
// Set this to 0 for Sparkfun CANBUS shields.
#define CANBUS8MHZ 0
//#define CANBUS8MHZ 1 // set to 0 for CANBUS module with 16MHz crystal
//////////////////////////////////////////////////////////////////////////////////////////////
// These are more things which need to be set.
#define DEBUG         1 // set to 0 for no debug messages, 1 for messages to console
#define LCD_DISPLAY   1 // set to 0 if 4x20 char LCD display is not present
#define MERG_DISPLAY  1 // set to 0 to save memory by leaving this out.

// Set GROVE 1 for a GROVE switch which is HIGH when pressed, otherwise 0
#define GROVE 1
//////////////////////////////////////////////////////////////////////////////////////////////
// 3rd party libraries
// Streaming   -- C++ stream style output, v5, (http://arduiniana.org/libraries/streaming/)
#include <Streaming.h>
//////////////////////////////////////////////////////////////////////////////////////////////
// CBUS library header files
#include <CBUS2515.h>               // CAN controller and CBUS class
#include <CBUSswitch.h>             // pushbutton switch
#include <CBUSLED.h>                // CBUS LEDs
#include <CBUSconfig.h>             // module configuration
#include <cbusdefs.h>               // MERG CBUS constants
#include <CBUSParams.h>

//////////////////////////////////////////////////////////////////////////////////////////////
#if LCD_DISPLAY
#define VERSION 2.1
/* libraries for LCD display module */
#include <Wire.h>    // Library for I2C comminications for display
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h >
// The hd44780 library figures out what to do.
// This corresponds to a display with an I2C expander pack.
// I could provide alternatives for other hardware.
hd44780_I2Cexp display(0x27);
//////////////////////////////////////////////////////////////////////////////////////////////
volatile unsigned long previousTurnon    = 0;
volatile unsigned long alight            = 10000;
volatile boolean       barGraphicsLoaded = false;
volatile boolean       showingSpeeds     = false;
// Function prototypes
void initialiseDisplay();
#if MERG_DISPLAY
void displayMergLogo();
#endif
void displayVersion();
void displaySetup();
#endif
//////////////////////////////////////////////////////////////////////////////////////////////
// constants
const byte VER_MAJ = 1;                  // code major version
const char VER_MIN = 'a';                // code minor version
const byte VER_BETA = 1;                 // code beta sub-version
const byte MODULE_ID = 99;               // CBUS module type

const byte LED_GRN = 4;                  // CBUS green SLiM LED pin
const byte LED_YLW = 5;                  // CBUS yellow FLiM LED pin
const byte SWITCH0 = 6;                  // CBUS push button switch pin

// Module pins
const byte MODULE_LED_PIN    = 4;        // Module LED Pin using Green Pin
const byte MODULE_SWITCH_PIN = 3;        // Module Switch Pin
const byte MODULE_SOUNDER    = 7;        // Module buzzer pin

// Adding a buzzer output for taught event
#define buzzer MODULE_SOUNDER  // Just use MODULE_SOUNDER
const byte BUZZER_TONE = 1000;   // Set the tone for the buzzer

//////////////////////////////////////////////////////////////////////////////////////////////
//CBUS pins
const byte CAN_INT_PIN = 2;  // Only pin 2 and 3 support interrupts
const byte CAN_CS_PIN = 10;
//const byte CAN_SI_PIN = 11;  // Cannot be changed
//const byte CAN_SO_PIN = 12;  // Cannot be changed
//const byte CAN_SCK_PIN = 13;  // Cannot be changed

// CBUS objects
CBUS2515 CBUS;                      // CBUS object
CBUSConfig config;                  // configuration object
//CBUSLED ledGrn, ledYlw;             // LED objects
//CBUSSwitch pb_switch;               // switch object

//////////////////////////////////////////////////////////////////////////////////////////////
// module objects replaced by IO_Abstraction devices.
//CBUSSwitch moduleSwitch;            // an example switch as input
//CBUSLED moduleLED;                  // an example LED as output

//////////////////////////////////////////////////////////////////////////////////////////////
// CBUS module parameters
// unsigned char params[21]; not used here.

// module name
const unsigned char mname[7] PROGMEM = { 'T', 'e', 'x', 't', 'L', ' ', ' ' };

// forward function declarations
void eventhandler(byte index, byte opc);
void framehandler(CANFrame *msg);
void checkSwitch(); // Forward declaration of the task function
void toggle();      // Forward declaration of toggle task
// Opcodes to be detected by framehandler
byte nopcodes = 8;
const byte opcodes[] PROGMEM = {OPC_ACON, OPC_ACOF, OPC_ACON1, OPC_ACOF1, OPC_ACON2, OPC_ACOF2, OPC_ACON3, OPC_ACOF3 };

//////////////////////////////////////////////////////////////////////////////////////////////
// Code finding out what message is to be displayed and holding the message texts.
// Index values for incoming event processing

// Index values for errors
enum errorStates {
  blankError,
  noError,
  emergencyStop,
  CANbusError,
  invalidError
};

enum eventTypes {
  nonEvent,
  testEvent,
  emergencyEvent,
  errorEvent,
  dataEvent,
  longEvent,
  invalidEvent
};


// This is following the methods in EzyBus_master to provide error messages.
// These have been limited to 16 chars to go on an LCD 2 by 16 display.
// blank_string is used to cancel an error message.
const char blank_string[]   PROGMEM  = "                ";
const char error_string_0[] PROGMEM  = "no error";
const char error_string_1[] PROGMEM  = "Emergency Stop";
const char error_string_2[] PROGMEM  = "CANbus error";
const char error_string_3[] PROGMEM  = "invalid error";

const char* const error_string_table[] PROGMEM = {
  blank_string, error_string_0, error_string_1, error_string_2, error_string_3
};

const byte MAX_ERROR_NO = 4;
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

// Functions to print error message with or without end of line.
void serialPrintError(int i)
{
 getErrorMessage(i);Serial.print(error_buffer); 
}
void serialPrintErrorln(int i)
{
 getErrorMessage(i);Serial.println(error_buffer);
}


// Forward declaration of variable used to detect switch change.
byte previous_switch;
byte ledOn;

//
// here we globally store the task ID of our repeating task, we need this to cancel it later.
//
int taskId = TASKMGR_INVALIDID; // Set to this value so that it won't get cancelled before it exists!


//
///  setup CBUS - runs once at power on called from setup()
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

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);
  // This will only process the defined opcodes.
  CBUS.setFrameHandler(framehandler, opcodes, nopcodes);
  //Serial << F("> Exiting setFrameHandler") << endl;

 // Retained for now.
 // set LED and switch pins and assign to CBUS
 /*
  ledGrn.setPin(LED_GRN);
  ledYlw.setPin(LED_YLW);
  CBUS.setLEDs(ledGrn, ledYlw);
  CBUS.setSwitch(pb_switch);

  // set CBUS LEDs to indicate mode
  CBUS.indicateMode(config.FLiM);
*/

  // configure and start CAN bus and CBUS message processing
  CBUS.setNumBuffers(2);         // more buffers = more memory used, fewer = less
  //Serial << F("> Exiting setNumBuffers") << endl;
#if CANBUS8MHZ
  CBUS.setOscFreq(8000000UL);   // select the crystal frequency of the CAN module to 8MHz
#else
  CBUS.setOscFreq(16000000UL);   // select the crystal frequency of the CAN module to 16Mhz
#endif
  //CBUS.setOscFreq(CAN_OSC_FREQ);   // select the crystal frequency of the CAN module
  //CBUS.setPins(10, 2);
  CBUS.setPins(CAN_CS_PIN, CAN_INT_PIN); // select pins for CAN bus CE and interrupt connections
  //Serial << F("> Exiting setPins") << endl;
  CBUS.begin();
  //Serial << F("Exiting setupCBUS") << endl;
}

//
/// setup - runs once at power on
//

void setup() {

  Serial.begin (115200);
  Serial << endl << endl << F("> ** CANTEXT2 v2b beta 2 ** ") << __FILE__ << endl;

  setupCBUS();
//#if DEBUG
  //Serial << F("after setupCBUS()") << endl;
//#endif
// Initialise displays
#if LCD_DISPLAY
  initialiseDisplay();
  delay(2000);
  #if MERG_DISPLAY

  displayMergLogo();
  delay(2000);
  //setupBarGraph();
  displayVersion();
  #endif
 // Code needed here for whatever set up is needed.
  displaySetup(); 
#endif
  // initialise CBUS switch
  /*
#if GROVE
  pb_switch.setPin(SWITCH0, HIGH);
  Serial << F("> Switch set to go HIGH when pressed") << endl;
#else
  pb_switch.setPin(SWITCH0, LOW);
  Serial << F("> Switch set to go LOW when pressed") << endl;
#endif

  // module reset - if switch is depressed at startup and module is in SLiM mode
  pb_switch.run();

  if (pb_switch.isPressed() && !config.FLiM) {
    Serial << F("> switch was pressed at startup in SLiM mode") << endl;
    config.resetModule(ledGrn, ledYlw, pb_switch);
  }
*/

#if GROVE
  ioDevicePinMode(arduinoPins, MODULE_SWITCH_PIN, LOW);
  Serial << F("> Module Switch ") << MODULE_SWITCH_PIN << F(" set to go HIGH when pressed") << endl;
#else
  // configure the module switch, active low
  ioDevicePinMode(arduinoPins, MODULE_SWITCH_PIN, INPUT_PULLUP);
  Serial << F("> Module Switch ") << MODULE_SWITCH_PIN << F(" set to go LOW when pressed") << endl;
#endif
  // Capture the initial state of the switch.
  previous_switch = ioDeviceDigitalReadS(arduinoPins, MODULE_SWITCH_PIN);
  
  // configure the module LED, attached to pin via a 1K resistor
  ioDevicePinMode(arduinoPins, MODULE_LED_PIN, OUTPUT);
  // Set initial state of the LED.
  ioDeviceDigitalWrite(arduinoPins, MODULE_LED_PIN, LOW);

  // Schedule task to run every 250 milliseconds.
  taskManager.scheduleFixedRate(250, checkSwitch);
  // add additional taskManager functions here

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

  // Run IO_Abstraction tasks.
  // This replaces actions taken here in the previous version.
  taskManager.runLoop();

  //
  /// bottom of loop()
  //
}

// This is the call back method that gets called when the incoming event is detected.
// There is a way to have a task with parameters which could be e.g. the pin No.
void toggle() {
  // now we write to the device, the 'S' version of the method automatically syncs.
  ioDeviceDigitalWriteS(arduinoPins, MODULE_LED_PIN, ledOn);

  ledOn = !ledOn; // toggle the LED state.
}

class Toggle : public Executable {
   private:
       byte led_pin;
       byte led_on;
   public:
      int taskId;    
      Toggle(byte led) : led_pin(led), led_on(HIGH) { taskId = TASKMGR_INVALIDID; }
      void exec() override {
         ioDeviceDigitalWriteS(arduinoPins, led_pin, ledOn);
         ledOn = !ledOn; // toggle the LED state.
      }
};

Toggle module_toggle(MODULE_LED_PIN);

// Check the switch and act if it has changed.
// This code has been moved from the loop()
void checkSwitch()
{
  // Use IO_Abstraction method to read the switch pin.
  byte new_switch = ioDeviceDigitalReadS(arduinoPins, MODULE_SWITCH_PIN);
  bool button_has_changed = (new_switch != previous_switch);
  // NOTE: new_switch is 0 if the switch is pressed and 1 otherwise.
  if(button_has_changed) 
  {
      previous_switch = new_switch;
#if DEBUG
      Serial << F(">Button state change detected ")<<new_switch << endl;
      if (!new_switch) {
        Serial << F("> button pressed") << endl;
      } else {
        Serial << F("> button released") << endl;        
      }
#endif
    byte opCode = (!new_switch ? OPC_ACON : OPC_ACOF);
    unsigned int eventNo = 1; // Converted to 2 bytes for safety.
    sendEvent(opCode,eventNo);
  }
}

// Send an event with information about this node.
bool sendEvent(byte opCode,unsigned int eventNo)
{
    CANFrame msg;
    msg.id = config.CANID;
    msg.len = 5;
    msg.data[0] = opCode;
    msg.data[1] = highByte(config.nodeNum);
    msg.data[2] = lowByte(config.nodeNum);
    msg.data[3] = highByte(eventNo); // event number (EN) could be > 255
    msg.data[4] = lowByte(eventNo); 
    msg.ext = false;
    msg.rtr = false;

    bool res = CBUS.sendMessage(&msg);
#if DEBUG
    if (res) {
      Serial << F("> sent CBUS message with opCode [ 0x") << _HEX(opCode) << F(" ] and event No ") << eventNo << endl;
    } else {
      Serial << F("> error sending CBUS event with opcode [ 0x") <<  _HEX(opCode) << F(" ]") << endl;
    }
#endif
#if LCD_DISPLAY
    if (!res) displayError(CANbusError,0,3);
#endif
    return res;
}

//
/// user-defined event processing function
/// called from the CBUS library when a learned event is received
/// it receives the event table index and the CAN frame
//

void eventhandler(byte index, CANFrame *msg) {

  // as an example, control an LED if the first EV equals 1

  Serial << F("> event handler: index = ") << index << F(", opcode = 0x") << _HEX(msg->data[0]) << endl;
  byte len = msg->len;
  Serial << F("> event handler: length = ") << len << endl;

  ////////////////////////////////////////////////////////////////////////////////////////////
  // Processing of all learned events
  // read the value of the first event variable (EV) associated with this learned event
  byte ev = 1;
  byte evval = config.getEventEVval(index, ev);
  byte op_code = msg->data[0];
  unsigned int node_number = (msg->data[1] << 8 ) + msg->data[2];
  unsigned int event_number = (msg->data[3] << 8 ) + msg->data[4];

  // set the LED according to the opcode of the received event, if the first EV equals 1
  // we use the blink() method of the LED object as an example

  Serial << F("> NN = ") << node_number << F(", EN = ") << event_number << endl;
  Serial << F("> op_code = ") << op_code << endl;
  Serial << F("> EV1 = ") << evval << endl;

#define LOOP_ON_OP_CODES 1
#if LOOP_ON_OP_CODES
  switch (op_code)
  {
     // Event on and off
     // Handle these together based on event no.
     case OPC_ACON:
     case OPC_ACOF:
     // Event numbers are tested against the enum of event types
     if (event_number == testEvent) {
     // set the LED according to the opcode of the received event, if the first EV equals 1
     // we use the blink() method of the LED object as an example
       if (evval == 1)
       {
         if (op_code == OPC_ACON) {
           Serial << F("> switching the LED to blink") << endl;
           module_toggle.taskId = taskManager.scheduleFixedRate(200, &module_toggle);
         } else if (op_code == OPC_ACOF) {
           Serial << F("> switching the LED off") << endl;
           // Check to make sure only a valid task gets cancelled.
           if(module_toggle.taskId != TASKMGR_INVALIDID) taskManager.cancelTask(module_toggle.taskId);
           // This is left in as the task could exit with the LED on.
           ioDeviceDigitalWrite(arduinoPins, MODULE_LED_PIN, LOW);
         }
       }
     } else if (event_number == emergencyEvent) {
         if ( op_code == OPC_ACON) {
#if DEBUG
            serialPrintErrorln(emergencyStop);
#endif
#if LCD_DISPLAY
            displayError(emergencyStop,0,3);
#endif 
            tone(buzzer,BUZZER_TONE);
        } else { // op_code == OPC_ACOF to cancel the error message
#if LCD_DISPLAY
            displayError(blankError,0,3);
#endif
            noTone(buzzer);
       }
     }
     break;
     
     case OPC_ACON1:
     case OPC_ACOF1:
       // event to display other error messages as indexed.
       if (event_number == errorEvent) {
         if( op_code == OPC_ACON1) {
           // extract error index from event data
           byte error = msg->data[5];
           // Check for error in range
           if ( (error < blankError) || (error > invalidError)) error = invalidError;
#if DEBUG
           serialPrintErrorln(error);
#endif
#if LCD_DISPLAY
           displayError(error,0,3);
#endif
         } else { // op_code == OPC_ACOF1 to cancel the error message
#if LCD_DISPLAY
           displayError(blankError,0,3);
#endif
         }    

      }
      break;
      default:
      // event to display section data
      // opcodes not sorted at the moment.
      if (event_number == dataEvent) {
    
      }
  }
      
#else
  // Event numbers are now tested against the enum of event types
  if (event_number == testEvent) {
  // set the LED according to the opcode of the received event, if the first EV equals 1
  // we use the blink() method of the LED object as an example

  if (evval == 1)
   {
    if (op_code == OPC_ACON) {
      Serial << F("> switching the LED to blink") << endl;
      //ioDeviceDigitalWrite(arduinoPins, MODULE_LED_PIN, HIGH);
      //ledOn = HIGH; // Initial value for the task now in the constructor
      module_toggle.taskId = taskManager.scheduleFixedRate(200, &module_toggle);
      //taskId = taskManager.scheduleFixedRate(200, toggle);
      //taskManager.execute(toggle);
      //moduleLED.blink();
    } else if (op_code == OPC_ACOF) {
      Serial << F("> switching the LED off") << endl;
      // Check to make sure only a valid task gets cancelled.
      if(module_toggle.taskId != TASKMGR_INVALIDID) taskManager.cancelTask(module_toggle.taskId);
      // This is left in as the task could exit with the LED on.
      ioDeviceDigitalWrite(arduinoPins, MODULE_LED_PIN, LOW);
      //moduleLED.off();
    }
   }
  }
 // emergency stop and cancel
  if (event_number == emergencyEvent) {
     if( op_code == OPC_ACON) {
#if DEBUG
     serialPrintErrorln(emergencyStop);
#endif
#if LCD_DISPLAY
     displayError(emergencyStop,0,3);
#endif 
      tone(buzzer,BUZZER_TONE);
    } else { // op_code == OPC_ACOF to cancel the error message
#if LCD_DISPLAY
     displayError(blankError,0,3);
#endif
     noTone(buzzer);
    }
  } 

  // event to display other error messages as indexed.
  if (event_number == errorEvent) {
     if( op_code == OPC_ACON1) {
      // extract error index from event data
      byte error = msg->data[5];
      // Check for error in range
      if ( (error < blankError) || (error > invalidError)) error = invalidError;
#if DEBUG
     serialPrintErrorln(error);
#endif
#if LCD_DISPLAY
     displayError(error,0,3);
#endif
    } else { // op_code == OPC_ACOF1 to cancel the error message
#if LCD_DISPLAY
     displayError(blankError,0,3);
#endif
    }    
  }

  // event to display section data
  if (event_number == dataEvent) {
    
  }
#endif

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
  Serial << F("> © Martin Da Costa (MERG M6223) 2020") << endl;
  Serial << F("> © John Fletcher (MERG M6777) 2020") << endl;
#if LCD_DISPLAY
   Serial << F("> LCD display available") << endl;
#if MERG_DISPLAY
   Serial << F("> MERG display available") << endl;
#endif
#endif
#if DEBUG
   Serial << F("> Error code test noError: ") << noError << endl;
   Serial << F("> Error code text is : ");
   serialPrintErrorln(noError);
#endif
  return;
}

/// user-defined frame processing function
/// called from the CBUS library for *every* CAN frame received
/// it receives a pointer to the received CAN frame
//  Added from new version of CBUS_empty

// Converted to get an opcode array now done when calling SetFrameHandler.
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
#if DEBUG
          Serial << F("Message received with Opcode [ 0x") << _HEX(msg->data[0]) << F(" ]")<< endl;
#endif
    for(byte i = 0;  i < nopcodes; i++)
    {
       if ( msg->data[0] == opcodes[i]) {
#if DEBUG
           Serial << F("Message recognised with Opcode [ 0x") << _HEX(opcodes[i]) << F(" ]")<< endl;
#endif
     // This will be executed if the code matches.
           //messagehandler(msg); Not implemented yet
           break;       
        }
    }
  }
  return;
}

// Code for LCD Display
#if LCD_DISPLAY

void customChars(const uint8_t chars[][8])
{
  for (int i = 0; i < 8; i++)
  {
    display.createChar(i, (uint8_t *)chars[i]);
  }
}

/*
void setupBarGraph()
{
  // Creat a set of new characters
  const uint8_t bars[][8] = {
    { B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00000000 }, // 0 - 1 segment
    { B00011000, B00011000, B00011000, B00011000, B00011000, B00011000, B00011000, B00000000 }, // 1 - 2 segments
    { B00011100, B00011100, B00011100, B00011100, B00011100, B00011100, B00011100, B00000000 }, // 2 - 3 segments
    { B00011110, B00011110, B00011110, B00011110, B00011110, B00011110, B00011110, B00000000 }, // 3 - 4 segments
    { B00011111, B00011111, B00011111, B00011111, B00011111, B00011111, B00011111, B00000000 }, // 4 - 5 segments
    { B00000001, B00000011, B00000111, B00001111, B00000111, B00000011, B00000001, B00000000 }, // 5 - < arrow rev
    { B00010000, B00011000, B00011100, B00011110, B00011100, B00011000, B00010000, B00000000 }, // 6 - > arrow fwd
    { B00001110, B00011011, B00010101, B00010111, B00010101, B00011011, B00001110, B00000000 }, // 7 - (c) symbol
  };

//void setupBarGraph()
//{

  customChars(bars);

  barGraphicsLoaded = true;
}
*/

void displayLogo(const char chars[4][20])
{
  for (int i = 0; i < 4; i++)
  {
    display.setCursor(0, i);
    displayChars(chars[i], 20);
  }
}

void displayChars(const char chars[20], int count)
{
  for (int j = 0; j < count; j++)
  {
    display.write(chars[j]);
  }
}

void initialiseDisplay()
{
  display.begin(20, 4); // initialize the lcd for 20 chars 4 lines, turn on backlight
  display.display();    // turn it on
  display.clear();
}

#if MERG_DISPLAY
  // Creat a set of new characters
  const uint8_t mergLogo[][8] = {
    { B00001111, B00011111, B00011111, B00011111, B00011100, B00011100, B00011100, B00011100 }, // 0
    { B00011111, B00011111, B00011111, B00011111, B00000000, B00000000, B00000000, B00000000 }, // 1
    { B00011100, B00011100, B00011100, B00011100, B00011100, B00011100, B00011100, B00011100 }, // 2
    { B00000000, B00000000, B00000000, B00000000, B00011111, B00011111, B00011111, B00011111 }, // 3
    { B00000111, B00000111, B00000111, B00000111, B00000111, B00000111, B00000111, B00000111 }, // 4
    { B00000000, B00000000, B00000000, B00000000, B00011111, B00011111, B00011111, B00011111 }, // 5
    { B00011111, B00011111, B00011111, B00011111, B00001111, B00000111, B00000111, B00000111 }, // 6
    { B00011111, B00011111, B00011111, B00011111, B00011111, B00011111, B00011111, B00011111 }  // 7
  };

void displayMergLogo()
{

  customChars(mergLogo);

  const char chars[4][20] PROGMEM = {
  char(0), char(1), char(6), char(1), char(1), char(2), ' ', char(0), char(1), char(1), ' ', char(0), char(1), char(1), char(2), ' ', char(0), char(1), char(1), char(2),
  char(2), ' ',     char(4), ' ',     ' ',     char(2), ' ', char(2), ' ',     ' ',     ' ', char(2), ' ',     ' ',     char(2), ' ', char(2), ' ',     ' ',     ' ',
  char(7), ' ',     char(4), ' ',     ' ',     char(2), ' ', char(7), char(1), ' ',     ' ', char(7), char(1), char(1), char(6), ' ', char(7), ' ',     char(1), char(2),
  char(7), ' ',     char(4), ' ',     ' ',     char(2), ' ', char(7), char(3), char(3), ' ', char(7), ' ',     ' ',     char(4), ' ', char(7), char(3), char(3), char(2)
  };
  displayLogo(chars);

  delay(2000);
}


void displayVersion()
{
  // Clear the buffer.
  display.clear();

  display.clear();
  display.setCursor(3, 0);
  display.write("CANTEXT v");
  display.print(VERSION);
  display.setCursor(0, 1);
  //display.write(char(7));
  display.setCursor(0, 1);
  display.write("Duncan Greenwood &");
  display.setCursor(0, 2);
  display.write("John Fletcher");
  delay(2000);

}
#endif

void displaySetup(){
  display.clear();
  display.setCursor(0, 0);
  display.write("MESSAGES");
  displayError(noError,0,3);
}

void displayError(int i,byte x,byte y)
{
  getErrorMessage(i);
  display.setCursor(x, y);
  display.write("E: ");
  display.write(error_buffer);
}


#endif



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

        sprintf(msgstr, "  max events = %d, EVs per event = %d, bytes per event = %d", config.EE_MAX_EVENTS, config.EE_NUM_EVS, config.EE_BYTES_PER_EVENT);
        Serial << msgstr << endl;

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
          sprintf(msgstr, " - %02d : %3hd | 0x%02hx", j, config.readNV(j), config.readNV(j));
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
        // Serial << F("> unknown command ") << c << endl;
        break;
    }
  }
}
