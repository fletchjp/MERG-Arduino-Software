// ArduinoRP2040BoostSpiritKeyNode

// Taken from spirit/example/q1/key_value_sequence
// adapted to look at CANCOMPUTE parsing.

// 3rd party libraries
#include <Streaming.h>

#undef F
#undef min

#include <exception>
#include <stdexcept>

// Solution to the sync_synchronize problem
//https://www.vexforum.com/t/i-am-learning-arm-none-eabi-compiler-recently-i-want-to-ask-some-questions/73973
#include <iostream> 
extern "C" void __sync_synchronize() {}

// Dummies to sort out compilation
namespace boost {

  void throw_exception( std::exception & e ) { }
  void throw_exception(const std::exception & e ) { }
  void throw_exception( std::runtime_error& e) { }
  void throw_exception(const std::runtime_error& e) { }

}



#include <string>
#include <vector>
#include <map>
// This does not work. Input types are not a good enough match.
template<class T>
inline Print &operator <<(Print &stream, const std::string &arg)
{
  stream.print(arg.c_str());
  return stream;
}
template<class T>
inline Print &operator <<(Print &stream, const char *arg)
{
  stream.print(arg);
  return stream;
}


#include <cstdio>
//#include <PicoThread.h>
#define BOOST_SPIRIT_SINGLE_GRAMMAR_INSTANCE
// No longer used - all changes restored in the files.
//#define BOOST_SPIRIT_DEBUG_OUT Serial
#include <boost_spirit_include_qi.hpp>
#include <boost_spirit_include_karma.hpp>
#include <boost_spirit_include_phoenix_core.hpp>
#include <boost_spirit_include_phoenix_operator.hpp>
#include <boost_spirit_include_phoenix_stl.hpp>
#include <boost_spirit_include_phoenix_bind.hpp>
#include <boost_phoenix_bind_bind_function.hpp>
#include <boost_fusion_include_std_pair.hpp>
//#include <boost_lambda_lambda.hpp>
#include <boost_bind.hpp>
#include <boost_bind_placeholders.hpp>
#include "skipper.hpp"

//#include <boost_spirit.hpp>

using namespace boost::spirit;

//////////////////////////////////////////////////////////
// actions example
//////////////////////////////////////////////////////////

namespace client
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;

    namespace qi = boost::spirit::qi;
    using ascii::space;

namespace parser {

    typedef std::map<std::string, std::string> pairs_type;
    //typedef std::map<std::string, std::pair<int,int>> event_type;

    template <typename Iterator>
    struct key_value_sequence 
      : qi::grammar<Iterator, pairs_type() ,skipper<Iterator> >
    {
        key_value_sequence()
          : key_value_sequence::base_type(query)
        {
            query =  pair >> *((qi::lit(';') | '&') >> pair);
            pair  =  key >> ('=') >> value ;
            key   =  qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
            value = +qi::char_("a-zA-Z_0-9");
        }

        qi::rule<Iterator, pairs_type(), skipper<Iterator> > query;
        qi::rule<Iterator, std::pair<std::string, std::string>(), skipper<Iterator> > pair;
        qi::rule<Iterator, std::string()> key, value;
    };
  
  }

}

//////////////////////////////////////////////////////////

// This comes from the cdc_multi example
// Helper: non-blocking "delay" alternative.
boolean delay_without_delaying(unsigned long time) {
  // return false if we're still "delaying", true if time ms has passed.
  // this should look a lot like "blink without delay"
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
  Serial.begin (115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 10000));
  t2 = millis() - t1;
  Serial.print("Waited for ");
  Serial.print(t2);
  Serial.println(" millis");
  while (!delay_without_delaying(10000) ) { };
  Serial << "ArduinoRP2040BoostSpiritKeyNode" << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit operations" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Boost Spirit Qi Parser" << endl;
  Serial << "------------------------------" << endl;
  
    using boost::spirit::qi::int_;
    using boost::spirit::qi::parse;

    std::string input("r1 = NN0;r2 = NN0;r3 = NN0");
    std::string::iterator begin = input.begin();
    std::string::iterator end = input.end();

    typedef std::string::iterator iterator_type;
    client::parser::key_value_sequence<std::string::iterator> p;
    client::parser::pairs_type m;
    client::parser::skipper<iterator_type>
        skipper;                                // Our skipper

    using ascii::space;

   if (!qi::phrase_parse(begin, end, p, skipper, m))
    {
        Serial << "-------------------------------- \n";
        Serial << "Parsing failed\n";
        Serial << "-------------------------------- \n";
    }
    else
    {
        Serial << "-------------------------------- \n";
        Serial << "Parsing succeeded, found entries:\n";
        client::parser::pairs_type::iterator end = m.end();
        for (client::parser::pairs_type::iterator it = m.begin(); it != end; ++it)
        {
            Serial << (*it).first.c_str();
            if (!(*it).second.empty())
              Serial << " = " << (*it).second.c_str();
            Serial << endl;
        }
        Serial << "---------------------------------\n";
    }
  Serial << "------------------------------" << endl;
  //Serial << "------------------------------" << endl;
  //Serial << "Boost Spirit Karma Generator" << endl;
  //Serial << "------------------------------" << endl;
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
