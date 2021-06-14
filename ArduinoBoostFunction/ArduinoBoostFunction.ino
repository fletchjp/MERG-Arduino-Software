// ArduinoBoostFunction
// This example now works as long as the AVR Boards are 1.8.2 not 1.8.3
#include <ArduinoSTL.h>
// This is an old version of boost from 2012.
#include <boost_1_51_0.h>
// Example use of a Boost header

//////////////////////////////////////////////////////////////////////
// boost::function
// I set out to work with an example of boost function.
// It has taken some time to get the header to compile.
// The problem is that boost::function throws exceptions
// which are not supported by the Arduino C++.
// The solution is to put some code here which will
// satsify the compiler.
//////////////////////////////////////////////////////////////////////
#define BOOST_DISABLE_ASSERTS
#define BOOST_NO_EXCEPTIONS
#define BOOST_EXCEPTION_DISABLE
#undef B1
#undef F  // This is needed for the DUE.
#include <iostream>
#include <exception>
#include <stdexcept>

namespace std {

   class runtime_error {
   public:
   runtime_error( std::exception & e ) { }
   runtime_error( std::string const & s ) { }
  };
}

namespace boost {

  void throw_exception( std::exception & e ) { }
  void throw_exception(const std::exception & e ) { }
  void throw_exception( std::runtime_error& e) { }
  void throw_exception(const std::runtime_error& e) { }

}

#include <boost/function.hpp>
#include <string>

/*
  class SomeError : public std::runtime_error {
    public:
      SomeError() : std::runtime_error(std::string("Some error")) {}
  };
*/

using namespace std;

int f0()
{
  return 0;
}

int f1(int x)
{
  return x + x;
}

int f2(int x, int y)
{
  return x + y;
}

void setup() {
  Serial.begin(9600);
}

void loop() {

}
