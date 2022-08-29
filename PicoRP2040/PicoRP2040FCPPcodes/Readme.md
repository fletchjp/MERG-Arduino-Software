# PicoRP2040FCPP examples

These are now in a folder of their own

Many of these are derived from previous work which is now being extended.

## PicoRP2040FCPP

Some basic tests of FC++.

I have added some of the code from the parser example as there seems to be a minimum amount of work below which I get no moditor output.

## PicoRP2040FCPPParser

This example is based on the old FC++ parser.cpp example. I have it mostly working.

There is a paper "Functional Pearls Monadic Parsing in Haskell" Graham Hutton and Erik Meijer.

This was I think the basis for the parser.cpp example. 

## PicoRP2040FCPPParser2

This is to extend the previous example and hopefully sort out some problems.

I have made quite a lot of progress with this and have hit a snag I cannot at the moment solve.

There are two operators **many** and **many1** which are part of the parser. I cannot get examples of use to compile.

I did earlier have a problem which I solved by replacing some **typedef** statements by **using** statements.

The problem may be part of the move to gcc10 and C++17 which I am using on the RP2040.

## PicoRP2040FCPPParser3

This is for new ideas on how to develop ParserM with a Parser object with type std::pair<A,StringL>

## PicoRP2040FCPPParser4

This is for new ideas on how to develop ParserM.

This has ParserL and ParserLM with the object type std::pair<List<A>,StringL>

This gives more options for some of the operations. I am going to develop both of them.

## PicoRP2040FCPPUNIFY

Based on my example unify.cpp bringing together Monad and Functional codes.

This has several more headers from the fcpp (FC++) library which are not in Github.

This example now has a number of new functors. 

I want to add them to the Arduino copy of the fcpp library. I now have a copy of that in Github so I can make changes there and copy them to the working version.

## PicoRP2040FCPPCategory

This example is based on the code in category_fcpp.cpp and had links with the unify.cpp code as well.

There is a lot of experimental code in this example.

These examples link up a lot of work which I have done over the years to implement various ideas about Monads and Functors.

The usual implementations of these are in Haskell and I am using FC++ as a way of translating to C++.

The work is easier now with modern C++17 than it was when the original FC++ work was done.

I want to add some experimental Monoid implementation ideas.

This code now has examples of Monoid implementation including Mstream and Mlist<T>.

This now includes the capability to infer the Monoid type when this is implemented.

This example is becoming a location for bringing together various definitions and ways of looking at things.

## PicoRP2040FCPPMonoids

This example has all of the monoid examples from PicoRP2040FCPPCategory

This includes the examples of Endo endomorphic operations.

I am going to move more definitions to fcpp/monoids.h to make them available for work in other examples.
