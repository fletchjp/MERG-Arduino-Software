/// @file ControllerTwoWayTaskClass.ino
/// Making the I2C operation a task class.

#include <TaskManagerIO.h>
#include <Wire.h>

#define TO_CONTROLLER_SIZE 3
#define TO_PERIPHERAL_SIZE  4

#define NODE_ADDRESS  8 // The I2C address of the peripheral node

#define NODE_READ_DELAY 1000 // Some delay between I2C node reads

byte messageToController[TO_CONTROLLER_SIZE];
byte messageToPeripheral[TO_PERIPHERAL_SIZE];

void  sendToPeripheral(int);
void  readFromPeripheral(int);


class SendAndReceive : public Executable {
   int address;
   public:
   SendAndReceive(int addr) : address(addr) { }
   void exec() override {
      sendToPeripheral(address);
      Serial.print("Sent to ");
      Serial.println(address);
      readFromPeripheral(address);
      Serial.println("Received");
   }

};

SendAndReceive sendAndReceive(NODE_ADDRESS);

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  Serial.println("Task Class Controller starting");
 // This is at the end of setup()
  taskManager.scheduleFixedRate(1000, &sendAndReceive);
}

void loop() {
    taskManager.runLoop();
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

void readFromPeripheral(int address) {
  // if data size is available from nodes
  Wire.requestFrom(address, 6);    // request 6 bytes from peripheral device #8
  Serial.print("Task class reading ");
  Serial.print(Wire.available());
  Serial.println(" characters");
  while (Wire.available()) { // peripheral may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  Serial.println(" ");
 
}
