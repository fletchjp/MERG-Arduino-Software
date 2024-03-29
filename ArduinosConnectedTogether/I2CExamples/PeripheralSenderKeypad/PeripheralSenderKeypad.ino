/// @file PeripheralSenderKeypad.ino
/// @brief Wire Peripheral Sender for a 4 by 4 keypad
/// adapted from Nicholas Zambetti <http://www.zambetti.com>
///
/// Demonstrates use of the Wire library
/// Sends data as an I2C/TWI peripheral device
/// Refer to the "Wire Master Reader" example for use with this
///
/// Created 29 March 2006
///
/// This example code is in the public domain.


#include <Wire.h>
#include <IoAbstraction.h>
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

const byte ROWS = 4; /// four rows
const byte COLS = 4; /// four columns
/// define the symbols on the buttons of the keypads
/// PROGMEM is important.
const char layout[] PROGMEM = "123A456B789C*0#D"; // Chars have to be in a string.
//  '1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'

/// These are in order of Keypad pins from 1 to 8.
/// Pin 1 is on the left with the pad face up.
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
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
uint8_t payload[10];

class MyKeyboardListener : public KeyboardListener {
public:
    void keyPressed(char key, bool held) override {
        Serial.print("Key ");
        Serial.print(key);
        Serial.print(" is pressed, held = ");
        Serial.println(held);
        // Note conversion of the 
        payload[0] = key;
    }
    void keyReleased(char key) override {
        Serial.print("Released ");
        Serial.println(key);
    }
} myListener;

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("Peripheral Sender Keypad running");
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event

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
  taskManager.runLoop();
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(payload[0]); // respond with message of 1 byte
  // as expected by master
}
