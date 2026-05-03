/*
 * timer_full
 *
 * Full example using the arduino-timer library.
 * Shows:
 *  - Setting a different number of tasks with microsecond resolution
 *  - disabling a repeated function
 *  - running a function after a delay
 *  - cancelling a task
 *
 * Added a number of new things including reporting the number of tasks.
 * No changes to the library.
 */

#include <arduino-timer.h>

auto timer = timer_create_default(); // create a timer with default settings
Timer<> default_timer; // save as above

// create a timer that can hold 1 concurrent task, with microsecond resolution
// and a custom handler type of 'const char *
Timer<1, micros, const char *> u_timer;


// create a timer that holds 16 tasks, with millisecond resolution,
// and a custom handler type of 'const char *
Timer<16, millis, const char *> t_timer;

bool toggle_led(void *) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle the LED
  return true; // repeat? true
}

bool print_message(const char *m) {
  Serial.print("print_message: ");
  Serial.println(m);
  return true; // repeat? true
}

// This is how to pass a timer as an argument.
template <typename T>
void no_of_tasks(T &timer, const char* name) {
  Serial.print("The number of "); Serial.print(name); Serial.print(" tasks is ");
  Serial.println(timer.size());
  if(timer.empty()) { Serial.print(name); Serial.println(" is empty"); }
}

void no_of_timer_tasks() {
  Serial.print("The number of timer tasks is ");
  Serial.println(timer.size());
  if(timer.empty()) Serial.println("The timer is empty");
}

bool cancel_tasks(void *) {
  Serial.print("cancel tasks: Called at: ");
  Serial.println(millis());
  timer.cancel();
  no_of_tasks(timer,"timer");
  return false;
}

bool cancel_t_timer_tasks(const char *message) {
  Serial.print(message); 
  Serial.print(": Called at: ");
  Serial.println(millis());
  t_timer.cancel();
  no_of_tasks(t_timer,"t_timer");
  return false;
}


size_t repeat_count = 1;
bool repeat_x_times(void *opaque) {
  size_t limit = (size_t)opaque;

  Serial.print("repeat_x_times: ");
  Serial.print(repeat_count);
  Serial.print("/");
  Serial.println(limit);

  return ++repeat_count <= limit; // remove this task after limit reached
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
  Serial.println("Full tests of arduino-timer");
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

  pinMode(LED_BUILTIN, OUTPUT); // set LED pin to OUTPUT

  // call the toggle_led function every 500 millis (half second)
  timer.every(500, toggle_led);

  // call the repeat_x_times function every 1000 millis (1 second)
  timer.every(1000, repeat_x_times, (void *)10);

  // call the print_message function every 1000 millis (1 second),
  // passing it an argument string
  t_timer.every(1000, print_message, "called every second");

  // call the print_message function in five seconds
  t_timer.in(5000, print_message, "delayed five seconds");

  // call the print_message function at time + 10 seconds
  t_timer.at(millis() + 10000, print_message, "call at millis() + 10 seconds");

  // call the toggle_led function every 500 millis (half second)
  auto task = timer.every(500, toggle_led);
  timer.cancel(task); // this task is now cancelled, and will not run

  // call print_message in 2 seconds, but with microsecond resolution
  u_timer.in(2000000, print_message, "delayed two seconds using microseconds");

  if (!u_timer.in(5000, print_message, "never printed")) {
  /* this fails because we created u_timer with only 1 concurrent task slot */
    Serial.println("Failed to add microsecond event - timer full");
  }

  // This had to be called before the next one, as that will kill it.
  // Changed to put it on the t_timer where it needs a message. 
  t_timer.at(20000,cancel_t_timer_tasks,"cancel t_timer tasks");

  timer.at(16000,cancel_tasks);

  no_of_timer_tasks();

  no_of_tasks(timer,"timer");

  // This is private.
  //Serial.println(sizeof(timer.tasks));
  //Serial.println(timer.max_tasks);
  // This would need to be added as a public member function.
  // This only works because u_timer was made with 1 task.
  // This is the only way I can think of to do this without
  // making changes to the library code.
  Serial.print("Maximum number of tasks in timer is ");
  Serial.println(sizeof(timer)/sizeof(u_timer));

  no_of_tasks(t_timer,"t_timer");

}

void loop() {
  timer.tick(); // tick the timer
  t_timer.tick();
  u_timer.tick();
}
