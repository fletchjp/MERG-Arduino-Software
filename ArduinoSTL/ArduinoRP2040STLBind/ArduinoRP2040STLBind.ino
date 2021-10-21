//////////////////////////////////////////////////////////
// Arduino RP2040 Boost tests
//////////////////////////////////////////////////////////

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


void setup() {
  Serial.begin(115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 10000));
  t2 = millis() - t1;
  Serial.print("Waited for ");
  Serial.print(t2);
  Serial.println(" millis");
  delay(2000);
  Serial.println("Arduino RP2040 Boost Test");
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
