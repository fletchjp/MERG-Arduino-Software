// state_defs.h

// Global variables for the actual state.

//enum { LED_off, LED_on } Led_State;
ENUM2(LED_off,LED_on,Led_State)

//enum { RESET, BLINK, WAIT } Blink_State;
ENUM3(RESET, BLINK, WAIT,Blink_State)

ENUM3(RED, YELLOW, GREEN,Signal3_State)

ALIAS_C_INT(STATE_DELAY,1000)
ALIAS_C_INT(LED,13)

ALIAS_UINT32(timeLastTransition,0)

/*
 * States for the blink_state machine now in state_defs.h.
 */

S_RESET(S0,machine,Blink_State,RESET,"State 0, reset",LED,timeLastTransition)
S_BLINK(S1,machine,Blink_State,BLINK,"State 1, blink",LED)
S_WAIT(S2,machine,Blink_State,WAIT,"State 2, wait",timeLastTransition)

/*
 * Transitions between states for the blink_state machine.
 * Each of these lines generates the transition function.
 */

TRANSITION_DELAY(transitionS0S1,timeLastTransition,STATE_DELAY)
TRANSITION_TRUE(transitionS1S2)
TRANSITION_DELAY(transitionS2S1,timeLastTransition,STATE_DELAY)

// see add_transitions.h
//ADD_TRANSITIONS(S0,transitionS0S1,S1,transitionS1S2,S2,transitionS2S1)
