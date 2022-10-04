/// RS485ReceivingDataWriter.ino

// DFR0219 example code to receive data and send output to I2C.

// Make I2C optional to take it out for production code.
#define WRITE_I2C

#ifdef WRITE_I2C
#include <Wire.h>
#define I2C_DEVICE 4  // Chose a device no.
#endif

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.
int ledPin = 13;
int val;

void setup() {
  // put your setup code here, to run once:
#ifdef WRITE_I2C
  Wire.begin(); // join i2c bus (address optional for master)
  Wire.write("RS485 Transmit Data Writer active\n");
#endif
  pinMode(ledPin, OUTPUT);
  pinMode(EN, OUTPUT);
  Serial.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
#ifdef WRITE_I2C
  Wire.beginTransmission(I2C_DEVICE); // transmit to device #I2C_DEVICE
#endif
  digitalWrite(EN, LOW); // Enable receiving data
  val = Serial.read();
  if (-1 != val) {
    if ('A' == val) {
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
      delay(500);
#ifdef WRITE_I2C
      Wire.write('A');
      Wire.endTransmission();    // stop transmitting
#endif 
    }
  }
}
