// DUEFCPP
// Demo of FC++ Maybe operation
// This will work on ARDUINO DUE but not on AVR boards.

#include "fcpp/prelude.h"

#include <Streaming.h>


using namespace fcpp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  Serial << endl << endl << F("> DUEFCPP ** ") << __FILE__ << endl;
  Serial << plus(1,2) << endl;
  Serial << plus(1.5,2.3) << endl;
  Serial << plus(1)(2) << endl;
  Serial << inc(1) << endl;

  Maybe<int> mx(2), my(3);
  Maybe<int> mz;
  if (mz.is_nothing()) Serial << "mz is nothing" << endl;
  mz = just(plus(mx.value(),my.value()));
  if (mz.is_nothing()) { 
     Serial << "mz is nothing" << endl;
  } else {
     Serial << "mz has " << mz.value() << endl;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
