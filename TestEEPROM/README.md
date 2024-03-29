# Test EEPROM

These are a set of programs being used to test some C++ programming ideas for external EEPROM.

Each program has been started to cover a topic, although they have expanded as time has gone on.

This now has several options.

I found the AT24C256 library which was a source to help me sort out errors in my first attempts.

I now also have the Sparkfun library which has better options for putting data of different types.

There is now a test for that here and some more examples available (not adapted for AT24C256).

I have now moved here some more examples which I put together as part of the work on external EEPROM for CBUS.

This includes one for the Raspberry Pi Pico RP2040 where I have found that with Arduino-Pico it is necessary to set the I2C pins.

## Test_I2C_EEPROM
 
Test program for the AT24C256 I2C EEPROM devices.
  
This is adapted from something I found online which did not work.
 
The read and write routines were not correct.

## Test_I2C_EEPROM_DUE

Version configured to use Wire1 on a DUE.

## Test_I2C_AT24C256_EEPROM

This uses an AT24C256 library which hides the details of read and write.

There is now an option to configure for Wire1.

## Test_I2C_AT24C256_EEPROM1

Example using Wire1 which is needed for the DUEs using CBUS.

## Test_I2C_AT24C256_EEPROM_DUE

This uses an AT24C256 library which hides the details of read and write.

There is now an option to configure for Wire1.

This is the one where I found that including eeprom.begin() in the constructor failed for Wire1.

This required a change to the AT24C256 library which broke the previous examples as eeprom.begin() has now to be in setup.

## Test_I2C_Sparkfun_EEPROM

This is basically the Sparkfun EEPROM Example 2 with the settings for the AT24C256.

This library has the advantage of having a put routine which can handle different variable types in a consistent way.

## AT24C256 examples

These are examples for the AT24C256 library.

## EEROM_tester_DUE and EEPROM_tester_RP2040

These examples use the Wire library to communicate with the EEPROM.

These have been extended from and I2C scanner code.

### EEPROM_tester_RP2040

This code has been used to sort out the problems of getting the code to work with Arduino-Pico.

The pins needed for I2C have to be set and there are different choices for Wire and Wire1.

## DUE Flash Storage examples

### DUEFlashStorageExample DueFlashStorageStructExample

These are copies of two of the examples which come wiht the DUEFlashStorage library.

### DUEcanImpleExample

This is a test routine for the flash storage code written for use with OpenLCB.
