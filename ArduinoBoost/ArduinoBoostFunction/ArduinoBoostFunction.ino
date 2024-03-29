// ArduinoBoostFunction
// This example now works as long as the AVR Boards are 1.8.2 not 1.8.3
// The current version of the AVR boards is 1.8.5 so I think I will abandon these codes.
#include <ArduinoSTL.h>
// This is an old version of boost from 2012.
#include <boost_1_51_0.h>
// Example use of a Boost header
/*
Sketch uses 10536 bytes (32%) of program storage space. Maximum is 32256 bytes.
Global variables use 1510 bytes (73%) of dynamic memory, leaving 538 bytes for local variables. Maximum is 2048 bytes.
*/

//////////////////////////////////////////////////////////////////////
// boost::function
// I set out to work with an example of boost function.
// It has taken some time to get the header to compile.
// The problem is that boost::function throws exceptions
// which are not supported by the Arduino C++.
// The solution is to put some code here which will
// satsify the compiler.
//////////////////////////////////////////////////////////////////////
#define BOOST_DISABLE_ASSERTS
#define BOOST_NO_EXCEPTIONS
#define BOOST_EXCEPTION_DISABLE
#undef B1
#undef F  // This is needed for the DUE.
#include <iostream>
#include <exception>
#include <stdexcept>

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

#include <boost/utility/result_of.hpp>
#include <boost/function.hpp>
#include <string>

/*
  class SomeError : public std::runtime_error {
    public:
      SomeError() : std::runtime_error(std::string("Some error")) {}
  };
*/

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
  std::cout << "=========================================" << std::endl;
  boost::function0<int> g00(f0);
  boost::function<int()> g0(f0);
  boost::function1<int,int> g11(f1);
  boost::function<int(int x)> g1(f1);
  boost::function2<int,int,int> g22(f2);
  boost::function<int(int x,int y)> g2(f2);

  typedef boost::function<int()> type0;
  typedef boost::function0<int>  type00;

  if(*g0.target<pointer_to_func0>() == f0) {
    std::cout << "g0 contains f0" << std::endl;
  } else {
    std::cout << "g0 does not contain f0" << std::endl;
  }
  if(*g1.target<pointer_to_func1>() == f1) {
    std::cout << "g1 contains f1" << std::endl;
  } else {
    std::cout << "g1 does not contain f1" << std::endl;
  }
  if(*g2.target<pointer_to_func2>() == f2) {
    std::cout << "g2 contains f2" << std::endl;
  } else {
    std::cout << "g2 does not contain f2" << std::endl;
  }
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
  pointer_to_func0 p0; // Instance of pointer to type.
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
  pointer_to_func1 p1; // Instance of pointer to type.
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


  
  std::cout << "=========================================" << std::endl;
 
  
}

void loop() {

}
