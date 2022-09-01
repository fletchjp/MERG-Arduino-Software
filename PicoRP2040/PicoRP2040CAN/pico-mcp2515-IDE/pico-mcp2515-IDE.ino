// pico-mcp2515-IDE
// Arduino IDE example using pico-mcp2515 library.

#include <stdio.h>
#include "pico/stdlib.h"
#include "mcp2515.h"

MCP2515 can0;
struct can_frame rx;

void setup() {
  // put your setup code here, to run once:
   // stdio_init_all();
  while(!Serial) {}
   Serial.begin(115200);
   Serial.print("\n\npico-mcp2515-IDE\n");
#ifdef ARDUINO_PICO_VERSION_STR
   Serial.printf("ARDUINO_PICO_VERSION_STR ");
   Serial.println(ARDUINO_PICO_VERSION_STR);
#endif

    //Initialize interface
    can0.reset();
    can0.setBitrate(CAN_1000KBPS, MCP_16MHZ);
    can0.setNormalMode();

}

void loop() {
  // put your main code here, to run repeatedly:
       if(can0.readMessage(&rx) == MCP2515::ERROR_OK) {
            printf("New frame from ID: %10x\n", rx.can_id);
        }
 
}
