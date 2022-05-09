/// @file matrixKeyboardMartinsEncoderEventI2C44.ino
/// @brief Example of matrix keyboard support for 4 by 4 Keypad built into IoAbstraction on I2C adding Martin's encoder.
///
/// The example is not working with the encoder.
///
/// A call to Wire.begin() is needed.
///
/// This example shows how to use the matrix keyboard support that's built into IoAbstraction,
/// it can be used out the box with either a 3x4 or 4x4 keypad, but you can modify it to use
/// any matrix keyboard quite easily.
///
/// It just sends the characters that are typed on the keyboard to Serial. The keyboard in This
/// example is connected directly to a PCF8574.
///
/// This version sets up the keyboard adapting the custom_keyboard example.
///
/// Adding in the code for the encoder.
///
/// I am going to configure this for a UNO
///
/// Swap the pins to get the opposite action
#define SWAP_PINS 0

 
#include <Wire.h>
#include <IoAbstraction.h>
#include <IoAbstractionWire.h>
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

#include "EncoderMD.h"

boolean TurnDetected;

const int PinCLK=2;   // Generating interrupts using CLK signal
const int PinDT=3;    // Reading DT signal
const int PinSW=4;     // Reading Push Button switch
// Also connect +5V and ground.

#if SWAP_PINS
EncoderMD encoder(PinDT,PinCLK);
#else
EncoderMD encoder(PinCLK,PinDT);
#endif

int RotaryPosition=0;    // To store Stepper Motor Position

int PrevPosition;     // Previous Rotary position Value to check accuracy

void setupPCI()
{
  cli();
  PCICR  |= 0b00000100;  //Set Pin Change Interrupt on Register D
  PCMSK2 |= 0b00001100;  //Set pins 2 & 3 for interrupt
  sei();
}

ISR(PCINT2_vect)  // Pin 2 & 3 interrupt vector
{
  encoder.encoderISR();
}

const int POWER_PIN=12; //Extra 5V output on pin 12.
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
/// four columns
const byte COLS = 4; 
/// define the symbols on the buttons of the keypads
/// PROGMEM is important.
const char layout[] PROGMEM = "123A456B789C*0#D"; // Chars have to be in a string.
//  '1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'
//};
/// These are in order of Keypad pins from 1 to 8.
/// Pin 1 is on the left with the pad face up.
byte rowPins[ROWS] = {100, 101, 102, 103}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {104, 105, 106, 107}; //connect to the column pinouts of the keypad
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
    //while(!Serial);
    Serial.begin(115200);
    /// Wire.begin(); was missing!!
    //Wire.begin();
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
    setupPCI();
    // These are done in the encoder constructor
    //pinMode(PinCLK,INPUT);
    //pinMode(PinDT,INPUT);  
    //pinMode(POWER_PIN,OUTPUT);
    //digitalWrite(POWER_PIN, HIGH);
    pinMode(PinSW,INPUT);
    digitalWrite(PinSW, HIGH); // Pull-Up resistor for switch
    encoder.setLimits(0,100);

    Serial.println("Keyboard and encoder are initialised!");
}

void loop() {
/** as this indirectly uses taskmanager, we must include taskManager.runLoop(); in loop. */
    taskManager.runLoop();

      if (!(digitalRead(PinSW))) {   // check if button is pressed
    if (RotaryPosition == 0) {  // check if button was already pressed
       } else {
        //small_stepper.step(-(RotaryPosition*50));
        RotaryPosition=0; // Reset position to ZERO
        encoder.setPosition(RotaryPosition);
        Serial.print("X ");
        Serial.println(RotaryPosition);
        PrevPosition = RotaryPosition;
      }
    }

   // Runs if rotation was detected
  RotaryPosition = encoder.getPosition();
  TurnDetected = (RotaryPosition != PrevPosition);
  if (TurnDetected)  {
    PrevPosition = RotaryPosition; // Save previous position in variable
    Serial.println(RotaryPosition);
  }

}
