// ArduinoRP2040BoostSpiritActions

// Taken from spirit/example/q1/actions

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
//#include <boost_lambda_lambda.hpp>
#include <boost_bind.hpp>
#include <boost_bind_placeholders.hpp>

//#include <boost_spirit.hpp>

using namespace boost::spirit;

//////////////////////////////////////////////////////////
// actions example
//////////////////////////////////////////////////////////

// Presented are various ways to attach semantic actions
//  * Using plain function pointer
//  * Using simple function object
//  * Using boost.bind with a plain function
//  * Using boost.bind with a member function
//  * Using boost.lambda

namespace client
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;

    // A plain function
    void print(int const& i)
    {
        Serial << i << endl;
    }

    // A member function
    struct writer
    {
        void print(int const& i) const
        {
            Serial << i << endl;
        }
    };

    // A function object
    struct print_action
    {
        void operator()(int const& i, qi::unused_type, qi::unused_type) const
        {
            Serial << i << endl;
        }
    };

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
  Serial << "ArduinoRP2040BoostSpiritActions ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit operations" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Boost Spirit Qi Parser" << endl;
  Serial << "------------------------------" << endl;
  
    using boost::spirit::qi::int_;
    using boost::spirit::qi::parse;
    using client::print;
    using client::writer;
    using client::print_action;

    { // example using plain function

        char const *first = "{42}", *last = first + std::strlen(first);
        //[tutorial_attach_actions1
        parse(first, last, '{' >> int_[&print] >> '}');
        //]
    }

    { // example using simple function object

        char const *first = "{43}", *last = first + std::strlen(first);
        //[tutorial_attach_actions2
        parse(first, last, '{' >> int_[print_action()] >> '}');
        //]
    }

   { // example using boost.bind with a plain function

        char const *first = "{44}", *last = first + std::strlen(first);
        //[tutorial_attach_actions3
        using boost::placeholders::_1;
        parse(first, last, '{' >> int_[boost::bind(&print, _1)] >> '}');
        //]
    }

    { // example using boost.bind with a member function

        char const *first = "{45}", *last = first + std::strlen(first);
        //[tutorial_attach_actions4
        using boost::placeholders::_1;
        writer w;
        parse(first, last, '{' >> int_[boost::bind(&writer::print, &w, _1)] >> '}');
        //]
    }
/* This compiles without a direct reference to boost::phoenix::bind. */
/* However there is no output from this or the next one */
/*
    { // example using boost phoenix bind with a plain function
        char const *first = "{46}", *last = first + std::strlen(first);
        using boost::spirit::qi::_1;
        parse(first, last, '{' >> int_[&print, _1] >> '}');
         //]
    }
 */
/*
    { // example using boost.phoenix.bind with a member function

        char const *first = "{47}", *last = first + std::strlen(first);
        //[tutorial_attach_actions4
        using boost::phoenix::placeholders::arg1;
        writer w;
        parse(first, last, '{' >> int_[&writer::print, &w, arg1] >> '}');
        //]
    }
*/

/* This is not working. It does not like Serial in there.
    { // example using boost.lambda

        namespace lambda = boost::lambda;
        char const *first = "{46}", *last = first + std::strlen(first);
        using lambda::_1;
        //[tutorial_attach_actions5
        parse(first, last, '{' >> int_[Serial << _1 << '\n'] >> '}');
        //]
    }
*/
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
