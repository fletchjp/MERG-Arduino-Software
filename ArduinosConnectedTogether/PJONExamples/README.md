# Arduinos connected together using PJON

This contains some examples collected to show the use of PJON.

## What is PJON?

See https://www.pjon.org/ 

It is a networking protocol which can run over a variety of systems.

# Any

Examples with multiple connections.

# ThroughSerial 

## Serial ports directly connected

## BlinkTest

Basic examples using the Serial port.

I have a set using SoftSerial for a second serial port for the Arduino UNO.

## BlinkWithResponse

Example with response using the Serial port.

I have a set using SoftSerial for a second serial port for the Arduino UNO.

## Keypad

The BlinkTest example has been extended to pass the key press on the keypad to the receiving Arduino.

When connected over the serial line the two wires have to be crossed over.

## RS485 over Serial

These are moved to the RS485examples folder.

## RS485-Blink

Not yet tried out. I have started to build examples using SoftSerial.

## RS485-BlinkWithResponse

Not yet tried out.

# SoftwareBitBang

An alternative connection to ThroughSerial using only one wire.

I am extending the examples.

## BlinkWithResponse

Example with response using Bitbang.

## ClassMemberCallback

This example is now working between two Elegoo UNOs.

It will not work with the MiniLab as the receiver.

There are several cases like this and it seems to be the MiniLab which is the problem.

I am investigating whether there is some configuration which will sort out the problem.

It is possible that the problem is other hardware present e.g. for CAN.

## Sensor Examples

These use SoftwareBitBang
 
## Local Chat Examples

These use SoftwareBitBang to implement chat between two Arduinos.

These are adapted from a wireless example in the PJON library.

There are two examples, with and without task management using TaskManagerIO to show how this can be used.

