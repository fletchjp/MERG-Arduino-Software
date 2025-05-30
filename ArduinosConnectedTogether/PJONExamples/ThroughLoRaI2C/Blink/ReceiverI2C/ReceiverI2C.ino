// ReceiverI2C
// Name changed to make it easier to find.
// This now compiles.
// I cannot test it until I have checked out the transmit code.

#include <PJONThroughLoraI2C.h>

/* To use this example, please download the mLink third party Library */

PJONThroughLoraI2C bus(44);

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
	/* Make use of the payload before sending something, the buffer where payload points to is
	   overwritten when a new message is dispatched */
	if (payload[0] == 'B') {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(30);
		digitalWrite(LED_BUILTIN, LOW);
	}
};

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW); // Initialize LED 13 to be off

	mLink.init(); // Set up mLink

	// Synchronous acknowledgement is not supported
  bus.set_acknowledge(false);
	// Obligatory to initialize Radio with correct frequency
	bus.strategy.setFrequency(868100000UL);
	// Optional - mLink has its values.
	// bus.strategy.setSignalBandwidth(250E3);
	bus.begin();

	bus.set_receiver(receiver_function);
};

void loop() {
	bus.receive(1000);
};
