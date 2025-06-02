/// @file TransmitterWithErrorHandler.ino
/// @brief Transmitter with error handler from Transmitter1 in Any/MultiStrategyLink

#include <PJONSoftwareBitBang.h>

PJONSoftwareBitBang bus(45);

/// error_handler
void error_handler(uint8_t code, uint16_t data, void * /*custom_pointer*/) {
  if(code == PJON_CONNECTION_LOST) {
    Serial.print("Connection lost with device id ");
    Serial.println(bus.packets[data].content[0], DEC);
  }
};

void setup() {
  while (!Serial);
  Serial.begin(115200);
  Serial.println("Bitbang Test: Transmitter with error handler");
  #if defined(ARDUINO_AVR_NANO_EVERY)
    Serial.println("ARDUINO_AVR_NANO_EVERY");
    Serial.println("Serial1 is available");
    #if defined(AVR_NANO_4809_328MODE)
      Serial.println("Simulation of 328 mode is available"); 
    #endif
   #else
     Serial.println("Not an ARDUINO_AVR_NANO_EVERY");
   #endif

  bus.strategy.set_pin(12);
  bus.set_error(error_handler);
  if (!bus.strategy.can_start()) {
    Serial.println("PJON not running - bus.strategy.can_start() returns false");
  } else {
    bus.begin();
    bus.send_repeatedly(44, "B", 1, 1000000); // Send B to device 44 every second
    Serial.println("PJON bus is running");
  }
}

void loop() {
  bus.update();
};
