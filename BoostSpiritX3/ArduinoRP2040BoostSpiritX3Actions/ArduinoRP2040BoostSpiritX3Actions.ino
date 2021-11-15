/// @file ArduinoRP2040BoostSpiritX3Actions.ino
/// @brief Example of Boost Spirit X3 Actions
///
/// Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)
/// This is taken from spirit/example/x3/actions
/// and adapted to run on the Arduino NANO RP2040 connect.
///
/// This has involved a number of adaptions to the Arduino environment.

// 3rd party libraries
#include <Streaming.h>

#undef F
#undef min


#include <exception>
#include <stdexcept>
#include <iostream> 
#include <string>
#include <vector>
#include <cstdio>

#include "ArduinoCode.h"

#include <boost_spirit_home_x3.hpp>

//////////////////////////////////////////////////////////
/// actions example
//////////////////////////////////////////////////////////
///
/// Presented are various ways to attach semantic actions
///  * Using plain function pointer
///  * Using simple function object

namespace client
{
    namespace x3 = boost::spirit::x3;
    using x3::_attr;

    struct print_action
    {
        template <typename Context>
        void operator()(Context const& ctx) const
        {
            Serial << _attr(ctx) << endl;
        }
    };
}


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
  Serial << "ArduinoRP2040BoostSpiritX3Actions ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit X3 operations" << endl;
    using boost::spirit::x3::int_;
    using boost::spirit::x3::parse;
    using client::print_action;
   { /// example using function object

        char const *first = "{43}", *last = first + std::strlen(first);
        parse(first, last, '{' >> int_[print_action()] >> '}');
    }

    { /// example using C++14 lambda

        char const *first = "{44}", *last = first + std::strlen(first);
        auto f = [](auto& ctx){ Serial << _attr(ctx) << endl; };
        parse(first, last, '{' >> int_[f] >> '}');
    }

  Serial << "------------------------------" << endl;
  //Serial << "------------------------------" << endl;
  //Serial << "Boost Spirit Karma Generator" << endl;
  //Serial << "------------------------------" << endl;
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
