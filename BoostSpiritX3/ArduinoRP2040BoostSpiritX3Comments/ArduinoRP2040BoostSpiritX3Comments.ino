/// @file ArduinoRP2040BoostSpiritX3Comments.ino
/// @brief Boost Spirit X3 comments example with annotation
///
/// Taken from https://stackoverflow.com/questions/65614720/boost-spirit-x3-tokenizer-with-annotation-does-not-work
///
/// This example is the first time I have been able to get annotation to work.
///
/// I think this is a good basis for further experiments.


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

#include "ArduinoCode.h"

#include <boost_config_warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
namespace x3 = boost::spirit::x3;

struct SingleLineComment{};
struct Whitespace       {};

/// This uses std::variant.
/// Perhaps explore using X3 variant instead.
using Variant = std::variant<SingleLineComment, Whitespace>;

/// Token structure
struct Token : Variant, x3::position_tagged {
    using Variant::Variant;
};

/// namespace for the project changed from anonymous namespace
namespace project {
    struct position_cache_tag;
    
    /// Parser namespace
    namespace Parser {
        struct annotate_position {
            template <typename T, typename Iterator, typename Context>
                inline void on_success(Iterator first, Iterator last, T &ast, Context const &context) const {
                    auto &position_cache = x3::get<position_cache_tag>(context);
                    position_cache.annotate(ast, first, last);
                }
        };
        
        /// unique on success hook classes
        template <typename> struct Hook {}; // no annotate_position mix-in
        template <> struct Hook<Token> : annotate_position   {};

        /// definition of as lambda function used to generate the rules
        template <typename T>
        static auto constexpr as = [](auto p, char const* name = typeid(decltype(p)).name()) {
            return x3::rule<Hook<T>, T> {name} = p;
        };

        /// rule definitions
        auto singleLineComment = as<SingleLineComment>("//" >> x3::omit[*(x3::char_ - x3::eol)]);
        auto whitespace        = as<Whitespace>       (x3::omit[+x3::ascii::space]);
        auto token             = as<Token>            (singleLineComment | whitespace, "token");
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

    )";
    position_cache positions{content.begin(), content.end()};

    using project::position_cache_tag;

    auto parser = x3::with<position_cache_tag>(positions)[*project::Parser::token];

     /// vector for token results
    std::vector<Token> tokens;
    if (parse(begin(content), end(content), parser >> x3::eoi, tokens)) {
        Serial << "Found " << tokens.size() << " tokens" << endl;

        for (auto& token : tokens) {
            auto pos = positions.position_of(token);
            std::string s;
            if (token.index()) s = "space"; else s = "comment";
             std::stringstream ss;
            /// std::quoted is new in C++14
            ss << std::quoted(std::string_view(&*pos.begin(), pos.size()));
            Serial
                << s << "\t"
                << ss.str()
                << endl;
        }
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
