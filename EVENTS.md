# Events

Back to [README](README.md).

## April 2021

### 12th April 2021

 I raised the avr32/io.h issue with the Arduino Extension. I have added the SPI example DigitalPotControl as an example of the error. There is activity on the issue.

 I have started to use subfiles to slim down [README](README.md).

 ### 11th April 2021

 I have learned how to send a pull requests to Martin on his two repositories. These have now been merged in.
 
 I am continuing to work on the problem of Arduino configuration with VS Code. I have made some progress sorting out some errors I had made through not understanding some configuration options.

 ### 6th April 2021

 I have added .gitignore to the CBUSmINnOUT repository to ignore the .vscode files.

 ### 3rd April 2021

 I have forked Martin Da Costa's code for CBUSmINnOUT in order to use it for a prototype panel on the Totem Mini Lab.

 I needed the updates to the CBUS libraries which are not yet in the Arduino system. I have upgraded them by hand.

## March 2021


 ### 19th March 2021

 I am thinking about what to do with the CANCMDDC2 codes of which there are a lot of versions as a result of the work done last year without version control.
 
 The latest version CANCMDDC_V2_9 is the first to have Task Scheduling which should have been in the previous versions and so is a base from which to work.

 There is an earlier version CANCMDDC_V3_0 where I had started to add RS485 code to an earlier version.

 Things are complicated as the name from the point of view of MERG FCU will be CANCMDDC as the previous version of CANCMDDC does not support CBUS events.

 I am going to make a version of this with the H bridge controllers although it will need to evolve a version which will support the DC controllers when those become available for CBUS.

 I think this means that the repository should be named CANCMDDC rather than CANCMDDC2. Holding it with the other CANCMDDC can if neeed be handled using branches.

 So I need that repository.

 ### 16th March 2021

 Starting work on the arduino code for the DC Controler from John Purbrick. I have a copy of the hardware put together for me by Robert Thomas. I am exploring using this with the software called arduino_throttle.

 ## February 2021

 ### 25th February 2021.
 
 I am restarting this work as a result of Sven Rosvall's Zoom talk to the Arduino SIG. After some discussion with him I am going to have a series of different repositories for different pieces of software.
 I have set up a project in my ConnectedText wiki where I can document the repositories and their relationships.
 To start with the work in progress will remain on the local computer and not be moved to the public site on GitHub.

 Overall documentation of what is going on can be done here.

 ### 26th February 2021

 I have added in Visual Micro to the installation of Visual Studio Code which I am using for text editing. https://www.visualmicro.com/.

 I added a VSC option for C++ which it seemed to be needing and then the Arduino IDE. This added some configuration files to the current repository. One of these files recorded the board I have specified and also indentified the *.ino file in the repository.

 NOTE: This will make it difficult to have files for two different boards in the same repository.

 I do now understand the configuration I have to do. It does allow me to verify the code. There is an option to choose a serial port. It also wants an output path to be specified.

 It has detected an Octave based C++ compiler.
 
 ## 2020

 ### 29th November 2020.
 
 I have added pwmtest as an experiment to see what would happen. It is a test for the addresses of I2C devices.

 ### 28th November

Created Arduino-IR-Task repository


Back to [README](README.md).