# TestBetterEnum

@mainpage TestBetterEnum

This tests the Better Enum code and other enum ideas.

## BETTER_ENUM

BETTER_ENUM is a macro which sets up enum classes (scoped enum)

https://hackaday.com/2017/05/05/using-modern-c-techniques-with-arduino/

It does some things neatly but not comparison which has to be cast.

## Anonymous namespace

I have managed to get Doxygen to document the anonymous namespace.

This required a change in the configuration file to set 

EXTRACT_ANON_NSPACES   = YES

