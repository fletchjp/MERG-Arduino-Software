# DFRobot DFR0219 example codes

These are codes for the DFR0219 Arduino shields which have now arrived.

One of the problems is that these shields use the same Serial port which is used for uploading Arduino code. There is a switch which controls programming or running.

This means that the Serial Monitor is not available during code running. I would usually use this for monitoring code operation.

There are a couple of ways around this. One is to have another RS485 device which is using either SoftSerial or Serial1 on a MEGA so that the Serial Monitor can be seen there.

Another way is to add another Arduino on an I2C link to receive output and send it to the Serial Monitor.

There is an example which can be used for this - I2CExamples/PeripheralReceiver

I have now built a number of new examples adding Writer to the name to indicate writing to I2C which I have made a compile time option in the codes.

All of the RS485 examples now have the flashing of the LED pin added, which was previously only in the ones receiving data.

## Controller Writer

Copied here to be used as needed.

## PeripheralReceiver

Copied here to be adapted.

## RS485TransmitingData

I have now had this working to the Serial Monitor.

## RS485ReceivingData

This example flashes an LED when data is received.

These are examples from the shield description. Each only does one way transmission.

I now have these working. The red LED is flashing on both and the serial monitor on the transmitter is also getting the data. 

The flashing of the two LEDs differs slightly in timing and they drift apart.

I have now looked at using Auto485 as well.

## RS485AutoData

First try with Auto485 in use.

I have made a small change to my copy of Auto485 to remove an Arduino warning message.

I have modified to have separate TRASMIT and receive versions depending on #define TRANSMIT.

## RS485AutoDataTM

This has been developed using TaskManagerIO. Not yet tested.

This also has separate TRANSMIT and receive versions depending on #define TRANSMIT.

## RS485AutoDataWriter

Adding I2C output of what is transmitted and received (incomplete)

## RS485TransmitingDataWriter

Adding I2C output of what is transmitted.

## RS485ReceivingDataWriter

Adding I2C output of what is received.

## RS485TransmitDataTM

Adding task management using TaskManagerIO with a callback function which alternately transmits (turning the LED on) and turns the LED off.

This is a minature state machine.

## RS485ReceivingDataTM

Adding task management using TaskManagerIO with callback functions.

The TM programs have now been tested and give a much better timing than the ones without TM.

## RS485TransmitDataTMC

Adding task management using TaskManagerIO with an event class (incomplete)

## RS485ReceivingDataTMC

Adding task management using TaskManagerIO with an event class (incomplete)

## RS485TransmitDataFRT

Version using the FRT library and FreeRTOS instead of TaskManagerIO. Transmit is working

## RS485ReceivingDataFRT

Version using the FRT library and FreeRTOS instead of TaskManagerIO. 

Receiving is failing to work unless the receipt is done in the loop().

I swapped priority so that SwitchTask is 2 and ReceiveTask is 1. This works for three or four cycles and then goes wrong.
