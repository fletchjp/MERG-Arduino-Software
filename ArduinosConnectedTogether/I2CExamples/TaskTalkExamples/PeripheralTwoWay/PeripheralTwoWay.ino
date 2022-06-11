/// @file PeripheralTwoWay.ino

#include <Wire.h>

// Change this unique address for each I2C peripheral node
#define NODE_ADDRESS 8

void requestEvent();
void receiveEvent(int);

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

void loop()
{
  // A delay is not needed here - it responds to the controller.
  //delay(NODE_READ_DELAY);
}

// function that executes whenever data is received from controller
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  Serial.println("receiveEvent");
  while(Wire.available()) // loop through all but the last
  { // Now reading as byte.
    byte c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  Serial.println(" ");         // print the integer
}

// a controller is calling and requesting something.
// this function is registered as an event, see setup()
void requestEvent() {
  // definitely send something back.
  Serial.println("requestEvent");
  Wire.write("hello "); // respond with message of 6 bytes
}
