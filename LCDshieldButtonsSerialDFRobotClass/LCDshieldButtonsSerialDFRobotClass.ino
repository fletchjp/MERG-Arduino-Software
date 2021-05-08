// LCD Shield Buttons Serial with Class using analogInput
// DFRobot shield version

#include <IoAbstraction.h>
#include <AnalogDeviceAbstraction.h>
#include <TaskManagerIO.h>
#include <DeviceEvents.h>

// This is the input pin where analog input is received.
#define ANALOG_IN_PIN A0

// here we create the abstraction over the standard arduino analog IO capabilities
ArduinoAnalogDevice analog; // by default it assumes 10 bit read, 8 bit write

int previous_analog = 0;
int analog_value;

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

int x;
int prevx = 0;
int range;
int prevrange = 0;
int y = 0;

// Serial IO
#define SERIAL_SPEED            19200   // Speed of the serial port.


// Here we create an analog event that will be triggered when the the analog level exceeds 75%. it is triggered every
// 100 milliseconds and when triggered runs the code in the exec() method.
class MyAnalogExceedsEvent : public AnalogInEvent {
public:
    MyAnalogExceedsEvent(AnalogDevice* device, pinid_t pin) :
            AnalogInEvent(device, pin, 0.75, AnalogInEvent::ANALOGIN_EXCEEDS, 100000UL) {
    }

    void exec() override {
        Serial.print("Trigger AnalogInEvent Threshold ");
        Serial.print(analogThreshold);
        Serial.print(", value ");
        Serial.println(lastReading);
    }
};

// This is the previous way of working with a direct read.
void checkA0()
{
 x = analogRead (0);
 if (x < 175){         // was 50
  range = 1;
 } else if (x < 350){ // was 250
  range = 2;
 } else if (x < 500){ // unchanged
  range = 3;
 } else if (x < 800){ // was 650
  range = 4;
 } else if (x < 850){ // unchanged
  range = 5;
 } //else { range = 0; }

 if (range != prevrange) {
 Serial.print(range);
 Serial.print(" ");
 Serial.print(x);
 lcd.setCursor(10,1);
 switch (range) {
  case 1:
  {
   lcd.print ("Right ");
   //if (y == 0) {
   Serial.println(" Right");
     //y = 1;
   //}
   break;
  }
  case 2:
  {
   lcd.print ("Up    ");
   Serial.println(" Up");
   break;
  }
  case 3:
  {
   lcd.print ("Down  ");
   Serial.println(" Down");
   break;
  }
  case 4:
  {
   lcd.print ("Left  ");
   Serial.println(" Left ");
   break;
  }
  case 5:
  {
   lcd.print ("Select");
   Serial.println(" Select");
   break;
  }
  default:
  break;
 }
 prevrange = range;
 }
  
}

void setup() {
  // put your setup code here, to run once:
  // set up the device pin directions upfront.
  analog.initPin(ANALOG_IN_PIN, DIR_IN);

  // Initialise
  Serial.begin(SERIAL_SPEED);             // Start Serial IO.
  Serial.println("DFRobotTaskClass");
  //analogWrite(pin_d6,50);
 lcd.begin(16, 2);
 lcd.setCursor(0,0);
 lcd.print("DFRobotTaskClass");
 lcd.setCursor(0,1);
 lcd.print("Press Key:");

 // this is how to register an event with task manager
 taskManager.registerEvent(new MyAnalogExceedsEvent(&analog, ANALOG_IN_PIN), true);

 taskManager.scheduleFixedRate(500, [] {
        // Modified from the original example to only print when there is a change.
        analog_value = analog.getCurrentValue(ANALOG_IN_PIN);
        if (analog_value != previous_analog) {
        Serial.print("Analog input value is ");
        Serial.print(analog.getCurrentValue(ANALOG_IN_PIN));
        Serial.print("/");
        Serial.print(analog.getMaximumRange(DIR_IN, ANALOG_IN_PIN));
        Serial.print(" - ");
        Serial.print(analog.getCurrentFloat(ANALOG_IN_PIN) * 100.0F);
        Serial.println('%');
        previous_analog = analog_value;
        }

   // This is at the end of setup()
  taskManager.scheduleFixedRate(250, checkA0);
    });

}

void loop() {
  // put your main code here, to run repeatedly:
  taskManager.runLoop();

}
