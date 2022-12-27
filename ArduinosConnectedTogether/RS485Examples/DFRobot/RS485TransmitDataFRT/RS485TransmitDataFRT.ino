// RS485TransmitDataFRT
// Changing to use frt and FreeRTOS 
// instead of task management using TaskManagerIO
// I do not need the event class for this case.
// See example RS485TransmitDataTMC for an event class
// Now set up so that alternate calls to transmit do different things.
// This is the solution to having two different activities
// alternately at 500 millisecond intervals

// DFR0219 example code to transmit data

// With the board set to PROG the output goes to the Serial Monitor if the speed is set correctly.

#include <frt.h>

int EN = 2;  // RS485 enable/disable pin for Rx/Tx
// High to transmit, low to receive.

// Add flashing of LED pin.
const int ledPin = 13;

enum { LED_off, LED_on } Led_State;

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


  class TransmitTask final :
	public frt::Task<TransmitTask>
  {
  public:
  // Now set up so that alternate calls to transmit do different things.
  // This is the solution to having two different activities
  // alternately at 500 millisecond intervals
   bool run() {
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
      // This is the replacement for a delay.
  		msleep(500, remainder);
    }
	private:
		unsigned int remainder = 0;
  };

  TransmitTask transmit_task;

} // end of namespace

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Led_State = LED_off;
  transmit_task.start(1);
}

void loop() {
  // put your main code here, to run repeatedly:
}
