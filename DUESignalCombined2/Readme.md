# DUESignalCombined2

This is a further version of DUESignalCombined.

It is now using the combined.h header instead of composite.h 

At the moment there are only simple examples to make sure that all of the operations are working.

These codes are configured to use an Arduino DUE at the moment. That is where I have the FC++ code I am using available. 
I think that it is possible to port this to a Raspberry Pi Pico. 

///////////////////////////////////////////////////////////////////////////////  
// This combines the Composite and Observer Patterns.  
// All objects declared are Components, Subjects and Observers.  
// Components can be either Composite or Leaf.  
// Composite components can have other items (Composite or Leaf) as children using Add(&child)  
// Any composite can have more than one child and any object can have more than one parent.  
// Optionally child.RegisterParents() makes the parents of the child into observers as well.  
// Any observer has to define handlers which will respond to particular integer values from   
// the subjects being observed. The handlers are member functions and they are indexed using the integer value.  
// See the examples in classes Track and Section in the code.  
// It is also possible to make other composite objects observers of any object.   
///////////////////////////////////////////////////////////////////////////////

All Components are now also Subjects and Observers.  
The classes Observer and EventHandler have been combined as there was nothing much in Observer.  
The use of FC++ has removed the need for EventHandler to have a template of the upper type.  
This is developing SignalCombined to use the FC++ available in DUEobserver2.  

The reason for doing this is to improve the way in which events can be handled.  
In SignalCombined this is done through an EventHandler class which uses CRTP.  
This has been eliminated using FC++. Event handling is now independent of the type.  
This can be done because the map now holds a pointer to function which is independent of the type of object.  
This will enable the parents of an object to become observers automatically if that is desired.

## Where next?

I need to put the headers into a library once they are stable. These have changed from the ones before I used FC++.

I also need to put the FC++ files into a library so that only the ones needed are compiled.

## Usability

At the moment the users have to make the handler code, which is reasonable as that is a user function.  
There is also a need to choose values for the event numbers which are used to index the actions and trigger the notification.  
Those need to be chosen so that there are no duplicates in any Subjct or Observer.  
Finally, there is a need to put the handlers into the index in each Observer class.  
This is done in the constructor for the class.  
What is inserted is a pointer to the member function in the class. 

      auto p =  fcpp::curry( fcpp::ptr_to_fun(&Section::turnON), this);  
      handlers[TURN_ON] = p;  

Here TURN_ON has already been declared as a global const int value.  
This is the only location where the user has to use FC++ code.  
This could be made into a macro to hide the details.
