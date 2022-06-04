/// @file ControllerReaderTimeout.ino
/// Extending to use timeouts.
///
/// @brief Wire Controller Reader
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

#define WIRE_HAS_TIMEOUT

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(3000 /* us */, true /* reset_on_timeout */);
#endif
  Serial.begin(115200);  // start serial for output
  Serial.println("Controller Reader");
}

void loop() {
  #if defined(WIRE_HAS_TIMEOUT)
  Wire.clearWireTimeoutFlag();
  #endif
  uint8_t what_results = Wire.requestFrom(8, 6);    // request 6 bytes from peripheral device #8
  Serial.print(what_results);
  Serial.println(" bytes available");
  if (what_results == 0) {
#if defined(WIRE_HAS_TIMEOUT)
    if (Wire.getWireTimeoutFlag())
        Serial.println("It was a timeout");
#endif
  } else {
    while (Wire.available()) { // peripheral may send less than requested
      char c = Wire.read(); // receive a byte as character
      Serial.print(c);         // print the character
    }
    Serial.println(" ");
  }
  delay(500);
}
