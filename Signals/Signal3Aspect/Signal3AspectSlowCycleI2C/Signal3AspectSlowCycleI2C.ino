// Signal3AspectSlowCycleI2C

// The first thing to note is that the PWM range is to 4096 not 255

// Converting example to use I2C and PCA9685

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "SlowPCALight.h"

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

int trackPin = 2;
// These are now on the pwm and in order top to bottom
int greenPin  = 1;
int redPin    = 3;
int yellowPin = 2;

SlowPCALight greenLight (pwm, greenPin);
SlowPCALight redLight   (pwm, redPin, true);
SlowPCALight yellowLight(pwm, yellowPin);

void setup()
{
  Serial.begin(9600);
  Serial.println("Signal 3 Aspect Slow using I2C and PCA9685");

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

  // These are the existing pins on the Arduino.
  pinMode(trackPin, INPUT_PULLUP);
  //pinMode(greenPin, OUTPUT);
  //pinMode(redPin, OUTPUT);
  //pinMode(yellowPin, OUTPUT);
  pwmWrite(pwm, greenPin, HIGH);
  pwmWrite(pwm, yellowPin, LOW);
  pwmWrite(pwm, redPin, LOW);
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
///  delay(5000);

 //pwmWrite(pwm, greenPin, LOW);
 //pwmWrite(pwm, redPin, HIGH);
 //   redLight.set(false);
 //   greenLight.set(true);
 //   yellowLight.set(false);
 //  greenLight.update();
 //  redLight.update();
  // yellowLight.update();
 for (int intensity = 4096; intensity >= 0; intensity-=32)
 {
    pwm.setPWM(greenPin,intensity,4096-intensity);
    delay(100);
 }
/*
  delay(5000);
//  pwmWrite(pwm, redPin, LOW);
//  pwmWrite(pwm, yellowPin, HIGH);
    yellowLight.set(true);     
    greenLight.set(false);
    redLight.set(false);
    greenLight.update();
    redLight.update();
    yellowLight.update();
 
  delay(5000);
//  pwmWrite(pwm, yellowPin, LOW);
//  pwmWrite(pwm, greenPin, HIGH);
    greenLight.set(true);
    redLight.set(false);
    yellowLight.set(false);
    greenLight.update();
    redLight.update();
    yellowLight.update();

  delay(5000);
*/
}
