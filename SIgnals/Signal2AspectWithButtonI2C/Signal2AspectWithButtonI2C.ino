// Signal2AspectWithButtonI2C

// Now to make a version to use I2C and the PCA9685

// Changed pins for CBUS
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "FastPWMLight.h"
#include "Signal2Aspect.h"
#include "PinInput.h"
//#include "FastLight.h"

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

PinInput button(10);
// These are now on the pwm and in order top to bottom
int greenPin  = 1;
int redPin    = 3;

FastPWMLight greenLight(pwm,greenPin,true);
FastPWMLight redLight(pwm,redPin,false );
Signal2Aspect signal(button, greenLight, redLight);

void setup()
{
    Serial.begin(9600);
    Serial.println("Signal 2 Aspect starting");
}

void loop()
{
  signal.update();
}
