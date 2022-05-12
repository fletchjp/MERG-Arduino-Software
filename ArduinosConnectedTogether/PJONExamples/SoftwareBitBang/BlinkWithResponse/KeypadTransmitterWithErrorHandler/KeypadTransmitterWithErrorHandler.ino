/// @file KeypadTransmitterWithErrorHandler.ino
/// @brief Keypad Transmitter with error handler from Transmitter1 in Any/MultiStrategyLink

#include <PJONSoftwareBitBang.h>

PJONSoftwareBitBang bus(45);

/// error_handler
void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if(code == PJON_CONNECTION_LOST) {
    Serial.print("Connection lost with device id ");
    Serial.println(bus.packets[data].content[0], DEC);
  }
};

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

#include <IoAbstraction.h>
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

const byte ROWS = 4; /// four rows
const byte COLS = 4; /// four columns
/// define the symbols on the buttons of the keypads
/// PROGMEM is important.
const char layout[] PROGMEM = "123A456B789C*0#D"; // Chars have to be in a string.
//  '1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'

void setup() {
  Serial.begin(115200);
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
