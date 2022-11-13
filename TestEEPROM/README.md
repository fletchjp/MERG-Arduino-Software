# Test EEPROM

These are a set of programs being used to test some C++ programming ideas for external EEPROM.

Each program has been started to cover a topic, although they have expanded as time has gone on.

This now has several options.

I found the AT24C256 library which was a source to help me sort out errors in my first attempts.

I now also have the Sparkfun library which has better options for putting data of different types.

## Test_I2C_EEPROM
 
Test program for the AT24C256 I2C EEPROM devices.
  
This is adapted from something I found online which did not work.
 
The read and write routines were not correct.

## Test_I2C_AT24C256_EEPROM1

Example using Wire1 which is needed for the DUEs using CBUS.

## Test_I2C_EEPROM_DUE

Version configured to use Wire1 on a DUE.

## Test_I2C_AT24C256_EEPROM

This uses an AT24C256 library which hides the details of read and write.

There is now an option to configure for Wire1.

# Test_I2C_AT24C256_EEPROM_DUE

This uses an AT24C256 library which hides the details of read and write.

There is now an option to configure for Wire1.

This is the one where I found that including eeprom.begin() in the constructor failed for Wire1.

This required a change to the AT24C256 library which broke the previous examples as eeprom.begin() has now to be in setup.

