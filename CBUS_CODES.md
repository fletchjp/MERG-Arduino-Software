 <img align="right" src="arduino_cbus_logo.png"  width="150" height="75">

# CBUS Codes

This is a branch for the location of CBUS codes, some of which will be related to the codes in the main branch.

I will not modify the other Markdown files in the branch, keeping information in this and other new files.

## Codes

### CANASIGNALI2C_CS15

This is derived from work using I2C to provide a PWM driven signal controlled by an IR detector. I have modified this as a first test of the use of Pin 15 instead of Pin 10 for the chip select function needed for SCI in order to run the Arduino CBUS library. So far I have only tested it as an isolated module and not yet attempted to connect it to anything else.

The reason for this experiment is that Pin 10 is not available for use with the 1602 display with buttons when used as a shield rather than an I2C item.

