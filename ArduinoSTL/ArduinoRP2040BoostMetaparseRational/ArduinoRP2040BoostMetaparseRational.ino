// ArduinoRP2040BoostMetaparseRational
// I have put -Wno_narrowing into platform.txt


#include <exception>
#include <stdexcept>

// Dummies to sort out compilation
namespace boost {

  void throw_exception( std::exception & e ) { }
  void throw_exception(const std::exception & e ) { }
  void throw_exception( std::runtime_error& e) { }
  void throw_exception(const std::runtime_error& e) { }

}

#include <Streaming.h>

#undef F
#undef min // This is also needed.
#include <cstdio>
#include <string>
#include <boost_metaparse.hpp>
#include <boost_type_index.hpp>
#include <boost_rational.hpp>
#include <boost_config.hpp>

// This will need to be implemented somehow.
// The docs did not bother to explain to use boost rational!!!
// This code is to be found in the example file.
template <class Num, class Denom>
struct rational{
  rational() {}
  typedef rational type;

  // This explains why the example did not compile!!!
  static boost::rational<int> run()
  {
    return boost::rational<int>(Num::type::value, Denom::type::value);
  }
  
};

using namespace boost::metaparse;
// Define the grammar
typedef
  sequence_apply2<
    rational,

    token<int_>,
    one_of<
      last_of<lit_c<'/'>, token<int_> >,
      empty<boost::mpl::int_<1> >
    >
  >
  rational_grammar;

typedef build_parser<entire_input<rational_grammar>> rational_parser;

#define RATIONAL(s) \
  (rational_parser::apply<BOOST_METAPARSE_STRING(s)>::type::run())

// Generic template for Streaming adapted to support boost::rational<T>.
template<class T>
inline Print &operator <<(Print &stream, const boost::rational<T> &arg) 
{ // Basic version - there are some special cases e.g. numerator = 0 and denominator = 1
  T num   = arg.numerator();
  T denom = arg.denominator();
  stream.print(num);
  if (num != 0 && denom != 1) {
       stream.print("/"); 
       stream.print(denom);
  }
  return stream;
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
  Serial << "ArduinoRP2040BoostMetaparseRational ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Metaparse operations" << endl;
  const boost::rational<int> x = RATIONAL("1/3");
  Serial << "------------------------------" << endl;
  Serial << "Output examples" << endl;
  Serial << "------------------------------" << endl;
  // Serial << does not support this type unless it is extended.
  Serial << x << endl;
  Serial << x.numerator() << "/" << x.denominator() << endl;
  Serial << RATIONAL("0/3") << endl;
  Serial << RATIONAL("3/1") << endl;
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
