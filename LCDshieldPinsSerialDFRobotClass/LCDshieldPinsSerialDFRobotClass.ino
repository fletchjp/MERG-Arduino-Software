// LCD Shield Pins Serial with Class
// using DfRobotInputAbstraction.h to model input pins.

// LCD Shield with Buttons DFRobot model 

#include <IoAbstraction.h>
#include <DfRobotInputAbstraction.h>
#include <TaskManagerIO.h>
#include <DeviceEvents.h>

// This is the input pin where analog input is received.
// It is in fact set as default defining dfRobotKeys.
#define ANALOG_IN_PIN A0

// here we create the abstraction over the standard arduino analog IO capabilities
// ArduinoAnalogDevice analog; // by default it assumes 10 bit read, 8 bit write
// This uses the default settings for analog ranges.
IoAbstractionRef dfRobotKeys = inputFromDfRobotShield();

// LCD shield with buttons example code

/*
Arduino 2x16 LCD - Detect Buttons
modified on 18 Feb 2019
by Saeed Hosseini @ Electropeak
https://electropeak.com/learn/
Using data from here:
https://wiki.dfrobot.com/Arduino_LCD_KeyPad_Shield__SKU__DFR0009_
I have also played with the values.
*/
#include <LiquidCrystal.h>
//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

//int x;
//int prevx = 0;
//int range;
//int prevrange = 0;

// Serial IO
#define SERIAL_SPEED            19200   // Speed of the serial port.

void logKeyPressed(const char* whichKey, bool heldDown) {
    Serial.print("Key ");
    Serial.print(whichKey);
    lcd.setCursor(10,1);
    lcd.print (whichKey);
    Serial.println(heldDown ? " Held" : " Pressed");
}

/**
 * Along with using functions to receive callbacks when a button is pressed, we can
 * also use a class that implements the SwitchListener interface. Here is an example
 * of implementing that interface. You have both choices, function callback or
 * interface implementation.
 */
class MyKeyListener : public SwitchListener {
private:
    const char* whatKey;
public:
    // This is the constructor where we configure our instance
    MyKeyListener(const char* what) {
        whatKey = what;
    }

    // when a key is pressed, this is called
    void onPressed(pinid_t /*pin*/, bool held) override {
        logKeyPressed(whatKey, held);
    }

    // when a key is released this is called.
    void onReleased(pinid_t /*pin*/, bool held) override {
        Serial.print("Release ");
        logKeyPressed(whatKey, held);
    }
};

MyKeyListener selectKeyListener("SELECT");



void setup() {
  // put your setup code here, to run once:
  // set up the device pin directions upfront.
  // analog.initPin(ANALOG_IN_PIN, DIR_IN);

  // Initialise
  while(!Serial);
  Serial.begin(SERIAL_SPEED);             // Start Serial IO.
  Serial.println("DFRobotTaskClass with pin translation");
  //analogWrite(pin_d6,50);
 lcd.begin(16, 2);
 lcd.setCursor(0,0);
 lcd.print("DFRobotTaskClass");
 lcd.setCursor(0,1);
 lcd.print("Press Key:");

    // initialise the switches component with the DfRobot shield as the input method.
    // Note that switches is the sole instance of SwitchInput
    switches.initialise(dfRobotKeys, false); // df robot is always false for 2nd parameter.

    // now we add the switches, each one just logs the key press, the last parameter to addSwitch
    // is the repeat frequency is optional, when not set it implies not repeating.
    switches.addSwitch(DF_KEY_DOWN, [](pinid_t /*pin*/, bool held) { logKeyPressed("DOWN  ", held);}, 20);
    switches.addSwitch(DF_KEY_UP, [](pinid_t /*pin*/, bool held) { logKeyPressed("UP   ", held);}, 20);
    switches.addSwitch(DF_KEY_LEFT, [](pinid_t /*pin*/, bool held) { logKeyPressed("LEFT  ", held);}, 20);
    switches.addSwitch(DF_KEY_RIGHT, [](pinid_t /*pin*/, bool held) { logKeyPressed("RIGHT ", held);}, 20);
    switches.onRelease(DF_KEY_RIGHT, [](pinid_t /*pin*/, bool) { Serial.println("RIGHT has been released");});
    
    switches.addSwitchListener(DF_KEY_SELECT, &selectKeyListener);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  taskManager.runLoop();

}
