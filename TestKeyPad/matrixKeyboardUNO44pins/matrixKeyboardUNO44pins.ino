/// @file matrixKeyboardUNO44pins.ino
/// @brief Example of matrix keyboard support for UNO 4 by 4 Keypad built into IoAbstraction
///
/// Allowing for different pin start no.
///
/// This example shows how to use the matrix keyboard support that's built into IoAbstraction,
/// it can be used out the box with either a 3x4 or 4x4 keypad, but you can modify it to use
/// any matrix keyboard quite easily.
/// It just sends the characters that are typed on the keyboard to Serial. The keyboard in This
/// example is connected directly to Arduino pins, but could just as easily be connected over
/// a PCF8574, MCP23017 or other IoAbstraction.
/// This version sets up the keyboard adapting the custom_keyboard example.
 
#include <Wire.h>
#include <IoAbstraction.h>
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

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
const byte COLS = 4; /// four columns
/// define the symbols on the buttons of the keypads
/// PROGMEM is important.
const char layout[] PROGMEM = "123A456B789C*0#D"; // Chars have to be in a string.
//  '1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'
//};
/// These are in order of Keypad pins from 1 to 8.
/// Pin 1 is on the left with the pad face up.
const byte PIN_START = 3; // ONE less than the first pin.
byte rowPins[ROWS] = {9+PIN_START, 8+PIN_START, 7+PIN_START, 6+PIN_START}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5+PIN_START, 4+PIN_START, 3+PIN_START, 2+PIN_START}; //connect to the column pinouts of the keypad
/// This seems fussy. ROWS and COLS will not work here.
uint8_t rows = ROWS;
uint8_t cols = COLS;
/// Set up the keyLayout
KeyboardLayout keyLayout(rows, cols, layout);

///
/// We need a keyboard manager class too
///
MatrixKeyboardManager keyboard;

/// this example connects the pins directly to an arduino but you could use
/// IoExpanders or shift registers instead.
IoAbstractionRef arduinoIo = ioUsingArduino();

///
/// We need a class that extends from KeyboardListener. This gets notified when
/// there are changes in the keyboard state.
///
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

void setup() {
/// setup 
    while(!Serial);
    Serial.begin(115200);

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

    Serial.println("Keyboard is initialised!");
}

void loop() {
/** as this indirectly uses taskmanager, we must include taskManager.runLoop(); in loop. */
    taskManager.runLoop();
}
