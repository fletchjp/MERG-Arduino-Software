//////////////////////////////////////////////////////////////////////////////////
// DUEobserver
//////////////////////////////////////////////////////////////////////////////////
// This is new example to work up the observer model using FC++ techniques.
// I am lucky that there is a good staring point in ecoop1b
// which I chose at random to get going.
// The ecoop examples were written (not by me) to illustrate a paper (reference to be found).
// ecoop1b is a fuller example than is in the paper.
// In the meantime I had made my own implementation which goes further than this.
// I am going to implement parts of my own code here.
//////////////////////////////////////////////////////////////////////////////////
// I have had an interesting time with this. It turns out that the ecoop1b model has a state
// and the one I had has an event code. I have been attempting to combine the two things.
// I have reached the point where that works, but only when both have type int.
// More work is needed on this.
// At the moment I am not sure about which observer is being contacted and why.



// A first attempt to get FC++ working with ArduinoSTL and be able to use std::cout etc.

// This will now work with FC++ as an Arduino library - just include "prelude.h" and it will find it.


#include<ArduinoSTL.h>

#include <boost_1_51_0.h>
// This is used in FC++ to allow for boost usage.
#define FCPP_ARDUINO_BOOST
#include <iostream>
#include <vector>
#include <map>
//This does not compile
//#define FCPP_DEBUG 
#include "fcpp/prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;
using std::vector;

/////////////////////////////////////////////////////////////////////
// version from ecoop1b which has state
/////////////////////////////////////////////////////////////////////

class Subject {
   typedef vector< Fun0<void> > V;
   V observers;
public:
   void attach( Fun0<void> o ) {
      observers.push_back( o );
   }
   void notify() {
      for( V::iterator i=observers.begin(); i!=observers.end(); ++i ) {
         (*i)();
      }
   }
};

class ConcreteSubject : public Subject {
   int state;
public:
   ConcreteSubject() : state(0) {}
   int get_state() const { return state; }
   void inc() {
     state++;
     cout << "s: About to notify new state" << endl;
     notify();
     cout << "s: New state notified" << endl;
   }
};

class ConcreteObserverA {
   ConcreteSubject& subject;
public:
   ConcreteObserverA( ConcreteSubject& s ) : subject(s) {
      s.attach( makeFun0(
       //curry( ptr_to_fun(&ConcreteObserverA::update), this ) ) );
         lazy1( ptr_to_fun(&ConcreteObserverA::update), this ) ) );
   }
   void update() {
      cout << "A's update: new state is " << subject.get_state() << endl;
   }
#ifdef FCPP_DEBUG
   void debug() {
     cout << "sprint ( lazy1(ptr_to_fun(&ConcreteObserverA::update), this ) ) : " << endl
          << sprint ( lazy1(ptr_to_fun(&ConcreteObserverA::update), this ) )
          << endl;
   }
#endif
};

class ConcreteObserverB {
   ConcreteSubject& subject;
public:
   ConcreteObserverB( ConcreteSubject& s ) : subject(s) {
      s.attach( makeFun0(
       //curry( ptr_to_fun(&ConcreteObserverB::get_notification), this ) ) );
       lazy1( ptr_to_fun(&ConcreteObserverB::get_notification), this ) ) );
   }
   void get_notification() {
      cout << "B's get_notification: new state is "
           << subject.get_state() << endl;
   }
};

////////////////////////////////////////////////////////////////////////
// Version from plant.cpp
// This version is more complicated and can carry a payload of type E.
// E defines the event which does not change.
// S defines a state variable which can change.
////////////////////////////////////////////////////////////////////////

// This is used to keep the constructor happy.
// This is a problem as it assumes E is type int.
int nullfun(int i) { std::cout << "nullfun" << std::endl; return 0; } 

fcpp::Fun1<int,int> nullfun11 = fcpp::ptr_to_fun(&nullfun);

template <class E>
class BareSubject {
  // At the moment this only stores one function to be called.
  // This needs to be made into a vector etc.
public:
  typedef typename  fcpp::Fun1<E,E> fun_type;
  //typedef std::vector<fun_type> V;
  typedef E Event;
  E event_;
private:
  fun_type f_ ;
  typedef std::pair<E, fun_type> fcpp_value_type;
  typedef std::vector<fcpp_value_type> fcpp_container;
  fcpp_container fcpp_data_;
  
public:
  BareSubject() : f_(nullfun11) { }
  void Attach(fun_type f, E e) {  
    //std::cout << "Attach has event " << e << std::endl;
    f_ = f; fcpp_data_.push_back(std::make_pair(e,f)); 
    event_ = e; 
    //f_(e); 
    }
  E Notify(E e) { 
    //std::cout << "Did Notify get called?" << std::endl;
    //std::cout << "f(" << event_ << ") = " << std::endl;  
      f_(event_); return event_;
    }
  E get_event(E e) { return event_; }
  void Flush() { f_(event_); }
  void NotifyAll() {    
    for (fcpp_iterator i = fcpp_data_.begin(); i != fcpp_data_.end(); ++i ) {
      (i->second)(i->first);
    }
  }
  void ListAll() {
    std::cout << "List all events stored" << std::endl;
    for (fcpp_iterator i = fcpp_data_.begin(); i != fcpp_data_.end(); ++i ) {
      std::cout << (i->first) << /*" " << (i->second)(i->first) <<*/ std::endl;
    }
  }
  int get_state() const { return 0; }
  int get_index() const { return 0; }
protected:
  typedef typename fcpp_container::iterator fcpp_iterator;
};

template <class S,class E>
class StateSubject : public BareSubject<E> {
   S state;
   int index;
public:
   StateSubject() : state(0), index(0) {}
   StateSubject(const int &i) : state(0), index(i) {}
   int get_index() const { return index; }
   S get_state() const { return state; }  
   void inc() {
     state++;
     //cout << "ss: About to notify new state" << endl;
     // Some of this is to fool the compiler.
     E e = BareSubject<E>::get_event(e);
     BareSubject<E>::Notify(e);
     //cout << "ss: New state notified" << endl;
   }
};


template <class Subject>
class ConcreteObserver {
public:
  typedef typename Subject::Event Event;
private:
  Subject& subject_;
  typedef std::vector<std::pair<Event,const Subject> > EStype;
  typedef std::map<const Event,std::pair<Event, const Subject> > ESmap;
  EStype ES; 
  ESmap ESm;
public:
  Event event_;
  ConcreteObserver () { }  
  ConcreteObserver (Subject &s, Event e) : subject_(s), event_(e) {
    s.Attach( fcpp::curry2( fcpp::ptr_to_fun( &ConcreteObserver::be_notified), this), e);
    ES.push_back(std::make_pair(e,s));
    ESm.insert(std::make_pair(e,std::make_pair(e,s)));
  }
  void AddSubject(Subject &s, Event e) {
    //std::cout << "AddSubject has event " << e << std::endl;
    s.Attach( fcpp::curry2( fcpp::ptr_to_fun( &ConcreteObserver::be_notified), this), e);
    ES.push_back(std::make_pair(e,s));
    ESm.insert(std::make_pair(e,std::make_pair(e,s)));
//  ESm.insert(e,s);
    //s.Flush();
  }
  int be_notified(Event e) {
    event_ = e;
    // This returns the state from the first subject attached.
    std::cout << "New event is " << event_ << " with state " << subject_.get_state() 
              << " from index " << subject_.get_index()<< std::endl;
    // This finds the correct subject but the wrong version with the original state. 
    //std::cout << "New event is " << event_ << " with state " << ESm[e].second.get_state() 
    //          << " from index " << ESm[e].second.get_index() << std::endl;
    //std::cout << "New event is " << event_ << " with state " << ESm[e].first << std::endl;
    return event_;
  }
  ConcreteObserver &operator += (Subject &s) { AddSubject(s,event_); return this; }
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) { ::delay(10); }
  cout << "========================" << endl;
  cout << "ecoop1b Observer example" << endl;
  cout << "========================" << endl;
   ConcreteSubject s;
   ConcreteObserverA ao(s);
   ConcreteObserverB bo(s);
   //(void)ao;
   //(void)bo;
   s.inc();
   s.inc();
#ifdef FCPP_DEBUG
   ao.debug();
#endif
  cout << "========================" << endl;
  cout << "New versions" << endl;
  cout << "========================" << endl;
 
  typedef BareSubject<int> BareConcrete;
  typedef StateSubject<int,int> StateConcrete;

  // This FC++ observer has no knowledge of the other stuff.
  BareConcrete bareconcrete10,bareconcrete11;
  // The ConcreteObserver class is identical.
  ConcreteObserver<BareConcrete> bareobserver1(bareconcrete10,10);  
  ConcreteObserver<BareConcrete> bareobserver2(bareconcrete11,11);  
  ConcreteObserver<BareConcrete> bareobserver3(bareconcrete11,12); 
  bareobserver1.AddSubject(bareconcrete11,14);

  int e = 10;
  bareconcrete11.ListAll();
  std::cout << "============================================" << std::endl;
  e = bareconcrete10.Notify(e);
  std::cout << "============================================" << std::endl;
  bareconcrete11.NotifyAll();
  std::cout << "============================================" << std::endl;
  StateConcrete stateconcrete20(20),stateconcrete21(21),stateconcrete22(22);
  ConcreteObserver<StateConcrete> stateobserver1(stateconcrete20,20);
  ConcreteObserver<StateConcrete> stateobserver2(stateconcrete21,21);
  stateobserver1.AddSubject(stateconcrete22,22);
  stateconcrete20.inc();
  stateconcrete20.inc();
  stateconcrete20.inc();
  stateconcrete21.inc();
  stateconcrete22.inc(); // This returns state the same as no 20 when I think it should be 1.
  // It is coming from no 20 and not no 22!
  std::cout << "============================================" << std::endl;

}

void loop() {
  // put your main code here, to run repeatedly:

}
