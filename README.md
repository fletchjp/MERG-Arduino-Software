 <img align="right" src="arduino_merg_logo.png"  width="150" height="75">

 # MERG-Arduino-Software

 ## Introduction

 Information about the GitHub work for the MERG Arduino SIG.

 This is not the only GitHub site. Several other MERG members have their own sites and we are colaborating to develop Arduino codes of use to the wider MERG membership. 

 John Fletcher M6777

 ## Software for MERG Arduino projects

 I am aiming to help MERG members to collect together Arduino software for projects.

 Some projects will make use of libraries which are already somewhere else on Github.

 The initial plan was for each separate project to be in a separate GitHub repository which could be one thing or a group of related things. This repository now has a lot of examples of how to use the various libraries.

 ## Licence

 I need to sort out the licence situation before creating and publishing any more repositories.

 I am thinking of using the CC BY-NC-SA as used by Duncan Greenwood and recommended. I have found the full text. I have not yet found the text of the summary which he uses.

 ## C++ Libraries

 I have now found libraries for ArduinoSTL and two versions of Boost for Arduino which I have forked to my github. These need to be installed into the arduino/libraries of your setup to be of use. Taken together these make it possible to port C++ software to the Arduino. I have things running both on AVR (MEGA and UNO) and also on DUE. The more recent Boost only runs on a DUE.

 My reason for doing this is to be able to make use of previous work with C++ to solve practical problems in the Arduino environment.

 I have moved the examples for each library to a separate folder.

 ### Boost Libraries

 For those who don't know, Boost Libraries are a set of peer reviewed C++ libraries. https://www.boost.org/ They have been developed over many years and a lot of the ideas have been incorporated into C++ as it has evolved. As the C++ available on Arduinos is limited to C++11 (think 2011), a lot of things in Boost are not otherwise available. Not all of this is usable in the Arduino environment, although a lot is.

 I am going to build some examples to show what can be useful.

 ### Experience

 As I have developed examples I have come across a number of compatibility problems which I have been able to work around by one means or another. The main limitation is that the Streaming library is not compatible with using std::cout. The best thing is not to use it at all with ArduinoSTL. Various other difficulties are discussed in the comments in the examples.

 I have managed to solve all the problems so far without any changes to either of the libraries. This is no long true as I have to add to the limits.h file in ArduinoSTL to get some things to compile.
 
 ### ArduinoSTL

 This provides a set of std C++ headers. There is one problem which is that it does not work with the Arduino AVR boards version 1.8.3 (latest). It needs version 1.8.2 which can be configured from the Boards Manager on the Arduino IDE. I have not explored alternative IDEs.

 There are two examples ArduinoSTLHello and ArduinoSTLIntegers in this repository.

 ### Arduino-Boost

 This provides a copy of the headers for Boost 1.51.0 which is an old version of Boost from 2012. Although this is an old version it provides a lot of useful things which can solve some problems and make more readable code.

There is a simple example ArduinoBoostIntegers in this repository which I plan to expand.

There is an example ArduinoBoostFunction which shows a set of simple uses of boost::function. This has some extra code in it to overcome the fact that Arduino C++ does not support exceptions and boost::function uses them. The extra code contains user defined routines to intercept the boost::function calls so that the code compiles. This will need to be borne in mind in any implementation.

There are two more examples, ArduinoBoostFunctionBind and ArduinoBoostFunctionPhoenix which show different ways to delete a string from a vector of strings.

 ### Boost for Arduino

I have found this more recent Boost 1.66.0 and I am looking to get this working as well. This version provides a lot more things which were added to Boost after 2012. It dates from December 2017.

At the moment I have problems with AVR (MEGA) with examples using boost function. I have not been able to resolve these. They seem to relate to changes in the internal organisation of Boost between 1.51.0 and 1.66.0.

I do have boost function examples working with DUE. This has involved a lot of experimentation as the C++11 implementation available has various limitations which I have been able to work around.

There are two examples, ArduinoBoost166FunctionBind and ArduinoBoost166FunctionPhoenix which show different ways to delete a string from a vector of strings. These work only on a DUE.

A lot of the issues getting the code to work are explained in the example ArduinoBoost166FunctionPhoenix.

I am now adding more examples. In particular ArduinoBoost166FunctionLazy is starting to use some of the features I added to Boost Phoenix back around 2015.

 ## Branches

 I have decided to hold some codes in branches of this repository, rather than having a lot of other repositories at this stage.

 NOTE: when working with branches I usually have the files visible in VSCODE and also in the Windows Explorer. It is important to close both these things before changing branch as the views will be outofdate and editing them would be an error.

 I have now modified the codes on both the branches to follow the changes in CANmINnOUT. There is no way to do this automatically.

 ### cbus_codes

 This is the first of the branches. It will have some CBUS modules, some related to the non CBUS codes in this main branch.

 This is going well and I am developing two modules CAN1602BUT and CANMODEM.

 ### due_cbus

 This branch is for codes for the Arduino DUE to run CBUS.

 I now have a module CANDUE which is now working.

 This branch is now developing and I have a second DUE because there is some interesting possibilities to develop functional software using FC++. This will not work on the AVRs (ordinary Arduinos) as the library support is different. This can be overcome by using ArduinoSTL.

 ## Logos

 Exploring the use of logos of different sizes.

 ![alt text][MergLogo]

 ![alt text][ArduinoMergLogo]

 <img src="arduino_merg_logo.png" width="200" height="100" alt="Arduino MERG Logo 200 x 100">

 <img src="arduino_merg_logo.png" width="100" height="50" alt="Arduino MERG Logo 100 x 50">
 
 <p float="left">
   <img src="arduino_merg_logo.png" width="50" height="25">
   <img src="arduino_merg_logo.png" width="50" height="25">
   <img src="arduino_merg_logo.png" width="50" height="25">
   <img src="arduino_merg_logo.png" width="50" height="25">
   <img src="arduino_merg_logo.png" width="50" height="25">
   <img src="arduino_merg_logo.png" width="50" height="25">
   <img src="arduino_merg_logo.png" width="50" height="25">
   <img src="arduino_merg_logo.png" width="50" height="25">
   <img src="arduino_merg_logo.png" width="50" height="25">
 </p>


 <img align="right" src="arduino_cbus_logo.png"  width="150" height="75">

 ### Arduino CBUS Logo

 There is also now an Arduino CBUS logo which is being used as seen above for Arduino CBUS projects.

 ## MERG Knowledgebase

 ### GitHub on the Knowledgebase

 There is information on the MERG Arduino SIG pages about our use of GitHub.

 [Arduino Github Guidance](https://www.merg.org.uk/merg_wiki/doku.php?id=arduino:software:github)

 ## Reflection

 Somewhere for [reflection on GitHub learning](REFLECTION.md) during the work on the projects.
 
 ## Software

 Intially the only software I kept here were general purpose utilities and simple examples to demonstrate errors.

 I am now expanding that to include some other nonCBUS software which may have CBUS versions elsewhere.

 ### LCDshieldButtonsSerial

 This is a code to demonstrate the use of a shield with a 1602 display and 5 buttons attached to an Arduino UNO. It has been adapted from one in the public domain to add serial output.

 HARDWARE: D1 Robot shield. Note that the screw on the blue block may need turning to get the contrast set so that a display is visible.

 SOFTWARE: LiquidCrystal library

 ### LCDshieldButtonsSerialDFRobot

 This is a modified code for the DFRobot shield.

 ### LCDshieldButtonsSerialDFRobotCB

 This is modified to use TaskManagerIO to control a callback on pin A0.

 This reads A0 directly.
 
 ### LCDshieldButtonsSerialDFRobotClass

 This does something more complicated using a class inheriting from AnalogInEvent. See the analogExample with the IoAbstraction library for the origin.

 ## Editors

 There is a choice of editors for working with GitHub. I am using Virtual Studio Code (VS Code). This recognises the Arduino files and generates .json configuration files. I am going to use .gitignore so that these files are not catalogued by GitHub although they do seem to get into the repository.

 Different contributors may be using different editors and this may mean that other files also get into the repository.

 ### VS Code 

 I have discovered that the Arduino add on for this editor assumes that there is going to be one sketch in each repository. I want to have several so that a group of different examples on the same theme can be together in a logical way. I can use VS Code to edit and organise the files. Its configuration is not going to be compatible. I can use .gitignore to tell GitHub to ignore the vscode files.

 I am moving [VSCODE learning](VSCODE.md) to a separate file.

 ## Repositories

 I have started to add separate repositories for the Arduino codes which are already on the Knowledgebase.

 ### Arduino-IR-Task

 This has the codes for IR detector and task scheduling simple examples.

 ### Arduino-Servo-Switch

 This has the codes for the Servo examples including switching between tasks.

 ### CANASERVO 

 **Note** This is not yet published.

 Arduino CBUS module to control a servo using inputs from two IR detectors. This is derived from the code in the example switch2IOAbs3 in Arduino-Servo-Switch.

 ### CBUSmINnOUT

 This is a fork from Martin Da Costa.
 It needs CBUS 1.1.14 and CBUSconfigs 1.1.10 which are now in the Arduino system.

 I have added .gitignore to ignore the configuration files for VS Code.

 ### CBUS-DC-Cab CBUS-DC-Throttle

 These are two private repositories which I have forked from Martin Da Costa to work on development.

 I have now contributed the start of a README.md to each of them.

 ### Arduino_DC_Throttle

 This is the prototype code for the original hardware designed by John Purbrick. This is also forked from Martin Da Costa.

 ### SignalBox

 This is a fork of the work of Tony Clulow implementing new code for the Ezybus hardware.

 ### CANCMDDC2

 I have started to bring in the old versions. I have used Version 2.9 as a starting point for new work.

 ### CBUS_DC_Throttle_exp

 A version of CBUS_DC_Throttle with class code.

 ## Events

 I am moving [Events](EVENTS.md) to a separate file.

 See events for information on ArduinoSTL examples.
 
## Location

I have moved this to a new location: E:\GitHub\Arduino which I will now use for the different local repositories for Arduino work.

I have deleted the previous RS485 branch as that is now covered by the Arduino-RS485 repository.

I will need to move files here to add them to the repository.

## Learning

 I have just made a discovery. I went to commit this and found that I was on the RS485 branch, which probably needs to become a separate repository.
 I had made the changes on that branch and was able to change branches and take the changes with me. I did not know that was possible.

## Queries

I would like to move the whole RS485 branch to be a separate repository. At the moment I do not know how to do that.

I have resolved this by creating a new repository for RS485 work and I will put the files there as I want them.

### Tags and Releases

I attempted to create a tag in Arduino-IR-Tasks using gitg. That apparently worked and I could add a description.
This was not recoginised in GitHub desktop so I deleted it and created it in GitHub desktop instead. That does not permit a description (as far as I have found).
GitHub desktop tags are in History.
I have pushed that to the GitHub version where a tag can be changed into a release with a description
I have had problems with gitg updating. I think I will use this as read only and open it when I want to look at the history of a repository in a concise way.

### Syntax

I will document my learning of the Markdown syntax here.  At the moment all I have done is to add some headings and can see how they work.

I have found a cheatsheet for Markdown on GitHub and added it to the Knowledgebase page. Here is a link to it too:

[Markdown Cheatsheet](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)

[MergLogo]: merg_logo.png "MERG logo"
[ArduinoMergLogo]: arduino_merg_logo.png "Arduino MERG logo"




