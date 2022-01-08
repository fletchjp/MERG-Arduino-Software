/// @file TestModern.ino
/// @brief Test ideas from Hackaday.
///
/// https://hackaday.com/2017/05/05/using-modern-c-techniques-with-arduino/

// 3rd party libraries
#include <Streaming.h>

/// How big are your integers?
///
///int8_t/uint8_t - a signed/unsigned type that is exactly 8 bits in size.
///int16_t/uint16_t - a signed/unsigned type that is exactly 16 bits in size.
///int32_t/uint32_t - a signed/unsigned type that is exactly 32 bits in size.
///int64_t/uint64_t - a signed/unsigned type that is exactly 64 bits in size.

/// Anonymous Namespaces
///
/// This means that these items are only visible in this file.
namespace {
  const int16_t SOME_VAR = 1000;  // Now it's type-safe
  int16_t count_up = 0;  // No need to use static
  const int16_t numLeds = 0;  // Still declared const.
 
  class thisClassHasACommonName {
    //...
  };
}

/// Using Using
///
/// These are equivalent
typedef void(*fp1)(int, const char*);
/// This one is more readable. 
using fp2 = void(*)(int, const char*);
/// It can also have a template version
template <typename T>
using fp3 = void(*)(T, const char*);

/// nullptr function declared

int *GetResult() { return nullptr; }

/// Default initialization

class Foo {
  Foo() /* : fooString(nullptr)*/ { }
  // comment out unused parameter names.
  Foo(const char* /*str*/) /* : fooString(nullptr) */ {  }
  Foo(const Foo& /*other*/) /* : fooString(nullptr) */ { }
  private:
    char* fooString = nullptr; // replaces all the initialisations
};

/// Scoping your enums - definition

enum class Colour {
  white,
  blue,
  yellow
};


void setup() {
  // put your setup code here, to run once:
  while(!Serial);
  Serial.begin (115200);
  Serial << endl << endl << F("> ** TestModern ** ") << __FILE__ << endl;

  Serial << count_up << endl;

/// Automatic for the people

  int myValue = 5; 
  auto myValuePtr = &myValue;
  auto& temp1 = myValue; 
  const auto& temp2 = myValue;

  Serial << temp1 << " " << temp2 << " " << *myValuePtr << endl;

/// nullptr function used 

  auto result = GetResult();
 
  if (result == nullptr) {
       Serial <<F("GetResult returns a nullptr") << endl;
    }

/// Scoping your enums - use

    Colour colour = Colour::white; // Correct.
    // Cast is needed to the underlying int type.
    if (colour == Colour::white) Serial << (int)colour << F(" is white") << endl;
}

void loop() {
  // put your main code here, to run repeatedly:

}
