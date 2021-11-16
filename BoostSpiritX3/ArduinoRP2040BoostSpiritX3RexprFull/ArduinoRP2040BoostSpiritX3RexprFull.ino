/// @file ArduinoRP2040BoostSpiritX3RexprFull.ino
/// @brief Example of Boost Spirit X3 Rexpr full example
///
/// Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)
/// This is taken from spirit/example/x3/rexpr/rexpr_full
/// and adapted to run on the Arduino NANO RP2040 connect.
///
/// The main code is in rexpr/rexpr_full/test/parse_rexpr_test.cpp
///
/// This has involved a number of adaptions to the Arduino environment.
///
/// At the moment this example compiles without the annotation
///
/// I have attempted to remove all of the error handling as it uses exceptions. 
// 
///
///  Example Data:
///
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
#include <iterator>
#include <algorithm>
#include <sstream>

#include <cstdio>

#include "ArduinoCode.h"

#include <boost_config_warning_disable.hpp>
#include <boost_spirit_home_x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/io.hpp>

#include "ast.hpp"
#include "rexpr.hpp"
#include "error_handler.hpp"
#include "config.hpp"
#include "printer.hpp"

//#include "rexpr_def.hpp"
//#include "config.hpp"

/*
namespace rexpr { namespace parser
{
    BOOST_SPIRIT_INSTANTIATE(
        rexpr_type, iterator_type, context_type);
}}
*/

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
    std::string source = input; // We will read the contents here.

    using rexpr::rexpr; // Our grammar
    using rexpr::parser::iterator_type;
    iterator_type iter(source.begin());
    iterator_type const end(source.end());

    // Our AST
    rexpr::ast::rexpr ast;



    // Go forth and parse!
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, rexpr(), space, ast);


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
