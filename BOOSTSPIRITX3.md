# Boost Spirit X3 (Version 3)

I did not think that this was available with BoostForArduino (Boost 1.66.0).

It turns out that it is with an early version 3.0.0.

I have had some difficulty finding why it did not compile and sorting it out so that it now does.

I am going to explore it in parallel with version 2 which I have been using up until now.

I understand that one of the differences is that it no longer uses Boost Phoenix and Boost Proto.

I have listened to the beginning of a talk by Joel De Guzman when he was starting on X3. Interesting.

There was due to be a version of karma for X3 which does not exist yet.

I am working through the Boost Spirit X3 examples. These take the user through a series of stages which introduce the features of X3 in an organised way.

I have now solved a propblem of some warning messages by adding -Wno-pasabi to the cpp flags.

### ArduinoRP2040BoostSpiritX3Actions

An exploration of some ways of using Boost Spirit X3 in comparison with the other version of this file.

### ArduinoRP2040BoostSpiritX3NumList1

Taken from spirit/example/x3/num_list/num_list1

These examples do the same example introducing features one by one.

### ArduinoRP2040BoostSpiritX3NumList2

Taken from spirit/example/x3/num_list/num_list2

From this point on the results are stored in a vector.

### ArduinoRP2040BoostSpiritX3NumList3

Taken from spirit/example/x3/num_list/num_list3

### ArduinoRP2040BoostSpiritX3NumList4

Taken from spirit/example/x3/num_list/num_list4

### ArduinoRP2040BoostSpiritX3Roman

Taken from spirit/example/x3/Roman

This shows how to combine parsers into a more complex operations.

### ArduinoRP2040BoostSpiritX3Employee

Taken from spirit/example/x3/employee

This shows how to input data to a structure.

I have not been able to use the boost Fusion output option and have made my own output routine.

I have been able to test using Boost Fusion access routines.

### ArduinoRP2040BoostSpiritX3Minimal

This is a version of the employee example broken into separate files for the different parts of the problem.

The idea is that they can be considered separately and may be components of different problems.

This example is not in Boost 1.66.0. I have taken it from Boost 1.77.0.

It was not difficult to adapt using code from the employee example.

### ArduinoRP2040BoostSpiritX3Annotation

This extends the employee example using annotation to add access iterators.

I have not succeeded in getting the new features to work properly.

### ArduinoRP2040BoostSpiritX3Annotation2

This extends the Annotation example to add an employee number.

### ArduinoRP2040BoostSpiritX3Annotation3

This extends the Annotation example to add an entry structure with number separate.

## Back to Boost Spirit and RP2040

[Boost Spirit page](BOOSTSPIRIT.md) | [RP2040 page](RP2040.md)
