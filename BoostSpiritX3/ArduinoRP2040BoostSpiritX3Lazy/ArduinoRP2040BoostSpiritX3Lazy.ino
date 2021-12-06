/// @file ArduinoRP2040BoostSpiritX3Lazy.ino
/// @brief Example of Boost Spirit X3 Lazy
///
/// Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)
///
/// This is based on an example: 
///
/// https://stackoverflow.com/questions/60171119/boost-spirit-x3-lazy-parser
///
/// I was lead to this looking for examples of subcontext and any_parser, both of which are in X3 and lack examples.
///
/// I  having now provided an output operator for an x3::variant.
///
/// There is an example in variantcode.h which does not compile in the main code.
///
/// boost::variant does have one provided all the types support operator<< for a stream.
/// I have used that with strstream to output to Serial.
/// Note the ends to terminate the string.
/// The same for x3::variant produces no output.
/// For any x3::variant v, v.get() returns the boost::variant.
///
/// This has involved a number of adaptions to the Arduino environment.
///
/// X3 Variant is based on Boost Variant
///
/// I have included some simple examples of the recursive use of boost::variant types.
///
//////////////////////////////////////////////////////////
/// x3 variant example
/// Definition code now in variantCode.h
//////////////////////////////////////////////////////////


// 3rd party libraries
#include <Streaming.h>

#undef F
#undef min

#include <exception>
#include <stdexcept>
#include <iostream> 
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>

#include "ArduinoCode.h"

#include <boost_spirit_home_x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include "any_parser.hpp"

void setup() {
  /// put your setup code here, to run once:
  Serial.begin (115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 10000));
  t2 = millis() - t1;
  Serial.print("Waited for ");
  Serial.print(t2);
  Serial.println(" millis");
  while (!delay_without_delaying(10000) ) { };
  Serial << "ArduinoRP2040BoostSpiritX3Lazy ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit X3 lazy parser operations" << endl;
 
  run_lazy_example();
  
  Serial << "------------------------------" << endl;
  while (!delay_without_delaying(10000) ) { };
  Serial << "------------------------------" << endl;
  pinMode(LED_BUILTIN, OUTPUT);
}

//////////////////////////////////////////////////////////

int LEDstate = 0;

void loop() {
  /// put your main code here, to run repeatedly:
  if (delay_without_delaying(1000)) {
    LEDstate = !LEDstate;
    digitalWrite(LED_BUILTIN, LEDstate);
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    if (LEDstate) {
      Serial.println("Arduino blink ON");
    } else {
      Serial.println("Arduino blink OFF");
    }
  }

}
