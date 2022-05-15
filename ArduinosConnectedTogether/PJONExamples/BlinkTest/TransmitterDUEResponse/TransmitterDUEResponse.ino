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

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  /* Make use of the payload before sending something, the buffer where payload points to is
     overwritten when a new message is dispatched */
  if(payload[0] == 'B') {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("B");
    delay(30);
    digitalWrite(LED_BUILTIN, LOW);
  }
};

/// error_handler
void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if(code == PJON_CONNECTION_LOST) {
    Serial.print("Connection lost with device id ");
    Serial.println(bus.packets[data].content[0], DEC);
  }
};

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("TransmitterDUESerial running with response and error handling");
 // Using Serial1
  Serial1.begin(9600);
  while (!Serial1) continue;
  //if (Serial1.available() ) Serial.println("Serial1 is available");
  //else Serial.println("Serial1 is not available");
  bus.strategy.set_serial(&Serial1); // Pass the Serial object you want to use for communication
  bus.set_receiver(receiver_function);
  bus.set_error(error_handler);
  bus.begin();
  // None of this is useful.
  //if(!bus.begin() ) { The manual says this should work but it does not.
  //  Serial.println("PJON begin returns false");;
  //} else {
  //if (!bus.strategy.can_start()) {
  //  Serial.println("PJON not running - bus.strategy.can_start() returns false");
  //} else {
    bus.send_repeatedly(44, "B", 1, 1000000); // Send B to device 44 every second
    Serial.println("PJON bus is running");
  //}
  //}
};

void loop() {
  bus.update();
  uint16_t response_value = bus.receive();
  //if (response_value != 65535) Serial.println(response_value);
};         
