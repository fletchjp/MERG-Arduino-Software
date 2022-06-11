/// @file PeripheralTwoWay.ino

#include <Wire.h>

#define TO_CONTROLLER_SIZE 3
#define TO_PERIPHERAL_SIZE  4

#define NODE_READ_DELAY 1000 // Some delay between I2C node reads

// Change this unique address for each I2C peripheral node
#define NODE_ADDRESS 8

byte messageToController[TO_CONTROLLER_SIZE];
byte messageToPeripheral[TO_PERIPHERAL_SIZE];
byte nodeReceive[TO_PERIPHERAL_SIZE];

void requestEvent();
void receiveEvent();

void setup()
{
  Wire.begin(NODE_ADDRESS);     // join i2c bus with address NODE_ADDRESS
  Wire.onReceive(receiveEvent); // register receive event
  Wire.onRequest(requestEvent); // register request event
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
  // A delay is not needed here - it responds to the controller.
  //delay(NODE_READ_DELAY);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  Serial.println("receiveEvent");
  while(1 < Wire.available()) // loop through all but the last
  { // Now reading as byte.
    byte c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
  Wire.write(x);
}

// a controller is calling and requesting something.
// this function is registered as an event, see setup()
void requestEvent() {
  // definitely send something back.
  Serial.println("requestEvent");
  Wire.write("hello "); // respond with message of 6 bytes
}
