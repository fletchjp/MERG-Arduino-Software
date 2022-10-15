//////////////////////////////////////////////////////////////////////////////////////////
// Signal2Aspect_IR_I2C_Cb3
// using a class for the IR pin detection adapted from 
// Signal2Aspect_IR_I2C_Callback
//////////////////////////////////////////////////////////////////////////////////////////
// This uses the SignalControl library
// Reduced to two aspect for now.
// Change to use IR detector to switch the lights.
// Using IOAbstraction and TaskManagerIO Libraries
//
// John Fletcher 2021 john@bunbury28.plus.com
////////////////////////////////////////////////////////////////////////////////
// converted from
// Signal3AspectCycleI2C

#include <IoAbstraction.h>
#include <TaskManagerIO.h>

IoAbstractionRef arduinoPins = ioUsingArduino();

// Converting example to use I2C and PCA9685

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// Change to use SignalControl library
#include "FastPWMLight.h"
#include "Signal2Aspect.h"
#include "DigitalInput.h"

// The IR signal is pulled high and goes low when there is a detection.
const int Signal_Pin = 5;
const int IR_Pin = 6;
const int LED_Pin = 7;

int ir_signal = 0;
int previous_signal = -1;

// Copied from CANASIGNAL and changed to DistanceInput
class BoolInput : public DigitalInput
{
   bool _state;

public:
   BoolInput() : _state(false) {}
   BoolInput(bool state) : _state(state) {}
 
   void set_state(bool state) { _state = state; }
   
   virtual bool get() const
   {
     return _state;
   }
 
};

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Example from Merg Journal June 2020 page49
// Sven Rosvall

// Arduino Nano as in the article.

/* This case ignores the  trackPin and cycles through the colours.
*/

//int trackPin = 2;
// These are now on the pwm and in order top to bottom except missing 0
const uint8_t greenPin1  = 1;
const uint8_t redPin1    = 3;
const uint8_t yellowPin1 = 2;

// Starting values set here are not applied until the setup() action.
FastPWMLight greenLight1(pwm,greenPin1,true);
FastPWMLight yellowLight1(pwm,yellowPin1,false);
FastPWMLight redLight1(pwm,redPin1,false );

BoolInput fromEvent;
// Yellow light is ignored.
Signal2Aspect signal1(fromEvent,greenLight1, redLight1);

// Check the signal and act if it has changed. (not used in this version)
void checkIRpin()
{
  ir_signal = ioDeviceDigitalReadS(arduinoPins, Signal_Pin);
  Serial.println(ir_signal);
  if (ir_signal != previous_signal) {
    previous_signal = ir_signal;
    // Change the state of the signal event.
    fromEvent.set_state(!fromEvent.get());
  }
}

class CheckIRpin : public Executable {
   private:
     int sig_pin;
     //int led_pin;
     // These are now local to each instance.
     int ir_signal;
     int previous_signal;
   public:
     CheckIRpin (int s) : sig_pin(s) { previous_signal = -1;}
     void exec() override {
        ir_signal = ioDeviceDigitalReadS(arduinoPins, sig_pin);
        Serial.println(ir_signal);
        if (ir_signal != previous_signal) {
          previous_signal = ir_signal;
    // Change the state of the signal event.
          fromEvent.set_state(!fromEvent.get());
        }
     }
};

CheckIRpin checkPin(Signal_Pin);

void setup()
{
  Serial.begin(9600);
  Serial.println("Signal 2 Aspect using library, I2C and PCA9685 with IR detector");

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

 // These are the existing pins on the Arduino.
 // pinMode(trackPin, INPUT_PULLUP);
  redLight1.update();
  yellowLight1.update();
  greenLight1.update();

  ioDevicePinMode(arduinoPins, Signal_Pin, INPUT_PULLUP);
  ioDevicePinMode(arduinoPins, IR_Pin, OUTPUT);

  ioDeviceDigitalWrite(arduinoPins, IR_Pin, HIGH);

  //taskManager.scheduleFixedRate(250, checkIR);
  // Class instance method - this could be used to check different sets of pins.
  // CheckIRpin checkPin(Signal_Pin); moved global to be sure it goes on existing.
  taskManager.scheduleFixedRate(250, &checkPin);

}

void loop()
{
      taskManager.runLoop();
      signal1.update();
}
