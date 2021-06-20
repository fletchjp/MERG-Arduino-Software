// ArduinoBoost166FunctionList
// This example is beginning to use the lazy list operations possible in Boost Phoenix Function.
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
#define BOOST_PHOENIX_NO_LAZY_EXCEPTIONS

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

using namespace boost::phoenix;

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
  std::cout << "-----------------------" << std::endl;
  std::cout << "Boost phoenix lazy examples." << std::endl;
  std::cout << "-----------------------" << std::endl;
  using namespace infix;
  namespace phx = boost::phoenix;
  std::cout << "phx::plus(2,3)() = " << phx::plus(2,3)() << std::endl;
  int ph  =  2 ^phx::plus^ 3;
  int ph2 =  3 ^phx::minus^ 2;
  std::cout << "2 ^phx::plus^ 3  = " << ph  << std::endl;
  std::cout << "3 ^phx::minus^ 2 = " << ph2  << std::endl;

  using boost::phoenix::arg_names::arg1;
  using boost::phoenix::arg_names::arg2;

    std::cout << "============================" << std::endl;
    std::cout << "list<T> tests" << std::endl;
    std::cout << "============================" << std::endl;
    using phx::list;
    using phx::cons;
    list<int> l0;
    list<int> l1 = cons(1,l0);
    list<int> l2 = cons(2,l1);
    list<int> l3 = cons(3,l2);
    list<int> l4 = cons(4,l3);
    std::cout << "head(l1)()           = " << head(l1)() << std::endl;
    std::cout << "head(arg1)(l1)       = " << head(arg1)(l1) << std::endl;
    std::cout << "head(l2)()           = " << head(l2)() << std::endl;
    std::cout << "head(tail(l2))()     = " << head(tail(l2))() << std::endl;
    std::cout << "head(tail(arg1))(l2) = " << head(tail(arg1))(l2) << std::endl;
    std::cout << "-----------------------" << std::endl;

#else
  std::cout << "All examples out of use for AVR" << std::endl;
#endif
  std::cout << "=========================================" << std::endl;

}

void loop() {

}
