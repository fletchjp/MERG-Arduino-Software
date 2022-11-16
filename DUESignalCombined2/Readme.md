# DUESignalCombined2

This is a further version of DUESignalCombined.

It is now using the combined.h header instead of composite.h 

///////////////////////////////////////////////////////////////////////////////  
// This combines the Composite and Observer Patterns.  
// All objects declared are Components, Subjects and Observers.  
// Components can be either Composite or Leaf.  
// Composite components can have other items (Composite or Leaf) as children using Add(&child)  
// An composite can have more than one child and any object can have more than one parent.  
// Optionally child.RegisterParents() makes the parents of the child into observers as well.  
// Any observer has to define handlers which will respond to particular integer values from   
// the subjects being observed. The handlers are member functions and they are indexed using the integer value.  
// See the examples in classes Track and Section in the code.  
// It is also possible to make other composite objects observers of any object.   
///////////////////////////////////////////////////////////////////////////////

All Components are now also Subjects and Observers.

The classes Observer and EventHandler have been combined as there was nothing much in Observer.

The use of FC++ had removed the need for EventHandler to have a template of the upper type.

This is developing SignalCombined to use the FC++ available in DUEobserver2.

The reason for doing this is to improve the way in which events can be handled.

In SignalCombined this is done through an EventHandler class which uses CRTP.

This has been eliminated using FC++. Event handling is now independent of the type.

This can be done because the map now holds a pointer to function which is independent of the type of object.

I have done this on the simpler version - it could now be extended to the combined version.

This will enable the parents of an object to become observers automatically if that is desired.

## Where next?

I need to put the headers into a library once they are stable.

I also need to put the FC++ files into a library so that only the ones needed are compiled.

