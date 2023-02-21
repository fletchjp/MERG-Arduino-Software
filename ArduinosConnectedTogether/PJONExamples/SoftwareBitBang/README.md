# SoftwareBitBang

More details of investigations with SoftwareBitBang.

I now have several Arduinos mounted in Totem frames which will make it easier to experiment.

I have already changed the receiver to use pin 4 as pin 12 is not available where CBUS is using SPI on Arduino UNOs.

## BlinkWithResponse

Example with response using Bitbang.

## Send Arbitrary Data Type

This pair are a good example of sending a struct and recovering it at the other end.

## ClassMemberCallback

This example is now working between two Elegoo UNOs.

It will not work with the MiniLab as the receiver.

There are several cases like this and it seems to be the MiniLab which is the problem.

I am investigating whether there is some configuration which will sort out the problem.

It is possible that the problem is other hardware present e.g. for CAN.

## Send and receive

Two devices with similar structure including error handling.

## TaskMgrI2C20x4PJON

This example is not working properly on the Totem Minilab. This is being investigated with simpler cases.

## Send Arbitrary values

This code has this advice which I have not been following:

Avoid simultaneous transmission of Serial and SoftwareBitBang data
  
Serial.flush();


## Network Analysis

### Two Arduinos

This gives good data between two adjacent Arduino UNOs.

Here is data for the four modes. 1 and 2 seem O.K. 3 and 4 show deterioration.

---------------------
SWBB_MODE 1
Packet Overhead: 10B - Total: 570B
Bandwidth: 1710B/s
Data throughput: 1140B/s
Packets sent: 57
Fail (no acknowledge from receiver): 0
Busy (Channel is busy or affected by interference): 0
Delivery success rate: 100.00 %
---------------------

---------------------
SWBB_MODE 2
Packet Overhead: 10B - Total: 640B
Bandwidth: 1920B/s
Data throughput: 1280B/s
Packets sent: 64
Fail (no acknowledge from receiver): 0
Busy (Channel is busy or affected by interference): 0
Delivery success rate: 100.00 %
---------------------

---------------------
SWBB_MODE 3
Packet Overhead: 10B - Total: 870B
Bandwidth: 2610B/s
Data throughput: 1740B/s
Packets sent: 87
Fail (no acknowledge from receiver): 1
Busy (Channel is busy or affected by interference): 0
Delivery success rate: 98.85 %
---------------------

Fail varies between 0 and 2.

---------------------
SWBB_MODE 4
Packet Overhead: 10B - Total: 870B
Bandwidth: 2610B/s
Data throughput: 1740B/s
Packets sent: 87
Fail (no acknowledge from receiver): 7
Busy (Channel is busy or affected by interference): 0
Delivery success rate: 91.95 %
---------------------

Fail varies between 3 and 9.

### Arduino and Nano with Keypad and Encoder

---------------------
SWBB_MODE 1
Packet Overhead: 10B - Total: 570B
Bandwidth: 1710B/s
Data throughput: 1140B/s
Packets sent: 57
Fail (no acknowledge from receiver): 0
Busy (Channel is busy or affected by interference): 1
Delivery success rate: 100.00 %
---------------------

### Arduino and Minilab

No contact at all - channel is always busy. I had the wrong pin set. 

At the moment I cannot get a new version of the code to load. It does help to get the settings right.

The Minilab is different hardware and also has an SPI connection to an MCP2515.

I tried again with the correct pin setting in software with the same result.

I need to do some different tests to find out what is going on.

### RP2040BlinkTest

I cannot find anything about whether I tested these codes.

The library warns it does not run on RP2040.

WARNING: library PJON claims to run on atmelavr, avr, esp8266, esp32, sam, samd, teensy, tiny, stm32, STM32F1 architecture(s) and may be incompatible with your current board which runs on rp2040 architecture(s).

