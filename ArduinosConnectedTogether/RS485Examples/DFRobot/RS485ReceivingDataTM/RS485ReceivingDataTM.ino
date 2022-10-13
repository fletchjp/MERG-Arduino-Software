// RS485ReceivingDataTM
// Add task management using TaskManagerIO
// This needs to be an event driven task.
// DFR0219 example code to receive data

#include <TaskManagerIO.h>

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.
int ledPin = 13;
int val;

enum { LED_off, LED_on } Led_State;

void receive_data()
{
  
}

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  Serial.begin(19200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Led_State = LED_off;
}

void loop() {
  // put your main code here, to run repeatedly:
  taskManager.runLoop();
  digitalWrite(EN, LOW); // Enable receiving data
  val = Serial.read();
  if (-1 != val) {
    if ('A' == val) {
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
      delay(500);
    }
  }
}
