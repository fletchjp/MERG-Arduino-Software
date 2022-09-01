//  dph_test_rp2040_raw
// from https://github.com/KevinOConnor/can2040

#include "can2040.h"
#include "can2040.c"

static struct can2040 cbus;

void print(struct can2040_msg *msg) {
    Serial.print(msg->id, HEX);
    Serial.print("](");
    Serial.print(msg->dlc);
    Serial.print(")");
    uint32_t i;
    for(i=0; i<msg->dlc; i++) {
      Serial.print(" ");
      Serial.print(msg->data[i], HEX);
    }
}
//static
void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
  // This routine receives all CAN messages
  // including received, transmitted, and errors
  // Also, this is called during an interrupt, so should be minimal.  
   Serial.print("\nRcvd: [");
   print(msg);
}

//static 
void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

static void canbus_setup(void)
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 125000;
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0_IRQn, PIOx_IRQHandler);
    NVIC_SetPriority(PIO0_IRQ_0_IRQn, 1);
    NVIC_EnableIRQ(PIO0_IRQ_0_IRQn);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

// sample message, this one is for OpenLCB, and the top bit is set to make it an extended header.  
// static struct can2040_msg msg0 = {0x995B4001, 8, 1,2,3,4,5,6,7,0};

// trial MERG message: ASON CANID=1(needs to be 4-bytes), NBYTES=4, NN=1, EN=3
// Hope this works!  
static struct can2040_msg msg0 = {0x00000301, 4, 0x00,0x01,0x00,0x3};


void setup() {
  // put your setup code here, to run once:
  while(!Serial) {}
   Serial.begin(115200);
   Serial.print("\n\nvirtualRP2040CANexample\n");
#ifdef ARDUINO_PICO_VERSION_STR
   Serial.printf("ARDUINO_PICO_VERSION_STR ");
   Serial.println(ARDUINO_PICO_VERSION_STR);
#endif
   canbus_setup();
   print(&msg0);   // see if the assignment worked.  
}

void loop() {
  delay(2000);
  msg0.data[3]++;             // change the the EN for ducks
  Serial.print("\nSend: [");
  print(&msg0);
  int z = can2040_transmit(&cbus, &msg0);
}
