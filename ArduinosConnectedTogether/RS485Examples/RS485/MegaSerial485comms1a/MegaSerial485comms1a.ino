//MegaSerial485comms1a for developments

// This is to receive transmission using Serial1

#define RS485pin 3
#define LEDpin 13

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(RS485pin,OUTPUT);
  pinMode(LEDpin,OUTPUT);
  Serial.println("MEGA 1 has started");
  digitalWrite(LEDpin,HIGH);
  delay(5000);
  Serial.println("MEGA 1 exiting setup");
}

void loop() {
  // put your main code here, to run repeatedly:

  char getdata = 'c';

  digitalWrite(LEDpin,LOW);
  digitalWrite(RS485pin,LOW); //Receive

  if (Serial1.available()) {
      Serial.print('x');
      while(Serial1.available() && getdata !='X') {
        getdata = Serial1.read();
        Serial.print(getdata);
      }
      Serial.println("");
  }
   delay(5000);
   digitalWrite(LEDpin,HIGH);
   delay(5000);

}
