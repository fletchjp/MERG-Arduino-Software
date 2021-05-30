 <img align="right" src="arduino_cbus_logo.png"  width="150" height="75">

# DUE CBUS

This is a development from the CBUS Codes branch for work with the Arduino DUE.

## Testing

I am now starting to test the code. I have some queries which I have sent to Duncan Greenwood.

## Codes

### CANDUE

This is a working copy of CBUS_empty_SAM3X8E_v1 for some exploration and testing.

I have now added the serial commands 'r' and 'z' from CANmINnOUT to this example and changed its name and version.

Testing has gone well and I now am confident this it a platform for developments.

This and some other codes make use of DueFlashStorage, a library to emulate EEPROM. The contents are preserved between reboots but not when the code is updated.

I am adding more code to the eventhandler.

I have added more changes to bring the event code into line with CANmINnOUT code.

I have also corrected the lack of a return value from sendEvent.

I have also changer to use DEBUG_PRINT and added failure detection to processSwitches.

### DUEFCPP

This is an experimental code to look at getting FC++ working with the DUE. This is heavily modified to get rid of all use of <iostream> which is not supported. This means that all debug and traits output has been disabled. The code has a copy of the fcpp files.

### DUEMONAD

This is another experiment with FC++ using the Monad approach. It has its own copy of the fcpp files and some have been changed compared to the ones in DUEFCPP

### CBUSSAM3X8E

This is Duncan Greenwood's library for the CBUS communication. It is not under the control of this GitHub.

I have made a small change to correct the error code test as it was falsely reporting a read failure.

### CBUS_empty_SAM3X8E_v1

This is Duncan Greenwood's first example. At the moment the code assumes that there will be a button and LEDs for the Slim/Flim change.

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

