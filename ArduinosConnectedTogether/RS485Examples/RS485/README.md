# RS485 Examples

This folder has all my old examples working with RS485 on Arduinos.

There are examples which work with UNO and NANO and also some for the MEGA as well.

The UNO and NANO have only one hardware serial port, which I have usually used for the serial monitor.

## Software Serial varieties

To overcome this I have explored the use of a software serial implementation, of which there are several.

I have some documentation on the ConnectedText page Arduino RS485 (not yet ported to the new computer).

That recommends AltSoftSerial which I have made use of. There are examples here.

## AltSoftSerial

This is my preferred choice for soft serial implementation.

## Auto485

This is a library which hides some of the detail of RS485 transmit and receive for hardware serial transmission.

## AutoSoft485

I made this to provide the equivalent of Auto485 with AltSoftSerial.


## Codes

The following have now been compiled.

### TestAuto485

### AltSoftSerial

### AltSoftSerialAuto485

AutoSoft485 is not available as a library.



