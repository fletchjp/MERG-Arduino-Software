// MegaRS485Master
//
// developed from MegaSerial485comms1
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
  
  Serial.println("MEGA Hardware Serial1 Master");
  // Use LEDpin to show what the communication is doing.
  digitalWrite(LEDpin,LOW);
  digitalWrite(RS485pin, RS485Receive);  // Init Transceiver
  delay(2000);
  Serial.println("Use Serial Monitor, type in upper window, ENTER");
}

void loop() {
  // put your main code here, to run repeatedly:

  char getdata = 'c';

  digitalWrite(LEDpin,LOW);

  // Check for input from the Serial monitor.
  // Send whatever it is to the other Arduino.
  if (Serial.available())
  {
    digitalWrite(LEDpin,HIGH);
    // Send whatever is there to send.
    while(Serial1.available()) {
    byteReceived = Serial.read();
    digitalWrite(RS485pin,RS485Transmit);  // Enable RS485 Transmit 
    Serial1.write(byteReceived);           // Send byte to Remote Arduino
    delay(10);
    }
    digitalWrite(RS485pin, RS485Receive);  // Disable RS485 Transmit        
    digitalWrite(LEDpin,LOW);
  }
  
  if (Serial1.available()) {
     digitalWrite(LEDpin,HIGH);
     Serial.print('x');
      while(Serial1.available() && getdata !='X') {
        byteReceived = Serial1.read();  // Read received byte
        Serial.print(byteReceived);
      }
      Serial.println("");
      digitalWrite(LEDpin,LOW);
   }
   //delay(5000);
   //digitalWrite(LEDpin,HIGH);
   //delay(5000);
}
