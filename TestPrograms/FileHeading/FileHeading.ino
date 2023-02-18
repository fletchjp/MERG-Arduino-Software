/// @file TestWarnings.ino
/// @brief Test code warnings.
///
/// There are two problems.
///
/// One is that the Arduino IDE normal setting turns off all the warnings.
///
/// The other is that the Arduino IDE uses -fpermissive which means that some errors become warnings.
/// 
/// As warnings are turned off there are then no warnings of some errors.

// 3rd party libraries
#include <Streaming.h>

void setup() {
  // put your setup code here, to run once:
  while(!Serial);
  Serial.begin (115200);
  Serial << endl << endl << F("> ** TestWarnings ** ") << __FILE__ << endl;
}

void loop()
{
  
}
