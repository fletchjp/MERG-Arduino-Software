/// @file ControllerWriter.ino
/// @brief Wire Master Writer
/// by Nicholas Zambetti <http://www.zambetti.com>
///
/// Demonstrates use of the Wire library
/// Writes data to an I2C/TWI Peripheral device
/// Refer to the "Wire Peripheral Receiver" example for use with this
///
/// Created 29 March 2006
///
/// This example code is in the public domain.


#include <Wire.h>

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);           // start serial for output
  Serial.println("Controller Writer active");
}

byte x = 0;

void loop()
{
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  Serial.print("x is ");
  Serial.println(x);
  x++;
  delay(500);
}
