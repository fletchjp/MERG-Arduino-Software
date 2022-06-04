# Arduinos Connected together using I2C

These are a set of programs being used to test connecting two Arduinos together

## I2C connection

There are a set of codes for this on this page:

https://docs.arduino.cc/learn/communication/wire

### Controller Reader

Sketch to read data from a peripheral using I2C and the Wire library.

### Controller Reader Timeout

Sketch to read data from a peripheral using I2C and the Wire library.

Extended to use timeouts to check the status of the peripheral.

### Controller Reader Error Codes

Sketch to read data from a peripheral using I2C and the Wire library.

This uses a modified version of requestFrom which reports errors from endTransmission.

### Controller Reader Error Codes Timeout

Sketch to read data from a peripheral using I2C and the Wire library.

This uses a modified version of requestFrom which reports errors from endTransmission.

Extended to use timeouts to check the status of the peripheral.

### Peripheral Sender

Sketch to send data from a peripheral using I2C and the Wire library.

### Controller Writer

Sketch to send data to a peripheral using I2C and the Wire library.

### Peripheral Receiver

Sketch to receive data sent to a peripheral using I2C and the Wire library.

## Experiments

I have had the second pair of sketches working between two UNOs.

I now have the pair below working to show data collection.

### Controller Reader Keypad

Sketch to read data from a peripheral using I2C and the Wire library to send keypad data.

### Peripheral Sender Keypad

Sketch to send data from a peripheral using I2C and the Wire library to send keypad data.

## Two Way Example

This is a new set of codes to explore two way communication using I2C between two Arduinos.

I have example code from /// Adapted using information from this page

https://www.instructables.com/Aduino-IC-2-Way-Communication/

which may have some problems.

There is now a working example of two way communication.
