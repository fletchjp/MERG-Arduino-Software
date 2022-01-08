/// @file TestModern.ino
/// @brief Test ideas from Hackaday.
///
/// https://hackaday.com/2017/05/05/using-modern-c-techniques-with-arduino/

// 3rd party libraries
#include <Streaming.h>

void setup() {
  // put your setup code here, to run once:
  while(!Serial);
  Serial.begin (115200);
  Serial << endl << endl << F("> ** TestModern ** ") << __FILE__ << endl;
}

void loop() {
  // put your main code here, to run repeatedly:

}
