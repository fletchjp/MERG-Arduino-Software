# CAN1602PINIOTM

This is now working and has been tested with the Arduino CAN shield developed by Martin Da Costa.

Extending CAN1602PINIO using LiquidCrystalIO DrawingEvent.

The updating of the screen is done through the DrawingEvent so that all screen updates are done in one task..

This key updates pass through the nominated routine logKeyPressed to transfer the update.

There is error updates are also coordinated.

## Libraries

The following libraries are needed: 

IoAbstraction, TaskManagerIO SimpleCollection and LiquidCrystalIO from 

Streaming Bounce2 SPI

CBUS CBUS2515 CBUSLED CBUSSwitch CBUSconfig ACAN2515

