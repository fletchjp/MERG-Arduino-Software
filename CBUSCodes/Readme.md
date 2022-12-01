# CBUSCodes

Codes for CBUS on various different technologies.

## Arduino AVR

There are a lot of codes, mainly using the Arduino CBUS Libraries.

## Arduino DUE

I have some codes I have been using for some time.

## RP2040

I am working on several different ways of connecting a Raspberry Pi Pico to CBUS.

I have found that it is a BAD idea to use   while(!Serial) on the RP2040 codes as the Serial connection is disrupted by this.

Thanks to Duncan Greenwood who has sent me codes to try out.

### CBUS_empty_Pico

These codes are an attempt to use CBUS2515 with the CAN Hat and a Pico.

The codes use external EEPROM.  At the moment there is a bug in the latest version.

### CBUSMCPCAN_example

These codes use CBUSMCPCAN instead of CBUS2515.

