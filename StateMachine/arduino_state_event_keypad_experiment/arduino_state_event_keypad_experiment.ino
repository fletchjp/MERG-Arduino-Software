// arduino_state_event_keypad
// Modify to include keypad
// This example now runs IoAbstraction and TaskManagerIO to run the state machine.

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

const int STATE_DELAY = 5000;
const int LED = 13;

char key_pressed = 'Z';

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
