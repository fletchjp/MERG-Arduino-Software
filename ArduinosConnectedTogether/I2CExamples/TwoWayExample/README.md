# Arduinos Connected together using I2C

## Two Way Example

This is a new set of codes to explore two way communication using I2C between two Arduinos.

I have example code from /// Adapted using information from this page

https://www.instructables.com/Aduino-IC-2-Way-Communication/

which may have some problems.

I have put together a first version of this code.

I now have initial ideas working.

I now have the code working to send information in both directions.

### Integration with TaskManagerIO

I would like the controller code to be integrated with TaskManagerIO so that I can use it with other codes.

As usual there is some information on the pages for the libraries without a specific example.

### Why develop this code?

I would like to use PJON to communicate around the network.

Unfortunately it is not possible to use this with code running TaskManagerIO.

I want to have I2C code running in TaskManagerIO which will exchange information with a peripheral Arduino running both I2C and PJON.

The first stage for that is to have two way communication using I2C.

All the previous examples only work in one direction.
