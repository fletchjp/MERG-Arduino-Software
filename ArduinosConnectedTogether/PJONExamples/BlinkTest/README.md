# BlinkTest

Basic examples using the Serial port

## UNO

The problem for me is that on a UNO I also use the Serial port to see what is going on.


## SoftwareSerial

The alternative is to use other pins and SoftwareSerial

https://arduinojson.org/v6/how-to/do-serial-communication-between-two-boards/

I now have this working sending a PJON message.

## Other Boards

UNO boards only have one Serial port so I can use some other things which have more ports.

https://www.arduino.cc/en/reference/serial

The MEGA has three more serial ports

### DUE

The DUE has three more ports which are 3.3V ports.

#### TransmitterDUEResponse and ReceiverSerialDUEResponse

This pair of codes work between a pair of Arduino DUE boards connected using Serial1 (pins 18 and 19).

Unfortunately these pins are blanked but not used by the CAN shields I am planning to use with the DUE boards.

I do not know if there is any other way to get access to the alternate Serial pins.

PJON does not support SoftwareBitBang for the DUE at the moment.
