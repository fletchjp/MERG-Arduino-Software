/// @file TransmitterSoftSerial.ino
/// @brief BlinkTestTransmitter using a Soft Serial port
/// Using https://arduinojson.org/v6/how-to/do-serial-communication-between-two-boards/

// 3rd party libraries
#include <Streaming.h>
#include <PJONThroughSerial.h>
#include <SoftwareSerial.h>

const byte VER_MAJ  = 1;
const byte VER_MIN  = 0;
const byte VER_DETAIL = 0;

void heading()
{
  Serial << endl << endl << __FILE__ << endl;
  Serial << F("Ver: ") << VER_MAJ << F(".") << VER_MIN << F(".") << VER_DETAIL;
  Serial << F(" compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(" using compiler ") << __cplusplus << endl;
}

// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(10, 11); // RX, TX

PJONThroughSerial bus(45);

void setup() {

  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("TransmitterSoftSerial running");
  heading();
  linkSerial.begin(4800);
  bus.strategy.set_serial(&linkSerial); // Pass the Serial object you want to use for communication
  bus.begin();
  bus.send_repeatedly(44, "B", 1, 1000000); // Send B to device 44 every second
  Serial.println("PJON bus running");
};

void loop() {
  bus.update();
};
