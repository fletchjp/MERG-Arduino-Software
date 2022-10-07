/// RS485AutoDataWriter.ino

// DFR0219 example code to transmit and receive data using Auto485 and send output to I2C.
// Make I2C optional to take it out for production code.
#define WRITE_I2C

#ifdef WRITE_I2C
#include <Wire.h>
#define I2C_DEVICE 4  // Chose a device no.
#endif
#include <Auto485.h>
// I have made a small change to the header file to avoid a warning from the Arduino compiler.

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.

// Add flashing of LED pin.
int ledPin = 13;

Auto485  bus(EN); // Single pin version

void setup() {
  // put your setup code here, to run once:
  //pinMode(EN, OUTPUT);
  //Serial.begin(19200);
#ifdef WRITE_I2C
  Wire.begin(); // join i2c bus (address optional for master)
#endif
  bus.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(EN, HIGH); // Enable data transmit
  //Serial.print('A');
  if (bus.available() > 0 ) {
#ifdef WRITE_I2C
     Wire.beginTransmission(I2C_DEVICE); // transmit to device #I2C_DEVICE
     Wire.write("Reading ");
#endif
    while (bus.available() > 0 ) {
      bus.write(bus.read()); // mode -> transmit
    }
    bus.println(); // mode -> receive.
  }
  //delay(1000);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}
