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

"The Arduino Due has three additional 3.3V TTL serial ports:
Serial1 on pins 19 (RX) and 18 (TX);
Serial2 on pins 17 (RX) and 16 (TX),
Serial3 on pins 15 (RX) and 14 (TX).
Pins 0 and 1 are also connected to the corresponding pins of the ATmega16U2 USB-to-TTL Serial chip,
which is connected to the USB debug port.
Additionally, there is a native USB-serial port on the SAM3X chip, SerialUSB'. "


#### TransmitterDUEResponse and ReceiverSerialDUEResponse

This pair of codes work between a pair of Arduino DUE boards connected using Serial1 (pins 18 and 19).

Unfortunately these pins are blanked but not used by the CAN shields I am planning to use with the DUE boards.

I do not know if there is any other way to get access to the alternate Serial pins. I have searched and not found anything.

PJON does not support SoftwareBitBang for the DUE at the moment.
