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

I have made a small change to my copy of Auto485 to remove an Arduino warning message.

## AutoSoftSerial485

I made this to provide the equivalent of Auto485 with AltSoftSerial. I am now not sure this is good code.

## Codes

The following have now been compiled.

### TestAuto485

### AltSoftSerial

### AltSoftSerialAuto485

AutoSoft485 is not available as a library although I could make it a library.

I am now working to improve this code. Note that each code has its own copy at the moment.

### AltSoftSerialUno2

This is the code where I have modified AutoSoft485

### AutoSoftSerialUno2

### AutoSoftSerialUno2a

This has more structure.

### MegaSerial485comms1a

Basic example receiving information.

### TestAuto485Mega1a

Version of the previous code using Auto485.

### KISnet examples

I have some of these from John Purbrick which I have not included here as they work completely differently.
