/// @file TransmitterDue.ino
/// @brief BlinkTestTransmitter for Arduino DUE

/*
"The Arduino Due has three additional 3.3V TTL serial ports:
Serial1 on pins 19 (RX) and 18 (TX);
Serial2 on pins 17 (RX) and 16 (TX),
Serial3 on pins 15 (RX) and 14 (TX).
Pins 0 and 1 are also connected to the corresponding pins of the ATmega16U2 USB-to-TTL Serial chip,
which is connected to the USB debug port.
Additionally, there is a native USB-serial port on the SAM3X chip, SerialUSB'. "
*/

#include <PJONThroughSerial.h>

PJONThroughSerial bus(45);

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("ReceiverDUESerial running");
 // Using Serial1  
  bus.strategy.set_serial(&Serial1); // Pass the Serial object you want to use for communication
  bus.begin();
  bus.send_repeatedly(44, "B", 1, 1000000); // Send B to device 44 every second
};

void loop() {
  bus.update();
};         
