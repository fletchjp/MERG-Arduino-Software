# DUESignalCombined2

This is a further version of DUESignalCombined.

It is now using the combined.h header instead of composite.h 

All Components are now also Subjects.

The classes Observer and EventHandler have been combined as there was nothing much in Observer.

The use of FC++ had removed the need for EventHandler to have a template of the upper type.

This is developing SignalCombined to use the FC++ available in DUEobserver2.

The reason for doing this is to improve the way in which events can be handled.

In SignalCombined this is done through an EventHandler class which uses CRTP.

This has been eliminated using FC++. Event handling is now independent of the type.

This can be done because the map now holds a pointer to function which is independent of the type of object.

I have done this on the simpler version - it could now be extended to the combined version.

This will enable the parents of an object to become observers automatically if that is desired.
