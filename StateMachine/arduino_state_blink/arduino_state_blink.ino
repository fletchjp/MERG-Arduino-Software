// arduino_state_blink
// Simple example using the StateMachine library.

/*
 * Converting example from 
 * https://arduinoplusplus.wordpress.com/2019/07/06/finite-state-machine-programming-basics-part-1/
 */

#include <StateMachine.h>

const int STATE_DELAY = 1000;
const int LED = 13;

StateMachine machine = StateMachine();

/***********************************************************
 * What do we mean by state?
 * Do we mean the state of the LED (On/Off)
 * or the state of the process (Blinking/waiting)?
 * I think that is up to the user.
 ***********************************************************/



static enum { LED_off, LED_on } led_state;

static enum { RESET, BLINK, WAIT } blink_state;




void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
