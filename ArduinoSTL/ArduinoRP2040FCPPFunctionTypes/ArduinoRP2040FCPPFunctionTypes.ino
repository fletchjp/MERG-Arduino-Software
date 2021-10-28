//////////////////////////////////////////////////////////////////////
// Arduino RP2040 FC++ Function Type
// Starting from old example member_function_types
// This used Boost Function Types and I am going to compare Boost Callable Traits.
//////////////////////////////////////////////////////////////////////
// was DUEFCPP
// Demo of FC++ Maybe operation
// This will work on ARDUINO DUE but not on AVR boards.

#undef F
#undef A
#include <boost_function_types_function_type.hpp>
#include <boost_function_types_parameter_types.hpp>
#include <boost_function_types_function_arity.hpp>
#include <boost_typeof_std_utility.hpp>
#include <typeinfo>
#include <string>
#include <boost_core_demangle.hpp>
//#include <boost_units/detail/utility.hpp>
// I cannot get to it the usual way as BoostForArduino does not go to the detail level with headers.
// This is what is in there
namespace boost {

namespace units {

namespace detail {

inline
std::string
demangle(const char* name)
{
    std::string demangled = core::demangle(name);

    const std::string::size_type prefix_len = sizeof("boost::units::") - 1;
    std::string::size_type i = 0;
    for (;;)
    {
        std::string::size_type pos = demangled.find("boost::units::", i, prefix_len);
        if (pos == std::string::npos)
            break;

        demangled.erase(pos, prefix_len);
        i = pos;
    }

    return demangled;
}

} // namespace detail

template<class L>
inline std::string simplify_typename(const L& /*source*/)
{
    return detail::demangle(typeid(L).name());
}

} // namespace units

} // namespace boost

using boost::units::detail::demangle;


#include "fcpp_prelude.h"

#include <Streaming.h>

// This came from https://software.intel.com/en-us/forums/topic/501879
// where it is a problem with intel compiler.
template<class T1 = int, class T2 = int>
class A {};

template<typename... TYPES>
void foo2(const A<TYPES...>& a) {
}

struct bar {
    int foo(int, char) { return 0; }
    template <class T>
    T foot(T const & t) { return t; }
    template <typename... XYZ>
    void fooxyz(const XYZ&... xyz) { }
};

using namespace fcpp;

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

void tests() {
    foo2(A<double>());

    typedef BOOST_TYPEOF(&bar::foo)
        foo_type;
    int arity = boost::function_types::function_arity<foo_type>::value;
    typedef boost::mpl::at_c<boost::function_types::parameter_types<foo_type>,1>::type
        arg1;
    typedef boost::mpl::at_c<boost::function_types::parameter_types<foo_type>,2>::type
        arg2;

    typedef BOOST_TYPEOF_TPL(&bar::foot<int>)
        foot_type;
    int arity_foot = boost::function_types::function_arity<foot_type>::value;

    // This only works for a single argument.
    typedef BOOST_TYPEOF_TPL(&bar::fooxyz<int>)
        fooxyz_type;
    int arity_fooxyz = boost::function_types::function_arity<fooxyz_type>::value;

    Serial << demangle(typeid(foo_type).name()).c_str() << endl;
    Serial << "arity = " << arity << endl;
    Serial << demangle(typeid(arg1).name()).c_str() << ",";
    Serial << demangle(typeid(arg2).name()).c_str() << endl;

    Serial << demangle(typeid(foot_type).name()).c_str() << endl;
    Serial << "arity = " << arity_foot << endl;
    Serial << demangle(typeid(fooxyz_type).name()).c_str() << endl;
    Serial << "arity = " << arity_fooxyz << endl;
    return;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 10000));
  t2 = millis() - t1;
  Serial.print("Waited for ");
  Serial.print(t2);
  Serial.println(" millis");
  while (!delay_without_delaying(5000) ) { };
  Serial << endl << endl << " Arduino RP2040 FC++ " << __FILE__ << endl;
  Serial << "Some simple FC++ operations" << endl;
  Serial << "plus(1,2) = " << plus(1,2) << endl;
  Serial << "plus(1.5,2.3) = " << plus(1.5,2.3) << endl;
  Serial << "plus(1)(2) = " << plus(1)(2) << endl;
  Serial << "inc(1) = " << inc(1) << endl;
  Serial << "--------------------------" << endl;

  Maybe<int> mx(2), my(3);
  Maybe<int> mz;
  if (mz.is_nothing()) Serial << "mz is nothing" << endl;
  mz = just(plus(mx.value(),my.value()));
  if (mz.is_nothing()) { 
     Serial << "mz is nothing" << endl;
  } else {
     Serial << "mz has " << mz.value() << endl;
  }
  Serial << "--------------------------" << endl;

  List<double>::iterator idx1, idx2;
  List<double> x1 = list_with(0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0);
  List<double> x2 = fcpp::map(fcpp::minus(1.0),x1);
  double sumx1 = foldl(plus,0.,x1);
  for (idx1 = x1.begin(), idx2 = x2.begin(); idx1 != x1.end(); ++idx1, ++idx2) {
     Serial << *idx1 << " " << *idx2 << endl;
  }
  Serial << "Sum of x1 = " << sumx1 << endl;
  Serial << "--------------------------" << endl;

  List<int> ix1 = enumFromTo(0,10);
  List<bool> bodds = fcpp::map(odd,ix1);
  List<bool> bevens = fcpp::map(even,ix1);
  List<int> odds = filter(odd,ix1);
  int sum_odds = foldl(plus,0,odds);
  int sum_odds2 = foldl(plus,0,filter(odd,ix1));
  List<int>::iterator iix1;
  List<bool>::iterator ib1, ib2;
  Serial << "Length of bodds is " << length(bodds) << endl;
  for (iix1 = ix1.begin(), ib1 = bodds.begin(), ib2 = bevens.begin(); iix1 != ix1.end(); ++iix1, ++ib1, ++ib2) {
     Serial << *iix1 << " " << *ib1 << " " << *ib2 << endl;
  }

  Serial << "Length of odds is " << length(odds) << endl;
  Serial << "sum of the odds is " << sum_odds << endl;
  tests();
  while (!delay_without_delaying(5000) ) { };

}

void loop() {
  // put your main code here, to run repeatedly:

}
