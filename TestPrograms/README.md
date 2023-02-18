# Test Programs

These are a set of programs being used to test some C++ programming ideas.

A lot of this is in response to the realisation that the Arduino IDE default settings hide warnings.

Each program has been started to cover a topic, although they have expanded as time has gone on.

## File Heading

Code based on a suggestion of Alan Lomax.

The idea is that if you put code like this at the start of your code then if you run an Arduino this output to the serial monitor will give information on the code compiled, the version and when it was compiled.

## i2c_address_detect

Detect I2C devices 0n 0x10. Adafruit

## i2c_scanner

Scans for I2C devices connected using the usual Wire library

## i2c_scanner_DSS

Scans for I2C devices connected using the I2C library from DSS.

https://github.com/DSSCircuits/I2C-Master-Library

This is a test of this library which is about 10 years old.

The interface is different from the one used by Wire.

## i2c_scanner_DUE

Version of 2c_scanner configured for Wire1 for use with a DUE.

## TestWarnings (-fpermissive)

On the AVR boards (NANO, UNO and MEGA for example) the Arduino IDE default settings for the compiler include -fpermissive.

This converts a number of errors to warnings, which are then hidden.

There seems to be no list of what -fpermissive actually allows so it is necessary to hunt around to find some answers.

I have changed this to use a do { } while structure.

## TestSwitch (switch statements)

One of the problem areas with -fpermissive has been around switch statements.

In particular, local variable definitions within a switch are a source of errors which are being masked.

## TestModern (scoped enums and other things)

The use of enum is something which C++ inherited from C.

Starting from C++11 there is a facility called a scoped enum or enum class which tightens up on the way an enum works.

## TestBetterEnum (more enum examples)

## MultipleSerialTest and DUESerialTest

Two programs for investigation of the Arduino DUE multiple serial ports.
