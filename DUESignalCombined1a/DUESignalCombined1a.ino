// DUESignalCombined1a
// Exploring use of FC++ from DUEobserver2
// The objective is to get rid of the CRTP used for EventHandler.
// Further changes to combine Observer and EventHandler classes as Observer.
// FC++ ONLY
// I am changing this to be 1a as version 2 is going to be something different.

// SignalCombined
// Based on Arduino STL Composite5 and Observer6
// Modified from modernescpp.com example.
// This example is now working.
// I think there is quite a lot of promise in having this structure available.
// Adding some things from refactoring.guru design
// Adding support for multiple parents.
/////////////////////////////////////////////////////////////////////////////////
// This does not have enough memory on a UNO.
// I am using a MEGA
/////////////////////////////////////////////////////////////////////////////////

#include <ArduinoSTL.h>

#include <boost_1_51_0.h>
// This is used in FC++ to allow for boost usage.
#define FCPP_ARDUINO_BOOST

#include <vector>
#include <map>
//#include <algorithm>
#include <string>

#include "fcpp/prelude.h"
using namespace fcpp;
#define USE_FCPP

#include "composite.h"

#include "observer.h"

using namespace std;

vector<int> trial;

// These are Event numbers which need to be distinct in each class where they are used.
// I could use different event nos for different senders to tell them apart.
static int TURN_ON = 0;
static int TURN_OFF = 1;

#ifdef USE_FCPP
class ConcreteSubject : public Subject {
   int state;
public:
   ConcreteSubject(int const&i = 0) : state(i) {}
   int get_state() const { return state; }
   void inc() {
     state++;
     cout << "s: About to notify new state" << endl;
     notify();
     cout << "s: New state notified" << endl;
   }
};

#endif

enum class Object_Type : int { Track_type, Section_type, Signal_type, LED_type, Detector_type };

class Track : public Composite {
  const Object_Type type_ = Object_Type::Track_type; 
public:
   Track(const string& n) : Composite(n) { }
   Object_Type Get_Type() const { return type_; }
};

class Section : public Composite, public  Observer
{
  const Object_Type type_ = Object_Type::Section_type; 
public:
   Section(const string& n) : Composite(n) { 
      auto p =  fcpp::curry( fcpp::ptr_to_fun(&Section::turnON), this);
      auto q =  fcpp::curry( fcpp::ptr_to_fun(&Section::turnOFF), this);
      handlers[TURN_ON] = p;
      handlers[TURN_OFF] = q;
   }
   Object_Type Get_Type() const { return type_; }
private:
   int turnON()
   {
      std::cout << GetName() << " section Turn on" << std::endl;
      return TURN_ON;
   }
   int turnOFF()
   {
      std::cout << GetName() << " section Turn off" << std::endl;
      return TURN_OFF;
   }
};

class Signal : public Composite, public  Observer
{
   const Object_Type type_ = Object_Type::Signal_type; 
public:
   Signal(const string& n) : Composite(n) { 
      auto p =  fcpp::curry( fcpp::ptr_to_fun(&Signal::turnON), this);
      auto q =  fcpp::curry( fcpp::ptr_to_fun(&Signal::turnOFF), this);
      handlers[TURN_ON] = p;
      handlers[TURN_OFF] = q;
   }
   Object_Type Get_Type() const { return type_; }
private:
   int turnON()
   {
      std::cout << GetName() << " signal Turn on" << std::endl;
      return TURN_ON;
   }
   int turnOFF()
   {
      std::cout << GetName() << " signal Turn off" << std::endl;
      return TURN_OFF;
   }
};

class LED : public Leaf {
   const Object_Type type_ = Object_Type::LED_type; 
public:
   LED(const string& n) : Leaf(n)  { }
   Object_Type Get_Type() const { return type_; }
};

class Detector : public Leaf,  public Subject {
   const Object_Type type_ = Object_Type::Detector_type; 
public:
   Detector(const string& n) : Leaf(n) { }
   Object_Type Get_Type() const { return type_; }
};

void setup() {
 ::delay(2000);
  Serial.begin(115200); 
  while (!Serial) { ::delay(10); }
  ::delay(2000);
  printf("Signal Combined Patterns\n");
  for (int i = 0; i < 5; i++) trial.push_back(i);
  // range-based for loop
  for (auto i : trial) cout << i << " ";
  cout << endl;
  cout << "Test of Signal Composite Pattern" << endl;

#ifdef USE_FCPP
// This is out of use.
  //ConcreteSubject s0, s1(1);
  //Observer obsA(s0);
  //Observer obsB(s1);
  //s0.inc();
  //s1.inc();
#else
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
#endif
  cout << "====================================================================" << endl;
  cout << "Signal Composite experiments" << endl;
  cout << "====================================================================" << endl;

  Track down_line("down_line");

  Section section1("section_1");
  Section section2("section_2");
  
  Signal home_signal("home");
  Signal starter_signal("starter");

// Leaf definitions

  LED home_red("home_red");
  LED home_yellow("home_yellow");
  LED home_green("home_green");

  LED starter_red("starter_red");
  LED starter_yellow("starter_yellow");
  LED starter_green("starter_green");

  Detector home_detector("home_detector");
  Detector starter_detector("starter_detector");

  home_signal.Add(&home_red);
  home_signal.Add(&home_yellow);
  home_signal.Add(&home_green);

  starter_signal.Add(&starter_red);
  starter_signal.Add(&starter_yellow);
  starter_signal.Add(&starter_green);

  section1.Add(&home_signal);
  section1.Add(&home_detector);
  section1.Add(&starter_detector);

  cout << home_signal.GetName() << " has " << home_signal.Operation() << endl;

  cout << starter_signal.GetName() << " has " << starter_signal.Operation() << endl;
  cout << section1.GetName() << " has " << section1.Operation() << endl;

  section2.Add(&starter_signal);
  section2.Add(&starter_detector);
 
  cout << section2.GetName() << " has " << section2.Operation() << endl;

  down_line.Add(&section1);
  down_line.Add(&section2);
  
  cout << down_line.GetName() << " has " << down_line.Operation() << endl;

  down_line.Remove(&section2);
  cout << down_line.GetName() << " has " << down_line.Operation() << endl;

  cout << "====================================================================" << endl;
  cout << "Start of Signal Observer experiments" << endl;
  cout << "====================================================================" << endl;


  std::cout << "home_detector has " << home_detector.numberOfObservers() << " observers" << std::endl;
  std::cout << "Call to registerObserver for home_detector";
  if (home_detector.registerObserver(home_signal)) {
    std::cout << " succeeded for " << home_signal.GetName() << " signal" << std::endl;
  }
  else std::cout << " failed" << std::endl;
  std::cout << "Call to registerObserver for home_detector";
  if (home_detector.registerObserver(section1)) {
    std::cout << " succeeded for " << section1.GetName() << std::endl;
  }
  else std::cout << " failed" << std::endl;
  std::cout << "home_detector has " << home_detector.numberOfObservers() << " observers" << std::endl;
  std::cout << "Call to registerObserver for starter_detector";
  if (starter_detector.registerObserver(home_signal)) 
      std::cout << " succeeded for " << home_signal.GetName() << " signal" << std::endl;
  std::cout << "Call to registerObserver for starter_detector";
  if (starter_detector.registerObserver(starter_signal)) 
      std::cout << " succeeded for " << starter_signal.GetName() << " signal" << std::endl;
  std::cout << "starter_detector has " << starter_detector.numberOfObservers() << " observers" << std::endl;
//#ifndef USE_FCPP
  home_detector.notifyObservers(TURN_ON);
  
  home_detector.notifyObservers(TURN_OFF);

  cout << "====================================================================" << endl;
  cout << "End of Signal Combined experiments" << endl;
  cout << "====================================================================" << endl;


}

void loop() {
  
}
