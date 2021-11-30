l/// @file ArduinoRP2040BoostSpiritX3Custom
/// @brief The employee example using custom error handling.
///
/// Taken from spirit/example/x3/employee and adapted using code from
/// https://stackoverflow.com/questions/61721421/customizing-the-full-error-message-for-expectation-failures-boostspiritx3

// 3rd party libraries
#include <Streaming.h>

#undef F
#undef min

#include <exception>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <cstdio>

#include "ArduinoCode.h"

#include <boost_spirit_home_x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
//#include <boost/fusion/sequence.hpp>
//#include <boost/fusion/include/sequence.hpp>


namespace ast
{
    struct name : std::string, x3::position_tagged {
        using std::string::string;
        using std::string::operator=;
    };
    struct person   : x3::position_tagged { ast::name first_name, last_name; };
    struct employee : x3::position_tagged { int age; person who; double salary; };
    using boost::fusion::operator<<;

    
/*
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
*/
}

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
  //using namespace boost::fusion;

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

            int x = boost::fusion::at_c<0>(emp) ;
            Serial << "test: " << x << endl;

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
