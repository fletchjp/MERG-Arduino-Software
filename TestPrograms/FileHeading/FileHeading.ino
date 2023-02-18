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


//#define VERSION 1.0
const byte VER_MAJ  = 1;
const byte VER_MIN  = 0;
const byte VER_DETAIL = 0;

void heading()
{
  Serial << endl << endl << __FILE__ << endl;
  Serial << F("Ver: ") << VER_MAJ << F(".") << VER_MIN << F(".") << VER_DETAIL;
  Serial << F(" compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(" using compiler ") << __cplusplus << endl;
 
}
void setup() {
  // put your setup code here, to run once:
  while(!Serial);
  Serial.begin (115200);
  heading();
}

void loop()
{
  
}
