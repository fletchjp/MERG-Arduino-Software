/// @file ArduinoRP2040BoostSpiritX3YetAnother.ino
/// @brief A different example with error handling
///
/// https://wandbox.org/permlink/Up1mD3FTVbE02tJG
///
/// Moving most of the code into a separate file.
///
/// This example needs exceptions.
///
///


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

#include "yet_another.hpp"

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
  Serial << "ArduinoRP2040BoostSpiritYetAnother ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit X3 operations" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Boost Spirit X3 Parser" << endl;
  Serial << "------------------------------" << endl;

    using It = std::string::const_iterator;
    using D = square::peg::with_error_handling::diags;

   for (std::string const input : { 
            "   -89 0038  ",
            "   \"-89 0038\"  ",
            "   something123123      ;",
            // undecidable
            "",
            // violate expecations, no successful parse
            "   -89 oops  ",   // not an integer
            "   \"-89 0038  ", // missing "
            "   bareword ",    // missing ;
            // trailing debris, successful "main"
            "   -89 3.14  ",   // followed by .14
        })
    {
        Serial << "====== " << /*std::quoted(input) <<*/ "\n";

        It iter = input.begin(), end = input.end();
        Value v;
        std::vector<std::string> diags;

///      This won't compile without exceptions.
        if (parse(iter, end, x3::with<D>(diags) [square::peg::entry_point], v)) {
            //Serial << "Result value: " << v;
        } else {
            Serial << "Parsing failed";
        }

        Serial << " with " << diags.size() << " diagnostics messages: \n";

        for(auto& msg: diags) {
            Serial << " - " << msg << "\n";
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
