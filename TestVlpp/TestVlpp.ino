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
  void RunACallbackForMe(vl::Func<int(int, int)> callback) {
      // Now invoke (call) the callback
    int o = callback(1, 2);
    Serial.print("Value:");
    Serial.println(o); // We might be on an embedded system, use printf() and not std::cout
  }
  void UseACallbackForMe(vl::Func<int(int, int)> callback,int x,int y) {
    // Now invoke (call) the callback using the values provided
    int o = callback(x, y);
    Serial.print("Value:");
    Serial.println(o); // We might be on an embedded system, use printf() and not std::cout
  }
  

};

class MyClass {
public:
      int methodToCallback(int num1, int num2) {
          return num1 + num2;
      }
};


// These need to be global to be seen in loop.
    MyClass myClass;
    
    LibraryClass libraryClass;


void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  delay(5000);
  Serial.println("TestVlpp Functional Programming Library");

 // Use a lambda to capture myClass and call the member method
    libraryClass.RunACallbackForMe([&myClass](int num1, int num2) -> int {
        return myClass.methodToCallback(num1, num2);
    });

    libraryClass.UseACallbackForMe([&myClass](int num1, int num2) -> int {
        return myClass.methodToCallback(num1, num2);
    },3,4);


    // Alternate way to using a lambda, use std::bind instead. However I recommend the lambda way.
    //libraryClass.passACallbackToMe(std::bind(&MyClass::methodToCallback, myClass, std::placeholders::_1, std::placeholders::_2));

}

void loop() {
  // put your main code here, to run repeatedly:

}
