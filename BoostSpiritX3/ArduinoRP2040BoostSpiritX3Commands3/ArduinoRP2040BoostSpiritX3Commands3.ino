/// @file ArduinoRP2040BoostSpiritX3Commands3.ino
/// @brief Boost Spirit X3 commands example with annotation
///
/// Extension of the comments example taken from
/// https://stackoverflow.com/questions/65614720/boost-spirit-x3-tokenizer-with-annotation-does-not-work
///
/// The comments example is the first time I have been able to get annotation to work.
///
/// I am starting a third example to extend the command processing to cover "when" commands.
///
/// I will start with a simple command and extend it to cover recursive operation.
///
/// There is also work to be done to store the resulting structures.
///
/// I have now succeeded in parsing the define statements to store an Event - renamed from Define.
///
/// I have also sorted out how to store the results and also check for duplicate names.
///
/// I think have overloaded functions called store to store Person results in a vector and Event results in a map.
///
/// This code to uses x3::variant after experiments with the comments example.
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
#include <vector>
#include <map>
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

BOOST_FUSION_ADAPT_STRUCT(client::ast::Event,
    name, nn, en
)

BOOST_FUSION_ADAPT_STRUCT(client::ast::State,
    name, on_off
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

        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;

        using boost::spirit::x3::lexeme;
        using boost::spirit::x3::omit;
        using boost::spirit::x3::skip;
        using boost::spirit::x3::ascii::space;
        using boost::spirit::x3::lit;
        using boost::spirit::x3::int_;
        //using boost::spirit::x3::add;
        using ascii::char_;
        using client::ast::on_off_t;
        
        struct event_name_class;
        struct event_class;
        struct state_class;
        struct quoted_string_class;
        struct Person_class;

        /// Parsing into a struct adapted from:
        /// https://stackoverflow.com/questions/37749344/parsing-into-structs-with-boolean-or-enum-members-with-boost-spirit-x3
        struct on_off_table : x3::symbols<on_off_t> {
        on_off_table() {
            add ("off"   , on_off_t::off)
                ("on" ,    on_off_t::on);
          }
        } const on_off;
        
        x3::rule<event_name_class, std::string> const event_name = "event_name";
        x3::rule<event_class, client::ast::Event>   const event = "event";
        x3::rule<state_class, client::ast::State>   const state = "state";
        x3::rule<quoted_string_class, std::string> const quoted_string = "quoted_string";
        x3::rule<Person_class, client::ast::Person> const Person = "person";

        /// Adapted from https://stackoverflow.com/questions/59759810/how-do-you-get-a-string-out-of-a-boost-spirit-x3-lexeme-parser
        x3::rule<class identifier_rule_, std::string> const identifier_rule = "identifier_rule";
        auto const identifier_rule_def = x3::lexeme[(x3::alpha | x3::char_('$')) >> *(x3::alnum)];
        BOOST_SPIRIT_DEFINE(identifier_rule)

        /// The state rule parses an identifier and its state
        auto const state_def = lit("state(") >> identifier_rule >> ")" >> omit[+space] >> "is" >> omit[+space] >> on_off;
        /// The event rule parses an identifier and two numbers
        auto const event_def = lit("define") >> omit[+space] >> identifier_rule >> omit[+space] >> '=' 
                                             >> omit[+space] >> lit("NN:") >> int_
                                             >> omit[+space] >> lit("EN:") >> int_;
                                             //>> x3::omit[*(x3::char_ - x3::eol)];
        auto const quoted_string_def = lexeme['"' >> +(char_ - '"') >> '"'];
        /// The person rule uses omit[+space] to discard spaces after a keyword.
        auto const Person_def = lit("person") >> omit[+space] >> quoted_string >> ',' >> quoted_string;
         
        BOOST_SPIRIT_DEFINE(state,event,quoted_string,Person);

        /// rules defined like this cannot accept subrules such as quoted string.
        /// rule definition - singleLineComment
        auto singleLineComment = as<SingleLineComment>("//" >> x3::omit[*(x3::char_ - x3::eol)]);
        /// rule definition - Whitespace
        auto whitespace        = as<Whitespace>       (x3::omit[+x3::ascii::space]);
        /// rule definition - Define - for the moment just identify the keyword.
        auto define             = as<Define>            ("define" >> x3::omit[*(x3::char_ - x3::eol)]);
        /// rule definition - When - for the moment just identify the keyword.
        auto when              = as<When>             ("when" >> x3::omit[*(x3::char_ - x3::eol)]);
        /// rule definition - Token- this is the Variant for all the rules.
        auto token             = as<Token>            (singleLineComment | whitespace | event | when | Person, "token");
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

// define example which is now being parsed
define $name1 = NN:0 EN:1
define $name2 = NN:0 EN:2
when state($name1) is off within 1sec send on$name2
person "John","Fletcher"
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
              case 2 : s = "event"; break;
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
              if (which == 2 || which == 4) Serial << " : "  << token; //store(token);
              Serial << endl;
              
            } 
        }
        Serial << client::ast::events.size() << " event definitions found" << endl;
        Serial << client::ast::people.size() << " person entries found" << endl;
    } else {
          Serial << "parsing failed" << endl;
          Serial << "Found " << tokens.size() << " tokens" << endl;
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
