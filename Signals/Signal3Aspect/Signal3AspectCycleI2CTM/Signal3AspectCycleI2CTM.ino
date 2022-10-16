// Signal3AspectCycleI2CTM

// Adding task management to Signal3AspectCycleI2C

#include <TaskManagerIO.h>

// Converting example to use I2C and PCA9685

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "FastPWMLight.h"

enum { RED_on, YELLOW_on, GREEN_on } Led_State;


// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Example from Merg Journal June 2020 page49
// Sven Rosvall


/* This case ignores the  trackPin and cycles through the colours.
*/

int trackPin = 2;
// These are now on the pwm and in order top to bottom
int greenPin  = 1;
int redPin    = 3;
int yellowPin = 2;

void setup()
{
  Serial.begin(9600);
  Serial.println("Signal 3 Aspect using I2C and PCA9685 with Task Management");

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
  Led_State = GREEN_on;
  // This is at the end of setup()
  taskManager.scheduleFixedRate(5000,switch_LED);
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

void switch_LED()
{
  if (Led_State == GREEN_on) {
     pwmWrite(pwm, greenPin, LOW);
     pwmWrite(pwm, redPin, HIGH);
     Led_State = RED_on;
  } else if (Led_State == RED_on) {
     pwmWrite(pwm, redPin, LOW);
     pwmWrite(pwm, yellowPin, HIGH);
     Led_State = YELLOW_on;
  } else {
      pwmWrite(pwm, yellowPin, LOW);
      pwmWrite(pwm, greenPin, HIGH);
      Led_State = GREEN_on;
  }
}

void loop()
{
  taskManager.runLoop();

 // pwmWrite(pwm, greenPin, LOW);
 // pwmWrite(pwm, redPin, HIGH);

 // delay(5000);
 // pwmWrite(pwm, redPin, LOW);
 // pwmWrite(pwm, yellowPin, HIGH);

 // delay(5000);
 // pwmWrite(pwm, yellowPin, LOW);
 // pwmWrite(pwm, greenPin, HIGH);

 // delay(5000);
}
