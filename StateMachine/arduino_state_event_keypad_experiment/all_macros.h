// all macros as there are overlaps.

#define ENUM2(value1,value2,name)  enum { value1, value2 } name;
#define ENUM3(value1,value2,value3,name)  enum { value1, value2, value3 } name;
#define ENUM4(value1,value2,value3,value4,name)  enum { value1, value2, value3, value4 } name;
#define ENUM5(value1,value2,value3,value4,value5,name)  enum { value1, value2, value3, value4, value5 } name;

#define ALIAS_C_BYTE(name,value)   const byte name=value; 
#define ALIAS_C_INT(name,value)    const int name=value; 
#define ALIAS_C_UINT32(name,value) const uint32_t name=value; 

#define ALIAS_BYTE(name,value)     byte       name=value; 
#define ALIAS_UINT8(name,value)    uint8_t    name=value;
#define ALIAS_UINT32(name,value)   uint32_t name=value; 

#define LAYOUT(layout,chars)       const char layout[] PROGMEM = chars;

#define BYTE_ARRAY_4(name,size,a,b,c,d) byte name[size] = {a,b,c,d};
