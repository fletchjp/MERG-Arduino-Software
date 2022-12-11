// blinker.h

#ifndef BLINKER_H
#define BLINKER_H

// Blinker class using IOAbstraction BaseEvent
class Blinker : public BaseEvent {
  private: 
    byte blinker_pin;
    char reset_key;
    static const uint32_t NEXT_CHECK_INTERVAL = 60UL * 1000UL; // 60 seconds away, maximum is about 1 hour.
  public:
    int taskId;
    // Constructor
    Blinker(int pin, char key) : blinker_pin(pin), reset_key(key) 
    { 
        taskId = TASKMGR_INVALIDID;
    }
    // This deals with most of the state transitions.
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
    // This is also for reset - was state S0
    void setStart() { 
      Serial.println("State 0, reset");
      digitalWrite(blinker_pin,LOW);
      Blink_State = RESET;
      Led_State = LED_off;
      timeLastTransition = millis();
      Serial.println("State 2, wait"); // This is S0S2 - there is no code needed.
    }
    // This is State 1    
    void exec() override {
      // State 1
      Blink_State = BLINK;
      Serial.println("State 1, blink");
      digitalWrite(blinker_pin, !digitalRead(blinker_pin));
      timeLastTransition = millis();
      Serial.println("State 2, wait"); // This is S1S2 - there is no code needed.
    }
    // Destructor
    ~Blinker() override = default;
};



#endif
