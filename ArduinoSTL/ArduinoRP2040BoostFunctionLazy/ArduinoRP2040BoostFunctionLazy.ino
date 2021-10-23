//////////////////////////////////////////////////////////
// Arduino RP2040 Boost Function Lazy
// Adding Boost Phoenix lazy examples.
//////////////////////////////////////////////////////////

#include <exception>
#include <stdexcept>
#include <functional>
#include <type_traits>


// Dummies to sort out compilation
namespace boost {

  void throw_exception( std::exception & e ) { }
  void throw_exception(const std::exception & e ) { }
  void throw_exception( std::runtime_error& e) { }
  void throw_exception(const std::runtime_error& e) { }

}

#undef F
#define BOOST_PHOENIX_NO_LAZY_EXCEPTIONS

#include <boost_utility_result_of.hpp>
#include <boost_function.hpp>
#include <boost_bind.hpp>
#include <boost_phoenix_core.hpp>
#include <boost_phoenix_bind.hpp>
#include <boost_phoenix_operator_comparison.hpp>
#include <boost_phoenix_stl_algorithm_transformation.hpp>
#include <boost_phoenix_function.hpp>
// Headers for use of phoenix lazy operations.
#include <boost_phoenix_function_lazy_prelude.hpp>
#include <boost_phoenix_function_lazy_signature.hpp>
#include <string>
#include <vector>
//#include <boost_mp11_list.hpp>


// 3rd party libraries
#include <Streaming.h>

//////////////////////////////////////////////////////////
// std::decay which I have somehow missed from C++11.
// also decay_t = typename decay<T>::type; since C++14
// https://en.cppreference.com/w/cpp/types/decay
//////////////////////////////////////////////////////////

template <typename T, typename U>
struct decay_equiv : 
    std::is_same<typename std::decay<T>::type, U>::type 
{};

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

  /////////////////////////////////////////////////////////////
  // Phoenix version
  // Note that there has to be a distinct version
  // of InfixOpThingy to chose the correct second operator.
  // Also that the second operator returns the final value.
  // The return type has been used from lazy_signature.hpp
  ////////////////////////////////////////////////////////////
template <class LHS, class Fun>
struct InfixOpThingyPhoenix {
   // Note that storing const&s here relies on the fact that temporaries
   // are guaranteed to live for the duration of the full-expression in
   // which they are created.  There's no need to create copies.
   const LHS& lhs;
   const Fun& f;
   InfixOpThingyPhoenix( const LHS& l, const Fun& ff ) : lhs(l), f(ff) {}
  // This is an experiment to enable  1 ^f() for a 1 argument function.
  // It messes things for the two argument case.
  //typename boost::result_of<Fun(LHS)>::type
  // operator()() const { return f(lhs); }
};

template <class LHS, class F>
inline InfixOpThingyPhoenix<LHS,boost::phoenix::function<F> >
operator^( const LHS& lhs, const boost::phoenix::function<F>& f ) {
  return InfixOpThingyPhoenix<LHS,boost::phoenix::function<F> >(lhs,f);
}

// Experiment using std::decay which I had not heard of before.
namespace impl {
     //   RTFFXY   == ReturnTypeFunctoidFwithXandY   (used in thunk2)
      template <class F,class X,class Y>
      struct RTFFXY {
          typedef typename std::decay<F>::type FType;
          typedef typename std::decay<X>::type XType;
          typedef typename std::decay<Y>::type YType;
          typedef typename boost::result_of<FType(XType,YType)>::type FR;
          typedef typename boost::result_of<FR()>::type RR;
          typedef typename boost::phoenix::impl::remove_RC<RR>::type RType;
          typedef RType type;
      };
}

template <class LHS, class FF, class RHS>
// This is part of phoenix function lazy prelude.
// The headers for this are supplied above.
// One of the things supplied is easy ways to get the return types.
//inline typename boost::phoenix::impl::RTFFXY<FF,LHS,RHS>::type
inline typename impl::RTFFXY<FF,LHS,RHS>::type // local experiment
operator^( const InfixOpThingyPhoenix<LHS,FF>& x, const RHS& rhs ) {
  return x.f( x.lhs, rhs )();
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

// This comes from the cdc_multi example
// Helper: non-blocking "delay" alternative.
boolean delay_without_delaying(unsigned long time) {
  // return false if we're still "delaying", true if time ms has passed.
  // this should look a lot like "blink without delay"
  static unsigned long previousmillis = 0;
  unsigned long currentmillis = millis();
  if (currentmillis - previousmillis >= time) {
    previousmillis = currentmillis;
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 10000));
  t2 = millis() - t1;
  Serial << "Waited for " << t2 << " millis" << endl;
  while (!delay_without_delaying(5000) ) { };
  //delay(5000);
  Serial  << "Arduino RP2040 Boost Lazy Function Test" << endl;
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
  using namespace infix;
  Serial << "Infix for boost::function2" << endl;
  int z = 2 ^g2^ 3;
  int z2 = 3 ^g22^ 4;
  int z3 = 3 ^g22^ 4 ^g22^ 5;
  Serial << "2 ^g2^  3 = " << z << endl;
  Serial << "3 ^g22^ 4 = " << z2 << endl;
  Serial << "3 ^g22^ 4 ^g22^ 5 = " << z3 << endl;
  Serial.println("--------");
  //using namespace std::placeholders;
  auto f3a = boost::bind(f3,1,_1,_2);
  // Bound function has to be wrapped as a boost function.
  boost::function<int(int x,int y)> g3a(f3a);
  Serial << "auto f3a = boost::bind(f3,1,_1,_2)" << endl;
  Serial << "boost::function<int(int x,int y)> g3a(f3a);" << endl;
  int z4 = f3a(2,3);
  Serial << "f3a(2,3) = " << z4 << endl;
  int z5 = 2 ^g3a^ 3;
  Serial << "g3a(2,3) = " << z5 << endl;
  Serial << "-----------------------" << endl;
  Serial << "Boost phoenix lazy examples." << endl;
  Serial << "-----------------------" << endl;
  namespace phx = boost::phoenix;
  using boost::phoenix::arg_names::arg1;
  using boost::phoenix::arg_names::arg2;
  using boost::phoenix::local_names::_a;
  using boost::phoenix::local_names::_b;
  Serial << "phx::plus(2,3)() = " << phx::plus(2,3)() << endl;
  int ph  =  2 ^phx::plus^ 3;
  int ph2 =  3 ^phx::minus^ 2;
  Serial << "2 ^phx::plus^ 3  = " << ph  << endl;
  Serial << "3 ^phx::minus^ 2 = " << ph2  << endl;
  Serial << "-----------------------" << endl;
  using namespace boost::phoenix;
  int a = 123;
  int b = 256;
  Serial << "plus(arg1, arg2)(a, b) = "
            << phx::plus(arg1, arg2)(a, b) << endl;
  Serial << "plus(arg1, b)(a)       = "
            << plus(arg1, b)(a) << endl;
  Serial << "plus(a, arg2)(a,b)     = "
            << plus(a, arg2)(a,b) << endl;
  Serial << "plus(a, arg1)(b)       = "
            << plus(a, arg1)(b) << endl;
  Serial << "minus(a, b)()          = "
            << minus(a, b)() << endl;
  Serial << "plus(minus(a, b),b)()             = "
            << plus(minus(a, b),b)() << endl;
  Serial << "plus(minus(arg1, b),b)(a)         = "
            << plus(minus(arg1, b),b)(a) << endl;
  Serial << "plus(minus(arg1, arg2),b)(a,b)    = "
            << plus(minus(arg1, arg2),b)(a,b) << endl;
  Serial << "plus(minus(arg1, arg2),arg2)(a,b) = "
            << plus(minus(arg1, arg2),arg2)(a,b) << endl;

    Serial << "===============================" << endl;
    Serial << "Other numerical operators" << endl;
    Serial << "===============================" << endl;
    Serial << "multiplies(arg1,arg2)(3,6) = "
            << multiplies(arg1,arg2)(3,6) << endl;
    Serial << "divides(arg2,arg1)(3,6) = "
            << divides(arg2,arg1)(3,6) << endl;
    Serial << "modulus(arg2,arg1)(4,6) = "
            << modulus(arg2,arg1)(4,6) << endl;
// This clashes with min in Serial.
//  int res = phx::min(arg1,arg2)(4,6);
//  Serial << "phx::min(arg1,arg2)(4,6) = "
//         << phx::min(arg1,arg2)(4,6) << endl;
    Serial << "phx::max(arg1,arg2)(4,6) = "
              << phx::max(arg1,arg2)(4,6) << endl;
    Serial << "inc(arg1)(a) = "
              << inc(arg1)(a) << endl;
    Serial << "dec(arg1)(a) = "
              << dec(arg1)(a) << endl;
    Serial << "negate(arg1)(a) = "
              << negate(arg1)(a) << endl;
    Serial << "===============================" << endl;
    Serial << "Odds and ends" << endl;
    Serial << "===============================" << endl;
    Serial << "id(arg1)(a) = "
              << id(arg1)(a) << endl;
    Serial << "=====================================" << endl;
    Serial << "operations with operators" << endl;
    Serial << "arg1(x) returns the first argument" << endl;
    Serial << "arg2(x,y) returns the second argument" << endl;
    Serial << "=====================================" << endl;
    Serial << "(arg1 + arg2)(a,b)            = "
              << (arg1 + arg2)(a, b) << endl;
    Serial << "(arg1 - arg2)(a,b)            = "
              << (arg1 - arg2)(a, b) << endl;
    Serial << "(plus(arg1,arg2) - arg2)(a,b) = "
              << (plus(arg1,arg2) - arg2)(a, b) << endl;
    // This is not a good idea
    //Serial << "arg1(plus)(a,b) = "
    //          << arg1(plus)(a,b)<< endl;
    //Serial << "(arg1(a,arg2) - arg2)(plus,b) = "
    //          << (arg1(a,arg2) - arg2)(plus,b) << endl;
    // This does NOT work.
    //Serial << "(arg1 ^plus^ arg2)(a, b) = " << (arg1 ^plus^ arg2)(a, b) << endl; 
    Serial << "=========================" << endl;
    Serial << "let and lambda versions  " << endl;
    Serial << "=========================" << endl;
    // These require the header phoenix/scope.hpp
    Serial << "let(_a = arg1,_b = arg2)[_a + _b](a,b)       = "
              << let(_a = arg1,_b = arg2)[_a + _b](a,b) << endl;
    Serial << "lambda[arg1 + arg2]()(a,b)                   = "
              << lambda[arg1 + arg2]()(a,b) << endl;
    // This one does not compile.
    //Serial << "lambda()[arg1 + arg2]()(a,b)                 = "
    //          << lambda()[arg1 + arg2]()(a,b) << endl;
    Serial << "lambda(_a = arg1)[_a + arg2](a)(a,b)         = "
              << lambda(_a = arg1)[_a + arg2](a)(a,b) << endl;
    Serial << "lambda(_a = arg1)[_a + arg1](a)(b)           = "
              << lambda(_a = arg1)[_a + arg1](a)(b) << endl;
    Serial << "lambda(_a = arg1,_b = arg2)[_a + _b](a,b)()  = "
              << lambda(_a = arg1,_b = arg2)[_a + _b](a,b)() << endl;
    Serial << "-----------------------" << endl;
    Serial << "Boost phoenix lazy examples." << endl;
    Serial << "-----------------------" << endl;
    Serial << "============================" << endl;
    Serial << "list<T> tests" << endl;
    Serial << "============================" << endl;
    using phx::list;
    using phx::cons;
    list<int> l0;
    list<int> l1 = cons(1,l0);
    list<int> l2 = cons(2,l1);
    list<int> l3 = cons(3,l2);
    list<int> l4 = cons(4,l3);
    Serial << "head(l1)()           = " << head(l1)() << endl;
    Serial << "head(arg1)(l1)       = " << head(arg1)(l1) << endl;
    Serial << "head(l2)()           = " << head(l2)() << endl;
    Serial << "head(tail(l2))()     = " << head(tail(l2))() << endl;
    Serial << "head(tail(arg1))(l2) = " << head(tail(arg1))(l2) << endl;
    Serial << "-----------------------" << endl;
    typedef phx::list_iterator<int> list_iterator_int;
    list_iterator_int li;
    Serial << "l1 = ( ";
    for (li = l1.begin(); li != l1.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    Serial << "l2 = ( ";
    for (li = l2.begin(); li != l2.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    Serial << "l3 = ( ";
    for (li = l3.begin(); li != l3.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    Serial << "l4 = ( ";
    for (li = l4.begin(); li != l4.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    list<int> l5 = cons(5,NIL);
    Serial << "l5 = ( ";
    for (li = l5.begin(); li != l5.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    Serial << "list<int> l6 = take(2,l4)();" << endl;
    list<int> l6 = take(2,l4)();
    Serial << "l6 = ( ";
    for (li = l6.begin(); li != l6.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    list<int> l7 = cons(7,take(2,l4));
    Serial << "list<int> l7 = cons(7,take(2,l4));" << endl;
    Serial << "l7 = ( ";
    for (li = l7.begin(); li != l7.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    // I am surprised that this one works.
    list<int> l8 = take(1,take(3,l4))();
    Serial << "list<int> l8 = take(1,take(3,l4))();" << endl;
    Serial << "l8 = ( ";
    for (li = l8.begin(); li != l8.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    if (l8 < l7) Serial << "(l8 < l7) is true" << endl;
    list<int> l9 = list_with<>()(1,2,3,4,5);
    Serial << "list<int> l9 = list_with<>()(1,2,3,4,5);" << endl;
    Serial << "l9 = ( ";
    for (li = l9.begin(); li != l9.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    list<int> l10 = cat(l8,NIL);
    Serial << "list<int> l10 = cat(l8,NIL);" << endl;
    Serial << "l10 = ( ";
    for (li = l10.begin(); li != l10.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    list<int> l11 = cat(l0,l9);
    Serial << "list<int> l11 = cat(l0,l9);" << endl;
    Serial << "l11 = ( ";
    for (li = l11.begin(); li != l11.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    list<int> l12 = cat(l10,l9);
    Serial << "list<int> l12 = cat(l10,l9);" << endl;
    Serial << "l12 = ( ";
    for (li = l12.begin(); li != l12.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    list<int> l13 = cat(l8,take(2,l4));
    Serial << "list<int> l13 = cat(l8,take(2,l4));" << endl;
    Serial << "l13 = ( ";
    for (li = l13.begin(); li != l13.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    list<int> l14 = all_but_last(l13)();
    Serial << "list<int> l14 = all_but_last(l13)();" << endl;
    Serial << "l14 = ( ";
    for (li = l14.begin(); li != l14.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    list<int> l15 = drop(2,l12)();
    Serial << "list<int> l15 = drop(2,l12)();" << endl;
    Serial << "l15 = ( ";
    for (li = l15.begin(); li != l15.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;
    list<int> l16 = cons(arg1,l0)(16);
    Serial << "l16 = ( ";
    for (li = l16.begin(); li != l16.end(); li++)
      Serial << *li << " ";
    Serial << ")" << endl;

  Serial.println("--------");

  //delay(5000);
  while (!delay_without_delaying(5000) ) { };
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
