// Example2_Settings_AT24C256
// Settings modified for AT24C256 EEPROM

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

#include <Wire.h>

#include "SparkFun_External_EEPROM.h" // Click here to get the library: http://librarymanager/All#SparkFun_External_EEPROM
ExternalEEPROM myMem;

uint32_t mem_size, val1, val2;

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("I2C EEPROM example");
#ifdef __AVR_ATmega2560__
  Serial.println("running on Arduino MEGA 2560");
#endif
  Wire.begin();
  Wire.setClock(400000); //Most EEPROMs can run 400kHz and higher

  if (myMem.begin() == false)
  {
    Serial.println("No memory detected. Freezing.");
    while (1);
  }
  Serial.println("Memory detected!");

  //Set settings for this EEPROM
  //myMem.setMemorySize(512 * 1024 / 8); //In bytes. 512kbit = 64kbyte
  // Put values into 32 bit integers before calculation!
  val1 = 256;
  val2 = 128;
  // Either use variables or constants of the correct size to avoid overflows.
  //mem_size = val1 * val2;
  //mem_size = 256ul * 128ul;
  mem_size = 256ul * 1024 / 8;
  //mem_size = 32768;
  myMem.setMemorySize(32768); //In bytes. 256kbit = 32kbyte
  //myMem.setPageSize(128); //In bytes. Has 128 byte page size.
  myMem.setPageSize(64); //In bytes. Has 64 byte page size.
  myMem.enablePollForWriteComplete(); //Supports I2C polling of write completion
  myMem.setPageWriteTime(5); //5 ms max write time
  Serial.print("Calculated mem size in bytes: ");
  Serial.println(mem_size);
  Serial.print("Mem size in bytes: ");
  Serial.println(myMem.length());

  float myValue3 = -7.35;
  myMem.put(20, myValue3); //(location, data)
  float myRead3;
  myMem.get(20, myRead3); //location to read, thing to put data into
  Serial.print("I read: ");
  Serial.println(myRead3);
  
}

void loop()
{

}
