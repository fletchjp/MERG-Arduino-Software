/// @file MEGATaskIntegrationI2CData
/// @brief extend to have a 20 by 4 display as well and send some data
///
/// This now sends the time data to the peripheral.
///
/// This now uses the display to show the time elapsed and the keypad and encoder values.
///
/// Using another I2C link to connect to another Arduino for the PJON connection.
/// Code for this comes from ControllerTwoWayTaskClass.
/// This does not work with the Arduino and the display connected to different pins.
/// I have to find a way to chain the connections.
/// I had made the mistake of using two different sets of I2C pins from the MEGA.
/// This messes things up. NO. I think the two sets of pins are directly connected.
/// The problem is in the timing of the I2C accesses and I need to work on that.
/// I am going to attempt to resolve this using the timeout facilities of the Wire library.
///
/// I have made a lot of progress and the transmission to the peripheral Arduino now works.
/// I have still to work out how best to do the reverse transfer.
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
/// It just sends the characters that are typed on the keyboard to Serial. The keyboard in This
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

#include <Wire.h>
// This is true for the AVR version and should be defined in the library.
#define WIRE_HAS_TIMEOUT
#include <IoAbstraction.h>
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

/////////////////////////////////////////////////////////////
// New section for the I2C connection to another Arduino
// Code for this comes from ControllerTwoWayTaskClass.
/////////////////////////////////////////////////////////////
#define TO_CONTROLLER_SIZE 3
#define TO_PERIPHERAL_SIZE  8

#define PERIPHERAL_NODE   8 // The starting I2C address of peripheral node

byte messageToController[TO_CONTROLLER_SIZE];
byte messageToPeripheral[TO_PERIPHERAL_SIZE];
byte messageBuffer[TO_PERIPHERAL_SIZE];

void sendToPeripheral(int address, float value) {
  // Information on the conversion from float to char.
  // https://arduino.stackexchange.com/questions/49787/convert-byte-and-float-to-char
  dtostrf(value,3,2,messageBuffer);
  snprintf(messageToPeripheral,TO_PERIPHERAL_SIZE,messageBuffer);
  // message is 0123
  //for(int i = 0; i < TO_PERIPHERAL_SIZE; i++) {
  //  messageToPeripheral[i] = (byte)i;  
  //}
  Serial.print("About to send ");
  Serial.println(value);
  Wire.beginTransmission(address);
  // This queues the data which are sent with endTransmission.
  Wire.write(messageToPeripheral, TO_PERIPHERAL_SIZE);
#ifdef WIRE_HAS_TIMEOUT
  byte error = Wire.endTransmission(); // run transaction
  if (error) {
    Serial.println("Error occured when writing");
    if (error == 5) {
      Serial.println("It was a timeout");
      Wire.clearWireTimeoutFlag();
    } else {
      Serial.print("Error number "); 
      Serial.println(error);
    }
  }
#else
  Wire.endTransmission();
#endif
}

void readFromPeripheral() {
  #if defined(WIRE_HAS_TIMEOUT)
  Wire.clearWireTimeoutFlag();
  #endif
  // if data size is available from nodes
  uint8_t what_results = Wire.requestFrom(8, 6);    // request 6 bytes from peripheral device #8
  Serial.print(what_results);
  Serial.println(" bytes available");
  Serial.print("reading ");
  if (what_results == 0) {
#if defined(WIRE_HAS_TIMEOUT)
    if (Wire.getWireTimeoutFlag()) {
        Serial.println("It was a timeout");
        Wire.clearWireTimeoutFlag();
    }
#endif
  } else {
  //Serial.print(Wire.available());
  //Serial.println(" characters");
  while (Wire.available()) { // peripheral may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
    Serial.println(" ");
  }
}

class SendAndReceive : public Executable {
   int address;
    float elapsed_time;
    volatile bool emergency; // if an event comes from an external interrupt the variable must be volatile.
    bool timeChanged;
    bool hasChanged;
   public:
   SendAndReceive(int addr) : address(addr) { 
      elapsed_time = 0.0f;
      emergency = false;
      hasChanged = false;
      timeChanged = false;    
   }
   void exec() override {
     if (hasChanged) {
      sendToPeripheral(address, elapsed_time);
      Serial.print("Sent to ");
      Serial.println(address);
      hasChanged = false;
      readFromPeripheral();
      Serial.println("Received");
     }
   }
    /**
     * This is called by task manager every time the number of microseconds returned expires, if you trigger the
     * event it will run the exec(), if you complete the event, it will be removed from task manager.
     * @return the number of micros before calling again.
     */
     /*
    uint32_t timeOfNextCheck() override {
        setTriggered(hasChanged);
        return millisToMicros(500); // no point refreshing more often on an LCD, as its unreadable
    }
    */
    void setLatestStatus(float value) {
        elapsed_time = value;
        hasChanged = true;// we are happy to wait out the 500 millis
        timeChanged = true;
    }

};

SendAndReceive sendAndReceive(PERIPHERAL_NODE);

/////////////////////////////////////////////////////////////

#include <LiquidCrystalIO.h>

// When using the I2C version, these two extra includes are always needed. Doing this reduces the memory slightly for
// users that are not using I2C.
#include <IoAbstractionWire.h>
#include <Wire.h>

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
     * This sets the latest time, but only marks the event changed,
     * so it will need to poll in order to trigger.
     * This prevents excessive screen updates.
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
           Serial.print(encoderName);
           Serial.print(" ");
           Serial.println(RotaryPosition);
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
void onSpinwheelClicked1(pinid_t pin, bool heldDown) { //, MyEncoder &encoder, EncoderEvent &encoderEvent) {
  Serial.print("Button 1 pressed ");
  Serial.println(heldDown ? "Held" : "Pressed");
    if (encoderEvent1.RotaryPosition == 0) {  // check if button was already pressed
       } else {
        encoderEvent1.RotaryPosition=0; // Reset position to ZERO
        encoder1.setPosition(encoderEvent1.RotaryPosition);
        Serial.print("X ");
        Serial.println(encoderEvent1.RotaryPosition);
        drawingEvent.setLatestPos('1',encoderEvent1.RotaryPosition);    
        encoderEvent1.PrevPosition = encoderEvent1.RotaryPosition;
      }
}

/// @brief When the spinwheel2 is clicked, this function will be run as we registered it as a callback
void onSpinwheelClicked2(pinid_t pin, bool heldDown) { //, MyEncoder &encoder, EncoderEvent &encoderEvent) {
  Serial.print("Button 2 pressed ");
  Serial.println(heldDown ? "Held" : "Pressed");
    if (encoderEvent2.RotaryPosition == 0) {  // check if button was already pressed
       } else {
        encoderEvent2.RotaryPosition=0; // Reset position to ZERO
        encoder2.setPosition(encoderEvent2.RotaryPosition);
        Serial.print("Y ");
        Serial.println(encoderEvent2.RotaryPosition);
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
     byte* get_keys() const { return stored; }
};

/// Adapted from Key.h and made into a scoped enum.
enum class KeyState : byte { IDLE, PRESSED, HOLD, RELEASED };

/// This is called from the listener to report on the state.
void tell_the_state(char key_val, KeyState key_state = KeyState::IDLE) {
  //if (key_val != old_key) {
    Serial.print(F("Key "));
    Serial.print(key_val);
    if (key_state == KeyState::PRESSED) { 
      Serial.println(F(" pressed"));
      drawingEvent.setLatestKey(key_val);    
    } else if (key_state == KeyState::HOLD) { 
      Serial.println(F(" held"));
      //old_key = key_val;
    } else if (key_state == KeyState::RELEASED) { 
      Serial.println(F(" released"));
      //old_key = key_val;
    } else {
      Serial.println(F(" idle"));
    }
  //}
}

/// @brief We need a class that extends from KeyboardListener.
///This gets notified when there are changes in the keyboard state.
///
class MyKeyboardListener : public KeyboardListener {
public:
    void keyPressed(char key, bool hold) override {
        Serial.print("Key ");
        Serial.print(key);
        Serial.print(" is pressed, hold = ");
        Serial.println(hold);
        if (hold) tell_the_state(key,KeyState::HOLD);
        else tell_the_state(key,KeyState::PRESSED);
    }

    void keyReleased(char key) override {
        Serial.print("Released ");
        Serial.println(key);
        tell_the_state(key,KeyState::RELEASED);
    }
} myListener;

void redraw_display() {
  lcd.setCursor(0,0);
  // Print a message to the LCD.
  lcd.print("Task Integration!");
  lcd.setCursor(0,1);
  lcd.print("Time (secs) ");
  lcd.setCursor(0,2);
  lcd.print("Keypad ");
  lcd.setCursor(0,3);
  lcd.print("Enc1 ");
  lcd.setCursor(10,3);
  lcd.print("Enc2 ");
}


#define WIRE Wire

// Imported from i2c_scanner
void i2c_scanner()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    WIRE.beginTransmission(address);
    error = WIRE.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else {
    Serial.print(nDevices);
    Serial.println(" devices found - done\n");
  }
  
}

void setup() {
/// setup 
    while(!Serial);
    Serial.begin(115200);
  // for i2c variants, this must be called first.
  Wire.begin();
  #if defined(WIRE_HAS_TIMEOUT)
    Wire.setWireTimeout(3000 /* us */, true /* reset_on_timeout */);
  #endif
  i2c_scanner();
  // set up the LCD's number of columns and rows, must be called.
  lcd.begin(20, 4);
  // The following code is now needed which it was not before.
  // most backpacks have the backlight on pin 3.
  lcd.configureBacklightPin(3);
  lcd.backlight();
  redraw_display();  

  setupPCI();
  encoder1.setLimits(0,maximumEncoderValue);
  encoder2.setLimits(0,maximumEncoderValue);
  encoder1.setPosition (0);
  encoder1.setWrap (0);
  encoder2.setPosition (0);
  encoder2.setWrap (0);


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
    sendAndReceive.setLatestStatus(secondsFraction);
    Serial.println(secondsFraction);
  });

  taskManager.scheduleFixedRate(5000, [] {
    // Put in to replace any corruption.
    drawingEvent.redraw();
  });

    taskManager.registerEvent(&encoderEvent1);
    taskManager.registerEvent(&encoderEvent2);

    taskManager.registerEvent(&drawingEvent);

 // This is at the end of setup()
    taskManager.scheduleFixedRate(1000, &sendAndReceive);

    Serial.println("Display, keyboard, 2 encoders and encoderEvents are initialised!");
#ifdef  WIRE_HAS_TIMEOUT
    Serial.println("with Wire timeout active");
#endif
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

void loop() {
/** as this indirectly uses taskmanager, we must include taskManager.runLoop(); in loop. */
    taskManager.runLoop();
}
