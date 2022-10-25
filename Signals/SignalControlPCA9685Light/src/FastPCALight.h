// FastPCALight.h 
// I am changing the name from FastPWMLight.harder
// The old name was misleading as FastLight.h uses PWM.
// The difference is that this file uses a PCA9685 for PWM.
// This file will be used instead of the old one as I develop the examples.
// The functionality is unchanged.
// John Fletcher (M6777) john@bunbury28.plus.co.

#include "Light.h"
#include <Adafruit_PWMServoDriver.h>

class FastPCALight : public Light
{
  Adafruit_PWMServoDriver &pwm;
  int lightPin;
  bool lightOn;

public:
  FastPCALight(Adafruit_PWMServoDriver &pwm,int lightPin, bool lightOn)
    : pwm(pwm)
    , lightPin(lightPin)
    , lightOn(lightOn)
  {
    update();
    //pinMode(lightPin, OUTPUT);
  }

  void set(bool lightOn)
  {
    this->lightOn = lightOn;
  }

  void update()
  {
   if(lightOn == HIGH) {
     pwm.setPWM(lightPin,4096,0);  // pwm.setPin(pwmnum,0)
   } else {
     pwm.setPWM(lightPin,0,4096);  // pwm.setPin(pwmnum,4096)
   }
  }
};
