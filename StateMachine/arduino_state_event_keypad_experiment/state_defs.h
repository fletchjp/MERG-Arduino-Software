// state_defs.h

// Global variables for the actual state.

//enum { LED_off, LED_on } Led_State;
ENUM2(LED_off,LED_on,Led_State)

//enum { RESET, BLINK, WAIT } Blink_State;
ENUM3(RESET, BLINK, WAIT,Blink_State)

//enum { RED, YELLOW, GREEN } Signal3_State;
ENUM3(RED, YELLOW, GREEN,Signal3_State)

ALIAS_C_INT(STATE_DELAY,1000)
ALIAS_C_INT(LED,13)

ALIAS_UINT32(timeLastTransition,0)
