// arduino_state_keypad
// Modify to include keypad

// This example now runs the state machine example.
// Reset can be made by pressing key 0

// arduino_state_blink
// Simple example using the StateMachine library.

/*
 * Converting example from 
 * https://arduinoplusplus.wordpress.com/2019/07/06/finite-state-machine-programming-basics-part-1/
 */

#include <Wire.h>
#include <IoAbstraction.h>
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
// PROGMEM is important.
const char layout[] PROGMEM = "123A456B789C*0#D"; // Chars have to be in a string.
//  '1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'
//};
// These are in order of Keypad pins from 1 to 8.
// Pin 1 is on the left with the pad face up.
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
// This seems fussy. ROWS and COLS will not work here.
uint8_t rows = ROWS;
uint8_t cols = COLS;
KeyboardLayout keyLayout(rows, cols, layout);

//
// We need a keyboard manager class too
//
MatrixKeyboardManager keyboard;

// this examples connects the pins directly to an arduino but you could use
// IoExpanders or shift registers instead.
IoAbstractionRef arduinoIo = ioUsingArduino();

#include <StateMachine.h>

const int STATE_DELAY = 1000;
const int LED = 13;

char key_pressed = 'Z';

StateMachine machine = StateMachine();

/***********************************************************
 * What do we mean by state?
 * Do we mean the state of the LED (On/Off)
 * or the state of the process (Blinking/waiting)?
 * I think that is up to the user.
 ***********************************************************/


// Global variables for the actual state.

enum { LED_off, LED_on } Led_State;

enum { RESET, BLINK, WAIT } Blink_State;

uint32_t timeLastTransition = 0;
 
/*
 * States for the blink_state machine.
 */

// Reset
State* S0 = machine.addState([]() {
  if(machine.executeOnce){
    Serial.println("State 0, reset");
    digitalWrite(LED,LOW);
    Blink_State = RESET;
    Led_State = LED_off;
    timeLastTransition = millis();
  }
});

// Blink
State* S1 = machine.addState([]() {
    if(machine.executeOnce){
      Blink_State = BLINK;
      Serial.println("State 1, blink");
      digitalWrite(LED, !digitalRead(LED));
    }
});

// Wait
State* S2 = machine.addState([]() {
    if(machine.executeOnce){
      Blink_State = WAIT;
      Serial.println("State 2, wait");
      timeLastTransition = millis();
    }
});

/*
 * Transitions between states for the blink_state machine.
 */

bool transitionS0S1(){
    if (millis() - timeLastTransition >= STATE_DELAY) {
      return true;
    }
    return false;
}

// Go straight away to the wait state.
bool transitionS1S2(){
    return true;
}

bool transitionS2S1(){
    if (millis() - timeLastTransition >= STATE_DELAY) {
      return true;
    }
    return false;
}

// Go to the reset state
bool transitionS2S0(){
   if (key_pressed == '0') {
     Serial.println("Reset requested by key press");
     return true;
   }
   return false;
}

//
// We need a class that extends from KeyboardListener. this gets notified when
// there are changes in the keyboard state.
//
class MyKeyboardListener : public KeyboardListener {
public:
    void keyPressed(char key, bool held) override {
        Serial.print("Key ");
        Serial.print(key);
        Serial.print(" is pressed, held = ");
        Serial.println(held);
        key_pressed = key;
    }

    void keyReleased(char key) override {
        Serial.print("Released ");
        Serial.println(key);
        key_pressed = 'X';
    }
} myListener;



void setup() {
  // put your setup code here, to run once:
   while(!Serial) { }
   Serial.begin(115200);
   pinMode(LED,OUTPUT);
   Serial.println("Arduino state blink");

  
   // Add transition from S0 to S1
   S0->addTransition(&transitionS0S1,S1);
   // Add transitions from S1 to S2 and back again.
   S1->addTransition(&transitionS1S2,S2);
   S2->addTransition(&transitionS2S1,S1);
   S2->addTransition(&transitionS2S0,S0);

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

}

void loop() {
  // put your main code here, to run repeatedly:
    // as this indirectly uses taskmanager, we must include this in loop.
    taskManager.runLoop();
    machine.run();
  //delay(STATE_DELAY);
}
