#include "Signal3AspectState.h"
#include "DistanceTimer.h"
#include "BlockDistanceInput.h"
#include "SlowLight.h"

DistanceTimer distanceTimer1(2);
DistanceTimer distanceTimer2(8);

// 3,5,6,9,10,11 can be used for PWM output.
SlowLight greenLight1(3);
SlowLight redLight1(5, true);
SlowLight yellowLight1(6);
Signal3AspectState signal1(distanceTimer1, greenLight1, redLight1, yellowLight1,0);
unsigned int signal1state;

SlowLight greenLight2(9);
SlowLight redLight2(10, true);
SlowLight yellowLight2(11);
Signal3AspectState signal2(distanceTimer2, greenLight2, redLight2, yellowLight2,0);
unsigned int signal2state;

void setup() 
{
    Serial.begin(9600);
    Serial.println("Two Linked 3 Aspect signals starting");
}

void loop() 
{
  distanceTimer1.update();
  distanceTimer2.update();
  signal1.update();
  signal1state = signal1.get_state();
  signal2.update_state(signal1state+1);
}
