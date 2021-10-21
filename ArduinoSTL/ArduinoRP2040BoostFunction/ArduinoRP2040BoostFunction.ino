//////////////////////////////////////////////////////////
// Arduino RP2040 Boost Function
//////////////////////////////////////////////////////////

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

typedef int (*pointer_to_func0)();
typedef int (*pointer_to_func1)(int);
typedef int (*pointer_to_func2)(int,int);

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
  delay(2000);
  Serial  << "Arduino RP2040 Boost Function Test" << endl;

  boost::function0<int> g00(f0);
  boost::function<int()> g0(f0);
  // First order function definitions cause a crash.
  // This has now stopped because of other changes I have made!!
  boost::function1<int,int> g11(f1);
  boost::function<int(int x)> g1(f1);

  boost::function2<int,int,int> g22(f2);
  boost::function<int(int x,int y)> g2(f2);

  typedef boost::function<int()> type0;
  typedef boost::function0<int>  type00;
  typedef boost::function1<int,int>  type11;
  typedef boost::function<int(int)>  type1;

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
