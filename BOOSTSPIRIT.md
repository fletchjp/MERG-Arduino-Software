# Boost Spirit pages

I am planning to use Boost Spirit for the parsing of input such as the CANCOMPUTE input code.

It is also capable of generating strings from structured data and so could be used to pass strings over CBUS.

Link to [RP2040 page](RP2040.md)

## Boost Spirit Examples

This is becoming an important area of work as it is possible that Boost Spirit could be used to decode CANCOMPUTE files with possible enhancements.

It can be used both to decode a string to a structure set of values and also used to generate strings from values. This means that it could be used to develop ways to send information over CBUS using RFC 0005 methods.

### ArduinoRP2040BoostSpiritExample

This example is now working as an example of Boost Spirit Qi. It is planned to extend to Boost Spirit Karma

### ArduinoRP2040BoostSpiritNumList1

An implementation of the Boost Spirit Qi example num_list1 expanded to include Karma num_list1 as well.

### ArduinoRP2040BoostSpiritNumList2

An implementation of the Boost Spirit Qi example num_list2

### ArduinoRP2040BoostSpiritNumList3

An implementation of the Boost Spirit Qi example num_list3 with simplification.

### ArduinoRP2040BoostSpiritNumList4

An implementation of the Boost Spirit Qi example num_list4 with further simplification.

### ArduinoRP2040BoostSpiritActions

An exploration of some ways of using Boost Spirit

### ArduinoRP2040BoostSpiritKeyValue

This example enables decoding of Key1=Value1;Key2=Value2 pairs

### ArduinoRP2040BoostSpiritKeyNode

This is an adaption towards  Key=Node for CANCOMPUTE decoding

### ArduinoRP2040BoostSpiritKeyNodeSpace

I have added code to allow for blank spaces in the definitions  Key1 = Node1; etc.

This involved finding a file called skipper.hpp in the Qi examples.

I think this is an example of how parser components can be organised in separate files.

## Boost Spirit X3 (Version 3)

I did not think that this was available with BoostForArduino (Boost 1.66.0).

It turns out that it is with an early version 3.0.0.

I have had some difficulty finding why it did not compile and sorting it out so that it now does.

I am going to explore it in parallel with version 2 which I have been using up until now.

I understand that one of the differences is that it no longer uses Boost Phoenix and Boost Proto.

I have listened to the beginning of a talk by Joel De Guzman when he was starting on X3. Interesting.

There was due to be a version of karma for X3 which does not exist yet.

### ArduinoRP2040BoostSpiritX3Actions

An exploration of some ways of using Boost Spirit X3 in comparison with the other version of this file.

## FC++ Examples

I have brought in two existing examples **DUEFCPP** and **DUEMONAD**.

These examples use FC++ instead of BoostFC++ as there is much more done there.

I have made an Arduino library called **fcpp** which will run with these examples instead of them having their own copies.

### ArduinoRP2040FCPP

This is renamed from the existing examples **DUEFCPP**

### ArduinoRP2040MONAD
 
This is renamed from the existing examples **DUEMONAD**

### ArduinoRP2040FCPPFunctionTypes

This is copied from the member_function_types example. The examples all work and I have also added comparisons with callable_traits.

## Back to RP2040

[RP2040 page](RP2040.md)
