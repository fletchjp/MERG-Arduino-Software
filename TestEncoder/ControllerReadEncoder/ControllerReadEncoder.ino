/// @file ControllerReadEncoder.ino
/// @brief Wire Controller Reading from Encoder
/// by Nicholas Zambetti <http://www.zambetti.com>
///
/// Demonstrates use of the Wire library
/// Reads data from an I2C/TWI peripheral device
/// Refer to the "Wire Peripheral Sender" example for use with this
///
/// Created 29 March 2006
///
/// This example code is in the public domain.


#include <Wire.h>

int previous = 0;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  Serial.println("Ready to read from encoder");
}

void loop() {
  Wire.requestFrom(8, 1);    // request 6 bytes from peripheral device #8

  while (Wire.available()) { // peripheral may send less than requested
    int c = Wire.read();     // receive an int
    if ( c != previous) {
      Serial.print(c);         // print the value
      Serial.println(" ");
      previous = c;
    }
  }
  delay(500);
}
