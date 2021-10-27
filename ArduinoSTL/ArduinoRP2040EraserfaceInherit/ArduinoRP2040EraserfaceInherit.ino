//////////////////////////////////////////////////////////////////////
// Arduino RP2040 Eraserface Inherit
// This is an implementation of the example eraserface_inherit.cpp
// https://github.com/badair/eraserface
//////////////////////////////////////////////////////////////////////

#undef F
#include <memory>
#include <boost_callable_traits.hpp>
#include <type_traits>
#include <vector>
#include <boost_function.hpp>
#include <eraserface.hpp>

#include <Streaming.h>

using std::vector;

typedef boost::function0<void> F_type;


void setup() {
  Serial.begin(115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 10000));
  t2 = millis() - t1;
  Serial.print("Waited for ");
  Serial.print(t2);
  Serial.println(" millis");
  Serial << "Eraserface inherit example" << endl;

}

void loop() {
  // put your main code here, to run repeatedly:

}
