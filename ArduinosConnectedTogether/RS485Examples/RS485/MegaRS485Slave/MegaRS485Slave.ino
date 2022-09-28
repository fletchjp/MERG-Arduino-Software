// MegaRS485Master
//
// developed from MegaSerial485comms2
// using code from SoftRS485Master

// This is to send and receive transmission using Serial1

/*-----( Import needed libraries )-----*/

/*-----( Declare Constants and Pin Numbers )-----*/

#define RS485pin 3
#define LEDpin 13

#define RS485Transmit    HIGH
#define RS485Receive     LOW

/*-----( Declare objects )-----*/

/*-----( Declare Variables )-----*/
int byteReceived;
int byteSend;

void setup() {
  // Start the communications on Serial1
  Serial1.begin(9600);
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(9600);

  // Set pins to OUTPUT
  pinMode(RS485pin,OUTPUT);
  pinMode(LEDpin,OUTPUT);
  
  Serial.println("MEGA Serial Remote has started");
  // Use LEDpin to show what the communication is doing.
  digitalWrite(LEDpin,LOW);
  digitalWrite(RS485pin, RS485Receive);  // Init Transceiver
  delay(2000);
  Serial.println("Serial Remote waiting for message");
}

void loop() {
  // put your main code here, to run repeatedly:

  char getdata = 'c';
/*
  digitalWrite(LEDpin,HIGH);
  digitalWrite(RS485pin,LOW);
  Serial1.print("abcdefghX");
  delay(5000);
  digitalWrite(LEDpin,LOW);
  delay(5000);
  */ 

  if (Serial1.available()) {
      digitalWrite(RS485pin, RS485Receive);  // Disable RS485 Transmit 
      digitalWrite(LEDpin,HIGH);
      //while(Serial1.available() && getdata !='X') {
        byteReceived = Serial1.read();
        Serial.print(byteReceived);
      //}
      //Serial.println("");
      digitalWrite(RS485pin, RS485Transmit);  // Enable RS485 Transmit
      Serial1.write(byteReceived); // Send the byte back     
      digitalWrite(RS485pin, RS485Receive);  // Disable RS485 Transmit 
      delay(10);
      digitalWrite(LEDpin,LOW);
 }


}
