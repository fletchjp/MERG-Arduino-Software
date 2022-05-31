/// @file TransmitterWithErrorHandler.ino
/// @brief Transmitter with error handler from Transmitter1 in Any/MultiStrategyLink

#include <PJONSoftwareBitBang.h>

PJONSoftwareBitBang bus(45);

/// error_handler  
void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if(code == PJON_CONNECTION_LOST) {
    Serial.print("Connection lost with device id ");
    Serial.println(bus.packets[data].content[0], DEC);
    // Avoid Serial interference during test flushing
    Serial.flush();
  }
};

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  /* Make use of the payload before sending something, the buffer where payload points to is
     overwritten when a new message is dispatched */
  if(payload[0] == 'B') {
    Serial.println("BLINK");
    // Avoid Serial interference during test flushing
    Serial.flush();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);
    digitalWrite(LED_BUILTIN, LOW);
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Transmitter starting");
  // Avoid Serial interference during test flushing
  Serial.flush();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // Initialize LED 13 to be off

  bus.strategy.set_pin(12);
  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.begin();
  bus.send_repeatedly(44, "B", 1, 1000000); // Send B to device 44 every second
}

void loop() {
  bus.update();
  bus.receive(1000);
};
