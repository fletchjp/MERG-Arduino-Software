// RS485ReceivingDataFRT
// Changing to use frt and FreeRTOS 
// instead of task management using TaskManagerIO
// DFR0219 example code to receive data
//////////////////////////////////////////////////////////////////////////////////// 
// PSEUDOCODE
// Set TASK_off
// WHILE TASK_off IF data_available THEN IF value == 'A' THEN switch_on & set TASK_on
// switch_on: turn LED on and schedule switch_off in 500 milliseconds
// switch_off: turn LED off and set TASK_off
///////////////////////////////////////////////////////////////////////////////////
// This needs work to sort out how to schedule the tasks.
// I have made separate tasks SwitchTask and ReceiveTask.
// The idea is that ReceiveTask will run all the time at priority 2.
// It will turn on SwitchTask at priority 1 as needed.
// This task controls the timeing of the switches on and off.
// This idea DOES NOT WORK.
// What does work is to have the receipt of data in the loop()
// and then use the SwitchTask.
///////////////////////////////////////////////////////////////////////////////////

#include <frt.h>

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.
int ledPin = 13;
int value;

enum { LED_off, LED_on } Led_State;

enum { TASK_off, TASK_on} Task_State;

namespace
{
	// An anonymous namespace tells the linker that this code is only
	// referenced in this very compilation unit. That's good for larger
	// projects but not necessary for this sketch.

	// Define the tasks and implement run() for each.
	// - final is optional but good practice
	// - run() must be public
	// - sleeping with remainder is optional
	// - inlining run() is optional

  class SwitchTask final :
	public frt::Task<SwitchTask>
  {
    public:
    bool run() {
       if(Led_State == LED_off) {
         switch_on();
  		   msleep(500, remainder);
         switch_off();
      } else {
         switch_off();
   		   msleep(500, remainder);
         switch_on();
      }
      Task_State = TASK_off;
      return false; // Run once only.
    }

    void switch_off() {
      digitalWrite(ledPin, LOW);
      Led_State = LED_off;   
    }

    void switch_on() {
      digitalWrite(ledPin, HIGH);
      Led_State = LED_on;
    }

  private:
		unsigned int remainder = 0;

  };

  SwitchTask switch_task;

  class ReceiveTask final :
	public frt::Task<ReceiveTask>
  {
    public:
    bool run() {
      digitalWrite(EN, LOW); // Enable receiving data
      if (Task_State == TASK_off) {
        value = Serial.read();
        if (-1 != value) {
          if ('A' == value) {
            Task_State = TASK_on;
            switch_task.start(1);
          }
        }
      }
      return true;
    }
  private:
		unsigned int remainder = 0;
  };

  ReceiveTask receive_task;

}

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, HIGH);
  //delay(1000);
  //digitalWrite(ledPin, LOW);
  //delay(500);
  //digitalWrite(ledPin, HIGH);
  //delay(500);
  digitalWrite(ledPin, LOW);
  Led_State = LED_off;
  Task_State = TASK_off;
  //receive_task.start(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(EN, LOW); // Enable receiving data
  if (Task_State == TASK_off) {
      value = Serial.read();
      if (-1 != value) {
        if ('A' == value) {
           Task_State = TASK_on;
           switch_task.start(1);
        }
      }      
  }
}
