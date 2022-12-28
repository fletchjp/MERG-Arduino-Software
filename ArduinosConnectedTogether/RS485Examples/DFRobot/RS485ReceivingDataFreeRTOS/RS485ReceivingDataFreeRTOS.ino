// RS485ReceivingDataFreeRTOS
// Changing to use FreeRTOS directly without frt.
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
// This is now translated to use the FreeRTOS interface
// As a first case this always flashes.
////////////////////////////////////////////////////////////////////////////////// 

#include <Arduino_FreeRTOS.h>

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.
int ledPin = 13;
int value;

enum { LED_off, LED_on } Led_State;

enum { TASK_off, TASK_on} Task_State;

void TaskSwitch  (void *pvParameters);
void TaskReceive (void *pvParameters);

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Led_State = LED_off;
  Task_State = TASK_off;
  xTaskCreate(TaskSwitch, "Switch", 128, NULL, 2, NULL);
  xTaskCreate(TaskReceive, "Receive", 128, NULL, 1, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void switch_off() {
  digitalWrite(ledPin, LOW);
  Led_State = LED_off;   
}

void switch_on() {
  digitalWrite(ledPin, HIGH);
  Led_State = LED_on;
}

void TaskSwitch(void *pvParameters)
{
  (void) pvParameters;

  for (;;)
  {
    switch_on();
  	vTaskDelay(500 / portTICK_PERIOD_MS );
    switch_off();
  	vTaskDelay(500 / portTICK_PERIOD_MS );    
  }
}

void TaskReceive(void *pvParameters)
{
  for (;;)
  {
    digitalWrite(EN, LOW); // Enable receiving data
    if (Task_State == TASK_off) {
      value = Serial.read();
      if (-1 != value) {
        if ('A' == value) {
          Task_State = TASK_on;
   	      vTaskDelay(500 / portTICK_PERIOD_MS );    
          Task_State = TASK_off;
        }
      }      
    }
  }
}
