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
///////////////////////////////////////////////////////////////////////////////////

//#include <TaskManagerIO.h>
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

	// Define the transmit task and implement run().
	// - final is optional but good practice
	// - run() must be public
	// - sleeping with remainder is optional
	// - inlining run() is optional


  class ReceiveTask final :
	public frt::Task<ReceiveTask>
  {
  public:
private:
		unsigned int remainder = 0;
  };




void switch_off() {
    digitalWrite(ledPin, LOW);
    Led_State = LED_off;
    Task_State = TASK_off;    
}

void switch_on() {
    digitalWrite(ledPin, HIGH);
    Led_State = LED_on;
    //taskManager.scheduleOnce(500,switch_off);
}

}

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Led_State = LED_off;
  Task_State = TASK_off;
}

void loop() {
  // put your main code here, to run repeatedly:
  //taskManager.runLoop();
  digitalWrite(EN, LOW); // Enable receiving data
  if (Task_State == TASK_off) {
      value = Serial.read();
      if (-1 != value) {
        if ('A' == value) {
           Task_State = TASK_on;
           //taskManager.execute(switch_on);
        }
      } 
  }
}
