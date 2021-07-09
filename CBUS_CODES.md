 <img align="right" src="arduino_cbus_logo.png"  width="150" height="75">

# CBUS Codes

This is a branch for the location of CBUS codes, some of which will be related to the codes in the main branch.

I will not modify the other Markdown files in the branch, keeping information in this and other new files.

These codes are part of development of modules for control of DC layouts using DC controllers for each track section.

## Testing

I am now starting testing of these codes with CBUS and 
FCU.

I have not yet set up access to GitHub on the laptop I am using for testing. That means that I have to take a copy of the code to it. At least I no longer have to worry about versions of the code as they are all available.

### 9th July

### Codes added

I have added the arduino_dc_throttle code to the repository. At the moment it is stand alone and can be controlled from the Arduino IDE. I would like to have a version which could be controlled from another Arduino either through the Serial port or I2C.

I want to develop a unit with a keypad and encoder as a CBUS module. I have used both in the past standalone, so will have the code somewhere. If this is on a MEGA it will have a spare serial port.

I have also added matrixKeyboard2 which is an example which monitors a Keypad using KeyboardListener from IoAbstraction.

I have now added some codes for use with an encoder.

EncoderOnly is an Elegoo code for the encoder.

WithEncoder is Elegoo code with encoder and stepper motor.

buttonRotaryEncoder is an IoAbstraction example which I want to change to match the EncoderOnly pin use.


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
