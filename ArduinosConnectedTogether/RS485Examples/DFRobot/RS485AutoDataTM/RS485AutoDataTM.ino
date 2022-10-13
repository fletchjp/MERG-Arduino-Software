// DFR0219 example code to transmit and receive data using Auto485

// This is running as a transmitter.
//#define TRANSMIT

#include <Auto485.h>
// I have made a small change to the header file to avoid a warning from the Arduino compiler.

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.

// Add flashing of LED pin.
int ledPin = 13;
int val;

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
#ifdef TRANSMIT
  bus.print('A');
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
#else
  if (bus.available() > 0 ) {
     val = bus.read();
     if (-1 != val) {
        if ('A' == val) {
          digitalWrite(ledPin, HIGH);
          delay(500);
          digitalWrite(ledPin, LOW);
          delay(500);
        }
     }
  }
#endif
//  if (bus.available() > 0 ) {
//    while (bus.available() > 0 ) {
//      bus.write(bus.read()); // mode -> transmit
//    }  
//    bus.println(); // mode -> receive.
//  }
}
