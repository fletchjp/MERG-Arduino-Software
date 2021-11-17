/// @file ArduinoRP2040BoostSpiritX3Variant.ino
/// @brief Example of Boost Spirit X3 Variant
///
/// Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)
/// This is taken from spirit/test/x3/x3_variant
/// and adapted to run on the Arduino NANO RP2040 connect.
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
#include <strstream>
#include <string>
#include <vector>
#include <cstdio>

#include "ArduinoCode.h"

#include <boost_spirit_home_x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include "variantCode.h"


//////////////////////////////////////////////////////////

// This comes from the cdc_multi example
/// Helper: non-blocking "delay" alternative.
boolean delay_without_delaying(unsigned long time) {
  /// return false if we're still "delaying", true if time ms has passed.
  /// this should look a lot like "blink without delay"
  static unsigned long previousmillis = 0;
  unsigned long currentmillis = millis();
  if (currentmillis - previousmillis >= time) {
    previousmillis = currentmillis;
    return true;
  }
  return false;
}
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
  Serial << "ArduinoRP2040BoostSpiritX3Variant ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit X3 variant operations" << endl;
 
  {
     boost::variant<int,double> bv(3);
     Serial << bv.which() << endl;
     std::strstream sbv;
     sbv << bv << std::ends;
     Serial << sbv.str() << endl;
 
     ast v{123};
     Serial << "ast v{123};" << endl;
     Serial << boost::get<int>(v) << endl;
     v = "test";              
     Serial << boost::get<std::string>(v) << endl;
     v = "test2";              
     Serial << v << endl;
     v = true;
     Serial << boost::get<bool>(v) << endl;
     v = 3.14;
     Serial << boost::get<double>(v) << endl;
     Serial << v.get().which() << endl;
     std::strstream sv;
     sv << v.get() << std::ends; // Nothing
     Serial << sv.str() << endl;
     
    
  }
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
