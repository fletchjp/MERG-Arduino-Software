/// @file PeripheralTwoWay.ino
/// Adapted using information from 
/// https://www.instructables.com/Aduino-IC-2-Way-Communication/
/// which may have some problems.

/// PeripheralReceiver.ino
/// Wire Peripheral Receiver
/// by Nicholas Zambetti <http://www.zambetti.com>
///
/// Demonstrates use of the Wire library
/// Receives data as an I2C/TWI Peripheral device
/// Refer to the "Wire Master Writer" example for use with this
///
/// Created 29 March 2006
///
/// This example code is in the public domain.


#include <Wire.h>

#define TO_CONTROLLER_SIZE 3
#define TO_PERIPHERAL_SIZE  4

#define NODE_READ_DELAY 1000 // Some delay between I2C node reads

// Change this unique address for each I2C slave node
#define NODE_ADDRESS 8

byte messageToController[TO_CONTROLLER_SIZE];
byte messageToPeripheral[TO_PERIPHERAL_SIZE];
byte nodeReceive[TO_PERIPHERAL_SIZE];

void requestEvent();
void receiveEvent();

void setup()
{
  Wire.begin(NODE_ADDRESS);     // join i2c bus with address NODE_ADDRESS
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  Serial.begin(115200);           // start serial for output
  Serial.println("Peripheral Receiver active");
  Serial.print("Peripheral No ");
  Serial.println(NODE_ADDRESS);
}


void readFromMaster() {
  for(int i = 0; i < TO_PERIPHERAL_SIZE; i ++){
    nodeReceive[i] = Wire.read();
  }
  Serial.print("Controller says ");
  for(int i = 0; i < TO_PERIPHERAL_SIZE; i ++){
    Serial.print(nodeReceive[i]);  
  }
  Serial.println();
}

void loop()
{
  delay(NODE_READ_DELAY);

    if(Wire.available()) {
    readFromMaster();
    //sendToMaster();
  }

}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

// a controller is calling and requesting something.
void requestEvent() {
  // definitely send something back.
  Wire.write(2);  // one byte as an example.
}
