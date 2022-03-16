# Test Encoder and Keypad

These are a set of programs being used to test programming with an Encoder and a Keypad.

These use code from both the Test Encoder and Test Keypad programs. 

There are two types of keypad, both with 4 rows of keys. One sort have three columns and the other four columns.

I have code to support both types.

I also want to support direct pin access from an Arduino MEGA and I2C access from an Arduino UNO or NANO.

I have developed some codes for testing the different applications.

Most of these examples work with the IO abstraction library.

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
