/// @file TestVlpp.ino
/// @brief First experiment with Functional-Vlpp library
///
/// https://github.com/khoih-prog/functional-vlpp

// For this functional-vlpp library
#include <functional-vlpp.h>

/// Example adapted from https://blog.mbedded.ninja/programming/languages/c-plus-plus/callbacks/

class LibraryClass {
public:
  //void passACallbackToMe(std::function<int(int, int)> callback) {
  void passACallbackToMe(vl::Func<int(int, int)> callback) {
      // Now invoke (call) the callback
    int o = callback(1, 2);
        printf("Value: %i\n", o); // We might be on an embedded system, use printf() and not std::cout
  }
};

class MyClass {
public:
      int methodToCallback(int num1, int num2) {
          return num1 + num2;
      }
};


 


void setup() 
{
    MyClass myClass;
    
    LibraryClass libraryClass;

 // Use a lambda to capture myClass and call the member method
    libraryClass.passACallbackToMe([&myClass](int num1, int num2) -> int {
        return myClass.methodToCallback(num1, num2);
    });


    // Alternate way to using a lambda, use std::bind instead. However I recommend the lambda way.
    //libraryClass.passACallbackToMe(std::bind(&MyClass::methodToCallback, myClass, std::placeholders::_1, std::placeholders::_2));

}

void loop() {
  // put your main code here, to run repeatedly:

}
