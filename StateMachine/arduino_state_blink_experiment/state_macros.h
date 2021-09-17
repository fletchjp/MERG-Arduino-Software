// state_macros.h

#define ENUM2(value1,value2,name)  enum { value1, value2 } name;
#define ENUM3(value1,value2,value3,name)  enum { value1, value2, value3 } name;

#define ALIAS_C_INT(name,value)    const int name=value; 
#define ALIAS_C_UINT32(name,value) const uint32_t name=value; 
#define ALIAS_UINT32(name,value)   uint32_t name=value; 

#define TRANSITION_TRUE(name) bool name(){ return true; }

#define TRANSITION_DELAY(name,time,delay) bool name(){ if (millis() - time >= delay) return true; return false; }
