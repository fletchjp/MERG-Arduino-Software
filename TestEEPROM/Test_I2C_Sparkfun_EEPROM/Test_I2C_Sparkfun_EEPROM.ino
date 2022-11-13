// Test_I2C_Sparkfun_EEPROM
// Adapted from the Sparkfun EEPROM library example 2 which has this licence:
/*
  Read and write settings and calibration data to an external I2C EEPROM
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 11th, 2019
  License: This code is public domain but you buy me a beer if you use this 
  and we meet someday (Beerware license).
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14764

  This example demonstrates how to set the various settings for a given EEPROM.
  Read the datasheet! Each EEPROM will have specific values for:
  Overall EEPROM size in bytes (512kbit = 65536, 256kbit = 32768)
  Bytes per page write (64 and 128 are common)
  Whether write polling is supported
  
  The I2C EEPROM should have all its ADR pins set to GND (0). This is default
  on the Qwiic board.

  Hardware Connections:
  Plug the SparkFun Qwiic EEPROM to an Uno, Artemis, or other Qwiic equipped board
  Load this sketch
  Open output window at 115200bps
*/

// --------------------------------------
// Test_I2C_EEPROM test of AT24C256 on GR046 board 
// adapted from
// i2c_scanner
//
// Modified from https://playground.arduino.cc/Main/I2cScanner/
// --------------------------------------
// The read and write routines now work.
// --------------------------------------

#include <Wire.h>

// Set I2C bus to use: Wire, Wire1, etc.
#define WIRE Wire
#define EEPROM_I2C_ADDRESS_0 0x50
#define EEPROM_I2C_ADDRESS_1 0x51
#define N_CHARS 5
#define BIAS 5

uint16_t i = 0;

byte EEPROM_I2C_ADDRESS = NULL;

void writeAT24(uint16_t dataAddress, byte dataVal)
{
  WIRE.beginTransmission(EEPROM_I2C_ADDRESS);
  
  WIRE.write((byte)(dataAddress & 0xFF00) >> 8);
  WIRE.write((byte)(dataAddress & 0x00FF));
  WIRE.write(dataVal);
  WIRE.endTransmission();

  delay(5);
}

byte readAT24(byte dataAddress)
{
  byte rcvData = NULL;
   
  WIRE.beginTransmission(EEPROM_I2C_ADDRESS);
  WIRE.write((byte)(dataAddress & 0xFF00) >> 8);
  WIRE.write((byte)(dataAddress & 0x00FF));
  WIRE.endTransmission();

  delay(100);
  WIRE.requestFrom(EEPROM_I2C_ADDRESS,1);
  if (WIRE.available())
  {
    rcvData = WIRE.read();
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
          Serial.print("0x");
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
