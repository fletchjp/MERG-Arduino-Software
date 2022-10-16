// Example from Merg Journal June 2020 page 49
// Expanded to two linked signals
// Sven Rosvall

// Arduino Nano as in the article.
// Now modified to run with a PCA9685

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "FastPWMLight.h"

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

/* This case  uses the  trackPin and cycles through the colours.
    when a train is detected.
*/

/* The same pin allocation ias in TwoIndependentSignal3Aspect example. */
int trackPin = 2;
// These are now on the pwm and in order top to bottom except missing 0
int greenPin1 = 1;
int redPin1 = 3;
int yellowPin1 = 2;

int greenPin2 =4;
int redPin2 = 6;
int yellowPin2 =5;

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

void setup()
{
  Serial.begin(9600);
  Serial.println("Two Linked Signals 3 Aspect using I2C and PCA9685");

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

  pinMode(trackPin, INPUT_PULLUP);
  // Initial settings
  /* Start with the line clear */
  pwmWrite(pwm, greenPin1, HIGH);
  pwmWrite(pwm, redPin1, LOW);
  pwmWrite(pwm, yellowPin1, LOW);
  pwmWrite(pwm, greenPin2, HIGH);
  pwmWrite(pwm, redPin2, LOW);
  pwmWrite(pwm, yellowPin2, LOW);
}

void loop()
{

  while (digitalRead(trackPin) == HIGH )
 {
 }
 /* Train passes the first signal */
  pwmWrite(pwm, greenPin2, LOW);
  pwmWrite(pwm, redPin2, HIGH);
 
/* Train passes the secondsignal */
  delay(5000);
  pwmWrite(pwm, greenPin1, LOW);
  pwmWrite(pwm, redPin1, HIGH);
  pwmWrite(pwm, redPin2, LOW);
  pwmWrite(pwm, yellowPin2, HIGH);

  delay(5000);
  pwmWrite(pwm, redPin1, LOW);
  pwmWrite(pwm, yellowPin1, HIGH);
  pwmWrite(pwm, yellowPin2, LOW);
  pwmWrite(pwm, greenPin2, HIGH);

  delay(5000);
  pwmWrite(pwm, yellowPin1, LOW);
  pwmWrite(pwm, greenPin1, HIGH);

  //delay(5000);
}
