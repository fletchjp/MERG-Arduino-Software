
//MegaSerial485comms2a for developments

// This is to send transmission using Serial1

#define RS485pin 3
#define LEDpin 13

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(RS485pin,OUTPUT);
  pinMode(LEDpin,OUTPUT);
  Serial.println("MEGA 2 has started");
  digitalWrite(LEDpin,LOW);
  delay(5000);
  Serial.println("MEGA 2 exiting setup");
}

void loop() {
  // put your main code here, to run repeatedly:

  char getdata = 'c';

  digitalWrite(LEDpin,HIGH);
  digitalWrite(RS485pin,LOW);
  Serial1.print("abcdefghX");
  Serial.print("abcdefghX");
  Serial.println("");
  delay(5000);
  digitalWrite(LEDpin,LOW);
  delay(5000);
   
/*
  if (Serial1.available()) {
      while(Serial1.available() && getdata !='X') {
        getdata = Serial1.read();
        Serial.print(getdata);
      }
      Serial.println("");
  }
*/

}
