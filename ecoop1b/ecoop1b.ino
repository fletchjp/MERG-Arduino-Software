// ecoop1b 

// A first attempt to get FC++ working with ArduinoSTL and be able to use std::cout etc.

// At the moment it is missing boost/type_traits/is_integral.hpp

#include<ArduinoSTL.h>

#include <iostream>
#include <vector>
//#define FCPP_DEBUG
#include "fcpp/prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;
using std::vector;

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



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
