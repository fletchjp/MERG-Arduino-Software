/// @file ReceiverSoftSerial.ino
/// @brief BlinkTestReceiver using a Soft Serial port with response code
/// Using https://arduinojson.org/v6/how-to/do-serial-communication-between-two-boards/

#include <SoftwareSerial.h>

// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(10, 11); // RX, TX

#include <PJONThroughSerial.h>

PJONThroughSerial bus(44);

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
 /* Make use of the payload before sending something, the buffer where payload points to is
    overwritten when a new message is dispatched */
  if(payload[0] == 'B') {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);
    digitalWrite(LED_BUILTIN, LOW);
    if(!bus.update()) bus.reply("B", 1);
  }
};

void setup() {
   // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("ReceiverSoftSerial running with response");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // Initialize LED 13 to be off
  linkSerial.begin(4800);
  bus.strategy.set_serial(&linkSerial);
  bus.set_receiver(receiver_function);
  bus.begin();
  Serial.println("PJON bus running");
};

void loop() {
  bus.update();
  bus.receive();
};
