// BOTHFCPP
// Demo of FC++ Maybe operation
// This will work on ARDUINO DUE and also on AVR boards.
// The secret is to include this to have the std library.
#include<ArduinoSTL.h>

#include "prelude.h"

using namespace std;

using namespace fcpp;


void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  cout << "> BOTHFCPP ** " << __FILE__ << endl;
  cout << "Some simple FC++ operations" << endl;
  cout << "plus(1,2) = " << fcpp::plus(1,2) << endl;
  cout << "plus(1.5,2.3) = " << fcpp::plus(1.5,2.3) << endl;
  cout << "plus(1)(2) = " << fcpp::plus(1)(2) << endl;
  cout << "inc(1) = " << inc(1) << endl;
  cout << "--------------------------" << endl;

  Maybe<int> mx(2), my(3);
  Maybe<int> mz;
  if (mz.is_nothing()) cout << "mz is nothing" << endl;
  mz = just(fcpp::plus(mx.value(),my.value()));
  if (mz.is_nothing()) { 
     cout << "mz is nothing" << endl;
  } else {
     cout << "mz has " << mz.value() << endl;
  }
  cout << "--------------------------" << endl;

  List<double>::iterator idx1, idx2;
  List<double> x1 = list_with(0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0);
  List<double> x2 = fcpp::map(fcpp::minus(1.0),x1);
  double sumx1 = foldl(fcpp::plus,0.,x1);
  for (idx1 = x1.begin(), idx2 = x2.begin(); idx1 != x1.end(); ++idx1, ++idx2) {
     cout << *idx1 << " " << *idx2 << endl;
  }
  cout << "Sum of x1 = " << sumx1 << endl;
  cout << "--------------------------" << endl;

  List<int> ix1 = enumFromTo(0,10);
  List<bool> bodds = fcpp::map(odd,ix1);
  List<bool> bevens = fcpp::map(even,ix1);
  List<int> odds = filter(odd,ix1);
  int sum_odds = foldl(fcpp::plus,0,odds);
  int sum_odds2 = foldl(fcpp::plus,0,filter(odd,ix1));
  List<int>::iterator iix1;
  List<bool>::iterator ib1, ib2;
  cout << "Length of bodds is " << length(bodds) << endl;
  for (iix1 = ix1.begin(), ib1 = bodds.begin(), ib2 = bevens.begin(); iix1 != ix1.end(); ++iix1, ++ib1, ++ib2) {
     cout << *iix1 << " " << *ib1 << " " << *ib2 << endl;
  }

  cout << "Length of odds is " << length(odds) << endl;
  cout << "sum of the odds is " << sum_odds << endl;

}

void loop() {
  // put your main code here, to run repeatedly:

}
