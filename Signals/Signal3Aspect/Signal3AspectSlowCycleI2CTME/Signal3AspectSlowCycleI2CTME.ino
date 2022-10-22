// Signal3AspectSlowCycleI2CTME

// Task management version of Signal3AspectSlowCycleI2C

// I am going to use ideas from the example tasksUsingExecutable to allow
// the up and down fades to be simultaneous.

// This is NOT doing what I expected.

// I am going to use this as a testbed for code which will eventually be in SlowPCALight.
// I will keep that separate from the task management.

#include <TaskManagerIO.h>
#include <ExecWithParameter.h>

// The first thing to note is that the PWM range is to 4096 not 255

// Converting example to use I2C and PCA9685

///////////////////////////////////////////////////////////////////////
// PSEUDOCODE
// SET Led_State = GREEN_on
// FOR Led_State FADE OUT Led_State FADE_IN Next State SET Led_State = Next_State
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

// Advance declaration needed here.
void pwmWrite(Adafruit_PWMServoDriver &pwm,uint8_t pwmnum,byte val);

class SimultaneousSwitch : public Executable {
private:
   Adafruit_PWMServoDriver pwm;
   int pwmnum;
   bool up_or_down; 
public:
 SimultaneousSwitch(Adafruit_PWMServoDriver &pwm1, int pin) : pwm(pwm1), pwmnum(pin) 
 {
   up_or_down = true; 
 }
 void set_up_or_down(bool choice) {
   up_or_down = choice ;
 }
 void exec() override {
  if (up_or_down) {
       for (uint16_t i=2; i<255; i++) {
           pwmWrite(pwm,pwmnum,i);
           taskManager.yieldForMicros(10000);
       }    
  } else {
       for (uint16_t i=254; i>1; i--) {
           pwmWrite(pwm,pwmnum,i);
           taskManager.yieldForMicros(10000);
       }
       pwmWrite(pwm, pwmnum, LOW);    
  }
   
 }
};

SimultaneousSwitch switchGreen(pwm,greenPin);
SimultaneousSwitch switchYellow(pwm,yellowPin);
SimultaneousSwitch switchRed(pwm,redPin);

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
  pwmWrite(pwm, greenPin, HIGH);
  pwmWrite(pwm, yellowPin, LOW);
  pwmWrite(pwm, redPin, LOW);
  Led_State = GREEN_on;
  Next_State = RED_on;
  // This is at the end of setup()
  taskManager.scheduleFixedRate(5000,switch_LED);
}

// I need a routine to write to a pin via the PCA9685
// This first one is just ON/OFF.
void pwmWrite(Adafruit_PWMServoDriver &pwm,uint8_t pwmnum,byte val)
{
   int i = 0;
   if(val == HIGH) {
     pwm.setPWM(pwmnum,4096,0);  // pwm.setPin(pwmnum,0)
   } else if(val == LOW) {
     pwm.setPWM(pwmnum,0,4096);  // pwm.setPin(pwmnum,4096)
   } else {
     // I need to map val from 0 to 255 to 0 to 4096
     i = map(val, 0, 255, 0, 2047);
     pwm.setPWM(pwmnum,2048-i,2048+i);
   }
}


// Instruction to move a light up or down
void pwmMove(Adafruit_PWMServoDriver &pwm,uint8_t pwmnum, bool up)
{
  if (up) {
       for (uint16_t i=2; i<255; i++) {
           pwmWrite(pwm,pwmnum,i);
           taskManager.yieldForMicros(10000);
           //delay(10);
       }    
  } else {
       for (uint16_t i=254; i>1; i--) {
           pwmWrite(pwm,pwmnum,i);
           taskManager.yieldForMicros(10000);
           //delay(10);
       }
       pwmWrite(pwm, pwmnum, LOW);    
  }
}

void switch_LED()
{
  if (Led_State == GREEN_on) {
     switchGreen.set_up_or_down(false);
     taskManager.execute(&switchGreen);
     //pwmMove(pwm, greenPin, false);
     switchRed.set_up_or_down(true);
     taskManager.execute(&switchRed);
     //pwmMove(pwm, redPin, true);
     Led_State = RED_on;
     Next_State = YELLOW_on;
  } else if (Led_State == RED_on /*&& Task_State == TASK_off */) {
     // Do not switch off the RED while Task_State is TASK_on.
     switchRed.set_up_or_down(false);
     taskManager.execute(&switchRed);
     //pwmMove(pwm, redPin, false);
     switchYellow.set_up_or_down(true);
     taskManager.execute(&switchYellow);
     //pwmMove(pwm, yellowPin, true);
     Led_State = YELLOW_on;
     Next_State = GREEN_on;
  } else /*if (Task_State == TASK_off) */ {
     // Do not switch to GREEN while Task_State is TASK_on.
      switchYellow.set_up_or_down(false);
      taskManager.execute(&switchYellow);
    // pwmMove(pwm, yellowPin, false);
      switchGreen.set_up_or_down(true);
      taskManager.execute(&switchGreen);
      //pwmMove(pwm, greenPin, true);
      Led_State = GREEN_on;
      Next_State = RED_on;
  }
}

void loop()
{
  taskManager.runLoop();
}
