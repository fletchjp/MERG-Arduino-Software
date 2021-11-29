// RP2040FCPP
// Demo of FC++ Maybe operation
// This is modified from the version which will work on ARDUINO DUE and also on AVR boards.
// This is more or less a duplicate of DUEFCPP on the RP2040.

// 3rd party libraries
#include <Streaming.h>

#undef F
#undef min

#include <iostream>


/// Solution to the sync_synchronize problem
/// https://www.vexforum.com/t/i-am-learning-arm-none-eabi-compiler-recently-i-want-to-ask-some-questions/73973
extern "C" void __sync_synchronize() {}

// This will now work with FC++ as an Arduino library - just include "prelude.h" and it will find it.
#include "fcpp_prelude.h"


//using namespace std;

using namespace fcpp;

// This comes from the cdc_multi example
/// Helper: non-blocking "delay" alternative.
boolean delay_without_delaying(unsigned long time) {
  /// return false if we're still "delaying", true if time ms has passed.
  /// this should look a lot like "blink without delay"
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
  while (!delay_without_delaying(10000) ) { };
  Serial << "> RP2040FCPP ** " << __FILE__ << endl;
  Serial << "Some simple FC++ operations" << endl;
  Serial << "plus(1,2) = " << fcpp::plus(1,2) << endl;
  Serial << "plus(1.5,2.3) = " << fcpp::plus(1.5,2.3) << endl;
  Serial << "plus(1)(2) = " << fcpp::plus(1)(2) << endl;
  Serial << "inc(1) = " << inc(1) << endl;
  Serial << "--------------------------" << endl;

  Maybe<int> mx(2), my(3);
  Maybe<int> mz;
  if (mz.is_nothing()) Serial << "mz is nothing" << endl;
  mz = just(fcpp::plus(mx.value(),my.value()));
  if (mz.is_nothing()) { 
     Serial << "mz is nothing" << endl;
  } else {
     Serial << "mz has " << mz.value() << endl;
  }
  Serial << "--------------------------" << endl;

  List<double>::iterator idx1, idx2;
  List<double> x1 = list_with(0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0);
  List<double> x2 = fcpp::map(fcpp::minus(1.0),x1);
  double sumx1 = foldl(fcpp::plus,0.,x1);
  for (idx1 = x1.begin(), idx2 = x2.begin(); idx1 != x1.end(); ++idx1, ++idx2) {
     Serial << *idx1 << " " << *idx2 << endl;
  }
  Serial << "Sum of x1 = " << sumx1 << endl;
  Serial << "--------------------------" << endl;

  List<int> ix1 = enumFromTo(0,10);
  List<bool> bodds = fcpp::map(odd,ix1);
  List<bool> bevens = fcpp::map(even,ix1);
  List<int> odds = filter(odd,ix1);
  int sum_odds = foldl(fcpp::plus,0,odds);
  int sum_odds2 = foldl(fcpp::plus,0,filter(odd,ix1));
  List<int>::iterator iix1;
  List<bool>::iterator ib1, ib2;
  Serial << "Length of bodds is " << length(bodds) << endl;
  for (iix1 = ix1.begin(), ib1 = bodds.begin(), ib2 = bevens.begin(); iix1 != ix1.end(); ++iix1, ++ib1, ++ib2) {
     Serial << *iix1 << " " << *ib1 << " " << *ib2 << endl;
  }

  Serial << "Length of odds is " << length(odds) << endl;
  Serial << "sum of the odds is " << sum_odds << endl;

}

void loop() {
  // put your main code here, to run repeatedly:

}
