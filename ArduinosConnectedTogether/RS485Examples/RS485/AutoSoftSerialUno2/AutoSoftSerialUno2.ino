// AutoSoftSerialUno2

// Using AutoSoft485 in AltSoftSerialUno2 for developments.

// Based on MegaSerial485comm2a for an Arduino UNO.

// This needs to Tx on Pin 9 and Rx on Pin 8

// This is to send transmission using AltSoftSerial

//#define RS485pin 3
#define DE_PIN 3
#define RE_PIN 3

#define LEDpin 13

#include <AltSoftSerial.h>
#include "AutoSoft485.h"

AltSoftSerial altSerial;
AutoSoft485 bus(DE_PIN, RE_PIN, altSerial);

size_t datasize;

void setup() {
  Serial.begin(9600);
  Serial.println("AltSoftSerialUno2 Test Begin");
  //pinMode(RS485pin,OUTPUT);
  pinMode(LEDpin,OUTPUT);
  digitalWrite(LEDpin,HIGH);
  //digitalWrite(RS485pin,HIGH); //Send
  bus.begin(9600);
  bus.println("Hello World from AutoSoftSerialUno2");
  delay(1000);
  datasize =0;
}

void loop() {
  //char c;
  char getdata = 'c';
  
  digitalWrite(LEDpin,LOW);
  //digitalWrite(RS485pin,LOW); //Receive

  if (bus.available() > 0) {
    if (datasize ==0) Serial.println("AltSoftSerialUno2 is available");
    digitalWrite(LEDpin,LOW);
    while (bus.available() > 0) {
      datasize++;
      getdata = bus.read(); // mode -> receive
      Serial.print(getdata);
   }
    Serial.println(); // mode -> receive
    Serial.print(datasize);
    Serial.println(" characters");
  }
    // Now do some sending
  digitalWrite(LEDpin,HIGH);
  //digitalWrite(RS485pin,HIGH); // Transmit
  datasize = bus.print("abcdefghX");
  Serial.print("abcdefghX");
  Serial.println("");
  Serial.print(datasize);
  Serial.println(" characters");
  datasize =0;
  delay(1000);
//  digitalWrite(LEDpin,LOW);

/*
  if (Serial.available()) {
    c = Serial.read();
    altSerial.print(c);
  }
  if (altSerial.available()) {
    c = altSerial.read();
    Serial.print(c);
  }
*/  
}
