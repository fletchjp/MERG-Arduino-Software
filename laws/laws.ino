// laws 

// A first attempt to get FC++ functor laws operational

// This comes from the old file unify.cpp which has a lot of explanations.

// This will now work with FC++ as an Arduino library - just include "prelude.h" and it will find it.

#include<ArduinoSTL.h>

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

using namespace fcpp;

using std::cout;
using std::endl;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cout << "========================" << endl;
  cout << "Functor laws examples" << endl;
  cout << "========================" << endl;
  cout << "========================" << endl;

}

void loop() {
  // put your main code here, to run repeatedly:

}
