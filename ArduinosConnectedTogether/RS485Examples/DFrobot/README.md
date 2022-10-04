# DFRobot DFR0219 example codes

These are codes for the DFR0219 Arduino shields which have now arrived.

One of the problems is that these shields use the same Serial port which is used for uploading Arduino code. There is a switch which controls programming or running.

This means that the Serial Monitor is not available during code running. I would usually use this for monitoring code operation.

There are a couple of ways around this. One is to have another RS485 device which is using either SoftSerial or Serial1 on a MEGA so that the Serial Monitor can be seen there.

Another way is to add another Arduino on an I2C link to receive output and send it to the Serial Monitor.

There is an example which can be used for this - I2CExamples/PeripheralReceiver

I have now built a number of new examples adding Writer to the name to indicate writing to I2C which I have made a compile time option in the codes.

## Controller Writer

Copied here to be used as needed.

## PeripheralReceiver

Copied here to be adapted.

## RS485TransmitingData

I have now had this working to the Serial Monitor.

## RS485ReceivingData

This example flashes an LED when data is received.

These are examples from the shield description. Each only does one way transmission.

When I have these working I will look at using Auto485 as well.

## RS485AutoData

First try with Auto485 in use.

I have made a small change to my copy of Auto485 to remove an Arduino warning message.

## RS485AutoDataWriter

Adding I2C output of what is transmitted and received (incomplete)

## RS485TransmitingDataWriter

Adding I2C output of what is transmitted.

## RS485ReceivingDataWriter

Adding I2C output of what is received.

