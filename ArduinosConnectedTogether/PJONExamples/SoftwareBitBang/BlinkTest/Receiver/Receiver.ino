#include <PJONSoftwareBitBang.h>


PJONSoftwareBitBang bus(44);

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  /* Make use of the payload before sending something, the buffer where payload points to is
     overwritten when a new message is dispatched */
  if(payload[0] == 'B') {
    Serial.println("BLINK");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);
    digitalWrite(LED_BUILTIN, LOW);
  }
};

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // Initialize LED 13 to be off
  while (!Serial);
  Serial.begin(115200);
  Serial.println("Bitbang Test: Receiver");
  Serial.print("LED_BUILTIN is ");
  Serial.println(LED_BUILTIN);
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
  if (!bus.strategy.can_start()) {
    Serial.println("PJON not running - bus.strategy.can_start() returns false");
  } else {
    bus.begin();
    bus.set_receiver(receiver_function);
    Serial.println("PJON bus is running");
  }
};

void loop() {
  bus.receive(1000);
};
