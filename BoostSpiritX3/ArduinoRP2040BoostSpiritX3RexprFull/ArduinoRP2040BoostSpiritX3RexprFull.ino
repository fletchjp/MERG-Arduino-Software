/// @file ArduinoRP2040BoostSpiritX3Rexpr.ino
/// @brief Example of Boost Spirit X3 Rexpr minimal example
///
/// Boost Spirit 3.0.0 is included from Arduino Boost (Boost 1.66.0)
/// This is taken from spirit/example/x3/rexpr_min/rexpr
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

///////////////////////////////////////////////////////////////////////////////
//  Our AST
///////////////////////////////////////////////////////////////////////////////
namespace client { namespace ast
{
    namespace fusion = boost::fusion;
    namespace x3 = boost::spirit::x3;

    struct rexpr;

    struct rexpr_value : x3::variant<
            std::string
          , x3::forward_ast<rexpr>
        >
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    typedef std::map<std::string, rexpr_value> rexpr_map;
    typedef std::pair<std::string, rexpr_value> rexpr_key_value;

    struct rexpr
    {
        rexpr_map entries;
    };
}}

// We need to tell fusion about our rexpr struct
// to make it a first-class fusion citizen
BOOST_FUSION_ADAPT_STRUCT(client::ast::rexpr,
    entries
)

///////////////////////////////////////////////////////////////////////////////
//  AST processing
///////////////////////////////////////////////////////////////////////////////
namespace client { namespace ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  Print out the rexpr tree
    ///////////////////////////////////////////////////////////////////////////
    int const tabsize = 4;

    struct rexpr_printer
    {
        typedef void result_type;

        rexpr_printer(int indent = 0)
          : indent(indent) {}

        void operator()(rexpr const& ast) const
        {
            Serial << '{' << endl;
            for (auto const& entry : ast.entries)
            {
                tab(indent+tabsize);
                Serial << '"' << entry.first << "\" = ";
                boost::apply_visitor(rexpr_printer(indent+tabsize), entry.second);
            }
            tab(indent);
            //Serial << "    " << endl;
            Serial << '}' << endl;
        }

        void operator()(std::string const& text) const
        {
            Serial << '"' << text << '"' << endl;
        }

        void tab(int spaces) const
        {
            for (int i = 0; i < spaces; ++i)
                Serial << ' ';
        }

        int indent;
    };
}}

///////////////////////////////////////////////////////////////////////////////
//  Our rexpr grammar
///////////////////////////////////////////////////////////////////////////////
namespace client { namespace parser
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;

    using ascii::char_;
    using ascii::string;

    x3::rule<class rexpr_value, ast::rexpr_value>
        rexpr_value = "rexpr_value";

    x3::rule<class rexpr, ast::rexpr>
        rexpr = "rexpr";

    x3::rule<class rexpr_key_value, ast::rexpr_key_value>
        rexpr_key_value = "rexpr_key_value";

    auto const quoted_string =
        lexeme['"' >> *(char_ - '"') >> '"'];

    auto const rexpr_value_def =
        quoted_string | rexpr;

    auto const rexpr_key_value_def =
        quoted_string >> '=' >> rexpr_value;

    auto const rexpr_def =
        '{' >> *rexpr_key_value >> '}';

    BOOST_SPIRIT_DEFINE(rexpr_value, rexpr, rexpr_key_value);
}}

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
  Serial << "ArduinoRP2040BoostSpiritX3Rexpr ** " << endl << __FILE__ << endl;
  Serial << "Boost Spirit X3 parsing" << endl;

    // I need to sort out the input here.
    std::string storage = input; // We will read the contents here.

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
