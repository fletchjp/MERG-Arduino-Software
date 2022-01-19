/// @file matrixKeyboardI2C34.ino
/// @brief Example of matrix keyboard support for 3 by 4 Keypad built into IoAbstraction on I2C
///
/// The example is now working. A call to Wire.begin() is needed.
///
/// This example shows how to use the matrix keyboard support that's built into IoAbstraction,
/// it can be used out the box with either a 3x4 or 4x4 keypad, but you can modify it to use
/// any matrix keyboard quite easily.
///
/// It just sends the characters that are typed on the keyboard to Serial. The keyboard in This
/// example is connected directly to a PCF8574.
///
/// This version sets up the keyboard adapting the custom_keyboard example.
 
#include <Wire.h>
#include <IoAbstraction.h>
#include <IoAbstractionWire.h>
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

/// setup runs once
void setup() {
    while(!Serial);
    Serial.begin(115200);
    /// Wire.begin(); was missing!!
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

    /// start repeating at 850 millis then repeat every 350ms
    keyboard.setRepeatKeyMillis(850, 350);

    Serial.println("Keyboard is initialised!");
}

void loop() {
/** as this indirectly uses taskmanager, we must include taskManager.runLoop(); in loop. */
    taskManager.runLoop();
}
