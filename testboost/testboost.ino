// test boost

#include <ArduinoSTL.h>

#include <boost_1_51_0.h>
#define FCPP_ARDUINO_BOOST

#include <iostream>
// Need 5 parameter functoids for example.
#define FCPP152
#define FCPP_ENABLE_LAMBDA
#define FCPP_USE_PROMOTE
#define FCPP_DEBUG
#define FCPP_EXTRA_STRONG_CURRY
#define FCPP_PATTERN // define this to include pattern.h
#include "prelude.h"
#include "functors.h"
//#include "comonad.h"

using namespace fcpp;

// These are used in fcpp
//#include <boost/mpl/eval_if.hpp>
//#include <boost/type_traits/is_void.hpp>
//#include <boost/type_traits/is_integral.hpp>

//using std::cout;
//using std::endl;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
