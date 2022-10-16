// Signal3AspectWithButtonI2CTM

// Example from Merg Journal June 2020 page49
// Sven Rosvall

// Adding task management to Signal3AspectWithButtonI2C
//////////////////////////////////////////////////////////////////////////////////// 
// PSEUDOCODE
// Set GREEN_on
// 
///////////////////////////////////////////////////////////////////////////////////


#include <TaskManagerIO.h>

// Converting example to use I2C and PCA9685

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "FastPWMLight.h"

enum { RED_on, YELLOW_on, GREEN_on } Led_State;

enum { BUTTON_off, BUTTON_on } Button_State;

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
  Serial.begin(9600);
  Serial.println("Signal 3 Aspect with button using I2C and PCA9685");
  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

  pinMode(trackPin, INPUT_PULLUP);
  digitalWrite(greenPin, HIGH);

  pwmWrite(pwm, greenPin, HIGH);
  pwmWrite(pwm, yellowPin, LOW);
  pwmWrite(pwm, redPin, LOW);
  Led_State = GREEN_on;
  Button_State = BUTTON_off;
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

void switch_LED_to_RED()
{
     pwmWrite(pwm, yellowPin, LOW);
     pwmWrite(pwm, greenPin, LOW);
     pwmWrite(pwm, redPin, HIGH);
     Led_State = RED_on;
     taskManager.scheduleOnce(5000,button_off);
}

void button_off()
{
     Button_State = BUTTON_off;  
}

void switch_LED()
{
  if (Led_State == GREEN_on) {
     pwmWrite(pwm, greenPin, LOW);
     pwmWrite(pwm, redPin, HIGH);
     Led_State = RED_on;
  } else if (Led_State == RED_on && Button_State == BUTTON_off) {
     // Do not switch off the RED while Button_State is BUTTON_on.
     pwmWrite(pwm, redPin, LOW);
     pwmWrite(pwm, yellowPin, HIGH);
     Led_State = YELLOW_on;
  } else if (Button_State == BUTTON_off) {
     // Do not switch to GREEN while Button_State is BUTTON_on.
      pwmWrite(pwm, yellowPin, LOW);
      pwmWrite(pwm, greenPin, HIGH);
      Led_State = GREEN_on;
  }
}

void loop()
{

  taskManager.runLoop();

  if (digitalRead(trackPin) == LOW )
 {
   Button_State = BUTTON_on;
   taskManager.execute(switch_LED_to_RED);
 }

/*
  pwmWrite(pwm, greenPin, LOW);
  pwmWrite(pwm, redPin, HIGH);

  delay(5000);
  pwmWrite(pwm, redPin, LOW);
  pwmWrite(pwm, yellowPin, HIGH);

  delay(5000);
  pwmWrite(pwm, yellowPin, LOW);
  pwmWrite(pwm, greenPin, HIGH);
*/
  //delay(5000);
}
