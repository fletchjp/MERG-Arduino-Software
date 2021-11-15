/// @file ArduinoRP2040BoostSpiritX3Minimal
///
///
/// Taken from spirit/example/x3/minimal in Boost 1.77.0
///
/// This example shows how to break down the employee example into separate files for different parts of the code.
///
/// What I needed to do was to import all the files except the main file and then adapt this code to run it.
/// This has proved to be fairly easy. I had to edit ast.hpp to change to the output operator based on Serial.
/// There is a difference in the phrase_parse statement where employee has become employee().

/**
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
#include <string>
#include <vector>
#include <list>
#include <cstdio>

#include "ArduinoCode.h"

#include <boost_spirit_home_x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include "ast.hpp"
#include "ast_adapted.hpp"
#include "employee.hpp"


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
  using iterator_type = std::string::const_iterator;
  using client::employee;
  //typedef std::string::const_iterator iterator_type;
  //using client::parser::employee; // Our parser
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
        bool r = phrase_parse(iter, end, employee(), space, emp);

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
