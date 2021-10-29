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

// Adding things from example/return_type.cpp
using expect = int;

// foo is a function object
struct foo {
    void operator()(int, char, float) const {}
    int bar(int x) const { return x; }
};

struct foo2 {
    int operator()(int x, char, float) const { return x; }
    int bar(int x) const { return x; }
};

struct foo3 {
    void bar() const {}
};

using pmf2     = int(foo2::*)(int, char, float) const;

template<typename T>
void test() {
    using result = ct::return_type_t<T>;
    static_assert(std::is_same<expect, result>{}, "");
}

// Example adapted from the manual.
using const_removed = ct::remove_member_const_t<decltype(&foo3::bar)>;

static_assert(std::is_same<const_removed, void(foo3::*)()>::value, "");

void callable_tests() {
  // This is how to get pmf to a member function
    int (foo2::* pmf2bar)(int) const = &foo2::bar;

    // Use args_t to retrieve a parameter list as a std::tuple:
    assert_same<
        ct::args_t<foo>,
        std::tuple<int, char, float>
    >();

    // arg_at_t gives us indexed access to a parameter list
    /* This is not now included.
     * It was removed in the review for Boost acceptance.
    assert_same<
        ct::arg_at_t<1, foo>,
        char
    >();
    */
    // This is the equivalent operation using the tuple returned by ct::args_t.
    using SelectedType = std::tuple_element_t<1,ct::args_t<foo>>;
    assert_same<
        SelectedType,
        char
    >();
}

//////////////////////////////////////////////////////////
// These can replace the string literals and are better with C++17
static auto foobar = DYNO_STRING("foobar");
// I am defining these as dyno_something to avoid name clashes
// They could be in a namespace.
static auto dyno_call = DYNO_STRING("call");
static auto dyno_draw = DYNO_STRING("draw");

using namespace dyno::literals;

// Note that I am leaving out the std::ostream argument as that will be Serial.

// Define the interface of something that can be drawn
struct Drawable : decltype(dyno::requires(
  dyno_draw = dyno::method<void () const>
//  "draw"_s = dyno::method<void () const>
)) { };

// Define how concrete types can fulfill that interface
// I have changed this from being the named default_concept_map
// as that is then picked up by the std::vector<T> which fails.
// When I do it like this it works for the Square case.
// The alternative is used for Circle and the vector<T> case also works.
// It may be possible to reinstate the default_concept_map with a third template parameter
// using std::enable_if_t<something> to turn it on and off as needed.
// I have a hint for that in dyno/concept_map.hpp but not an example.
// Source for the code to test for std::vector. There must be an official version.
// https://stackoverflow.com/questions/9392777/enable-template-only-for-some-stdvectort2-type
template <class T>
struct is_not_std_vector { static const bool value=true; };
template <class T>
struct is_not_std_vector<std::vector<T> > { static const bool value=false; };
// This does now work
// It makes sure that the default concept map is NOT uses when T is a std::vector object.
template <class T>
auto const dyno::default_concept_map<Drawable, T, std::enable_if_t<is_not_std_vector<T>::value> > = dyno::make_concept_map(
//auto const dyno::concept_map<Drawable, T> = dyno::make_concept_map(
  dyno_draw = [](T const& self) { self.draw(); }
//  "draw"_s = [](T const& self) { self.draw(); }
);

// Define an object that can hold anything that can be drawn.
struct drawable {
  template <typename T>
  drawable(T x) : poly_{x} { }

  void draw() const
  { poly_.virtual_(dyno_draw)(); }

private:
  dyno::poly<Drawable> poly_;
};

struct Square {
  void draw() const { Serial << "Square"; }
};

struct Circle {
  void draw() const { Serial << "Circle"; }
};

//Circle now prints triangle
template <>
auto dyno::concept_map<Drawable, Circle> = dyno::make_concept_map(
  dyno_draw = [](Circle const& circle) {
    Serial << "triangle" << endl;
  }
);

//drawable ds(Square{});
//drawable dc(Circle{});
/*
 * This does not compile without this declaration
 * which is not needed in the example code.
 */
void f(drawable const &d);

void f(drawable const &d) {
  d.draw();
}

// Extensions of drawable from the Readme.

// Parametric concept map
// This does not work as it expects to fulfil the default concept map as well
// until that is modified to exclude this case - see the code for that higher up.
// The documentation does not make it clear that this is needed.
// I had to read the code of concept_map.hpp to find out more.
// I also found some examples using std::enable_if_t in builtin.hpp
template <typename T>
auto const dyno::concept_map<Drawable, std::vector<T>
//, std::void_t<decltype(Serial << std::declval<T>())>
> = dyno::make_concept_map(
//  "draw"_s = [](std::vector<T> const& v) {
    dyno_draw = [](std::vector<T> const& v) {
    for (auto const& x : v)
      Serial << x << ' ';
  }
);

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
  f(Square{}); // prints Square
  //ds.draw();
  Serial << endl;
  f(Circle{}); // prints Circle
  //dc.draw();
  // Parametric Concept Map example
  //Serial << endl;
  f(std::vector<int>{1, 2, 3}); // prints "1 2 3 "
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
