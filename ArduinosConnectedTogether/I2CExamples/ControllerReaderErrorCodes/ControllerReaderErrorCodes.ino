/// @file ControllerReaderErrorCodes.ino
/// Extending to look at error codes from requestFrom code.
/// This has a class MyWire which extends Wire.

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
extern "C" {
  #include "utility/twi.h"
}

class MyWire : public TwoWire {
private:
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;
  
public:
 MyWire() {}
 int myRequestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop);
};

uint8_t MyWire::rxBuffer[BUFFER_LENGTH];
uint8_t MyWire::rxBufferIndex = 0;
uint8_t MyWire::rxBufferLength = 0;

int MyWire::myRequestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
  int requestResult = 0;
  if (isize > 0) {
  // send internal address; this mode allows sending a repeated start to access
  // some devices' internal registers. This function is executed by the hardware
  // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

  Wire.beginTransmission(address);

  // the maximum size of internal address is 3 bytes
  if (isize > 3){
    isize = 3;
  }

  // write internal register address - most significant byte first
  while (isize-- > 0)
    Wire.write((uint8_t)(iaddress >> (isize*8)));
   requestResult = Wire.endTransmission(false);
   if (requestResult > 0) {
      Serial.print("endTransmission returns ");
      Serial.println(requestResult);
      return -requestResult;
   }
  }

  // clamp to buffer length
  if(quantity > BUFFER_LENGTH){
    quantity = BUFFER_LENGTH;
  }
  // perform blocking read into buffer
  uint8_t read = twi_readFrom(address, rxBuffer, quantity, sendStop);
  // set rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = read;

  return read;
}

MyWire myWire;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
}

void loop() {
  int what_results = myWire.requestFrom(8, 6);    // request 6 bytes from peripheral device #8
  if (what_results < 0 ) {    
    Serial.print("Error from requestFrom ");
    Serial.println(-what_results);
  }
  else {
    Serial.print(what_results);
    Serial.println(" bytes available");
    while (Wire.available()) { // peripheral may send less than requested
      char c = Wire.read(); // receive a byte as character
      Serial.print(c);         // print the character
    }
  }
  delay(500);
}
