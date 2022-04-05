/// @file TransmitterSoftSerialKeypad.ino
/// @brief BlinkTestTransmitter using a Soft Serial port and Keypad
/// Using https://arduinojson.org/v6/how-to/do-serial-communication-between-two-boards/

#include <PJONThroughSerial.h>
#include <SoftwareSerial.h>

// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(10, 11); // RX, TX

PJONThroughSerial bus(45);

void setup() {

  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("TransmitterSoftSerial running");

  linkSerial.begin(4800);
  bus.strategy.set_serial(&linkSerial); // Pass the Serial object you want to use for communication
  bus.begin();
  bus.send_repeatedly(44, "B", 1, 1000000); // Send B to device 44 every second
  Serial.println("PJON bus running");
};

void loop() {
  bus.update();
};
