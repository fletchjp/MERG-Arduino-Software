/// @file ControllerTwoWayTimeout.ino

/// This is code from here:
///https://www.arduino.cc/reference/en/language/functions/communication/wire/setwiretimeout/

#include <Wire.h>

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  #if defined(WIRE_HAS_TIMEOUT)
    Wire.setWireTimeout(3000 /* us */, true /* reset_on_timeout */);
  #endif
}

byte x = 0;

void loop() {
  /* First, send a command to the other device */
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(123);           // send command
  byte error = Wire.endTransmission(); // run transaction
  if (error) {
    Serial.println("Error occured when writing");
    if (error == 5)
      Serial.println("It was a timeout");
  }

  delay(100);

  /* Then, read the result */
  #if defined(WIRE_HAS_TIMEOUT)
  Wire.clearWireTimeoutFlag();
  #endif
  byte len = Wire.requestFrom(8, 1); // request 1 byte from device #8
  if (len == 0) {
    Serial.println("Error occured when reading");
    #if defined(WIRE_HAS_TIMEOUT)
    if (Wire.getWireTimeoutFlag())
      Serial.println("It was a timeout");
    #endif
  }

  delay(100);
}
