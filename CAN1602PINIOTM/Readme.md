# CAN1602PINIOTM

This is now working and has been tested with the Arduino CAN shield developed by Martin Da Costa.

Extending CAN1602PINIO using LiquidCrystalIO DrawingEvent.

The updating of the screen is done through the DrawingEvent so that all screen updates are done in one task..

This key updates pass through the nominated routine logKeyPressed to transfer the update. Key updates send an event when the button pressed has changed.

Input events include a demonstration using error updates which are also coordinated.

## Libraries

The following libraries are needed: 

IoAbstraction, TaskManagerIO SimpleCollection and LiquidCrystalIO from 

https://www.thecoderscorner.com/products/arduino-libraries/io-abstraction/

Streaming Bounce2 SPI - standard Arduino libraries,

CBUS CBUS2515 CBUSLED CBUSSwitch CBUSconfig ACAN2515  - Arduino CBUS libraries from https://github.com/MERG-DEV

