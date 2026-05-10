/*
 * timer_blink_print
 *
 * Blinks the built-in LED every half second, and prints a messages every
 * second using the arduino-timer library.
 *
 * Adding more calls to the library.
 */

#include <arduino-timer.h>

auto timer = timer_create_default(); // create a timer with default settings

bool toggle_led(void *) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle the LED
  return true; // repeat? true
}

bool cancel_tasks(void *) {
  Serial.print("cancel tasks: Called at: ");
  Serial.println(millis());
  timer.cancel();
  no_of_tasks();
  return false;
}

bool print_message(void *) {
  Serial.print("print_message: Called at: ");
  Serial.println(millis());
  return true; // repeat? true
}

void no_of_tasks() {
  Serial.print("The number of tasks is ");
  Serial.println(timer.size());
  if(timer.empty()) Serial.println("The timer is empty");
}

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
  Serial.println("Blink and print tests of arduino-timer");
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
  pinMode(LED_BUILTIN, OUTPUT); // set LED pin to OUTPUT
  printVersionInfo();
  Serial.print("C++ version is ");
  Serial.println(__cplusplus);

  // call the toggle_led function every 500 millis (half second)
  timer.every(500, toggle_led);

  // call the print_message function every 1000 millis (1 second)
  timer.every(1000, print_message);

  timer.at(10000,cancel_tasks); 

  no_of_tasks();
}

void loop() {
  timer.tick(); // tick the timer
}
