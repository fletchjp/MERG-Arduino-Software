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



### ArduinoRP2040BoostSpiritCalc8c

This is an attempt to combine the approach in Custom3 to diagnose incorrect input.

Back to [Boost Spirit X3 examples](BOOSTSPIRITX3.md)

