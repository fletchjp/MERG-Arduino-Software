// RS485ReceivingDataFRT2
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
// The idea is that ReceiveTask will run all the time at priority 1.
// It will turn on SwitchTask at priority 2 as needed.
// This task controls the timeing of the switches on and off.
// This works for the first three or four switches and then the LED switches
// on and off more quickly. Using a mutex around Task_State does not change things.
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

  //struct Data {
  //   bool task_on;
  //   bool led_on;     
  //   unsigned long timestamp;
  //};

  //frt::Queue<Data,10> queue;

  frt::Mutex task_mutex, led_mutex;
  // There is a very strange relationship between the stack size and crashing.
  // The default is 192U and it crashes after about 4 iterations.
  // The crash is delayed for smaller stack size and earlier for larger stack size.
  // After a crash the LED blinks regardless of tasks.
  // This does not happen when the task is run from the loop().
  // My guess is that the memory used for the stack is not being freed
  // and the Arduino is running out of memory!!!
  // I am looking into how I could free the memory.
  class SwitchTask final :
	public frt::Task<SwitchTask,32U> 
  {
    public:
    bool run() {
       //Data data;
       //if(Led_State == LED_off) {
         switch_on();
  		   msleep(500, remainder);
         switch_off();
      //} else {
        // switch_off();
   		  // msleep(500, remainder);
        // switch_on();
      //}
      //task_mutex.lock();
      //Task_State = TASK_off;
      //task_mutex.unlock();
      //data.task_on = false;
      //data.led_on = (Led_State == LED_on);
      //data.timestamp = millis();
      //queue.push(data);
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
      //task_mutex.lock();
      if (!switch_task.isRunning()) {
      //if (Task_State == TASK_off) {
        value = Serial.read();
        if (-1 != value) {
          if ('A' == value) {
           //task_mutex.unlock();
            if (!switch_task.isRunning()) {
              switch_task.start(2);
              Task_State = TASK_on;
            }
          }
        }
      }
      if (!switch_task.isRunning())
        Task_State = TASK_off;
      //task_mutex.unlock();
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
  receive_task.start(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*digitalWrite(EN, LOW); // Enable receiving data
  if (Task_State == TASK_off) {
      value = Serial.read();
      if (-1 != value) {
        if ('A' == value) {
           Task_State = TASK_on;
           switch_task.start(1);
      }
    }      
  } */
}
