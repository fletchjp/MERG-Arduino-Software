// Signal3AspectSlowFadeCycleI2C

// Simple example fades each in and out in turn

// The first thing to note is that the PWM range is to 4096 not 255

// Converting example to use I2C and PCA9685

///////////////////////////////////////////////////////////////////////
// PSEUDOCODE
// SET Current_State = GREEN_on, Next_State = RED_on
// FOR Current_State SET Led_State = Next_State
///////////////////////////////////////////////////////////////////////


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "SlowPCALight.h"

enum { RED_on, YELLOW_on, GREEN_on } Led_State, Current_State, Next_State;

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
  Serial.begin(115200);
  Serial.println("Signal 3 Aspect Slow using I2C and PCA9685 with Task Management");

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

  // These are the existing pins on the Arduino.
  pinMode(trackPin, INPUT_PULLUP);
  //pinMode(greenPin, OUTPUT);
  //pinMode(redPin, OUTPUT);
  //pinMode(yellowPin, OUTPUT);
  pwmWrite(pwm, greenPin, LOW);
  pwmWrite(pwm, yellowPin, LOW);
  pwmWrite(pwm, redPin, LOW);
  Current_State = GREEN_on;
  Next_State = RED_on;
}

// I need a routine to write to a pin via the PCA9685
// This first one is just ON/OFF.
void pwmWrite(Adafruit_PWMServoDriver &pwm,uint8_t pwmnum,byte val)
{
   if(val == HIGH) {
     pwm.setPWM(pwmnum,4096,0);  // pwm.setPin(pwmnum,0)
   } else if(val == LOW) {
     pwm.setPWM(pwmnum,0,4096);  // pwm.setPin(pwmnum,4096)
   } else {
     // I need to map val from 0 to 255 to 0 to 4096
     pwm.setPWM(pwmnum,0,4096);  // pwm.setPin(pwmnum,4096)
   }
}

// New routine
/*
void pwmWrite(Adafruit_PWMServoDriver &pwm,uint8_t pwmnum,uint8_t intensity)
{
     pwm.setPWM(pwmnum,intensity,4096-intensity);
}
*/
// Instruction to move a light up or down
void pwmMove(Adafruit_PWMServoDriver &pwm,uint8_t pwmnum, bool up)
{


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
 /* This is not the way to do it.
 for (int intensity = 4096; intensity >= 0; intensity-=32)
 {
    pwm.setPWM(greenPin,intensity,4096-intensity);
    delay(100);
 }
 */
   for (uint8_t pwmnum = greenPin; pwmnum < redPin+1; pwmnum++) {
 //     for (uint16_t i=0; i<4096; i += 8) {
 //        pwm.setPWM(pwmnum, 0, (i + (4096/16)*pwmnum) % 4096 );
 //     }
       for (uint16_t i=0; i<2048; i += 8) {
           pwm.setPWM(pwmnum, 2048-i, 2048+i );
           delay(10);
       }
       delay(1000);
       for (uint16_t i=0; i<2048; i += 8) {
           pwm.setPWM(pwmnum, i, 4095-i );
           delay(10);
       }
       //pwmWrite(pwm, pwmnum, LOW);
   }
   delay(1000);


 
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
