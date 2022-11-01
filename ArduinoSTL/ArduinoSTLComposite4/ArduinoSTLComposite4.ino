// Arduino STL Composite4
// Modified from modernescpp.com example.
// This example is now working.
// I think there is quite a lot of promise in having this structure available.
// Adding some things from refactoring.guru design

#include <ArduinoSTL.h>

#include <vector>
//#include <algorithm>
#include <string>

#include "composite.h"

using namespace std;

vector<int> trial;



void setup() {
  Serial.begin(115200); 
  printf("STL Composite Pattern\n");
  for (int i = 0; i < 5; i++) trial.push_back(i);
  // range-based for loop
  for (auto i : trial) cout << i << " ";
  cout << endl;
  cout << "Test of Composite Pattern" << endl;

  Composite composite("composite");
  Composite composite1("composite1");
  composite.print_();
  Leaf leaf1("leaf1");
  Leaf leaf2("leaf2");
  Leaf leaf3("leaf3");
  leaf1.print_();
  cout << endl;
  composite1.Add(&leaf1);
  composite1.Add(&leaf2);
  composite.Add(&composite1);
  composite.Add(&leaf3);

  composite1.print_();
  cout << endl;

  composite.print_();
  cout << endl;

  composite.Remove(&composite1);
  
  composite.print_();
  cout << endl;
}

void loop() {
  
}
