// Signal3AspectSlowWith_IR_I2CTME2

// Adding IR detection to Signal3AspectSlowCycleI2CTME2

// Task management version of Signal3AspectSlowCycleI2C

// I am going to use ideas from the example tasksUsingExecutable to allow
// the up and down fades to be simultaneous.

// New version to move two LEDs together in one task.

// Comment on Signal3AspectSlowCycleI2CTME:
// This is NOT doing what I expected. It is better with longer main separation.
// I think I have to make both moves in one task, so I will make another version.

// I am going to use this as a testbed for code which will eventually be in SlowPCALight.
// I will keep that separate from the task management.

///////////////////////////////////////////////////////////////////////
// PSEUDOCODE
// SET Led_State = GREEN_on ,  BUTTON_off, IR_off
// IF button is pressed and BUTTON_off set BUTTON_on
// IF IR is detected set IR_on
// IF BUTTON_on OR IR_on THEN
//    set RED_on and if TASK_off THEN after 5 seconds set TASK_off
//    set TASK_on, BUTTON_off, IR_off
// FOR Led_State FADE OUT Led_State FADE_IN Next State SET Led_State = Next_State
///////////////////////////////////////////////////////////////////////

#include <IoAbstraction.h>
#include <TaskManagerIO.h>
#include <ExecWithParameter.h>

// The first thing to note is that the PWM range is to 4096 not 255

// Converting example to use I2C and PCA9685

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SlowPCALight.h>

enum { RED_on, YELLOW_on, GREEN_on } Led_State, Current_State, Next_State;

enum { BUTTON_off, BUTTON_on } Button_State;

enum { IR_off, IR_on } IR_State;

enum { TASK_off, TASK_on} Task_State;


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

IoAbstractionRef arduinoPins = ioUsingArduino();

// The IR signal is pulled high and goes low when there is a detection.
const int Signal_Pin = 5;
const int IR_Pin = 6;
const int LED_Pin = 7;

int ir_signal = 1;
int previous_signal = -1;

// Check the signal and act if it has changed.
void checkIR()
{
  ir_signal = ioDeviceDigitalReadS(arduinoPins, Signal_Pin);
  //Serial.println(ir_signal);
  if (ir_signal != previous_signal) {
    if (!ir_signal) IR_State = IR_on; else IR_State = IR_off;
    previous_signal = ir_signal;
    // Changes to save some code by passing the reverse value !ir_signal
    ioDeviceDigitalWrite(arduinoPins, LED_Pin, !ir_signal);
  }
}

SlowPCALight greenLight (pwm, greenPin);
SlowPCALight redLight   (pwm, redPin, true);
SlowPCALight yellowLight(pwm, yellowPin);

// Advance declaration needed here.
void pwmWrite(Adafruit_PWMServoDriver &pwm,uint8_t pwmnum,byte val);

// This class is given two pin numbers, the one to switch on and the one to switch off.
// An object is defined for each change and they don't get changed after that.
// The objects get called in sequence.
class SimultaneousSwitch : public Executable {
private:
   Adafruit_PWMServoDriver pwm;
   int pwmnum_up, pwmnum_down;

public:
 SimultaneousSwitch(Adafruit_PWMServoDriver &pwm1, int pin_up, int pin_down) : pwm(pwm1), 
    pwmnum_up(pin_up), pwmnum_down(pin_down)
 { }
 void exec() override {
  //if (up_or_down) {
    uint16_t ii;
    for (uint16_t i=2; i<255; i++) {
       ii = 256-i;
       pwmWrite(pwm,pwmnum_up,i);
       pwmWrite(pwm,pwmnum_down,ii);
       taskManager.yieldForMicros(10000);
    }
    // Make sure the off LED is really off.
    pwmWrite(pwm, pwmnum_down, LOW);    
  }

};

// Declaration of three objects
SimultaneousSwitch switchGreenRed(pwm,redPin,greenPin);
SimultaneousSwitch switchRedYellow(pwm,yellowPin,redPin);
SimultaneousSwitch switchYellowGreen(pwm,greenPin,yellowPin);
// Extra object to go from yellow to red
SimultaneousSwitch switchYellowRed(pwm,redPin,yellowPin);

void setup()
{
  Serial.begin(115200);
  Serial.println("Signal 3 Aspect Slow with button and IR using I2C and PCA9685 with Task Management");

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

  ioDevicePinMode(arduinoPins, Signal_Pin, INPUT_PULLUP);
  ioDevicePinMode(arduinoPins, IR_Pin, OUTPUT);
  ioDevicePinMode(arduinoPins, LED_Pin, OUTPUT);

  ioDeviceDigitalWrite(arduinoPins, IR_Pin, HIGH);
  ioDeviceDigitalWrite(arduinoPins, LED_Pin, LOW);

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
  Button_State = BUTTON_off;
  Task_State = TASK_off;
  IR_State = IR_off;
  // This is at the end of setup()
  taskManager.scheduleFixedRate(10000,switch_LED);
  taskManager.scheduleFixedRate(250, checkIR);
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
// Not used in this code - remains for comparison.
void pwmMove(Adafruit_PWMServoDriver &pwm,uint8_t pwmnum, bool up)
{
  if (up) {
       for (uint16_t i=2; i<255; i++) {
           pwmWrite(pwm,pwmnum,i);
           taskManager.yieldForMicros(20000);
           //delay(10);
       }    
  } else {
       for (uint16_t i=254; i>1; i--) {
           pwmWrite(pwm,pwmnum,i);
           taskManager.yieldForMicros(20000);
           //delay(10);
       }
       pwmWrite(pwm, pwmnum, LOW);    
  }
}

void switch_LED()
{
  if (Led_State == GREEN_on) {
     taskManager.execute(&switchGreenRed);
     Led_State = RED_on;
     Next_State = YELLOW_on;
  } else if (Led_State == RED_on && Task_State == TASK_off) {
     // Do not switch off the RED while Task_State is TASK_on.
     taskManager.execute(&switchRedYellow);
     Led_State = YELLOW_on;
     Next_State = GREEN_on;
  } else if (Task_State == TASK_off) {
     // Do not switch to GREEN while Task_State is TASK_on.
     taskManager.execute(&switchYellowGreen);
     Led_State = GREEN_on;
     Next_State = RED_on;
  }
}

void switch_LED_to_RED()
{
  if (Led_State == GREEN_on) {
     taskManager.execute(&switchGreenRed);
     Led_State = RED_on;
     Next_State = YELLOW_on;
  } else if (Led_State == YELLOW_on) {
     taskManager.execute(&switchYellowRed);
     Led_State = RED_on;
     Next_State = YELLOW_on;
  }
  if (Task_State == TASK_off) // Only run this if it is not running.
      taskManager.scheduleOnce(5000,task_off);
  Task_State = TASK_on;
  Button_State = BUTTON_off;
  // This is needed to clear the detection state which otherwise stays on.
  IR_State = IR_off;
}

void task_off()
{
     Task_State = TASK_off;  
}

void loop()
{
  taskManager.runLoop();

  // Avoid duplicate calls.
  if ( (digitalRead(trackPin) == LOW || IR_State == IR_on)  && Button_State == BUTTON_off)
 {
   Button_State = BUTTON_on;
   taskManager.execute(switch_LED_to_RED);
   if ( (digitalRead(trackPin) == LOW))  Serial.print("trackPin Low ");
   if ( IR_State == IR_on ) Serial.print("IR state ON ");
   Serial.println("Switching to RED");
 }
  
}
