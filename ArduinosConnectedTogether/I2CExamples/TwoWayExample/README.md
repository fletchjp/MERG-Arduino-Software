# Arduinos Connected together using I2C

## Two Way Example

This is a new set of codes to explore two way communication using I2C between two Arduinos.

I have example code from /// Adapted using information from this page

https://www.instructables.com/Aduino-IC-2-Way-Communication/

which may have some problems.

I have put together a first version of this code.

I now have initial ideas working.

I now have the code working to send information in both directions.

### Integration with TaskManagerIO

I would like the controller code to be integrated with TaskManagerIO so that I can use it with other codes.

As usual there is some information on the pages for the libraries without a specific example.

### Why develop this code?

I would like to use PJON to communicate around the network.

Unfortunately it is not possible to use this with code running TaskManagerIO.

I want to have I2C code running in TaskManagerIO which will exchange information with a peripheral Arduino running both I2C and PJON.

The first stage for that is to have two way communication using I2C.

All the previous examples only work in one direction.

### ControllerTwoWay

### PeripheralTwoWay

This pair of codes can transfer messages in both directions.

### ControllerTwoWayTask

This now works for the Controller to do send information in the context of TaskManagerIO.

This uses a simple call back to a routine containing the previous transmission code.

### ControllerTwoWayTaskClass

This uses an executable class to run the transmission code. This can be reused to send at different rates to different peripherals.

It could also be adapted to send different messages.

### PeripheralTwoWayPJON

The peripheral workS in the context of PJON. I have removed the delay from loop().

I have still to test how it works with PJON.

