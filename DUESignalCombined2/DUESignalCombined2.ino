// DUESignalCombined2
// Exploring use of FC++ from DUEobserver2
// The objective is to get rid of the CRTP used for EventHandler.
// Further changes to combine Observer and EventHandler classes as Observer.
// FC++ ONLY
///////////////////////////////////////////////////////////////////////////////
// This combines the Composite and Observer Patterns.
// All objects declared are Components, Subjects and Observers.
// Components can be either Composite or Leaf.
// Composite components can have other items (Composite or Leaf) as children using Add(&child)
// An composite can have more than one child and any object can have more than one parent.
// Optionally child.RegisterParents() makes the parents of the child into observers as well.
// Any observer has to define handlers which will respond to particular integer values from 
// the subjects being observed. The handlers are member functions and they are indexed using the integer value.
// See the examples in classes Track and Section below.
// It is also possible to make other composite objects observers of any object. 
///////////////////////////////////////////////////////////////////////////////

// SignalCombined
// Based on Arduino STL Composite5 and Observer6
// Modified from modernescpp.com example.
// This example is now working.
// I think there is quite a lot of promise in having this structure available.
// Adding some things from refactoring.guru design
// Adding support for multiple parents.
/////////////////////////////////////////////////////////////////////////////////
// I am using an Arduino DUE for the FC++
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

using namespace std;

#include "combined.h"

//#include "observer.h"


vector<int> trial;

// These are Event numbers which need to be distinct in each class where they are used.
// I could use different event nos for different senders to tell them apart.
static int TURN_ON = 0;
static int TURN_OFF = 1;

enum class Object_Type : int { Track_type, Section_type, Signal_type, LED_type, Detector_type };

class Track : public Composite {
  const Object_Type type_ = Object_Type::Track_type; 
public:
   Track(const string& n) : Composite(n) { }
   Object_Type Get_Type() const { return type_; }
};

class Section : public Composite
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

class Signal : public Composite
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

// Leaf already has public Subject
class Detector : public Leaf //,  public Subject 
{
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
  cout << "====================================================================" << endl;
  printf("Signal Combined Patterns\n");

  cout << "====================================================================" << endl;
  cout << "Signal Combined experiments" << endl;
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

  if (starter_signal.hasParents() )
  {
     cout << starter_signal.GetName() << " has "  << starter_signal.numberOfParents()  << " parent(s)" << endl;
     cout << starter_signal.GetParentNames() << endl;
  } else cout << starter_signal.GetName() << " has no parents" << endl;

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
  if (home_detector.hasParents() )
  {
     cout << home_detector.GetName() << " has "  << home_detector.numberOfParents()  << " parent(s)" << endl;
     cout << home_detector.GetParentNames() << endl;
  } else cout << home_detector.GetName() << " has no parents" << endl;

  home_detector.notifyObservers(TURN_ON);
  
  home_detector.notifyObservers(TURN_OFF);
  if (starter_detector.hasParents() )
  {
     cout << starter_detector.GetName() << " has "  << starter_detector.numberOfParents()  << " parent(s)" << endl;
     cout << starter_detector.GetParentNames() << endl;
     // This means that the parents are also observers
     starter_detector.RegisterParents();
  } else cout << starter_detector.GetName() << " has no parents" << endl;

  starter_detector.notifyObservers(TURN_ON);

  starter_detector.notifyObservers(TURN_OFF);

  cout << "====================================================================" << endl;
  cout << "End of Signal Combined experiments" << endl;
  cout << "====================================================================" << endl;


}

void loop() {
  
}
