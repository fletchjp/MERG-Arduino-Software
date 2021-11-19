/// @file ArduinoRP2040BoostSpiritX3RexprAnnotation.ino
/// @brief Boost Spirit X3 Rexpr full example with annotation
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
/// and gives the same output as the simple version.
///
/// In this example I am going to restore the annotation.
///
/// I have brought in quite a lot of things which were not in the example, using the Annotation example.
/// It now compiles and has a link error which needs to be sorted out.
///
/// I have attempted to remove all of the error handling as it uses exceptions. 
/// 
///////////////////////////////////////////////////////////////////////////////
///
///  This is an extended version of the simple parser for X3 in 
///  ArduinoRP2040BoostSpiritX3Rexpr as a minimal starting point.
///  'rexpr' is a parser for a language resembling a minimal subset
///  of json, but limited to a dictionary (composed of key=value pairs)
///  where the value can itself be a string or a recursive dictionary.
///
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
#include "rexpr_def.hpp"
//#include "annotation.hpp"
#include "rexpr.hpp"
#include "error_handler.hpp"
#include "config.hpp"
#include "printer.hpp"

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

namespace rexpr
{
    parser::rexpr_type const& rexpr()
    {
        return parser::rexpr;
    }
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
  Serial << "ArduinoRP2040BoostSpiritX3RexprFull ** " << endl << __FILE__ << endl;
  Serial << "Boost Spirit X3 parsing" << endl;

    // I need to sort out the input here.
    std::string source = input; // We will read the contents here.
    Serial << "------------------------------" << endl;
    Serial << "Source :" << endl << source << endl;
    Serial << "------------------------------" << endl;
    using rexpr::rexpr; // Our grammar
    using rexpr::parser::iterator_type;
    using rexpr::parser::position_cache_tag;
    using position_cache = boost::spirit::x3::position_cache<std::vector<iterator_type>>;
    iterator_type iter(source.begin());
    iterator_type const end(source.end());
    position_cache positions{input.begin(), input.end()};

    // Our AST
    rexpr::ast::rexpr ast;

    using boost::spirit::x3::with;

    auto const parser =
        // we pass our position_cache to the parser so we can access
        // it later in our on_sucess handlers
        with<position_cache_tag>(std::ref(positions))
        [
            rexpr()
        ];


    // Go forth and parse!
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);

    if (success && iter == end)
    {
        Serial << "-------------------------\n";
        Serial << "Parsing succeeded\n";
        Serial << "-------------------------\n";
        rexpr::ast::rexpr_printer()(ast);
        //printer(ast);
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
