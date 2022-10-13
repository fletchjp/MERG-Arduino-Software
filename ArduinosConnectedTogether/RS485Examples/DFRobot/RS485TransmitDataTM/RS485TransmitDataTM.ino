// RS485TransmitDataTM
// Add task management using TaskManagerIO
// I do not need the event class for this case.
// See example RS485TransmitDataTMC for an event class
// DFR0219 example code to transmit data

// With the board set to PROG the output goes to the Serial Monitor if the speed is set correctly.

#include <TaskManagerIO.h>

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.

// Add flashing of LED pin.
const int ledPin = 13;

enum { LED_off, LED_on } Led_State;

void transmit()
{
  // put your main code here, to run repeatedly:
  digitalWrite(EN, HIGH); // Enable data transmit
  Serial.print('A');
  digitalWrite(ledPin, HIGH);
  delay(500); // I don't like the delay in here - think of something better.
  digitalWrite(ledPin, LOW);
  digitalWrite(EN, LOW);// Enable data receive
}

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  Serial.begin(19200);

  // This is at the end of setup()
  taskManager.scheduleFixedRate(1000,transmit);
}


void loop() {
  // put your main code here, to run repeatedly:
  taskManager.runLoop();
  //digitalWrite(EN, HIGH); // Enable data transmit
  //Serial.print('A');
  //digitalWrite(ledPin, HIGH);
  //delay(500);
  //digitalWrite(ledPin, LOW);
  //delay(500);
  //delay(1000);
}
