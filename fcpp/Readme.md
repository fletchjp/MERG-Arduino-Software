# fcpp library files

These files are a copy of the fcpp library in Arduino/libraries. That is the location which is used for compilation.

These copies will be used to update fcpp and provide a backup of the files in use.

At the moment there are only a few changes to make the files Arduino compatible for use with RP2040 devices.

This means that they are actually the master copies for documentation rather than being a backup.

## FC++ History

These codes have a very long history which I will not go into here.

I have maintained the historical practice that the header codes are all *.h.

The original codes were written for C++ long before there was C++11. A lot was achieved which can be done much more easily now.

The versions in use here have many enhancements I have written over the years which have not been made available previously.

Readers of the code will find many comments with dates going back over decades.

I have extended the functionality while remaining faithful to the original design ideas.

John Fletcher  August 2022

## Arduino limitations

These codes are compatible with gcc10 and C++17 as available on the Arduino IDE implementation of Earle Philhower.

The main limitation is that exceptions are not available which does mean some danger in use of the code.

There is a version of Boost library for the Arduino which is available. It is not uptodate.

Code which is a problem is not compiled when FCPP_ARDUINO is defined. It is defined in this version of prelude.h 

## New additions and significant changes

These will be noted here.

### src/fcpp_prelude.h

Added copyright and explanation of the need for the file.

### src/fcpp/prelude.h  

Added code for MaybeLike test to be used in unjust(). Also code for EitherLike test.

Identified and fixed a bug in the code.

Code for MaybeLike and EitherLike with functoids unjust and unright moved here.
					
### src/fcpp/functor.h  					

Add some explanation.

### src/function.h

Remove unused arguments of nullfunN functions to supress gcc10 warnings.