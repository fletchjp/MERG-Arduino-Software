/// @file CANTOMPINZ.ino
/// @brief This has experiments on an I2C connected Keypad
///
/// This runs on the Totem MiniLab.
///
/// NOTE: Pins for I2C are A5 (SCL) A4 (SDA) (NOT documented by Totem)
/// 
///////////////////////////////////////////////////////////////////////////////////
// CANTOTEMPINZ
// This is to do some experiments on I2C Keypad
// To do this I have had to turn off the long messages and framehandler.

// Version 1b beta 1
// Modification to start to use IoAbstraction and TaskManagerIO
// as has been done in CANCMDDC in CANCMDDC2
// Version 1b beta 2 Adding code to send text change messages.
// Version 1b beta 3 Adding code for framehandler.
// Version 1b beta 4 Correct bugs inherited from CANmINmOUT event code.
// Version 1b beta 5 Add Sven's modifications to CANmINmOUT event code.
// Version 1b beta 6 Correct lack of return value from sendEvent.
// Version 1b beta 7 Use DEBUG_PRINT and process send fail in processSwitches.
///////////////////////////////////////////////////////////////////////////////////
// Version 1c beta 1 Explore triggering a TaskManagerIO event from within a task.
// Version 1c beta 2 Convert event to criticalEvent and get rid of previous code.
// Version 1c beta 3 Expand criticalEvent processing to include an opcode.
// Version 1c beta 4 Expand criticalEvent processing to check that an opcode is available.
// Version 1c beta 5 Make processSerialInput a task.
// Version 1c beta 6 Use PROGMEM to reduce memory use for fixed data.
///////////////////////////////////////////////////////////////////////////////////
// Version 2a beta 1 Bring in code for buttons from CAN1602BUT
// Version 2a beta 2 Change button detection values for the new hardware.
// Version 2a beta 3 Initialise button = 0.
// Version 2a beta 4 IDEA - test whether CBUS is available, but how?
//                   I would like to be able to check so that I can be sure if NVs are available.
//                   I would also like to prevent send event failures.
//                   It turns out there is not an easy way to do that.
//                   I will have to define something such as NO_CBUS for testing.
///////////////////////////////////////////////////////////////////////////////////
// Version 3a beta 1 Bring in code for long messages from CANTEXTL
//                   Initial ideas. Inactive code added for receiving a message.
//                   I have not figured out the code for sending one.
//                   #define CBUS_LONG_MESSAGE to activate the code.
//                   Code now activated.
//                   Assign a sending thread and listen on other threads.
//                   This version now runs successfully off line from CBUS.
// Version 3a beta 2 Start on line testing of long messages.
// Version 3a beta 3 Add code to check on long message length
//                   Suspend critical event sending.
// Version 3a beta 4 Add arguments to long event failure message
// Version 3a beta 5 Take out event send (temporary)
//                   Change output buffer to make it global (permanent)
// Version 3a beta 6 Correct error in long message handler.
// Version 3a beta 7 Add bus_debug code
// Version 3a beta 8 Set TX buffers to 4
// Version 3a beta 9 Change arguments for longmessagehandler 
//                   to match new release of libraries.
// Version 3a beta 10 Minor changes
///////////////////////////////////////////////////////////////////////////////////
// Version 4y beta 1
// This version defines calibration for the pins after header files.
// This solves the compilation problem found previously.
// Version 4y beta 2 Reorder putting headers first.
///////////////////////////////////////////////////////////////////////////////////
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
// This did cause a failure and does not any longer since I use an anonymous namespace
//#define FAILURE

#include <Wire.h>
// IoAbstraction libraries
#include <IoAbstraction.h>
#include <IoAbstractionWire.h>
#include <DfRobotInputAbstraction.h>
#include <TaskManagerIO.h>
#include <DeviceEvents.h>
#include <KeyboardManager.h>

// This uses the default settings for analog ranges.
//IoAbstractionRef dfRobotKeys = inputFromDfRobotShield();

// 3rd party libraries
#include <Streaming.h>
#include <Bounce2.h>

// This can be edited to get a better answer
// Not used in this version - the code is in this file.
//#include "MyButtons.h"

#ifndef FAILURE
// CBUS library header files
#include <CBUS2515.h>            // CAN controller and CBUS class
#include "LEDControl.h"          // CBUS LEDs
#include <CBUSconfig.h>          // module configuration
#include <cbusdefs.h>            // MERG CBUS constants
#include <CBUSParams.h>
#endif
////////////////////////////////////////////////////////////////////////////////////////
// New policy to bring ALL headers above anything else at all.
// Maybe that is why they are called headers.
// The only exception would be defines affecting choices in a header.
////////////////////////////////////////////////////////////////////////////////////////
//#define CBUS_LONG_MESSAGE

#define DEBUG 0     // set to 0 for no serial debug

#if DEBUG
#define DEBUG_PRINT(S) Serial << S << endl
#else
#define DEBUG_PRINT(S)
#endif

#define ANALOG_IN_PIN A0

// Variables for buttons
//int x;
//int prevx = 0;
//int range;
//int prevrange = 0;
// Use these for the CBUS outputs
int button = 0;
int prevbutton = 0;

// These keys are not in the same order. LEFT UP DOWN RIGHT SELECT
const PROGMEM DfRobotAnalogRanges MyAvrRanges { 0.02F, 0.08F, 0.15F, 0.31F, 0.4F};

namespace {
/// This works in a header file and not in the Arduino ino file BEFORE other headers.
  IoAbstractionRef inputFromMyShield(uint8_t pin = A0, AnalogDevice* device = nullptr) {
    device = new ArduinoAnalogDevice();
    return new DfRobotInputAbstraction(&MyAvrRanges, pin, device);
  }
}
IoAbstractionRef dfRobotKeys = inputFromMyShield();
/// Declare that the first 100 pins are reserved for the Arduino.
MultiIoAbstractionRef multiIo = multiIoExpander(100);
/// four rows
const byte ROWS = 4;
/// three columns
const byte COLS = 3; 
/// define the symbols on the buttons of the keypads
/// PROGMEM is important.
const char layout[] PROGMEM = "123456789*0#"; // Chars have to be in a string.
//  '1','2','3','4','5','6','7','8','9','*','0','#'
//};
/// These are in order of Keypad pins from 1 to 7.
/// Pin 1 is on the left with the pad face up.
byte rowPins[ROWS] = {100, 101, 102, 103}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {104, 105, 106}; //connect to the column pinouts of the keypad
/// This seems fussy. ROWS and COLS will not work here.
uint8_t rows = ROWS;
uint8_t cols = COLS;
/// Set up the keyLayout
KeyboardLayout keyLayout(rows, cols, layout);
/// We need a keyboard manager class too
MatrixKeyboardManager keyboard;

/// We need a class that extends from KeyboardListener. This gets notified when
/// there are changes in the keyboard state.
class MyKeyboardListener : public KeyboardListener {
public:
    void keyPressed(char key, bool held) override {
        Serial.print("Key ");
        Serial.print(key);
        Serial.print(" is pressed, held = ");
        Serial.println(held);
    }

    void keyReleased(char key) override {
        Serial.print("Released ");
        Serial.println(key);
    }
} myListener;

#ifdef FAILURE
//CANTOTEMPINY:606:31: error: 'CANFrame' has not been declared
// void eventhandler(byte index, CANFrame *msg)
// This is although it IS declared in CBUS.h which is included from CBUS2515.h
// The problem goes away if the function inputFromMyShield is put into an anonymous namespace!
//  
// CBUS library header files
//#include <CBUS.h>
#include <CBUS2515.h>            // CAN controller and CBUS class
#include "LEDControl.h"          // CBUS LEDs
#include <CBUSconfig.h>          // module configuration
#include <cbusdefs.h>            // MERG CBUS constants
#include <CBUSParams.h>
#endif


////////////DEFINE MODULE/////////////////////////////////////////////////

// module name
const unsigned char mname[7] PROGMEM = { 'T', 'O', 'T', 'E', 'M', ' ', ' ' };

// forward function declarations
void eventhandler(byte index, byte opc);
//void framehandler(CANFrame *msg);

// Set opcodes for polling events
const byte nopcodes = 9;
const byte opcodes[] PROGMEM = {OPC_ACON, OPC_ACOF, OPC_ARON, OPC_AROF, OPC_ASON, OPC_ASOF, OPC_AREQ, OPC_ASRQ, OPC_CANID }; 

// constants
const byte VER_MAJ = 4;         // code major version
const char VER_MIN = 'y';       // code minor version
const byte VER_BETA = 2;       // code beta sub-version
const byte MODULE_ID = 99;      // CBUS module type

const unsigned long CAN_OSC_FREQ = 8000000;     // Oscillator frequency on the CAN2515 board

#define NUM_LEDS 2              // How many LEDs are there?
#define NUM_SWITCHES 2          // How many switchs are there?

//Module pins available for use are Pins 3 - 9 and A0 - A5
const byte LED[NUM_LEDS] PROGMEM = {8, 7};            // LED pin connections through typ. 1K8 resistor
const byte SWITCH[NUM_SWITCHES] PROGMEM = {9, 6};     // Module Switch takes input to 0V.

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
#ifdef CBUS_LONG_MESSAGE
// The Ardunio CBUS library does now support this.
// create an additional object at the top of the sketch:
CBUSLongMessage cbus_long_message(&CBUS);   // CBUS long message object
#endif

// Event Nos for different events to be sent
// enum base changed to avoid other events.
// These are ideas at the moment.
// enum made into a class
enum class EventNo : unsigned int {
  nonEvent = 100,  // not used
  testEvent,
  emergencyEvent,
  sendFailureEvent, // was errorEvent
  dataEvent,
  requestEvent,
  invalidEvent
};

#ifdef CBUS_LONG_MESSAGE
///////////////////////////////////////////////////////////////////////////////////////////////
// Long message setting up.
///////////////////////////////////////////////////////////////////////////////////////////////
const byte stream_id = 14; // Sending stream number - not the same as the ones to be read.
// a list of stream IDs to subscribe to (this ID is defined by the sender):
byte stream_ids[] = {11, 12, 13}; // These are the ones which this module will read.
// Long message output buffer which must be global otherwise it goes out of scope.
const unsigned int output_buffer_size = 32;
char long_message_output_buffer[output_buffer_size];
// a buffer for the message fragments to be assembled into
// either sized to the maximum message length, or as much as you can afford
const unsigned int input_buffer_size = 32;
byte long_message_data[input_buffer_size];
// create a handler function to receive completed long messages:
void longmessagehandler(void *fragment, const unsigned int fragment_len, const byte stream_id, const byte status);
const byte delay_in_ms_between_messages = 50;
#endif

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
  CBUS.setName((unsigned char *)mname);

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);
  // This will only process the defined opcodes.
  //CBUS.setFrameHandler(framehandler, (byte *)opcodes, nopcodes);

#ifdef CBUS_LONG_MESSAGE
  //DEBUG_PRINT(F("> about to call to subscribe") );
  // subscribe to long messages and register handler
  cbus_long_message.subscribe(stream_ids, (sizeof(stream_ids) / sizeof(byte)), long_message_data, input_buffer_size, longmessagehandler);
  // this method throttles the transmission so that it doesn't overwhelm the bus:
  cbus_long_message.setDelay(delay_in_ms_between_messages);
  cbus_long_message.setTimeout(1000);
  //DEBUG_PRINT(F("> CBUS_LONG_MESSAGE subscribed") );
#endif

  // configure and start CAN bus and CBUS message processing
  CBUS.setOscFreq(CAN_OSC_FREQ);   // select the crystal frequency of the CAN module
  CBUS.setPins(CAN_CS_PIN, CAN_INT_PIN);           // select pins for CAN bus CE and interrupt connections
  CBUS.setNumBuffers(2,2); // Set TX buffers. Default for RX is 4.
  CBUS.begin();
}

void bus_debug(void) {

  Serial << F("> transmit buffer sizes = ") << CBUS.canp->transmitBufferSize(0) << F(", ") << CBUS.canp->transmitBufferSize(1) << F(", ") << CBUS.canp->transmitBufferSize(2) << endl;
  Serial << F("> transmit buffer counts = ") << CBUS.canp->transmitBufferCount(0) << F(", ") << CBUS.canp->transmitBufferCount(1) << F(", ") << CBUS.canp->transmitBufferCount(2) << endl;
  Serial << F("> transmit buffer peaks = ") << CBUS.canp->transmitBufferPeakCount(0) << F(", ") << CBUS.canp->transmitBufferPeakCount(1) << F(", ") << CBUS.canp->transmitBufferPeakCount(2) << endl;
  Serial << F("> receive buffer = ") << CBUS.canp->receiveBufferSize() << F(", ") << CBUS.canp->receiveBufferCount() << F(", ") << CBUS.canp->receiveBufferPeakCount() << endl;
  Serial << F("> error flag register = ") << CBUS.canp->errorFlagRegister() << endl;
  Serial << endl;

  return;
}

void logKeyPressed(int pin,const char* whichKey, bool heldDown) {
    Serial.print("Key ");
    Serial.print(whichKey);
//    Serial.print(dfRobotKeys->readPort());
    Serial.print(analogRead(A0));
    //lcd.setCursor(10,1);
    //lcd.print (whichKey);
    Serial.println(heldDown ? " Held" : " Pressed");
    button = pin;
}


void setupSwitches()
{
    // initialise the switches component with the DfRobot shield as the input method.
    // Note that switches is the sole instance of SwitchInput
    switches.initialise(dfRobotKeys, false); // df robot is always false for 2nd parameter.

    // now we add the switches, each one just logs the key press, the last parameter to addSwitch
    // is the repeat frequency is optional, when not set it implies not repeating.
    switches.addSwitch(DF_KEY_DOWN, [](pinid_t pin, bool held) { logKeyPressed(pin,"DOWN  ", held);}, 20);
    switches.addSwitch(DF_KEY_UP, [](pinid_t pin, bool held) { logKeyPressed(pin,"UP    ", held);}, 20);
    // Note that the next two are switched around.
    switches.addSwitch(DF_KEY_LEFT, [](pinid_t pin, bool held) { logKeyPressed(pin,"RIGHT ", held);}, 20);
    switches.addSwitch(DF_KEY_RIGHT, [](pinid_t pin, bool held) { logKeyPressed(pin,"LEFT  ", held);}, 20);
    switches.addSwitch(DF_KEY_SELECT, [](pinid_t pin, bool held) { logKeyPressed(pin,"SELECT ", held);}, 20);
    //switches.onRelease(DF_KEY_RIGHT, [](pinid_t /*pin*/, bool) { Serial.println("RIGHT has been released");});
    
    //switches.addSwitchListener(DF_KEY_SELECT, &selectKeyListener);
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

/**
 * An event that extends BaseEvent allows for event driven programming, either notified by polling, interrupt or
 * another thread. There are two important methods that you need to implement, timeOfNextCheck that allows for polling
 * events, where you do the check in that method and trigger the event calling setTriggered(). Alternatively, like
 * this event, another thread or interrupt can trigger the event, in which case you call markTriggeredAndNotify() which
 * wakes up task manager. When the event is triggered, is exec() method will be called.
 */

void setup()
{
  Serial.begin (115200);
  Serial << endl << endl << F("> ** CANTOTEMPINZ ** ") << __FILE__ << endl;

  Wire.begin();
  /// Add 10 pins from 100 up.
  multiIoAddExpander(multiIo, ioFrom8574(0x20), 10);
    // Converted to copy the arrays.
    for (byte i = 0; i < ROWS; i++)
      keyLayout.setRowPin(i, rowPins[i]);
    for (byte i = 0; i < COLS; i++)
      keyLayout.setColPin(i, colPins[i]);

    /// create the keyboard mapped to arduino pins and with the layout chosen above.
    /// It will callback our listener
    keyboard.initialise(multiIo, &keyLayout, &myListener);
    /// create the keyboard mapped to arduino pins and with the layout chosen above.
    /// It will callback our listener
    keyboard.initialise(multiIo, &keyLayout, &myListener);

    /// start repeating at 850 millis then repeat every 350ms
    keyboard.setRepeatKeyMillis(850, 350);

    Serial.println("Keyboard is initialised!");

  setupCBUS();
  setupModule();
  setupSwitches();

  // Schedule tasks to run every 250 milliseconds.
  taskManager.scheduleFixedRate(250, runLEDs);
  taskManager.scheduleFixedRate(250, processSwitches);
  //taskManager.scheduleFixedRate(250, checkA0);
  taskManager.scheduleFixedRate(250, processSerialInput);
  taskManager.scheduleFixedRate(250, processButtons);
  //taskManager.registerEvent(&criticalEvent);

  // end of setup
  DEBUG_PRINT(F("> ready") << endl);
}

void loop()
{
  // do CBUS message, switch and LED processing
  CBUS.process();

#ifdef CBUS_LONG_MESSAGE
  cbus_long_message.process();
#endif
 
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
#ifdef CBUS_LONG_MESSAGE
   //char long_message_output_buffer[output_buffer_size]; assigned globally
   int string_length; // Returned by snprintf. This may exceed the actual length.
   unsigned int message_length;
#endif
   if (button != prevbutton) {
      DEBUG_PRINT(F("Button ") << button << F(" changed")); 
      opCode = OPC_ACON;
      sendEvent(opCode, button + NUM_SWITCHES);
#ifdef CBUS_LONG_MESSAGE
      //delay(250);
// Somewhere to send the long message.
      while(cbus_long_message.is_sending()) { } //wait for previous message to finish.
// bool cbus_long_message.sendLongMessage(const byte *msg, const unsigned int msg_len, 
//                        const byte stream_id, const byte priority = DEFAULT_PRIORITY);
//      strcpy(msg, "Hello world!");
      string_length = snprintf(long_message_output_buffer, output_buffer_size, "Button %d changed", button);
      message_length = strlen(long_message_output_buffer);
      if (message_length > 0) {
        if (cbus_long_message.sendLongMessage(long_message_output_buffer, message_length, stream_id) ) {
           Serial << F("long message ") << long_message_output_buffer << F(" sent to ") << stream_id << endl;
        } else {
           Serial << F("long message sending ") << long_message_output_buffer << F(" to ") << stream_id << F(" failed with message length ") << message_length << endl;
        }
      } else {
        Serial << F("long message preparation failed with message length ") << message_length << endl;
      }
      bus_debug(); // Print diagnostics.
#endif
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
     Serial << F (" NV = ") << nv << F(" NV Value = ") << nvval << endl;

     switch (nvval)
     {
      case 0:
        opCode = (moduleSwitch[i].fell() ? OPC_ACON : OPC_ACOF);
  
        DEBUG_PRINT(F("> Button ") << i
            << (moduleSwitch[i].fell() ? F(" pressed, send 0x") : F(" released, send 0x")) << _HEX(opCode));
        is_success = sendEvent(opCode, (i + 1));
        // Temporary test code to trigger event.
        //criticalEvent.setEvent(opCode,testEvent);
        // This is how to trigger something in the criticalEvent.
        //criticalEvent.markTriggeredAndNotify();

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
        // Send event to test display on CAN1602BUT.
      case 99:
        opCode = (switchState[i] ? OPC_ACON : OPC_ACOF);
        sendEvent(opCode,(unsigned int)EventNo::testEvent); // Test of new code.

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
    // Choose here which opcode and event code to send
    //criticalEvent.setEvent(OPC_ACON,sendFailureEvent);
    // This is how to trigger something in the criticalEvent.
    //criticalEvent.markTriggeredAndNotify();
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

/*
void framehandler(CANFrame *msg) {

  byte op_code = msg->data[0];
  
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

  if (  op_code ==  OPC_CANID ) {
  unsigned int node_number = (msg->data[1] << 8 ) + msg->data[2];
  unsigned int new_CANId = msg->data[3];
      Serial << F("> Frame to change CANId") << endl;
      Serial << F("> Node No   ") << node_number << endl;
      Serial << F("> new CANId ")  << new_CANId << endl;
  }

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
*/

#ifdef CBUS_LONG_MESSAGE
   byte new_message = true;
//
// Handler to receive a long message 
// 
void longmessagehandler(void *fragment, const unsigned int fragment_len, const byte stream_id, const byte status){
// I need an example for what goes in here.
   //  fragment[fragment_len] = 0;
// If the message is complete it will be in fragment and I can do something with it.
     if( new_message) { // Print this only for the start of a message.
        Serial << F("> user long message handler: stream = ") << stream_id << F(", fragment length = ") 
               << fragment_len << F(", fragment = |");
        new_message = false;
     }
     if ( status == CBUS_LONG_MESSAGE_INCOMPLETE ) {
     // handle incomplete message
        Serial.write((char *)fragment, fragment_len);
     } else if (status == CBUS_LONG_MESSAGE_COMPLETE) {
     // handle complete message
        Serial.write((char *)fragment, fragment_len);
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

void printConfig(void)
{
  // code version
  Serial << F("> code version = ") << VER_MAJ << VER_MIN << F(" beta ") << VER_BETA << endl;
  Serial << F("> compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(", compiler ver = ") << __cplusplus << endl;

  // copyright
  Serial << F("> © Martin Da Costa (MERG M6223) 2020") << endl;
  Serial << F("> © Duncan Greenwood (MERG M5767) 2021") << endl;
  Serial << F("> © John Fletcher (MERG M6777) 2021") << endl;
  Serial << F("> © Sven Rosvall (MERG M3777) 2021") << endl;

#ifdef CBUS_LONG_MESSAGE
   Serial << F("> Long message handling available") << endl;
   byte num_ids = (sizeof(stream_ids) / sizeof(byte));
   Serial << F("> Sending on stream ") << stream_id << endl;
   Serial << F("> Listening on ");
   for (byte i = 0; i < num_ids; i++) Serial << stream_ids[i] << F(" ");
   Serial << endl;
#endif

  
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
