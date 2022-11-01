// SignalComposite
// Based on Arduino STL Composite5
// Modified from modernescpp.com example.
// This example is now working.
// I think there is quite a lot of promise in having this structure available.
// Adding some things from refactoring.guru design
// Adding support for multiple parents.

#include <ArduinoSTL.h>

#include <vector>
//#include <algorithm>
#include <string>

#include "composite.h"

using namespace std;

vector<int> trial;

class Section : public Composite {

public:
   Section(const string& n) : Composite(n) { }
   
};

class Signal : public Composite {

public:
   Signal(const string& n) : Composite(n) { }
   
};


class LED : public Leaf {
  
public:
   LED(const string& n) : Leaf(n) { }

};

class Detector : public Leaf {
  
public:
   Detector(const string& n) : Leaf(n) { }

};




void setup() {
  Serial.begin(115200); 
  printf("Signal Composite\n");
  for (int i = 0; i < 5; i++) trial.push_back(i);
  // range-based for loop
  for (auto i : trial) cout << i << " ";
  cout << endl;
  cout << "Test of Signal Composite Pattern" << endl;

  Composite composite("composite");
  Composite composite1("composite1");
  composite.print_();
  Leaf leaf1("leaf1");
  Leaf leaf2("leaf2");
  Leaf leaf3("leaf3");
  Leaf leaf4("leaf4");
  leaf1.print_();
  cout << endl;
  composite1.Add(&leaf1);
  composite1.Add(&leaf2);
  composite.Add(&composite1);
  composite.Add(&leaf3);
  composite1.Add(&leaf3);

  composite1.print_();
  cout << endl;

  composite.print_();
  cout << endl;

  cout << composite.Operation() << endl;

  cout << leaf3.GetName() << " has parent(s) " << leaf3.GetParentNames() << endl;

  composite.Remove(&composite1);
  
  composite.print_();
  cout << endl;

  
  composite1.Remove(&leaf3);
  cout << leaf3.GetName() << " has parent(s) " << leaf3.GetParentNames() << endl;

  cout << leaf4.GetName() << " has parent(s) " << leaf4.GetParentNames() << endl;

  cout << composite.GetName() << " has parent(s) " << composite.GetParentNames() << endl;

  cout << "====================================================================" << endl;
  cout << "Signal Composite experiments" << endl;
  cout << "====================================================================" << endl;

  //Section section1("section_1");
  
  Signal home_signal("home");
  Signal starter_signal("starter");

  LED home_red("home_red");
  LED home_yellow("home_yellow");
  LED home_green("home_green");

  LED starter_red("starter_red");
 // LED starter_yellow("starter_yellow");
/*
  LED starter_green("starter_green");
*/
  //Detector home_detector("home_detector");
  
  home_signal.Add(&home_red);
  home_signal.Add(&home_yellow);
  home_signal.Add(&home_green);

//  section1.Add(&home_signal);
//  section1.Add(&home_detector);

  cout << home_signal.GetName() << " has " << home_signal.Operation() << endl;

  //cout << section1.GetName() << " has " << section1.Operation() << endl;



  //Detector starter_detector("starter_detector");

  starter_signal.Add(&starter_red);
/*
  starter_signal.Add(&starter_yellow);
  starter_signal.Add(&starter_green);

*/
  cout << starter_signal.GetName() << " has " << starter_signal.Operation() << endl;


}

void loop() {
  
}
