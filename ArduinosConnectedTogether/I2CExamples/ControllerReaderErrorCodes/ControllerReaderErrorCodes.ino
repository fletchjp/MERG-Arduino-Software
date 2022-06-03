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
 int myRequestFrom(uint8_t address, uint8_t quantity);
 int myRequestFrom(uint8_t, uint8_t, uint8_t);
 int myRequestFrom(int address, int quantity);
 int myRequestFrom(int address, int quantity, int sendStop);
 // These are needed as this does not use the Wire buffers for rx.
 // That means that these access functions have to be different.
 virtual int myAvailable(void);
 virtual int myRead(void);
 virtual int myPeek(void);
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

int MyWire::myRequestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
  return myRequestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

int MyWire::myRequestFrom(int address, int quantity)
{
  return myRequestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

int MyWire::myRequestFrom(uint8_t address, uint8_t quantity)
{
  return myRequestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

int MyWire::myRequestFrom(int address, int quantity, int sendStop)
{
  return myRequestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int MyWire::myAvailable(void)
{
  return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int MyWire::myRead(void)
{
  int value = -1;
  
  // get each successive byte on each call
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }
  return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int MyWire::myPeek(void)
{
  int value = -1;
  
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }

  return value;
}


MyWire myWire;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  Serial.println("Controller Reader with Error Code reporting");
}

void loop() {
  // My version returns any errors as a negative number
  int what_results = myWire.myRequestFrom(8, 6);    // request 6 bytes from peripheral device #8
  if (what_results < 0 ) {    
    Serial.print("Error from requestFrom ");
    Serial.println(-what_results);
  } else {
    Serial.print(what_results);
    Serial.println(" bytes available");
    while (myWire.myAvailable()) { // peripheral may send less than requested
      char c = myWire.myRead(); // receive a byte as character
      Serial.print(c);         // print the character
    }
    Serial.println(" ");
  }
  delay(500);
}
