//////////////////////////////////////////////////////////
// Arduino RP2040 Dyno Macro
// Adding examples from BoostFC++\fcpp_clients\dyno_overview_nonconst_macro.cpp
// These are going to be changed to remove all non-const examples.
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

///////////////////////////////////////////////////////////////////////////////////////
// Code for the new examples.
///////////////////////////////////////////////////////////////////////////////////////

// Implemented for one arg to get things going.
// The use is optional as the member functions can be implemented directly,
// see below.
// Taking out the template T which was for the output argument
#define DYNO_CONST_MEMBER_VOID(name, cname)            \
  void name() const { cname(); }
//  template <typename T> void name(T& t) const { cname(t); }
#define DYNO_CONST_MEMBER(ret, name, cname)           \
  ret name() const { return cname(); }
//  template <typename T> ret name(T& t) const { return cname(t); }
#define DYNO_CONST_MEMBER_VOID_ARGS(name, cname)            \
  template <typename ...Args> void name(Args... args) \
    const { cname(args...); }
#define DYNO_CONST_MEMBER_ARGS(ret, name, cname)            \
  template <typename ...Args> ret name(Args... args) const { return cname(args...); }

// The best I can do is typedefs which could be changed.
typedef int Arg1;
typedef double Arg2;

DYNO_INTERFACE(Drawable,
  (draw, void () const),
  (one, void (const Arg1&) const ),
  (two, void (const Arg1&,const Arg2&) const )
);

// This is a way to add the extra member function to Drawable.
// It also needs the constructor
// It is a way to make a templated version as well
// although not needed here.
//template <typename Arg1,typename Arg2>
struct Drawable_plus : public Drawable {
  template <typename T>
  Drawable_plus(T x) : Drawable(x) {}
  // Both need to be implemented if the non-const one is needed.
  //DYNO_CONST_MEMBER_VOID(draw, cdraw)
  //DYNO_NON_CONST_MEMBER_VOID(draw, Drawable )
  //DYNO_CONST_MEMBER_VOID_ARGS(one, draw )
  // These are equivalent to the macros.
  // These compile while the macro version does not.
  //void draw() const { cdraw(); }
  template <typename Arg1>
  void one(const Arg1& a1) const { one(a1); }
  template <typename Arg1,typename Arg2>
  void two(const Arg1& a1,const Arg2& a2) const { two(a1,a2); }
};

template<typename Arg1,typename Arg2>
struct Square {
  //void draw() { Serial << "Square (nc) "; s = 1; }
  // This is not needed as the link is made in Drawable_plus.
  // void draw() const { cdraw(out); }
  void draw() const { Serial << "Square (c) "; }
  void one(const Arg1& arg1) const
  {  Serial << arg1 << " "; }
  void two(const Arg1& arg1, const Arg2& arg2) const
  { Serial << arg1 << " " << arg2 << " "; }
};

struct Circle {
  //void draw() { Serial<< "Circle (nc) "; c = 2; }
  // This is not needed as the link is made in Drawable_plus.
  // void draw() const { cdraw(out); }
  void draw() const { Serial << "Circle (c) "; }
  template <typename Arg1>
  void one(const Arg1& arg1) const
  {  Serial << arg1 << " "; }
  template <typename Arg1,typename Arg2>
  void two(const Arg1& arg1, const Arg2& arg2) const
  {  Serial << arg1 << " " << arg2 << " "; }
  //template <typename Arg1>
};

// Advance declarations are needed here.
void fc(Drawable const& d);
void fca(Drawable const& d, const int& arg1);
void fcb(Drawable const& d, const int& arg1, const double& arg2 );
void fcp(Drawable_plus const& d);


void fc(Drawable const& d) {
  d.draw();  // calls the const version directly
}

//void fca(Drawable_plus<int,double> const& d, const int& arg)
void fca(Drawable const& d, const int& arg1)
{
  d.draw();
  // This call is crashing.
  d.one(arg1);
}

//void fcb(Drawable_plus<int,double> const& d, const int& arg1, const double& arg2 )
void fcb(Drawable const& d, const int& arg1, const double& arg2 )
{
  d.draw();
  d.two(arg1,arg2);
}

//void fcp(Drawable_plus<int,double> const& d)
void fcp(Drawable_plus const& d)
{
  d.draw(); // calls the const version
}


///////////////////////////////////////////////////////////////////////////////////////

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
  Serial << "Dyno examples" << endl;
  Serial.println("--------");
  Square<int,double> s;
  Circle c;
  fc(s); // prints Square (c)
  fc(c); // prints Circle (c)
  fcp(s); // prints Square (c)
  fcp(c); // prints Circle (c)
  Serial << endl;
  fca(s,2);
  fca(c,3);
  fcb(s,4,4.5);
  Serial << endl;
  Serial.println("--------");
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
