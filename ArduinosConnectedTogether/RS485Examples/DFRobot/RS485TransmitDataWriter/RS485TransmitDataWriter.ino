/// RS485TransmitDataWriter.ino

// DFR0219 example code to transmit data and send output to I2C.

// Make I2C optional to take it out for production code.
#define WRITE_I2C

#ifdef WRITE_I2C
#include <Wire.h>
#define I2C_DEVICE 4  // Chose a device no.
#endif


// With the board set to PROG the output goes to the Serial Monitor if the speed is set correctly.

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.


void setup() {
  // put your setup code here, to run once:
#ifdef WRITE_I2C
  Wire.begin(); // join i2c bus (address optional for master)
#endif
  pinMode(EN, OUTPUT);
  Serial.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(EN, HIGH); // Enable data transmit
  Serial.print('A');
  delay(1000);
}
