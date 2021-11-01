// ArduinoRP2040BoostSpiritExample

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
  Serial << "ArduinoRP2040BoostSpiritExample ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit operations" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Tests of operator << defines" << endl;
  Serial << "------------------------------" << endl;
  char const *tag = "tag";
  Serial << tag << endl;
  std::string rule("rule");
  Serial << rule.c_str() << endl;
  Serial << "------------------------------" << endl;
  Serial << "Boost Spirit Qi Parser" << endl;
  Serial << "------------------------------" << endl;
  std::string s("123");
  auto it = s.begin();
  bool match = qi::parse(it, s.end(), ascii::digit);
  Serial << match << " ";
  if (it != s.end()) Serial << std::string{it, s.end()}.c_str();
  Serial << endl;
  Serial << "------------------------------" << endl;
  Serial << "Boost Spirit Karma Generator" << endl;
  Serial << "------------------------------" << endl;
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
