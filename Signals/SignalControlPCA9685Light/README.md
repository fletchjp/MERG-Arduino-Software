 <img align="right" src="arduino_merg_logo.png"  width="150" height="75">

# SignalControlPCA9685Light
This Library is an extension to the SignalControl library (Sven Rosvall) to support 
LEDs connected using a PCA9685 board.

Version 0.2.0

Examples added.

To use this in your own work you need to download it from Github and install it where you keep your Arduino libraries.

John Fletcher <john@bunbury28.plus.com>

## Introduction

The SignalControl library has two ways to control an LED signal light, Fast and Slow.

Fast switches the light on or off.

Slow switches the light slowly using a timing message and an update in the Arduino loop.

The purpose of this library is to provide the same two facilities for LEDs connected to a PCA9685 board using I2C to connect to an Arduino.

The PWM provided on the PCA9685 uses 12 bit data so that the control range is 0 to 4095. This means that the internal code has to be different. 

This code is arranged so that the user supplies a value in the range 0 to 255, matching the values used in the SignalControl library.

## FastPCALight

This provides the same interface as FastLight.

## SlowPCALight

This can be used only with values provided directly to update it. The library update facility is not yet implemented.

## Examples

There are now two examples for this library. They are derived from Sven Rosvall's examples in the Signal Control library.

These examples do not need any external inputs. Instead they cycle the three lamps available in a 3 aspect signal.

The hardware required is a PCA9685 board connected using I2C with the default settings.

John Fletcher <M6777>


