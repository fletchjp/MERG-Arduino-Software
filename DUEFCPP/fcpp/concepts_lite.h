// concepts_lite.h

// New file to provide a model for concepts-lite examples with FC++

// Some code from here:
// http://wiki.hsr.ch/PeterSommerlad/ConceptsLiteExamples

#ifndef FCPP_CONCEPTS_LITE_DOT_H
#define FCPP_CONCEPTS_LITE_DOT_H

#ifdef __cpp_concepts
//#define FCPP_CONCEPTS_LITE

#include <boost/predef.h>

#if BOOST_COMP_GNUC >= BOOST_VERSION_NUMBER(6,0,0)
#define FCPP_GNUC_CONCEPTS_LITE
// Currently only defined for GNUC as CLANG does not work.
#define FCPP_CONCEPTS_LITE
#else
// At the present time what is available in Clang 3.8 seems
// to be extremely limited.
// October 2015 I have checked again and there seems to be no progress.
#define FCPP_CLANG_CONCEPTS_LITE
#endif
namespace concepts_lite {

#ifdef FCPP_GNUC_CONCEPTS_LITE

template <typename LTC>
concept bool LessThanComparable() {
	return requires(LTC a, LTC b) {
		{ a < b } -> bool;
	};
}

// This is from here:
// https://groups.google.com/a/isocpp.org/forum/#!topic/concepts/OOA4F_Dzq6o
template <typename T, typename U>
concept bool Convertible()
{
   return requires(T t) {
       {t} -> U;
   };
}
// I like the look of this from
// https://akrzemi1.wordpress.com/2014/06/26/clever-overloading/
/*template <typename T>
class optional
{
  T t;
 public:
 optional() : t(0) {}
 optional(T t_) : t(t_) {}
 
 T value() const { return t; }

template <typename U>
requires Convertible<U, T>()
T value_or(U const& v) const
{
  if (this)
    return this->value();
  else
    return v;
}
 

template <typename F>
T value_or(F const& f) const
{
  if (this)
    return this->value();
  else
    return f();
}

}; // class optional
*/

 // This is from N4377 p.7 but will not compile without the extra typename

template<typename T>
concept bool R() {
return requires (T i) {
typename T::type;
{*i} -> const typename T::type&;
};
}
#endif

} // namespace concepts_lite

namespace fcpp {

  // This is the first attempt to replace the concepts I defined
  // for the old concepts in concepts.h

struct Nothing {};

// I invented these ConstCallableN in imitation of the old concepts.
// I am not using them at all at the moment.
#ifdef FCPP_GNUC_CONCEPTS_LITE

template<typename F>
 concept bool ConstCallable0() {
    return requires (const F &f) {
      { f() } -> typename F::result_type;
    };
}

template <typename F, typename X>
  concept bool ConstCallable1() {
    return requires (const F& f, const X& x) {
      { f(x) } -> typename F::result_type;
    };
}

 template <typename F, typename X, typename Y>
  concept bool ConstCallable2() {
   return requires (const F& f, const X& x, const Y& y) {
     { f(x,y) } -> typename F::result_type;
    };
}

 template <typename F, typename X, typename Y, typename Z>
  concept bool ConstCallable3() {
   return requires (const F& f, const X& x, const Y& y, const Z& z) {
     { f(x,y,z) } -> typename F::result_type;
    };
}

#endif
#ifdef FCPP_CONCEPTS_LITE

 // This is the only concept in active use for FC++.
 // Detection of AutoCurryType defined in curry.h
   template <typename T>
   bool concept is_auto_curry() {
      return requires (T a) {
	  a.auto_curry();
      };
   }
#endif


} // namespace fcpp


#endif


#endif
