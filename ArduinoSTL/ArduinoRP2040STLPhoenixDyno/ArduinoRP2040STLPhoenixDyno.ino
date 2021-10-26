//////////////////////////////////////////////////////////
// Arduino RP2040 Boost Phoenix Dyno and Callable tests
// Adding examples from FC++NewCode\callable_traits_throwassert_example.cpp
// https://www.boost.org/doc/libs/master/libs/callable_traits/doc/html/index.html
// There is quite a lot which can be done using callable traits.
// I have now added an example of use of the dyno library as well.
////////////////////////////////////////////////////////////////////////////
// ThrowAssert is not usable with the Arduino-Pico system which does not support exceptions.
////////////////////////////////////////////////////////////////////////////

// This is a comparison of several ways of deleting an entry from a list of strings.

// This fails with the following link error.

// c:/users/user/appdata/local/arduino15/packages/rp2040/tools/pqt-gcc/1.3.1-a-7855b0c/bin/../lib/gcc/arm-none-eabi/10.3.0/../../../../arm-none-eabi/bin/ld.exe:
// c:/users/user/appdata/local/arduino15/packages/rp2040/tools/pqt-gcc/1.3.1-a-7855b0c/bin/../lib/gcc/arm-none-eabi/10.3.0/../../../../arm-none-eabi/lib/thumb\libstdc++.a(locale.o):
// in function `get_locale_cache_mutex': /workdir/repo/gcc-gnu/libstdc++-v3/src/c++98/locale.cc:36: undefined reference to `__sync_synchronize'

// unless boost/phoenix/core/debug.hpp is not compiled in boost/phoenix/core.hpp. It includes <iostream> which may well be the issue.

#undef F
#include <boost_utility_result_of.hpp>
#include <boost_function.hpp>
#include <boost_bind.hpp>
//#include <boost_locale.hpp> Not the answer
#include <boost_phoenix_core.hpp>
// Adding the contents of boost_phoenix_core.hpp one by one.
//#include <boost_phoenix_version.hpp>
//#include <boost_phoenix_core_limits.hpp>
//#include <boost_phoenix_core_actor.hpp>
//#include <boost_phoenix_core_debug.hpp> Aha this is it. Can I manage without it? It seems so.
//#include <boost_phoenix_core_is_actor.hpp>
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

bool IsNotGoose( const std::string& s )
{
  return s != "goose";
}

// Demonstration of use of std::not_fn which is new in C++17
// https://en.cppreference.com/w/cpp/utility/functional/not_fn
void delete_value1(std::vector< std::string > &list )
{
  list.erase( std::remove_if( list.begin(), list.end(), std::not_fn(IsNotGoose) ), list.end() );
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

///////////////////////////////////////////////////////
// Third example using boost phoenix for the comparison
///////////////////////////////////////////////////////

namespace phx = boost::phoenix;
using phx::placeholders::arg1;
using phx::placeholders::arg2;

void delete_value3(std::vector< std::string > &list, const std::string & value)
{
  list.erase( std::remove_if(
        list.begin(),
        list.end(),
        // This needs header boost/phoenix/operator/comparison.
        // arg1 is a Boost.Phoenix placeholder.
        arg1 == phx::cref( value ) ), 
        list.end() );
}

//////////////////////////////////////////////////////////////
// Fourth example using boost phoenix for the algorithm as well
//////////////////////////////////////////////////////////////

void delete_value4(std::vector< std::string > &list, const std::string & value)
{
  // This needs header boost/phoenix/stl/algorithm/transformation
  // It uses two Boost.Phoenix placeholders.
  list.erase( phx::remove_if( arg1, arg2 )
            ( list, arg1 == phx::cref( value ) ),
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
template <typename T>
auto const dyno::default_concept_map<Drawable, T> = dyno::make_concept_map(
  dyno_draw = [](T const& self) { self.draw(); }
//  "draw"_s = [](T const& self) { self.draw(); }
);

// Define an object that can hold anything that can be drawn.
struct drawable {
  template <typename T>
  drawable(T x) : poly_{x} { }

  void draw() const
  { poly_.virtual_("draw"_s)(); }

private:
  dyno::poly<Drawable> poly_;
};

struct Square {
  void draw() const { Serial << "Square"; }
};

struct Circle {
  void draw() const { Serial << "Circle"; }
};

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
  Serial.println("Arduino RP2040 Boost Phoenix Dyno Test");
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
  std::vector< std::string > list3 = make_list();
  delete_value3(list3,value);
  show_list1(list3);
  Serial.println("--------");
  std::vector< std::string > list4 = make_list();
  delete_value4(list4,value);
  show_list1(list4);
  Serial.println("--------");
  while (!delay_without_delaying(5000) ) { };
  Serial << "Dyno example" << endl;
  Serial.println("--------");
  f(Square{}); // prints Square
  //ds.draw();
  Serial << endl;
  f(Circle{}); // prints Circle
  //dc.draw();
  Serial << endl;
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
