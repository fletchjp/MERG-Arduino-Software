# DUESignalCombined

This is developing SignalCombined to use the FC++ available in DUEobserver2.

The reason for doing this is to improve the way in which events can be handled.

In SignalCombined this is done through an EventHandler class which uses CRTP.

I think this can be eliminated using FC++.

Initial tests with FC++ are now working using the simple Observer pattern code.

This has now been extended so that the event handling is now independent of the type.

This can be done because the map now holds a pointer to function which is independent of the type of object.

I have done this on the simpler version - it could now be extended to the combined version.

This will enable the parents of an object to become observers automatically if that is desired.

