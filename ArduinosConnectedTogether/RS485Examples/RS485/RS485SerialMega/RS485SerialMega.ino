/* Code from 
https://www.engineersgarage.com/arduino/rs485-communication-between-arduino-mega-and-arduino-pro-mini/
*/

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);//Using Serial1 Port
  Serial.begin(9600);
  pinMode(8, OUTPUT);//DE/RE Controling pin of RS-485
}

void loop() {
  // put your main code here, to run repeatedly:
 char getdata='m';

  digitalWrite(8,HIGH);//DE/RE=HIGH Transmit Enabled M1
  Serial1.print('9');//Write '9' and Fetch Data From Pro Mini
   
  digitalWrite(8,LOW);//DE/RE=LOW Receive Enabled M1
  delay(1000);
  
  if(Serial1.available()){ //If Serial Data is available
    
    while(Serial1.available() && getdata!='d')
    { 
    getdata=Serial1.read();
    Serial.print(getdata);
    }
    
    Serial.println("");
    }
}
