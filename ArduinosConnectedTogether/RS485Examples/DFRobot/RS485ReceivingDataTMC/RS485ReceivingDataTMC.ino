// RS485ReceivingDataTMC
// Add task management using TaskManagerIO
// This needs to be an event driven task using a class
// NOT OPERATIONAL
// DFR0219 example code to receive data
//////////////////////////////////////////////////////////////////////////////////// 
// PSEUDOCODE
// Set TASK_off
// WHILE TASK_off IF data_available THEN START receive_data and set TASK_on
// WHEN task is finished THEN set TASK_off
// WHILE TASK_on THEN any more tasks. (not needed)

///////////////////////////////////////////////////////////////////////////////////

// 3rd party libraries
#include <Streaming.h>
#include <TaskManagerIO.h>

const byte VER_MAJ  = 1;
const byte VER_MIN  = 0;
const byte VER_DETAIL = 1;


int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.
int ledPin = 13;
int value, saved_value;

enum { LED_off, LED_on } Led_State;

enum { TASK_off, TASK_on} Task_State;

class RS485_receive : public BaseEvent {
  int blinker_pin;
  public:
    int taskId;
    RS485_receive(int pin) : blinker_pin(pin)
    { 
        taskId = TASKMGR_INVALIDID;
    }
    uint32_t timeOfNextCheck() override {  
       return 100UL * 1000UL; // every 15 milliseconds we increment
    }  
    void exec() override {
        if ('A' == saved_value) {
           if (Led_State == LED_off) {
              digitalWrite(ledPin, HIGH);
              Led_State = LED_on;
              Task_State = TASK_on;
           } else {
              digitalWrite(ledPin, LOW);
              Led_State = LED_off;
              Task_State = TASK_off;
           }
           Task_State = TASK_off;
       }
    }
    ~RS485_receive() override = default;
};    


RS485_receive receive_data(ledPin);

void setup() {
  // put your setup code here, to run once:
  while (!Serial);
  pinMode(EN, OUTPUT);
  Serial.begin(19200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Led_State = LED_off;
  Task_State = TASK_off;
  saved_value = 'X';
}

void loop() {
  // put your main code here, to run repeatedly:
  taskManager.runLoop();
  digitalWrite(EN, LOW); // Enable receiving data
  if (Task_State == TASK_off) {
      value = Serial.read();
      if (-1 != value) {
        saved_value = value;
        Task_State = TASK_on;
        taskManager.registerEvent(&receive_data);
      } 
  }
}
