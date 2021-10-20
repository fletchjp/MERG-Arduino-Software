// Arduino RP2040 Boost tests

#undef F
#include <boost_utility_result_of.hpp>
#include <boost_function.hpp>
#include <boost_bind.hpp>
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
  Serial.println( s.c_str() );
}

void show_list1( const std::vector< std::string > &list )
{
  std::for_each(list.begin(), list.end(), out_string);
}

void setup() {
  Serial.begin(115200);
  while(!Serial) { }
  Serial.println("Arduino RP2040 Boost Test");

  std::string value = "goose";

  std::vector< std::string > list1 = make_list();
  delete_value1(list1);
  show_list1(list1);

}

void loop() {
  
}
