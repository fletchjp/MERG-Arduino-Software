/*
Multple Serial test

Receives from the main serial port, sends to the others.
Receives from serial port 1, sends to the main serial (Serial 0).

This example works only with boards with more than one serial like Arduino Mega, Due, Zero etc

For me it does not work on a MEGA. It compiles on a DUE.

The circuit:

    Any serial device attached to Serial port 1
    Serial monitor open on Serial port 0:

created 30 Dec. 2008
modified 20 May 2012
by Tom Igoe & Jed Roach
modified 27 Nov 2015
by Arturo Guadalupi

This example code is in the public domain.
Modified 9/7/2017 for Due support with SerialUSB
*/

void setup() {
  // put your setup code here, to run once:
  // initialize both serial ports:
  Serial.begin(9600);
  SerialUSB.begin(9600); // Initialize Native USB port
}

void loop() {
  // put your main code here, to run repeatedly:
// read from port 1, send to port 0:
  if (SerialUSB.available()) {
     int inByte = SerialUSB.read();
     Serial.write(inByte);
  }

// read from port 0, send to port 1:
if (Serial.available()) { 
    int inByte = Serial.read();
    SerialUSB.write(inByte);
  }
}
