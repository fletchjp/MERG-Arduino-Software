// timedBlink.ino
// I have the same code running under PlatformIO.
  
/*
Timed blink, this example shows how to build the simple Blink application using both IoAbstraction
and TaskManager. This gives an example of how quickly a simple application can be made to leverage
this library.

Because this example uses IoAbstraction, the LED could be on the end of an i2c expander or even
on a shift register. Further, if you then needed a second timed action, it would be trivial to
add.

This example runs unchanged on a Pico with no need to specify the LED>

*/

#include <Wire.h>
#include<IoAbstraction.h>
#include<TaskManagerIO.h>

// constant for the pin we will use
const int ledPin = LED_BUILTIN;

// the state of the pin, we will toggle it.
int ledOn = LOW;

// create an IO abstraction, so later we could put the led on a shift register or i2c.
IoAbstractionRef ioDevice = ioUsingArduino();

//
// A function to print information about the file being compiled.
//
void printVersionInfo(void) {
  Serial.println(__FILE__);
  Serial.print("Compiled on ");
  Serial.print(__DATE__);
  Serial.print(" at ");
  Serial.print(__TIME__);
  Serial.print(" with IDE version ");
  Serial.println(ARDUINO);
}

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("timed blink and print tests using TaskManagerIO");
#ifdef ARDUINO_ARCH_RP2040
    Serial.println("Running tests on a Pico");
#endif
#ifdef __AVR_ATmega2560__
    Serial.println("Running tests on a MEGA 2560");
#endif
#ifdef ARDUINO_MINIMA
    Serial.println("\nArduino R4 Minima");
#endif
#ifdef ARDUINO_UNOR4_WIFI
    Serial.println("\nArduino R4 Wifi");
#endif
  printVersionInfo();
  Serial.print("C++ version is ");
  Serial.println(__cplusplus);
	// set the pin we are to use as output using the io abstraction
	ioDevicePinMode(ioDevice, ledPin, OUTPUT);

	// and create the task that toggles the led every second.
	taskManager.scheduleFixedRate(1000, toggle);
}

// this is the call back method that gets called once a second
// from the schedule above.
void toggle() {
	// now we write to the device, the 'S' version of the method automatically syncs.
	ioDeviceDigitalWriteS(ioDevice, ledPin, ledOn);

	ledOn = !ledOn; // toggle the LED state.
}


void loop() {
	// this is all we should do in loop when using task manager.
	taskManager.runLoop();
}
