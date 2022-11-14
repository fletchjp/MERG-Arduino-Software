# DUEobserver2

This example has been adapted from observer and DUEobserver.

It is significant in that some problems with the previous versions have been solved.

Also this example now uses ptr_to_fun to store member functions which can be called to get values from a subject object.

This has been sorted out so that Subjects can be seen by more than one Observer, which can observe more than one Subject.

Event numbers are used to index in both Observers and Subjects so should be unique for each. 

The best thing is to have distinct event nos for all the events in a case.
