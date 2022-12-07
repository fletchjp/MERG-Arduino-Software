# CBUSCodes

Codes for CBUS on various different technologies.

## Arduino AVR

There are a lot of codes, mainly using the Arduino CBUS Libraries.

## Arduino DUE

I have some codes I have been using for some time.

I have copied some CANDUE codes from the due_cbus branch to have them available here for comparison.

## RP2040

I am working on several different ways of connecting a Raspberry Pi Pico to CBUS.

I have found that it is a BAD idea to use   while(!Serial) on the RP2040 codes as the Serial connection is disrupted by this.

Thanks to Duncan Greenwood who has sent me codes to try out.

### CBUS_empty_Pico

These codes are an attempt to use CBUS2515 with the CAN Hat and a Pico.

The codes use external EEPROM.  At the moment there is a bug in the latest version.

### CBUSMCPCAN_example

These codes use CBUSMCPCAN instead of CBUS2515.

There is now the option for external EEPROM in this one as well (example3).

### CBUSACAN2040 examples

I am starting a new set of examples based on the CBUSACAN2040 library.

This uses different hardware and software.

### CBUS_1in1out_ACAN2040_v2

This has the code for the external EEPROM and setting up with the FCU using the serial monitor.
