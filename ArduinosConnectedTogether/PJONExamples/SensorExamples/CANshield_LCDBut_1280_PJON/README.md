<img align="right" src="arduino_cbus_logo.png"  width="150" height="75">

# CANshield_LCDBut_1280_PJON

This is an example of an application of the Arduino CAN Shield (MERG Kit Locker #110).

The example code runs on an Arduino MEGA 1280 with a CAN shield and a DFRobot shield with a 2 by 16 LCD display and buttons.

This examples is fairly complex code as it has to support both the reading of the input buttons and the send information to the LCD display. 

https://www.dfrobot.com/product-51.html (DFR0009)

This now has added code for experiments with PJON. The display can now show temperature and humidity data transferred from another Arduino.

## Please note

The CANshield button and LEDs are not available for CBUS configuration

This is because the pins for the LEDs and buttons are also used by the display, quite apart from them being hidden behind the display.

All use of the button and LEDs has to be removed in the code as otherwise the display does not function correctly.

## More information

An Arduino program using the Arduino CAN Shield (MERG Kit Locker #110).
Information about the kit can be found at https://www.merg.org.uk/merg_wiki/doku.php?id=kits:110

The for this application it has to be built as a shield to fit onto an Arduino UNO.

This sketch provides the code which will enable users of the Arduino CAN Shield to test functionality of the shield.
Sketch is the Arduino name for the program code which is run on an Arduino.

Key Features:
- MERG CBUS interface.
- This can be configured to a CBUS network using the FLiM Configuration Utility (FCU) which only runs on Windows computers.
- This can be done using the Serial Monitor as described below.
- The puish button method is not available for this code.

## Overview

The program is written in C++ but you do not need to understand this to use the program.

## Loading the code and libraries

In order to load the code it is necessary to load the Arduino IDE (Interactive Development Environment) onto a computer.

There are currently two IDE versions, the older 1.8.19 and the newer 2.2.1. Either will do for this.
The IDE is available for different versions of Windows and also for other systems.

The IDE will manage both the code and also several libraries which can be loaded using a Library Manager.

All of the libraries needed for CANshield are available through the Library Manager which also prompts with updates for installed libraries.
The user installs libraries into a fixed location which has all of the libraries which have been loaded, available for any sketch.

Whenever a sketch is to be used the Arduino IDE calls a C++ compiler which compiles the code and a copy of each of the libraries needed. This can be done as a check that it allo works and then loaded onto the Arduino using the connection.

On a Windows computer the USB connection to the computer will be via a COM port and it is necessary to tell the Arduino IDE which COM port to use.

## Using CANshield

The MCP2515 interface requires five Arduino pins to be allocated. Three of these are fixed
in the architecture of the Arduino processor. One pin must be connected to an interrupt
capable Arduino pin. On the shield, this is set to pin 2 but a jumper can be removed and a
wire link used to connect to another interrupt pin if desired.  This may be appropriate
when using a Mega with additional interrupt inputs.

The Chip Select pin on the shield is connected to pin 10.  Once again, by removing the relevant
jumper, another pin can be wire linked for this function.

If you change the interrupt or chip select pins, make sure that you update the relevant pin 
allocations in the sketch.

If the MERG Kit 110 CAN Shield is used, the following pins are connected by default:

Pin | Description
--- | ---
Digital pin 2 | Interupt CAN
Digital pin 10| (SS)    CS    CAN
Digital pin 11| (MOSI)  SI    CAN
Digital pin 12| (MISO)  SO    CAN
Digital pin 13| (SCK)   Sck   CAN

Using the CAN Shield, the following pins are used for CBUS Initialisation:

NOTE: For this application the pin 10 is taken by the display shield and CS has to be made 15 instead.

**It is the users responsibility that the total current that the Arduino is asked to supply 
stays within the capacity of the on board regulator.  Failure to do this will result in 
terminal damage to your Arduino.**

Pins defined as inputs are active low.  That is to say that they are pulled up by an 
internal resistor. The input switch should connect the pin to 0 Volts.

Pins defined as outputs are active high.  They will source current to (say) an LED. It is 
important that a suitable current limiting resistor is fitted between the pin and the LED 
anode.  The LED cathode should be connected to ground.

### Library Dependencies

The following third party libraries are required for the basic CANshield code:

Library | Purpose
---------------|-----------------
Streaming  |*C++ stream style output, v5, (http://arduiniana.org/libraries/streaming/)*
Bounce2    |*Debounce of switch inputs*
ACAN2515   |*library to support the MCP2515/25625 CAN controller IC*
CBUS2515   |*CAN controller and CBUS class*
CBUSconfig |*module configuration*
CBUS       |*CBUS Class*
CBUSSwitch   |*Manage CBUS switch*
CBUSLED      |*Manage CBUS indicator yellow/green LEDs*

## The Serial Monitor

If your Arduino is connected to the Arduino IDE on your computer via the USB port, you can
access information by sending a character from the IDE.  The function of these characters
is as follows:

### 'r'
This character will cause the module to renegotiate its CBUS status by requesting a node number.
The FCU will respond as it would for any other unrecognised module.

### 'z'
This character needs to be sent twice within 2 seconds so that its action is confirmed.
This will reset the module and clear the EEPROM.  It should thus be used with care.

Other information is available using the serial monitor using other commands:

### 'n'
This character will return the node configuration.

### 'e'
This character will return the learned event table in the EEPROM.

### 'v'
This character will return the node variables.

### 'c'
This character will return the CAN bus status.

### 'h'
This character will return the event hash table.

### 'y'
This character will reset the CAN bus and CBUS message processing.

### '\*'
This character will reboot the module.

### 'm'
This character will return the amount of free memory. 
 
 
 
 
 
