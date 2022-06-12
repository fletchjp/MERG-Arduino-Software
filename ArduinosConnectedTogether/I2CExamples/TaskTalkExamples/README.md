# Arduinos Connected together using I2C 

## Two Way Examples for the talk on tasks 

These are codes which will be used for the talk on Arduino Task integration.

### Integration with TaskManagerIO

### ControllerTwoWay

### PeripheralTwoWay

This pair of codes can transfer messages in both directions.

### ControllerTwoWayTask

This now works for the Controller to do send information in the context of TaskManagerIO.

This uses a simple call back to a routine containing the previous transmission code.

### ControllerTwoWayTaskClass

This uses an executable class to run the transmission code. This can be reused to send at different rates to different peripherals.

It could also be adapted to send different messages.

### ControllerTwoWayTimout

Adapted from code in this location to explore use of timeout.

https://www.arduino.cc/reference/en/language/functions/communication/wire/setwiretimeout/

