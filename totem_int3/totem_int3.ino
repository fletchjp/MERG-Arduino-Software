// totem_int3 renamed to put it into CBUS_CODES

// interrupt example from https://www.tutorialspoint.com/arduino/arduino_interrupts.htm
//
// Use pin 3 to free pin 2 for CAN interface.
int pin = 3; //define interrupt pin to 3
volatile int state = LOW; // To make sure variables shared between an ISR
//the main program are updated correctly,declare them as volatile.

void blink();

void setup() {
  Serial.begin(9600);
  Serial.println("Starting the event example");

  pinMode(pin, INPUT_PULLUP); //set pin 2 as input pullup (this was missing)
	pinMode(13, OUTPUT); //set pin 13 as output
	attachInterrupt(digitalPinToInterrupt(pin), blink, CHANGE);
	//interrupt at pin 2 blink ISR when pin to change the value
	digitalWrite(13, state); //pin 13 equal the state value
  Serial.println(state);
} 
void loop() { 
	//digitalWrite(13, state); //pin 13 equal the state value
} 

void blink() { 
	//ISR function
	state = !state; //toggle the state when the interrupt occurs
  digitalWrite(13, state); //pin 13 equal the state value
  Serial.println(state);
}
