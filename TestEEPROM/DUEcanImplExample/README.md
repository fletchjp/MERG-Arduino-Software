# DUEcanImplExample

This example has been written to test my simulation of EEPROM for the Arduino DUE implementation of OpenLCB code.

I had written a first version which did not work.

The problem is that the Arduino DUE does not have EEPROM and simulates it in Flash memory using a library DueFlashStorage.

That library lacks the member functions update, put and get which are used by the OpenLCB code so I have had to add them in.

My additional code is in the DUE/DUEcanImpl.h and DUE/DUEcanImpl.cpp files.

This example tests these functions as an instance EEPROM.

John Fletcher <M6777>
