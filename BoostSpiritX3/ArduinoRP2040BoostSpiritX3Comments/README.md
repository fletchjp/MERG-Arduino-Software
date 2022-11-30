@mainpage

## ArduinoRP2040BoostSpiritX3Comments

### Example of Boost Spirit X3 Comments

Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)

I have now upgraded to Boost Spirit 3.0.9 which is included in the library copy and being used with only a couple of minor things needed.

This is taken from 
https://stackoverflow.com/questions/65614720/boost-spirit-x3-tokenizer-with-annotation-does-not-work
and adapted to run on the Arduino NANO RP2040 connect.

This is a significant development as it is the first example where I have been able to get annotation to work. The code is rather different from the examples which come with Spirit X3, something which I need to investigate.

I think this example provides a good starting point for further experiments.

I have now changed from using std::variant to x3::variant in this code.

This has involved a number of adaptions to the Arduino environment which are contained in the sketch file.

Common features have been moved to ArduinoCode.h which can be used in other examples.

This file only runs on the Arduino NANO RP2040 connect.