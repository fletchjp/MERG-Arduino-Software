/////////////////////////////////////////////////////////////////////////////////////////
// arduino_state_event_keypad_experiment
// Modify to include keypad
// This example now runs IoAbstraction and TaskManagerIO to run the state machine.
//
// Now also using header files to define various things.
// I have also moved the Blinker code into blinker.h.
// Also the keypadlistener is in keypadlistener.h
//
// All the header files are now in definitions.h
//
//////////////////////////////////////////////////////////////////////////////////////
//
// This is equivalent to arduino_state_keypad and has lower memory and data.
// The states S0 S1 and S2 referred to are from the other code.
//
// Reset can be made by pressing key 0
//
///////////////////////////////////////////////////////////////////////////////////////
/*
 * Converting example from 
 * https://arduinoplusplus.wordpress.com/2019/07/06/finite-state-machine-programming-basics-part-1/
 */

#include <Wire.h>
#include <IoAbstraction.h>
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

#include "definitions.h"

KeyboardLayout keyLayout(rows, cols, layout);
//
// We need a keyboard manager class too
//
MatrixKeyboardManager keyboard;

// this examples connects the pins directly to an arduino but you could use
// IoExpanders or shift registers instead.
IoAbstractionRef arduinoIo = ioUsingArduino();

/***********************************************************
 * What do we mean by state?
 * Do we mean the state of the LED (On/Off)
 * or the state of the process (Blinking/waiting)?
 * I think that is up to the user.
 ***********************************************************/

// In this case:

// States:

// S0 : start or reset with the LED low.
// S1 : blink the LED changing the LED state.
// S2 : wait a set time.

// Transitions:

// S0S2 : go to S2
// S1S2 : go to S2
// S2S1 : at the end of waiting, go to S1.
// S2S0 : on key press break off and go to S0.

// Blinker now in header file blinker.h

Blinker blinker(LED,'0'); // LED and reset key.

//
// We need a class that extends from KeyboardListener. this gets notified when
// there are changes in the keyboard state. Now in keypadlistener.h
MyKeyboardListener myListener;

void setup() {
  // put your setup code here, to run once:
   while(!Serial) { }
   Serial.begin(115200);
   pinMode(LED,OUTPUT);
   Serial.println("Arduino state event blink");

    // Converted to copy the arrays.
    for (byte i = 0; i < ROWS; i++)
      keyLayout.setRowPin(i, rowPins[i]);
    for (byte i = 0; i < COLS; i++)
      keyLayout.setColPin(i, colPins[i]);

    // create the keyboard mapped to arduino pins and with the layout chosen above.
    // it will callback our listener
    keyboard.initialise(arduinoIo, &keyLayout, &myListener);

    // start repeating at 850 millis then repeat every 350ms
    keyboard.setRepeatKeyMillis(850, 350);

    Serial.println("Keyboard is initialised!");
    blinker.setStart();
    // Note to register as an event here.
    taskManager.registerEvent(&blinker);
}

void loop() {
    // put your main code here, to run repeatedly:
    // as this indirectly uses taskmanager, we must include this in loop.
    taskManager.runLoop();
}
