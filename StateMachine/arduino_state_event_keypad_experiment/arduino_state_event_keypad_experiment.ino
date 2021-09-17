// arduino_state_event_keypad_experiment
// Modify to include keypad
// This example now runs IoAbstraction and TaskManagerIO to run the state machine.

// Now also using header files to define various things.

// This is equivalent to arduino_state_keypad and has lower memory and data.
// The states S0 S1 and S2 referred to are from the other code.

// Reset can be made by pressing key 0

/*
 * Converting example from 
 * https://arduinoplusplus.wordpress.com/2019/07/06/finite-state-machine-programming-basics-part-1/
 */

#include <Wire.h>
#include <IoAbstraction.h>
#include <TaskManagerIO.h>
#include <KeyboardManager.h>

// Macros for the definitions
#include "state_macros.h"
#include "keypad_macros.h"
// This now containes some definitions
#include "state_defs.h"
#include "keypad_defs.h"

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

class Blinker : public BaseEvent {
  private: 
    byte blinker_pin;
    char reset_key;
    static const uint32_t NEXT_CHECK_INTERVAL = 60UL * 1000UL; // 60 seconds away, maximum is about 1 hour.
  public:
    int taskId;
    Blinker(int pin, char key) : blinker_pin(pin), reset_key(key) 
    { 
        taskId = TASKMGR_INVALIDID;
    }
    uint32_t timeOfNextCheck() override {  
      //Serial.println("Event Next Check");
       Blink_State = WAIT;
       if (key_pressed == reset_key) { // This is S2S0
          Serial.println("Reset requested by key press");
          setStart();
       }  
       // This is the same as S0S1 and S2S1
       auto elapsed_millis = millis() - timeLastTransition;
       //Serial.println(elapsed_millis);
       if (elapsed_millis >= STATE_DELAY) {
          //Serial.println(elapsed_millis);
          //setTriggered(true);
          markTriggeredAndNotify();
       }
       return 100UL * 1000UL; // every 15 milliseconds we increment
    }
    void setStart() { // This is also for reset - was state S0
      Serial.println("State 0, reset");
      digitalWrite(blinker_pin,LOW);
      Blink_State = RESET;
      Led_State = LED_off;
      timeLastTransition = millis();
      Serial.println("State 2, wait");
    }     
    void exec() override {
      // State 1
      Blink_State = BLINK;
      Serial.println("State 1, blink");
      digitalWrite(blinker_pin, !digitalRead(blinker_pin));
      timeLastTransition = millis();      
      Serial.println("State 2, wait");
    }  
    ~Blinker() override = default;
};

Blinker blinker(LED,'0'); // LED and reset key.

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
