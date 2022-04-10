/// @file Transmitter.ino
/// @brief Transmitter for Raspberry Pi Pico experiments with change pin number.

#include <PJONSoftwareBitBang.h>

#define BITBANG_PIN 12

PJONSoftwareBitBang bus(45);

/// error_handler
void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if(code == PJON_CONNECTION_LOST) {
    Serial.print("Connection lost with device id ");
    Serial.println(bus.packets[data].content[0], DEC);
  }
  if(code == PJON_PACKETS_BUFFER_FULL) {
    Serial.print("Packet buffer is full, has now a length of ");
    Serial.println(data);
    Serial.println("Possible wrong bus configuration!");
    Serial.println("higher PJON_MAX_PACKETS if necessary.");
  }
  if(code == PJON_CONTENT_TOO_LONG) {
    Serial.print("Content is too long, length: ");
    Serial.println(data);
  }
};

void setup() {
  while (!Serial);
  Serial.begin(115200);
  Serial.println("Raspberry Pi Pico Bitbang Test: Transmitter");
  bus.strategy.set_pin(BITBANG_PIN);
  bus.set_error(error_handler);
  bus.begin();
  bus.send_repeatedly(44, "B", 1, 1000000); // Send B to device 44 every second
  Serial.println("PJON bus is running");

}

void loop() {
  bus.update();
};
