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
