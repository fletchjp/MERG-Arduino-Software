@mainpage

## ArduinoRP2040BoostSpiritX3Lazy

### Example of Boost Spirit X3 Lazy

Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)

This is based on an example: 

https://stackoverflow.com/questions/60171119/boost-spirit-x3-lazy-parser

I was lead to this looking for examples of subcontext and any_parser, both of which are in X3 and lack examples.

Being adapted from the variant example.

Some of the definitions only work when moved to the header file variantCode.h 

I have added some more code for std::variant obtained from here:
https://stackoverflow.com/questions/61392947/transitioning-boost-spirit-parser-from-boostvariant-to-stdvariant/61409717#61409717

I have also added some code from 
https://github.com/SuperV1234/vittorioromeo.info/blob/master/extra/visiting_variants/variant_aliases.hpp
which moves between std::variant and boost::variant for visiting.

This has involved a number of adaptions to the Arduino environment which are contained in the sketch file.

Common features have been moved to ArduinoCode.h which can be used in other examples.

This file only runs on the Arduino NANO RP2040 connect.
