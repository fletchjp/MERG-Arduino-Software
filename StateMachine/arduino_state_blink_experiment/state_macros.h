// state_macros.h

#define ENUM2(value1,value2,name)  enum { value1, value2 } name;
#define ENUM3(value1,value2,value3,name)  enum { value1, value2, value3 } name;

#define ALIAS_C_INT(name,value)    const int name=value; 
#define ALIAS_C_UINT32(name,value) const uint32_t name=value; 
#define ALIAS_UINT32(name,value)   uint32_t name=value; 

#define TRANSITION_TRUE(name) bool name(){ return true; }

#define TRANSITION_DELAY(name,time,delay) bool name(){ if (millis() - time >= delay) return true; return false; }

#define S_RESET(name,machine,state,value,message,pin,time) State* name = machine.addState([]() { \
  if(machine.executeOnce){ \
    state = value; \
    Serial.println(message); \
    digitalWrite(pin,LOW); \
    time = millis(); \
  } \
});

#define S_BLINK(name,machine,state,value,message,pin) State* name = machine.addState([]() { \
    if(machine.executeOnce){ \
      state = value; \
      Serial.println(message); \
      digitalWrite(pin, !digitalRead(pin)); \
    } \
});

#define S_WAIT(name,machine,state,value,message,time) State* name = machine.addState([]() { \
    if(machine.executeOnce){ \
      state = value; \
      Serial.println(message); \
      time = millis(); \
    } \
});
