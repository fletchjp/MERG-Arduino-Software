// RS485AutoDataTM
// Add task management using TaskManagerIO
// DFR0219 example code to transmit and receive data using Auto485
//////////////////////////////////////////////////////////////////////////////////// 
// PSEUDOCODE

// This is running as a transmitter when TRANSMIT is defined.
#define TRANSMIT

#include <TaskManagerIO.h>
#include <Auto485.h>
// I have made a small change to the header file to avoid a warning from the Arduino compiler.

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.

// Add flashing of LED pin.
int ledPin = 13;
int value;

Auto485  bus(EN); // Single pin version

enum { LED_off, LED_on } Led_State;

enum { TASK_off, TASK_on} Task_State;

#ifdef TRANSMIT

void transmit()
{
  // Now set up so that alternate calls to transmit do different things.
  // This is the solution to having two different activities
  // alternately at 500 millisecond intervals
  if (Led_State == LED_off) {
    digitalWrite(EN, HIGH); // Enable data transmit
    bus.print('A');
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

#else

void switch_off() {
    digitalWrite(ledPin, LOW);
    Led_State = LED_off;
    Task_State = TASK_off;    
}

void switch_on() {
    digitalWrite(ledPin, HIGH);
    Led_State = LED_on;
    taskManager.scheduleOnce(500,switch_off);
}

#endif

void setup() {
  // put your setup code here, to run once:
  //pinMode(EN, OUTPUT);
  //Serial.begin(19200);
  bus.begin(19200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Led_State = LED_off;
  Task_State = TASK_off;
#ifdef TRANSMIT
  taskManager.scheduleFixedRate(500,transmit);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(EN, HIGH); // Enable data transmit
  taskManager.runLoop();
#ifndef TRANSMIT
  if (Task_State == TASK_off) {
    if (bus.available() > 0 ) {
      value = bus.read();
      if (-1 != value) {
        if ('A' == value) {
           Task_State = TASK_on;
           taskManager.execute(switch_on);
        }
      }
    }
  }
#endif
}
