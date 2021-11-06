// ArduinoRP2040BoostSpiritX3NumList1

// Taken from spirit/example/x3/num_list/num_list1 and spirit/example/karma/num_list1 (suspended)

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
#include <boost_spirit_home_x3.hpp>
// There is a problem combining x3 and karma with signbit, boost::spirit::detail::signbit vs boost::spirit::x3::signbit.
//#include <boost_spirit_include_karma.hpp>


//////////////////////////////////////////////////////////
// num_list1 example
//////////////////////////////////////////////////////////

namespace client
{
 //   namespace karma = boost::spirit::karma;
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;
 
    ///////////////////////////////////////////////////////////////////////////
    //  Our number list parser enhanced as in karma/num_list1
    ///////////////////////////////////////////////////////////////////////////
    //[tutorial_numlist1
    //template <typename Iterator>
    //bool parse_numbers(Iterator first, Iterator last, std::list<double> &v)
   ///////////////////////////////////////////////////////////////////////////
    //  Our number list parser
    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator>
    bool parse_numbers(Iterator first, Iterator last)
    {
        using x3::double_;
        using x3::phrase_parse;
        using ascii::space;

        bool r = phrase_parse(
            first,                          // Start Iterator
            last,                           // End Iterator
            double_ >> *(',' >> double_),   // The Parser
            space                           // The Skip-Parser
        );
        if (first != last) // fail if we did not get a full match
            return false;
        return r;
    }
    /*
    ///////////////////////////////////////////////////////////////////////////
    //  Our number list generator
    ///////////////////////////////////////////////////////////////////////////
    //[tutorial_karma_numlist1
    template <typename OutputIterator>
    bool generate_numbers(OutputIterator& sink, std::list<double> const& v)
    {
        using karma::double_;
        using karma::generate_delimited;
        using ascii::space;

        bool r = generate_delimited(
            sink,                           // destination: output iterator
            double_ << *(',' << double_),   // the generator
            space,                          // the delimiter-generator
            v                               // the data to output 
        );
        return r;
    }
    //]
    */
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
  //Serial << "Boost Spirit Karma Generator" << endl;
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
        std::list<double> v;      // here we put the data to generate
        if (client::parse_numbers(str.begin(), str.end()))
        {
            Serial << "---------------------\n";
            Serial << "Parsing succeeded\n";
            Serial << str.c_str() << " Parses OK: " << endl;
        }
            /*
            std::string generated;
            std::back_insert_iterator<std::string> sink(generated);
            if (!client::generate_numbers(sink, v))
            {
                Serial << "-------------------------\n";
                Serial << "Generating failed\n";
                Serial << "-------------------------\n";
            }
            else
            {
                Serial << "-------------------------\n";
                Serial << "Generated: " << generated.c_str() << "\n";
                Serial << "-------------------------\n";
            }
          } */
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
