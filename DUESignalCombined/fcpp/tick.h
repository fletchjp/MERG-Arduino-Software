 //
 // Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
 //
 // Permission to use, copy, modify, distribute and sell this software
 // and its documentation for any purpose is granted without fee,
 // provided that the above copyright notice and this permission notice
 // appear in all source code copies and supporting documentation. The
 // software is provided "as is" without any express or implied
 // warranty.

#ifndef FCPP_TICK_DOT_H
#define FCPP_TICK_DOT_H

 //////////////////////////////////////////////////////////////////////
 // This provides a subset of the Tick library for use with FC++.
 // This must have at least C++11.
 // This will be in a namespace when I have worked out what to use.
 //////////////////////////////////////////////////////////////////////

// Tick headers
#include <tick/builder.h>     // For TICK_TRAIT

#ifdef FCPP_TICK_XXX
// This is an alternate location of the definition of is_auto_curry.
// This was tried out during development and is not now used.
///////////////////////////////////////////////////////////////////////
// FC++ Tick Traits
///////////////////////////////////////////////////////////////////////
// This is the only concept in active use for FC++.
// Detection of AutoCurryType defined in curry.h
TICK_TRAIT(is_auto_curry)
{
   template <class T>
     //bool concept is_auto_curry() {
     auto require (T&& a) -> valid<
     decltype(a.auto_curry())
     >;
};
///////////////////////////////////////////////////////////////////////
#endif

#include <tick/requires.h>    // For TICK_REQUIRES
#include <tick/tag.h>         // For tick::tag
#include <tick/trait_check.h> // For TICK_TRAIT_CHECK
//#include <tick/traits/is_pod.h> // For tick::is_pod

// These do not get defined as part of the main headers.
#ifndef FCPP_ARDUINO
#define TICK_TEST_CHECK(...) if (!(__VA_ARGS__)) std::cout << "*****FAILED: " << #__VA_ARGS__ << "@" << __FILE__ << ": " << __LINE__ << std::endl
#endif
// Renamed to start with TICK_
#define TICK_TRAIT_CHECK_SUCCESS(...) \
static_assert(std::is_same<typename tick::detail::trait_check_impl<__VA_ARGS__>::type, tick::TRAIT_CHECK_FAILURE<>>::value, "Trait check failure")

// Renamed to start with TICK_
#define TICK_TRAIT_CHECK_FAIL(...) \
static_assert(not std::is_same<typename tick::detail::trait_check_impl<__VA_ARGS__>::type, tick::TRAIT_CHECK_FAILURE<>>::value, "Trait check failure")


#ifdef FCPP_TICK
///////////////////////////////////////////////////////////////////////
// FC++ Tick Traits
///////////////////////////////////////////////////////////////////////
// This is the only concept in active use for FC++.
// Detection of AutoCurryType defined in curry.h
TICK_TRAIT(is_auto_curry)
{
   template <class T>
     //bool concept is_auto_curry() {
     auto require (T&& a) -> valid<
     decltype(a.auto_curry())
     >;
};
///////////////////////////////////////////////////////////////////////
#endif

// These are part of the development and not used in FC++.
// is_integral was added in tick_test and fails for the iterators.
TICK_TRAIT(is_incrementable /*, std::is_integral<_> */)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(x++),
        decltype(++x)
    >;
};

TICK_TRAIT(is_decrementable, is_incrementable<_>)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(x--),
        decltype(--x)
    >;
};

TICK_TRAIT(is_advanceable, is_decrementable<_>)
{
  template<class T, class Number>
    auto require(T&& x, Number n) -> valid<
        decltype(x += n)
    >;
};

TICK_TRAIT(is_numeric, is_incrementable<_>, is_decrementable<_>, is_advanceable<_, int>)
{
};

template<class Iterator>
void advance_impl(Iterator& it, int n, tick::tag<is_advanceable>)
{
  it += n;
}

template<class Iterator>
void advance_impl(Iterator& it, int n, tick::tag<is_decrementable>)
{
  if (n > 0) while (n--) ++it;
  else
    {
      n *= -1;
      while (n--) --it;
    }
}

template<class Iterator>
void advance_impl(Iterator& it, int n, tick::tag<is_incrementable>)
{
  while (n--) ++it;
}

template<class Iterator>
void advance(Iterator& it, int n)
{
  advance_impl(it,n,tick::most_refined<is_advanceable<Iterator> >());
}

template<class Iterator>
void advance_numeric(Iterator& it, int n)
{
    advance_impl(it, n, tick::most_refined<is_numeric<Iterator>>());
}

template<class T, TICK_REQUIRES(is_incrementable<T>())>
void increment(T& x)
{
    x++;
}

template<class T, TICK_REQUIRES(is_decrementable<T>())>
void decrement(T& x)
{
    x--;
}

///////////////////////////////////////////////////////////
// New traits from the pdf slides. (see tick_test4.cpp)
///////////////////////////////////////////////////////////

TICK_TRAIT(is_streamable)
{
  template<class Stream, class T>
  auto require(Stream&& s, T&& x) -> valid<
     decltype(s << x)
  >;
};

TICK_TRAIT(is_iterator,
std::is_copy_constructible<_>,
std::is_copy_assignable<_>,
std::is_destructible<_>)
{
  template<class T>
  auto require(T x) -> valid<
    decltype(*x),
    decltype(returns<T&>(++x))
  >;
};

// Here Pflutz defines adl:
//http://pfultz2.com/blog/2014/12/12/compare-overloading-2/

namespace adl {

using std::begin;
using std::end;

template<class R>
auto adl_begin(R&& r) -> decltype(begin(r));

template<class R>
auto adl_end(R&& r) -> decltype(end(r));
}

TICK_TRAIT(is_range)
{
  template<class T>
  auto require(T&& x) -> valid<
    decltype(returns<is_iterator<_>>(adl::adl_begin(x))),
    decltype(returns<is_iterator<_>>(adl::adl_end(x)))
  >;
};


#endif
