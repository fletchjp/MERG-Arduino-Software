# LiquidCrystalIO
 
## MEGATaskIntegrationPJON
 
I have worked on this to have good integration of the display with the other tasks.

This is using task management from IoAbstraction and TaskManagerIO library family.

This includes both KeyboardManager and LiquidCrystalIO which take care of a lot of the housekeeping.

The display is using I2C with the Wire library.

The essential feature is to have an event called DrawingEvent which is the only event to draw to the display after the initial setup.

All the other tasks which want to display communicate with the display by calling a member function to pass the value to be displayed.

Sometimes the different displays interact and I have scheduled a task to redraw the display every 5 seconds.

I have now sorted out the keypad output so that a sequence of keys is shown until # is pressed which clears the line and resets for a new line.

I have now fitted the unit with an MCP2515 to connect it to CBUS. That will make it a different code.

I am exploring what happens with PJON. I think there is a strong interaction with the TaskManagerIO routines.

I have put the bus.receive() into a task and run it every 5 milliseconds.

---------------------
Packet Overhead: 10B - Total: 270B
Bandwidth: 810B/s
Data throughput: 540B/s
Packets sent: 27
Fail (no acknowledge from receiver): 31
Busy (Channel is busy or affected by interference): 0
Delivery success rate: -14.81 %
---------------------

This is the best response I can get with bus.receive(5000).

I think this means that nearly all the time is being taken by the receive function.

I am thinking that I need to offload SoftwareBitBang to a client Arduino using I2C. 

I would need to chain the I2C as it is already used for the display.
