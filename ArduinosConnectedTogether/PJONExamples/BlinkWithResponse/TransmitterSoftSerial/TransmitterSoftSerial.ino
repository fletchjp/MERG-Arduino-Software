/// @file TransmitterSoftSerial.ino
/// @brief BlinkTestTransmitter using a Soft Serial port with response code
/// Using https://arduinojson.org/v6/how-to/do-serial-communication-between-two-boards/

#include <PJONThroughSerial.h>
#include <SoftwareSerial.h>

// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(10, 11); // RX, TX

PJONThroughSerial bus(45);

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  /* Make use of the payload before sending something, the buffer where payload points to is
     overwritten when a new message is dispatched */
  if(payload[0] == 'B') {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);
    digitalWrite(LED_BUILTIN, LOW);
  }
};

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("TransmitterSoftSerial running with response");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // Initialize LED 13 to be off
  linkSerial.begin(4800);
  bus.strategy.set_serial(&linkSerial); // Pass the Serial object you want to use for communication
  bus.set_receiver(receiver_function);
  bus.begin();
  bus.send_repeatedly(44, "B", 1, 1000000); // Send B to device 44 every second
  Serial.println("PJON bus running");
};

void loop() {
  bus.update();
  bus.receive();
};
