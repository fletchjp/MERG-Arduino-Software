@mainpage

## ArduinoRP2040BoostSpiritX3Commands3

### Example of Boost Spirit X3 Commands3

Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)

I have now upgraded to Boost Spirit 3.0.9 which is included in the library copy and being used with only a couple of minor things needed.

This is extended from this example for comments:
https://stackoverflow.com/questions/65614720/boost-spirit-x3-tokenizer-with-annotation-does-not-work
and adapted to run on the Arduino NANO RP2040 connect.

I have now changed from using std::variant to x3::variant in this code.

I am starting a third example to extend the command processing to cover "when" commands.

I will start with a simple command and extend it to cover recursive operation.

I have been developing the enums needed for the various cases to be parsed.

There is also work to be done to store the resulting structures.

I have now also been able to set up printing of the parsed data for a person. This includes discarding the spaces after the keyword using omit[+space]

I have now extended this to parse the event information from define statements and store the results in a map indexed on the name,

I have now reached the next stage of decoding a simple when statement.

More work is needed to cover the cases with multiple terms.

This file only runs on the Arduino NANO RP2040 connect.
