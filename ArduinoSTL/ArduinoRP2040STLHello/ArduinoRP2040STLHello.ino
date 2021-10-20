//#include <ArduinoSTL.h>
#include "pico/stdio.h"
#include <vector>

void setup() {
  //Serial.begin(115200);
  //while(!Serial) { }
  // This gives a link error so it must be in a header but not a library....
  //stdio_init_all();
  //pico::stdio_usb_init();
  std::vector<int> v;
  // This does not go to Serial.
  printf("Hello World\n");
}

void loop() {
  
}
