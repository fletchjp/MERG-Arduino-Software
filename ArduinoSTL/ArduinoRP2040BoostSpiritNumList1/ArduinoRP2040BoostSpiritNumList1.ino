// ArduinoRP2040BoostSpiritNumList1

// Taken from spirit/example/q1/num_list1 

// 3rd party libraries
#include <Streaming.h>

#undef F
#undef min
/* Needed for DUE?
#undef round
#undef B1
#undef B2
#undef B3
#undef B4
#undef B5
#undef B6
#undef B7
#undef TC1
#undef TC2
*/

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
//#include <boost_spirit.hpp>

using namespace boost::spirit;

//////////////////////////////////////////////////////////
// num_list1 example
//////////////////////////////////////////////////////////

namespace client
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    ///////////////////////////////////////////////////////////////////////////
    //  Our number list parser
    ///////////////////////////////////////////////////////////////////////////
    //[tutorial_numlist1
    template <typename Iterator>
    bool parse_numbers(Iterator first, Iterator last)
    {
        using qi::double_;
        using qi::phrase_parse;
        using ascii::space;

        bool r = phrase_parse(
            first,                          /*< start iterator >*/
            last,                           /*< end iterator >*/
            double_ >> *(',' >> double_),   /*< the parser >*/
            space                           /*< the skip-parser >*/
        );
        if (first != last) // fail if we did not get a full match
            return false;
        return r;
    }
    //]
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
  while (!delay_without_delaying(5000) ) { };
  Serial << "ArduinoRP2040BoostSpiritNumList1 ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit operations" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Boost Spirit Qi Parser" << endl;
  Serial << "------------------------------" << endl;
  Serial << "A comma separated list of numbers.\n";
  std::string str("1.5, 2.765, -1.987");
  Serial << str.c_str() << endl;
  int len = str.length();
  int n = 0;
  while (n < 1)
  {
       //if (str.empty() || str[0] == 'q' || str[0] == 'Q')
       //     break;
        n++;
        if (client::parse_numbers(str.begin(), str.end()))
        {
            Serial << "---------------------\n";
            Serial << "Parsing succeeded\n";
            Serial << str.c_str() << " Parses OK: " << endl;
        }
        else
        {
            Serial << "-------------------------\n";
            Serial << "Parsing failed\n";
            Serial << "-------------------------\n";
        }
        //if(str.begin() == str.end() ) break;
  }

  
  //Serial << "------------------------------" << endl;
  //Serial << "Boost Spirit Karma Generator" << endl;
  //Serial << "------------------------------" << endl;
  Serial << "------------------------------" << endl;
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
