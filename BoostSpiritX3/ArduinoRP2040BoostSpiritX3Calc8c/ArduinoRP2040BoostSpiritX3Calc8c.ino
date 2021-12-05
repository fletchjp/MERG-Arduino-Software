/// @file ArduinoRP2040BoostSpiritX3Calc8c.ino
/// @brief Example of Boost Spirit X3 Calc8 modified to use custom diagnostics
///
/// Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)
/// This is taken from spirit/example/x3/calc/calc8 from Boost 1.77.0
/// and adapted to run on the Arduino NANO RP2040 connect.
///
/// At the moment this runs with correct input and crashes using an undefined variable.
///
/// I have changed expectation (>) to sequence (>>) in the code which is now being reversed.
///
/// I am going to use BOOST_SPIRIT_X3_NO_EXCEPTION and custom error diagnostics.
///
/// I thought that this would be easy. I am struggling to sort out the parser configuration.
///
/// I have now got this down to a single undefined reference to do with parse_rule.
///
/// I found that was due to a lack of modifications in the expression code.
///
/// The parser is now in a separate file parser.hpp
///
/// I still cannot get it working with the diagnostics and I think the problem is in the compiler code which is still doing error handling.
///
/// I have now found that there was a problem with the context_type where only one with<> can be handled at one time.
///
/// This has involved a number of adaptions to the Arduino environment.
///
///////////////////////////////////////////////////////////////////////////////
///
///  Now we'll introduce variables and assignment. This time, we'll also
///  be renaming some of the rules -- a strategy for a grander scheme
///  to come ;-)
///
///  This version also shows off grammar modularization. Here you will
///  see how expressions and statements are built as modular grammars.
///
///  [ JDG April 9, 2007 ]       spirit2
///  [ JDG February 18, 2011 ]   Pure attributes. No semantic actions.
///  [ JDG May 17, 2014 ]        Ported from qi calc7 example.
///
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

/// This switches off exceptions and provides a different way to get error information.
#define BOOST_SPIRIT_X3_NO_EXCEPTION
#include "ArduinoCode.h"

#include "ast.hpp"
#include "vm.hpp"
#include "compiler.hpp"
#include "statement.hpp"
#include "expression.hpp"
//#include "statement_def.hpp"
#include "error_handler.hpp"
#include "config.hpp"
#include "custom.hpp"
#include "parser.hpp"

#include <iostream>


#include <boost_config_warning_disable.hpp>
#include <boost_spirit_home_x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/io.hpp>


/// Example data
std::string const input = R"( var a = 123;
   var b = 456;
   var d = 1;
   var c = a + b * 2 + d;

)";

namespace x3 = boost::spirit::x3;

void setup() {
  /// put your setup code here, to run once:
  Serial.begin (115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 20000));
  t2 = millis() - t1;
  Serial.print("Waited for ");
  Serial.print(t2);
  Serial.println(" millis");
  while (!delay_without_delaying(20000) ) { };
  Serial << "ArduinoRP2040BoostSpiritX3Calc8 ** " << endl << __FILE__ << endl;
  Serial << "Boost Spirit X3 parsing" << endl;

    // I need to sort out the input here.
    //std::string storage = input; // We will read the contents here.

    Serial << "/////////////////////////////////////////////////////////\n\n";
    Serial << "Statement parser...\n\n";
    Serial << "/////////////////////////////////////////////////////////\n\n";
    Serial << "Type some statements... ";
    Serial << "An empty line ends input, compiles, runs and prints results\n\n";
    Serial << "Example:\n\n";
    Serial << "    var a = 123;\n";
    Serial << "    var b = 456;\n";
    Serial << "    var c = a + b * 2;\n\n";
    Serial << "-------------------------\n";

    std::string source = input;
    Serial << "input: " << input << endl;
    Serial << "-------------------------\n";

/// parse function moved to parser.hpp
    bool success = parse(source);
    Serial << "-------------------------\n\n";

    Serial << "Bye... :-) \n\n";

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
