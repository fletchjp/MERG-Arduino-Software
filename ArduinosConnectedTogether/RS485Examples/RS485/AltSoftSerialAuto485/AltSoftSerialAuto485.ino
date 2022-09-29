// AltSoftSerialAuto485

// I would like to combine AltSoftSerial with Auto485


#include <AltSoftSerial.h>
#include "AutoSoft485.h"

#define DE_PIN 3
#define RE_PIN 3

#define LEDpin 13

AltSoftSerial altSerial(DE_PIN, RE_PIN);
AutoSoft485 bus(DE_PIN, RE_PIN, altSerial);

void setup() {
  Serial.begin(9600);
  Serial.println("AltSoftSerial Test Begin");
  altSerial.begin(9600);
  altSerial.println("Hello World");
}

void loop() {
  char c;

  if (Serial.available()) {
    c = Serial.read();
    altSerial.print(c);
  }
  if (altSerial.available()) {
    c = altSerial.read();
    Serial.print(c);
  }
}
