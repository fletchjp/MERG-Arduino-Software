// DEcanImplExample.ino
// This is to test the EEPROM implementation for the OpenLCB work.

/* This example will write a struct to memory which is a very convinient way of storing configuration parameters.
 Try resetting the Arduino Due or unplug the power to it. The values will stay stored. */

#ifdef __SAM3X8E__
#include "processor.h"            // auto-selects the processor type, and CAN lib, EEPROM lib etc.  
#else
#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;
#endif
// 3rd party libraries
#include <Streaming.h>


// The struct of the configuration.
struct Configuration {
  uint8_t a;
  uint8_t b;
  int32_t bigInteger;
  char* message;
  char c;
};

// initialize one struct
Configuration configuration;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial << "DUEcanImpl testing" << endl;
#ifdef __SAM3X8E__
  Serial << "Using OpenLCB EEPROM with " << EEPROM.length() << endl;
  Serial << "Location 0 " << EEPROM.read(0) << endl;
  //EEPROM.write(0,1);
  //Serial.flush();
#else
  /* Flash is erased every time new code is uploaded. Write the default configuration to flash if first time */
  // running for the first time?
  uint8_t codeRunningForTheFirstTime = dueFlashStorage.read(0); // flash bytes will be 255 at first run
  Serial.print("codeRunningForTheFirstTime: ");
  if (codeRunningForTheFirstTime) {
    Serial.println("yes");
    /* OK first time running, set defaults */
    configuration.a = 1;
    configuration.b = 0;
    configuration.bigInteger = 1147483647; // my lucky number
    configuration.message = "Hello world!";
    configuration.c = 's';

    // write configuration struct to flash at adress 4
    byte b2[sizeof(Configuration)]; // create byte array to store the struct
    memcpy(b2, &configuration, sizeof(Configuration)); // copy the struct to the byte array
    dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash

    // write 0 to address 0 to indicate that it is not the first time running anymore
    dueFlashStorage.write(0, 0); 
  }
  else {
    Serial.println("no");
  }
#endif
#ifdef __SAM3X8E__
    Serial << "Using OpenLCB EEPROM with " << EEPROM.length() << endl;
    Serial << "Location 0 " << EEPROM.read(0) << endl;
#endif

}

void loop() {
  /* read configuration struct from flash */
#ifndef __SAM3X8E__
  byte* b = dueFlashStorage.readAddress(4); // byte array which is read from flash at adress 4
  Configuration configurationFromFlash; // create a temporary struct
  memcpy(&configurationFromFlash, b, sizeof(Configuration)); // copy byte array to temporary struct
#endif

#ifdef __SAM3X8E__
  Serial << "Testing in loop" << endl;
#else
  // print the content
  Serial.print("a:");
  Serial.print(configurationFromFlash.a);

  Serial.print(" b:");
  Serial.print(configurationFromFlash.b);

  Serial.print(" bigInteger:");
  Serial.print(configurationFromFlash.bigInteger);

  Serial.print(" message:");
  Serial.print(configurationFromFlash.message);

  Serial.print(" c:");
  Serial.print(configurationFromFlash.c);
  Serial.println();
  Serial.println();
  /* change some values in the struct and write them back */
  // increment b by 1 (modulus 100 to start over at 0 when 100 is reached)
  configurationFromFlash.b = (configurationFromFlash.b+1) % 100;

  // change the message
  String message = configurationFromFlash.message;
  if (configurationFromFlash.message == "Hello world!")
    configurationFromFlash.message = "Hello Arduino Due!";
  else
    configurationFromFlash.message = "Hello world!";

  // write configuration struct to flash at adress 4
  byte b2[sizeof(Configuration)]; // create byte array to store the struct
  memcpy(b2, &configurationFromFlash, sizeof(Configuration)); // copy the struct to the byte array
  dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
#endif

  delay(1000);
}
