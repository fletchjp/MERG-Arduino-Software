/// @file TestBetterEnum.ino
/// @brief Test ideas from Hackaday and Better Enum
///
/// BETTER_ENUM is a macro which sets up enum classes (scoped enum)
///
/// https://hackaday.com/2017/05/05/using-modern-c-techniques-with-arduino/
///
/// It does some things neatly but not comparison which has to be cast.



// 3rd party libraries
#include <Streaming.h>
#include <ArduinoSTL.h>
#include <type_traits>
// https://github.com/aantron/better-enums
#include "enum.h" //requires cstddef fom ArduinoSTL to work here.

/// Example declaration 
BETTER_ENUM(Word, int, Hello, World)

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
  const int16_t SOME_VAR = 1000;  /// Now it's type-safe
  int16_t count_up = 0;  /// No need to use static
  const int16_t numLeds = 0;  /// Still declared const.
  /// thisClassHasACommonName
  class thisClassHasACommonName {
   public:
     int x;
     thisClassHasACommonName() : x(0) {}
     thisClassHasACommonName(int y) : x(y) {}
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
enum class Colour : int {
  white,
  blue,
  yellow
};

/// BETTER_ENUMS defining a scoped enum.
BETTER_ENUM(Colours,int,white,red,blue)

/// example from https://forum.arduino.cc/t/solve-enum-on-uno-versus-due-why-due-need-static_cast/545642
enum foo { bar, foobar, barfoo };

void setup() {
  // put your setup code here, to run once:
  while(!Serial);
  Serial.begin (115200);
  Serial << endl << endl << F("> ** TestBetterEnum ** ") << __FILE__ << endl;

  Serial << (+Word::Hello)._to_string() << " " << (+Word::World)._to_string() << endl;

/// Use of variable from an anonyomous namespace
  Serial << count_up << endl;

/// thisClassHasACommonName
  thisClassHasACommonName example(2);
  Serial << example.x << endl;

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

/// Scoping your enums - use this
    Colour colour = Colour::white;
/// Cast is needed to the underlying int type for output.
    if (colour == Colour::white) Serial << (int)colour << F(" is white") << endl;
/// This is not available.
///if (colour == Colour::white) Serial << (std::underlying_type<Colour>::type)colour << F(" is white") << endl;

/// BETTER_ENUM example
    Colours colours = Colours::white;
    Colours colours2 = Colours::white;
    // This needs a cast to Colours to compile.
    if (colours == (Colours)Colours::white) Serial << colours._to_integral() << " is " 
                                                   << colours._to_string() << endl;
    if (colours == colours2) Serial << colours._to_integral() << " is " 
                                                   << colours._to_string() << endl;
                                                   
/// https://forum.arduino.cc/t/solve-enum-on-uno-versus-due-why-due-need-static_cast/545642
    int valueToEvaluate = 2;  // warning: invalid conversion from 'int' to 'foo' [-fpermissive]
    foo myFoo = valueToEvaluate; //this compile only on UNO

}

void loop() {
  // put your main code here, to run repeatedly:

}
