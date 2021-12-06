# Boost Spirit X3 (Version 3) calculation examples

Back to [Boost Spirit X3 examples](BOOSTSPIRITX3.md)

There are a number of examples in Boost Spirit X3 Version 3.0.9 which develop a method for doing calculation.

I have adapted a number of theses examples to work towards the work I want to do for MERG.

The AST (Abstract Syntax Tree) approach is important as it allows the syntax of the problem to be defined separately from other parts of the code.

This means that the larger programs are in many different files. Doxygen documentation is useful for keeping track of theses.

Some things which are easy in one file get more difficult in many files as the definitions need to be consistent between files. The importance of this will become clear.

### ArduinoRP2040BoostSpiritCalc4c

A Calculator example demonstrating generation of AST (Abstract Syntax Tree).

The AST, once created, is traversed, 

1) To print its contents and

2) To evaluate the result.

### ArduinoRP2040BoostSpiritCalc8

This is taken from spirit/example/x3/calc/calc8 from Boost 1.77.0
and adapted to run on the Arduino NANO RP2040 connect.

This runs with correct input and crashes using an undefined variable.

This lead me to discover the use of expectation (>) instead of sequence (>>) to signal that if there is no match there should be a failure.

The immediate action was to replace (>) with (>>) to avoid the crashes.

### ArduinoRP2040BoostSpiritCalc8c

This is an attempt to combine the approach in Custom3 to diagnose incorrect input and allow for expectation.

I thought this would be straightforward and this has not been so. I needed to learn a lot about how the coding works for the context when the code is over several files.

One result of this is a code in custom.h which should be portable to different problems.

Another result is a lot more knowledge about how Spirit X3 works.

It turns out that the definition of the context type can only have two or three template arguments. This is defined in x3\support\context.hpp.

It there are more than three arguments in an attempt to include more than one with<> operator in the parser, then the definition must be split into stages, passing on the previous defintion.

This is how it is arranged in this example.

I still need to do some work to get the debug output working properly.

Back to [Boost Spirit X3 examples](BOOSTSPIRITX3.md)

