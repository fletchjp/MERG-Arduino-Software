// Signal3AspectWithButtonI2C

// Example from Merg Journal June 2020 page49
// Sven Rosvall

// Arduino Nano as in the article.

// Converting example to use I2C and PCA9685

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "FastPWMLight.h"


/* This case  uses the  trackPin and cycles through the colours.
    when a train is detected.
*/

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int trackPin = 2;
// These are now on the pwm and in order top to bottom
int greenPin  = 1;
int redPin    = 3;
int yellowPin = 2;

void setup()
{
  Serial.begin(115200);
  Serial.println("Signal 3 Aspect with button using I2C and PCA9685");
  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

  pinMode(trackPin, INPUT_PULLUP);
  digitalWrite(greenPin, HIGH);

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

  while (digitalRead(trackPin) == HIGH )
 {
 }

  pwmWrite(pwm, greenPin, LOW);
  pwmWrite(pwm, redPin, HIGH);

  delay(5000);
  pwmWrite(pwm, redPin, LOW);
  pwmWrite(pwm, yellowPin, HIGH);

  delay(5000);
  pwmWrite(pwm, yellowPin, LOW);
  pwmWrite(pwm, greenPin, HIGH);

  //delay(5000);
}
