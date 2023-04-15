<img align="right" src="arduino_cbus_logo.png"  width="150" height="75">

# CAN_Wrapper

An Arduino program that provides a MERG CBUS interface that can interface with the users
application.  The program is derived from CANmINnOUT, which was originally developed as
a technology demonstrator. CANmINnOUT thus serves as an exmaple of how to use this program
and can be found here:

https://github.com/MartinDaCosta53/CANmINnOUT

Key Features:
- MERG CBUS interface
- Modular construction to ease adaptation to your application.

## Overview

The program does not allow for the inclusion of a CBUS setup switch or LEDs. Consequently,
the following library versions are required:
- CBUS Version 1.1.14 or later
- CBUSConfig 1.1.10 or later
Notwithstanding the fact that they are not used, CBUSSwitch and CBUSLED libraries must be 
available for access by the CBUS and CBUSConfig libraries for reason of backwards library
compatibility.

It is possible that the CBUS setup switch could be reinstated for purposes of requesting a
Node Number.  The LEDs remain superfluous as SLiM is not supported and the module is always
in FLiM.

## Setting up CAN_Wrapper for use

The MCP2515 interface requires five Arduino pins to be allocated. Three of these are fixed
in the architecture of the Arduino processor. One pin must be connected to an interrupt
capable Arduino pin. Thus, the total number of pins available for input or output is:
- UNO  13 pins
- NANO 15 pins (Note that two of these, A6 & A7, are analogue inputs only)
- MEGA 63 pins

**It is the users responsibility that the total current that the Arduino is asked to supply 
**stays within the capacity of the on board regulator.  Failure to do this will result in 
**terminal damage to your Arduino.

### Library Dependancies

The following third party libraries are required:
Library | Purpose
---------------|-----------------
Streaming.h  |*C++ stream style output, v5, (http://arduiniana.org/libraries/streaming/)*
ACAN2515.h   |*library to support the MCP2515/25625 CAN controller IC*
CBUS2515.h   |*CAN controller and CBUS class
CBUSconfig.h |*module configuration*
CBUS.h       |*CBUS Class*
cbusdefs.h   |*Definition of CBUS codes*
CBUSParams.h   |*Manage CBUS parameters*
CBUSSwitch.h   |*library compatibility*
CBUSLED.h      |*library compatibility*

### Pre-Processor Options

Various output reports can be made to the serial monitor for use in debugging by
changing 0 to 1 for the DEBUG macro in the DEFINE MODULE section at line c.73.
```
#define DEBUG 0       // set to 0 for no serial debug
```

### Application Configuration

#### Module Name
The module name can be adjusted as required to reflect the module configuration.  Only one
character is allowed between each pair of ' ' and the total number of characters must be seven.
```
// module name
unsigned char mname[7] = { 'C', 'A', 'N', 'w', 'r', 'a', 'p' };
```

#### Oscillator Frequency
If the oscillator frequency on your CAN2515 board is not 16MHz, change the number to match. The 
module will not work if this number does not match the oscillator frequency.
```
const unsigned long CAN_OSC_FREQ = 16000000;     // Oscillator frequency on the CAN2515 board
```

#### Framehandler
The following section of code configures the module for using the library framehandler. If the
application does not need framehandler, it can be safely deleted. If not included, the function
setupCBUS() must also be modified to delete:
```
// register our CBUS frame handler to receive messages in user list
  CBUS.setFrameHandler(framehandler, (byte *) opcodes, nopcodes);
```
Framehandler is NOT included in the CANmINnOUT example.
```
// Forward Function declarations
void framehandler(CANFrame *msg);

// User list of opcodes.  This list is an example and can be changed as required. List is retained in program memory
const byte opcodes[] PROGMEM = {OPC_DFNON, OPC_DFNOF, OPC_DSPD, OPC_GLOC, OPC_KLOC, OPC_PLOC};

// Calculate number of opcodes in list
const byte nopcodes = sizeof(opcodes) / sizeof(opcodes[0]);
DEBUG_PRINT(F("> Number of op codes ") << nopcodes);
```
The list of op codes at line c.108 is just an example.  It can consist of as many or as few op codes 
as required.  They op codes specified MUST be in the cbusdefs.h or a compile error will occur.
The hex op code may also be used (eg OPC_DFNON could be replaced by 0x49)

## sendEvent function

This function can be overloaded as required by the op code.  Included are functions for no data bytes
and one data byte.  Simple extrapolation of these allows for two or three data bytes.

## sendFrame function

This function is a more general version of the sendEvent function.  This function is only required if 
the application is required to generate op codes that are not within the subset of taught events 
supported by the CBUS library.  Included are functions for op code only and two data bytes.  Simple
extrapolation of these allows for one to seven data bytes.

## CBUS Op Codes for Learnt Events

The following learnt event Op codes are supported by the CBUS library:
OP_CODE | HEX | Function
----------|---------|---------
 OPC_ACON | 0x90 | On event
 OPC_ACON1| 0xB0 | On event with one data byte
 OPC_ACON2| 0xD0 | On event with two data byte
 OPC_ACON3| 0xF0 | On event with three data byte
 OPC_ACOF | 0x91 | Off event
 OPC_ACOF1| 0xB1 | Off event with one data byte
 OPC_ACOF2| 0xD1 | Off event with two data byte
 OPC_ACOF3| 0xF1 | Off event with three data byte
 OPC_ARON | 0x93 | Accessory response event on
 OPC_AROF | 0x94 | Accessory response event off
 OPC_ASON | 0x98 | Short event on
 OPC_ASON1| 0xB8 | Short event on with one data byte
 OPC_ASON2| 0xD8 | Short event on with two data byte
 OPC_ASON3| 0xF8 | Short event on with three data byte
 OPC_ASOF | 0x99 | Short event off
 OPC_ASOF1| 0xB9 | Short event off with one data byte
 OPC_ASOF2| 0xD9 | Short event off with two data byte
 OPC_ASOF3| 0xF9 | Short event off with three data byte

Other op codes that are not supported in the CBUS library with learnt event variables
can be accomodated in the frame handler.
 
## Set Up and the Serial Monitor

Without a CBUS switch, it is necessary to have another means of registering the module on 
the CBUS and acquiring a node number.  This is accomplished by sending a single character to 
the Arduino using the serial send facility in the serial monitor.

#### 'r'
This character will cause the module to renegotiate its CBUS status by requesting a node number.
The FCU will respond as it would for any other unrecognised module.

#### 'z'
This character needs to be sent twice within 2 seconds so that its action is confirmed.
This will reset the module and clear the EEPROM.  It should thus be used with care.

Other information is available from the serial monitor using other commands:

#### 'n'
This character will return the node configuration.

#### 'e'
This character will return the learned event table in the EEPROM.

#### 'v'
This character will return the node variables.

#### 'c'
This character will return the CAN bus status.

#### 'h'
This character will return the event hash table.

#### 'y'
This character will reset the CAN bus and CBUS message processing.

#### '\*'
This character will reboot the module.

#### 'm'
This character will return the amount of free memory. 
 
