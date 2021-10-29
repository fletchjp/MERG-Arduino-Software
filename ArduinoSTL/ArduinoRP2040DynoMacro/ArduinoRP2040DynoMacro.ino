//////////////////////////////////////////////////////////
// Arduino RP2040 Dyno Macro
// Adding examples from BoostFC++\fcpp_clients\dyno_overview_nonconst_macro.cpp
// 
////////////////////////////////////////////////////////////////////////////

#undef F
#include <boost_utility_result_of.hpp>
#include <boost_function.hpp>
#include <boost_bind.hpp>
#include <boost_phoenix_core.hpp>
#include <boost_phoenix_bind.hpp>
#include <boost_phoenix_operator_comparison.hpp>
#include <boost_phoenix_stl_algorithm_transformation.hpp>
#include <boost_callable_traits.hpp>
#include <cstddef>
#include <string>
#include <array>
#include <iterator>
#include <utility>
#include <vector>
#include <functional>
#include <type_traits>
#include <tuple>
#include <typeinfo>
// default_concept_map disabled as it uses typeid which is not available for the RP2040 
#include <dyno.hpp>
#include <dyno_testing.hpp>
#include <dyno_any_iterator.hpp>


// 3rd party libraries
#include <Streaming.h>

//////////////////////////////////////////////////////////

namespace ct = boost::callable_traits;

// This function template helps keep our example code neat
template<typename A, typename B>
void assert_same(){ static_assert(std::is_same<A, B>::value, ""); }

// These can replace the string literals and are better with C++17
static auto foobar = DYNO_STRING("foobar");
// I am defining these as dyno_something to avoid name clashes
// They could be in a namespace.
static auto dyno_call = DYNO_STRING("call");
static auto dyno_draw = DYNO_STRING("draw");



// Extensions of drawable from the Readme.



// Non member functions. The code for this is in the README with no example of its use which I can find.
// I do not understand the wording here. They are not non member functions.

// Define the interface of something that can be eaten
struct Eatable : decltype(dyno::requires(
  "eat"_s = dyno::function<void (dyno::T const&)>
)) { };

// Define how concrete types can fulfill that interface
template <typename T>
auto const dyno::default_concept_map<Eatable, T> = dyno::make_concept_map(
  "eat"_s = [](T const& self) { self.eat(); }
  //           ^^^^^^^^^^^^^^ matches the concept definition
);

// Define an object that can hold anything that can be eaten.
struct eatable {
  template <typename T>
  eatable(T x) : poly_{x} { }

  void eat() const
  { poly_.virtual_("eat"_s)(poly_); }
  //                        ^^^^^ passing the poly explicitly

private:
  dyno::poly<Eatable> poly_;
};

struct Cake {
  void eat() const { Serial << "cake" << endl; }
};

void g(eatable const &e);

void g(eatable const &e)
{
   e.eat();
}

struct Anything : decltype(dyno::requires(
  "anything"_s = dyno::function<void (dyno::T const&)>
)) { };

template <typename T>
auto const dyno::default_concept_map<Anything, T> = dyno::make_concept_map(
  "anything"_s = [](T const& self) { self(); } // Note that it calls itself.
  //           ^^^^^^^^^^^^^^ matches the concept definition
);

// Define an object that can hold anything that can be eaten.
struct anything {
  template <typename T>
  anything(T x) : poly_{x} { }

  void operator()() const
  { poly_.virtual_("anything"_s)(poly_); }
  //                        ^^^^^ passing the poly explicitly

private:
  dyno::poly<Anything> poly_;
};

// The requirement on an anything struct is to implement operator()().
struct Cheese {
  void operator()() const { Serial << "cheese" << endl; }
};

// Here is how to excute a non-member function, via a wrapper struct!
void fromage() { Serial << "fromage" << endl; }

struct Fromage {
  void operator()() const { fromage(); }
};

void h(anything const &a);

void h(anything const &a)
{
   a();
}


//
// Example of creating a naive equivalent to `std::function` using the library.
//

template <typename Signature>
struct Callable;

template <typename R, typename ...Args>
struct Callable<R(Args...)> : decltype(dyno::requires(
  dyno::CopyConstructible{},
  dyno::MoveConstructible{},
  dyno::Destructible{},
  dyno_call = dyno::function<R (dyno::T const&, Args...)>
  //"call"_s = dyno::function<R (dyno::T const&, Args...)>
)) { };

template <typename R, typename ...Args, typename F>
auto const dyno::default_concept_map<Callable<R(Args...)>, F> = dyno::make_concept_map(
//  "call"_s = [](F const& f, Args ...args) -> R {
    dyno_call = [](F const& f, Args ...args) -> R {
    return f(std::forward<Args>(args)...);
  }
);

// Provide the default storage policy as in dyno/poly.hpp
template <typename Signature, typename StoragePolicy = dyno::remote_storage>
struct basic_function;

template <typename R, typename ...Args, typename StoragePolicy>
struct basic_function<R(Args...), StoragePolicy> {
  template <typename F = R(Args...)>
  basic_function(F&& f) : poly_{std::forward<F>(f)} { }

  R operator()(Args ...args) const
  { return poly_.virtual_("call"_s)(poly_, std::forward<Args>(args)...); }

private:
  dyno::poly<Callable<R(Args...)>, StoragePolicy> poly_;
};

template <typename Signature>
using function = basic_function<Signature>;

template <typename Signature>
using function_sbo = basic_function<Signature, dyno::sbo_storage<16>>;

template <typename Signature>
using function_view = basic_function<Signature, dyno::non_owning_storage>;

template <typename Signature> // could also templatize the size
using inplace_function = basic_function<Signature, dyno::local_storage<32>>;

//
// Tests
//

struct ToStringAdd {
  ToStringAdd(int num) : num_(num) { }
  std::string to_string_add(int i) const { return std::to_string(num_ + i); }
  int num_;
};

struct ToStringSub {
  ToStringSub(int num) : num_(num) { }
  std::string to_string_sub(int i) const { return std::to_string(num_ - i); }
  int num_;
};

struct ToString {
  std::string operator()(int i) const { return std::to_string(i); }
};



void test_functions() {
    inplace_function<std::string(int)> tostring = std::to_string;
    std::string one = tostring(1);
    Serial << "one = " << one.c_str() << endl;
    {
        auto lambda = [](std::string const& s) {
             return s.size();
        };
        function<int(std::string const&)> sizefn = lambda;
        Serial << "sizefn(\"abcdef\") = " << sizefn("abcdef") << endl;
    }
}

void test_iterators() {
  ////////////////////////////////////////////////////////////////////////////
  // Iteration
  ////////////////////////////////////////////////////////////////////////////
  {
    using Iterator = any_iterator<int, std::forward_iterator_tag>;
    std::vector<int> input{1, 2, 3, 4};
    std::vector<int> result;

    Iterator first{input.begin()}, last{input.end()};
    for (; first != last; ++first) {
      result.push_back(*first);
    }
    if (result != input ) Serial << "Iterator test 1 failed" << endl;
    else  Serial << "Iterator test 1 succeeded" << endl;
    //DYNO_CHECK(result == input);
  }

  {
    using Iterator = any_iterator<int, std::bidirectional_iterator_tag>;
    std::array<int, 4> input{{1, 2, 3, 4}};
    std::array<int, 4> result;
    std::reverse_iterator<Iterator> first{Iterator{input.end()}},
                                    last{Iterator{input.begin()}};
    Iterator out{result.begin()};

    for (; first != last; ++first, ++out) {
      *out = *first;
    }
    std::array<int, 4> expected{{4, 3, 2, 1}};
    if (result != expected ) Serial << "Iterator test 2 failed" << endl;
    else  Serial << "Iterator test 2 succeeded" << endl;
    //DYNO_CHECK(result == expected);
  }
  
}

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
  while (!delay_without_delaying(5000) ) { };
  Serial.println("Arduino RP2040 Dyno Macro Tests");
  Serial.println("--------");
  while (!delay_without_delaying(5000) ) { };
  Serial << "Dyno example" << endl;
  Serial.println("--------");
  Serial << endl;
  g(Cake{});
  h(Cheese{});
  h(Fromage{});
  Serial.println("--------");
  test_iterators();
  Serial.println("--------");
  test_functions();
  Serial.println("--------");
 
  pinMode(LED_BUILTIN, OUTPUT);

}

//////////////////////////////////////////////////////////

int LEDstate = 0;

void loop() {
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
