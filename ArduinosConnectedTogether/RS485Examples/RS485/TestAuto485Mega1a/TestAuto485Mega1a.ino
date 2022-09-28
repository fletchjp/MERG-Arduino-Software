// TestAutoMega1a to emulate MegaSerial485comms1a for developments

// This is to receive transmission using Serial1

#include <Auto485.h>

#define DE_PIN 3
#define RE_PIN 3

#define LEDpin 13

Auto485 bus(DE_PIN, RE_PIN,Serial1); // new Auto485 wrapper using DE_PIN & RE_PIN to toggle read/write mode on the MAX485

size_t datasize;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("TestAuto485Mega1a begin");
  pinMode(LEDpin,OUTPUT);
  bus.begin(9600); // open bus at 9600bps
  datasize = bus.println("Hello world, Mega1a is now sending"); // at the end of println, we return to listening
  Serial.print(datasize);
  Serial.println(" characters");
  datasize =0;
  digitalWrite(LEDpin,LOW);
  if (datasize ==0) Serial.println("Mega1a is now receiving");
}

void loop() {
    
  char getdata = 'c';

  // put your main code here, to run repeatedly:
  if (bus.available() > 0) {
    if (datasize ==0) Serial.println("Mega1 is available");
    digitalWrite(LEDpin,HIGH);
    while (bus.available() > 0) {
      datasize++;
      getdata = bus.read();
      Serial.print(getdata);
    }
    //bus.println(); // mode -> receive
    Serial.println("");
    Serial.print(datasize);
    Serial.println(" characters");
  }
  datasize =0;
  delay(1000);
  digitalWrite(LEDpin,LOW);
  //Serial.print(datasize);
  //Serial.println(" characters");

}
