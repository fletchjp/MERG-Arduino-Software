// SlowPCALight.h 
// This is being developed from SlowLight.h
// The difference is that this file uses a PCA9685 for PWM.
// The functionality is unchanged.
// John Fletcher (M6777) john@bunbury28.plus.co.
#include "Light.h"
#include <Adafruit_PWMServoDriver.h>

const long defaultTransitionInterval = 500;

class SlowPCALight : public Light
{
  Adafruit_PWMServoDriver &pwm;
  int lightPin;
  bool lightOn;
  long transitionInterval;
  unsigned long timer;

public:
// SlowPCALight(Adafruit_PWMServoDriver &pwm,int lightPin, bool lightOn = false)
//      : pwm(pwm)
// I don't like calling one constructor from another.
// I think that argument reordering is needed so that defaults can be used.
//    : SlowLight(lightPin, defaultTransitionInterval, lightOn)
//  {
//  }

  SlowPCALight(Adafruit_PWMServoDriver &pwm,int lightPin, bool lightOn = false, 
               long transitionInterval = defaultTransitionInterval)
    : pwm(pwm), lightPin(lightPin)
    , lightOn(lightOn)
    , transitionInterval(transitionInterval)
    , timer(-transitionInterval)
  {
    //pinMode(lightPin, OUTPUT);
  }

  void set(bool lightOn)
  {
    if (this->lightOn == lightOn)
    {
      // No change
      return;
    }

    this->lightOn = lightOn;

    unsigned long now = millis();
    if (now > timer + transitionInterval)
    {
      // Previous setting was fully achieved. Start timer from now.
      this->timer = now;
    }
    else
    {
      // Previous setting was not fully achieved. Fix timer to make transition
      // smooth.
      this->timer = now + (now - timer) - transitionInterval;
    }
  }

  void update()
  {
	// This has to be rather different for the PCA9685 where PWM is different.
	// This is no good at all at the moment  
    unsigned long now = millis();
    int intensity;
    if (now > timer + transitionInterval)
    {
      intensity = lightOn ? 4096 : 0;
    }
    else if (lightOn)
    {
      intensity = map(now - timer, 0, transitionInterval, 0, 4096);
    }
    else
    {
      intensity = map(now - timer, 0, transitionInterval, 4096, 0);
    }
    //analogWrite(lightPin, intensity);
	//pwm.setPWM(lightPin,intensity,4096-intensity);
  }
};
