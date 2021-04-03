# MERG-Arduino-Software
 
 ## Software for MERG Arduino projects

 I am aiming to help MERG members to collect together Arduino software for projects.

 Some projects will make use of libraries which are already somewhere else on Github.

 Each separate project will be in a separate GitHub repository which could be one thing or a group of related things.

 John Fletcher M6777

 ## Licence ##

 I need to sort out the licence situation before creating and publishing any more repositories.

 I am thinking of using the CC BY-NC-SA as used by Duncan Greenwood and recommended. I have found the full text. I have not yet found the text of the summary which he uses.

 ## MERG Knowledgebase

 There is information on the MERG Arduino SIG pages about our use of GitHub.

 [Arduino Github Guidance](https://www.merg.org.uk/merg_wiki/doku.php?id=arduino:software:github)
 
 ## Software

 The only software I will keep here are general purpose utilities.

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
 It needs CBUS 1.1.14 and CBUSconfigs 1.1.10 which are not yet in the Arduino system.

 ## Events

 ### 3rd April 2021

 I have forked Martin Da Costa's code for CBUSmINnOUT in order to use it for a prototype panel on the Totem Mini Lab.

 I need the updates to the CBUS libraries which are not yet in the Arduino system. I could upgrade them by hand.

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

 ### 29th November 2020.
 
 I have added pwmtest as an experiment to see what would happen. It is a test for the addresses of I2C devices.

 ### 28th November

Created Arduino-IR-Task repository

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




