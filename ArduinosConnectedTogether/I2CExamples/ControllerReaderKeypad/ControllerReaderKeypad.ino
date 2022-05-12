/// @file ControllerReaderKeypad.ino
/// @brief Wire Controller Reader to get keypad data
/// adapted from Nicholas Zambetti <http://www.zambetti.com>
///
/// Demonstrates use of the Wire library
/// Reads data from an I2C/TWI peripheral device
/// Refer to the "Wire Peripheral Sender" example for use with this
///
/// Created 29 March 2006
///
/// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("ControllerReaderKeypad running");
  Wire.begin();        // join i2c bus (address optional for master)
}

char old_c = 0;

void loop() {
  Wire.requestFrom(8, 1);    // request 1 bytes from peripheral device #8

  while (Wire.available()) { // peripheral may send less than requested
    char c = Wire.read(); // receive a byte as character
    if (c != old_c) {
       old_c = c;
       Serial.println(c);         // print the character
    }
  }

  delay(500);
}
