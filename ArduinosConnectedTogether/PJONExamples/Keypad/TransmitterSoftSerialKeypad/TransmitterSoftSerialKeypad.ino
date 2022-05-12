/// @file TransmitterSoftSerialKeypad.ino
/// @brief BlinkTestTransmitter using a Soft Serial port and Keypad
/// Using https://arduinojson.org/v6/how-to/do-serial-communication-between-two-boards/

#include <PJONThroughSerial.h>
#include <SoftwareSerial.h>

// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(10, 11); // RX, TX
//Wires are crossed over

PJONThroughSerial bus(45);

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
        bus.send(44, payload, 1); // Send key to device 44
    }
    void keyReleased(char key) override {
        Serial.print("Released ");
        Serial.println(key);
    }
} myListener;

void setup() {

  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("TransmitterSoftSerial running with Keypad");

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


  linkSerial.begin(4800);
  bus.strategy.set_serial(&linkSerial); // Pass the Serial object you want to use for communication
  bus.begin();
  bus.send_repeatedly(44, "X", 1, 1000000); // Send X to device 44 every second
  Serial.println("PJON bus running");
};

void loop() {
/** as this indirectly uses taskmanager, we must include taskManager.runLoop(); in loop. */
  taskManager.runLoop();
  bus.update();
};
