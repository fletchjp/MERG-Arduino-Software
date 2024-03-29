#include <Auto485.h>

#define DE_PIN 2
#define RE_PIN 3

Auto485 bus(DE_PIN, RE_PIN); // new Auto485 wrapper using DE_PIN & RE_PIN to toggle read/write mode on the MAX485

void setup() {
  // put your setup code here, to run once:
  bus.begin(9600); // open bus at 9600bps
  bus.println("Hello world, now listening"); // at the end of println, we return to listening

}

void loop() {
  // put your main code here, to run repeatedly:
  if (bus.available() > 0) {
    while (bus.available() > 0)
      bus.write(bus.read()); // mode -> transmit
    bus.println(); // mode -> receive
  }
  delay(1000);

}
