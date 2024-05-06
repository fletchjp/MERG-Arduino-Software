/*
  BoardDefinitions.h

  Tells the application how to implement certain functions based on Arduino hardware type.
*/

#ifndef _BoardDefinitions_HEADER_H
#define _BoardDefinitions_HEADER_H


// Access to the standard types and constants of the Arduino language (this is automatically added to normal sketches, but not to libraries)
#include <Arduino.h>

// Here are the CHOICES you can make

// Choose this option (or not) for DUE
// #define BOARD_DUE_USE_SerialUSB // For DUE (optionally use the NATIVE USB port for logging program output.
// For a mature application this is better, because it allows connection of USB mid-execution without triggering a program restart.

// Board hardware. Automatically determined

#if defined(__AVR_ATmega2560__)
    #define BOARD_MEGA2560
    // #warning "Board being used is the MEGA2560"
    #define INT_MAX 0x7FFF
#elif defined(__SAM3X8E__) // AT91SAM3X8E
    #define BOARD_DUE
    #define INT_MAX 0x7FFFFFFFL
    // #warning "Board being used is the DUE AT91SAM3X8E"
#else
    #error "Board not correctly defined in BoardDefinitions.h. Check you selected Tools->Board correctly!"
#endif



// Don't mess with anything BELOW, they are all dependent on things you determined ABOVE

#if defined(BOARD_DUE) && defined(BOARD_DUE_USE_SerialUSB)
    #define SERIALOUT SerialUSB
//    #define SERIAL_WAIT_AT_START
#else
    #define SERIALOUT Serial
#endif

// Things for DUE
#ifdef BOARD_DUE
    #define BOARD_RAM_START 0x20070000 // See section 7.2.1 in Arduino Due Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf
    #define BOARD_RAM_END   0x20087FFF // See section 7.2.1 in Arduino Due Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf
#endif

// Things for non-DUE, i.e. MEGA2560 etc.
#ifndef BOARD_DUE
    #define BOARD_HAS_EEPROM  // Not for DUE!
    #define BOARD_USE_PROGMEM // Not for DUE!
    #define BOARD_USE_2515_CANBUS_VIA_SPI // Not for DUE!
#endif

#define BOARD_LITTLE_ENDIAN // LSB first
// #define BOARD_BIG_ENDIAN // MSB first

#endif
