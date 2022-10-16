////////////////////////////////////////////////////////////////////////////////////
// CANASIGNALI2C 0a Beta 1  from CANASIGNAL 1a Beta 2
//
// This is the version using I2C to drive a PCA9685 to control the signals.
// Also this is going to control two signals based on TwoLinkedSignals3AspectBI2C
//
////////////////////////////////////////////////////////////////////////////////////
// Version information for CANASIGNAL
// 0a Beta 1 Starting to configure the code.
//           Set REMOTE_SIGNAL_INPUT to 0 for now.
//           First go at structure completed so that it compiles.
//           Device nos are device_number1 and device_number2,
//           fromEvent wrongly used at present.
////////////////////////////////////////////////////////////////////////////////////
// 0a Beta 2 Starting to develop GET_DEVICE_NOS_FROM_NVS.
////////////////////////////////////////////////////////////////////////////////////
// I have included A in the name to indicate an Arduino based CBUS Module.
//
// NOTE: Pin 7 is not a PWM pin and needs to be changed for RED light. Pins need a rethink.
//
// Combining Duncan Greenwood's Arduino CBUS Library with 
// Sven Rosvall's SignalControl Library (2020)
////////////////////////////////////////////////////////////////////////////////////
// Version information for CANASIGNAL
// First try at a CBUS module to control a colour light signal.
// 0.1  Add signal control using a local button as already tested on its own.
////////////////////////////////////////////////////////////////////////////////////
// 0.2  Start to add control using events.
//      Added routines sendEvent and sendEvent1 from other codes.
//      Code added for BoolInput - as yet untested.
////////////////////////////////////////////////////////////////////////////////////
// 0a Beta 3  Sort how to receive an event
//      Defined version using defs.  Current is 0a Beta 3
//      I have made the choice of local or remote control a compile time choice.
////////////////////////////////////////////////////////////////////////////////////
// 0a Beta 4  Explore use of the NVs to control the options.
//      This now uses NV 1 to control whether to use the CBUS event to control the signal.
//      Defining REMOTE_SIGNAL_INPUT_EVENT 0 still disables all definition of the external input.
////////////////////////////////////////////////////////////////////////////////////
// 0a Beta 5 Change all sendEvent routines to make eventNo unsigned int instead of byte.
//           Also updated event error messages to include the opcode.
//           Corrected bug in BoolInput default constructor.
//           Added code to respond to request for state.
////////////////////////////////////////////////////////////////////////////////////
// 0a Beta 6 Change eventhandler to use switch loop on opcodes.
////////////////////////////////////////////////////////////////////////////////////
// 0a Beta 7 Use framehandler to look for incoming AREQ.
////////////////////////////////////////////////////////////////////////////////////
// 0a Beta 8 Option to use short events to look for incoming ASRQ and respond.
////////////////////////////////////////////////////////////////////////////////////
// 1a Beta 1 Tidy up and move to version 1.
//           Removed USE_CODE_SWITCH - now the chosen version.
////////////////////////////////////////////////////////////////////////////////////
// 1a Beta 2 Add response to ASON/ASOF commands to device number.
////////////////////////////////////////////////////////////////////////////////////
// I am building a prototype using an Arduino UNO clone and various Maplin components I have to
// hand.  I think a neater solution would be to use small piece of stripboard for the external
// components.

// There is also a need for a CAN interface to CBUS.
// There several types available.  See for example the article in the
// MERG Journal Vol 53 No 2 June 2019 p.33 A CBUS Sound Player David Thomas 

// NOTE: Different CAN interfaces can have different crystal frequencies and this needs to be coded.
// See the definition of CANBUS8MHZ.  The default is 16MHz.

// John Fletcher (M6777) john@bunbury28.plus.com   June 2020
////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANT - This depends on the choice of hardware used to connect to CBUS.
//////////////////////////////////////////////////////////////////////////////////////////////
// Set this to 1 for CANBUS modules with 8 Mhz Crystal
// Set this to 0 for Sparkfun CANBUS shields.
#define CANBUS8MHZ 0
//////////////////////////////////////////////////////////////////////////////////////////////
// These are some more things which need to be set.
#define DEBUG         1 // set to 0 for no debug messages, 1 for messages to console

// This is a problem for me as I have some of these GROVE switches.
// Set GROVE 1 for a GROVE switch which is HIGH when pressed, otherwise 0
#define GROVE 0

// Choose remote or local input.
// This is still used to control whether the remote signal input is configured.
#define REMOTE_SIGNAL_INPUT_EVENT 0 // 1 for remote signal input, 0 for local input
#define ACCESSORY_REQUEST_EVENT 1   // 1 for response to request for state.
#define USE_SHORT_EVENTS 1 // Use ASRQ instead ot AREQ etc. Needs a device number
#if USE_SHORT_EVENTS
#define GET_DEVICE_NOS_FROM_NVS 1 // Get the device no from the NVs
// I need a scheme for this which will include an NV to say how many device nos are configured.
// Each device no will need two bytes.
#endif

// I2C libraries
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// Signal control libraries as needed.
#include "PinInput.h"
#include "Signal3AspectState.h"
#include "FastPWMLight.h"
#include "DistanceTimer.h"
#include "BlockDistanceInput.h"
#include "DigitalInput.h"
//#include "SlowLight.h"

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// These correspond with the 2 buttons
DistanceTimer distanceTimer1(2);
DistanceTimer distanceTimer2(8);

class BoolInput : public DigitalInput
{
   bool _state;

public:
   BoolInput() : _state(false) {}
   BoolInput(bool state) : _state(state) {}
 
   void set_state(bool state) { _state = state; }
   
   virtual bool get() const
   {
     return _state;
   }
 
};

// 3rd party libraries
#include <Streaming.h>

// CBUS library header files
#include <CBUS2515.h>               // CAN controller and CBUS class
#include <CBUSswitch.h>             // pushbutton switch
#include <CBUSLED.h>                // CBUS LEDs
#include <CBUSconfig.h>             // module configuration
#include <cbusdefs.h>               // MERG CBUS constants

// local header for CBUS pin settings and other constants.
#include "defs.h"

// CBUS objects
CBUS2515 CBUS;                      // CBUS object
CBUSConfig config;                  // configuration object
CBUSLED ledGrn, ledYlw;             // LED objects
CBUSSwitch pb_switch;               // switch object
// CBUS_1in1out4a with GROVE 0

// module objects
CBUSSwitch moduleSwitch;            // an example switch as input
CBUSLED moduleLED;                  // an example LED as output

/////////////////////////////////////////////////////////////////////////////////
// Set up for signals is done here
// There is nothing needed in the setup loop
// except to document what has been done.
/////////////////////////////////////////////////////////////////////////////////
// These are now on the pwm and in order top to bottom except missing 0
int greenPin1 = 1;
int redPin1 = 3;
int yellowPin1 = 2;

int greenPin2 =4;
int redPin2 = 6;
int yellowPin2 =5;

FastPWMLight greenLight1(pwm,greenPin1,true);
FastPWMLight yellowLight1(pwm,yellowPin1,true);
FastPWMLight redLight1(pwm,redPin1,false );
Signal3AspectState signal1(distanceTimer1, greenLight1, redLight1, yellowLight1,0);
unsigned int signal1state;

FastPWMLight greenLight2(pwm,greenPin2,true);
FastPWMLight yellowLight2(pwm,yellowPin2,false);
FastPWMLight redLight2(pwm,redPin2,false );

Signal3AspectState signal2(distanceTimer2, greenLight2, redLight2, yellowLight2,0);
unsigned int signal2state;


// The Signal LEDS provide a different LED support mechanism.
// These values have to be different from the CBUS values.
//#define GREEN_LIGHT 3
//#define RED_LIGHT   7
#define SIGNAL_BUTTON 6

PinInput button(SIGNAL_BUTTON);
BoolInput fromEvent;
//BoolInput fromEvent(true); // Default constructor fails.

//SlowLight greenLight(GREEN_LIGHT);
//SlowLight redLight(RED_LIGHT );
#if REMOTE_SIGNAL_INPUT_EVENT
// set up a second signal with the same lights.....
//Signal2Aspect signalEvent(fromEvent, greenLight, redLight);
#endif
//Signal2Aspect signalLocal(button, greenLight, redLight);

#if USE_SHORT_EVENTS
// Define device numbers for the signals - for now
unsigned int device_number1 = 513;
unsigned int device_number2 = 514;
#if GET_DEVICE_NOS_FROM_NVS
#define MAX_NUM_DEVICE_NOS 2
unsigned int device_numbers[MAX_NUM_DEVICE_NOS];
#endif
#endif

// local copy of NV1
byte localNV1;

// Event Nos for different events to be sent
enum eventNos {
  noEvent,  // not used
  testEvent,
  stopEvent,
  requestEvent
};

///////////////////////////////////////////////////////////////////////////////////

// CBUS module parameters
unsigned char params[21];

// module name
unsigned char mname[7] = { 'A', 'S', 'I', 'G', 'N', 'A', 'L' };

// forward function declarations
void eventhandler(byte index, byte opc);
void framehandler(CANFrame *msg);
// Set opcodes for polling events
byte nopcodes = 3;
byte opcodes[] = {OPC_AREQ, OPC_ASRQ, OPC_CANID }; 

// Module pins defined here - these are not the CBUS pins.
#define LED         4  // was 13     // Pin number for the LED using green LED
#define SOUNDER     9  // 
#define SWITCH      7  // use RED signal to send return message.

//
/// setup - runs once at power on
//

// Adding a buzzer output for taught event
int buzzer = SOUNDER;

void setup() {
  pinMode(buzzer, OUTPUT);

  Serial.begin (115200);
  Serial << endl << endl << F("> ** CANASIGNALI2C module ** ") << __FILE__ << endl;

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
  localNV1 = config.readNV(1);
#if GET_DEVICE_NOS_FROM_NVS
  // For now with one device number
  device_number1 = (config.readNV(2) << 8 ) + config.readNV(3);;
  device_number2 = (config.readNV(4) << 8 ) + config.readNV(5);;
#endif
  // set module parameters
  params[0] = 20;                     //  0 num params = 10
  params[1] = 0xa5;                   //  1 manf = MERG, 165
  params[2] = VER_MIN;                //  2 code minor version
  params[3] = MODULE_ID;              //  3 module id, 99 = undefined
  params[4] = config.EE_MAX_EVENTS;   //  4 num events
  params[5] = config.EE_NUM_EVS;      //  5 num evs per event
  params[6] = config.EE_NUM_NVS;      //  6 num NVs
  params[7] = VER_MAJ;                //  7 code major version
  params[8] = 0x07;                   //  8 flags = 7, FLiM, consumer/producer
  params[9] = 0x32;                   //  9 processor id = 50
  params[10] = PB_CAN;                // 10 interface protocol = CAN, 1
  params[11] = 0x00;
  params[12] = 0x00;
  params[13] = 0x00;
  params[14] = 0x00;
  params[15] = '3';
  params[16] = '2';
  params[17] = '8';
  params[18] = 'P';
  params[19] = CPUM_ATMEL;
  params[20] = VER_BETA;

  // assign to CBUS
  CBUS.setParams(params);
  CBUS.setName(mname);

  // initialise CBUS switch
#if GROVE
  pb_switch.setPin(SWITCH0, HIGH);
  Serial << F("> Switch set to go HIGH when pressed") << endl;
#else
  pb_switch.setPin(SWITCH0, LOW);
  Serial << F("> Switch set to go LOW when pressed") << endl;
#endif

  // module reset - if switch is depressed at startup and module is in SLiM mode
  pb_switch.run();

  if (pb_switch.isPressed() && (pb_switch.getCurrentStateDuration() > 20) && !config.FLiM) {
    Serial << F("> switch was pressed at startup in SLiM mode") << endl;
    config.resetModule(ledGrn, ledYlw, pb_switch);
  }

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);

  // register our CAN frame handler, to receive *every* CAN frame
  //CBUS.setFrameHandler(framehandler);
  // This will only process the defined opcodes.
  CBUS.setFrameHandler(framehandler, opcodes, nopcodes);
  
  // set LED and switch pins and assign to CBUS
  ledGrn.setPin(LED_GRN);
  ledYlw.setPin(LED_YLW);
  CBUS.setLEDs(ledGrn, ledYlw);
  CBUS.setSwitch(pb_switch);

  // set CBUS LEDs to indicate mode
  CBUS.indicateMode(config.FLiM);

  // configure and start CAN bus and CBUS message processing
  CBUS.setNumBuffers(4);
#if CANBUS8MHZ
  CBUS.setOscFreq(8000000UL);   // MCP2515 CANBUS 8Mhz 
#endif
  CBUS.setPins(10, 2);
  CBUS.begin();

  // configure the module switch, attached to pin 8, active from GROVE
#if GROVE
  moduleSwitch.setPin(SWITCH, HIGH);
  Serial << F("> Module Switch set to go HIGH when pressed") << endl;
#else
  moduleSwitch.setPin(SWITCH, LOW);
  Serial << F("> Module Switch set to go LOW when pressed") << endl;
#endif

  // configure the module LED, attached to pin 13 via a 1K resistor
  moduleLED.setPin(LED);

//////////////////////////////////////////////////////////////////////////
// Documentation of signal setup
//  Serial << F("> Signal set up with red as ") << RED_LIGHT << endl;
//  Serial << F("> Signal set up with green as ") << GREEN_LIGHT << endl;
//  Serial << F("> Signal set up with signal button as ") << SIGNAL_BUTTON << endl;
///////////////////////////////////////////////////////////////////////////
#if USE_SHORT_EVENTS
  Serial << F("> Using short events with device number1 ") << device_number1 << endl;
  Serial << F("> Using short events with device number2 ") << device_number2 << endl;
#endif

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
  /// give the switch and LED code some time to run
  //

  moduleSwitch.run();
  moduleLED.run();

  //
  /// test for switch input
  /// as an example, it must be have been pressed or released for at least half a second
  /// then send a long CBUS event with opcode ACON for on and ACOF for off
  /// event number (EN) is 1

  /// you can just watch for this event in FCU or JMRI, or teach it to another CBUS consumer module
  //

  if (moduleSwitch.stateChanged()) {
#if DEBUG
    Serial << F("> button state change detected") << endl;  
    //if (moduleSwitch.getCurrentStateDuration() > 500) {
    if(moduleSwitch.isPressed() ) {
      Serial << F("> button pressed, send ") << OPC_ACON << endl;  
    }  else {
      Serial << F("> button released, send ") << OPC_ACOF  << endl;      
    }
#endif

// sendEvent is now in a separate function.
// This could be converted to short events.
    byte opCode = (moduleSwitch.isPressed() ? OPC_ACON : OPC_ACOF);
    unsigned int eventNo = 1;  // Converted to 2 bytes for safety.
    sendEvent(opCode,eventNo);

  }

///////////////////////////////////////////////
// Update signal with current settings
// This is now controlled by NV 1
// if the remote event is configured.
#if REMOTE_SIGNAL_INPUT_EVENT
     if (localNV1 == 1) {
        signalEvent.update();
     } else {
#else
//        signalLocal.update();
// For now copy what is in TwoLinkedSignals3AspectBI2C
  distanceTimer1.update();
  distanceTimer2.update();
  signal1.update();
  signal1state = signal1.get_state();
  signal2.update_state(signal1state+1);
  signal2state = signal2.get_state();
#endif
#if REMOTE_SIGNAL_INPUT_EVENT
     }
#endif

///////////////////////////////////////////////

  //
  /// bottom of loop()
  //
}

// JPF copied sendEvent from CANTEXT code.
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
      Serial << F("> error sending CBUS message with opcode [ 0x") <<  _HEX(opCode) << F(" ]") << endl;
    }
#endif
    return res;
}

// Send an event routine built to start sending events based with one extra byte
// The events can be ACON1 or ACOF1 with 1 byte of data.
bool sendEvent1(byte opCode, unsigned int eventNo, byte item)
{
    CANFrame msg;
    msg.id = config.CANID;
    msg.len = 6;
    msg.data[0] = opCode;
    msg.data[1] = highByte(config.nodeNum);
    msg.data[2] = lowByte(config.nodeNum);
    msg.data[3] = highByte(eventNo); // event number (EN) could be > 255
    msg.data[4] = lowByte(eventNo); 
    msg.data[5] = item;    // The extra byte
    msg.ext = false;
    msg.rtr = false;

    bool res = CBUS.sendMessage(&msg);
#if DEBUG
    if (res) {
      Serial << F("> sent CBUS event with opCode [ 0x") << _HEX(opCode) << F(" ] and event No ") << eventNo << F(" with item ") << item << endl;
    } else {
      Serial << F("> error sending CBUS event with opcode [ 0x") <<  _HEX(opCode) << F(" ]") << endl;
     }
#endif
    return res;
}


// Send an event routine built to start sending events based with extra bytes
bool sendEventN(byte opCode, unsigned int eventNo, byte n, const byte* buf)
{
  // The events can be ACON1, ACOF1, ACON2, ACOF2, ACON3, ACOF3 with 1 2 or 3 bytes of data.
  // I am not checking the match between opcode and data length (as yet)
  if (n == 0) {
     // no data, use the old method.
     return sendEvent(opCode, eventNo);
  } else {
    CANFrame msg;
    msg.id = config.CANID;
    msg.len = 5+n;
    msg.data[0] = opCode;
    msg.data[1] = highByte(config.nodeNum);
    msg.data[2] = lowByte(config.nodeNum);
    msg.data[3] = highByte(eventNo); // event number (EN) could be > 255
    msg.data[4] = lowByte(eventNo); 
    for(byte i = 0;  i< n; i++)
    {
       msg.data[i+5] = buf[i];
    }
    msg.ext = false;
    msg.rtr = false;

    bool res = CBUS.sendMessage(&msg);
#if DEBUG
    if (res) {
      Serial << F("> sent CBUS event with opCode [ 0x") << _HEX(opCode) << F(" ] and event No ") << eventNo;
      Serial << F(" with ") << n << F(" data : ");
      for (byte i = 0;  i< n; i++ ) Serial << buf[i] << F(" ");
      Serial << endl;
    } else {
      Serial << F("> error sending CBUS event with opcode [ 0x") <<  _HEX(opCode) << F(" ]") << endl;
    }
#endif
    return res;
  }
}

//
/// user-defined event processing function
/// called from the CBUS library when a learned event is received
/// it receives the event table index and the CAN frame
//

void eventhandler(byte index, CANFrame *msg) {

  // as an example, control an LED if the first EV equals 1

  Serial << F("> event handler: index = ") << index << F(", opcode = 0x") << _HEX(msg->data[0]) << endl;

  // read the value of the first event variable (EV) associated with this learned event

  /* 
 *  I am going to change the event handler to process based on the opcode as is done in the
 *  message handler.  This is a big change and I am going to maintain both versions of the
 *  code for the time being
 *  
 */
    byte op_code = msg->data[0];
    unsigned int node_number = (msg->data[1] << 8 ) + msg->data[2];
    // This is not true in all cases.
    // For some it is the device number
    unsigned int event_number = (msg->data[3] << 8 ) + msg->data[4];
    // For now get the first event value
    byte ev = 1;
    byte evval = config.getEventEVval(index, ev - 1);
    Serial << F("> NN = ") << node_number << F(", EN = ") << event_number << endl;
    Serial << F("> op_code = ") << op_code << endl;
    Serial << F("> EV1 = ") << evval << endl;
    switch (op_code)
    {
         // Event on and off
         // Handle these together based on event no.
         case OPC_ACON:
         case OPC_ACOF:
         if (evval == 1) {
            if (op_code == OPC_ACON) {
               Serial << F("> switching the LED on") << endl;
               moduleLED.blink();
            } else if (op_code == OPC_ACOF) {
               Serial << F("> switching the LED off") << endl;
               moduleLED.off();
            }
         }

#if REMOTE_SIGNAL_INPUT_EVENT
         if (evval = 88) {
            if (localNV1 == 1) {
                if ( op_code ==  OPC_ACON ) {
                  fromEvent.set_state(true);
                  Serial << F("> SIGNAL ON") << endl;
               } else if (op_code == OPC_ACOF) {
                  fromEvent.set_state(false);
                  Serial << F("> SIGNAL OFF") << endl;
               }
            }
         }
#endif
         if (evval = 99) {
             if ( op_code ==  OPC_ACON ) {
               tone(buzzer, 1000);
               Serial << F("> BUZZER ON") << endl;
             } else if (op_code == OPC_ACOF) {
               noTone(buzzer);
               Serial << F("> BUZZER OFF") << endl;
             }
         }

         break;
#if USE_SHORT_EVENTS
         // Event on and off
         // Handle these together based on event no.
         case OPC_ASON:
         case OPC_ASOF:
         if (event_number ==  device_number1) {
            if (op_code == OPC_ASON) {
               Serial << F("> short event switching signal on") << endl;
               fromEvent.set_state(true);
            } else if (op_code == OPC_ASOF) {
               Serial << F("> short event switching the signal off") << endl;
               fromEvent.set_state(false);
            }
         }      
         break;
#endif
         // This does not work as these opcodes do not come as events - see framehandler.
         case OPC_AREQ:
         case OPC_ASRQ:
#if ACCESSORY_REQUEST_EVENT
#if USE_SHORT_EVENTS
         // I think this is not processed at the moment - not learnt.
         // Processing of request for the signal status.
         // Returning with the device number which came in.
         Serial << F("processing  ASRQ with index ") << index << F(", device number ") << device_number1;
         Serial << F(" and event value(1) ") << evval << endl; 
         if (fromEvent.get()) {
             sendEvent(OPC_ARSON,device_number1);
             Serial << F("response ON") << endl; 
         } else { 
             sendEvent(OPC_ARSOF,device_number1);
             Serial << F("response Off") << endl; 
         }
#else
         // Processing of request for the signal status.
         // Returning with the event number which came in.
         Serial << F("processing  AREQ with index ") << index << F(", event no ") << event_number;
         Serial << F(" and event value(1) ") << evval << endl; 
         if (fromEvent.get()) {
             sendEvent(OPC_ARON,event_number);
             Serial << F("response ON") << endl; 
         } else { 
             sendEvent(OPC_AROF,event_number);
             Serial << F("response Off") << endl; 
         }
#endif
#endif

         break;
         default:
         // ignore any other CBUS events
#if DEBUG
         Serial << F("Event ignored with Opcode [ 0x") << _HEX(op_code) << F(" ]")<< endl;
#endif
    }

  return;
}

//
/// user-defined frame processing function
/// called from the CBUS library for *every* CAN frame received
/// it receives a pointer to the received CAN frame
//

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

#if ACCESSORY_REQUEST_EVENT
#if USE_SHORT_EVENTS
  // Check that the incoming ARSQ has the correct device no.
  // This could become a look up if there is more than one device in the module.
  unsigned int event_number = (msg->data[3] << 8 ) + msg->data[4];
  if (  op_code == OPC_ASRQ  && event_number == device_number1) {
         // Processing of request for the signal status.
         // Returning with the device number which came in.
         Serial << F("> processing  ASRQ with device number ") << device_number1 << endl;
         if (fromEvent.get()) {
             sendEvent(OPC_ARSON,device_number1);
             Serial << F("> response ON") << endl; 
         } else { 
             sendEvent(OPC_ARSOF,device_number1);
             Serial << F("> response OFF") << endl; 
         }
  }
#else
  // This will process all AREQs so it will need to be changed to be more specific.
  if (  op_code == OPC_AREQ ) {
         unsigned int node_number = (msg->data[1] << 8 ) + msg->data[2];
         // This is not true in all cases.
         // For some it is the device number
         unsigned int event_number = (msg->data[3] << 8 ) + msg->data[4];
         // Processing of request for the signal status.
         // Returning with the event number which came in.
         Serial << F(" > processing  AREQ with node number ") << node_number << F(", event no ") << event_number;
         //Serial << F(" and event value(1) ") << evval << endl;
         Serial << endl; 
         if (fromEvent.get()) {
             sendEvent(OPC_ARON,event_number);
             Serial << F("> response ON") << endl; 
         } else { 
             sendEvent(OPC_AROF,event_number);
             Serial << F("> response OFF") << endl; 
         }
  }
#endif
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
  Serial << F("> NV[1] = ") << config.readNV(1) << endl;

  // copyright
  Serial << F("> © Duncan Greenwood (MERG M5767) 2019") << endl;
  Serial << F("> © John Fletcher (MERG M6777) 2020") << endl;
  Serial << F("> © Sven Rosvall (MERG M3777) 2020") << endl;
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
