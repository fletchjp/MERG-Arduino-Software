///////////////////////////////////////////////////////////////////////////////////
// CANTOTEM
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
//                   #define CBUS_LONG_MESSAGE 1 to activate the code.
#define CBUS_LONG_MESSAGE 1
//                   Partial code now activated.
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

#if DEBUG
#define DEBUG_PRINT(S) Serial << S << endl
#else
#define DEBUG_PRINT(S)
#endif

// IoAbstraction libraries
#include <IoAbstraction.h>
#include <AnalogDeviceAbstraction.h>
#include <TaskManagerIO.h>
// IoAbstraction reference to the arduino pins.
IoAbstractionRef arduinoPins = ioUsingArduino();

#define ANALOG_IN_PIN A0
// here we create the abstraction over the standard arduino analog IO capabilities
ArduinoAnalogDevice analog; // by default it assumes 10 bit read, 8 bit write

// 3rd party libraries
#include <Streaming.h>
#include <Bounce2.h>

// Variables for buttons
int x;
int prevx = 0;
int range;
int prevrange = 0;
// Use these for the CBUS outputs
int button = 0;
int prevbutton = 0;



// CBUS library header files
#include <CBUS2515.h>            // CAN controller and CBUS class
#include "LEDControl.h"          // CBUS LEDs
#include <CBUSconfig.h>          // module configuration
#include <cbusdefs.h>            // MERG CBUS constants
#include <CBUSParams.h>

////////////DEFINE MODULE/////////////////////////////////////////////////

// module name
const unsigned char mname[7] PROGMEM = { 'T', 'O', 'T', 'E', 'M', ' ', ' ' };

// forward function declarations
void eventhandler(byte index, byte opc);
void framehandler(CANFrame *msg);

// Set opcodes for polling events
byte nopcodes = 9;
const byte opcodes[] PROGMEM = {OPC_ACON, OPC_ACOF, OPC_ARON, OPC_AROF, OPC_ASON, OPC_ASOF, OPC_AREQ, OPC_ASRQ, OPC_CANID }; 

// constants
const byte VER_MAJ = 3;         // code major version
const char VER_MIN = 'a';       // code minor version
const byte VER_BETA = 1;        // code beta sub-version
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
// The Ardunio CBUS library does not yet support this.
// create an additional object at the top of the sketch:
CBUSLongMessage cbus_long_message(&CBUS);   // CBUS long message object
#endif

// Event Nos for different events to be sent
// enum base changed to avoid other events.
// These are ideas at the moment.
enum eventNos {
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

#ifdef CBUS_LONG_MESSAGE
  // subscribe to long messages and register handler
  cbus_long_message.subscribe(stream_ids, (sizeof(stream_ids) / sizeof(byte)), long_message_data, buffer_size, longmessagehandler);
  // this method throttles the transmission so that it doesn't overwhelm the bus:
  cbus_long_message.setDelay(delay_in_ms_between_messages);
  cbus_long_message.setTimeout(1000);
#endif

  // configure and start CAN bus and CBUS message processing
  CBUS.setNumBuffers(2);         // more buffers = more memory used, fewer = less
  CBUS.setOscFreq(CAN_OSC_FREQ);   // select the crystal frequency of the CAN module
  CBUS.setPins(CAN_CS_PIN, CAN_INT_PIN);           // select pins for CAN bus CE and interrupt connections
  CBUS.begin();
}

void checkA0()
{
 x = analogRead (0);
 //Serial.println(x);
 if (x < 20){         // 175 was 50
  range = 1;
 } else if (x < 60){ // 350 was 250
  range = 2;
 } else if (x < 120){ // 500 was unchanged
  range = 3;
 } else if (x < 200){ // 800 was 650
  range = 4;
 } else if (x < 400){ // 850 was unchanged
  range = 5;
 } //else { range = 0; }
if (range != prevrange) {
 Serial.print(range);
 Serial.print(" ");
 Serial.print(x);
 //lcd.setCursor(10,1);
 switch (range) {
  case 1:
  {
   //lcd.print ("Right ");
   DEBUG_PRINT(F(" SW1 Left"));
   break;
  }
  case 2:
  {
   //lcd.print ("Up    ");
   DEBUG_PRINT(F(" SW2 Up"));
   break;
  }
  case 3:
  {
   //lcd.print ("Down  ");
   DEBUG_PRINT(F(" SW3 Down"));
   break;
  }
  case 4:
  {
   //lcd.print ("Left  ");
   DEBUG_PRINT(F(" SW4 Right"));
   break;
  }
  case 5:
  {
   //lcd.print ("Select");
   DEBUG_PRINT(F(" SW5 Select"));
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

/**
 * An event that extends BaseEvent allows for event driven programming, either notified by polling, interrupt or
 * another thread. There are two important methods that you need to implement, timeOfNextCheck that allows for polling
 * events, where you do the check in that method and trigger the event calling setTriggered(). Alternatively, like
 * this event, another thread or interrupt can trigger the event, in which case you call markTriggeredAndNotify() which
 * wakes up task manager. When the event is triggered, is exec() method will be called.
 */

// It is somewhat confusing that this event class is not a CBUS event - it is an event within the TaskManagerIO framework.
// The idea is that this code can respond when something out of course happens in the other tasks which are running.
// It is possible to have more than one instance of this class.
class CriticalEvent : public BaseEvent {
private:
    volatile byte eventValue;
    volatile byte opCode;
    volatile bool called;
    static const uint32_t NEXT_CHECK_INTERVAL = 600UL * 1000000UL; // 10 * 60 seconds away, maximum is about 1 hour.
public:
    CriticalEvent() {
        eventValue = 0; opCode = 0; eventValue = 0;
        called = false;
    }

    /**
     * Here we tell task manager when we wish to be checked upon again, to see if we should execute. In polling events
     * we'd do our check here, and mark it as triggered if our condition was met, here instead we just tell task manager
     * not to call us for 60 seconds at a go
     * @return the time to the next check
     */
    uint32_t timeOfNextCheck() override {
        // Nothing to do here.
        return 250UL * 1000UL;
    }

    /**
     * This is called when the event is triggered.
     * This code checks that an opcode and event no have been supplied using setEvent which must be called first.
     * This means that this code is needed to set off an event.
     *   criticalEvent.setEvent(opCode,testEvent);
     *   criticalEvent.markTriggeredAndNotify();
     */
    void exec() override {
        // Check that an opCode has been set.
        if (called) {
          Serial.print("Critical event ");
          Serial.print(opCode);
          Serial.print(" ");
          Serial.println(eventValue);
        // This sends a CBUS event to somewhere else.
          sendEvent(opCode,eventValue);
          called = false; // reset so that values must be sent again
        } else {
          Serial.println("Critical event called without data");
        }
    }

    // This needs to be called by the code generating the critical event before triggering.
    void setEvent(byte opcode,byte event) {
      called = true;  // Set here to force setting of values
      eventValue = event; opCode = opcode; 
    }
    
    /**
     * We should always provide a destructor.
     */
    ~CriticalEvent() override = default;
} criticalEvent;

void setup()
{
  Serial.begin (115200);
  Serial << endl << endl << F("> ** CANTOTEM ** ") << __FILE__ << endl;

  setupCBUS();
  setupModule();

  // Schedule tasks to run every 250 milliseconds.
  taskManager.scheduleFixedRate(250, runLEDs);
  taskManager.scheduleFixedRate(250, processSwitches);
  taskManager.scheduleFixedRate(250, checkA0);
  taskManager.scheduleFixedRate(250, processSerialInput);
  taskManager.scheduleFixedRate(250, processButtons);
  taskManager.registerEvent(&criticalEvent);

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
   char msg[16];
   byte stream_id = 1;
#endif
   if (button != prevbutton) {
      DEBUG_PRINT(F("Button ") << button << F(" changed")); 
      opCode = OPC_ACON;
      sendEvent(opCode, button + NUM_SWITCHES);
#ifdef CBUS_LONG_MESSAGE
// Somewhere to send the long message.
      while(cbus_long_message.is_sending()) { } //wait for previous message to finish.
// bool cbus_long_message.sendLongMessage(const byte *msg, const unsigned int msg_len, 
//                        const byte stream_id, const byte priority = DEFAULT_PRIORITY);
      strcpy(msg, "Hello world!");
      if (cbus_long_message.sendLongMessage(msg, strlen(msg), stream_id) ) {
        Serial << F("long message ") << msg << F(" sent to ") << stream_id << endl;
      }
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
        criticalEvent.setEvent(opCode,testEvent);
        // This is how to trigger something in the criticalEvent.
        criticalEvent.markTriggeredAndNotify();

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
        sendEvent(opCode,testEvent); // Test of new code.

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
    criticalEvent.setEvent(OPC_ACON,sendFailureEvent);
    // This is how to trigger something in the criticalEvent.
    criticalEvent.markTriggeredAndNotify();
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
