# DUESignalCombined

This is developing SignalCombined to use the FC++ available in DUEobserver2.

The reason for doing this is to improve the way in which events can be handled.

In SignalCombined this is done through an EventHandler class which uses CRTP.

I think this can be eliminated using FC++.

Initial tests with FC++ are now working using the simple Observer pattern code.
