/// @file PJRCBasic.ino
/// @brief Test for encoder based on PJRC encoder library
///
/// https://www.pjrc.com/teensy/td_libs_Encoder.html
///
/// This is configured as an Arduino library. However, there is another one of the same name in the system.
///
/// This example is now working - encoder only, no button.

/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include "Encoder.h"

const int PinCLK=2;   // Generating interrupts using CLK signal
const int PinDT=3;    // Reading DT signal

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(PinCLK, PinDT);
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(115200);
  Serial.println("PJRC Basic Encoder Test:");
}

long oldPosition  = -999;

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}
