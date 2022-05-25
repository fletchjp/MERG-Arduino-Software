# LiquidCrystalIO
 
## MEGATaskIntegration 
 
I have worked on this to have good integration of the display with the other tasks.

This is using task management from IoAbstraction and TaskManagerIO library family.

This includes both KeyboardManager and LiquidCrystalIO which take care of a lot of the housekeeping.

The display is using I2C with the Wire library.

The essential feature is to have an event called DrawingEvent which is the only event to draw to the display after the initial setup.

All the other tasks which want to display communicate with the display by calling a member function to pass the value to be displayed.

Sometimes the different displays interact and I have scheduled a task to redraw the display every 5 seconds.

I have now sorted out the keypad output so that a sequence of keys is shown until # is pressed which clears the line and resets for a new line.
