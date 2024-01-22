/// @file MEGATaskIntegrationCBUD
/// @brief extend to have a 20 by display as well and have a CBUS interface.
#define VERSION 0.8
///
/// The CBUS code is based on the work in CANCMDDC which is the only MEGA code for CBUS which I have.
///
/// I have put in the structures for CBUS using the ArduinoCBUS libraries.
///
/// For the moment I have not included the long message code.
///
/// I do need to decide what events to send and receive.
///
/// Code is now running. It does not work when setupCBUS() is not run!
///
/// This now uses the display to show the time elapsed and the keypad and encoder values.
///
/// derived from matrixKeyboardMartinsLibraryMEGA34Event2.ino
/// Example of matrix keyboard support for MEGA 3 by 4 Keypad built into IoAbstraction adding 2 of Martin's encoders via an event
///
/// This now runs Martin's encoder library integrated in the IoAbstraction code.
///
/// I am now extending to two encoders using the method devised by Martin for this.
///
/// Some previously global variables have to be now inside the EncoderEvent class so that they can be duplicated.
/// This has now been carried out.
///
/// This is now adapted to report the state of the key.
///
/// This example shows how to use the matrix keyboard support that's built into IoAbstraction,
/// it can be used out the box with either a 3x4 or 4x4 keypad, but you can modify it to use
/// any matrix keyboard quite easily.
/// It just sends the characters that are typed on the keyboard to Serial. The keyboard in this
/// example is connected directly to Arduino pins, but could just as easily be connected over
/// a PCF8574, MCP23017 or other IoAbstraction.
/// This version sets up the keyboard adapting the custom_keyboard example.
///
/// MEGA pin settings changed from the CANCMDDC code
/// A0 to A7 do not work with Martin's encoder as PCI interrupts are not available.
/// Digital pin 38             Encoder 1 Switch (SW on encoder)
/// Digital pin 40             Encoder 2 Switch (SW on encoder)
/// Digital / Analog pin 8     Encoder 1 A (CLK on encoder)
/// Digital / Analog pin 10    Encoder 2 A (CLK on encoder)
/// Digital / Analog pin 9     Encoder 1 B (DT  on encoder)
/// Digital / Analog pin 11    Encoder 2 B (DT  on encoder)
///
/// Versions 0.x are development versions while the CBUS code is being put together.
///              These have no event capability either in or out.



#include <Wire.h>
//#include <IoAbstraction.h> not needed here
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

#include <LiquidCrystalIO.h>

// When using the I2C version, this include is always needed. Doing this reduces the memory slightly for
// users that are not using I2C.
#include <IoAbstractionWire.h>

// 3rd party libraries as I use C++ style streaming output as used in Arduino CBUS library examples.
#include <Streaming.h>

// CBUS library header files
#include <CBUS2515.h>               // CAN controller and CBUS class
#include <CBUSswitch.h>             // pushbutton switch
#include <CBUSLED.h>                // CBUS LEDs
#include <CBUSconfig.h>             // module configuration
#include <cbusdefs.h>               // MERG CBUS constants
#include <CBUSParams.h>

// Set this to 1 for CANBUS modules with 8 Mhz Crystal
// Set this to 0 for Sparkfun CANBUS shields or Arduino Kit 110 shield
#define CANBUS8MHZ 1
#define DEBUG      1 // set to 0 for no debug messages, 1 for messages to console
//#define SERIAL_OUTPUT  // define for serial message to console

// For most standard I2C backpacks one of the two helper functions below will create you a liquid crystal instance
// that's ready configured for I2C. Important Note: this method assumes a PCF8574 running at 100Khz. If otherwise
// use a custom configuration as you see in many other examples.

// If your backpack is wired RS,RW,EN then use this version
LiquidCrystalI2C_RS_EN(lcd, 0x27, false)

// If your backpack is wired EN,RW,RS then use this version instead of the above.
//LiquidCrystalI2C_EN_RS(lcd, 0x20, false)

void redraw_display();

/**
 * Here we create an event that handles all the drawing for an application, in this case printing out readings
 * of a sensor when changed. It uses polling and immediate triggering to show both examples
 */
class DrawingEvent : public BaseEvent {
private:
    float elapsed_time;
    volatile bool emergency; // if an event comes from an external interrupt the variable must be volatile.
    bool timeChanged;
    bool hasChanged;
    bool keyChanged;
    bool posChanged;
    bool do_redraw;
    char key_value;
    byte key_pos;
    byte next_pos;
    int pos1, pos2;
public:
    DrawingEvent() {
      elapsed_time = 0.0f;
      emergency = false;
      hasChanged = false;
      timeChanged = false;
      keyChanged = false;
      do_redraw = false;
      key_value = ' ';
      pos1 = pos2 = 0;
      key_pos = 0;
      posChanged = 0;
    }

    /**
     * This is called by task manager every time the number of microseconds returned expires, if you trigger the
     * event it will run the exec(), if you complete the event, it will be removed from task manager.
     * @return the number of micros before calling again.
     */
    uint32_t timeOfNextCheck() override {
        setTriggered(hasChanged);
        return millisToMicros(500); // no point refreshing more often on an LCD, as its unreadable
    }

   /**
     * This is called when the event is triggered, it prints all the data onto the screen.
     */
    void exec() override {
        hasChanged = false;
        // Here we need to output to the display.
        // This is now done only for the sections which have changed.
        if (timeChanged) {
          lcd.setCursor(12, 1);
          lcd.print(elapsed_time);
          timeChanged = false;
        }
        if (keyChanged) { 
          lcd.setCursor(7+key_pos, 2);
          lcd.print(key_value); keyChanged = false;
          key_pos = next_pos;
          if (next_pos == 0) {
            lcd.print("         ");
          }
        }
        if (posChanged) {
          lcd.setCursor(5, 3);
          lcd.print("     ");
          lcd.setCursor(5, 3);
          lcd.print(pos1);
          lcd.setCursor(15, 3);
          lcd.print("     ");
          lcd.setCursor(15, 3);
          lcd.print(pos2);
          posChanged = false;
        }
        if (do_redraw) {
           redraw_display();
           do_redraw = false;
        }
        //lcd.setCursor(0, 2);
        //lcd.print(emergency ? "emergency!!" : "           ");
    }

    /**
     * This sets the latest temperature and heater status, but only marks the event changed,
     * so it will need to poll in order to trigger.
     * This prevents excessive screen updates.
     * @param temp the new temperature
     * @param on if the heater is on
     */
    void setLatestStatus(float value) {
        elapsed_time = value;
        hasChanged = true;// we are happy to wait out the 500 millis
        timeChanged = true;
    }
    void setLatestKey(char key) {
        key_value = key;
        switch (key_value) {
          case '#' : 
          {
            // Put in # to show complete number.
            // A second # clears the numbers.
             if (next_pos == 0 ) { key_value = ' '; }
             next_pos = 0;
             break;
          }
          case '*' : 
          {
            // Replace the previous number with a *
            // and the next key will replace it.
            if( next_pos > 0) { next_pos--; key_pos--; }
            break;
          }
          default: next_pos = key_pos + 1;
        }
        hasChanged = true;// we are happy to wait out the 500 millis
        keyChanged = true;
    }    
    void setLatestPos(char enc,int pos) {
        if (enc == '1') { pos1 = pos; }
        else { pos2 = pos; }
        hasChanged = true;// we are happy to wait out the 500 millis
        posChanged = true;
    }    
    /**
     * Triggers an emergency that requires immediate update of the screen
     * @param isEmergency if there is an urgent notification
     */
    void triggerEmergency(bool isEmergency) {
        emergency = isEmergency;
        markTriggeredAndNotify(); // get on screen asap.
    }
    // Call the redraw from time to time.
    void redraw() {
        hasChanged = true;// we are happy to wait out the 500 millis
        do_redraw = true;
    }
};

// create an instance of the above class
DrawingEvent drawingEvent;

/// @brief Declaration of Arduino abstraction for Input/Output.
/// This works for pins connected directly to an Arduino
IoAbstractionRef arduinoIo = ioUsingArduino();

/// Swap the pins to get the opposite action
/// This is equivalent to changing over the wires.
#define SWAP_PINS 1

#include "EncoderMD.h"

volatile byte lastPins = 0;

/// The pins onto which the rotary encoders switches are connected
const int spinwheelClickPin1 = 38; /// SW on encoder1
const int spinwheelClickPin2 = 40; /// SW on encoder2
/// In this case the pins are passed to Martin's code instead.
const int encoderAPin1 = A8;  /// CLK on encoder1 
const int encoderBPin1 = A9;  /// DT  on encoder1
const int encoderAPin2 = A10; /// CLK on encoder2 
const int encoderBPin2 = A11; /// DT  on encoder2

/// the maximum (0 based) value that we want the encoder to represent.
const int maximumEncoderValue = 128;

/// an LED that flashes as the encoder changes
const int ledOutputPin = 13;


#if SWAP_PINS
/// Use thess settings for right hand turn to increase the value.
EncoderMD encoder1(encoderBPin1,encoderAPin1);
EncoderMD encoder2(encoderBPin2,encoderAPin2);
#else
/// Use these settings for left hand turn to increase the value.
EncoderMD encoder1(encoderAPin1,encoderBPin1);
EncoderMD encoder2(encoderAPin2,encoderBPin2);
#endif

/// @brief The PCI setup is specific to the pins being used, here A8 to A11.
/// PCI does not work on the MEGA pins A0 to A7.
void setupPCI()
{
  cli();
  PCICR  |= 0b00000100;  //Set Pin Change Interrupt on Register K
  PCMSK2 |= 0b00001111;  //Set A8, A9, A10 & A11 for interrupt
  sei();
}

/// @brief EncoderEvent - now uses class variable to support multiple encoders.
///
/// char encoderName variable is provided so that outputs can be distinguished.
class EncoderEvent : public BaseEvent {
private:
/// Used to note when the encoder position has changed.
    char encoderName;
    EncoderMD &encoder;
    static const uint32_t NEXT_CHECK_INTERVAL = 60UL * 1000000UL; // 60 seconds away, maximum is about 1 hour.
public:
    boolean TurnDetected;
    int RotaryPosition;
    int PrevPosition;
    EncoderEvent(EncoderMD &encoder_, char name_) : encoderName(name_), encoder(encoder_)  {
      RotaryPosition = 0; PrevPosition = 0;
    }
    /// @brief timeOfNextCheck now replaced by call from ISR calling markTriggeredAndNotify().
    uint32_t timeOfNextCheck() override {
        return 250UL * 1000UL; // every 100 milliseconds we roll the dice
    }
    void exec() override {
         //Serial.print("exec called with ");
         RotaryPosition = encoder.getPosition();
         //Serial.println(RotaryPosition);
         TurnDetected = (RotaryPosition != PrevPosition);
         if (TurnDetected)  {         
           PrevPosition = RotaryPosition; // Save previous position in variable
#ifdef SERIAL_OUTPUT
           Serial.print(encoderName);
           Serial.print(" ");
           Serial.println(RotaryPosition);
#endif
           drawingEvent.setLatestPos(encoderName,RotaryPosition);    
           // here we turn the led on and off as the encoder moves.
           ioDeviceDigitalWriteS(arduinoIo, ledOutputPin, RotaryPosition % 2);
         }
    }
    /**
     * We should always provide a destructor.
     */
    ~EncoderEvent() override = default;
};

/// Declare encoderEvent instances for each encoder.
EncoderEvent encoderEvent1(encoder1,'1');
EncoderEvent encoderEvent2(encoder2,'2');

//class EncoderEvent;

/// @brief When the spinwheel1 is clicked, this function will be run as we registered it as a callback
///
/// There have to be separate routines for each encoder to distinguish the variables.
void onSpinwheelClicked1(pinid_t /*pin*/, bool heldDown) { //, MyEncoder &encoder, EncoderEvent &encoderEvent) {
#ifdef SERIAL_OUTPUT
  Serial.print("Button 1 pressed ");
  Serial.println(heldDown ? "Held" : "Pressed");
#endif
    if (encoderEvent1.RotaryPosition == 0) {  // check if button was already pressed
       } else {
        encoderEvent1.RotaryPosition=0; // Reset position to ZERO
        encoder1.setPosition(encoderEvent1.RotaryPosition);
#ifdef SERIAL_OUTPUT
        Serial.print("X ");
        Serial.println(encoderEvent1.RotaryPosition);
#endif
        drawingEvent.setLatestPos('1',encoderEvent1.RotaryPosition);    
        encoderEvent1.PrevPosition = encoderEvent1.RotaryPosition;
      }
}

/// @brief When the spinwheel2 is clicked, this function will be run as we registered it as a callback
void onSpinwheelClicked2(pinid_t /*pin*/, bool heldDown) { //, MyEncoder &encoder, EncoderEvent &encoderEvent) {
#ifdef SERIAL_OUTPUT
  Serial.print("Button 2 pressed ");
  Serial.println(heldDown ? "Held" : "Pressed");
#endif
    if (encoderEvent2.RotaryPosition == 0) {  // check if button was already pressed
       } else {
        encoderEvent2.RotaryPosition=0; // Reset position to ZERO
        encoder2.setPosition(encoderEvent2.RotaryPosition);
#ifdef SERIAL_OUTPUT
        Serial.print("Y ");
        Serial.println(encoderEvent2.RotaryPosition);
#endif
        drawingEvent.setLatestPos('2',encoderEvent2.RotaryPosition);    
        encoderEvent2.PrevPosition = encoderEvent2.RotaryPosition;
      }
}

//
// We need to make a keyboard layout that the manager can use. choose one of the below.
// The parameter in brackets is the variable name.
//
//MAKE_KEYBOARD_LAYOUT_3X4(keyLayout)
//MAKE_KEYBOARD_LAYOUT_4X4(keyLayout)
//
// I want to use the alternative based on the custom_keyboard example.
//const char pgmLayout[] PROGMEM = "charsColByRow";
//KeyboardLayout layout(rows, cols, const char* pgmLayout)

const byte ROWS = 4; /// four rows
const byte COLS = 3; /// four columns
/// define the symbols on the buttons of the keypads
/// PROGMEM is important.
const char layout[] PROGMEM = "123456789*0#"; // Chars have to be in a string.
//  '1','2','3','4','5','6','7','8','9','*','0','#'
//};
/// These are in order of Keypad pins from 1 to 8.
/// Pin 1 is on the left with the pad face up.
byte rowPins[ROWS] = {49, 47, 45, 43}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {41, 39, 37}; //connect to the column pinouts of the keypad
/// This seems fussy. ROWS and COLS will not work here.
uint8_t rows = ROWS;
uint8_t cols = COLS;
/// Set up the keyLayout
KeyboardLayout keyLayout(rows, cols, layout);

///
/// We need a keyboard manager class too
///
MatrixKeyboardManager keyboard;

//char old_key = 'Z';

// Class to store the keys pressed in order.
// This is a class so that more than one implementation could be used.

const byte MAX_CHARS = 5;

class StoreKeys {
   char stored[MAX_CHARS];
   byte n_stored;
public:
   StoreKeys() : n_stored(0) { }
   void print_keys() { // This can be adapted to pass the chars to the long message.
        for (byte i = 0; i <= n_stored; i++) {
           Serial.print(stored[i]);
        }
        Serial.println(" ");
   }
   void store_key(char key) {
     switch (key) {
       case '*': //delete previous key unless there are none.
       {
        if (n_stored > 0 ) n_stored--;
         break;
       }
       case '#': //String is complete - send it.
       {
           print_keys();
           break;
       }
       default:  // store the key
       {
           if (n_stored < MAX_CHARS) {
              stored[n_stored] = key; n_stored++;
           }
           break;
       }
     }
   }
     void reset_keys() { n_stored = 0; }
     byte num_keys() const { return n_stored; }
     char* get_keys() { return stored; }
};

/// Adapted from Key.h and made into a scoped enum.
enum class KeyState : byte { IDLE, PRESSED, HOLD, RELEASED };

/// This is called from the listener to report on the state.
void tell_the_state(char key_val, KeyState key_state = KeyState::IDLE) {
  //if (key_val != old_key) {
#ifdef SERIAL_OUTPUT
    Serial.print(F("Key "));
    Serial.print(key_val);
#endif
    if (key_state == KeyState::PRESSED) { 
#ifdef SERIAL_OUTPUT
     Serial.println(F(" pressed"));
#endif
      drawingEvent.setLatestKey(key_val);    
    } else if (key_state == KeyState::HOLD) { 
#ifdef SERIAL_OUTPUT
      Serial.println(F(" held"));
#endif
      //old_key = key_val;
    } else if (key_state == KeyState::RELEASED) { 
#ifdef SERIAL_OUTPUT
      Serial.println(F(" released"));
#endif
      //old_key = key_val;
    } else {
#ifdef SERIAL_OUTPUT
      Serial.println(F(" idle"));
#endif
    }
  //}
}

/// @brief We need a class that extends from KeyboardListener.
///This gets notified when there are changes in the keyboard state.
///
class MyKeyboardListener : public KeyboardListener {
public:
    void keyPressed(char key, bool hold) override {
#ifdef SERIAL_OUTPUT
        Serial.print("Key ");
        Serial.print(key);
        Serial.print(" is pressed, hold = ");
        Serial.println(hold);
#endif
        if (hold) tell_the_state(key,KeyState::HOLD);
        else tell_the_state(key,KeyState::PRESSED);
    }

    void keyReleased(char key) override {
#ifdef SERIAL_OUTPUT
        Serial.print("Released ");
        Serial.println(key);
#endif
        tell_the_state(key,KeyState::RELEASED);
    }
} myListener;

void redraw_display() {
  lcd.setCursor(0,0);
  // Print a message to the LCD.
  lcd.print("Task CBUS Ver ");lcd.print(VERSION);
  lcd.setCursor(0,1);
  lcd.print("Time (secs) ");
  lcd.setCursor(0,2);
  lcd.print("Keypad ");
  lcd.setCursor(0,3);
  lcd.print("Enc1 ");
  lcd.setCursor(10,3);
  lcd.print("Enc2 ");
}

// CBUS objects
CBUSConfig config;                  // configuration object
CBUS2515 CBUS;                      // CBUS object - all library versions
//CBUS2515 CBUS(&config);           // CBUS object - new versions only
#ifdef CBUS_LONG_MESSAGE
// create an additional object at the top of the sketch:
CBUSLongMessage cbus_long_message(&CBUS);   // CBUS long message object
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// module objects replaced by IO_Abstraction devices.
CBUSLED ledGrn, ledYlw;             // LED objects
CBUSSwitch pb_switch;               // switch object

//////////////////////////////////////////////////////////////////////////////////////////////
// constants
const byte VER_MAJ = 1;                  // code major version
const char VER_MIN = 'a';                // code minor version
const byte VER_BETA = 8;                 // code beta sub-version
const byte MODULE_ID = 99;               // CBUS module type

const byte LED_GRN = 4;                  // CBUS green SLiM LED pin
const byte LED_YLW = 5;                  // CBUS yellow FLiM LED pin
const byte SWITCH0 = 6;                  // CBUS push button switch pin

/* Pins used by SPI interface to CAN module */
/* These are the  values for the MEGA2560 */
#define CHIPSELECT  53
#define CBUSINTPIN  18 // 49

// module name
const unsigned char mname[7] PROGMEM = { 'M', 'E', 'G', 'A', 'T', 'S', ' K'};

// forward function declarations
void eventhandler(byte index, byte opc);
bool sendEvent(byte opCode,unsigned int eventNo);

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
  CBUS.setName((byte *)mname);

  // register our CBUS event handler, to receive event messages of learned events
  CBUS.setEventHandler(eventhandler);
//  CBUS.setFrameHandler(framehandler,(byte *) opcodes, nopcodes);

#ifdef CBUS_LONG_MESSAGE
  // subscribe to long messages and register handler
  cbus_long_message.subscribe(stream_ids, (sizeof(stream_ids) / sizeof(byte)), long_message_data, buffer_size, longmessagehandler);
  // this method throttles the transmission so that it doesn't overwhelm the bus:
  cbus_long_message.setDelay(delay_in_ms_between_messages);
  cbus_long_message.setTimeout(1000);
#endif

  // set LED and switch pins and assign to CBUS
  ledGrn.setPin(LED_GRN);
  ledYlw.setPin(LED_YLW);
  CBUS.setLEDs(ledGrn, ledYlw);
  CBUS.setSwitch(pb_switch);

  // set CBUS LEDs to indicate mode
  CBUS.indicateMode(config.FLiM);

  // configure and start CAN bus and CBUS message processing
  CBUS.setNumBuffers(4,4);  // Set TX buffers. Default for RX is 4.
  // more buffers = more memory used, fewer = less
#if CANBUS8MHZ
  //CBUS.setOscFreq(CAN_OSC_FREQ);   // select the crystal frequency of the CAN module
  CBUS.setOscFreq(8000000UL);   // MCP2515 CANBUS 8Mhz 
#endif
  CBUS.setPins(CHIPSELECT,CBUSINTPIN); // Values of the pins for a MEGA
  CBUS.begin();
}

void eventhandler(byte index, CANFrame *msg) {
 // as an example, control an LED if the first EV equals 1

  Serial << F("> event handler: index = ") << index << F(", opcode = 0x") << _HEX(msg->data[0]) << endl;

/* 
 *  I am going to change the event handler to process based on the opcode as is done in the
 *  message handler. The old alternative is now removed.
 */
    byte op_code = msg->data[0];
    unsigned int node_number = (msg->data[1] << 8 ) + msg->data[2];
    // This is not true in all cases.
    // For some it is the device number
    unsigned int event_number = (msg->data[3] << 8 ) + msg->data[4];
    // For now get the first event value
    byte ev = 1;
    //byte evval = config.getEventEVval(index, ev - 1); I think the library has changed.
    byte evval = config.getEventEVval(index, ev);
    Serial << F("> NN = ") << node_number << F(", EN = ") << event_number << endl;
    Serial << F("> op_code = ") << op_code << endl;
    Serial << F("> EV1 = ") << evval << endl;

    switch (op_code)
    {
        // Event on and off
        // Handle these together based on event no.
         case OPC_ACON:
         case OPC_ACOF:
         case OPC_ASON:
         case OPC_ASOF:

         break;
         // Accessory response events
         // Handle these together based on event no.
         case OPC_ARON:
         case OPC_AROF:
         case OPC_ARSON:
         case OPC_ARSOF:

         break;
         default:
         
         break;
    }
}

/// Send an event routine built to start sending events based on input.
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
      Serial << F("> sent CBUS event with opCode [ 0x") << _HEX(opCode) << F(" ] and event No ") << eventNo << endl;
    } else {
      Serial << F("> error sending CBUS event wit opcode [ 0x") <<  _HEX(opCode) << F(" ]") << endl;
    }
#endif
    return res;
}

/// Send an event routine built to start sending events based with one extra byte
/// The events can be ACON1 or ACOF1 with 1 byte of data.
/// Alternatively, short events ASON1 or ASOF1.
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
      Serial << F("> sent CBUS event with opCode [ 0x") << _HEX(opCode) << F(" ] and event No ") << eventNo << endl;
    } else {
      Serial << F("> error sending CBUS event wit opcode [ 0x") <<  _HEX(opCode) << F(" ]") << endl;
    }
#endif
    return res;
}

void setupDisplay()
{
  // for i2c variants, this must be called first.
  Wire.begin();
  // set up the LCD's number of columns and rows, must be called.
  lcd.begin(20, 4);
  // The following code is now needed which it was not before.
  // most backpacks have the backlight on pin 3.
  lcd.configureBacklightPin(3);
  lcd.backlight();
  redraw_display();  

}

void setupEncoders()
{
  encoder1.setLimits(0,maximumEncoderValue);
  encoder2.setLimits(0,maximumEncoderValue);
  encoder1.setPosition (0);
  encoder1.setWrap (0);
  encoder2.setPosition (0);
  encoder2.setWrap (0);
}



void setup() {
/// setup 
  Serial.begin(115200); while(!Serial); delay(2000);
  Serial << endl << endl << F("> ** MEGA Task Integration for CBUS ** ") << __FILE__ << endl;
  setupCBUS();
  setupDisplay();
  setupPCI();
  setupEncoders();

  // here we initialise as output the output pin we'll use
  ioDevicePinMode(arduinoIo, ledOutputPin, OUTPUT);

  // First we set up the switches library, giving it the task manager and tell it to use arduino pins
  // We could also of chosen IO through an i2c device that supports interrupts.
  // If you want to use PULL DOWN instead of PULL UP logic, change the true to false below.
  switches.initialise(arduinoIo, true);

  // now we add the switches, we dont want the spinwheel button to repeat, so leave off the last parameter
  // which is the repeat interval (millis / 20 basically) Repeat button does repeat as we can see.
  switches.addSwitch(spinwheelClickPin1, onSpinwheelClicked1); //, encoder1, encoderEvent1);
  switches.addSwitch(spinwheelClickPin2, onSpinwheelClicked2); //, encoder1, encoderEvent1);

    // Converted to copy the arrays.
    for (byte i = 0; i < ROWS; i++)
      keyLayout.setRowPin(i, rowPins[i]);
    for (byte i = 0; i < COLS; i++)
      keyLayout.setColPin(i, colPins[i]);

    /// create the keyboard mapped to arduino pins and with the layout chosen above.
    /// It will callback our listener
    keyboard.initialise(arduinoIo, &keyLayout, &myListener);

    /// start repeating at 850 millis then repeat every 350ms
    keyboard.setRepeatKeyMillis(850, 350);

  taskManager.scheduleFixedRate(1000, [] {
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    //lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    float secondsFraction =  millis() / 1000.0F;
    drawingEvent.setLatestStatus(secondsFraction);
#ifdef SERIAL_OUTPUT
    Serial.println(secondsFraction);
#endif
  });

  taskManager.scheduleFixedRate(5000, [] {
    // Put in to replace any corruption.
    drawingEvent.redraw();
  });

    taskManager.registerEvent(&encoderEvent1);
    taskManager.registerEvent(&encoderEvent2);

    taskManager.registerEvent(&drawingEvent);
    taskManager.scheduleFixedRate(250, processSerialInput);

    Serial.println("> Display, keyboard, 2 encoders and encoderEvents are initialised!");
    Serial << F("> ready") << endl << endl;

}

/// @brief ISR routine now calls the encoder and also the encoderEvent as well.
ISR(PCINT2_vect)  /// Pin A8 to A11 interrupt vector
{
byte pins = PINK & 0b00001111;
byte change = pins ^ lastPins;
lastPins = pins;

 if ( change & 0b000011)
 {
    encoder1.encoderISR();
    encoderEvent1.markTriggeredAndNotify();
 }
 if ( change & 0b00001100)
 {
    encoder2.encoderISR();
    encoderEvent2.markTriggeredAndNotify();
 }
}

void printConfig(void) {

  // code version
  Serial << F("> code version = ") << VER_MAJ << VER_MIN << F(" beta ") << VER_BETA << endl;
  Serial << F("> compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(", compiler ver = ") << __cplusplus << endl;
  Serial << F("> Code defines VERSION ") << VERSION << endl;
#if CANBUS8MHZ
  Serial << F("> Set for 8Mhz crystal") << endl;
#endif
  Serial << F("> CS Pin ") << CHIPSELECT << endl;
  Serial << F("> INT Pin ") << CBUSINTPIN << endl; 
// copyrights collected so far
  Serial << F("> © Duncan Greenwood (MERG M5767) 2019") << endl;
  Serial << F("> © John Fletcher (MERG M6777) 2019") << endl;
#if DEBUG
  #if CBUS_EVENTS
    Serial << F("> CBUS Events available") << endl;
  #endif
#endif

  return;
}



void loop() {

  ///
  /// do CBUS message, switch and LED processing
  //
  CBUS.process();

  //

  /// process console commands now in a task
  //

  //processSerialInput();

/** as this indirectly uses taskmanager, we must include taskManager.runLoop(); in loop. */
    taskManager.runLoop();
}

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

