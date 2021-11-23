/// @file ArduinoRP2040BoostSpiritX3Commands2.ino
/// @brief Boost Spirit X3 commands example with annotation
///
/// Extension of the comments example taken from
/// https://stackoverflow.com/questions/65614720/boost-spirit-x3-tokenizer-with-annotation-does-not-work
///
/// The comments example is the first time I have been able to get annotation to work.
///
/// I have now started to explore how to parse commands as well.
///
/// For the moment I am parsing the person command and getting back the values parsed.
///
/// I am starting a new copy to extend the commands.
///
/// I have changed the rule name from person to Person.
///
/// This code to uses x3::variant after experiments with the comments example.
///
/// I have now reached a point where I can get access to the content of a person object.
///
/// See the ArduinoRP2040BoostSpiritX3Commands.ino example.


// 3rd party libraries
#include <Streaming.h>

#undef F
#undef min


#include <exception>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <variant>
#include <string>
#include <sstream>
#include <cstdio>


#include "ArduinoCode.h"

#include <boost_config_warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

#include "variantCode.h"

/*struct QuotedString     {
    QuotedString(std::string const &str = "") : str(str) {}
    std::string str;
};*/


BOOST_FUSION_ADAPT_STRUCT(client::ast::Person,
    first_name, last_name
)

//using client::ast::person;

/// namespace for the project changed from anonymous namespace to client to match client::ast.
namespace client {
    struct position_cache_tag;
    
    /// Parser namespace defines the parser structure for the problem.
    namespace Parser {

        /// annotate_position provides the on_success operator
        struct annotate_position {
            template <typename T, typename Iterator, typename Context>
                inline void on_success(Iterator first, Iterator last, T &ast, Context const &context) const {
                    auto &position_cache = x3::get<position_cache_tag>(context);
                    position_cache.annotate(ast, first, last);
                }
        };
        
        /// unique on success hook classes

        /// no annotate_position mix-in
        template <typename> struct Hook {};
        /// When there is a specialisation it is annotated.
        template <> struct Hook<Token> : annotate_position   {}; 

        /// definition of as lambda function used to generate the rules
        template <typename T>
        static auto constexpr as = [](auto p, char const* name = typeid(decltype(p)).name()) {
            return x3::rule<Hook<T>, T> {name} = p;
        };

        namespace ascii = boost::spirit::x3::ascii;

        using boost::spirit::x3::lexeme;
        using ascii::char_;
        
        struct quoted_string_class;
        struct Person_class;

        x3::rule<quoted_string_class, std::string> const quoted_string = "quoted_string";
        x3::rule<Person_class, client::ast::Person> const Person = "person";

        auto const quoted_string_def = lexeme['"' >> +(char_ - '"') >> '"'];
        auto const Person_def = quoted_string >> ',' >> quoted_string;
         
        BOOST_SPIRIT_DEFINE(quoted_string,Person);

        /// rule definition - singleLineComment
        auto singleLineComment = as<SingleLineComment>("//" >> x3::omit[*(x3::char_ - x3::eol)]);
        /// rule definition - Whitespace
        auto whitespace        = as<Whitespace>       (x3::omit[+x3::ascii::space]);
        /// rule definition - Define - for the moment just identify the keyword.
        auto define            = as<Define>           ("define" >> x3::omit[*(x3::char_ - x3::eol)]);
        /// rule definition - When - for the moment just identify the keyword.
        auto when              = as<When>             ("when" >> x3::omit[*(x3::char_ - x3::eol)]);
        /// rule definition - Token- this is the Variant for all the rules.
        auto token             = as<Token>            (singleLineComment | whitespace | define | when | Person, "token");
    }
}

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
  // put your setup code here, to run once:
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
  Serial << "ArduinoRP2040BoostSpiritX3Comments ** " << endl << __FILE__ << endl;
  Serial << "Boost Spirit X3 parsing" << endl;

 
    using It             = std::string::const_iterator;
    using position_cache = x3::position_cache<std::vector<It>>;

/// Example data
    std::string const content = R"(// first single line comment

// second single line comment

// define example which is not yet being parsed
define $name1 = NN:0 EN:1
define $name2 = NN:0 EN:2
when state($name1) is off within 1sec send on$name2
"John","Fletcher"
)";

   
    position_cache positions{content.begin(), content.end()};

    using client::position_cache_tag;

    auto parser = x3::with<position_cache_tag>(positions)[*client::Parser::token];

     /// vector for token results
    std::vector<Token> tokens;
    if (parse(begin(content), end(content), parser >> x3::eoi, tokens)) {
        Serial << "Found " << tokens.size() << " tokens" << endl;

        for (auto& token : tokens) {
            auto pos = positions.position_of(token);
            std::string s;
            unsigned which = token.get().which();
            switch (which) 
            {
              case 0 : s = "comment"; break;
              case 1 : s = "space"; break;
              case 2 : s = "define"; break;
              case 3 : s = "when"; break;
              case 4 : s = "person"; 
              break;
              default: s = "unknown"; break;
            }
             std::stringstream ss;
            /// std::quoted is new in C++14
            if ( (which != 1) || pos.size() == 2 ) { // get rid of short whitespace
              ss << std::quoted(std::string_view(&*pos.begin(), pos.size()));
              Serial
                  << s << "\t"
                  << ss.str();
              if (which == 4) Serial << " : "  << token << endl; 
              Serial << endl;
              
            } //else {
              //Serial << s << " " << pos.size() << endl;
              //}
        }
    } else {
          Serial << "parsing failed" << endl;
    }
  Serial << "------------------------------" << endl;
  while (!delay_without_delaying(10000) ) { };
  Serial << "------------------------------" << endl;
  pinMode(LED_BUILTIN, OUTPUT);
}

//////////////////////////////////////////////////////////

int LEDstate = 0;

void loop() {
  // put your main code here, to run repeatedly:
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
