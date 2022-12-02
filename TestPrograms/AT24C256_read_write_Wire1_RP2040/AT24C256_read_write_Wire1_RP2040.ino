// AT24C256 read write example for Wire1
// copied from the AT24C256 library example

#include <AT24C256.h>

// Set I2C bus to use: Wire, Wire1, etc.
#define WIRE Wire1

AT24C256 eeprom(0x50,&Wire1);

void setup() {
  delay(5000);
  char message[30];
  char writemessage[] = "Hello AT24C256 World!";
  WIRE.setSDA(6);
  WIRE.setSCL(7);
  //WIRE.begin(0x30);
  
  Serial.begin(115200);
  delay(5000);
  Serial.println("AT24C256 library example");
  WIRE.setSDA(6);
  WIRE.setSCL(7);
  eeprom.begin();
  Serial.println("reading eeprom");
  eeprom.read(0, (uint8_t*) message, sizeof(message));
  Serial.println(message);

  eeprom.write(0, (uint8_t*)writemessage, sizeof(message));
  Serial.println("wrote eeprom");
  
  Serial.println("reading eeprom");
  eeprom.read(0, (uint8_t*) message, sizeof(message));
  Serial.println(message);
}

void loop() {
  // put your main code here, to run repeatedly:

}
