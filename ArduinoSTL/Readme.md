# ArduinoSTL examples

Some examples using the ArduinoSTL library.

## ArduinoSTLHello

Basic example

## ArduinoSTLIntegers

## ArduinoSTLVector

Simple vector example with range based for loop example.

## ArduinoSTLComposite

Example of the Composite Pattern which is now working.

## ArduinoSTLComposite2

Example of the Composite Pattern with the composite classes in a header.

## ArduinoSTLComposite3

Example of the Composite Pattern with the composite classes in a header.

Using list instead of vector for the children.

## ArduinoSTLComposite4

The code has been changed to use a different example from refactoring.guru.

This supports holding parent information (on one parent).

I have retained the support of named items and added GetName to return the information.

## ArduinoSTLComposite5

This extends the previous example to allow for multiple parents.

I have now used this as a starting point for SignalComposite.

## ArduinoSTLObserver

Implementation of the Observer Pattern from previous code.

I want to explore combining Observer and Composite.

## ArduinoSTLObserver2

I am exploring another simpler implementation of the Observer Pattern.

The example source is not complete code and there are also alternative suggestions.

This compiles without a working example.
 
## ArduinoSTLObserver3

Alternative suggestions.

It is not possible to implement some features of this proposal on the ArduinoSTL system.

There are several ways to approach this.

1. Attempt to work around within this proposal e.g use Boost function instead of std::function.
   I have tried this. There are problems with both of the Boost versions I have.
2. Merge the two ways together avoiding the features which are not supported.
3. Move this idea to another system e.g. Raspberry Pi PICO.

## ArduinoSTLObserver4

Attempt to work with Boost. This has failed as I cannot sort out the clashes between libraries.

## ArduinoSTLObserver5

I am going instead to work around things for the ARM boards. This is looking more promising.


