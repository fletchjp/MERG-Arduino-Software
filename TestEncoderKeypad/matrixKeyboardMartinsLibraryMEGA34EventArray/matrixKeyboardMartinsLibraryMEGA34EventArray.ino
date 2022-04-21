/// @file matrixKeyboardMartinsLibraryMEGA34EventArray.ino
/// @brief Example of matrix keyboard support for MEGA 3 by 4 Keypad built into IoAbstraction using Array for Martin's encoders via an event
///
/// This now runs Martin's encoder library integrated in the IoAbstraction code.
///
/// I am now extending to two encoders using the method devised by Martin for this.
/// I now want to put the encoders into an array for use with CANCMDDC.
/// This is a problem so I am exploring how to do it in this example.
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
#include <IoAbstraction.h>
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

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
/// Encoder names to be consistent
const char encoder_name1 = '1';
const char encoder_name2 = '2';

/// the maximum (0 based) value that we want the encoder to represent.
const int maximumEncoderValue = 128;

/// an LED that flashes as the encoder changes
const int ledOutputPin = 13;

/// Array and class
#define NUM_CONTROLLERS 2

// Wrapper class to provide the interface.
// Note that the encoderEvents cannot be copied and so are not included here.
// The encoders are declared outside this class.
// This means that this wrapper class can look like the previous encoder class
// to the main program while using a totally different encoder library.
class encoderControllerClass
{
  private:
      EncoderMD &enc;
      bool    ignorePush = false;
      int spinwheelClickPin;
      char encoder_name;
  // -------------------------------------------------

  public:
      uint8_t       newPos = 0;
      uint8_t       lastPos = 0;
      unsigned long push = 0;
  // -------------------------------------------------
  /// Constructor from encoder which already exists
  encoderControllerClass (EncoderMD &encoder,uint8_t setPinS,char enc_name = 'x') : enc(encoder) {
      spinwheelClickPin = setPinS;
      encoder_name = enc_name;
  }
  /// Added for use in onSpinWheelClicked routines
  char get_name() const { return encoder_name; }
  /// read, write and pushbutton code still needed here.                     
};


#if SWAP_PINS
/// Use thess settings for right hand turn to increase the value.
EncoderMD encoder1(encoderBPin1,encoderAPin1);
EncoderMD encoder2(encoderBPin2,encoderAPin2);
#else
/// Use these settings for left hand turn to increase the value.
EncoderMD encoder1(encoderAPin1,encoderBPin1);
EncoderMD encoder2(encoderAPin2,encoderBPin2);
#endif

/// Now construct the encoders first and then put then into the array.
struct {
  encoderControllerClass encoderController;
} encoders[NUM_CONTROLLERS] = {
                {encoderControllerClass(encoder1, spinwheelClickPin1,encoder_name1)},
                {encoderControllerClass(encoder2, spinwheelClickPin2,encoder_name2)}
};

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
EncoderEvent encoderEvent1(encoder1,encoder_name1);
EncoderEvent encoderEvent2(encoder2,encoder_name2);

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
        //Serial.print(encoders[1].get_name());
        Serial.print(" ");
        Serial.println(encoderEvent1.RotaryPosition);
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
        Serial.print("2 ");
        Serial.println(encoderEvent2.RotaryPosition);
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

/// Adapted from Key.h and made into a scoped enum.
enum class KeyState : byte { IDLE, PRESSED, HOLD, RELEASED };

/// This is called from the listener to report on the state.
void tell_the_state(char key_val, KeyState key_state = KeyState::IDLE) {
  //if (key_val != old_key) {
    Serial.print(F("Key "));
    Serial.print(key_val);
    if (key_state == KeyState::PRESSED) { 
      Serial.println(F(" pressed"));
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

void setup() {
/// setup 
    while(!Serial);
    Serial.begin(115200);

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

    taskManager.registerEvent(&encoderEvent1);
    taskManager.registerEvent(&encoderEvent2);

    Serial.println("Keyboard, 2 encoders and encoderEvents are initialised!");
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
   // Runs if rotation was detected
   /*
  RotaryPosition = encoder.getPosition();
  TurnDetected = (RotaryPosition != PrevPosition);
  if (TurnDetected)  {
    PrevPosition = RotaryPosition; // Save previous position in variable
    Serial.println(RotaryPosition);
  }
  */
}
