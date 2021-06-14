// ArduinoBoostFunction
// This example now works as long as the AVR Boards are 1.8.2 not 1.8.3
#include <ArduinoSTL.h>
// This is an old version of boost from 2012.
#include <boost_1_51_0.h>
// Example use of a Boost header
#define BOOST_DISABLE_ASSERTS
#define BOOST_NO_EXCEPTIONS
#undef B1
#include <iostream>
//#include <boost/function.hpp>
#include <string>



using namespace std;

void setup() {
  Serial.begin(9600);
}

void loop() {

}
