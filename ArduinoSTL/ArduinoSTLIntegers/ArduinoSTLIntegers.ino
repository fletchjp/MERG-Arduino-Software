// ArduinoSTLIntegers
// This example now works as long as the AVR Boards are 1.8.2 not 1.8.3
// It also will not work with AVR 1.8.5 - multiple definition of std::nothrow.
#include <ArduinoSTL.h>

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
