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

int EEPROM_I2C_ADDRESS = NULL;

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

  delay(5);
  Wire.requestFrom(EEPROM_I2C_ADDRESS,1);
  if (Wire.available())
  {
    rcvData = wire.read();
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
}


void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    WIRE.beginTransmission(address);
    error = WIRE.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else {
    Serial.print(ndevices);
    Serial.println(" found - done\n");
  }
  delay(5000);           // wait 5 seconds for next scan
}
