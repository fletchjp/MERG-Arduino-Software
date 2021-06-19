// ArduinoBoostFunctionPhoenix
// This example now works as long as the AVR Boards are 1.8.2 not 1.8.3
#include <ArduinoSTL.h>
// This is an old version of boost from 2012.
#include <boost_1_51_0.h>
// Example use of a Boost header

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
#include <string>
#include <vector>

#define BOOST_NO_CWCHAR // Needed for example4
#include <boost/utility/result_of.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/phoenix/operator/comparison.hpp>
#include <boost/phoenix/stl/algorithm/transformation.hpp>


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
  std::cout << "==============================================" << std::endl;
  std::string value = "goose";

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
 
  
}

void loop() {

}
