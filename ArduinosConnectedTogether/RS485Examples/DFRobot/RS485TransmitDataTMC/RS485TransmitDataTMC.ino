// RS485TransmitDataTM
// Add task management using TaskManagerIO with event class
// DFR0219 example code to transmit data

// With the board set to PROG the output goes to the Serial Monitor if the speed is set correctly.

// 3rd party libraries
#include <TaskManagerIO.h>

const byte VER_MAJ  = 0;
const byte VER_MIN  = 1;
const byte VER_DETAIL = 0;

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
    // Now set up so that alternate calls to transmit do different things.
    // This is the solution to having two different activities
    // alternately at 500 millisecond intervals
      if (Led_State == LED_off) {
        digitalWrite(EN, HIGH); // Enable data transmit
        Serial.print('A');
        digitalWrite(blinker_pin, HIGH);
        // The following is not necessary here - there could be extensions.
        digitalWrite(EN, LOW);  // Enable data receive
        Led_State = LED_on;
      } else {
       //delay(500); // I don't like the delay in here - think of something better.
        digitalWrite(blinker_pin, LOW);
        Led_State = LED_off;
      }   
    }
    ~RS485_transmit() override = default;
};

RS485_transmit transmit(ledPin);


void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  Serial.begin(115200);
  digitalWrite(ledPin, LOW);
  Led_State = LED_off;
  taskManager.scheduleFixedRate(500,&transmit);

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
