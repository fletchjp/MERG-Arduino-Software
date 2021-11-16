/// @file ArduinoRP2040BoostSpiritX3RexprFull.ino
/// @brief Example of Boost Spirit X3 Rexpr full example
///
/// Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)
/// This is taken from spirit/example/x3/rexpr/rexpr_full
/// and adapted to run on the Arduino NANO RP2040 connect.
///
/// This has involved a number of adaptions to the Arduino environment.
///
///////////////////////////////////////////////////////////////////////////////
///
///  A simple parser for X3 intended as a minimal starting point.
///  'rexpr' is a parser for a language resembling a minimal subset
///  of json, but limited to a dictionary (composed of key=value pairs)
///  where the value can itself be a string or a recursive dictionary.
///
//  Example:
//
//  {
//      "color" = "blue"
//      "size" = "29 cm."
//      "position" = {
//          "x" = "123"
//          "y" = "456"
//      }
//  }
//
///////////////////////////////////////////////////////////////////////////////

// 3rd party libraries
#include <Streaming.h>

#undef F
#undef min


#include <exception>
#include <stdexcept>
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>

#include "ArduinoCode.h"

#include <boost_config_warning_disable.hpp>
#include <boost_spirit_home_x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/io.hpp>

#include "rexpr_def.hpp"
#include "config.hpp"




/// Sample input:
///
std::string input = R"(
  {
      "color" = "blue"
      "size" = "29 cm."
      "position" = {
          "x" = "123"
          "y" = "456"
      }
  }
)";

namespace rexpr { namespace parser
{
    BOOST_SPIRIT_INSTANTIATE(
        rexpr_type, iterator_type, context_type);
}}



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
  Serial << "ArduinoRP2040BoostSpiritX3RexprFull ** " << endl << __FILE__ << endl;
  Serial << "Boost Spirit X3 parsing" << endl;

    // I need to sort out the input here.
    std::string storage = input; // We will read the contents here.
/*
    using client::parser::rexpr; // Our grammar
    client::ast::rexpr ast; // Our tree

    using boost::spirit::x3::ascii::space;
    std::string::const_iterator iter = storage.begin();
    std::string::const_iterator end = storage.end();
    bool r = phrase_parse(iter, end, rexpr, space, ast);

    if (r && iter == end)
    {
        Serial << "-------------------------\n";
        Serial << "Parsing succeeded\n";
        Serial << "-------------------------\n";
        client::ast::rexpr_printer printer;
        printer(ast);
    }
    else
    {
        std::string::const_iterator some = iter+30;
        std::string context(iter, (some>end)?end:some);
        Serial << "-------------------------\n";
        Serial << "Parsing failed\n";
        Serial << "stopped at: \": " << context << "...\"\n";
        Serial << "-------------------------\n";
    }
*/

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
