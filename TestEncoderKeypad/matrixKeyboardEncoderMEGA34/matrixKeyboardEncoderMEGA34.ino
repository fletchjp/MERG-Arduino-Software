/// @file matrixKeyboardEncoderMEGA34.ino
/// @brief Example of matrix keyboard support for MEGA 3 by 4 Keypad built into IoAbstraction adding encoder
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
/// MEGA pin settings from the CANCMDDC code
/// Digital pin 38             Encoder 1 Switch (SW on encoder)
/// Digital pin 40             Encoder 2 Switch (SW on encoder)
/// Digital / Analog pin 0     Encoder 1 A (CLK on encoder)
/// Digital / Analog pin 1     Encoder 2 A (CLK on encoder)
/// Digital / Analog pin 8     Encoder 1 B (DT  on encoder)
/// Digital / Analog pin 9     Encoder 2 B (DT  on encoder)

#include <Wire.h>
#include <IoAbstraction.h>
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

// The pin onto which we connected the rotary encoders switch
const int spinwheelClickPin = 38; /// SW on encoder
// The two pins where we connected the A and B pins of the encoder. I recomend you dont change these
// as the pin must support interrupts.
// IoAbstraction does not have an option to turn off the interrupt.
const int encoderAPin = 2; //A0; /// CLK on encoder 
const int encoderBPin = 3; //A8; /// DT  on encoder
// the maximum (0 based) value that we want the encoder to represent.
const int maximumEncoderValue = 128;

// an LED that flashes as the encoder changes
const int ledOutputPin = 13;

/// This is used in the buttonRotaryEncoder code.
///auto boardIo = internalDigitalIo();
/// Reading the source code ioUsingArduino is a synonym on Arduino systems.
/// So I will use arduinoIo for boardIo.

/// this example connects the pins directly to an arduino but you could use
/// IoExpanders or shift registers instead.
IoAbstractionRef arduinoIo = ioUsingArduino();

//
// When the spinwheel is clicked, this function will be run as we registered it as a callback
//
void onSpinwheelClicked(pinid_t pin, bool heldDown) {
  Serial.print("Button pressed ");
  Serial.println(heldDown ? "Held" : "Pressed");
}

//
// Each time the encoder value changes, this function runs, as we registered it as a callback
//
void onEncoderChange(int newValue) {
  Serial.print("Encoder change ");
  Serial.println(newValue);

  // here we turn the led on and off as the encoder moves.
  ioDeviceDigitalWriteS(arduinoIo, ledOutputPin, newValue % 2);
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

///
/// We need a class that extends from KeyboardListener. This gets notified when
/// there are changes in the keyboard state.
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

  // here we initialise as output the output pin we'll use
  ioDevicePinMode(arduinoIo, ledOutputPin, OUTPUT);

  // First we set up the switches library, giving it the task manager and tell it to use arduino pins
  // We could also of chosen IO through an i2c device that supports interrupts.
  // If you want to use PULL DOWN instead of PULL UP logic, change the true to false below.
  switches.initialise(arduinoIo, true);

  // now we add the switches, we dont want the spinwheel button to repeat, so leave off the last parameter
  // which is the repeat interval (millis / 20 basically) Repeat button does repeat as we can see.
  switches.addSwitch(spinwheelClickPin, onSpinwheelClicked);

  // now we set up the rotary encoder, first we give the A pin and the B pin.
  // we give the encoder a max value of 128, always minumum of 0.
  setupRotaryEncoderWithInterrupt(encoderAPin, encoderBPin, onEncoderChange);
  switches.changeEncoderPrecision(maximumEncoderValue, 100);

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

    Serial.println("Keyboard and encoder are initialised!");
}

void loop() {
/** as this indirectly uses taskmanager, we must include taskManager.runLoop(); in loop. */
    taskManager.runLoop();
}
