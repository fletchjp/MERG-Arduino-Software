/// @file ArduinoRP2040BoostSpiritX3Calc4c.ino
/// @brief Example of Boost Spirit X3 Calc4c
///
/// Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)
/// This is taken from spirit/example/x3/calc/calc4c
/// and adapted to run on the Arduino NANO RP2040 connect.
///
/// This has involved a number of adaptions to the Arduino environment.
///
///////////////////////////////////////////////////////////////////////////////
///
///  A Calculator example demonstrating generation of AST. The AST,
///  once created, is traversed, 1) To print its contents and
///  2) To evaluate the result.
///
///  [ JDG April 28, 2008 ]      For BoostCon 2008
///  [ JDG February 18, 2011 ]   Pure attributes. No semantic actions.
///  [ JDG January 9, 2013 ]     Spirit X3
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

#include "ArduinoCode.h"

#include "grammar.hpp"

#include <boost_config_warning_disable.hpp>
#include <boost_spirit_home_x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/io.hpp>

/// Example data
std::string const input = R"( (1 + 2) * (3 + 4) )";

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
  Serial << "ArduinoRP2040BoostSpiritX3Calc4c ** " << endl << __FILE__ << endl;
  Serial << "Boost Spirit X3 parsing" << endl;

    // I need to sort out the input here.
    //std::string storage = input; // We will read the contents here.

    Serial << "/////////////////////////////////////////////////////////\n\n";
    Serial << "Expression parser...\n\n";
    Serial << "/////////////////////////////////////////////////////////\n\n";
 
    typedef std::string::const_iterator iterator_type;
    typedef client::ast::program ast_program;
    typedef client::ast::printer ast_print;
    typedef client::ast::eval ast_eval;

    std::string str = input;
    Serial << "input: " << input << endl;
    //while (begin(input) != end(input))
    //{
        //if (str.empty() || str[0] == 'q' || str[0] == 'Q')
        //    break;

        auto& calc = client::calculator;    // Our grammar
        ast_program program;                // Our program (AST)
        ast_print print;                    // Prints the program
        ast_eval eval;                      // Evaluates the program

        iterator_type iter = str.begin();
        iterator_type end = str.end();
        boost::spirit::x3::ascii::space_type space;
        bool r = phrase_parse(iter, end, calc, space, program);

        if (r && iter == end)
        {
            Serial << "-------------------------\n";
            Serial << "Parsing succeeded\n";
            print(program);
            Serial << "\nResult: " << eval(program) << endl;
            Serial << "-------------------------\n";
        }
        else
        {
            std::string rest(iter, end);
            Serial << "-------------------------\n";
            Serial << "Parsing failed\n";
            Serial << "stopped at: \"" << rest << "\"\n";
            Serial << "-------------------------\n";
        }
    //}

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
