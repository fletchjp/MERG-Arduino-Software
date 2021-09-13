 <img align="right" src="arduino_cbus_logo.png"  width="150" height="75">

# CBUS Codes

This is a branch for the location of CBUS codes, some of which will be related to the codes in the main branch.

I will not modify the other Markdown files in the branch, keeping information in this and other new files.

These codes are part of development of modules for control of DC layouts using DC controllers for each track section.

## CBUS Long Message codes

CBUS long messages are sent using a stream, which has a stream number. The sending module sends to a specific number and the other modules listen to the streams as needed, not the one they send on. This means that I need to have a scheme for the allocation of the stream numbers. For the moment I will assign a different number to each module, using a series starting at 11. If there are multiple instances of a particular module code the numbers will have to be set in some other way, such as configuration. I can build in some checks.

Sending streams assigned

11  CANTEXTL - currently on old hardware
12  CAN1602PIN 
13
14  CANTOTEM

## Testing

I am now starting testing of these codes with CBUS and 
FCU.

I have not yet set up access to GitHub on the laptop I am using for testing. That means that I have to take a copy of the code to it. At least I no longer have to worry about versions of the code as they are all available.

### 13th September

Duncan has now taken things a step further and I need to adapt my codes to his new version. I am going to do the AVR ones first and leave the DUE unchanged for the moment.

### 8th September

There are new versions of some of the Arduino CBUS libraries. There are some changes needed for all codes which use the long message part of the library. I will note here when the codes have been changed.

CAN1602PIN now done. Buffers now (2,2) as (4,4) crashed.
CANTOTEM now done.
CANTEXTL now done.


### 7th September ###

CANTOTEM:   The buffering of output messages is a success.
CAN1602PIN: add code to display the received message and add buffering.
CANTEXTL:   add code for display and buffering.

This provides a new standard for operations with the Arduino CBUS Library.

### 4th September ###

I have modified CAN1602PIN to trial using CBUS.available() to check whether a previous event has been cleared. This code is near to the memory limit for a UNO.

### 1st September ###

I am using a modified version of CBUSconfig which allows for testing of reset.

### 30th August ###

I now have the long message codes in CANTOTEM and CAN1602PIN working and have been able to transfer a message from both. For now I have turned off the sending of an event just before sending the long message as that messed up the long message. I also learned that the message buffer must be global (or at least outside the sending function) as a buffer within the function goes out of scope and the message content can be lost.

### 25th August 

I have found a bug in the updated of CBUS2515 and the codes are now working. More testing with long messages can now start. I have new ideas to follow up following a Zoom discussion.

I did some testing with mixed results. CANTOTEM kept issuing events for a reason I don't understand. CAN1602PIN would not register with FCU and I think was not sending any events at all (that was because it was not plugged in!). CANDUE did work (it belongs on the other branch) and was useful to see what was going on. I did see some long messages from CANTOTEM, some with zero size which is worrying. I want to find the cause of the problem with CAN1602PIN.

There must be a difference between the configurations on my different computers. When I compiled the CAN1602PIN code on the laptop it was bigger - too big to load into a UNO. I tracked this down to different versions of several things. There were some libraries to update. The main difference is the AVR board version, which I had set at 1.8.2 on the main computer instead of the latest 1.8.3 which gives bigger binary files. I have set the laptop to 1.8.2 for now as well.

I have put code into CANTOTEM to monitor the size of long messages.

### 21st and 23rd August

I have been adding code to send and receive the long messages to CANTEXTL and CANTOTEM. This code has been locked out of use. I have now activated it successfully using an update of Duncan's library. I now have examples which complete the codes. Initial off line tests failed.

### 17th August

I have news from Duncan Greenwood that he is updating the Arduino CBUS library to support CBUS long messages. This is very exciting and I will be putting together an example for when the library is released.

### 14th & 16th August

I have ordered a device with 5 buttons to connect via an analog pin. I want to fit this to the TOTEM frame expanding CANTOTEM. I have now used the code from CAN1602BUT rather than CAN1602PIN which assumes the whole DfRobot shield.

The buttons have now arrived and I have adjusted the button detection values in CANTOTEM to detect the buttons correctly. I now need to find a way to fix the buttons to the frame.

### 9th July

### Codes added

I have added the arduino_dc_throttle code to the repository. At the moment it is stand alone and can be controlled from the Arduino IDE. I would like to have a version which could be controlled from another Arduino either through the Serial port or I2C.

I want to develop a unit with a keypad and encoder as a CBUS module. I have used both in the past standalone, so will have the code somewhere. If this is on a MEGA it will have a spare serial port.

I have also added matrixKeyboard2 which is an example which monitors a Keypad using KeyboardListener from IoAbstraction.

I have now added some codes for use with an encoder.

EncoderOnly is an Elegoo code for the encoder.

WithEncoder is Elegoo code with encoder and stepper motor.

buttonRotaryEncoder is an IoAbstraction example which I want to change to match the EncoderOnly pin use.

I have just found two codes in the IoAbstraction library examples which relate to the DFRobot display and pins board which I am using in CAN1602BUT. They are dfRobotAnalogInSwitches and dfRobotRotaryEncoder. They are not so far as I can see mentioned in the main document. They use a header file DfRobotInputAbstraction.h which is documented in the reference material.

The importance of this is that enables the analog input on the DFRobot display with buttons to be treated as though they are pins. I have been exploring using this. I now have a new code CAN1602PIN which is derived from CAN1602BUT using the abstraction.

I am also exploring the IoAbstraction methods for a rotary encoder. They also use buttons instead of the encoder. The first example is myButtonRotaryEncoder.

### 3rd June 

Testing of CANTOTEM 1c beta 3 with criticalEvent code. A first test has been successful. Now I need to update the code in CAN1602BUT to display the correct message.

### 31st May

I have put into CANTOTEM and example code where one task sends a message to another task.

### 28th May

Sven has suggested using a DEBUG_PRINT macro instead of conditional code and has implemented it in CANmINnOUT.

To be consistent I am going to do that in CAN1602BUT and CANTOTEM.

### 27th May 

I have made changes to CAN1602BUT and CANTOTEM reflecting changes suggested by Sven Rosvall to CANmINnOUT.

I have also corrected the error in each of the lack of a return value from sendEvent.

I am also going to add send failure detection in ProcessSwitches as well.

### 26th May

I have corrected two errors in the event generation code in CAN1602BUT and CANTOTEM which came from the CANmINnOUT code I used as a basis. I had not noticed them as I had not run these cases.

### 18th May

Martin's repository CBUSmINnOUT has been renamed CANmINnOUT and I need to change any references here.

### 13th May

I fitted both the modules with connectors for CBUS.

I ran CANALCDBUT and it did not get recognised by CBUS.

I ran CANTOTEM and it did connect to CBUS but only with a node number of 65535 which I could not change. It needs a longer connecting link.

I have now found that both modules need to be reset to Flim mode with 'z' 'z' before doing 'r' to connect to FCU.

## Codes

Note: The codes using the signal control library now have an additional file FastPWMLight.h which is an extension of mine and not part of the library.

### CANASIGNALI2C_CS15

This is derived from work using I2C to provide a PWM driven signal controlled by an IR detector. I have modified this as a first test of the use of Pin 15 instead of Pin 10 for the chip select function needed for SCI in order to run the Arduino CBUS library. So far I have only tested it as an isolated module and not yet attempted to connect it to anything else.

The reason for this experiment is that Pin 10 is not available for use with the 1602 display with buttons when used as a shield rather than an I2C item.

### CANALCDSIGNALI2C_CS15

This is the first code built to see if the display can work with the Arduino CBUS library. I have built this on the signal code as it easier to add things to a code than build one completely from scratch.

I think that the I2C for the signals can be linked from A4 and A5 which are available on the DFRobot board.

I also moved the IR and Signal pins to A2 and A3 as they were on pins which are used by the display.

### CANALCDBUT (was CANALCDNOSIGNALI2C_CS15)

This is a version with an option to disable the signal code for testing without a signal and detection.

This code was missing the reset processing using 'z' 'z' and it has now been added.

I am thinking to build a new version of this code based on the CANmINnOUT code (as I have for CANTOTEM). I need a new name for it! One reason is that there is so much old history in this code.

### CAN1602BUT

I have now built this version. I have given it 1 button and 1 LED in the code although they are not actually present. I have now added code to convert the button presses into events. This has now been tested successfully with CBUS and events from it taught to another module (CANPAN). I have given it the version 1b as otherwise FCU thinks the version is 32.

I have started to add code from CANTEXT to process an incoming message to the display.

I have now done some tests of this against CANTOTEM.

1b beta 11 Make processSerialInput a task (suggested by Sven Rosvall).

### Another Idea (yet to be named)

I am also developing a version where the display is connected using an I2C backpack for all the connections except the A0 and reset.

## Totem Codes

Codes configured for the UNO on the Totem Minilab.

### Totem_int3

Simple case to raise interrupt on Pin 3 when a button is pushed.

### TOTEMmINnOUT

Copy of CANmINnOUT by Martin Da Costa.

A base for testing on the Totem Minilab.

### CANTOTEM

This has been modified to use TaskManagerIO.

I am now using this version for testing against CAN1602BUT. I have added some new code for framehandler so that I can see what traffic is passing.

I have also added code to eventhandler to see the node number and event number of incoming events.

I have also added code to raise a message in a task from another task. This is not yet tested. It is now modified and raises a criticalEvent withing the TaskManagerIO structure. This is now used to send CBUS events.

1c beta 3. I have extended the criticalEvent code and now set this up for testing.

1c beta 5 Make processSerialInput a task (suggested by Sven Rosvall).
