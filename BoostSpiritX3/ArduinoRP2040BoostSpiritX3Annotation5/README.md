@mainpage

## ArduinoRP2040BoostSpiritX3Annotation5

### Example of Boost Spirit X3 Annotation

Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)

This is taken from spirit/example/x3/annotation from Boost 1.77.0
and adapted to run on the Arduino NANO RP2040 connect.

This example extends the example to save the parsed data into a map which uses the employee number as the key.

It now also checks for duplicate key values and does not enter any duplicates.

Common features have been moved to ArduinoCode.h which can be used in other examples.

This file only runs on the Arduino NANO RP2040 connect.

I have not been able to get the annotation to the original source string to work.

There is also an issue between the vector output and the map if some duplicate entries are left out.

I do not have a use at the moment for the annotation.