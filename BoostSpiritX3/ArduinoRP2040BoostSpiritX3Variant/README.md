@mainpage

## ArduinoRP2040BoostSpiritX3Variant

### Example of Boost Spirit X3 Variant

Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)

This is taken from spirit/test/x3/x3_variant
and adapted to run on the Arduino NANO RP2040 connect.

Some of the definitions only work when moved to the header file variantCode.h 

I have added some more code for std::variant obtained from here:
https://stackoverflow.com/questions/61392947/transitioning-boost-spirit-parser-from-boostvariant-to-stdvariant/61409717#61409717

This has involved a number of adaptions to the Arduino environment which are contained in the sketch file.

Common features have been moved to ArduinoCode.h which can be used in other examples.

This file only runs on the Arduino NANO RP2040 connect.
