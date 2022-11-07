// --------------------------------------
// Test_I2C_AT24C256_EEPROM test of AT24C256 on GR046 board 
// adapted from
// i2c_scanner and now using AT24C256_library
//
// Modified from https://playground.arduino.cc/Main/I2cScanner/
// --------------------------------------

//#include <Wire.h>
#include <AT24C256.h>

#define EEPROM_I2C_ADDRESS 0x50

AT24C256 eeprom(EEPROM_I2C_ADDRESS, &Wire1);


byte i = 0;

void setup() {

  Serial.begin(115200);
  while (!Serial)
     delay(10);
  Serial.println("\nI2C AT24C256 EEPROM Test");
  pinMode(13,OUTPUT);
}

  char message[30];
  char writemessage[] = "1234567890";

void loop() {

    Serial.println("Sending...");
    Serial.println("----------------");


      Serial.print(EEPROM_I2C_ADDRESS,HEX);
      Serial.println(" device found");
      eeprom.write(0,(uint8_t*) writemessage, sizeof(writemessage));
      delay(1000);
      Serial.println("Data from EEPROM");
      Serial.println("----------------");
      eeprom.read(0, (uint8_t*) message, sizeof(message));
      Serial.println(message);
      Serial.println("----------------");
      
    delay(10000);           // wait 5 seconds for next scan
}
