 # MERG-Arduino-Software
 
 ![alt text][MergLogo]

 ![alt text][ArduinoMergLogo]

 Information about the GitHub work for the MERG Arduino SIG.

 This is not the only GitHub site. Several other MERG members have their own sites and we are colaborating to develop Arduino codes of use to the wider MERG membership. 

 John Fletcher M6777

 ## Software for MERG Arduino projects

 I am aiming to help MERG members to collect together Arduino software for projects.

 Some projects will make use of libraries which are already somewhere else on Github.

 Each separate project will be in a separate GitHub repository which could be one thing or a group of related things.

 ## Licence

 I need to sort out the licence situation before creating and publishing any more repositories.

 I am thinking of using the CC BY-NC-SA as used by Duncan Greenwood and recommended. I have found the full text. I have not yet found the text of the summary which he uses.

 ## MERG Knowledgebase

 ### GitHub on the Knowledgebase

 There is information on the MERG Arduino SIG pages about our use of GitHub.

 [Arduino Github Guidance](https://www.merg.org.uk/merg_wiki/doku.php?id=arduino:software:github)

 ## Reflection

 Somewhere for [reflection on GitHub learning](REFLECTION.md) during the work on the projects.
 
 ## Software

 The only software I will keep here are general purpose utilities and simple examples to demonstrate errors. 

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

