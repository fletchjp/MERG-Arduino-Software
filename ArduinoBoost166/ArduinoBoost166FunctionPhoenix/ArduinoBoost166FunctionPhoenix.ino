// ArduinoBoost166FunctionPhoenix
// I have had a lot of trouble to getting boost forward to work with Boost for Arduino (1.66.0)
// It dates from 2017 after structural changes within Boost compared to 1.51.0 from 2012.
// The problems have now been resolved and I think I have a good basis for further work.
// I am using this define to show that I am using Boost for Arduino which I am now only using on a DUE.
#define BOOST_FOR_ARDUINO
// I have found out that compiling on a DUE sets the following define: ARDUINO_SAM_DUE.
// This can be used to test in the source code and have different code for DUE and AVR (MEGA).

// I have had to change the file limits.h in the ArduinoSTL library.
// The problems are all caused by the need to get the functional/hash code to compile.
// There is an extra need for a DUE.

// I have put the following into limits.h to get rid of a problem with functional/hash in 1.66.0
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
//#include <utility>
// All these defines are needed to sort things out.
// They point to limitations in the compiler on the AVR and DUE.
// These have been arrived at by trial and error and inspection of the source code.
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

#include <string>
#include <vector>

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
bool IsGoose( const std::string& s )
{
  return s == "goose";
}

void delete_value1(std::vector< std::string > &list )
{
  list.erase( std::remove_if( list.begin(), list.end(), IsGoose ), list.end() );
}

void out_string(const std::string  &s)
{
  std::cout << s << std::endl;
}

void show_list1( const std::vector< std::string > &list )
{
  std::for_each(list.begin(), list.end(), out_string);
}


//////////////////////////////////////////////
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


void setup() {
  Serial.begin(9600);
  std::cout << std::endl;
  std::cout << "==============================================" << std::endl;
  std::cout << "Boost function bind example running on Arduino" << std::endl;
  std::cout << "using Boost for Arduino (1.66.0)" << std::endl;
  #ifdef ARDUINO_SAM_DUE
  std::cout << "running on an Arduino DUE" << std::endl;
  #else
  std::cout << "running on MEGA (AVR) which is liable to crash" << std::endl;
  #endif
  std::cout << "==============================================" << std::endl;
  std::string value = "goose";

  #ifdef ARDUINO_SAM_DUE
  std::vector< std::string > list1 = make_list();
  delete_value1(list1);
  show_list1(list1);
  std::cout << "--------------------------------" << std::endl;
  std::vector< std::string > list2 = make_list();
  delete_value2(list2,value);
  show_list1(list2);
  std::cout << "--------------------------------" << std::endl;
  std::vector< std::string > list3 = make_list();
  delete_value3(list3,value);
  show_list1(list3);
  std::cout << "--------------------------------" << std::endl;
  std::vector< std::string > list4 = make_list();
  delete_value4(list4,value);
  show_list1(list4);
  std::cout << "--------------------------------" << std::endl;

#else
  std::cout << "All examples out of use for AVR" << std::endl;
#endif
  std::cout << "=========================================" << std::endl;

}

void loop() {

}
