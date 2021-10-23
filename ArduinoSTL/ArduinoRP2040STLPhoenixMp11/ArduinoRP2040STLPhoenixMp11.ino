//////////////////////////////////////////////////////////
// Arduino RP2040 Boost Phoenix tests plus mp11
//////////////////////////////////////////////////////////

// This is a comparison of several ways of deleting an entry from a list of strings.

// This fails with the following link error.

// c:/users/user/appdata/local/arduino15/packages/rp2040/tools/pqt-gcc/1.3.1-a-7855b0c/bin/../lib/gcc/arm-none-eabi/10.3.0/../../../../arm-none-eabi/bin/ld.exe:
// c:/users/user/appdata/local/arduino15/packages/rp2040/tools/pqt-gcc/1.3.1-a-7855b0c/bin/../lib/gcc/arm-none-eabi/10.3.0/../../../../arm-none-eabi/lib/thumb\libstdc++.a(locale.o):
// in function `get_locale_cache_mutex': /workdir/repo/gcc-gnu/libstdc++-v3/src/c++98/locale.cc:36: undefined reference to `__sync_synchronize'

// unless boost/phoenix/core/debug.hpp is not compiled in boost/phoenix/core.hpp. It includes <iostream> which may well be the issue.

#undef F
#define BOOST_PHOENIX_NO_LAZY_EXCEPTIONS
// Only needed for developments beyond Boost 1.66.0 which are not in any released Boost
#define BOOST_PHOENIX_FUNCTION_LAZY_USE_MP11

#include <boost_utility_result_of.hpp>
#include <boost_function.hpp>
#include <boost_bind.hpp>
//#include <boost_locale.hpp> Not the answer
#include <boost_phoenix_core.hpp>
// Adding the contents of boost_phoenix_core.hpp one by one.
//#include <boost_phoenix_version.hpp>
//#include <boost_phoenix_core_limits.hpp>
//#include <boost_phoenix_core_actor.hpp>
//#include <boost_phoenix_core_debug.hpp> Aha this is it. Can I manage without it? It seems so.
//#include <boost_phoenix_core_is_actor.hpp>
#include <boost_phoenix_bind.hpp>
#include <boost_phoenix_operator_comparison.hpp>
#include <boost_phoenix_stl_algorithm_transformation.hpp>
#include <boost_mp11.hpp>
#include <string>
#include <vector>
#include <functional>
#include <type_traits>
#include <typeinfo>


// 3rd party libraries
#include <Streaming.h>

///////////////////////////////////////////////
// mp11 definitions from boost_mp11_explore.cpp
///////////////////////////////////////////////

using namespace boost::mp11;


// Definitions from https://rawgit.com/pdimov/mp11/master/doc/html/mp11.html

// Turning the example into an actual list.
template <class X> using  listX = mp_list< mp_list<char[], void>, mp_list<>,
                                  std::tuple<int, float, char>, std::pair<int, float>,
                                  std::shared_ptr<X> >;

// Metafunctions
template<class...> using F1 = void;

template<class T> using F2 = T*;

template<class... T> using F3 = std::integral_constant<std::size_t, sizeof...(T)>;

// Quoted metafunctions
struct Q1 { template<class...> using fn = void; };

struct Q2 { template<class T> using fn = T*; };

struct Q3 { template<class... T> using fn =
  std::integral_constant<std::size_t, sizeof...(T)>; };

// Map examples
using M1 = std::tuple<std::pair<int, int*>, std::pair<float, float*>,
    std::pair<void, void*>>;

using M2 = mp_list<mp_list<int, int*>, mp_list<float>,
    mp_list<char, char[1], char[2]>>;



////////////////////////////////////////////
// More examples from
// http://pdimov.com/cpp2/simple_cxx11_metaprogramming.html

using list3_mp11 = mp_rename<std::tuple<int, float, void*>, mp_list>;

using list_mp11 = mp_list<int, char, float, double, void>;

//mp_quote_trait<F> transforms the C++03-style trait F into a quoted metafunction.
// This seems to be missing from the include files athough in the documentation.
// It now works with the current version. - so do I have to define it or not?
/*
template<template<class...> class F> struct mp_quote_trait
{
    template<class... T> using fn = typename F<T...>::type;
};
*/
// Code Example 42. Using mp_quote_trait with std::add_pointer

using QT_L1 = mp_list<int, void, float>;
//using QT_1  = mp_quote_trait<std::add_pointer>;
using QT_R1 = mp_transform_q<mp_quote_trait<std::add_pointer>, QT_L1>;
  // mp_list<int*, void*, float*>

// I came across this while trying to understand mp_inherit which is used in this case
// and is assumed to be self explanatory.

// Set contains uses inherit
typedef mp_set_contains<QT_L1,int>::type set_result_L1;

// I have then set out to find a version of mp_quote_trait which will handle it. Here it is.

// Extensions to MP11 invented to handle case of set_contains.
template<template<class, class> class F> struct mp_quote_trait_S_V
{
  template<class S, class V> using fn = typename F<S,V>::type;
};

template<class Q, class S, class V> using mp_invoke_S_V = typename Q::template fn<S,V>;

using mp_set_contains_q_t = mp_quote_trait_S_V<mp_set_contains>;

typedef mp_invoke_S_V<mp_set_contains_q_t,QT_R1,int*> set_result_R1;


////////////////////////////////////////////
// Set up the list here
////////////////////////////////////////////

std::vector< std::string > make_list() {
  std::vector< std::string > list;
  list.push_back( "duck" );
  list.push_back( "duck" );
  list.push_back( "goose" );
  return list;
}


//////////////////////////////////////////////
// First example using standard library only
//////////////////////////////////////////////

bool IsNotGoose( const std::string& s )
{
  return s != "goose";
}

// Demonstration of use of std::not_fn which is new in C++17
// https://en.cppreference.com/w/cpp/utility/functional/not_fn
void delete_value1(std::vector< std::string > &list )
{
  list.erase( std::remove_if( list.begin(), list.end(), std::not_fn(IsNotGoose) ), list.end() );
}

/////////////////////////////////////////////
// Second example using boost bind
//////////////////////////////////////////////

bool isValue(const std::string &s1, const std::string &s2)
{
  return s1==s2;
}

void delete_value2(std::vector< std::string > &list, const std::string & value)
{
  list.erase(
    std::remove_if(
        list.begin(),
        list.end(),
        boost::bind(
            isValue, // &isValue works as well.
            boost::placeholders::_1, // Boost.Bind placeholder
            boost::cref( value ) ) ),
    list.end() );
}

///////////////////////////////////////////////////////
// Third example using boost phoenix for the comparison
///////////////////////////////////////////////////////

namespace phx = boost::phoenix;
using phx::placeholders::arg1;
using phx::placeholders::arg2;

void delete_value3(std::vector< std::string > &list, const std::string & value)
{
  list.erase( std::remove_if(
        list.begin(),
        list.end(),
        // This needs header boost/phoenix/operator/comparison.
        // arg1 is a Boost.Phoenix placeholder.
        arg1 == phx::cref( value ) ), 
        list.end() );
}

//////////////////////////////////////////////////////////////
// Fourth example using boost phoenix for the algorithm as well
//////////////////////////////////////////////////////////////

void delete_value4(std::vector< std::string > &list, const std::string & value)
{
  // This needs header boost/phoenix/stl/algorithm/transformation
  // It uses two Boost.Phoenix placeholders.
  list.erase( phx::remove_if( arg1, arg2 )
            ( list, arg1 == phx::cref( value ) ),
            list.end() );
}

//////////////////////////////////////////////////////////
// Functions common to all examples
//////////////////////////////////////////////////////////

void out_string(const std::string  &s)
{
  Serial.println( s.c_str() );
}

void show_list1( const std::vector< std::string > &list )
{
  std::for_each(list.begin(), list.end(), out_string);
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
  Serial.print("Waited for ");
  Serial.print(t2);
  Serial.println(" millis");
  while (!delay_without_delaying(5000) ) { };
  //delay(2000);
  Serial.println("Arduino RP2040 Boost Phoenix Test");
  std::string value = "goose";

  std::vector< std::string > list1 = make_list();
  show_list1(list1);
  Serial.println("--------");
  delete_value1(list1);
  show_list1(list1);
  Serial.println("--------");
  std::vector< std::string > list2 = make_list();
  delete_value2(list2,value);
  show_list1(list2);
  Serial.println("--------");
  std::vector< std::string > list3 = make_list();
  delete_value3(list3,value);
  show_list1(list3);
  Serial.println("--------");
  std::vector< std::string > list4 = make_list();
  delete_value4(list4,value);
  show_list1(list4);
  Serial << "==============================================================" << endl;
  Serial << "Examples with mp_size<T>" << endl;
  Serial << "==============================================================" << endl;
  Serial << "mp_size<list_mp11>::value : " << mp_size<list_mp11>::value << endl;;
  Serial.println("--------");
  while (!delay_without_delaying(5000) ) { };
  pinMode(LED_BUILTIN, OUTPUT);

}

//////////////////////////////////////////////////////////

int LEDstate = 0;

void loop() {
  if (delay_without_delaying(1000)) {
    LEDstate = !LEDstate;
    digitalWrite(LED_BUILTIN, LEDstate);
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    if (LEDstate) {
      Serial.println("Arduino blink ON");
    } else {
      Serial.println("Arduino blink OFF");
    }
  }
}
