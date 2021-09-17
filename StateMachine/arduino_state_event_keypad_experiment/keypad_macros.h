// keypad_macros.h

#define ALIAS_C_BYTE(name,value)   const byte name=value; 
#define ALIAS_BYTE(name,value)     byte       name=value; 
#define ALIAS_UINT8(name,value)    uint8_t    name=value;

#define LAYOUT(layout,chars)       const char layout[] PROGMEM = chars;

#define BYTE_ARRAY_4(name,size,a,b,c,d) byte name[size] = {a,b,c,d};
