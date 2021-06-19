// ArduinoBoost166FunctionLazy
// This example is beginning to use the lazy operations possible in Boost Phoenix Function.
// Arduino DUE only.
#define BOOST_FOR_ARDUINO

// I have put this into limits.h to get rid of a problem with functional/hash in 1.66.0
// There are other modifications to limits.h as the hash code is fussy and
// I have not found a way to turn it off.
//#ifdef BOOST_FOR_ARDUINO
//  static constexpr bool is_iec559 = false;
//#else
//  static const bool is_iec559;
//#endif

#include <ArduinoSTL.h>
#include <iostream>
#include <exception>
#include <stdexcept>
//#include <typeinfo>
//#include <array>
#include <functional>
// All these defines are needed to sort things out.
#define BOOST_NO_CWCHAR
#define BOOST_NO_CXX11_CHAR16_T
#define BOOST_NO_CXX11_CHAR32_T
#define BOOST_NO_CXX11_HDR_TYPEINDEX
#define BOOST_NO_CXX11_HDR_ARRAY
#define BOOST_NO_CXX11_HDR_TUPLE
#define BOOST_NO_CXX11_SMART_PTR
#define BOOST_NO_CXX11_NULLPTR
#define BOOST_NO_STD_WSTRING
#define BOOST_DISABLE_ASSERTS
#define BOOST_NO_EXCEPTIONS
#define BOOST_EXCEPTION_DISABLE
#define BOOST_NO_CXX11_RVALUE_REFERENCES // Needed for boost::bind to work
#define BOOST_NO_CXX11_HDR_FUNCTIONAL    // Needed for boost::phoenix

namespace std {

   class runtime_error {
   public:
   runtime_error( std::exception & e ) { }
   runtime_error( std::string const & s ) { }
  };
}

namespace boost {

  void throw_exception( std::exception & e ) { }
  void throw_exception(const std::exception & e ) { }
  void throw_exception( std::runtime_error& e) { }
  void throw_exception(const std::runtime_error& e) { }

}

#undef A1
#undef A2
#undef A3
#undef A4
#undef A5
#undef A6
#undef A7
#undef A8
#undef A9
#undef B1
#undef F
#undef TC1
#undef TC2


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

//using namespace std;

// This is for an adapted copy from BoostFC++ operator.hpp
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

template <class LHS, class FF, class RHS>
// This is part of phoenix function lazy prelude.
// The headers for this are supplied above.
// One of the things supplied is easy ways to get the return types.
inline typename boost::phoenix::impl::RTFFXY<FF,LHS,RHS>::type
operator^( const InfixOpThingyPhoenix<LHS,FF>& x, const RHS& rhs ) {
  return x.f( x.lhs, rhs )();
}


}

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

void setup() {
  Serial.begin(9600);
  std::cout << std::endl;
  std::cout << "=========================================" << std::endl;
  std::cout << "Boost function example running on Arduino" << std::endl;
  std::cout << "using Boost for Arduino (1.66.0)" << std::endl;
  #ifdef ARDUINO_SAM_DUE
  std::cout << "running on an Arduino DUE" << std::endl;
  #else
  std::cout << "running on MEGA (AVR) which is liable to crash" << std::endl;
  #endif
  std::cout << "=========================================" << std::endl;

  #ifdef ARDUINO_SAM_DUE
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

  std::cout << "f0()  = " << f0() << std::endl;
  std::cout << "g00() = " << g00() << std::endl;
  std::cout << "g0()  = " << g0() << std::endl;
  pointer_to_func0 p0; // Instance of pointer to type.
  if (check(p0,g0)) {
    std::cout << "g0 contains p0;  (*p0)()    = ";
    p0 = *g0.target<pointer_to_func0>();
    std::cout << (*p0)() << std::endl;
  }
  //p0 = *g0.target<pointer_to_func0>();
  //std::cout << "(*p0)() = " << (*p0)() << std::endl;

  if(*g0.target<pointer_to_func0>() == f0) {
    std::cout << "g0 contains f0" << std::endl;
  } else {
    std::cout << "g0 does not contain f0" << std::endl;
  }
  int (*p11)(int) ;
  pointer_to_func1 p1; // Instance of pointer to type.
  p1 = &f1;
  p11 = &f1;
  std::cout << "f1(1)  = " << f1(1) << std::endl;
  std::cout << "(*p1)(2)  = " << (*p1)(2) << std::endl;
  std::cout << "(*p11)(3)  = " << (*p11)(3) << std::endl;
  std::cout << "g1(3)  = " << g1(3) << std::endl;
  std::cout << "g11(4)  = " << g11(4) << std::endl;
 
 
   if(*g11.target<pointer_to_func1>() == f1) {
    std::cout << "g11 contains f1" << std::endl;
  } else {
    std::cout << "g11 does not contain f1" << std::endl;
  }
  if(*g2.target<pointer_to_func2>() == f2) {
    std::cout << "g2 contains f2" << std::endl;
  } else {
    std::cout << "g2 does not contain f2" << std::endl;
  }

  if(g1.contains(&f1)) {
    std::cout << "g1 does have a valid pointer for f1" << std::endl;
  }

  if(g11.contains(&f1)) {
    std::cout << "g11 does have a valid pointer for f1" << std::endl;
    // For some reason the next line throws even though it has a valid value with AVR.
    g11.target<pointer_to_func1>();
  }

/* fails for g1  and g11 for AVR */
  if(g11.target<pointer_to_func1>()) {
    std::cout << "g11 does have a valid pointer for f1" << std::endl;
  }
  if(!g1.target<pointer_to_func2>()) {
    std::cout << "g1 does not have a valid pointer for f2" << std::endl;
  }

  if(!g2.target<pointer_to_func1>()) {
    std::cout << "g2 does not have a valid pointer for f1" << std::endl;
  }
 /*  This causes a crash even when the outer if returns false. (AVR) */
  if(g2.target<pointer_to_func1>()) {
  if(*g2.target<pointer_to_func1>() == f1) {
    std::cout << "g2 contains f1" << std::endl;
  } else {
    std::cout << "g2 does not contain f1" << std::endl;
  }} else {
    std::cout << "g2 does not have a valid pointer for f1" << std::endl;
  }
  if (contains(&f0,g0)) {
    std::cout << "g0 contains f0" << std::endl;
  } else {
    std::cout << "g0 does not contain f0" << std::endl;
  }
  if (contains(&f0,g00)) {
    std::cout << "g00 contains f0" << std::endl;
  } else {
    std::cout << "g00 does not contain f0" << std::endl;
  }

  if (contains(&f1,g0)) {
    std::cout << "g0 contains f1" << std::endl;
  } else {
    std::cout << "g0 does not contain f1" << std::endl;
  }


  if (contains(&f1,g1)) {
    std::cout << "g1 contains f1" << std::endl;
  } else {
    std::cout << "g1 does not contain f1" << std::endl;
  }

  if (contains(&f1,g11)) {
    std::cout << "g11 contains f1" << std::endl;
  } else {
    std::cout << "g11 does not contain f1" << std::endl;
  }


  if (contains(&f2,g2)) {
    std::cout << "g2 contains f2" << std::endl;
  } else {
    std::cout << "g2 does not contain f2" << std::endl;
  }
  if (contains(&f2,g22)) {
    std::cout << "g22 contains f2" << std::endl;
  } else {
    std::cout << "g22 does not contain f2" << std::endl;
  }
  // These examples recover the pointer if we know the type.
  //pointer_to_func0 p0; // Instance of pointer to type.
  if (check(p0,g0)) {
    std::cout << "g0 contains p0;  (*p0)()    = ";
    p0 = *g0.target<pointer_to_func0>();
    std::cout << (*p0)() << std::endl;
  }
  if (check(p0,g00)) {
    std::cout << "g00 contains p0; (*p0)()    = ";
    p0 = *g00.target<pointer_to_func0>();
    std::cout << (*p0)() << std::endl;
  }

  //pointer_to_func1 p1; // Instance of pointer to type.
 if (check(p1,g1)) {
    std::cout << "g1 contains p1;  (*p1)(2)   = ";
    p1 = *g1.target<pointer_to_func1>();
    std::cout << (*p1)(2) << std::endl;
  }

  if (check(p1,g11)) {
    std::cout << "g11 contains p1; (*p1)(3)   = ";
    p1 = *g11.target<pointer_to_func1>();
    std::cout << (*p1)(3) << std::endl;
  }


  pointer_to_func2 p2; // Instance of pointer to type.
  if (check(p2,g2)) {
    std::cout << "g2 contains p2;  (*p2)(2,3) = ";
    p2 = *g2.target<pointer_to_func2>();
    std::cout << (*p2)(2,3) << std::endl;
  }
  if (check(p2,g22)) {
    std::cout << "g22 contains p2; (*p2)(3,4) = ";
    p2 = *g22.target<pointer_to_func2>();
    std::cout << (*p2)(3,4) << std::endl;
  }
  std::cout << "-----------------------" << std::endl;
  using namespace infix;
  std::cout << "Infix for boost::function2" << std::endl;
  std::cout << "-----------------------" << std::endl;
  int z = 2 ^g2^ 3;
  int z2 = 3 ^g22^ 4;
  int z3 = 3 ^g22^ 4 ^g22^ 5;
  std::cout << "2 ^g2^  3 = " << z << std::endl;
  std::cout << "3 ^g22^ 4 = " << z2 << std::endl;
  std::cout << "3 ^g22^ 4 ^g22^ 5 = " << z3 << std::endl;
  std::cout << "-----------------------" << std::endl;
  std::cout << "Boost phoenix lazy examples." << std::endl;
  std::cout << "-----------------------" << std::endl;
  namespace phx = boost::phoenix;
  using boost::phoenix::arg_names::arg1;
  using boost::phoenix::arg_names::arg2;
  using boost::phoenix::local_names::_a;
  using boost::phoenix::local_names::_b;
  std::cout << "phx::plus(2,3)() = " << phx::plus(2,3)() << std::endl;
  int ph  =  2 ^phx::plus^ 3;
  int ph2 =  3 ^phx::minus^ 2;
  std::cout << "2 ^phx::plus^ 3  = " << ph  << std::endl;
  std::cout << "3 ^phx::minus^ 2 = " << ph2  << std::endl;
  std::cout << "-----------------------" << std::endl;
  using namespace boost::phoenix;
  int a = 123;
  int b = 256;
  std::cout << "plus(arg1, arg2)(a, b) = "
            << phx::plus(arg1, arg2)(a, b) << std::endl;
  std::cout << "plus(arg1, b)(a)       = "
            << plus(arg1, b)(a) << std::endl;
  std::cout << "plus(a, arg2)(a,b)     = "
            << plus(a, arg2)(a,b) << std::endl;
  std::cout << "plus(a, arg1)(b)       = "
            << plus(a, arg1)(b) << std::endl;
  std::cout << "minus(a, b)()          = "
            << minus(a, b)() << std::endl;
  std::cout << "plus(minus(a, b),b)()             = "
            << plus(minus(a, b),b)() << std::endl;
  std::cout << "plus(minus(arg1, b),b)(a)         = "
            << plus(minus(arg1, b),b)(a) << std::endl;
  std::cout << "plus(minus(arg1, arg2),b)(a,b)    = "
            << plus(minus(arg1, arg2),b)(a,b) << std::endl;
  std::cout << "plus(minus(arg1, arg2),arg2)(a,b) = "
            << plus(minus(arg1, arg2),arg2)(a,b) << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "Other numerical operators" << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "multiplies(arg1,arg2)(3,6) = "
              << multiplies(arg1,arg2)(3,6) << std::endl;
    std::cout << "divides(arg2,arg1)(3,6) = "
              << divides(arg2,arg1)(3,6) << std::endl;
    std::cout << "modulus(arg2,arg1)(4,6) = "
              << modulus(arg2,arg1)(4,6) << std::endl;
    std::cout << "min(arg1,arg2)(4,6) = "
              << min(arg1,arg2)(4,6) << std::endl;
    std::cout << "max(arg1,arg2)(4,6) = "
              << max(arg1,arg2)(4,6) << std::endl;
    std::cout << "inc(arg1)(a) = "
              << inc(arg1)(a) << std::endl;
    std::cout << "dec(arg1)(a) = "
              << dec(arg1)(a) << std::endl;
    std::cout << "negate(arg1)(a) = "
              << negate(arg1)(a) << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "Logical operators" << std::endl;
    std::cout << "===============================" << std::endl;
    if (equal(arg1,arg1)(a)) std::cout << "equal(arg1,arg1)(a) is true "
              << std::endl;
    if (not_equal(arg1,arg2)(a,b))
       std::cout << "not_equal(arg1,arg2)(a,b) is true "
              << std::endl;
    if (greater(arg2,arg1)(a,b))
       std::cout << "greater(arg2,arg1)(a,b) is true "
              << std::endl;
    if (less(arg1,arg2)(a,b)) std::cout << "less(arg1,arg2)(a,b) is true "
              << std::endl;
    if (greater_equal(arg2,arg1)(a,b))
       std::cout << "greater_equal(arg2,arg1)(a,b) is true "
              << std::endl;
    if (less_equal(arg1,arg2)(a,b))
       std::cout << "less_equal(arg1,arg2)(a,b) is true "
              << std::endl;
    if (less_equal(arg1,arg1)(a))
       std::cout << "less_equal(arg1,arg1)(a) is true "
              << std::endl;
    if (logical_and(less(arg1,arg2),greater(arg2,arg1))(a,b))
       std::cout << "logical_and(less(arg1,arg2),greater(arg2,arg1))(a,b) is true "
              << std::endl;
    if (logical_or(less(arg1,arg2),less(arg2,arg1))(a,b))
       std::cout << "logical_or(less(arg1,arg2),less(arg2,arg1))(a,b) is true "
              << std::endl;
    if (logical_not(greater(arg1,arg2))(a,b))
       std::cout << "logical_not(greater(arg1,arg2))(a,b) is true "
              << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "Odds and ends" << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "id(arg1)(a) = "
              << id(arg1)(a) << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "operations with operators" << std::endl;
    std::cout << "=========================" << std::endl;
    // This requires the header phoenix/operator.hpp
    std::cout << "(arg1 + arg2)(a,b)            = "
              << (arg1 + arg2)(a, b) << std::endl;
    std::cout << "(arg1 - arg2)(a,b)            = "
              << (arg1 - arg2)(a, b) << std::endl;
    std::cout << "(plus(arg1,arg2) - arg2)(a,b) = "
              << (plus(arg1,arg2) - arg2)(a, b) << std::endl;
    // This does NOT work.
    //std::cout << "(arg1 ^plus^ arg2)(a, b) = " << (arg1 ^plus^ arg2)(a, b) << std::endl; 
    std::cout << "=========================" << std::endl;
    std::cout << "let and lambda versions  " << std::endl;
    std::cout << "=========================" << std::endl;
    // These require the header phoenix/scope.hpp
    std::cout << "let(_a = arg1,_b = arg2)[_a + _b](a,b)       = "
              << let(_a = arg1,_b = arg2)[_a + _b](a,b) << std::endl;
    std::cout << "lambda[arg1 + arg2]()(a,b)                   = "
              << lambda[arg1 + arg2]()(a,b) << std::endl;
    // This one does not compile.
    //std::cout << "lambda()[arg1 + arg2]()(a,b)                 = "
    //          << lambda()[arg1 + arg2]()(a,b) << std::endl;
    std::cout << "lambda(_a = arg1)[_a + arg2](a)(a,b)         = "
              << lambda(_a = arg1)[_a + arg2](a)(a,b) << std::endl;
    std::cout << "lambda(_a = arg1)[_a + arg1](a)(b)           = "
              << lambda(_a = arg1)[_a + arg1](a)(b) << std::endl;
    std::cout << "lambda(_a = arg1,_b = arg2)[_a + _b](a,b)()  = "
              << lambda(_a = arg1,_b = arg2)[_a + _b](a,b)() << std::endl;


#else
  std::cout << "All examples out of use for AVR" << std::endl;
#endif
  std::cout << "=========================================" << std::endl;

}

void loop() {

}
