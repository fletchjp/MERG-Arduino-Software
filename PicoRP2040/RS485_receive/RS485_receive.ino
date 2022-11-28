// RS485_receive.ino
// Adapted from RS485CANHat
// Using code from the waveshare example
// Arduino-pico docs say that the only baud rate supported is 115200

// Exploring what is available for the Hat when attached to a Pico.

// Pinouts from https://learn.sb-components.co.uk/RS485-CAN-HAT

// GP(Pico) GPIO(RPi) Name  Function

//     2      11      SCK   SPI clock input
//     3      10      MOSI  SPI data input
//     4       9      MISO  SPI data output
//     5       8      CS    SPI chip selection

//    22      25      INT   Interrupt
//     1      15      RS485 UART receieve
//     0      14      RS485 UART transmit
//     6       4      RS485 TX/RX control line


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define LED_PIN 25

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1

#define UART_TX_PIN 0
#define UART_RX_PIN 1

void setup() {
   uint8_t flag = 1;

}

void loop() {

 
}
