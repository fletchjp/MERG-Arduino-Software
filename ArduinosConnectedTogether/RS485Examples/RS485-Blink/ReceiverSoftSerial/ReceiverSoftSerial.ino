/// @file ReceiverSoftSerial.ino
/// @brief BlinkTestReceiver using a Soft Serial port
/// Using https://arduinojson.org/v6/how-to/do-serial-communication-between-two-boards/

/* PJON MAX485 serial modules RS485 communication exampl */
 
 #include <SoftwareSerial.h>

// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(10, 11); // RX, TX


#include <PJONThroughSerial.h>


PJONThroughSerial bus(44);

void setup() {
   // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("RS485 ReceiverSoftSerial running");
  // Initialize LED 13 to be off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  // Initialize Serial instance used for PJON communication
  linkSerial.begin(115200);
  bus.strategy.set_serial(&linkSerial);
  // Set enable pins
  bus.strategy.set_RS485_rxe_pin(3);
  bus.strategy.set_RS485_txe_pin(2);
  bus.begin();
  bus.set_receiver(receiver_function);
  Serial.println("PJON bus running");
};

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  /* Make use of the payload before sending something, the buffer where payload points to is
     overwritten when a new message is dispatched */
  if(payload[0] == 'B') {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(3);
    digitalWrite(LED_BUILTIN, LOW);
    delay(3);
  }
}

void loop() {
  bus.update();
  bus.receive();
};
