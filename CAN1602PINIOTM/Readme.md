# CAN1602PINIOTM

This software is designed to work with an Arduino UNO and a DFRobot DFR0009 shield with a 1602 display and a set of buttons.

Pressing a button changes the display and also sends a CBUS event.

Incoming CBUS events with suitable event numbers also change the display. This is being tested as error messages.

This is now working and has been tested with the Arduino CAN shield developed by Martin Da Costa.

## Task management

The code is also an example of the use of task management using the IoAbstraction family of libraries.

Extending CAN1602PINIO using LiquidCrystalIO DrawingEvent.

The updating of the screen is done through the DrawingEvent so that all screen updates are done in one task..

The key updates pass through the nominated routine logKeyPressed to transfer the update. Key updates send an event when the button pressed has changed.

Input events include a demonstration using error updates which are also coordinated.

## Libraries

The following libraries are needed. They can be loaded where necessary from the Arduino IDE library management.

IoAbstraction, TaskManagerIO SimpleCollection and LiquidCrystalIO from 

https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/

Streaming Bounce2 SPI Wire EEPROM - standard Arduino libraries,

CBUS CBUS2515 CBUSLED CBUSSwitch CBUSconfig ACAN2515  - Arduino CBUS libraries from https://github.com/MERG-DEV

