@mainpage

## ArduinoRP2040BoostSpiritX3Commands

### Example of Boost Spirit X3 Commands

Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)

I have now upgraded to Boost Spirit 3.0.9 which is included in the library copy and being used with only a couple of minor things needed.

This is extended from this example for comments:
https://stackoverflow.com/questions/65614720/boost-spirit-x3-tokenizer-with-annotation-does-not-work
and adapted to run on the Arduino NANO RP2040 connect.

I have now changed from using std::variant to x3::variant in this code.

I am developing parsing experiments.

I am attempting to combine the rules from comments with the rules from Rexpr which are done differently.

So far I have been able to implement person which does now parse properly.

I have now also been able to set up printing of the parsed data for a person.

This has involved a number of adaptions to the Arduino environment which are contained in the sketch file.

Common features have been moved to ArduinoCode.h which can be used in other examples.

This file only runs on the Arduino NANO RP2040 connect.
