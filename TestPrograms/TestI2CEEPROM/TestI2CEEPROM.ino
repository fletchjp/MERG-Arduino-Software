// --------------------------------------
// TestI2CEEPROM test of AT24C256 on GR046 board 
// adapted from
// i2c_scanner
//
// Modified from https://playground.arduino.cc/Main/I2cScanner/
// --------------------------------------

#include <Wire.h>

// Set I2C bus to use: Wire, Wire1, etc.
#define WIRE Wire
#define EEPROM_I2C_ADDRESS_0 0x50
#define EEPROM_I2C_ADDRESS_1 0x51
#define N_CHARS 5
#define BIAS 5

byte i = 0;

byte EEPROM_I2C_ADDRESS = NULL;

void writeAT24(byte dataAddress, byte dataVal)
{
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  
  Wire.write(dataAddress);
  Wire.write(dataVal);
  Wire.endTransmission();

  delay(5);
}

byte readAT24(byte dataAddress)
{
  byte rcvData = NULL;
   
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write(dataAddress);
  Wire.endTransmission();

  delay(100);
  Wire.requestFrom(EEPROM_I2C_ADDRESS,1);
  if (Wire.available())
  {
    rcvData = Wire.read();
  }
  return rcvData;
}



void setup() {
  WIRE.begin();

  Serial.begin(115200);
  while (!Serial)
     delay(10);
  Serial.println("\nI2C EEPROM Test");
  pinMode(13,OUTPUT);
  EEPROM_I2C_ADDRESS = EEPROM_I2C_ADDRESS_0;
  Serial.print(BIAS);
  Serial.println(" bias");
}


void loop() {
  byte error, address, rcvData;
  int nDevices;

    Serial.println("Sending...");
    Serial.println("----------------");

    WIRE.beginTransmission(EEPROM_I2C_ADDRESS);
    error = WIRE.endTransmission();

    if (error == 0) {
      Serial.print(EEPROM_I2C_ADDRESS,HEX);
      Serial.println(" device found");
      for (i = 0; i < N_CHARS; i++)
      {
        writeAT24(i+BIAS, 33+i);
        delay(10);
      }  
      delay(1000);
      Serial.println("Data from EEPROM");
      Serial.println("----------------");
      for (i = 0; i < N_CHARS; i++)
      {
        //Serial.println((char)readAT24(i));
        rcvData = readAT24(i+BIAS);
        if (rcvData == NULL) {
          Serial.println("no data received");
        } else {
          Serial.println(rcvData,HEX);
        }
      }
      Serial.println("----------------");
      
    } else {
      Serial.print(EEPROM_I2C_ADDRESS);
      Serial.println(" device not found");
    }
    delay(10000);           // wait 5 seconds for next scan
}