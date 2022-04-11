/// @file Receiver.ino
/// @brief Receiver for Raspberry Pi Pico experiments with change pin number.

#define LED_OUTPUT_PIN 6
#define BITBANG_PIN 12

#include <PJONSoftwareBitBang.h>


PJONSoftwareBitBang bus(44);

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  /* Make use of the payload before sending something, the buffer where payload points to is
     overwritten when a new message is dispatched */
  if(payload[0] == 'B') {
    Serial.println("BLINK");
    digitalWrite(LED_OUTPUT_PIN, HIGH);
    delay(30);
    digitalWrite(LED_OUTPUT_PIN, LOW);
  }
};

void setup() {
  while (!Serial);
  Serial.begin(115200);  
  Serial.println("Raspberry Pi Pico Bitbang Test: Receiver");

  pinMode(LED_OUTPUT_PIN, OUTPUT);
  digitalWrite(LED_OUTPUT_PIN, HIGH);
  delay(300);
  digitalWrite(LED_OUTPUT_PIN, LOW); // Initialize LED 13 to be off

  bus.strategy.set_pin(BITBANG_PIN);
  bus.begin();
  bus.set_receiver(receiver_function);
  Serial.println("PJON bus is running");
  delay(300);
  digitalWrite(LED_OUTPUT_PIN, HIGH);
  delay(300);
  digitalWrite(LED_OUTPUT_PIN, LOW); // Initialize LED 13 to be off

};

void loop() {
  bus.receive(1000);
};
