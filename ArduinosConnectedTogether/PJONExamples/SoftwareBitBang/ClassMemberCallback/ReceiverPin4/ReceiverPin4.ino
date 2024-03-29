// ReceiverPin4
// This works with an ELEGOO UNO as receiver but not on the MiniLab.


// Uncomment to use the mode you prefer (default SWBB_MODE 1)
// #define SWBB_MODE 1 // 1.95kB/s - 15625Bd
// #define SWBB_MODE 2 // 2.21kB/s - 17696Bd
// #define SWBB_MODE 3 // 2.94kB/s - 23529Bd
// #define SWBB_MODE 4 // 3.40kB/s - 27210Bd

#include <PJONSoftwareBitBang.h>


PJONSoftwareBitBang bus(44);

// Custom class
class MyClass {
  public:
    void doit() {
      Serial.println("Custom instance member function called correctly!");
    };
};

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  if(packet_info.custom_pointer)
    ((MyClass*)packet_info.custom_pointer)->doit();
};

MyClass myObject; // Custom class instance

void setup() {
  bus.strategy.set_pin(4);
  bus.begin();
  bus.set_receiver(receiver_function);
  bus.set_custom_pointer(&myObject); // Pointer to custom class instance
  Serial.begin(115200);
  Serial.println("Starting 1 second custom class instance pointer callback test...");
};

void loop() {
  long time_ = millis();
  unsigned int response_ = 0;
  while(millis() - time_ < 1000) response_ = bus.receive();
};
