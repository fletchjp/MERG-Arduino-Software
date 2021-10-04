// definitions.h

// Gather all the definitions into one file.

// Macros for the definitions
#include "all_macros.h"
// This now containes some definitions
#include "state_defs.h"
#include "keypad_defs.h"

// Blinker now in header file
#include "blinker.h"

//
// We need a class that extends from KeyboardListener. this gets notified when
// there are changes in the keyboard state. Now in keypadlistener.h
//
#include "keypadlistener.h"
