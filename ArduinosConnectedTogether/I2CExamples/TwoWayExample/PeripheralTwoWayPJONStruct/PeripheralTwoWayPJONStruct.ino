/// @file PeripheralTwoWayPJONStruct.ino
/// @brief Getting data from the MEGATaskIntegrationI2CData as a struct
/// Adding PJON handling.
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

#include <PJONSoftwareBitBang.h>

PJONSoftwareBitBang bus(44);

#include <Wire.h>

#define TO_CONTROLLER_SIZE 3
#define TO_PERIPHERAL_SIZE  10

#define NODE_READ_DELAY 1000 // Some delay between I2C node reads

// Change this unique address for each I2C slave node
#define NODE_ADDRESS 8

byte messageToController[TO_CONTROLLER_SIZE];
byte messageToPeripheral[TO_PERIPHERAL_SIZE];
byte nodeReceive[TO_PERIPHERAL_SIZE];
char PJONbuffer[TO_PERIPHERAL_SIZE];

void requestEvent();
void receiveEvent();

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  /* Make use of the payload before sending something, the buffer where payload points to is
     overwritten when a new message is dispatched */
  if(payload[0] == 'B') {
    Serial.println("BLINK");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);
    digitalWrite(LED_BUILTIN, LOW);
    // Avoid simultaneous transmission of Serial and SoftwareBitBang data
    Serial.flush();
    bus.reply("B", 1);
  }
};

void setup()
{
  Wire.begin(NODE_ADDRESS);     // join i2c bus with address NODE_ADDRESS
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  Serial.begin(115200);           // start serial for output
  Serial.println("Peripheral Receiver active for data");
  Serial.print("Peripheral No ");
  Serial.println(NODE_ADDRESS);
  // Avoid simultaneous transmission of Serial and SoftwareBitBang data
  Serial.flush();
  bus.strategy.set_pin(4);
  bus.begin();
  bus.set_receiver(receiver_function);
}

/*
void readFromMaster() {
  for(int i = 0; i < TO_PERIPHERAL_SIZE; i ++){
    nodeReceive[i] = Wire.read();
  }
  Serial.print("Controller says ");
  for(int i = 0; i < TO_PERIPHERAL_SIZE; i ++){
    Serial.print(nodeReceive[i]);  
  }
  Serial.println();
  // Avoid simultaneous transmission of Serial and SoftwareBitBang data
  Serial.flush();
}
*/

void loop()
{
  // I think the delay needs to go for PJON.
  //delay(NODE_READ_DELAY);
    bus.update();
    bus.receive(1000);
/*
    if(Wire.available()) {
    readFromMaster();
    //sendToMaster();
  }
*/
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  float value;
  byte i = 0;
  Serial.println("receiveEvent");
  while(Wire.available()) // loop through all but the last
  { // Now reading as byte.
    char c = Wire.read(); // receive byte as a character
    PJONbuffer[i] =  c;
    nodeReceive[i] = (byte)c;
    i++;
    Serial.print(c);         // print the character
  }
  // Use bytes to reconstruct value
  //memcpy(&value, nodeReceive, sizeof(value));
  //Serial.println(value);
  //int x = Wire.read();    // receive byte as an integer
  //Serial.println(x);         // print the integer
  // Avoid simultaneous transmission of Serial and SoftwareBitBang data
  Serial.flush();
  bus.send(45,PJONbuffer,i);
  //Wire.write(x);
}

// a controller is calling and requesting something.
void requestEvent() {
  // definitely send something back.
  Serial.println("requestEvent");
  // Avoid simultaneous transmission of Serial and SoftwareBitBang data
  Serial.flush();
  Wire.write("hello "); // respond with message of 6 bytes
  //Wire.write(2);  // one byte as an example.
}
