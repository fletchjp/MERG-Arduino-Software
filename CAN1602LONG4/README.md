# CAN1602LONG4

@mainpage CAN1602LONG4

The version here uses a new version of the Arduino CBUS library which is not yet released.

## Long message example with pins and a display

This code has been developed to make more use of the TaskManger facilities to control the sending of a long message.

This code supports a shield with buttons and a display with 2 rows of 16 characters.

It has been programmed so that the buttons send a long string message.

This is now being changes to restructure the task management

The display shows a received long string message.

The code sends on one stream ID and listens on several others.

The choice of stream IDs is made within the program.
