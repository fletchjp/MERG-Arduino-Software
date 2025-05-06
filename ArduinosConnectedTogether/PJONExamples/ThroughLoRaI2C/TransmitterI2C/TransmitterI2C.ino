// TransmitterI2C.ino
// Version for mLink LoRA

#include <PJONThroughLoraI2C.h>
/*
To use this example, please download the mLink third party Library
*/

PJONThroughLora bus(45);

void setup() {
	// Synchronous acknowledgement is not supported
  bus.set_acknowledge(false);
	// Obligatory to initialize Radio with correct frequency
	bus.strategy.setFrequency(868100000UL);
	// Optional
	bus.strategy.setSignalBandwidth(250E3);

	bus.begin();
	bus.send_repeatedly(44, "B", 1, 1000000); // Send B to device 44 every second
};

void loop() {
	bus.update();
};
