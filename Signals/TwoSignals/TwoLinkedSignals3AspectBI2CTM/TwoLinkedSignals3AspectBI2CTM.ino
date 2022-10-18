// TwoLinkedSignals3AspectBI2CTM

// Task management added to TwoLinkedSignals3AspectBI2C

// This version uses the modified SignalControl library and a PCA9685 

// This will involve considerable rethinking as this already has elements of a state mechanism
// in the Signals library, in particular in Signal3AspectState.h which was an addition to the library.

// I suspect I will need to bring the state mechanism into the main code as in other examples.

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Signal3AspectState.h"
#include "FastPWMLight.h"
#include "DistanceTimer.h"
#include "BlockDistanceInput.h"
//#include "SlowLight.h"// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// These correspond with the 2 buttons
DistanceTimer distanceTimer1(2);
DistanceTimer distanceTimer2(8);

// 3,5,6,9,10,11 can be used for PWM output.
// These are now on the pwm and in order top to bottom except missing 0
int greenPin1 = 1;
int redPin1 = 3;
int yellowPin1 = 2;

int greenPin2 =4;
int redPin2 = 6;
int yellowPin2 =5;

FastPWMLight greenLight1(pwm,greenPin1,true);
FastPWMLight yellowLight1(pwm,yellowPin1,true);
FastPWMLight redLight1(pwm,redPin1,false );

Signal3AspectState signal1(distanceTimer1, greenLight1, redLight1, yellowLight1,0);
unsigned int signal1state;

FastPWMLight greenLight2(pwm,greenPin2,true);
FastPWMLight yellowLight2(pwm,yellowPin2,false);
FastPWMLight redLight2(pwm,redPin2,false );

Signal3AspectState signal2(distanceTimer2, greenLight2, redLight2, yellowLight2,0);
unsigned int signal2state;

void setup() 
{
    Serial.begin(115200);
    Serial.println("Two Linked 3 Aspect signals starting");
}

void loop() 
{
  distanceTimer1.update();
  distanceTimer2.update();
  signal1.update();
  signal1state = signal1.get_state();
  signal2.update_state(signal1state+1);
  signal2state = signal2.get_state();
}
