//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_SMART_DOT_H
#define FCPP_SMART_DOT_H

#include "signature.h"

namespace fcpp {

//////////////////////////////////////////////////////////////////////
// Feature: Smartness
//////////////////////////////////////////////////////////////////////
// If F is smart, then we can refer to these entities:
//
//    FunctoidTraits<F>::template accepts<N>::args
//       A bool which says whether F can accept N arguments
//
//    FunctoidTraits<F>::max_args
//       An int which says what the most arguments F can accept is
//
//    FunctoidTraits<F>::template ensure_accepts<N>::args()
//       A no-op call that compiles only if F can accept N args
//
//    FunctoidTraits<F>::template has_sig
//       A bool which says that a smart object has a sig.
//       Designed to sort out cases where it is looking
//       for a sig which is not there, which is happening.
//
// We use traits so that if you happen to ask a non-smart functoid these
// questions, you will hopefully get a literate error message.

// I have moved RT to rt.h because it wass declared ahead of this
// in signature.h. It is now declared afterwards.

// Usage: Insert the following in code to check that the template type
// T has this trait. Compiles silently if it does.
// typedef typename SmartHasSig<FunctoidTraits<T>::has_sig>::Result Has_Sig;
// Still under evaluation.
// I think this is a test for smartness rather than sig itself.
// Some things have a Sig without being defined through Full something.
// So this is throwing up false reports rather than helping to find
// the ones which don't actually have a Sig.

// I now have a better test for the Sig using Boost TTI.

template <bool has_sig> struct SmartHasSig;

template <>
struct SmartHasSig<true>
{
  typedef bool Result;
};

struct SmartFunctoid { };

// We add crazy identifiers to ensure that users don't accidentally talk
// to functoids directly; they should always be going through the traits
// class to ask for info.
struct SmartFunctoid0 : public SmartFunctoid {
   template <class Dummy, int i> struct crazy_accepts {
      static const bool args = false;
   };
   template <class Dummy> struct crazy_accepts<Dummy,0> {
      static const bool args = true;
   };
   static const int crazy_max_args = 0;

   static const bool crazy_has_sig = true;
};

struct SmartFunctoid1 : public SmartFunctoid {
   template <class Dummy, int i> struct crazy_accepts {
      static const bool args = false;
   };
   template <class Dummy> struct crazy_accepts<Dummy,1> {
      static const bool args = true;
   };
   static const int crazy_max_args = 1;

   static const bool crazy_has_sig = true;
};

struct SmartFunctoid2 : public SmartFunctoid {
   template <class Dummy, int i> struct crazy_accepts {
      static const bool args = false;
   };
   template <class Dummy> struct crazy_accepts<Dummy,1> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,2> {
      static const bool args = true;
   };
   static const int crazy_max_args = 2;

   static const bool crazy_has_sig = true;
};

struct SmartFunctoid3 : public SmartFunctoid {
   template <class Dummy, int i> struct crazy_accepts {
      static const bool args = false;
   };
   template <class Dummy> struct crazy_accepts<Dummy,1> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,2> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,3> {
      static const bool args = true;
   };
   static const int crazy_max_args = 3;

   static const bool crazy_has_sig = true;
};

// FC++.1.5.2
#ifdef FCPP152
struct SmartFunctoid4 : public SmartFunctoid {
   template <class Dummy, int i> struct crazy_accepts {
      static const bool args = false;
   };
   template <class Dummy> struct crazy_accepts<Dummy,1> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,2> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,3> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,4> {
      static const bool args = true;
   };
   static const int crazy_max_args = 4;

   static const bool crazy_has_sig = true;
};

struct SmartFunctoid5 : public SmartFunctoid {
   template <class Dummy, int i> struct crazy_accepts {
      static const bool args = false;
   };
   template <class Dummy> struct crazy_accepts<Dummy,1> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,2> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,3> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,4> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,5> {
      static const bool args = true;
   };
   static const int crazy_max_args = 5;

   static const bool crazy_has_sig = true;
};

struct SmartFunctoid6 : public SmartFunctoid {
   template <class Dummy, int i> struct crazy_accepts {
      static const bool args = false;
   };
   template <class Dummy> struct crazy_accepts<Dummy,1> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,2> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,3> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,4> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,5> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,6> {
      static const bool args = true;
   };
   static const int crazy_max_args = 6;

   static const bool crazy_has_sig = true;
};

struct SmartFunctoid7 : public SmartFunctoid {
   template <class Dummy, int i> struct crazy_accepts {
      static const bool args = false;
   };
   template <class Dummy> struct crazy_accepts<Dummy,1> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,2> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,3> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,4> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,5> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,6> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,7> {
      static const bool args = true;
   };
   static const int crazy_max_args = 7;

   static const bool crazy_has_sig = true;
};

struct SmartFunctoid8 : public SmartFunctoid {
   template <class Dummy, int i> struct crazy_accepts {
      static const bool args = false;
   };
   template <class Dummy> struct crazy_accepts<Dummy,1> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,2> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,3> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,4> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,5> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,6> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,7> {
      static const bool args = true;
   };
   template <class Dummy> struct crazy_accepts<Dummy,8> {
      static const bool args = true;
   };
   static const int crazy_max_args = 8;

   static const bool crazy_has_sig = true;
};

#endif

namespace impl {
   template <class F, bool b> struct NeededASmartFunctoidButInsteadGot {};
   template <class F> struct NeededASmartFunctoidButInsteadGot<F,true> {
      typedef F Type;
   };
   template <bool b> struct Ensure;
   template <> struct Ensure<true> {};
} // end namespace impl

// I have created a separate struct for the variadic traits
// used only with variadic FC++.
template <class MaybeASmartFunctoid>
struct VariadicTraits {
#ifdef FCPP_USE_BOOST_TTI_VARIADICS
  // Compile time
  BOOST_MPL_ASSERT((has_variadic_Sig<MaybeASmartFunctoid>));
  // Run time
  static const bool variadic_Sig() {
    return has_variadic_Sig<MaybeASmartFunctoid>::value;
  }
#endif
};

template <class MaybeASmartFunctoid>
struct FunctoidTraits {
  // Experiment with BOOST_TTI
#ifdef FCPP_USE_BOOST_TTI
  // Compile time
  // This now works after change in lambda.h for a two parameter call in FullN.
  // lambda.h : Call to SmartHasSig taken out Tue Aug 27 14:29:42 2013 +0100
  // fcpp/lambda.h  Take out direct test using SmartHasSig for 2 parameter case.
  // This test was not done elsewhere in lambda.h
  // This is because I put it in as an experiment in 2010!!
  BOOST_MPL_ASSERT((has_template_Sig<MaybeASmartFunctoid>));
  // Run time
  static const bool template_Sig() {
    return has_template_Sig<MaybeASmartFunctoid>::value;
  }
#endif
   typedef
      typename impl::NeededASmartFunctoidButInsteadGot<MaybeASmartFunctoid,
         Inherits<MaybeASmartFunctoid,SmartFunctoid>::value>::Type
      F;
   template <int i> struct accepts {
     /* These will not compile with CUDA nvcc */
#ifndef FCPP_CUDA_COMPATIBLE
     static const bool args = F::template crazy_accepts<int,i>::args;
#else
     typedef typename F::template crazy_accepts<int,i> something;
     static const bool args = something::args;
     //     static const bool args = false;
#endif
   };
   template <int i> struct ensure_accepts {
#ifndef FCPP_CUDA_COMPATIBLE
     static const bool ok = F::template crazy_accepts<int,i>::args;
#else
     typedef typename F::template crazy_accepts<int,i> something;
     static const bool ok = something::args;
     //     static const bool ok = false;
     // inline static void args() {  }
#endif
     inline static void args() { (void) impl::Ensure<ok>(); }
   };
   static const int max_args = F::crazy_max_args;

   static const bool has_sig = F::crazy_has_sig;
};

// This can be used for run time.
// Retain the old version for template arguments.
template <class F>
/* const */ int FunctoidTraits_max_args()
    {
      return FunctoidTraits<F>::max_args;
    }

} // end namespace fcpp



#endif

#include "rt.h"
