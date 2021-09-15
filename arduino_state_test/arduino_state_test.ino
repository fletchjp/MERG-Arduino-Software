
#include <StateMachine.h>


const int LED = 13;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial) { }
  pinMode(LED,OUTPUT);
  Serial.println("Arduino state blink");

}

void loop() {
  // put your main code here, to run repeatedly:

}
