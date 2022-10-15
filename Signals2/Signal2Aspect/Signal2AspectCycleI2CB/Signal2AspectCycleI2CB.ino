//////////////////////////////////////////////////////////////////////////////////////////
// Signal2AspectCycleI2CB
//////////////////////////////////////////////////////////////////////////////////////////
// This uses the SignalControl library
// Reduced to two aspect for now.
//
// John Fletcher 2021 john@bunbury28.plus.com
////////////////////////////////////////////////////////////////////////////////
// converted from
// Signal3AspectCycleI2C

// Converting example to use I2C and PCA9685

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// Change to use SignalControl library
#include "FastPWMLight.h"
#include "Signal2Aspect.h"
#include "DigitalInput.h"

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

void setup()
{
  Serial.begin(9600);
  Serial.println("Signal 2 Aspect using library, I2C and PCA9685 with simple cycle");

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

 // These are the existing pins on the Arduino.
 // pinMode(trackPin, INPUT_PULLUP);
  redLight1.update();
  yellowLight1.update();
  greenLight1.update();
}

// I need a routine to write to a pin via the PCA9685
// This first one is just ON/OFF.
void pwmWrite(Adafruit_PWMServoDriver &pwm,uint8_t pwmnum,byte val)
{
   if(val == HIGH) {
     pwm.setPWM(pwmnum,4096,0);  // pwm.setPin(pwmnum,0)
   } else {
     pwm.setPWM(pwmnum,0,4096);  // pwm.setPin(pwmnum,4096)
   }
}

void loop()
{
    signal1.update();
    delay(2000);
    fromEvent.set_state(!fromEvent.get());
}
