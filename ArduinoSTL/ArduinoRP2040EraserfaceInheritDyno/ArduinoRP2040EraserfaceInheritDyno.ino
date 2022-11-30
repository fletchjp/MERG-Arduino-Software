//////////////////////////////////////////////////////////////////////
// Arduino RP2040 Eraserface Inherit Dyno
// This is an implementation of the example eraserface_inherit_dyno.cpp
// https://github.com/badair/eraserface
//////////////////////////////////////////////////////////////////////

#include <exception>
#include <stdexcept>

// Dummies to sort out compilation
namespace boost {

  void throw_exception( std::exception & e ) { }
  void throw_exception(const std::exception & e ) { }
  void throw_exception( std::runtime_error& e) { }
  void throw_exception(const std::runtime_error& e) { }

}
#undef F
#include <memory>
#include <boost_callable_traits.hpp>
#include <type_traits>
#include <vector>
#include <boost_function.hpp>
#include <dyno.hpp>
#include <eraserface.hpp>

#include <Streaming.h>

using std::vector;

typedef boost::function0<void> F_type;

//////////////////////////////////////////////////////////////
// The following comment is from when I worked on this before.
// There is reference in the Dyno code of non const usage.
// In view of the problems with the macro example I am unsure
// what can be done here.
// The point of Dyno is to build a common interface which can be 
// reused on different types.
// Use with non const objects is tricky.
///////////////////////////////////////////////////////////////
// Define the interface so that notify_const can be called.
// This is now working.
// It seems it can only handle const functions.
DYNO_INTERFACE(HasNotify,
  (notify_const, void () const)
//  (notify,       void () )
);


class A {
   typedef vector< F_type > V;
   V observers;
public:
   void attach( F_type o ) {
      observers.push_back( o );
   }
   void notify() {
      Serial << "A::notify called" << endl;
      for( V::iterator i=observers.begin(); i!=observers.end(); ++i ) {
         (*i)();
      }
   }
  // Test for ERASERFACE_ADD_MEMBER
   int something() { return 2; }
   V get_observers() { return observers; }
};

typedef boost::function<void (A*)>  A_type;
class B;
typedef boost::function<void (B*)>  B_type;

#define ERASERFACE_ADD_MEMBER_VOID(name, base)  \
  void name() {  base::name() ; }

#define ERASERFACE_ADD_MEMBER(ret, name, base)  \
  ret name() {  return base::name() ; }

typedef vector< F_type > V_type;

class B : public A {
   int b_state;
public:
   B() : b_state(0) {}
   int get_state() const { return b_state; }
  //using std::static_cast<void (B::*)()>(void (A::notify)());
  // void notify() {
  //    Serial << "Calling A::notify();" << endl; A::notify();
  //  }
  A_type a_notify = &A::notify;
  B_type b_notify = &A::notify;  // Making a B object!
  void aa_notify() { a_notify(this); }
  void bb_notify() { b_notify(this); }
  ERASERFACE_ADD_MEMBER_VOID(notify,A)
  ERASERFACE_ADD_MEMBER(int,something,A)
  ERASERFACE_ADD_MEMBER(V_type,get_observers,A)
   void test() {
    //if (std::is_base_of<A, B>::value)
    if (std::is_base_of<A, B>() ) // C++14
      Serial << "A is a base class of B" << endl;
    //if (std::is_convertible<A, B>::value)
    bool b2a = std::is_convertible<B*, A*>::value;
    bool a2b = std::is_convertible<A*, B*>::value;
    if (b2a) Serial << "B* convertible to A*" << endl;
    if (!a2b) Serial << "A* is not convertible to B*" << endl;
    //static_cast<void (B::*)()>(void (A::notify)() );
  }
   void inc() {
     b_state++;
     Serial << "B notifies state as " << b_state << endl;
     //A::notify();
     b_notify(this);
   }
};

typedef B *B_ptr;

DEFINE_ERASERFACE(A_interface,
      ((attach,      void(F_type) ))
      ((notify,      void() ))
      ((something,   int()  ))
      ((get_observers, V_type() ))
);

DEFINE_ERASERFACE(B_interface,
      ((get_state,   int() const ))
                  ((test,        void() ))
      ((notify,      void() ))
      ((something,   int()  ))
      ((get_observers, V_type() ))
      ((bb_notify,   void() ))
      ((aa_notify,   void() ))
      ((inc   ,      void() ))
);

void setup() {
  Serial.begin(115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 10000));
  t2 = millis() - t1;
  Serial.print("Waited for ");
  Serial.print(t2);
  Serial.println(" millis");
  Serial << "Eraserface inherit example" << endl;
  A a0;
  B b0;
  A_interface a_i = a0;
  B_interface b_i = b0;
  b_i.test();
  b_i.notify(); Serial << "aa ";
  b_i.aa_notify(); Serial << "bb ";
  b_i.bb_notify();
  b_i.inc();
  Serial << "b_i.something() : " << b_i.something() << endl;
  V_type obs = b_i.get_observers();
  Serial << "obs.size() : " << obs.size() << endl;

}

void loop() {
  // put your main code here, to run repeatedly:

}