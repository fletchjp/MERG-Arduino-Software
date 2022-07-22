//////////////////////////////////////////////////////////
// Arduino RP2040 Boost Phoenix tests
//////////////////////////////////////////////////////////

// This is a comparison of several ways of deleting an entry from a list of strings.

// This fails with the following link error.

// c:/users/user/appdata/local/arduino15/packages/rp2040/tools/pqt-gcc/1.3.1-a-7855b0c/bin/../lib/gcc/arm-none-eabi/10.3.0/../../../../arm-none-eabi/bin/ld.exe:
// c:/users/user/appdata/local/arduino15/packages/rp2040/tools/pqt-gcc/1.3.1-a-7855b0c/bin/../lib/gcc/arm-none-eabi/10.3.0/../../../../arm-none-eabi/lib/thumb\libstdc++.a(locale.o):
// in function `get_locale_cache_mutex': /workdir/repo/gcc-gnu/libstdc++-v3/src/c++98/locale.cc:36: undefined reference to `__sync_synchronize'

// unless boost/phoenix/core/debug.hpp is not compiled in boost/phoenix/core.hpp. It includes <iostream> which may well be the issue.

#undef F
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
#include <string>
#include <vector>
#include <functional>

// 3rd party libraries
#include <Streaming.h>

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
            _1, // Boost.Bind placeholder
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

///////////////////////////////////////////////////////
// Fifth example using boost phoenix bind for the bind and for the comparison
///////////////////////////////////////////////////////

void delete_value5(std::vector< std::string > &list, const std::string & value)
{
  list.erase(
    std::remove_if(
        list.begin(),
        list.end(),
        boost::phoenix::bind(
            isValue, // &isValue works as well.
            arg1, // Boost.Bind placeholder
            boost::cref( value ) ) ),
    list.end() );
}

///////////////////////////////////////////////////////
// Sixth example using boost phoenix directly for the comparison
///////////////////////////////////////////////////////

void delete_value6(std::vector< std::string > &list, const std::string & value)
{
  list.erase(
    std::remove_if(
        list.begin(),
        list.end(),
        arg1 == phx::cref( value ) ),
    list.end() );
}

///////////////////////////////////////////////////////
// Seventh example using boost phoenix directly for the comparison
///////////////////////////////////////////////////////

void delete_value7(std::vector< std::string > &list, const std::string & value)
{
  list.erase(
    phx::remove_if( arg1, arg2 )
        (list, arg1 == phx::cref( value ) ),
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
  Serial.println("Arduino RP2040 Boost Phoenix Test");
  std::string value = "goose";

  std::vector< std::string > list1 = make_list();
  show_list1(list1);
  Serial.println("--------1");
  delete_value1(list1);
  show_list1(list1);
  Serial.println("--------2");
  std::vector< std::string > list2 = make_list();
  delete_value2(list2,value);
  show_list1(list2);
  Serial.println("--------3");
  std::vector< std::string > list3 = make_list();
  delete_value3(list3,value);
  show_list1(list3);
  Serial.println("--------4");
  std::vector< std::string > list4 = make_list();
  delete_value4(list4,value);
  show_list1(list4);
  Serial.println("--------5");
  std::vector< std::string > list5 = make_list();
  delete_value5(list5,value);
  show_list1(list5);
  Serial.println("--------6");
  std::vector< std::string > list6 = make_list();
  delete_value6(list6,value);
  show_list1(list6);
  Serial.println("--------7");
  std::vector< std::string > list7 = make_list();
  delete_value7(list7,value);
  show_list1(list7);
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