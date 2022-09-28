// TestAutoMega2a to emulate MegaSerial485comms2a for developments

// This is to send transmission using Serial1 and Streaming

#include <Streaming.h>
#include <Auto485.h>

#define DE_PIN 3
#define RE_PIN 3

#define LEDpin 13

Auto485 bus(DE_PIN, RE_PIN,Serial1); // new Auto485 wrapper using DE_PIN & RE_PIN to toggle read/write mode on the MAX485

size_t datasize;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial.println("TestAuto485Mega2a begin");
  Serial  << F("TestAuto485Mega2a begin") << endl;
  pinMode(LEDpin,OUTPUT);
  bus.begin(9600); // open bus at 9600bps
  //datasize = bus.println("Hello world, Mega2 now listening"); // at the end of println, we return to listening
  bus << F("Hello world, Mega2 now listening") << endl; // at the end of println, we return to listening
  Serial.print(datasize);
  Serial.println(" characters");
  datasize =0;
  digitalWrite(LEDpin,LOW);
  if (datasize ==0) Serial.println("bus is receiving");
}

void loop() {

  char getdata = 'c';
  
  // put your main code here, to run repeatedly:
  if (bus.available() > 0) {
    if (datasize ==0) Serial.println("Mega2 is available");
    digitalWrite(LEDpin,LOW);
    while (bus.available() > 0) {
      getdata = bus.read(); // mode -> transmit
      Serial.print(getdata);
   }
    Serial.println(); // mode -> receive
    Serial.print(datasize);
    Serial.println(" characters");
  }
    // Now do some sending
  digitalWrite(LEDpin,HIGH);
  datasize =0;
  datasize = bus.print("abcdefghX");
  Serial.print("abcdefghX");
  Serial.println("");
  Serial.print(datasize);
  Serial.println(" characters");

  delay(2000);
  digitalWrite(LEDpin,LOW);
  //Serial.print(datasize);
  //Serial.println(" characters");

}
