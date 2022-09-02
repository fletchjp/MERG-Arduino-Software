# fcpp library files

These files are a copy of the fcpp library in Arduino/libraries. That is the location which is used for compilation.

These copies will be used to update fcpp and provide a backup of the files in use.

At the moment there are only a few changes to make the files Arduino compatible for use with RP2040 devices.

This means that they are actually the master copies for documentation rather than being a backup.

I have made some more changes to get the codes working with AVR UNO and DUEs. Those changes seem to be alright on the Pico as well.

FCPP_ARDUINO is defined in all cases to isolate features not supported.

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

Add new operator xor2 to do exclusive or.

I have made DropWhileEqual into a full functoid and used it to solve some other problems.
					
### src/fcpp/functors.h  					

Add some explanation.

Added new Bimap2 operator to extend Bimap to two argument functions, bimap2(f,g,p1,p2)

This returns the pair  (f(p1.first,p2.first),g(p1.second,p2.second)

### src/fcpp/function.h

Remove unused arguments of nullfunN functions to supress gcc10 warnings.

### src/fcpp/monoids.h  					

This is a new file to hold the functoid codes which have been developed using an RPi Pico.

This now includes the implementations Mstring and Mlist.

I have decided not to include the output operators which are Arduino specific.

This has now been renamed from functoids.h.

It now has quite a lot more code for the versions which infer the operands.

There are now specializations in mmappend for Fun1<T,T> and Full1<F>.

There is more code to come for MonoidT<MonoidType> implementations which make it easy to have more monoids.

Added a new constructor so that a MonoidType can be constructed from its own type.

### src/fcpp/pattern.h

Added new Parallel2 operator to extend Parallel to two argument functions, parallel2(makePair(f,g),p1,p2)
