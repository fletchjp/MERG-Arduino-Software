/// @file ControllerTwoWay.ino
/// Adapted using information from 
/// https://www.instructables.com/Aduino-IC-2-Way-Communication/
/// which may have some problems.

/// This sends a known number of characters.
/// The number to get back does not have to be known.

/// ControllerReader.ino
/// Wire Controller Reader
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

#define TO_CONTROLLER_SIZE 3
#define TO_PERIPHERAL_SIZE  4

#define START_NODE   8 // The starting I2C address of slave nodes
#define END_NODE     9 // last node to probe +1

#define NODE_READ_DELAY 1000 // Some delay between I2C node reads

byte messageToController[TO_CONTROLLER_SIZE];
byte messageToPeripheral[TO_PERIPHERAL_SIZE];

void  sendToPeripheral(int);
void  readFromPeripheral();

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  Serial.println("Controller starting");
}

void loop() {

   for (int address = START_NODE; address < END_NODE; address++) {
      sendToPeripheral(address);
      Serial.print("Sent to ");
      Serial.println(address);
      readFromPeripheral();
      Serial.println("Received");
   }
   delay(NODE_READ_DELAY);
}

void sendToPeripheral(int address) {
  // message is 0123
  for(int i = 0; i < TO_PERIPHERAL_SIZE; i++) {
    messageToPeripheral[i] = (byte)i;  
  }

  Wire.beginTransmission(address);
  Wire.write(messageToPeripheral, TO_PERIPHERAL_SIZE);
  Wire.endTransmission();
}

void readFromPeripheral() {
  // if data size is available from nodes
  Wire.requestFrom(8, 6);    // request 6 bytes from peripheral device #8
  Serial.print("reading ");
  Serial.print(Wire.available());
  Serial.println(" characters");
  while (Wire.available()) { // peripheral may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

}
