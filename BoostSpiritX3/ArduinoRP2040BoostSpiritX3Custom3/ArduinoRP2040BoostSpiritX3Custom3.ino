/// @file ArduinoRP2040BoostSpiritX3Custom3.ino
/// @brief The employee example using custom error handling.
///
/// Taken from spirit/example/x3/employee and adapted using code from
/// https://stackoverflow.com/questions/61721421/customizing-the-full-error-message-for-expectation-failures-boostspiritx3
///
/// Moving most of the code into a separate file.
///
/// I now have the annotation of success working. The error message is not being called.
/// In this version the code is moved to custom.hpp. The result is unchanged.
///
/// I have also added some code to look at the situation on failure.
///
/// This example now finds the error information for the first error without using exceptions. 
///
/// This depends on a modification to Boost Spirit X3 in file \boost\spirit\home\x3\directive\expect.hpp.
/// This is only active if BOOST_SPIRIT_X3_NO_EXCEPTION is defined.
/// 
/// The modification uses a global variable called boost::spirit::x3::where_was_I defined as std::vector<std::string>
///
/// This can be inspected when there is a failure. This uses the custom diagnostics handler which is not problem dependent.


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

/// This switches off exceptions and provides a different way to get error information.
#define BOOST_SPIRIT_X3_NO_EXCEPTION
#include <boost_spirit_home_x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
//#include <boost/fusion/sequence.hpp>
//#include <boost/fusion/include/sequence.hpp>

#include "custom.hpp"

// We need to tell fusion about our employee struct
// to make it a first-class fusion citizen. This has to
// be in global scope.
BOOST_FUSION_ADAPT_STRUCT(ast::person, first_name, last_name)
BOOST_FUSION_ADAPT_STRUCT(ast::employee, age, who, salary)





//////////////////////////////////////////////////////////
static std::string const
    good_input = R"({ 23, "Amanda", "Stefanski", 1000.99 },
        { 35, "Angie", "Chilcote", 2000.99 }
    )", 
    bad_input = R"(
        { 23,
 'Amanda', "Stefanski", 1000.99 },
    )";
//////////////////////////////////////////////////////////

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
  Serial << "ArduinoRP2040BoostSpiritCustom ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit X3 operations" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Boost Spirit X3 Parser" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Employee structure example.\n";

  Serial << "/////////////////////////////////////////////////////////\n\n";
  Serial << "             An employee parser for Spirit...\n\n";
  Serial << "/////////////////////////////////////////////////////////\n\n";

  Serial << "With custom diagnostics only:" << endl;
  /// Good input case
  parse(good_input);

  Serial << "\n\n ----- Now with parse error:" << endl;
  /// Bad input case
  parse(bad_input);

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
