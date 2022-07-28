# Pico RP2040

This folder is to work with files for the Pico RP2040 using the Arduino-pico system

I have found already that codes have to be distinct from the ones for the Arduino NANO RP2040 Connect as the output to the serial port has to be different.

## Examples

I will be converting some examples to work with the Pico RP2040.

I would like to be able to use the equivalent of the Arduino Streaming library with the Pico code. I have not yet found a way to do this.

### PicoRP2040FCPP

Some basic tests of FC++.

I have added some of the code from the parser example as there seems to be a minimum amount of work below which I get no moditor output.

### PicoRP2040FCPPParser

This example is based on the old FC++ parser.cpp example. I have it mostly working.

There is a paper "Functional Pearls Monadic Parsing in Haskell" Graham Hutton and Erik Meijer.

This was I think the basis for the parser.cpp example. 

### PicoRP2040FCPPParser2

This is to extend the previous example and hopefully sort out some problems.

I have made quite a lot of progress with this and have hit a snag I cannot at the moment solve.

There are two operators **many** and **many1** which are part of the parser. I cannot get examples of use to compile.

I did earlier have a problem which I solved by replacing some **typedef** statements by **using** statements.

The problem may be part of the move to gcc10 and C++17 which I am using on the RP2040.

### PicoRP2040FCPPParser3

This is for new ideas on how to develop ParserM.


