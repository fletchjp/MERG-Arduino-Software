// SignalCombined3
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
// This now uses DesignPatterns library.
/////////////////////////////////////////////////////////////////////////////////

#include <ArduinoSTL.h>

#include <vector>
//#include <algorithm>
#include <string>

// Now in DesignPatterns library
#include <combined.h>

using namespace std;

// These are Event numbers which need to be distinct in each class where they are used.
// I could use different event nos for different senders to tell them apart.
#define TURN_ON  0
#define TURN_OFF 1

#define SIGNAL_ON 2
#define SIGNAL_OFF 3

enum class Object_Type : int { Track_type, Section_type, Signal_type, LED_type, Detector_type };

class Track : public Composite {
  const Object_Type type_ = Object_Type::Track_type; 
public:
   Track(const string& n) : Composite(n) { }
   Object_Type Get_Type() const { return type_; }
};

class Section : public Composite, public EventHandler<Section> {
  const Object_Type type_ = Object_Type::Section_type; 
public:
   Section(const string& n) : Composite(n) { 
      handlers[TURN_ON] = &Section::turnON; 
      handlers[TURN_OFF] = &Section::turnOFF;     
   }
   Object_Type Get_Type() const { return type_; }
private:
   void turnON(Subject *subject)
   {
      std::cout << GetName() << " section Turn on" << std::endl;
   }
   void turnOFF(Subject *subject)
   {
      std::cout << GetName() << " section Turn off" << std::endl;
   }
};

// Signal is now both a Subject and an Observer.
class Signal : public Composite, public EventHandler<Signal> {
   const Object_Type type_ = Object_Type::Signal_type; 
public:
   Signal(const string& n) : Composite(n) { 
      handlers[TURN_ON] = &Signal::turnON; 
      handlers[TURN_OFF] = &Signal::turnOFF;     
   }
   Object_Type Get_Type() const { return type_; }
private:
   void turnON(Subject *subject)
   {
      std::cout << GetName() << " signal Turn on" << std::endl;
      notifyObservers(SIGNAL_ON);
   }
   void turnOFF(Subject *subject)
   {
      std::cout << GetName() << " signal Turn off" << std::endl;
      notifyObservers(SIGNAL_OFF);
  }
};


class LED : public Leaf {
   const Object_Type type_ = Object_Type::LED_type; 
public:
   LED(const string& n) : Leaf(n)  { }
   Object_Type Get_Type() const { return type_; }
};

class Detector : public Leaf {
   const Object_Type type_ = Object_Type::Detector_type; 
public:
   Detector(const string& n) : Leaf(n) { }
   Object_Type Get_Type() const { return type_; }
};

void setup() {
  delay(2000);
  Serial.begin(115200); 
  while (!Serial) { }
  delay(2000);
  printf("Signal Combined Patterns\n");
  cout << "====================================================================" << endl;
  cout << "Test of Signal Combined Pattern 3" << endl;
  cout << "Signal Composite experiments with DesignPatterns library" << endl;
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
  cout << "====================================================================" << endl;
  std::cout << "home_signal has " << home_signal.numberOfObservers() << " observers" << std::endl;
  std::cout << "Call to registerObserver for home_detector";
  if (home_signal.registerObserver(section1)) {
    std::cout << " succeeded for " << section1.GetName() << std::endl;
  }
  else std::cout << " failed" << std::endl;
  std::cout << "home_signal has " << home_signal.numberOfObservers() << " observers" << std::endl;
 cout << "====================================================================" << endl;


  home_detector.notifyObservers(TURN_ON);
  
  home_detector.notifyObservers(TURN_OFF);


  cout << "====================================================================" << endl;
  cout << "End of Signal Combined experiments" << endl;
  cout << "====================================================================" << endl;


}

void loop() {
  
}
