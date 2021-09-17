// arduino_state_blink_experiment
// Simple example using the StateMachine library.
// Exploring some ideas from the DC++EX coding.

#include <StateMachine.h>
StateMachine machine = StateMachine();

// Macros for the definitions
#include "state_macros.h"

// This now containes some definitions
#include "state_defs.h"

/*
 * Converting example from 
 * https://arduinoplusplus.wordpress.com/2019/07/06/finite-state-machine-programming-basics-part-1/
 */



/***********************************************************
 * What do we mean by state?
 * Do we mean the state of the LED (On/Off)
 * or the state of the process (Blinking/waiting)?
 * I think that is up to the user.
 ***********************************************************/

/*
 * States for the blink_state machine now in state_defs.h.
 */

/*
 * Transitions between states for the blink_state machine now in state_defs.h.
 */

/* This is now replaced by add_transitions.h which is configurable.
void add_transitions()
{
  // Add transition from S0 to S1
  S0->addTransition(&transitionS0S1,S1);
  // Add transitions from S1 to S2 and back again.
  S1->addTransition(&transitionS1S2,S2);
  S2->addTransition(&transitionS2S1,S1);

}
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial) { }
  pinMode(LED,OUTPUT);
  Serial.println("Arduino state blink");

  //add_transitions();
  #include "add_transitions.h"

}

void loop() {
  // put your main code here, to run repeatedly:
  machine.run();
  //delay(STATE_DELAY);
}
