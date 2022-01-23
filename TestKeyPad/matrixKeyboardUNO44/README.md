@mainpage Keypad Example for UNO with 4 by 4 Keypad

## matrixKeyboardUNO44

An Arduino sketch which uses a IoAbstraction libraries to control a keypad.
 
 This example shows how to use the matrix keyboard support that's built into IoAbstraction,
 it can be used out the box with either a 3x4 or 4x4 keypad, but you can modify it to use
 any matrix keyboard quite easily.
 It just sends the characters that are typed on the keyboard to Serial. The keyboard in This
 example is connected directly to Arduino pins, but could just as easily be connected over
 a PCF8574, MCP23017 or other IoAbstraction.
 This version sets up the keyboard adapting the custom_keyboard example.
 
