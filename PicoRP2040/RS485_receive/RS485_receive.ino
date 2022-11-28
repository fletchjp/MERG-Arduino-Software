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

const uint LED_PIN = 25;

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

void setup() {
   uint8_t flag = 1;
   //stdio_init_all();
   uart_init(UART_ID,115200);
   gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
   gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
   uart_set_baudrate(UART_ID, BAUD_RATE);
   uart_set_hw_flow(UART_ID, false, false);
   uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
   uart_set_fifo_enabled(UART_ID, true);
   uart_puts(UART_ID, "RS485 receive test\r\n");

   gpio_init(LED_PIN);
   gpio_set_dir(LED_PIN, GPIO_OUT);
}

// The flashing works
void loop() {
   gpio_put(LED_PIN, 1);
   sleep_ms(500);
   gpio_put(LED_PIN, 0);
   sleep_ms(500);
}
