/// @file ControllerTwoWayTimeout.ino

/// This is code from here:
///https://www.arduino.cc/reference/en/language/functions/communication/wire/setwiretimeout/

#include <Wire.h>

// I think this should be defined by the library.
// I get no errors when I define it myself.
// Looking at the code of the library it does implement the timeout members.
// It does not however define this macro.
// Timeouts were implemented for AVR in 2020.
#define WIRE_HAS_TIMEOUT

#define NODE_ADDRESS   8 // The I2C address of the peripheral node

void setup() {
  Serial.begin(115200);           // start serial for output
  Serial.println("Controller Two Way");
#ifdef  WIRE_HAS_TIMEOUT
  Serial.println("with Timeout active");
#endif
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
  byte len = Wire.requestFrom(NODE_ADDRESS, 1); // request 1 byte from device #8
  if (len == 0) {
    Serial.println("Error occured when reading");
    #if defined(WIRE_HAS_TIMEOUT)
    if (Wire.getWireTimeoutFlag())
      Serial.println("It was a timeout");
    #endif
  } else {
    Serial.println(len);
  }

  delay(100);
}
