// ArduinoRP2040BoostSpiritX3Roman

// Taken from spirit/example/x3/roman

/*
 * https://ostack.cn/?qa=302784/
That warning is telling you that there was a subtle ABI change
(actually a conformance fix) between 6 and 7.1,
such that libraries built with 6.x or earlier may not work properly
when called from code built with 7.x (and vice-versa).
As long as all your C++ code is built with GCC 7.1 or later, you can safely ignore this warning.
To disable it, pass -Wno-psabi to the compiler
 */

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
#include <list>
// This does not work. Input types are not a good enough match.
//template<class T>
inline Print &operator <<(Print &stream, const std::string &arg)
{
  stream.print(arg.c_str());
  return stream;
}
//template<class T>
inline Print &operator <<(Print &stream, const char *arg)
{
  stream.print(arg);
  return stream;
}


#include <cstdio>
#include <boost_spirit_home_x3.hpp>

namespace client
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    ///////////////////////////////////////////////////////////////////////////////
    //  Parse roman hundreds (100..900) numerals using the symbol table.
    //  Notice that the data associated with each slot is the parser's attribute
    //  (which is passed to attached semantic actions).
    ///////////////////////////////////////////////////////////////////////////////
    struct hundreds_ : x3::symbols<unsigned>
    {
        hundreds_()
        {
            add
                ("C"    , 100)
                ("CC"   , 200)
                ("CCC"  , 300)
                ("CD"   , 400)
                ("D"    , 500)
                ("DC"   , 600)
                ("DCC"  , 700)
                ("DCCC" , 800)
                ("CM"   , 900)
            ;
        }

    } hundreds;

    ///////////////////////////////////////////////////////////////////////////////
    //  Parse roman tens (10..90) numerals using the symbol table.
    ///////////////////////////////////////////////////////////////////////////////
    struct tens_ : x3::symbols<unsigned>
    {
        tens_()
        {
            add
                ("X"    , 10)
                ("XX"   , 20)
                ("XXX"  , 30)
                ("XL"   , 40)
                ("L"    , 50)
                ("LX"   , 60)
                ("LXX"  , 70)
                ("LXXX" , 80)
                ("XC"   , 90)
            ;
        }

    } tens;

    ///////////////////////////////////////////////////////////////////////////////
    //  Parse roman ones (1..9) numerals using the symbol table.
    ///////////////////////////////////////////////////////////////////////////////
    struct ones_ : x3::symbols<unsigned>
    {
        ones_()
        {
            add
                ("I"    , 1)
                ("II"   , 2)
                ("III"  , 3)
                ("IV"   , 4)
                ("V"    , 5)
                ("VI"   , 6)
                ("VII"  , 7)
                ("VIII" , 8)
                ("IX"   , 9)
            ;
        }

    } ones;

    ///////////////////////////////////////////////////////////////////////////////
    //  roman (numerals) grammar
    //
    //      Note the use of the || operator. The expression
    //      a || b reads match a or b and in sequence. Try
    //      defining the roman numerals grammar in YACC or
    //      PCCTS. Spirit rules! :-)
    ///////////////////////////////////////////////////////////////////////////////
    namespace parser
    {
        using x3::eps;
        using x3::lit;
        using x3::_val;
        using x3::_attr;
        using ascii::char_;

        auto set_zero = [](auto& ctx){ _val(ctx) = 0; };
        auto add1000 = [](auto& ctx){ _val(ctx) += 1000; };
        auto add = [](auto& ctx){ _val(ctx) += _attr(ctx); };

        x3::rule<class roman, unsigned> const roman = "roman";

        auto const roman_def =
            eps                 [set_zero]
            >>
            (
                -(+lit('M')     [add1000])
                >>  -hundreds   [add]
                >>  -tens       [add]
                >>  -ones       [add]
            )
        ;

        BOOST_SPIRIT_DEFINE(roman); // Note: this macro picks up the definition of roman_def.
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
  while (!delay_without_delaying(5000) ) { };
  Serial << "ArduinoRP2040BoostSpiritNumList1 ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit X3 operations" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Boost Spirit X3 Parser" << endl;
  Serial << "------------------------------" << endl;
  Serial << "A Roman number.\n";

  typedef std::string::const_iterator iterator_type;
  using client::parser::roman; // Our parser

  std::string str("MCCXXVIII");
  unsigned result;

  Serial << str << endl;
  int len = str.length();
  int n = 0;
  while (n < 1)
  {
       //if (str.empty() || str[0] == 'q' || str[0] == 'Q')
       //     break;
        n++;

        iterator_type iter = str.begin();
        iterator_type const end = str.end();
        bool r = parse(iter, end, roman, result);

        if (r && iter == end)
        {
            Serial << "---------------------\n";
            Serial << "Parsing succeeded\n";
            Serial << "Result = " << result << endl;

            Serial << "\n-------------------------\n";

        }
        else
        {
            Serial << "-------------------------\n";
            Serial << "Parsing failed\n";
            Serial << "-------------------------\n";
        }
  }

  
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
