// ArduinoRP2040BoostSpiritX3Employee

// Taken from spirit/example/x3/employee

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
#include <boost/fusion/include/adapt_struct.hpp>
//#include <boost/fusion/include/io.hpp>

namespace client { namespace ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  Our employee struct
    ///////////////////////////////////////////////////////////////////////////
    struct employee
    {
        int age;
        std::string surname;
        std::string forename;
        double salary;
    };

    //using boost::fusion::operator<<;
    // I cannot use the fusion IO so I am instead doing this which works.
    inline Print &operator <<(Print &stream, const employee &emp)
    {
       stream.print("[");
       stream.print(emp.age);
       stream.print(",");
       stream.print(emp.surname.c_str());
       stream.print(",");
       stream.print(emp.forename.c_str());
       stream.print(",");
       stream.print(emp.salary);       
       stream.print("]");
      return stream;
    }

}}

// We need to tell fusion about our employee struct
// to make it a first-class fusion citizen. This has to
// be in global scope.

BOOST_FUSION_ADAPT_STRUCT(client::ast::employee,
    age, surname, forename, salary
)

namespace client
{
    ///////////////////////////////////////////////////////////////////////////////
    //  Our employee parser
    ///////////////////////////////////////////////////////////////////////////////
    namespace parser
    {
        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;

        using x3::int_;
        using x3::lit;
        using x3::double_;
        using x3::lexeme;
        using ascii::char_;

        x3::rule<class employee, ast::employee> const employee = "employee";

        auto const quoted_string = lexeme['"' >> +(char_ - '"') >> '"'];

        auto const employee_def =
            lit("employee")
            >> '{'
            >>  int_ >> ','
            >>  quoted_string >> ','
            >>  quoted_string >> ','
            >>  double_
            >>  '}'
            ;

        BOOST_SPIRIT_DEFINE(employee);
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
  Serial << "Employee structure example.\n";

  Serial << "/////////////////////////////////////////////////////////\n\n";
  Serial << "             An employee parser for Spirit...\n\n";
  Serial << "/////////////////////////////////////////////////////////\n\n";


  using boost::spirit::x3::ascii::space;
  typedef std::string::const_iterator iterator_type;
  using client::parser::employee; // Our parser

  std::string str("employee{33, \"Jones\", \"John\", 300.5 }");
  Serial << str << endl;
  //Serial << str.c_str() << endl;
  int len = str.length();
  int n = 0;
  while (n < 1)
  {
       //if (str.empty() || str[0] == 'q' || str[0] == 'Q')
       //     break;
        n++;

        client::ast::employee emp;
        iterator_type iter = str.begin();
        iterator_type const end = str.end();
        bool r = phrase_parse(iter, end, employee, space, emp);

        if (r && iter == end)
        {
            //Serial << boost::fusion::tuple_open('[');
            //Serial << boost::fusion::tuple_close(']');
            //Serial << boost::fusion::tuple_delimiter(", ");

            Serial << "---------------------\n";
            Serial << "Parsing succeeded\n";
            Serial << "got: " << emp << endl;

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
