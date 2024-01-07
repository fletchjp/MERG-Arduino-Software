/// @file TestWhichBoard.ino
/// @brief Test which board the code is running on.
///
/// There are two libraries in the Arduino libraries which will help with doing this.
///
/// Board_Identity and MyMacros 
///
/// The second one extends the first one and will be used here.
///
/// To avoid the library overhead, the code can be inspected to find the macros needed.

#define BOARD_INDENTITY_WARNING
#include <MyMacros.h>

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

void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);
  //Serial << F("enter a number");
}
