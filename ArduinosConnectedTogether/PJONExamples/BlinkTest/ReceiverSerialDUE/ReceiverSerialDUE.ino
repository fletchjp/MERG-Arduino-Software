/// @file ReceiverSerialDUE.ino
/// @brief BlinkTestReceiver using a DUE Serial1 port
/// Using https://arduinojson.org/v6/how-to/do-serial-communication-between-two-boards/

/*
"The Arduino Due has three additional 3.3V TTL serial ports:
Serial1 on pins 19 (RX) and 18 (TX);
Serial2 on pins 17 (RX) and 16 (TX),
Serial3 on pins 15 (RX) and 14 (TX).
Pins 0 and 1 are also connected to the corresponding pins of the ATmega16U2 USB-to-TTL Serial chip,
which is connected to the USB debug port.
Additionally, there is a native USB-serial port on the SAM3X chip, SerialUSB'. "
*/

// 3rd party libraries
#include <Streaming.h>
#include <PJONThroughSerial.h>

const byte VER_MAJ  = 1;
const byte VER_MIN  = 0;
const byte VER_DETAIL = 0;

void heading()
{
  Serial << endl << endl << __FILE__ << endl;
  Serial << F("Ver: ") << VER_MAJ << F(".") << VER_MIN << F(".") << VER_DETAIL;
  Serial << F(" compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(" using compiler ") << __cplusplus << endl;
}

PJONThroughSerial bus(44);

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
 /* Make use of the payload before sending something, the buffer where payload points to is
    overwritten when a new message is dispatched */
  if(payload[0] == 'B') {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);
    digitalWrite(LED_BUILTIN, LOW);
  }
};

void setup() {
   // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(115200);
  while (!Serial) continue;
  heading();
  Serial.println("ReceiverDUESerial running");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // Initialize LED 13 to be off
  // Using Serial1
  Serial1.begin(4800);
  bus.strategy.set_serial(&Serial1);
  bus.set_receiver(receiver_function);
  bus.begin();
  Serial.println("PJON bus running");
};

void loop() {
  bus.receive(1000);
};
