@mainpage

## ArduinoRP2040BoostSpiritX3Custom3

### Example of Boost Spirit X3 Custom3

Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)

This is an example implementing custom diagnosis
and adapted to run on the Arduino NANO RP2040 connect.

So far it does diagnosis on success and the failure message does not appear.

I have relocated the custom code to a separate file and the result is unchanged.

Further experiments to see what can be done.

This example now finds the error information for the first error without using exceptions. 
This depends on a modification to Boost Spirit X3 in file \boost\spirit\home\x3\directive\expect.hpp.
This is only active if BOOST_SPIRIT_X3_NO_EXCEPTION is defined.
 

The modification uses a global variable called boost::spirit::x3::where_was_I defined as std::vector<std::string>
It is defined as extern in the library file and has to be defined somewhere in the user code to be sure it is only defined once.

In this example that is in ArduinoCode.h

This has involved a number of adaptions to the Arduino environment which are contained in the sketch file.

Common features have been moved to ArduinoCode.h which can be used in other examples.

This file only runs on the Arduino NANO RP2040 connect.
