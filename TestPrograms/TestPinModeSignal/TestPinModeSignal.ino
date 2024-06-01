/// @file TestPinModeSignal.ino
/// @brief Test operation of pinMode.
///
/// It has been reported that pinMode should not be used in a constructor called outside setup().
///
/// This code is to test this using some code from the Signal Control library.

// 3rd party libraries
#include <Streaming.h>

// SignalLibrary code.
#include "PinInput.h"

PinInput button(2);

const byte VER_MAJ  = 1;
const byte VER_MIN  = 0;
const byte VER_DETAIL = 0;

void heading()
{
 Serial << endl << endl << __FILE__ << endl;
  Serial << F("Ver: ") << VER_MAJ << F(".") << VER_MIN << F(".") << VER_DETAIL;
  Serial << F(" compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(" using compiler ") << __cplusplus << endl;
}

//
//    FILE: getPinMode.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.00
// PURPOSE: getPinMode
//    DATE: 2014-05-11
//     URL:
//
// Released to the public domain
//

int getPinMode(uint8_t pin)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *reg, *out;

  if (port == NOT_A_PIN) return -1;

  // JWS: can I let the optimizer do this?
  reg = portModeRegister(port);
  out = portOutputRegister(port);

  if ((~*reg & bit) == bit) // INPUT OR PULLUP
  {
    if ((~*out & bit)  == bit) return INPUT;
    else return INPUT_PULLUP;
  }
  return OUTPUT;
}

// Tests from the code.
void setup() 
{
  while(!Serial);
  Serial.begin (115200);
  heading();

  Serial.println("Start ");

  Serial.println("INPUT");
  pinMode(13, INPUT);
  int a = getPinMode(13);
  Serial.println(INPUT, DEC);
  Serial.println(a, DEC);

  Serial.println("INPUT_PULLUP");
  pinMode(13, INPUT_PULLUP);
  int b = getPinMode(13);
  Serial.println(INPUT_PULLUP, DEC);
  Serial.println(b, DEC);

  Serial.println("OUTPUT");
  pinMode(13, OUTPUT);
  int c = getPinMode(13);
  Serial.println(OUTPUT, DEC);
  Serial.println(c, DEC);


  Serial.println("PinInput(2) outside setup()");
  int d = getPinMode(2);
  Serial.println(d, DEC);

  PinInput button3(3);

  Serial.println("PinInput(3) inside setup()");
  int e = getPinMode(3);
  Serial.println(e, DEC);

}


void loop() {


}
