// RS485TransmitDataTM
// Add task management using TaskManagerIO
// I do not need the event class for this case.
// See example RS485TransmitDataTMC for an event class
// Now set up so that alternate calls to transmit do different things.
// This is the solution to having two different activities
// alternately at 500 millisecond intervals

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
  // Now set up so that alternate calls to transmit do different things.
  // This is the solution to having two different activities
  // alternately at 500 millisecond intervals
  if (Led_State == LED_off) {
    digitalWrite(EN, HIGH); // Enable data transmit
    Serial.print('A');
    digitalWrite(ledPin, HIGH);
    // The following is not necessary here - there could be extensions.
    digitalWrite(EN, LOW);  // Enable data receive
    Led_State = LED_on;
  } else {
    //delay(500); // I don't like the delay in here - think of something better.
    digitalWrite(ledPin, LOW);
    Led_State = LED_off;
  }   
}

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  Serial.begin(19200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Led_State = LED_off;
  // This is at the end of setup()
  taskManager.scheduleFixedRate(500,transmit);
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
