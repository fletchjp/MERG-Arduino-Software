/// @file Transmitter.ino
/// @brief Transmitter for Raspberry Pi Pico experiments with change pin number.

#include <PJONSoftwareBitBang.h>

#define BITBANG_PIN 12

PJONSoftwareBitBang bus(45);

void setup() {
  bus.strategy.set_pin(BITBANG_PIN);
  bus.begin();
  bus.send_repeatedly(44, "B", 1, 1000000); // Send B to device 44 every second

  Serial.begin(115200);
  Serial.println("Raspberry Pi Pico Bitbang Test: Transmitter");
}

void loop() {
  bus.update();
};
