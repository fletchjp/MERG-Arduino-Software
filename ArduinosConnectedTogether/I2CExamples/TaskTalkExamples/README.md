# Arduinos Connected together using I2C 

## Two Way Examples for the talk on tasks 

These are codes which will be used for the talk on Arduino Task integration.

### Integration with TaskManagerIO

This pair of codes can transfer messages in both directions using the Wire library.

### ControllerTwoWay

This sends and receives data from code called withing the loop() and controlled with a delay()

### PeripheralTwoWay

This receives and sends data using callbacks registered with Wire library functions.

### ControllerTwoWayTask

This now works for the Controller to do send information in the context of TaskManagerIO.

This uses a simple call back to a routine containing the previous transmission code.

There is no need for a delay in loop() as a TaskManangerIO routine controls the frequency of transmission.

### ControllerTwoWayTaskClass

This uses an executable class to run the transmission code. This can be reused to send at different rates to different peripherals.

It could also be adapted to send different messages.

### ControllerTwoWayTimout

Adapted from code in this location to explore use of timeout.

https://www.arduino.cc/reference/en/language/functions/communication/wire/setwiretimeout/

