/// @file matrixKeyboardUNO44pinsI2CEncoderEvent.ino
/// @brief Example of matrix keyboard support for UNO 4 by 4 Keypad built into IoAbstraction
///
/// Adding encoder from MartinsEncoder and use an event for it.
///
/// Move keypad to I2C.
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
#include <IoAbstractionWire.h>
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

/// Declare that the first 100 pins are reserved for the Arduino.
MultiIoAbstractionRef multiIo = multiIoExpander(100);

/// the maximum (0 based) value that we want the encoder to represent.
const int maximumEncoderValue = 128;

/// an LED that flashes as the encoder changes
const int ledOutputPin = 13;


/// I am going to configure this for a UNO or NANO
///
/// Swap the pins to get the opposite action
#define SWAP_PINS 0

#include "EncoderMD.h"

boolean TurnDetected;

const int PinCLK=2;   // Generating interrupts using CLK signal
const int PinDT=3;    // Reading DT signal
const int PinSW=4;     // Reading Push Button switch
// Also connect +5V and ground.

#if SWAP_PINS
EncoderMD encoder1(PinDT,PinCLK);
#else
EncoderMD encoder1(PinCLK,PinDT);
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
           delay(5);// to give time for the print buffer to clear      
           PrevPosition = RotaryPosition; // Save previous position in variable
           Serial.print(encoderName);
           Serial.print(" ");
           Serial.println(RotaryPosition);
           // here we turn the led on and off as the encoder moves.
           ioDeviceDigitalWriteS(multiIo, ledOutputPin, RotaryPosition % 2);
         }
    }
    /**
     * We should always provide a destructor.
     */
    ~EncoderEvent() override = default;
};

/// Declare encoderEvent instances for each encoder.
EncoderEvent encoderEvent1(encoder1,'1');

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
        encoderEvent1.PrevPosition = encoderEvent1.RotaryPosition;
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
const byte COLS = 4; /// four columns
/// define the symbols on the buttons of the keypads
/// PROGMEM is important.
const char layout[] PROGMEM = "123A456B789C*0#D"; // Chars have to be in a string.
//  '1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'
//};
/// These are in order of Keypad pins from 1 to 8.
/// Pin 1 is on the left with the pad face up.
byte rowPins[ROWS] = {100, 101, 102, 103}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {104, 105, 106, 107}; //connect to the column pinouts of the keypad
//const byte PIN_START = 3; // ONE less than the first pin.
//byte rowPins[ROWS] = {9+PIN_START, 8+PIN_START, 7+PIN_START, 6+PIN_START}; //connect to the row pinouts of the keypad
//byte colPins[COLS] = {5+PIN_START, 4+PIN_START, 3+PIN_START, 2+PIN_START}; //connect to the column pinouts of the keypad
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
    setupPCI();
    encoder1.setLimits(0,maximumEncoderValue);
    encoder1.setPosition (0);
    encoder1.setWrap (0);
  // These are done in the encoder constructor
  //pinMode(PinCLK,INPUT);
  //pinMode(PinDT,INPUT);  
    pinMode(PinSW,INPUT);
    digitalWrite(PinSW, HIGH); // Pull-Up resistor for switch
    // here we initialise as output the output pin we'll use
    ioDevicePinMode(multiIo, ledOutputPin, OUTPUT);

    while(!Serial);
    Serial.begin(115200);
    Wire.begin();
    /// Add 10 pins from 100 up.
    multiIoAddExpander(multiIo, ioFrom8574(0x20), 10);
  // First we set up the switches library, giving it the task manager and tell it to use arduino pins
  // We could also of chosen IO through an i2c device that supports interrupts.
  // If you want to use PULL DOWN instead of PULL UP logic, change the true to false below.
    switches.initialise(multiIo, true);

  // now we add the switches, we dont want the spinwheel button to repeat, so leave off the last parameter
  // which is the repeat interval (millis / 20 basically) Repeat button does repeat as we can see.
    switches.addSwitch(PinSW, onSpinwheelClicked1); //, encoder1, encoderEvent1);

    // Converted to copy the arrays.
    for (byte i = 0; i < ROWS; i++)
      keyLayout.setRowPin(i, rowPins[i]);
    for (byte i = 0; i < COLS; i++)
      keyLayout.setColPin(i, colPins[i]);

    /// create the keyboard mapped to arduino pins and with the layout chosen above.
    /// It will callback our listener
    keyboard.initialise(multiIo, &keyLayout, &myListener);
    //keyboard.initialise(arduinoIo, &keyLayout, &myListener);

    /// start repeating at 850 millis then repeat every 350ms
    keyboard.setRepeatKeyMillis(850, 350);

    taskManager.registerEvent(&encoderEvent1);

    Serial.println("Keyboard and encoder are initialised with events!");
}

ISR(PCINT2_vect)  // Pin 2 & 3 interrupt vector
{
    encoder1.encoderISR();
    encoderEvent1.markTriggeredAndNotify();
}

void loop() {
/** as this indirectly uses taskmanager, we must include taskManager.runLoop(); in loop. */
    taskManager.runLoop();

}
