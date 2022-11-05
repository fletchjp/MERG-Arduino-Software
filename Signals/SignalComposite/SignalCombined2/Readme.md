# SignalCombined2

Experiments with the Composite Design Pattern using code developed in ArduinoSTL

This now being combined with the Observer Design Pattern from ArduinoSTLObserver6

A second example exploring the possibilities.

This is based on ArduinoSTLComposite5 which supports multiple parents,

This soon ran out of memory on a UNO so I am using a MEGA instead.

I have something running which shows the building of a structure.

There are now working examples of messages being passed.

This will experiment with integration of the registration into the Composite pattern.

I have made some progress. I have not worked out how to register parents as observers without having to change the separate codes.

I have managed to make all items subjects.

I cannot make all parents into observers as that needs a CRTP template and I cannot store heterogeneous parents in a list.

That problem is not solved at the moment - observers have to be declared.

The previous examples run on this code.

If I want to get around the problem I am going to have to change the way I am doing the Eventhandler.
