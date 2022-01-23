# Test Keypad

These are a set of programs being used to test programming with a Keypad

There are two types of keypad, both with 4 rows of keys. One sort have three columns and the other four columns.

I have code to support both types.

I also want to support direct pin access from an Arduino MEGA and I2C access from an Arduino UNO or NANO.

I have developed some codes for testing the different applications.

Most of these examples work with the IO abstraction library.

## abstractingPinsPCF8574

This simple demo shows the use of the IO abstraction library with both regular IO pins, and also
with pins on a PCF8574, there's another example for multiIO, this just shows the basics.

## I2Ckeypad_keymap

Test of 4by4 keypad on I2C

The keypad works with I2C connected to PCF8574 board at 0x20

## matrixKeyboard2

Example of matrix keyboard support built into IoAbstraction

This example shows how to use the matrix keyboard support that's built into IoAbstraction,
it can be used out the box with either a 3x4 or 4x4 keypad, but you can modify it to use
any matrix keyboard quite easily.

It just sends the characters that are typed on the keyboard to Serial. The keyboard in This
example is connected directly to Arduino pins, but could just as easily be connected over
a PCF8574, MCP23017 or other IoAbstraction.

This version sets up the keyboard adapting the custom_keyboard example.

This is the basis for the remaining examples.

## matrixKeyboardI2C34

Example of matrix keyboard support for 3 by 4 Keypad built into IoAbstraction on I2C

The example is now working. A call to Wire.begin() is needed.
 
## matrixKeyboardI2C44

Example of matrix keyboard support for 4 by 4 Keypad built into IoAbstraction on I2C

The example is now working. A call to Wire.begin() is needed.

## matrixKeyboardMEGA34

Example of matrix keyboard support for MEGA 3 by 4 Keypad built into IoAbstraction

This is now adapted to report the state of the key.

## matrixKeyboardMEGA44

Example of matrix keyboard support for MEGA 4 by 4 Keypad built into IoAbstraction

This is now adapted to report the state of the key.
