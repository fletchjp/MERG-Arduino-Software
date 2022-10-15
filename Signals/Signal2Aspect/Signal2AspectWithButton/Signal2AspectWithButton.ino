// Signal2AspectWithButton

// Changed pins for CBUS

#include "Signal2Aspect.h"
#include "PinInput.h"
#include "FastLight.h"

PinInput button(2);

FastLight greenLight(3,true);
FastLight redLight(5,false );
Signal2Aspect signal(button, greenLight, redLight);

void setup()
{
    Serial.begin(9600);
    Serial.println("Signal 2 Aspect starting");
}

void loop()
{
  signal.update();
}