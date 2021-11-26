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
/// There is a lot more to the full CANCOMPUTE than I realised. There is a logical structure to follow.
///
/// I have now succeeded in parsing the define statements to store an Event - renamed from Define.
///
/// I have also sorted out how to store the results and also check for duplicate names.
///
/// I am having a lot of problems getting the when rule to parse data.
///
/// I have broken it down into stages and now have the state part parsing O.K. and outputting data.
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

BOOST_FUSION_ADAPT_STRUCT(client::ast::Item,
    on_off, name
)

BOOST_FUSION_ADAPT_STRUCT(client::ast::Time,
    time, time_unit
)

BOOST_FUSION_ADAPT_STRUCT(client::ast::When,
    expression, time, actions
)


/// Rules moved into rulesCode.h
#include "rulesCode.h"


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
person "John","Fletcher"
state($name1) is off
state($name2) is on
)";

//when $name1 is off within 1sec send on$name2
//state(
   
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
              case 3 : s = "state"; break;
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
              if (which > 1 && which < 5) Serial << " : "  << token; //store(token);
              Serial << endl;
              
            } 
        }
        Serial << client::ast::events.size() << " event definitions found" << endl;
        Serial << client::ast::some_states.size() << " state definitions found" << endl;
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
