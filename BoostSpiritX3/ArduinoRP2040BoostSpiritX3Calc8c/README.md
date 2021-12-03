@mainpage

## ArduinoRP2040BoostSpiritX3Calc8c

### Example of Boost Spirit X3 Calc8c

Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0) and Boost Spirit X3 from Boost 1.77.0

This is taken from spirit/example/x3/calc/calc8 from Boost 1.77.0 
and adapted to run on the Arduino NANO RP2040 connect.

I have changed expectation (>) to sequence (>>) in the code which I am now going to reverse.

I am going to use BOOST_SPIRIT_X3_NO_EXCEPTION and custom error diagnostics.

This has involved a number of adaptions to the Arduino environment which are contained in the sketch file.

Common features have been moved to ArduinoCode.h which can be used in other examples.

This file only runs on the Arduino NANO RP2040 connect.
