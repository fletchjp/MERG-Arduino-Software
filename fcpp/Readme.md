# fcpp library files

These files are a copy of the fcpp library in Arduino/libraries. That is the location which is used for compilation.

These copies will be used to update fcpp and provide a backup of the files in use.

At the moment there are only a few changes to make the files Arduino compatible for use with RP2040 devices.

## FC++ History

These codes have a very long history which I will not go into here.

The original codes were written for C++ long before there was C++11. A lot was achieved which can be done much more easily now.

The versions in use here have many enhancements I have written over the years which have not been made available previously.

Readers of the code will find many comments with dates going back over decades.

I have extended the functionality while remaining faithful to the original design ideas.

John Fletcher  August 2022

## Arduino limitations

These codes are compatible with gcc10 and C++17 as available on the Arduino IDE implementation of Earle Philhower.

The main limitation is that exceptions are not available which does mean some danger in use of the code.

There is a version of Boost library for the Arduino which is available. It is not uptodate.

## New additions and significant changes

These will be noted here.

src/fcpp_prelude.h  Added copyright and explanation of the need for the file.
src/fcpp/prelude.h  Added code for MaybeLike test to be used in unjust().

