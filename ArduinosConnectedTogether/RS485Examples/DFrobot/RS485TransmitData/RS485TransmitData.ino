// DFR0219 example code to transmit data

// With the board set to PROG the output goes to the Serial Monitor if the speed is set correctly.

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.


void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  Serial.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(EN, HIGH); // Enable data transmit
  Serial.print('A');
  delay(1000);
}
