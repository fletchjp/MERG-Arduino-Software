// ArduinoBoost166Integers
// This example now works as long as the AVR Boards are 1.8.2 not 1.8.3
#include <ArduinoSTL.h>
// This is an old version of boost from 2012.
//#include <boost_1_51_0.h>
//#include <typeinfo>
//#include <array>
// Example of use with Boost for Arduino
// replace / with _ in nameing of headers.
#include <boost_type_traits_is_void.hpp>

using namespace std;

void setup() {
  Serial.begin(9600);
  cout << "Feed me an integers." << endl;
}

void loop() {
  int foo;
  if (cin >> foo) { 
    cout << "You fed me " << foo << endl;
  }else{
    cin.clear();
    cin.ignore();
  }
}
