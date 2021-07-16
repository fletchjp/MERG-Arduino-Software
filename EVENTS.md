# Events

Back to [README](README.md).

## July 2021

### 16th July

I have been working on the CBUS codes bringing in some more examples and adapting them to the latest standards of CBUS interface.

I have also found that one of them, CANTEXT, is tight on memory so I have been making use of PROGMEM to move const data to the program memory. This can be extended to other codes even though the memory there may not be so tight.

## May 2021

### 8th May

I have been doing a lot on the hardware side, looking at Arduino shields which have a 16 by 2 display and buttons. These are used in Ezybus and also by Tony Clulow in his work with similar hardware and different code.

There are some examples here of codes which test the buttons and report on the display. I have expanded the original codes to use task management in the same way as I have done with other devices. The examples are here.

I want to expand this work to include a version which works with CBUS. This puts extra demands on the pins and can be done in two ways.

1. Using the DFRobot version of the shield I can use pins 11 12 and 13 for the SCI interface and pin 15 for the CS. Pin 10 is not available as it is used for the display backlight.

2. Use a backpack to convert the display to I2C, when the SCI pins are free anyway. I plan to use a D1Robot shield to test this.

## April 2021

### 18th April

I think I have now resolved all the configuration problems with Arduino usage.

### 16th April

Made a serious start on CANCMDDC2 codes.

### 15th April

Sorted out a problem for Martin on CBUS_DC_Throttle_exp

### 14th April

Expand use of the avr-gcc compiler on avr-gcc branches, see [VS CODE learning](VSCODE.md). I have had no problems with this. I can merge the branches in while keeping the avr-gcc branch for more experiments.

### 13th April

I have updated VSCODE.md to reflect the learning about VS Code and the solution of a number of problems.

I have merged some changes Martin had made to the CBUS codes and set up pull requests for my .vscode configuration changes.

I am still investigating how to solve the issue of the register names.

### 12th April

 I raised the avr32/io.h issue with the Arduino Extension. I have added the SPI example DigitalPotControl as an example of the error. There is activity on the issue.

 I have started to use subfiles to slim down [README](README.md).

 ### 11th April

 I have learned how to send a pull requests to Martin on his two repositories. These have now been merged in.
 
 I am continuing to work on the problem of Arduino configuration with VS Code. I have made some progress sorting out some errors I had made through not understanding some configuration options.

 ### 6th April

 I have added .gitignore to the CBUSmINnOUT repository to ignore the .vscode files.

 ### 3rd April

 I have forked Martin Da Costa's code for CBUSmINnOUT in order to use it for a prototype panel on the Totem Mini Lab.

 I needed the updates to the CBUS libraries which are not yet in the Arduino system. I have upgraded them by hand.

## March 2021


 ### 19th March

 I am thinking about what to do with the CANCMDDC2 codes of which there are a lot of versions as a result of the work done last year without version control.
 
 The latest version CANCMDDC_V2_9 is the first to have Task Scheduling which should have been in the previous versions and so is a base from which to work.

 There is an earlier version CANCMDDC_V3_0 where I had started to add RS485 code to an earlier version.

 Things are complicated as the name from the point of view of MERG FCU will be CANCMDDC as the previous version of CANCMDDC does not support CBUS events.

 I am going to make a version of this with the H bridge controllers although it will need to evolve a version which will support the DC controllers when those become available for CBUS.

 I think this means that the repository should be named CANCMDDC rather than CANCMDDC2. Holding it with the other CANCMDDC can if neeed be handled using branches.

 So I need that repository.

 ### 16th March

 Starting work on the arduino code for the DC Controler from John Purbrick. I have a copy of the hardware put together for me by Robert Thomas. I am exploring using this with the software called arduino_throttle.

 ## February 2021

 ### 25th February
 
 I am restarting this work as a result of Sven Rosvall's Zoom talk to the Arduino SIG. After some discussion with him I am going to have a series of different repositories for different pieces of software.
 I have set up a project in my ConnectedText wiki where I can document the repositories and their relationships.
 To start with the work in progress will remain on the local computer and not be moved to the public site on GitHub.

 Overall documentation of what is going on can be done here.

 ### 26th February

 I have added in Visual Micro to the installation of Visual Studio Code which I am using for text editing. https://www.visualmicro.com/.

 I added a VSC option for C++ which it seemed to be needing and then the Arduino IDE. This added some configuration files to the current repository. One of these files recorded the board I have specified and also indentified the *.ino file in the repository.

 NOTE: This will make it difficult to have files for two different boards in the same repository.

 I do now understand the configuration I have to do. It does allow me to verify the code. There is an option to choose a serial port. It also wants an output path to be specified.

 It has detected an Octave based C++ compiler.
 
 ## 2020

 ### 29th November
 
 I have added pwmtest as an experiment to see what would happen. It is a test for the addresses of I2C devices.

 ### 28th November

Created Arduino-IR-Task repository


Back to [README](README.md).