// Temperature

// Basic example to explore using different output options.

// I am not sure that this works at all with the Pico
//#include <Streaming.h>


/* Released into the public domain */

void setup() {
  Serial.begin(115200);
  delay(5000);
}

void loop() {
  Serial.printf("Core temperature: %2.1fC\n", analogReadTemp());
  delay(1000);
}
