/// @file DUESerialTest.ino

/*

https://forum.arduino.cc/t/how-to-program-due-serial-ports/207911/11

Check each side separately.

Have you checked Serial1 on the DUE side ?
The sketch below checks Serial1 and Serial2: 
connect a jumper wire between RX1/TX2 and another one between RX2/TX1,
connect your DUE to the PC side via the programming port:

Have you connected the grounds together ?

Have you connected RX1(DUE) to TX(Pro Mini) and RX1(DUE) to TX(Pro Mini) ?
*/

char c = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
  Serial1.begin(250000);
  Serial2.begin(250000);

  Serial2.print("Hello");}

void loop() {
  // put your main code here, to run repeatedly:
 String s;
  s = "";

  while (Serial1.available() > 0) {
    c = Serial1.read();
    s += c;

  }
  if (s.length() > 0) {
    Serial.println(s);
    Serial2.print(s);
  }
}
