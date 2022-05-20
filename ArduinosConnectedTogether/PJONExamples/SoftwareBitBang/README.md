# SoftwareBitBang

More details of investigations with SoftwareBitBang.

I now have several Arduinos mounted in Totem frames which will make it easier to experiment.

I have already changed the receiver to use pin 4 as pin 12 is not available where CBUS is using SPI on Arduino UNOs.

## BlinkWithResponse

Example with response using Bitbang.

## ClassMemberCallback

This example is now working between two Elegoo UNOs.

It will not work with the MiniLab as the receiver.

There are several cases like this and it seems to be the MiniLab which is the problem.

I am investigating whether there is some configuration which will sort out the problem.

It is possible that the problem is other hardware present e.g. for CAN.

## TaskMgrI2C20x4PJON

This example is not working properly on the Totem Minilab. This is being investigated with simpler cases.

## Network Analysis

This gives good data between two adjacent Arduino UNOs.

---------------------
Packet Overhead: 10B - Total: 570B
Bandwidth: 1710B/s
Data throughput: 1140B/s
Packets sent: 57
Fail (no acknowledge from receiver): 0
Busy (Channel is busy or affected by interference): 0
Delivery success rate: 100.00 %
---------------------
