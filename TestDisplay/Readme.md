# MERG-Arduino-Software

 ## Test Display
 
 This is a collection of test programs for Arduino display options.
 
 Some codes are getting too big for the memory on an Arduino UNO and for those I am using MEGA 1280 or MEGA 2560 instead.
 
 ### LiquidCrystalIO
 
 The supplied programs are for a 16 by 2 display although I have some codes running for 20 by 4 displays.
 
 I am collecting some examples for this library which works with TaskmanagerIO.
 
 TaskMgrIntegration is one which shows how to build the screen updating into a task.
 
 Counter23017 shows how to use the task manager to schedule updating of the screen.
 
 The code is integrated with the IOAbstraction and TaskmanagerIO libraries.
 
 I am extending the examples to include a 20 by 4 display, starting with HelloI2C20x4 to sort out the basics.
 
 I am now having a big problem with LiquidCrystalIO not working with a 20 by 4 display. Code which worked before no longer works.
 
 I have reported the problem as one with the library as the hardware works with another library.
 
 I have now also reported the solution which is to put configuration for the backlight into setup(). This was previously done in the constructor.

 #### TaskMgrI2C20x4
 
 This is new code to implement task management in the style of TaskMgrIntegration with a different problem.
 
 It demonstrates updating regularly plus one off events. All updating is done in one task.
  
### TFT Display

Codes for Arduino use of TFT displays. At the moment only some basic tests.

