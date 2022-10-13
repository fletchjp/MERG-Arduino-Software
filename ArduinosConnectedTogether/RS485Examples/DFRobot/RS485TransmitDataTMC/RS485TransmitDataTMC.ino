// RS485TransmitDataTM
// Add task management using TaskManagerIO with event class
// DFR0219 example code to transmit data

// With the board set to PROG the output goes to the Serial Monitor if the speed is set correctly.

#include <TaskManagerIO.h>

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.

// Add flashing of LED pin.
const int ledPin = 13;

enum { LED_off, LED_on } Led_State;

class RS485_transmit : public BaseEvent {
  int blinker_pin;
  public:
    int taskId;
    RS485_transmit(int pin) : blinker_pin(pin)
    { 
        taskId = TASKMGR_INVALIDID;
    }
    uint32_t timeOfNextCheck() override {  
       return 100UL * 1000UL; // every 15 milliseconds we increment
    }  
    void exec() override {
      
    }
    ~RS485_transmit() override = default;
};

RS485_transmit transmit(ledPin);


void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  Serial.begin(19200);

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
}
