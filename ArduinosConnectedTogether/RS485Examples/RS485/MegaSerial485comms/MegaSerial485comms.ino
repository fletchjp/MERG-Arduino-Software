
#define RS485pin 8
#define LEDpin 13

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(RS485pin,OUTPUT);
  Serial.println("MEGA has started");
}

void loop() {
  // put your main code here, to run repeatedly:

  char getdata = 'c';

  digitalWrite(LEDpin,LOW);
  digitalWrite(RS485pin,LOW);

  if (Serial1.available()) {
      while(Serial1.available() && getdata !='X') {
        getdata = Serial1.read();
        Serial.print(getdata);
      }
      Serial.println("");
  }
  
}
