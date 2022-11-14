 <img align="right" src="arduino_cbus_logo.png"  width="150" height="75">

# DUE CBUS

This is a development from the CBUS Codes branch for work with the Arduino DUE.

Now that I have two DUEs working with the board with 2 CAN interfaces, I am looking at more of the example software which is available. I also have a third DUE which I will use for other program work.

Some of the examples will now work with DUE and AVR (MEGA or UNO).

I would like to start to add CBUS long message handling. This will depend on the release of a new version of the CBUSSAM3X8E library to support the extra functions.

## Testing

I am now starting to test the code. I have some queries which I have sent to Duncan Greenwood.

I now have a second DUE which now has a two CAN interface.

I have been using this for testing non CBUS code as the units lose CBUS configuration when software is reloaded. I will do this now on a third DUE.

See below at CANDUE for the testing of CBUS Long Message handling.

## External EEPROM

One of the problems with using the DUEs for CBUS modules is that the internal EEPROM is a simulation. It only persists until the program script is updated.

This means that any CBUS configuration is then lost and has to be redone using FCU. This is a pain while developing and testing code.

One way around this problem is to use an external EEPROM which will retain information. These are available using either I2C or SPI to communicate.

I am experimenting with an I2C based external EEPROM using AT24C256 chips which hold 256k bits (32k bytes). This is more than enough for configuration data saved by the Arduino CBUS Library.

The CAN interfaces from SK Pang which I am using use the pins for the main (Wire) I2C interface on the DUE so I am using the alternative (Wire1) interface.

There is a specific library for the AT24C256 which I have used and the CBUSconfig library (part of the Arduino CBUS Library) has also been adapted. 

Testing with the Arduino CBUS Library is in progress. 

## Codes

### CBUS library

This is not technically DUE code. I have made a copy here of the working copy I am using to develop the Long Message Multiple Listening code which is currently not working. The current version is worse than the one before and unfortunately I did not make a backup from the working location in C:\Arduino\libraries. I will now back up here as I make changes.

I have updated with a new version to try to sort out the problems.

### CANDUE

This is a working copy of CBUS_empty_SAM3X8E_v1 for some exploration and testing. This uses the library CBUSSAM3X8E from Duncan Greenwood.

I have now added the serial commands 'r' and 'z' from CANmINnOUT to this example and changed its name and version.

Testing has gone well and I now am confident this it a platform for developments.

This and some other codes make use of DueFlashStorage, a library to emulate EEPROM. The contents are preserved between reboots but not when the code is updated.

I am adding more code to the eventhandler.

I have added more changes to bring the event code into line with CANmINnOUT code.

I have also corrected the lack of a return value from sendEvent.

I have also changer to use DEBUG_PRINT and added failure detection to processSwitches.

I have documented the DUE pin map which is in a lot of ways similar to the Arduino MEGA although some pins have different uses.

I have been extending this code to work with CBUS long messages.

I have added code for use with a 20 by 4 LCD.

This code is now modified to run with the new version of the Arduino CBUS libraries.

I have a problem with the long message code which showed itself when I sent two messages at the same time to the CAN_DUE code and it did not cope. I now have an ammended version of the code which should copy, by rejecting any message which comes from a different stream while a message is being processed. I am about to test this. The modified code is for DUE only as there may not be memory for it on a UNO.

I have now tested a new version from Duncan Greenwood which deals with all the problems about receiving multiple messages.

### CANDUE3

A development of CANDUE

### CANDUE3EEPROM

A version of CANDUE3 using external EEPROM

### DUE_arduino_state_machine

Example for the StateMachine library modified to work with the DUE.

   // Only works on the DUE with the return type added ->bool
   S0->addTransition( []()->bool{ //convert to (bool (*)())

### DUEFCPP

This is an experimental code to look at getting FC++ working with the DUE. This is heavily modified to get rid of all use of <iostream> which is not supported. This means that all debug and traits output has been disabled. The code has a copy of the fcpp files.

### BOTHFCPP

This is a version of DUEFCPP which will work with DUE or MEGA. This has ArduinoSTL instead of Streaming.

### DUEMONAD

This is another experiment with FC++ using the Monad approach. It has its own copy of the fcpp files and some have been changed compared to the ones in DUEFCPP

### BOTHMONAD

This is a version of DUEMONAD which will work with DUE or MEGA. This has ArduinoSTL instead of Streaming.

A change to the EitherM monad to add join has now been ported to the fcpp library.

### CBUSSAM3X8E

This is Duncan Greenwood's library for the CBUS communication. It is not under the control of this GitHub.

I have made a small change to correct the error code test as it was falsely reporting a read failure.

### CBUS_empty_SAM3X8E_v1

This is Duncan Greenwood's first example. At the moment the code assumes that there will be a button and LEDs for the Slim/Flim change.

## uCOS-II

This is a task scheduling library which I have installed which will run on the DUE. It is more advanced than the ones I am using on the Arduino. I have only a simple led_blink example at the moment which I have run.

### led_blink

I have copied this uCOS-II example to begin doing some things.

## FC++ codes

Now that I have ArduinoSTL to provide the C++ std library I can start to use the FC++ codes on both AVR and DUE. I have put the files here for now.

This means that I now have two different versions of the FC++ codes. This one can do debug output.

The previous one used only Serial and did not support debug output.

At the moment I have to copy the version of fcpp which I need into each example. I would like to end up with fcpp as a common code which can be used by all the examples. I think the one step towards that is to get rid of the reduced version which I have been using on the DUE examples up to now. First I need to check the new version on a DUE. Yes, the ecoop1b example now runs on both.

I have a problem with two functoids, and_ and or_ which do not compile on the DUE for either version. I do not understand this at the moment. I have taken them out of use.

I have now managed to turn FC++ into an Arduino library called fcpp. All that is needed to use it is to include "prelude.h" after defining any options needed.

I also now have a copy of Boost 1.51.0 which I am using with FC++. This has enabled me to run more of the examples which rely on boost/mpl to do some type deduction. When using this the main code should define FCPP_ARDUINO_BOOST to ensure that boost headers are obtained as needed.

### ecoop1b

This is a first test file to get things sorted out. It is now working with both versions of fcpp.

### laws

This example explores the implementation of Functor laws using FC++.

### observer

This is a development of the ecoop1b example to look at monitoring changes using the subject/observer model.

I have quite a lot of code for this which I am going to adapt.

### DUEobserver

This continues the adaption of the observer example.

### DUEobserver2

Further development of DUEobserver.

## SK Pang codes

I have downloaded several of the example codes which come with the two channel CAN shield. These use the same due_can library used by Duncan's library CBUSSAM3X8E. 
### due_can

There are quite a lot of useful examples with due_can. It uses the  can_common library.

## CBUS Codes

This is a branch for the location of CBUS codes, some of which will be related to the codes in the main branch.

I will not modify the other Markdown files in the branch, keeping information in this and other new files.

These codes are part of development of modules for control of DC layouts using DC controllers for each track section.

## Codes

### CANASIGNALI2C_CS15

This is derived from work using I2C to provide a PWM driven signal controlled by an IR detector. I have modified this as a first test of the use of Pin 15 instead of Pin 10 for the chip select function needed for SCI in order to run the Arduino CBUS library. So far I have only tested it as an isolated module and not yet attempted to connect it to anything else.

The reason for this experiment is that Pin 10 is not available for use with the 1602 display with buttons when used as a shield rather than an I2C item.

### CANALCDSIGNALI2C_CS15

This is the first code built to see if the display can work with the Arduino CBUS library. I have built this on the signal code as it easier to add things to a code than build one completely from scratch.

I think that the I2C for the signals can be linked from A4 and A5 which are available on the DFRobot board.

I also moved the IR and Signal pins to A2 and A3 as they were on pins which are used by the display.

### CANALCDNOSIGNALI2C_CS15

This is a version with an option to disable the signal code for testing without a signal and detection.

### Another Idea (yet to be named)

I am also developing a version where the display is connected using an I2C backpack for all the connections except the A0 and reset.

## Other DUE codes

I am finding some more libraries which support multiple tasks. There is one called scheduler which is part of the official Arduino set although not for AVR boards so DUE only as far as I am concerned. It allows for multiple loops. 

### MultipleBlinks

This is an example of the Scheduler library for a DUE.
