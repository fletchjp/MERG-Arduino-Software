# Test EEPROM

These are a set of programs being used to test some C++ programming ideas for external EEPROM.

Each program has been started to cover a topic, although they have expanded as time has gone on.


## Test_I2C_EEPROM
 
Test program for the AT24C256 I2C EEPROM devices.
  
This is adapted from something I found online which did not work.
 
The read and write routines were not correct.

## Test_I2C_EEPROM_DUE

Version configured to use Wire1 on a DUE.

## Test_I2C_AT24C256_EEPROM

This uses an AT24C256 library which hides the details of read and write.

There is now an option to configure for Wire1.

