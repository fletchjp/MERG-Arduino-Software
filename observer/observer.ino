// observer
// This is new example to work up the observer model using FC++ techniques.
// I am lucky that there is a good staring point in ecoop1b
// which I chose at random to get going.
// The ecoop examples were written (not by me) to illustrate a paper (reference to be found).
// ecoop1b is a fuller example than is in the paper.
// In the meantime I had made my own implementation which goes further than this.
// I am going to implement parts of my own code here.

// A first attempt to get FC++ working with ArduinoSTL and be able to use std::cout etc.

// This will now work with FC++ as an Arduino library - just include "prelude.h" and it will find it.

#include<ArduinoSTL.h>

#include <boost_1_51_0.h>
// This is used in FC++ to allow for boost usage.
#define FCPP_ARDUINO_BOOST
#include <iostream>
#include <vector>
#define FCPP_DEBUG
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;
using std::vector;

/////////////////////////////////////////////////////////////////////
// version from ecoop1b
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
   void debug() {
     cout << "sprint ( lazy1(ptr_to_fun(&ConcreteObserverA::update), this ) ) : " << endl
          << sprint ( lazy1(ptr_to_fun(&ConcreteObserverA::update), this ) )
          << endl;
   }
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
////////////////////////////////////////////////////////////////////////

// This is used to keep the constructor happy.
int nullfun(int i) { std::cout << "nullfun" << std::endl; return 0; } 

fcpp::Fun1<int,int> nullfun11 = fcpp::ptr_to_fun(&nullfun);

template <class E>
class BareSubject {
  // At the moment this only stores one function to be called.
  // This needs to be made into a vector etc.
public:
  typedef typename  fcpp::Fun1<E,E> fun_type;
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
protected:
  typedef typename fcpp_container::iterator fcpp_iterator;
};

template <class Subject>
class ConcreteObserver {
  //  Subject& subject_;
public:
  typedef typename Subject::Event Event;
  Event event_;
  ConcreteObserver () { }  
  ConcreteObserver (Subject &s, Event e) : /*subject_(s),*/ event_(e) {
    s.Attach( fcpp::curry2( fcpp::ptr_to_fun( &ConcreteObserver::be_notified), this), e);
  }
  void AddSubject(Subject &s, Event e) {
    //std::cout << "AddSubject has event " << e << std::endl;
    s.Attach( fcpp::curry2( fcpp::ptr_to_fun( &ConcreteObserver::be_notified), this), e);
    //s.Flush();
  }
  int be_notified(Event e) {
    event_ = e;
    std::cout << "New event is " << event_ << std::endl;
    return event_;
  }
  ConcreteObserver &operator += (Subject &s) { AddSubject(s,event_); return this; }
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
   ao.debug();
  cout << "========================" << endl;
  cout << "New versions" << endl;
  cout << "========================" << endl;

 // This could be used to update e.g. all output streams with a common property
  // and also send individual information to different streams and stream types.
 
  typedef BareSubject<int> BareConcrete;

  // This FC++ observer has no knowledge of the other stuff.
  BareConcrete bareconcrete10,bareconcrete11;
  // The ConcreteObserver class is identical.
  ConcreteObserver<BareConcrete> bareobserver1(bareconcrete10,10);  
  ConcreteObserver<BareConcrete> bareobserver2(bareconcrete11,11);  
  ConcreteObserver<BareConcrete> bareobserver3(bareconcrete11,12); 
  bareobserver1.AddSubject(bareconcrete11,14);

  int e;
  bareconcrete11.ListAll();
  std::cout << "============================================" << std::endl;
  e = bareconcrete10.Notify(e);
  std::cout << "============================================" << std::endl;
  bareconcrete11.NotifyAll();

}

void loop() {
  // put your main code here, to run repeatedly:

}
