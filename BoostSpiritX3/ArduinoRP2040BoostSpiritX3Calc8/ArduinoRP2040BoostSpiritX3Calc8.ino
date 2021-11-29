/// @file ArduinoRP2040BoostSpiritX3Calc8.ino
/// @brief Example of Boost Spirit X3 Calc8
///
/// Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)
/// This is taken from spirit/example/x3/calc/calc8 from Boost 1.77.0
/// and adapted to run on the Arduino NANO RP2040 connect.
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

#include "ArduinoCode.h"

#include "ast.hpp"
#include "vm.hpp"
#include "compiler.hpp"
#include "statement.hpp"
#include "error_handler.hpp"
#include "config.hpp"
#include <iostream>


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

    using client::parser::iterator_type;
    iterator_type iter(source.begin());
    iterator_type end(source.end());

    client::vmachine vm;                                    // Our virtual machine
    client::code_gen::program program;                      // Our VM program
    client::ast::statement_list ast;                        // Our AST

    using boost::spirit::x3::with;
    using client::parser::error_handler_type;
    using client::parser::error_handler_tag;
    error_handler_type error_handler(iter, end, std::cerr); // Our error handler

    // Our compiler
    client::code_gen::compiler compile(program, error_handler);

    // Our parser
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<error_handler_tag>(std::ref(error_handler))
        [
            client::statement()
        ];

    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);

    std::cout << "-------------------------\n";

    if (success && iter == end)
    {
        if (compile(ast))
        {
            std::cout << "Success\n";
            std::cout << "-------------------------\n";
            vm.execute(program());

            std::cout << "-------------------------\n";
            std::cout << "Assembler----------------\n\n";
            program.print_assembler();

            std::cout << "-------------------------\n";
            std::cout << "Results------------------\n\n";
            program.print_variables(vm.get_stack());
        }
        else
        {
            std::cout << "Compile failure\n";
        }
    }
    else
    {
        std::cout << "Parse failure\n";
    }

    std::cout << "-------------------------\n\n";

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
