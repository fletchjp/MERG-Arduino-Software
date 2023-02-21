/// @file FileHeading.ino
/// @brief File heading code to put in other codes.
///

// 3rd party libraries
#include <Streaming.h>

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
