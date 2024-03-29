# Test Encoder and Keypad

These are a set of programs being used to test programming with an Encoder and a Keypad.

These use code from both the Test Encoder and Test Keypad programs. 

There are two types of keypad, both with 4 rows of keys. One sort have three columns and the other four columns.

I have code to support both types.

I also want to support direct pin access from an Arduino MEGA and I2C access from an Arduino UNO or NANO.

I have developed some codes for testing the different applications.

Most of these examples work with the IOAbstraction library.

## Martin's Library

Martin Da Costa has written a library for use with encoders. There are two versions in use here. 

The first is a local copy of his files which has now been replaced by use of a library (encoderMD).

The use of the first version will not be continued. The examples are retained as they illustrate some features.

## What works when the Keypad is on the I2C?

I have been attempting to combine code for the encoder using Martin's library with a keypad connected using I2C.

So far I have had mysterious failures when using both.

Initial tests have been with a 4duino which has only one 5V output pin when I need two.

I am changing over to use a NANO with a base board with plenty of 5V and ground pins.

I now have two examples working with the NANO hardware. These are in other folders.

### matrixKeyboardUNO44pins

This allows the start pin for the keypad to be adjusted easily. 

The original settung clashes with the pins used for the encoder.

### MartinsEncoder

Base example for the encoder library using IOAbstraction.

## matrixKeyboardUNO44pinsEncoder

This combines the two codes.

It works although when the encoder is turned fast the number can be missing a digit. I have put in delay(5) in the encoder print to avoid missing digits.

## matrixKeyboardUNO44pinsI2CEncoder

This is the next step to move the keypad to I2C and see if that works. Yes it does.

I have put in delay(5) in the encoder print to avoid missing digits.

## matrixKeyboardUNO44pinsI2CEncoderEvent

This moves the encoder to use the IOAbstraction event structure. This now works too.

I have put in delay(5) in the encoder print to avoid missing digits.

## matrixKeyboardEncoderMEGA34

Example of matrix keyboard support for MEGA with a 3 by 4 Keypad built into IoAbstraction with encoder

This is now adapted to report the state of the key.

## matrixKeyboardMartinsEncoderMEGA34

Example of matrix keyboard support for MEGA 3 by 4 Keypad built into IoAbstraction adding Martin's encoder

This runs Martin's encoder separately from the IoAbstraction code. I would like to integrate it.

## matrixKeyboardMartinsEncoderMEGA34Event

This now runs Martin's encoder integrated in the IoAbstraction code using EncoderEvent.

It has also been modified for encoderISR to return true if there is a change.
This is to help with multiple encoders.

## matrixKeyboardMartinsEncoderMEGA34Event2

This now runs Martin's encoder integrated in the IoAbstraction code.

I am now extending to two encoders.

Some previously global variables have to be now inside the EncoderEvent class so that they can be duplicated.

## matrixKeyboardMartinsLibraryMEGA34Event2

This version uses Martin's encoder library integrated in the IoAbstraction code.

This case also has two encoders.

## matrixKeyboardMartinsLibraryMEGA34EventArray

This version uses Martin's encoder library integrated in the IoAbstraction code.

This case has two encoders accessible using an array.

This is a test for use with CANCMDDC.

