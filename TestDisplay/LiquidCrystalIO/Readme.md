# LiquidCrystalIO
 
 I am collecting some examples for this library which works with TaskmanagerIO.
  
 For details of the library see https://www.thecoderscorner.com/products/arduino-libraries/liquidcrystal-io/
 
 There is mention that there are advantages in using this as the task manager can do other tasks while the display is updating.
 
 There is a limitation that only one task manager task should draw to the display.
 
 I am looking for examples which illustrates this point.
 
 ## Problem
 
 LiquidCrystalIO codes which used to work with 20 by 4 displays do so no longer and I have now found out why.
 
 The reason was a change in the way the display is configured. Something which was in the constructor now has to be in setup().
 
 Calls have to be made to setup the backlight properly. This will be needed in all code using I2C displays.
 
 I have now updated all the codes here which needed the update.
 
 The hardware does work with the hd44780 library instead so it is not the hardware.
 
 ## TaskMgrIntegration 
 
 This is one which shows how to build the screen updating into a task.
 
 ## Counter23017 
 
 This shows how to use the task manager to schedule updating of the screen.
 
 The library code is at https://github.com/davetcc/LiquidCrystalIO on Github.
 
 ## TaskMgrI2C20x4 
 
 This shows how to build in tasks.
 
 ## MEGATaskIntegration 
 
 This will add the display to code on a MEGA which has a keypad and two encoders as well.
 
 I have fitted it with a DFRobot DFR0265 shield which provides more ground and 5v pins.
 
 This is now mounted in the red Totem frame with the display mounted as well.
 
 Display now tested.
 
 ## MEGATaskIntegration I2C
 
 I have added code to exchange information with another Arduino.
 
 This works with the PeripheralTwoWay example code.
 
 I now have it working with two way transfer using the timout codes.
 
 ## MEGATaskIntegration I2C Data

 I want to extend the previous example to send some of the data available.
 
 It now sends the time value to the peripheral.
 
 This is a challenge as it involves converting the float value to chars.
 
 ## MEGATaskIntegration I2C Struct
 
 ## MEGATaskIntegration I2C PJON
 
