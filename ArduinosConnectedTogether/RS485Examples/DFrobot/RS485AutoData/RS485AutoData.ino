// DFR0219 example code to transmit and receive data using Auto485

#include <Auto485.h>
// I have made a small change to the header file to avoid a warning from the Arduino compiler.

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.

Auto485  bus(EN); // Single pin version

void setup() {
  // put your setup code here, to run once:
  //pinMode(EN, OUTPUT);
  //Serial.begin(19200);
  bus.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(EN, HIGH); // Enable data transmit
  //Serial.print('A');
  if (bus.available() > 0 ) {
    while (bus.available() > 0 )
      bus.write(bus.read()); // mode -> transmit
    bus.println(); // mode -> receive.
  }
  delay(1000);
}
