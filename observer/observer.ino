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

#include <iostream>
#include <vector>
#define FCPP_DEBUG
#include "prelude.h"

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

}

void loop() {
  // put your main code here, to run repeatedly:

}
