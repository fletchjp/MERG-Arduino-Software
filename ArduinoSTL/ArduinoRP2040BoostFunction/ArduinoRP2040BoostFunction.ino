//////////////////////////////////////////////////////////
// Arduino RP2040 Boost Function
//////////////////////////////////////////////////////////

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
#include <boost_utility_result_of.hpp>
#include <boost_function.hpp>
#include <boost_bind.hpp>
#include <string>
#include <vector>


// 3rd party libraries
#include <Streaming.h>

//////////////////////////////////////////////////////////
// This is for an adapted copy from BoostFC++ operator.hpp
//////////////////////////////////////////////////////////
namespace infix  {

template <class LHS, class Fun>
struct InfixOpThingy {
   // Note that storing const&s here relies on the fact that temporaries
   // are guaranteed to live for the duration of the full-expression in
   // which they are created.  There's no need to create copies.
   const LHS& lhs;
   const Fun& f;
   InfixOpThingy( const LHS& l, const Fun& ff ) : lhs(l), f(ff) {}
};


template <class LHS, class F>
inline InfixOpThingy<LHS,boost::function<F> >
operator^( const LHS& lhs, const boost::function<F>& f ) {
  return InfixOpThingy<LHS,boost::function<F> >(lhs,f);
}

template <class LHS, class A1, class A2, class R>
inline InfixOpThingy<LHS,boost::function2<R,A1,A2> >
operator^( const LHS& lhs, const boost::function2<R,A1,A2>& f ) {
  return InfixOpThingy<LHS,boost::function2<R,A1,A2> >(lhs,f);
}

template <class LHS, class FF, class RHS>
inline typename boost::result_of<FF(LHS,RHS)>::type
operator^( const InfixOpThingy<LHS,FF>& x, const RHS& rhs ) {
   return x.f( x.lhs, rhs );
}


}

//////////////////////////////////////////////////////////
// Test functions and pointers
//////////////////////////////////////////////////////////

int f0()
{
  return 0;
}

int f1(int x)
{
  return x + x;
}

int f2(int x, int y)
{
  return x + y;
}

int f3(int x, int y, int z)
{
  return x + y + z;
}

typedef int (*pointer_to_func0)();
typedef int (*pointer_to_func1)(int);
typedef int (*pointer_to_func2)(int,int);
typedef int (*pointer_to_func3)(int,int,int);

template <typename F,typename G>
bool contains(const F& f,const G &g)
{
  if (g.template target<F>())
  return (*g.template target<F>() == *f);
  else return false;
}

template <typename F,typename G>
bool check(const F& f,const G &g)
{
  return (g.template target<F>());
}

//////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 10000));
  t2 = millis() - t1;
  Serial << "Waited for " << t2 << " millis" << endl;
  delay(5000);
  Serial  << "Arduino RP2040 Boost Function Test" << endl;
  Serial.println("--------");

  boost::function0<int> g00(f0);
  boost::function<int()> g0(f0);
  boost::function1<int,int> g11(f1);
  boost::function<int(int x)> g1(f1);

  boost::function2<int,int,int> g22(f2);
  boost::function<int(int x,int y)> g2(f2);

  boost::function3<int,int,int,int> g33(f3);
  boost::function<int(int x,int y,int z)> g3(f3);

  typedef boost::function<int()> type0;
  typedef boost::function0<int>  type00;
  typedef boost::function1<int,int>  type11;
  typedef boost::function<int(int)>  type1;
  typedef boost::function2<int,int,int>  type22;
  typedef boost::function<int(int, int)>  type2;
  typedef boost::function3<int,int,int,int>  type33;
  typedef boost::function<int(int, int,int)>  type3;

  Serial << "f0()  = " << f0() << endl;
  Serial << "g00() = " << g00() << endl;
  Serial << "g0()  = " << g0() << endl;
  Serial.println("--------");

  pointer_to_func0 p0; // Instance of pointer to type.
  if (check(p0,g0)) {
    Serial << "g0 contains p0;  (*p0)()    = ";
    p0 = *g0.target<pointer_to_func0>();
    Serial << (*p0)() << endl;
  }
  if(*g0.target<pointer_to_func0>() == f0) {
    Serial << "g0 contains f0" << endl;
  } else {
    Serial << "g0 does not contain f0" << endl;
  }
  Serial.println("--------");
  int (*p11)(int) ;
  pointer_to_func1 p1; // Instance of pointer to type.
  p1 = &f1;
  p11 = &f1;
  Serial << "f1(1)  = " << f1(1) << endl;
  Serial << "(*p1)(2)  = " << (*p1)(2) << endl;
  Serial << "(*p11)(3)  = " << (*p11)(3) << endl;
  Serial << "g1(3)  = " << g1(3) << endl;
  Serial << "g11(4)  = " << g11(4) << endl;
  Serial.println("--------");
   if(*g11.target<pointer_to_func1>() == f1) {
    Serial << "g11 contains f1" << endl;
  } else {
    Serial << "g11 does not contain f1" << endl;
  }
  if(*g2.target<pointer_to_func2>() == f2) {
    Serial << "g2 contains f2" << endl;
  } else {
    Serial << "g2 does not contain f2" << endl;
  }

  if(g1.contains(&f1)) {
    Serial << "g1 does have a valid pointer for f1" << endl;
  }

  if(g11.contains(&f1)) {
    Serial << "g11 does have a valid pointer for f1" << endl;
    // For some reason the next line throws even though it has a valid value with AVR.
    g11.target<pointer_to_func1>();
  }

/* fails for g1  and g11 for AVR */
  if(g11.target<pointer_to_func1>()) {
    Serial << "g11 does have a valid pointer for f1" << endl;
  }
  if(!g1.target<pointer_to_func2>()) {
    Serial << "g1 does not have a valid pointer for f2" << endl;
  }

  if(!g2.target<pointer_to_func1>()) {
    Serial << "g2 does not have a valid pointer for f1" << endl;
  }
  Serial.println("--------");
  if (contains(&f2,g2)) {
    Serial << "g2 contains f2" << endl;
  } else {
    Serial << "g2 does not contain f2" << endl;
  }
  if (contains(&f2,g22)) {
    Serial << "g22 contains f2" << endl;
  } else {
    Serial << "g22 does not contain f2" << endl;
  }
  Serial.println("--------");
  pointer_to_func2 p2; // Instance of pointer to type.
  if (check(p2,g2)) {
    Serial << "g2 contains p2;  (*p2)(2,3) = ";
    p2 = *g2.target<pointer_to_func2>();
    Serial << (*p2)(2,3) << endl;
  }
  if (check(p2,g22)) {
    Serial << "g22 contains p2; (*p2)(3,4) = ";
    p2 = *g22.target<pointer_to_func2>();
    Serial << (*p2)(3,4) << endl;
  }

  
  Serial.println("--------");
  if (contains(&f3,g3)) {
    Serial << "g3 contains f3" << endl;
  } else {
    Serial << "g3 does not contain f3" << endl;
  }
  if (contains(&f3,g33)) {
    Serial << "g33 contains f3" << endl;
  } else {
    Serial << "g33 does not contain f3" << endl;
  }
  Serial.println("--------");
  pointer_to_func3 p3; // Instance of pointer to type.
  if (check(p3,g3)) {
    Serial << "g3 contains p3;  (*p3)(1,2,3) = ";
    p3 = *g3.target<pointer_to_func3>();
    Serial << (*p3)(1,2,3) << endl;
  }
  if (check(p3,g33)) {
    Serial << "g33 contains p3; (*p3)(2,3,4) = ";
    p3 = *g33.target<pointer_to_func3>();
    Serial << (*p3)(2,3,4) << endl;
  }

  Serial.println("--------");
  delay(5000);
  pinMode(LED_BUILTIN, OUTPUT);

}


//////////////////////////////////////////////////////////


void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("Arduino blink ON");
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("Arduino blink OFF");
  delay(1000);                       // wait for a second

}
