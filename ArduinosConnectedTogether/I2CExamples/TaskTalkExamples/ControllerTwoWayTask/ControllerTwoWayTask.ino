/// @file ControllerTwoWayTask.ino
/// Making the I2C operation a task.

#include <TaskManagerIO.h>
#include <Wire.h>

#define TO_CONTROLLER_SIZE 3
#define TO_PERIPHERAL_SIZE  4

#define NODE_ADDRESS   8 // The I2C address of the peripheral node

#define NODE_READ_DELAY 1000 // Some delay between I2C node reads

byte messageToController[TO_CONTROLLER_SIZE];
byte messageToPeripheral[TO_PERIPHERAL_SIZE];

void  sendToPeripheral(int);
void  readFromPeripheral();



// This code has been moved from the loop().
void sendAndReceive() {

   int address = NODE_ADDRESS;
   sendToPeripheral(address);
   Serial.print("Sent to ");
   Serial.println(address);
   readFromPeripheral();
   Serial.println("Received");

}

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  Serial.println("Task Controller starting");

  // This sets up sendAndReceive to run once per second.
  taskManager.scheduleFixedRate(1000, sendAndReceive);
}

void loop() {
    taskManager.runLoop();
   // There is no need for a delat any longer.
   //delay(NODE_READ_DELAY);
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
  Wire.requestFrom(NODE_ADDRESS, 6);    // request 6 bytes from peripheral device #8
  Serial.print("Task reading ");
  Serial.print(Wire.available());
  Serial.println(" characters");
  while (Wire.available()) { // peripheral may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  Serial.println(" ");

}
