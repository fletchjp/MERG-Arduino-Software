@mainpage

## ArduinoRP2040BoostSpiritX3LazyParser3

### Example of Boost Spirit X3 Lazy parser3

Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)

Lazy example modified to NOT use the any_parser code. It defines lazy_rule instead.

The lazy parser code is in a file which is problem independent.

Storing the results of parsing and also the failures.

This is a comparison with the Lazy example using any_parser based on an example: 

https://stackoverflow.com/questions/60171119/boost-spirit-x3-lazy-parser

I was lead to this looking for examples of subcontext and any_parser, both of which are in X3 and lack examples

Being adapted from the variant example.

This has involved a number of adaptions to the Arduino environment which are contained in the sketch file.

Common features have been moved to ArduinoCode.h which can be used in other examples.

This file only runs on the Arduino NANO RP2040 connect.
