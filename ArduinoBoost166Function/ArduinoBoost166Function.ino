// ArduinoBoost166Function
// I am having a lot of trouble to get this to work with Boost for Arduino (1.66.0)
// There are still problems in functional/hash.
#define BOOST_FOR_ARDUINO
#include <ArduinoSTL.h>
//#include <typeinfo>
//#include <array>
#include <functional>
// All these defines are needed to sort things out.
#define BOOST_NO_CWCHAR
#define BOOST_NO_CXX11_HDR_TYPEINDEX
#define BOOST_NO_CXX11_HDR_ARRAY
#define BOOST_NO_CXX11_HDR_TUPLE
#define BOOST_NO_CXX11_SMART_PTR
#define BOOST_NO_STD_WSTRING
#define BOOST_DISABLE_ASSERTS
#define BOOST_NO_EXCEPTIONS
#define BOOST_EXCEPTION_DISABLE

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

#undef B1
#undef F


#include <boost_function.hpp>
// Example use of a Boost header
#include <boost_type_traits.hpp>

using namespace std;

void setup() {
  Serial.begin(9600);
  cout << "Feed me an integers." << endl;
}

void loop() {
  int foo;
  if (cin >> foo) { 
    cout << "You fed me " << foo << endl;
  }else{
    cin.clear();
    cin.ignore();
  }
}
