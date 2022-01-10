# Test Programs

These are a set of programs being used to test some C++ programming ideas.

A lot of this is in response to the realisation that the Arduino IDE default settings hide warnings.

Each program has been started to cover a topic, although they have expanded as time has gone on.

## -fpermissive (TestWarnings)

On the AVR boards (NANO, UNO and MEGA for example) the Arduino IDE default settings for the compiler include -fpermissive.

This converts a number of errors to warnings, which are then hidden.

There seems to be no list of what -fpermissive actually allows so it is necessary to hunt around to find some answers.

## switch statements (TestSwitch)

One of the problem areas with -fpermissive has been around switch statements.

In particular, local variable definitions within a switch are a source of errors which are being masked.

## scoped enums

The use of enum is something which C++ inherited from C.

Starting from C++11 there is a facility called a scoped enum or enum class which tightens up on the way an enum works.

