# Boost Spirit X3 (Version 3) further examples

Back to [Boost Spirit X3 examples](BOOSTSPIRITX3.md)

These examples are more adaption of the ones which have been collected from a number of sources.

### ArduinoRP2040BoostSpiritX3Rexpr

### ArduinoRP2040BoostSpiritX3RexprFull

### ArduinoRP2040BoostSpiritX3RexprAnnotation

### ArduinoRP2040BoostSpiritX3Variant

### ArduinoRP2040BoostSpiritX3Comments

### ArduinoRP2040BoostSpiritX3Commands

### ArduinoRP2040BoostSpiritX3Commands2

### ArduinoRP2040BoostSpiritX3Commands3

### ArduinoRP2040BoostSpiritX3Commands4

### ArduinoRP2040BoostSpiritCustom

This example introduces another approach to diagnostic output which I am using in several examples.

This adapts the employee example using code from
 
https://stackoverflow.com/questions/61721421/customizing-the-full-error-message-for-expectation-failures-boostspiritx3

### ArduinoRP2040BoostSpiritCustom2

Moving most of the code into a separate filecustom.hpp. The result is unchanged.

I now have the annotation of success working.

I have also added some code to look at the situation on failure.

### ArduinoRP2040BoostSpiritCustom3

This example now finds the error information for the first error without using exceptions. 

This depends on a modification to Boost Spirit X3 in file \boost\spirit\home\x3\directive\expect.hpp.
This is only active if BOOST_SPIRIT_X3_NO_EXCEPTION is defined.

This has provided a basis which I have used in Calc8c - see [Boost Spirit X3 calculation examples](BOOSTSPIRITX3Calculation.md)

Back to [Boost Spirit X3 examples](BOOSTSPIRITX3.md)





