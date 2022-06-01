# LiquidCrystalIO
 
## MEGATaskIntegrationI2C

I have worked on this to have good integration of the display with the other tasks.

This is using task management from IoAbstraction and TaskManagerIO library family.

This includes both KeyboardManager and LiquidCrystalIO which take care of a lot of the housekeeping.

This version is adding the I2C connection needed to work with PJON.

The combination is not working.

I thought this was because I had used different pins from the Arduino MEGA for the I2C connection.

I now think that this was not the case as the pins are connected together on the MEGA and are valid alternatives.

Instead I think this is a question of software. I cannot do both at the same time without risk of locking.

I have to look carefully at the software options.

I am finding that I have to link the two I2C connections from the same pins on the controller.

The display is using I2C with the Wire library.

The essential feature is to have an event called DrawingEvent which is the only event to draw to the display after the initial setup.

All the other tasks which want to display communicate with the display by calling a member function to pass the value to be displayed.

Sometimes the different displays interact and I have scheduled a task to redraw the display every 5 seconds.

It is important to keep close control of the updating of the display which is now done only in one routine. This has reduced the previous corruption of the display.

I have now sorted out the keypad output so that a sequence of keys is shown until # is pressed which clears the line and resets for a new line.

I have extend this to store the characters entered so that the string can be sent using the CBUS Long Message code.

I have used a star as a delete character and # to indicate send.

This has now been implemented in the display part of the code.

I have now fitted the unit with an MCP2515 to connect it to CBUS. That will make it a different code.

This code is being developed for the Leamington and Warwick Model Railway Club project, though I will use different hardware for that.