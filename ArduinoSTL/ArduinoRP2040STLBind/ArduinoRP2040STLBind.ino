 //////////////////////////////////////////////////////////
// Arduino RP2040 Standard tests
// This version does not use Boost Libraries
// I am going to use this to develop 
// new understanding of C++17 std features.
// It also uses delay_without_delaying
//////////////////////////////////////////////////////////

#undef F
// Boost replaced by std:: equivalents as I did not have the libraries
//#include <boost_utility_result_of.hpp>
//#include <boost_function.hpp>
//#include <boost_bind.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>

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

bool IsGoose( const std::string& s )
{
  return s == "goose";
}

void delete_value1(std::vector< std::string > &list )
{
  list.erase( std::remove_if( list.begin(), list.end(), IsGoose ), list.end() );
}

/////////////////////////////////////////////////////
// Second example using standard bind and placeholder
/////////////////////////////////////////////////////

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
        std::bind(
            isValue, // &isValue works as well.
            std::placeholders::_1, // std placeholder
            std::cref( value ) ) ),
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
// std::decay which I have somehow missed from C++11.
// also decay_t = typename decay<T>::type; since C++14
// https://en.cppreference.com/w/cpp/types/decay
//////////////////////////////////////////////////////////

template <typename T, typename U>
struct decay_equiv : 
    std::is_same<typename std::decay<T>::type, U>::type 
{};

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
    Serial << "bools as alpha not available, 1 = true\n" 
           << decay_equiv<int, int>::value << '\n'
           << decay_equiv<int&, int>::value << '\n'
           << decay_equiv<int&&, int>::value << '\n'
           << decay_equiv<const int&, int>::value << '\n'
           << decay_equiv<int[2], int*>::value << '\n'
           << decay_equiv<int(int), int(*)(int)>::value << endl;
  Serial.println("--------");
  // Test this routine
  while (!delay_without_delaying(5000) ) { };
  pinMode(LED_BUILTIN, OUTPUT);
}

//////////////////////////////////////////////////////////
int LEDstate = 0;

void loop() {
  // My first attempt had two separate loops which did not work consistently.
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
