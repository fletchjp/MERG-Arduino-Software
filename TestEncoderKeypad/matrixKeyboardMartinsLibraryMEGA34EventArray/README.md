@mainpage Keypad Example for MEGA with 3 by 4 Keypad and 2 of Martin's encoder library with Event code

## matrixKeyboardMartinsEncoderMEGA34EventArray

An Arduino sketch which uses a IoAbstraction libraries to control a keypad adding Martin's encoder library.

This now handles two encoders which are put into an array.

It has one interrupt routine and works out which one has been turned.
 
 This example shows how to use the matrix keyboard support that's built into IoAbstraction,
 it can be used out the box with either a 3x4 or 4x4 keypad, but you can modify it to use
 any matrix keyboard quite easily.
 It just sends the characters that are typed on the keyboard to Serial. The keyboard in This
 example is connected directly to Arduino pins, but could just as easily be connected over
 a PCF8574, MCP23017 or other IoAbstraction.
 This version sets up the keyboard adapting the custom_keyboard example.
 
## Development

This is being developed with code for one or more encoders, for work with CANCMDDC.

This now uses the IoAbstraction Event code for the encoders.

The encoders are put into an array to suit the interface in CANCMDDC.

The events cannot be in the array as the Event class instances cannot be copied.

Some of the other code has to be for one instance, so this is not a fully scalable example.

Two encoders and a 3 by 4 keypad are now operational.

