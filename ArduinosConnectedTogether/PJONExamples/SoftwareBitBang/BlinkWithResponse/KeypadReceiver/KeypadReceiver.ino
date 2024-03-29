/// @file KeypadReceiver.ino
/// @brief Keypad Receiver

#include <PJONSoftwareBitBang.h>


PJONSoftwareBitBang bus(44);

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  /* Make use of the payload before sending something, the buffer where payload points to is
     overwritten when a new message is dispatched */
  if(payload[0] == 'X') {
    Serial.println("BLINK");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);
    digitalWrite(LED_BUILTIN, LOW);
    bus.reply("X", 1);
  } else {
    char c = payload[0]; // This does the conversion.
    Serial.println(c);
    
  }
};

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // Initialize LED 13 to be off

  bus.strategy.set_pin(12);
  bus.begin();
  bus.set_receiver(receiver_function);

  Serial.begin(115200);
  Serial.println("Keypad Receiver running");
};

void loop() {
  bus.update();
  bus.receive(1000);
};
