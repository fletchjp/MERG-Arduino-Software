//===========================================================
// variadic_fcpp.h
//===========================================================
//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
// Copyright (c) 2007-2016 John Fletcher
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

//===========================================================
// May/June 2016
//
// Nearly all of the many options for FullN are now implemented with
// concepts lite and FCPP_TICK as alternatives governed by compile time
// definitions of FCPP_CONCEPTS_LITE or FCPP_TICK. This has not changed
// what the code does and mainly shows the relative ways of implementing
// the checks for autocurrying. This can serve as a demonstration of the
// use of both.
//
// Now that I have implemented FCPP_TICK as an alternative to concepts lite
// in full.h I wondered if I it could be used here as well.
// The answer to that is positive. It is available with gcc and clang.
//
// April 2016 I am wondering if the work in concepts_lite can also be
// applied in VFC++. This is only currently available with gcc 6.0.1 and
// C++17.
//
// August 2017 I now also have gcc 6.4 and 7.1 available.
//===========================================================

// I am now implementing concepts lite for the autocurrying of FullN.

// March 2016 Revisiting as part of the work on Applicative Functors.
// Now the arguments can be functoids, which changes things.
// I have done more work on composeNN and RT to get the argument
// types passed correctly. I am also adding more debug information.
//
// I have had to change quite a lot things as a result of my decision to
// add argument types into RT. This now supports up to 4 argument types
// and defines surplus ones to Void so that the code has something to find.
// In currying I have had to change to Sig instead of RT as wrong things
// were being picked up for FCPP_CXX11 cases.

// It is all now working for the examples including ThunkFuncToFuncN which
// needs to use the FC++ version of RT for return types.

// July 2013 Change _A_ to _b_ see lambda.h
// Note: It was entirely comments which needed to be changed.

// May 2012
// I have begun to remove use of promotion when working with C++11 in favour
// of using decltype and auto.
// This is now complete in PlusN. See examples in test_fcpp11.cpp.
// The file variadic_fcpp.h now does not use promotion when FCPP_CXX11
// is defined.

// This now has multipliesN (up to 6 arguments) modelled on plusN.

// March 2012
// This is a modified version running with Clang 3.1 (prerelease) and libc++ v1.

// Oct/Nov 2009
// Starting with gcc 4.4.0 and -std=c++0x
// - one bug found in struct PlusNHelper<0,Types...>
// at about line 2534 (line no may change) - added return 0.

// I have got back into this code and started to add new things.
// Part of the motivation is to get rid of some limits by using template
// template ideas. I am making very limited progress with that.
// I am also adding some more cases to the currying, using PlusN
// for the examples. This throws up some interesting cases where there are
// a number of arguments postponed and the first subsequent call does not
// supply all that is needed for the gap.
// The simplest one of these is plusN(a,_b,_c,d)(b)(c) where the call with (b)
// does not supply an argument for _c and so a further functoid is returned.
// I have found out how to fix this as a specialisation within binder1and4ofN
// to call binder1and2and4ofN which had to be built for this.

// I have now added a lot of things to implement some of FC++ for variadic use.
// I started with the things which use the FC++ trait for max_args to choose
// helpers and have implemented equivalents for VFC++.

// These have been done:

// beforeN
// afterN
// composeN
// composeNN
// ignoreN
// duplicateN

// These all work - see examples in fcppideas9.cpp

// I have also attempted bLiftN for monads but have not got that to work.
// I am getting close on this one, using FCPP_LAMBDA_AUTOCURRY which I had
// worked out before. I am missing something on the Sig chain with plusN,
// but things work with fcpp::plus.
// I now have sorted something out.
// What is needed is specialisations in FullN to pick up
// FC++ autocurrying. A few cases will be needed.
// The implementation of this is to merge the autocurrying types of FC++
// and VFC++. Autocurrying is needed at two levels to sort out the problem
// with the operator,
// firstly in the user code and secondly in coding XBlifterN.

// There are a few new binders required, or workarounds using more than one.

// This file is now even bigger than when I first thought of splitting it.

// One area which needs more work is FunN which is incomplete by comparison
// with FC++ and also has some problems.

//===========================================================
// December 2008 work restarted,
// in the hope of joining up with variadic fusion
// to make a serious example of what can be done with this.

// I have found some problems with the previous code -
// see fcppideas9.cpp for discussion of this.

// I have successfully added bindXofN following the methods
// I used in FC++ 1.5.2.

// I have now added some new functions and operators.
// __ is renamed as _abc and __ is deprecated and will be removed soon.
// _a, _b, _c are added as equivalents of FC++ _.
// I have added binders 1and3ofN and 2and3ofN to deal with some simple
// cases. I have removed the option of the unknown gap at the start.

// NOTE: plusN(_a,b,c)(a) works and also plusN(_a,b,c)(a,_abc)(d).
// i.e. although plusN(_a,b,c) does not have a trailing _abc it is not closed,
// and so the subsequent call can have _abc. Thus the rule for closure
// is not to have any unresolved parameter, either a trailing _abc or any
// in the sequence.

// More cases need to be implemented of e.g. plusN(_a,b), plusN(_a,_b,c) etc.
// These can be done easily using binders available already.

// I have now also done the tricky case plusN(_a,_b,c)(a)(b) where the second
// list is not long enough to fill the gap. This involves a special handler
// for binderAofN. The same code works for binderXofN and FullN.

// This could be extended to further cases, which would involve some
// more work on binders.

// I am now allowing for plusN(_a,_b,c)(a,_abc)(b) on the grounds that it is
// not needed as the case is still not closed by (a), so more must be to come.

// plusN(a,_b,_c,d)(b)(c) now works as well - see binder1and4ofN for details.

// I am currently (2016) refactoring the code.
// I had not recently read my own notes, so I attempted to
// make PlusN(a,_b,c,_abc) not realising that the _abc was not needed.
// This case now exists.

// This file is getting big so it could be an idea to break it up.

// The example uses e.g. plusN could be another file.

//===========================================================

// April 2007 with some notes on later changes.

// This has been substantially extended as follows:

// bind1ofN, bind1and2ofN, bind1and2and3ofN functoids
// e.g  bind1ofN(plusN,1)(2)  bind1and2ofN(plusN,1,2)(3)

// ===================================================================
// NOTE: I need to find an alternative symbol for __ which is reserved
//       for the systems people.
//       Now renamed as _abc and references changed below.
// ===================================================================

// currying notation plusN(1,_abc)(2) introduced to allow
// for more arguments to follow.
// plusN(1,2,_abc)(3) and plusN(1,2,3,_abc)(4) are also supported.

// bindMofN functoid for up to 4 bound arguments, now corrected.

// This can be changed again when the compiler bug is sorted.

// currying notation plusN(__,1)(2) introduced to allow
// for up to four arguments to follow.
// NOTE: This is seriously flawed and has been removed

// bindAofN now added with Bind1, Bind2 and Bind3.

// This does not yet have a currying notation.
// It could be plusN(_,2) plusN(_,_,3) etc using FC++ placeholder.
// Now being implemented - see above.

// I could also have a version where the binder was in a tuple with the
// value in the call. This would make multiple arguments easier
// without making a long call. I think I can do this with an alternate
// constructor.
// (I have implemented this in FC++ 1.5.2 so it should be possible here.)
// Now implemented as bindXofN.

// revisions and corrections to FullN which previously did not
// give the correct return type.

// makeTupleN functoid.

// See examples in fcppideas8.cpp and fcppideas9.cpp

// This file also shows interworking with FC++ lambda and monads.


// FunN could do with a look at in the light of developments in FullN.

//===========================================================
// Taken from

// fcppideas2.cpp

// Renamed as substantially different from first attempts
// before 11/11/06.

// Variadic examples to develop FC++.
// FC++ is available but not used in the new code, except for IRef.

// This now contains variadic versions of FunType, CFunType
// and RT from signature.h

// This file now runs an example of plusN using FullN.

// As far as I know this all works for 4 argument versions.

// This code is heavily modified in the quest for solution
// of the problems with a function with no arguments.

// Current problems.
// nullfunN.  I cannot get this to work. I think I need to
// do code for getting a pointer to a member function.
// At the moment I have not done this, which is logically
// part of ptr_to_funN which I have not done at the moment.
// This will also be linked to currying, not yet started
// apart from some helper functions.

// Note: This is a JPF addition to FC++ to enable Fun objects
// to be put in a map, which requires a default constructor,
// which in turn requires a representation object.

// Some parts of FunN = converter functions - are not implemented.

// Compare with the previous code.

// It looks as though my historic fudge is not needed any more.
// For a long time gcc did not put the tr1 namespace in the right place.
// See old discussion with Doug Gregor.
// see tr1_namespace.hpp which I have left unused.
/*#ifdef __VARIADIC_TEMPLATES
#define VARIADIC
#define TR1 std
#endif

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#define VARIADIC
#define TR1 std
#endif
*/
#define FCPP_VARIADIC
#define TR1 std
#ifndef FCPP_ARDUINO
#include <iostream>
#endif
#include <stdexcept>
#ifdef FCPP_VARIADIC
#include <functional>
#include <tuple>
#endif

// Code from ~/BoostFC++/fcpp/fcpp_clients/lazy_prelude.hpp
// to allow the option of using MP11.
// Modified to use FCPP_ defines.
// This requires at least C++11
#ifdef FCPP_USE_MP11
#include "boost/mp11.hpp"
#include <boost/core/demangle.hpp>
#ifndef FCPP_USE_MP11_TRUE_CXX11
#include <variant>
#else
#include <boost/variant.hpp>
#endif
#endif

//#ifdef __GXX_CONCEPTS__
#ifdef FCPP_CONCEPTS
#include <concepts>


auto concept Callable<typename F, typename... Args> {
typename result_type;
result_type operator()(F&, Args...);
}

#endif

// Needed for promotion in plus etc.
// Now included in prelude if asked for.
// Promotion is not now used with C++11
//#include "promote.h"
#ifndef FCPP_CXX11
#define FCPP_USE_PROMOTE
#endif

// FC++ includes for version 1.5
// This was including the wrong version of prelude.
#include "prelude.h"
// Use FC++ debug traits.
#ifdef FCPP_DEBUG
#include "debug_traits.h"
#endif

#ifndef FCPP_CXX11
// Some useful standard promotions.
MK_PROMOTION(size_t, int, int)
MK_PROMOTION(size_t,double,double)
MK_PROMOTION(int,double,double)

#include "promoteN.hpp"
#endif

// Definitions which use variadic templates.
#ifdef FCPP_VARIADIC

#ifndef FCPP_VARIADIC_FCPP_H
#define FCPP_VARIADIC_FCPP_H

// Attempt to define structures to unpack the types in a package.
// Idea being to enable FunType to be more general.
// Specialisations are needed for each size, so the gain is not that great.

using namespace fcpp; // put in for debug_traits

namespace variadic_fcpp {

namespace experiment {

  // Here is a new way of doing this without having to specialise
  // each size. This is passed a sequence e.g. mpl vector.
  template <typename Sequence> struct FindLastSeq;

  template <template < typename...> class Sequence,
            typename Head,
            typename... Tail>
  struct FindLastSeq< Sequence <Head, Tail...> >
  {
    typedef typename FindLastSeq<Sequence<Tail...> >::Last Last;
  };

  template <template < typename...> class Sequence,
            typename Head>
  struct FindLastSeq< Sequence <Head> >
  {
    typedef Head Last;
  };

}

  template <typename...> struct FindLast;

  template < typename Head,
             typename... Tail>
  struct FindLast< Head, Tail...>
  {
    typedef typename FindLast<Tail...>::Last Last;
  };

  template < typename Head >
  struct FindLast< Head >
  {
    typedef Head Last;
  };

  namespace the_old_way {

template <int N,typename... Types> struct FindLast;

template <typename L>
struct FindLast<1,L>
{
  typedef L Last;
};

template <typename A1,typename L>
struct FindLast<2,A1,L>
{
  typedef L Last;
};

template <typename A1,typename A2,typename L>
struct FindLast<3,A1,A2,L>
{
  typedef L Last;
};

template <typename A1,typename A2,typename A3,typename L>
struct FindLast<4,A1,A2,A3,L>
{
  typedef L Last;
};

template <typename A1,typename A2,typename A3,typename A4,typename L>
struct FindLast<5,A1,A2,A3,A4,L>
{
  typedef L Last;
};

template <typename A1,typename A2,typename A3,typename A4,typename A5,typename L>
  struct FindLast<6,A1,A2,A3,A4,A5,L>
{
  typedef L Last;
};

template <typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename L>
  struct FindLast<7,A1,A2,A3,A4,A5,A6,L>
{
  typedef L Last;
};

template <typename A1,typename A2,typename A3,typename A4,
          typename A5,typename A6,typename A7,typename L>
  struct FindLast<8,A1,A2,A3,A4,A5,A6,A7,L>
{
  typedef L Last;
};

  }

template <int N,int I,typename... Types> struct FindIth;

template <typename L>
struct FindIth<1,1,L>
{
  typedef L First;
};

template <typename A, typename L>
struct FindIth<2,1,A,L>
{
  typedef A First;
};

template <typename A, typename L>
struct FindIth<2,2,A,L>
{
  typedef L Second;
};

struct Void { };

template <int N,typename... Types> struct FindAll;

template <>
struct FindAll<0>
{
  typedef Void First;
  typedef Void Second;
  typedef Void Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A>
struct FindAll<1,A>
{
  typedef A First;
  typedef Void Second;
  typedef Void Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B>
struct FindAll<2,A,B>
{
  typedef A First;
  typedef B Second;
  typedef Void Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C>
struct FindAll<3,A,B,C>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D>
struct FindAll<4,A,B,C,D>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef D Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D,typename E>
  struct FindAll<5,A,B,C,D,E>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef D Fourth;
  typedef E Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D,typename E,typename F>
  struct FindAll<6,A,B,C,D,E,F>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef D Fourth;
  typedef E Fifth;
  typedef F Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D,typename E,typename F,typename G>
  struct FindAll<7,A,B,C,D,E,F,G>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef D Fourth;
  typedef E Fifth;
  typedef F Sixth;
  typedef G Seventh;
};

template <int N,typename... Types> struct FindFirstHalf;
template <int N,typename... Types> struct FindSecondHalf;

template <typename A,typename B>
struct FindFirstHalf<2,A,B>
{
  typedef A First;
  typedef Void Second;
  typedef Void Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B>
struct FindSecondHalf<2,A,B>
{
  typedef B First;
  typedef Void Second;
  typedef Void Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D>
struct FindFirstHalf<4,A,B,C,D>
{
  typedef A First;
  typedef B Second;
  typedef Void Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D>
struct FindSecondHalf<4,A,B,C,D>
{
  typedef C First;
  typedef D Second;
  typedef Void Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D,typename E,typename F>
struct FindFirstHalf<6,A,B,C,D,E,F>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D,typename E,typename F>
struct FindSecondHalf<6,A,B,C,D,E,F>
{
  typedef D First;
  typedef E Second;
  typedef F Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D,
          typename E,typename F,typename G,typename H>
struct FindFirstHalf<8,A,B,C,D,E,F,G,H>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef D Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D,
          typename E,typename F,typename G,typename H>
struct FindSecondHalf<8,A,B,C,D,E,F,G,H>
{
  typedef E First;
  typedef F Second;
  typedef G Third;
  typedef H Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <int N,typename... Types> struct FindAllButLast;

template <typename L>
struct FindAllButLast<1,L>
{
  typedef Void First;
  typedef Void Second;
  typedef Void Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename L>
struct FindAllButLast<2,A,L>
{
  typedef A First;
  typedef Void Second;
  typedef Void Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename L>
struct FindAllButLast<3,A,B,L>
{
  typedef A First;
  typedef B Second;
  typedef Void Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename L>
struct FindAllButLast<4,A,B,C,L>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef Void Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D,typename L>
struct FindAllButLast<5,A,B,C,D,L>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef D Fourth;
  typedef Void Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D,typename E,typename L>
  struct FindAllButLast<6,A,B,C,D,E,L>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef D Fourth;
  typedef E Fifth;
  typedef Void Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D,typename E,typename F,typename L>
  struct FindAllButLast<7,A,B,C,D,E,F,L>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef D Fourth;
  typedef E Fifth;
  typedef F Sixth;
  typedef Void Seventh;
};

template <typename A,typename B,typename C,typename D,
          typename E,typename F, typename G,typename L>
  struct FindAllButLast<8,A,B,C,D,E,F,G,L>
{
  typedef A First;
  typedef B Second;
  typedef C Third;
  typedef D Fourth;
  typedef E Fifth;
  typedef F Sixth;
  typedef G Seventh;
};

// These are applications to the FC++ FunType and
// CFunType which are much simpler.

// New one with args only
template <typename... Types>
struct FunArgs
{
  //typedef typename FindLast<sizeof...(Types),Types...>::Last  ResultType;
  typedef typename FindAll<sizeof...(Types),Types...>::First  Arg1Type;
  typedef typename FindAll<sizeof...(Types),Types...>::Second Arg2Type;
  typedef typename FindAll<sizeof...(Types),Types...>::Third  Arg3Type;
  typedef typename FindAll<sizeof...(Types),Types...>::Fourth Arg4Type;
  typedef typename FindAll<sizeof...(Types),Types...>::Fifth Arg5Type;
  typedef typename FindAll<sizeof...(Types),Types...>::Sixth Arg6Type;
  typedef typename FindAll<sizeof...(Types),Types...>::Seventh Arg7Type;
  // Add more as needed and define more specialisations above...
};

// This is an attempt to deal with the repetition of Arg types in SigN in
// different functoids. This will also make it easier to extend to cover
// more arguments.
template <int N, typename... XYZ>
struct ArgN;

template <class Dummy>
struct ArgN<0,Dummy>
{
   typedef Void Arg1Type;
   typedef Void Arg2Type;
   typedef Void Arg3Type;
   typedef Void Arg4Type;
   typedef Void Arg5Type;
   typedef Void Arg6Type;
   typedef Void Arg7Type;
};

template <typename A>
struct ArgN<1,A>
{
   typedef A Arg1Type;
   typedef Void Arg2Type;
   typedef Void Arg3Type;
   typedef Void Arg4Type;
   typedef Void Arg5Type;
   typedef Void Arg6Type;
   typedef Void Arg7Type;
};

template <typename A, typename B>
struct ArgN<2,A,B>
{
   typedef A Arg1Type;
   typedef B Arg2Type;
   typedef Void Arg3Type;
   typedef Void Arg4Type;
   typedef Void Arg5Type;
   typedef Void Arg6Type;
   typedef Void Arg7Type;
};

template <typename A, typename B, typename C>
struct ArgN<3,A,B,C>
{
   typedef A Arg1Type;
   typedef B Arg2Type;
   typedef C Arg3Type;
   typedef Void Arg4Type;
   typedef Void Arg5Type;
   typedef Void Arg6Type;
   typedef Void Arg7Type;
};

template <typename A, typename B, typename C, typename D>
struct ArgN<4,A,B,C,D>
{
   typedef A Arg1Type;
   typedef B Arg2Type;
   typedef C Arg3Type;
   typedef D Arg4Type;
   typedef Void Arg5Type;
   typedef Void Arg6Type;
   typedef Void Arg7Type;
};

template <typename A, typename B, typename C, typename D, typename E>
struct ArgN<5,A,B,C,D,E>
{
   typedef A Arg1Type;
   typedef B Arg2Type;
   typedef C Arg3Type;
   typedef D Arg4Type;
   typedef E Arg5Type;
   typedef Void Arg6Type;
   typedef Void Arg7Type;
};

template <typename A, typename B, typename C, typename D, typename E,
          typename FF>
struct ArgN<6,A,B,C,D,E,FF>
{
   typedef A Arg1Type;
   typedef B Arg2Type;
   typedef C Arg3Type;
   typedef D Arg4Type;
   typedef E Arg5Type;
   typedef FF Arg6Type;
   typedef Void Arg7Type;
};

template <typename A, typename B, typename C, typename D, typename E,
          typename FF, typename G>
struct ArgN<7,A,B,C,D,E,FF,G>
{
   typedef A Arg1Type;
   typedef B Arg2Type;
   typedef C Arg3Type;
   typedef D Arg4Type;
   typedef E Arg5Type;
   typedef FF Arg6Type;
   typedef G Arg7Type;
};


// This is the way in which FunType is implemented in the boost FC++
// without the argument types.
// This came later, but I did not appreciate the significance
// when I put the extensions into FC++ 1.5 so I put all the arguments
// back in.  That was a mistake which is now causing problems.
// Now that I have begun to achieve interoperability between
// FC++.1.5 (FCCP152) and this variadic FC++ it would be nice to change
// both over to use this and not have a lot of problems with the return
// type and the extra (and not used) argument types.

// If I start to use it here I will loose the interoperability.
// For the moment I have put this here under its boost name

// With a bit more thinking I have sorted out the problem with Sig in FullN
// which was the cause of the problem (lack of the return type).
// That is now resolved so the short term need to implement this has gone.
#ifdef FCPP_BOOST_TYPE
template <class R>
struct fun_type {
   typedef R result_type;
};
#endif

// Mostly this is used to pass the result type around and the argument types
// are not much used. They could be taken out entirely (following boost FC++).
// I had not bothered much about this as I was concentrating on concepts in FC++.
// Variadic concepts have not happened and now concepts are not in C++0x.
// So thinking about the best way to do this can come back on the agenda.
template <typename... Types>
struct FunType
{
  // Work on variadic MPL (see elsewhere) has lead to better ways
  // to define things such as FindLast without a fixed limit
  // or lots of specialisation.
  //typedef typename FindLast<sizeof...(Types),Types...>::Last  ResultType;
  typedef typename FindLast<Types...>::Last  ResultType;
  typedef typename FindAllButLast<sizeof...(Types),Types...>::First  Arg1Type;
  typedef typename FindAllButLast<sizeof...(Types),Types...>::Second Arg2Type;
  typedef typename FindAllButLast<sizeof...(Types),Types...>::Third  Arg3Type;
  typedef typename FindAllButLast<sizeof...(Types),Types...>::Fourth Arg4Type;
  typedef typename FindAllButLast<sizeof...(Types),Types...>::Fifth Arg5Type;
  typedef typename FindAllButLast<sizeof...(Types),Types...>::Sixth Arg6Type;
  typedef typename FindAllButLast<sizeof...(Types),Types...>::Seventh Arg7Type;
  // Add more as needed and define more specialisations above...
};

template <typename... Types>
struct CFunType : public FunType<Types...>
{
  struct Sig : public FunType<Types...> {};
};

template <int i,typename T,typename... Types> struct RTN;


// Needs to be implemented - see fcpp/smart.h
 struct SmartFunctoidN : public fcpp::SmartFunctoid  {

// October 2009 - reviewing this I think it could be used to report
// implementation limits.
// For the moment I am implementing this in namespace variadic_fcpp.
// It may be that I should move it to fcpp for compatibility and interworking.

// Further thoughts - for interoperability with FC++,
// this class should inherit from fcpp::SmartFunctoid and provide
// apropriate settings.

// I have done the inheritance and it doesn't seem to break anything.
// I have gone one further and set it as seven arguments maximum for now,
// which corresponds with some other aspects of the implementation.
// I have changed this to minus one, which allows e.g. beforeN and afterN
// which are implemented below to implement a variadic version of the
// corresponding FC++ functoids.

   static const int crazy_max_args = -1;

   // Added to resolve problem with Clang 3.1
   // Two parameter PlusN would not work.
   static const bool crazy_has_sig = true;


// I have been looking in FC++ to see where the traits are actually used.
// This turns out to be in things like before and after where the number
// of arguments of a functoid is important for the implementation.
// I have now sorted out a variadic implementation - see beforeN and afterN.

// The following is the comment from FC++ 1.5
// It is not obvious how to implement this in VFC++.
// The return values need to relate to the usage. I feel some
// helpers coming on...
//
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
// We use traits so that if you happen to ask a non-smart functoid these
// questions, you will hopefully get a literate error message.


};


//////////////////////////////////////////////////////////////////////
// Feature: Variadic support for RT
// This now uses RTN to support each number of arguments.
// This uses the smartness max args which has the value -1 for the
// variadic case which enables that to be handled separately.
// RT and RTN give argument types up to Arg4Type. More may be needed.
//
// I have defined RTNBase to make the RT code more readable.
// typedef RTN<fcpp::FunctoidTraits<T>::max_args,T,Types...> RTNBase;
// To do this I have had to add explict smartness to a number of
// the variadic structs and this may not be complete.
// Done previously:
//                     FullN, FullNR, FunN
// Now added:
//                    numerous binder1ofN etc.
//                    PtrToFunN,
//                    XBeforerN, BeforeN,
//                    XAftererMN,AfterN,
//                    XComposeN, XComposeN2, XComposeN3, XComposeN4
//                    ComposeN, ComposeN2, ComposeNN,
//                    XTFTFN, ThunkFunkToFunkN,
//                    XIgnorerN, IgnoreN,
//                    XDuplicaterN, DuplicateN,
//                    XBLifterN, BLiftN,
//                    MultipliesN, PlusN,
//                    MakeTupleN
//
// Also example codes in fcppideas9_clang.cpp
//////////////////////////////////////////////////////////////////////

// This is the variadic case. It is now used by the smart property.
template <typename T,typename... Types>
  struct RTN<-1,T,Types...> : public ArgN<sizeof... (Types),Types...> {
  typedef typename T::template Sig<Types...>::ResultType ResultType;
};

/* This is an attempt to sort out ptr_to_funN with no arguments.
template <typename T>
  struct RTN<-1,T> : public ArgN<0,T> {
  typedef typename T::template Sig<>::ResultType ResultType;
};
*/

template <typename T>
  struct RTN<0,T> : CFunType<T>
  {
    typedef Void Arg1Type;
    typedef Void Arg2Type;
    typedef Void Arg3Type;
    typedef Void Arg4Type;
};

template <typename T,typename A>
  struct RTN<1,T,A> : FunType<T,A>
  {
    typedef Void Arg2Type;
    typedef Void Arg3Type;
    typedef Void Arg4Type;
};

template <typename T,typename A,typename B>
  struct RTN<2,T,A,B> : FunType<T,A,B>
  {
    typedef Void Arg3Type;
    typedef Void Arg4Type;
};

template <typename T,typename A,typename B, typename C>
  struct RTN<3,T,A,B,C> : FunType<T,A,B,C>
  {
    typedef Void Arg4Type;
  };


template <typename T,typename A,typename B, typename C, typename D>
  struct RTN<4,T,A,B,C,D> : FunType<T,A,B,C,D>
  {
  };

template <typename T,typename A,typename B, typename C, typename D,
                     typename E>
  struct RTN<5,T,A,B,C,D,E> : FunType<T,A,B,C,D,E>
  {
  };

template <typename T,typename A,typename B, typename C, typename D,
                     typename E,typename F>
  struct RTN<6,T,A,B,C,D,E,F> : FunType<T,A,B,C,D,E,F>
  {
  };


template <typename F,typename... Types>
struct RT {
  typedef RTN<fcpp::FunctoidTraits<F>::max_args,F,Types...> RTNBase;
  typedef typename RTNBase::ResultType ResultType;
  typedef typename RTNBase::Arg1Type Arg1Type;
  typedef typename RTNBase::Arg2Type Arg2Type;
  typedef typename RTNBase::Arg3Type Arg3Type;
  typedef typename RTNBase::Arg4Type Arg4Type;
  //typedef typename RTNBase::Arg5Type Arg5Type;
  //typedef typename RTNBase::Arg6Type Arg6Type;
};

////////////////////////////////////////////////////////////////////////
// CurryableNHelper for FullN.
////////////////////////////////////////////////////////////////////////

template <int N,typename R,typename F,typename... Args>
struct CurryableNHelper;

template <typename R,typename F>
struct CurryableNHelper<0,R,F>
{
  static inline R go( const F& f) {
    //std::cout << "CurryableNHelper<0,R,F>::go called" << std::endl;
    return f();
  }
};

template <typename R,typename F,typename X>
struct CurryableNHelper<1,R,F,X>
{
  static inline R go( const F& f, const X& x) {
    //std::cout << "CurryableNHelper<1,R,F,X>::go called" << std::endl;
    return f(x);
  }
};

template <typename R,typename F,typename X,typename Y>
struct CurryableNHelper<2,R,F,X,Y>
{
  static inline R go( const F& f, const X& x, const Y& y) {
    //std::cout << "CurryableNHelper<2,R,F,X,Y>::go called" << std::endl;
    return f(x,y);
  }
};

template <typename R,typename F,typename X,typename Y,typename Z>
struct CurryableNHelper<3,R,F,X,Y,Z>
{
  static inline R go( const F& f, const X& x, const Y& y,const Z& z) {
    return f(x,y,z);
  }
};

template <typename R,typename F,typename X,typename Y,typename Z,typename W>
struct CurryableNHelper<4,R,F,X,Y,Z,W>
{
  static inline R go( const F& f, const X& x, const Y& y,const Z& z, const W& w) {
    return f(x,y,z,w);
  }
};

template <typename R,typename F,typename X,typename Y,typename Z,typename V,typename W>
  struct CurryableNHelper<5,R,F,X,Y,Z,V,W>
{
  static inline R go( const F& f, const X& x, const Y& y,const Z& z,
                                  const V& v, const W& w)
  {
    return f(x,y,z,v,w);
  }
};

template <typename R,typename F,typename X,typename Y,typename Z,
                     typename U,typename V,typename W>
  struct CurryableNHelper<6,R,F,X,Y,Z,U,V,W>
{
  static inline R go( const F& f, const X& x, const Y& y, const Z& z,
                                  const U& u, const V& v, const W& w)
  {
    return f(x,y,z,u,v,w);
  }
};

template <typename R,typename F,typename X,typename Y,typename Z,
                     typename T,typename U,typename V,typename W>
  struct CurryableNHelper<7,R,F,X,Y,Z,T,U,V,W>
{
  static inline R go( const F& f, const X& x, const Y& y, const Z& z,
		      const T& t, const U& u, const V& v, const W& w)
  {
    return f(x,y,z,t,u,v,w);
  }
};

////////////////////////////////////////////////////////////////////////
// Binders for 1 or 2 arguments declared ahead of FullN
// so that they can be used inside it.
// The corresponding functoids are defined after FullN.
////////////////////////////////////////////////////////////////////////

// This is used in FullN to indicate that there are more arguments to come.
// e.g. plusN(1,_abc)(2)
// A double underscore was used to avoid confusion
// with the FC++ use of one underscore.
// I have also implemented e.g. plusN(_abc,1)(2) but this is
// ambiguous as it is not clear how many arguments come before the one
// supplied. The user should know this so I am planning to make this
// e.g. plusN(_abc,bind_fourth,4)(1,2,3)
// The use of _abc except at the end is no longer supported.
// It is now replaced by the following, where _a, _b and _c are alternative names
// equivalent to the FC++ use of _ e.g. plus(_,2)(1).
// plusN(_a,_b,_c,4)(1,2,3)


struct MoreToCome {
#if (defined (FCPP_CONCEPTS_LITE) || defined(FCPP_TICK) )
  void more_to_come() const {};  // For detection purposes
  typedef MoreToCome ElementType; // Dummy to fool TICK.
#endif
};
MoreToCome _abc;  // __ is not allowed as it is reserved.
// __ is deprecated and has been removed here and from example code.

// This is equivalent to _ in FC++. I have made it different
// as the two things may need to coexist in code.
// I will link them also when using concepts lite.
#if (defined(FCPP_LAMBDA_AUTOCURRY) || defined(FCPP_CONCEPTS_LITE)  || defined(FCPP_TICK))
// What this does is make them equivalent when lambda currying is in use.
typedef fcpp::AutoCurryType OneMoreHere;
#else
struct OneMoreHere {};
#endif
OneMoreHere _a, _b, _c, _d, _e, _f; // alternative names

#ifdef FCPP_CONCEPTS_LITE
// This is for testing of the failure cases.
struct Rubbish {
  void rubbish() const {};  // For detection purposes
};
 // For some reason I do not understand, this works here but not
 // when located before the definition of MoreToCome.
 // This is the only concept in active use for VFC++.
 // Detection of MoreToCome defined in variadic_fcpp.h
 // Defined here as that is timely.
   template <typename T>
   bool concept is_more_to_come() {
      return requires (T m) {
	  m.more_to_come();
      };
   }

   template <typename T>
   bool concept is_rubbish() {
      return requires (T r) {
	 r.rubbish();
      };
   }

#endif

#ifdef FCPP_TICK
///////////////////////////////////////////////////////////////////////
// FC++ Tick Traits
///////////////////////////////////////////////////////////////////////
// This is the only concept in active use for VFC++.
// Detection of MoreToCome defined in curry.h
TICK_TRAIT(is_more_to_come)
{
   template <class T>
     auto require (T&& a) -> valid<
     decltype(a.more_to_come())
     >;
};
///////////////////////////////////////////////////////////////////////
#endif

struct Bind1 { int n = 1; };
Bind1 bind_first;

struct Bind2 { int n = 2; };
Bind2 bind_second;

struct Bind3 { int n = 3; };
Bind3 bind_third;

struct Bind4 { int n = 4; };
Bind4 bind_fourth;

struct Bind5 { int n = 5; };
Bind5 bind_fifth;

struct Bind6 { int n = 6; };
Bind6 bind_sixth;

// The binders depend on FullN so that it must be forward declared
template <class F> struct FullN;

// makeFullN is also needed so it is declared here.  (c.f. FC++ curry.h)
template <class F> FullN<F> makeFullN( const F& f ) { return FullN<F>(f); }

// It works if the binders are moved here, leaving the functoids afterwards.

template <class Nary, typename Arg1>
class binder1ofN  : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
public:
   binder1ofN( const Nary& g, const Arg1& a ) : f(g), x(a) {}
   template <typename... XYZ>
     struct Sig :
   public FunType<Arg1,XYZ..., typename RT<Nary,Arg1,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Void Arg3Type;
       typedef Void Arg4Type;
     };

   template <typename... XYZ>
     typename RT<Nary,Arg1,XYZ...>::ResultType operator()(const XYZ&... xyz) const
   {
     // std::cout << "x = " << x << std::endl;
     return f(x, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	 temp += std::string("binder1ofN::")
	      + debug_traits_name(f);
  	 temp += std::string("( ");
	 temp += debug_name_or_value(x);
         temp += std::string(" , ... )");
	 //}
       return temp;
     } else
       return std::string("binder1ofN");
     }
#endif
};

template <class Nary, typename Arg1, typename Arg2>
class binder1and2ofN  : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
   const Arg2 y;
public:
   binder1and2ofN( const Nary& g, const Arg1& a, const Arg2& b ) : f(g), x(a), y(b) {}
   template <typename... XYZ>
     struct Sig :
   public FunType<XYZ..., typename RT<Nary,Arg1,Arg2,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg2 Arg3Type;
       typedef Void Arg4Type;
     };

   template <typename... XYZ>
     typename RT<Nary,Arg1,Arg2,XYZ...>::ResultType operator()(const XYZ&... xyz) const
   {
     return f(x, y, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     using namespace fcpp;
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder1and2ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( ")
             + debug_name_or_value(x)
	     + std::string(" , ")
       	     + debug_name_or_value(y)
             + std::string(" , ... )");
        return temp;
     } else
        return std::string("binder1and2ofN");
   }
#endif
};

template <class Nary, typename Arg1, typename Arg3>
class binder1and3ofN : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
   const Arg3 z;
public:
   binder1and3ofN( const Nary& g, const Arg1& a, const Arg3& c ) : f(g), x(a), z(c) {}
   template <typename B,typename... XYZ>
     struct Sig :
     public FunType<Arg1,B,Arg3,XYZ..., typename RT<Nary,Arg1,B,Arg3,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg3 Arg3Type;
       typedef Void Arg4Type;
     };

   template <typename B,typename... XYZ>
     typename RT<Nary,Arg1,B,Arg3,XYZ...>::ResultType operator()(const B& b,const XYZ&... xyz) const
   {
     return f(x, b, z, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     //std::string temp;
     // if ( debug_traits_has_name<Nary>() ) {
     //	temp +=  std::string("binder1and3ofN::")
     //	     + debug_traits_name(f);
     using namespace fcpp;
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder1and3ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( ")
             + debug_name_or_value(x)
	     + std::string(" , _ , ")
       	     + debug_name_or_value(z)
             + std::string(" , ... )");
        return temp;
     } else
        return std::string("binder1and3ofN");
   }
#endif
};

// Forward declarations needed.
template <class Nary, typename Arg1, typename Arg2, typename Arg4>
  class binder1and2and4ofN;

template <class Nary, typename Arg1, typename Arg2, typename Arg5>
  class binder1and2and5ofN;

template <class Nary, typename Arg1, typename Arg3, typename Arg5>
  class binder1and3and5ofN;

template <class Nary, typename Arg1, typename Arg4, typename Arg5>
  class binder1and4and5ofN;

template <class Nary, typename Arg1, typename Arg2, typename Arg3, typename Arg5>
  class binder1and2and3and5ofN;

// Alternative using helper function. Tests O.K. but not in use.
/*
template <typename R,typename F,typename... Args>
struct binder1and4ofNHelper;

template <typename R,typename F,typename A,typename B, typename C>
  struct binder1and4ofNHelper<R,F,A,B,C>
{
  static inline R go( const F& f, const A& a, const B& b, const C& c)
  {
    return makeFullN(binder1and2and4ofN<F,B,A,C>(f,a,b,c) );
  }
};
*/

template <class Nary, typename Arg1, typename Arg4>
class binder1and4ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
   const Arg4 u;
public:
   binder1and4ofN( const Nary& g, const Arg1& a, const Arg4& d ) : f(g), x(a), u(d) {}

   template <typename... XYZ> struct Sig;

   template <typename B,typename C,typename... XYZ>
     struct Sig<B,C,XYZ...> :
     public FunType<Arg1,B,C,Arg4,XYZ..., typename RT<Nary,Arg1,B,C,Arg4,XYZ...>::ResultType >
   {
     typedef Nary Arg1Type;
     typedef Arg1 Arg2Type;
     typedef Arg4 Arg3Type;
     typedef Void Arg4Type;
   };

   template <typename B>
   struct Sig<B>
     {
       typedef FullN<binder1and2and4ofN<Nary,Arg1,B,Arg4> > ResultType;
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg4 Arg3Type;
       typedef Void Arg4Type;
     };

   // This is a special case for bind1and4ofN(f,a,d)(b)(c).
   // The call to this object has to be handed on to one which will hold
   // three arguments and wait for the fourth one to make the closure.
   // This is the first time I have done this in these binders.
   // There are similar specialisations in binderAofN.
   // The intention is that any reasonable argument combination should work.
   // I had errors when missing the second const from this function definition.
   template <typename B>
     typename Sig<B>::ResultType operator() (const B& b) const
     {
       return makeFullN(binder1and2and4ofN<Nary,Arg1,B,Arg4>(f,x,b,u) );
       // Alternative using helper function.
       //return  binder1and4ofNHelper<typename Sig<B>::ResultType,Nary,Arg1,B,Arg4>::go
       //	 (f,x,b,u);
     }

   template <typename B,typename C,typename... XYZ>
     typename RT<Nary,Arg1,B,C,Arg4,XYZ...>::ResultType operator()
     (const B& b,const C& c,const XYZ&... xyz) const
   {
     return f(x, b, c, u, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     using namespace fcpp;
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp += std::string("binder1and4ofN::")
	     + debug_traits_name(f);
        temp += std::string("( ")
             + debug_name_or_value(x)
	     + std::string(" , _ , _ , ")
       	     + debug_name_or_value(u)
             + std::string(" , ... )");
        return temp;
     } else
        return std::string("binder1and4ofN");
   }
#endif
};

template <class Nary, typename Arg1, typename Arg5>
class binder1and5ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
   const Arg5 v;
public:
   binder1and5ofN( const Nary& g, const Arg1& a, const Arg5& e ) : f(g), x(a), v(e) {}

   template <typename... XYZ> struct Sig;

   template <typename B,typename C,typename D,typename... XYZ>
     struct Sig<B,C,D,XYZ...> :
     public FunType<Arg1,B,C,D,Arg5,XYZ..., typename RT<Nary,Arg1,B,C,D,Arg5,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg5 Arg3Type;
       typedef Void Arg4Type;
     };

   template <typename B>
   struct Sig<B>
     {
       typedef FullN<binder1and2and5ofN<Nary,Arg1,B,Arg5> > ResultType;
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg5 Arg3Type;
       typedef Void Arg4Type;
     };

   /* I have come to the conclusion this is not needed as it is curried in FullN. */
   /*
   template <typename B>
   struct Sig<OneMoreHere,B>
     {
       typedef FullN<binder1and3and5ofN<Nary,Arg1,B,Arg5> > ResultType;
     };
   */
   template <typename B,typename C>
     struct Sig<B,C>
     {
       typedef FullN<binder1and2and3and5ofN<Nary,Arg1,B,C,Arg5> > ResultType;
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg5 Arg3Type;
       typedef Void Arg4Type;
     };

   // This is a special case for bind1and5ofN(f,a,e)(b)(c,d).
   // The call to this object has to be handed on to one which will hold
   // three arguments and wait for the fourth one to make the closure.
   // This is the first time I have done this in these binders.
   // There are similar specialisations in binderAofN.
   // The intention is that any reasonable argument combination should work.
   // I had errors when missing the second const from this function definition.
   template <typename B>
     typename Sig<B>::ResultType operator() (const B& b) const
     {
       return makeFullN(binder1and2and5ofN<Nary,Arg1,B,Arg5>(f,x,b,v) );
     }
   /* I have come to the conclusion this is not needed as it is curried in FullN. */
   /*
   template <typename B>
     typename Sig<OneMoreHere,B>::ResultType operator() (const OneMoreHere &,const B& b) const
     {
       return makeFullN(binder1and3and5ofN<Nary,Arg1,B,Arg5>(f,x,b,v) );
     }
   */
   template <typename B,typename C>
     typename Sig<B,C>::ResultType operator() (const B& b,const C& c) const
     {
       return makeFullN(binder1and2and3and5ofN<Nary,Arg1,B,C,Arg5>(f,x,b,c,v) );
     }

   template <typename B,typename C,typename D,typename... XYZ>
     typename RT<Nary,Arg1,B,C,D,Arg5,XYZ...>::ResultType operator()
     (const B& b,const C& c,const D&d,const XYZ&... xyz) const
   {
     //std::cout << "binder1and5ofN operator() called." << std::endl;
     return f(x, b, c, d, v, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     using namespace fcpp;
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder1and5ofN::")
	     + debug_traits_name(f);
	//temp += std::string("::");
       	temp += std::string("( ")
             + debug_name_or_value(x)
	     + std::string(" , _ , _ , _ , ")
       	     + debug_name_or_value(v)
             + std::string(" , ... )");
        return temp;
     } else
        return std::string("binder1and5ofN");
   }
#endif
};


template <class Nary, typename Arg2, typename Arg3>
class binder2and3ofN  : public SmartFunctoidN
{
   const Nary f;
   const Arg2 y;
   const Arg3 z;
public:
   binder2and3ofN( const Nary& g, const Arg2& b, const Arg3& c ) : f(g), y(b), z(c) {}
   template <typename A,typename... XYZ>
     struct Sig :
     public FunType<A,Arg2,Arg3,XYZ..., typename RT<Nary,A,Arg2,Arg3,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg2 Arg2Type;
       typedef Arg3 Arg3Type;
       typedef Void Arg4Type;
     };

   template <typename A,typename... XYZ>
     typename RT<Nary,A,Arg2,Arg3,XYZ...>::ResultType operator()(const A& a,const XYZ&... xyz) const
   {
     return f(a, y, z, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     using namespace fcpp;
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder2and3ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( _ , ")
             + debug_name_or_value(y)
	     + std::string(" , ")
       	     + debug_name_or_value(z)
             + std::string(" , ... )");
        return temp;
     } else
       return std::string("binder2and3ofN");
   }
#endif
};

template <class Nary, typename Arg2, typename Arg4>
class binder2and4ofN  : public SmartFunctoidN
{
   const Nary f;
   const Arg2 y;
   const Arg4 z;
public:
   binder2and4ofN( const Nary& g, const Arg2& b, const Arg4& c ) : f(g), y(b), z(c) {}
   template <typename A,typename B,typename... XYZ>
     struct Sig :
     public FunType<A,Arg2,B,Arg4,XYZ..., typename RT<Nary,A,Arg2,B,Arg4,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg2 Arg2Type;
       typedef Arg4 Arg3Type;
       typedef Void Arg4Type;
     };

   template <typename A,typename B,typename... XYZ>
     typename RT<Nary,A,Arg2,B,Arg4,XYZ...>::ResultType operator()(const A& a,const B& b,const XYZ&... xyz) const
   {
     return f(a, y, b, z, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     using namespace fcpp;
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder2and4ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( _ , ")
             + debug_name_or_value(y)
	     + std::string(" , _ , ")
       	     + debug_name_or_value(z)
             + std::string(" , ... )");
        return temp;
     } else
       return std::string("binder2and4ofN");
   }
#endif
};

template <class Nary, typename Arg3, typename Arg4>
class binder3and4ofN  : public SmartFunctoidN
{
   const Nary f;
   const Arg3 y;
   const Arg4 z;
public:
   binder3and4ofN( const Nary& g, const Arg3& b, const Arg4& c ) : f(g), y(b), z(c) {}
   template <typename A,typename B,typename... XYZ>
     struct Sig :
     public FunType<A,B,Arg3,Arg4,XYZ..., typename RT<Nary,A,B,Arg3,Arg4,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg3 Arg2Type;
       typedef Arg4 Arg3Type;
       typedef Void Arg4Type;
     };

   template <typename A,typename B,typename... XYZ>
     typename RT<Nary,A,B,Arg3,Arg4,XYZ...>::ResultType operator()(const A& a,const B& b,const XYZ&... xyz) const
   {
     return f(a, b, y, z, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     using namespace fcpp;
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder3and4ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( _ , _ , ")
             + debug_name_or_value(y)
	     + std::string(" , ")
       	     + debug_name_or_value(z)
             + std::string(" , ... )");
        return temp;
     } else
       return std::string("binder3and4ofN");
   }
#endif
};


template <class Nary, typename Arg1, typename Arg2, typename Arg3>
class binder1and2and3ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
   const Arg2 y;
   const Arg3 z;
public:
   binder1and2and3ofN( const Nary& g, const Arg1& a, const Arg2& b, const Arg3& c ) :
     f(g), x(a), y(b), z(c) {}
   template <typename... XYZ>
     struct Sig :
   public FunType<Arg1,Arg2,Arg3,XYZ..., typename RT<Nary,Arg1,Arg2,Arg3,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg2 Arg3Type;
       typedef Arg3 Arg4Type;
     };

   template <typename... XYZ>
     typename RT<Nary,Arg1,Arg2,Arg3,XYZ...>::ResultType operator()(const XYZ&... xyz) const
   {
     return f(x, y, z, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder1and2and3ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( ")
             + debug_name_or_value(x)
	     + std::string(" , ")
             + debug_name_or_value(y)
	     + std::string(" , ")
       	     + debug_name_or_value(z)
             + std::string(" , ... )");
        return temp;
     } else
       return std::string("binder1and2and3ofN");
   }
#endif
};

template <class Nary, typename Arg1, typename Arg2, typename Arg4>
class binder1and2and4ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
   const Arg2 y;
   const Arg4 u;
public:
   binder1and2and4ofN( const Nary& g, const Arg1& a, const Arg2& b, const Arg4& d ) :
     f(g), x(a), y(b), u(d) {}
   template <typename C,typename... XYZ>
     struct Sig :
   public FunType<Arg1,Arg2,C,Arg4,XYZ..., typename RT<Nary,Arg1,Arg2,C,Arg4,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg2 Arg3Type;
       typedef Arg4 Arg4Type;
     };

   template <typename C,typename... XYZ>
     typename RT<Nary,Arg1,Arg2,C,Arg4,XYZ...>::ResultType operator()(const C& c,const XYZ&... xyz) const
   {
     return f(x, y, c, u, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder1and2and4ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( ")
             + debug_name_or_value(x)
	     + std::string(" , ")
             + debug_name_or_value(y)
	     + std::string(" , _ , ")
       	     + debug_name_or_value(u)
             + std::string(" , ... )");
        return temp;
     } else
       return std::string("binder1and2and4ofN");
   }
#endif
};

template <class Nary, typename Arg1, typename Arg2, typename Arg5>
class binder1and2and5ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
   const Arg2 y;
   const Arg5 v;
public:
   binder1and2and5ofN( const Nary& g, const Arg1& a, const Arg2& b, const Arg5& e ) :
     f(g), x(a), y(b), v(e) {}
   template <typename C,typename D,typename... XYZ>
     struct Sig :
   public FunType<Arg1,Arg2,C,D,Arg5,XYZ..., typename RT<Nary,Arg1,Arg2,C,D,Arg5,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg2 Arg3Type;
       typedef Arg5 Arg4Type;
     };

   template <typename C,typename D,typename... XYZ>
     typename RT<Nary,Arg1,Arg2,C,D,Arg5,XYZ...>::ResultType operator()(const C& c,const D&d,const XYZ&... xyz) const
   {
     //std::cout << "binder1and2and5ofN operator() called." << std::endl;
     return f(x, y, c, d, v, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder1and2and5ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( ")
             + debug_name_or_value(x)
	     + std::string(" , ")
             + debug_name_or_value(y)
	     + std::string(" , _ , _ , ")
       	     + debug_name_or_value(v)
             + std::string(" , ... )");
        return temp;
     } else
       return std::string("binder1and2and5ofN");
   }
#endif
};

template <class Nary, typename Arg1, typename Arg3, typename Arg4>
class binder1and3and4ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
   const Arg3 z;
   const Arg4 u;
public:
   binder1and3and4ofN( const Nary& g, const Arg1& a, const Arg3& c, const Arg4& d ) :
     f(g), x(a), z(c), u(d) {}
   template <typename B,typename... XYZ>
     struct Sig :
   public FunType<B,XYZ..., typename RT<Nary,Arg1,B,Arg3,Arg4,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg3 Arg3Type;
       typedef Arg4 Arg4Type;
     };

   template <typename B,typename... XYZ>
     typename RT<Nary,Arg1,B,Arg3,Arg4,XYZ...>::ResultType operator()(const B& b,const XYZ&... xyz) const
   {
     return f(x, b, z, u, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder1and3and4ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( ")
             + debug_name_or_value(x)
	     + std::string(" , _ , ")
             + debug_name_or_value(z)
	     + std::string(" , ")
       	     + debug_name_or_value(u)
             + std::string(" , ... )");
        return temp;
     } else
       return std::string("binder1and3and4ofN");
   }
#endif
};

template <class Nary, typename Arg1, typename Arg3, typename Arg5>
class binder1and3and5ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
   const Arg3 z;
   const Arg5 v;
public:
   binder1and3and5ofN( const Nary& g, const Arg1& a, const Arg3& c, const Arg5& e ) :
     f(g), x(a), z(c), v(e) {}
   template <typename B, typename D,typename... XYZ>
     struct Sig :
   public FunType<B,D,XYZ..., typename RT<Nary,Arg1,B,Arg3,D,Arg5,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg3 Arg3Type;
       typedef Arg5 Arg4Type;
     };

   template <typename B,typename D,typename... XYZ>
     typename RT<Nary,Arg1,B,Arg3,D,Arg5,XYZ...>::ResultType operator()(const B& b,const D&d,const XYZ&... xyz) const
   {
     return f(x, b, z, d, v, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder1and3and5ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( ")
             + debug_name_or_value(x)
	     + std::string(" , _ , ")
             + debug_name_or_value(z)
	     + std::string(" , _ , ")
       	     + debug_name_or_value(v)
             + std::string(" , ... )");
        return temp;
     } else
       return std::string("binder1and3and5ofN");
   }
#endif
};


template <class Nary, typename Arg1, typename Arg4, typename Arg5>
class binder1and4and5ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
   const Arg4 u;
   const Arg5 v;
public:
   binder1and4and5ofN( const Nary& g, const Arg1& a, const Arg4& d, const Arg5& e ) :
     f(g), x(a), u(d), v(e) {}
   template <typename B, typename C,typename... XYZ>
     struct Sig :
     public FunType<B,C,XYZ..., typename RT<Nary,Arg1,B,C,Arg4,Arg5,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg4 Arg3Type;
       typedef Arg5 Arg4Type;
     };

   template <typename B,typename C,typename... XYZ>
     typename RT<Nary,Arg1,B,C,Arg4,Arg5,XYZ...>::ResultType operator()(const B& b,const C&c,const XYZ&... xyz) const
   {
     return f(x, b, c, u, v, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder1and4and5ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( ")
             + debug_name_or_value(x)
	     + std::string(" , _ , _ , ")
             + debug_name_or_value(u)
	     + std::string(" , ")
       	     + debug_name_or_value(v)
             + std::string(" , ... )");
        return temp;
     } else
       return std::string("binder1and4and5ofN");
   }
#endif
};

template <class Nary, typename Arg2, typename Arg3, typename Arg4>
class binder2and3and4ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg2 y;
   const Arg3 z;
   const Arg4 u;
public:
   binder2and3and4ofN( const Nary& g, const Arg2& b, const Arg3& c, const Arg4& d ) :
     f(g), y(b), z(c), u(d) {}
   template <typename A,typename... XYZ>
     struct Sig :
   public FunType<A,XYZ..., typename RT<Nary,A,Arg2,Arg3,Arg4,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg2 Arg2Type;
       typedef Arg3 Arg3Type;
       typedef Arg4 Arg4Type;
     };

   template <typename A,typename... XYZ>
     typename RT<Nary,A,Arg2,Arg3,Arg4,XYZ...>::ResultType operator()(const A& a,const XYZ&... xyz) const
   {
     return f(a, y, z, u, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder2and3and4ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( _ , ")
             + debug_name_or_value(y)
	     + std::string(" , ")
             + debug_name_or_value(z)
	     + std::string(" , ")
       	     + debug_name_or_value(u)
             + std::string(" , ... )");
        return temp;
     } else
       return std::string("binder2and3and4ofN");
   }
#endif
};

template <class Nary, typename Arg1, typename Arg2, typename Arg3, typename Arg5>
class binder1and2and3and5ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg1 x;
   const Arg2 y;
   const Arg3 z;
   const Arg5 v;
public:
   binder1and2and3and5ofN( const Nary& g, const Arg1& a, const Arg2& b, const Arg3& c, const Arg5& e ) :
   f(g), x(a), y(b), z(c), v(e) {}
   template <typename D,typename... XYZ>
     struct Sig :
     public FunType<D,XYZ..., typename RT<Nary,Arg1,Arg2,Arg3,D,Arg5,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg1 Arg2Type;
       typedef Arg2 Arg3Type;
       typedef Arg5 Arg4Type;
     };

   template <typename D,typename... XYZ>
     typename RT<Nary,Arg1,Arg2,Arg3,D,Arg5,XYZ...>::ResultType operator()(const D&d,const XYZ&... xyz) const
   {
     return f(x, y, z, d, v, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     using namespace fcpp;
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder1and2and3and5ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( ")
             + debug_name_or_value(x)
	     + std::string(" , ")
             + debug_name_or_value(y)
	     + std::string(" , ")
       	     + debug_name_or_value(z)
	     + std::string(" , _ , ")
       	     + debug_name_or_value(v)
             + std::string(" , ... )");
        return temp;
     } else
        return std::string("binder1and2and3and5ofN");
   }
#endif
};


template <class Nary, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
class binder2and3and4and5ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg2 y;
   const Arg3 z;
   const Arg4 u;
   const Arg5 v;
public:
   binder2and3and4and5ofN( const Nary& g, const Arg2& b, const Arg3& c, const Arg4& d, const Arg5& e ) :
   f(g), y(b), z(c), u(d), v(e) {}
   template <typename A,typename... XYZ>
     struct Sig :
     public FunType<A,XYZ..., typename RT<Nary,A,Arg2,Arg3,Arg4,Arg5,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Arg2 Arg2Type;
       typedef Arg3 Arg3Type;
       typedef Arg4 Arg4Type;
       typedef Arg5 Arg5Type;
     };

   template <typename A,typename... XYZ>
     typename RT<Nary,A,Arg2,Arg3,Arg4,Arg5,XYZ...>::ResultType operator()(const A&a,const XYZ&... xyz) const
   {
     return f(a, y, z, u, v, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder2and3and4and5ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( _ , ")
             + debug_name_or_value(y)
	     + std::string(" , ")
             + debug_name_or_value(z)
	     + std::string(" , ")
       	     + debug_name_or_value(u)
	     + std::string(" , ")
       	     + debug_name_or_value(v)
             + std::string(" , ... )");
        return temp;
     } else
        return std::string("binder2and3and4and5ofN");
   }
#endif
};

template <class Nary, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
class binder2and3and4and5and6ofN   : public SmartFunctoidN
{
   const Nary f;
   const Arg2 y;
   const Arg3 z;
   const Arg4 u;
   const Arg5 v;
   const Arg6 w;
public:
 binder2and3and4and5and6ofN( const Nary& g, const Arg2& b, const Arg3& c, const Arg4& d, const Arg5& e, 
                         const Arg6& ff ) :
   f(g), y(b), z(c), u(d), v(e), w(ff) {}
   template <typename A,typename... XYZ>
     struct Sig :
     public FunType<A,XYZ..., typename RT<Nary,A,Arg2,Arg3,Arg4,Arg5,Arg6,XYZ...>::ResultType >
     {};

   template <typename A,typename... XYZ>
     typename RT<Nary,A,Arg2,Arg3,Arg4,Arg5,Arg6,XYZ...>::ResultType 
     operator()(const A&a,const XYZ&... xyz) const
   {
     return f(a, y, z, u, v, w, xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Nary>() ) {
	temp +=  std::string("binder2and3and4and5and6ofN::")
	     + debug_traits_name(f);
       	temp += std::string("( _ , ")
             + debug_name_or_value(y)
	     + std::string(" , ")
             + debug_name_or_value(z)
	     + std::string(" , ")
       	     + debug_name_or_value(u)
	     + std::string(" , ")
       	     + debug_name_or_value(v)
	     + std::string(" , ")
       	     + debug_name_or_value(w)
             + std::string(" , ... )");
        return temp;
     } else
       return std::string("binder2and3and4and5and6ofN");
   }
#endif
};

/*
   ============================
   Binding M arguments of N > M
   ============================
This was not feasible using the method above
because the line const Args... args; does not compile.
It has been necessary to devise some way of storing the unknown number of
actual bound arguments.
Doug Gregor has supplied how to store them using a tuple.
I have now sorted out helper functions to restore them.

These routines are fine, on the assumption that it is the first M
arguments which are bound. Unfortunately I have been using them for the
case where there was a gap before them. Also, my test case of plusN
did not show up when the arguments were in the wrong order.
*/
template <class Nary, typename... Args>
class binderMofN : public SmartFunctoidN
{
   const Nary f;
   const TR1::tuple<Args...> args;
public:
   typedef typename  TR1::tuple<Args...> tupleType;
#ifdef FCPP_DEBUG
   // Needed to get the name of the first arg.
   typedef typename TR1::tuple_element<0,tupleType>::type Arg1;
   //Arg1 x = TR1::get<0>(args); - this does not work
   //  - see below for the solution.
#endif
   // This makes a tuple from the incoming arguments.
   binderMofN( const Nary& g, const Args&... abc ) :
   f(g) ,  args(TR1::make_tuple(abc...)) {}

   template <int N, typename... XYZ>
   struct SigN;
   template <typename... XYZ>
   struct SigN<1,XYZ...> {
          typedef typename RT<Nary,
          typename TR1::tuple_element<0,tupleType>::type,
	    XYZ...>::ResultType ResultType;
	  typedef typename TR1::tuple_element<0,tupleType>::type Arg1Type;
          typedef Void Arg2Type;
          typedef Void Arg3Type;
          typedef Void Arg4Type;
   };

   template <typename... XYZ>
   struct SigN<2,XYZ...> {
          typedef typename RT<Nary,
          typename TR1::tuple_element<0,tupleType>::type,
          typename TR1::tuple_element<1,tupleType>::type,
	    XYZ...>::ResultType  ResultType;
	  typedef typename TR1::tuple_element<0,tupleType>::type Arg1Type;
	  typedef typename TR1::tuple_element<1,tupleType>::type Arg2Type;
          typedef Void Arg3Type;
          typedef Void Arg4Type;
   };

   template <typename... XYZ>
   struct SigN<3,XYZ...> {
          typedef typename RT<Nary,
          typename TR1::tuple_element<0,tupleType>::type,
          typename TR1::tuple_element<1,tupleType>::type,
          typename TR1::tuple_element<2,tupleType>::type,
	    XYZ...>::ResultType ResultType;
	  typedef typename TR1::tuple_element<0,tupleType>::type Arg1Type;
	  typedef typename TR1::tuple_element<1,tupleType>::type Arg2Type;
	  typedef typename TR1::tuple_element<2,tupleType>::type Arg3Type;
          typedef Void Arg4Type;
   };

   template <typename... XYZ>
   struct SigN<4,XYZ...> {
          typedef typename RT<Nary,
          typename TR1::tuple_element<0,tupleType>::type,
          typename TR1::tuple_element<1,tupleType>::type,
          typename TR1::tuple_element<2,tupleType>::type,
          typename TR1::tuple_element<3,tupleType>::type,
	    XYZ...>::ResultType ResultType;
	  typedef typename TR1::tuple_element<0,tupleType>::type Arg1Type;
	  typedef typename TR1::tuple_element<1,tupleType>::type Arg2Type;
	  typedef typename TR1::tuple_element<2,tupleType>::type Arg3Type;
	  typedef typename TR1::tuple_element<3,tupleType>::type Arg4Type;
   };

   template <typename... XYZ>
   struct SigN<5,XYZ...> {
          typedef typename RT<Nary,
          typename TR1::tuple_element<0,tupleType>::type,
          typename TR1::tuple_element<1,tupleType>::type,
          typename TR1::tuple_element<2,tupleType>::type,
          typename TR1::tuple_element<3,tupleType>::type,
          typename TR1::tuple_element<4,tupleType>::type,
	    XYZ...>::ResultType ResultType;
	  typedef typename TR1::tuple_element<0,tupleType>::type Arg1Type;
	  typedef typename TR1::tuple_element<1,tupleType>::type Arg2Type;
	  typedef typename TR1::tuple_element<2,tupleType>::type Arg3Type;
	  typedef typename TR1::tuple_element<3,tupleType>::type Arg4Type;
	  typedef typename TR1::tuple_element<4,tupleType>::type Arg5Type;
   };

   template <typename... XYZ>
     struct Sig : public SigN<sizeof...(Args),XYZ...>
     {};


   // Previous Sig now replaced.
   //  template <typename... XYZ>
   //  struct Sig :
   //public FunType<XYZ..., typename RT<Nary,Args...,XYZ...>::ResultType >
   //  {};

   template <int N, typename... ABC > struct binderHelper;

   template <typename... XYZ>
     struct binderHelper<1,XYZ...> {
       static inline typename RT<Nary,
          typename TR1::tuple_element<0,tupleType>::type,
          XYZ...>::ResultType
	  go(const Nary& ff,const tupleType& abc,const XYZ&... xyz)
       {
	 return ff(TR1::get<0>(abc),xyz...);
       }
     };

   template <typename... XYZ>
     struct binderHelper<2,XYZ...> {
       static inline typename RT<Nary,
          typename TR1::tuple_element<0,tupleType>::type,
          typename TR1::tuple_element<1,tupleType>::type,
          XYZ...>::ResultType
	  go(const Nary& ff,const tupleType& abc,const XYZ&... xyz)
       {
	 /*	 std::cout << "binderHelper<2,XYZ...> has args "
                   << TR1::get<0>(abc) << ", "
                   << TR1::get<1>(abc)
                   << std::endl; */
	 return ff(TR1::get<0>(abc),TR1::get<1>(abc),xyz...);
       }
     };

   template <typename... XYZ>
     struct binderHelper<3,XYZ...> {
       static inline typename RT<Nary,
          typename TR1::tuple_element<0,tupleType>::type,
          typename TR1::tuple_element<1,tupleType>::type,
          typename TR1::tuple_element<2,tupleType>::type,
          XYZ...>::ResultType
	  go(const Nary& ff,const tupleType& abc,const XYZ&... xyz)
       {
	 /*	 std::cout << "binderHelper<3,XYZ...> has args "
                   << TR1::get<0>(abc) << ", "
                   << TR1::get<1>(abc) << ", "
                   << TR1::get<2>(abc)
                   << std::endl; */
	 return ff(TR1::get<0>(abc),TR1::get<1>(abc),TR1::get<2>(abc),xyz...);
       }
     };

   template <typename... XYZ>
     struct binderHelper<4,XYZ...> {
       static inline typename RT<Nary,
          typename TR1::tuple_element<0,tupleType>::type,
          typename TR1::tuple_element<1,tupleType>::type,
          typename TR1::tuple_element<2,tupleType>::type,
          typename TR1::tuple_element<3,tupleType>::type,
          XYZ...>::ResultType
	  go(const Nary& ff,const tupleType& abc,const XYZ&... xyz)
       {
	 /*	 std::cout << "binderHelper<4,XYZ...> has args "
                   << TR1::get<0>(abc) << ", "
                   << TR1::get<1>(abc) << ", "
                   << TR1::get<2>(abc) << ", "
                   << TR1::get<3>(abc)
                   << std::endl; */
	 return ff(TR1::get<0>(abc),TR1::get<1>(abc),
                   TR1::get<2>(abc),TR1::get<3>(abc),xyz...);
       }
     };

   template <typename... XYZ>
     struct binderHelper<5,XYZ...> {
       static inline typename RT<Nary,
          typename TR1::tuple_element<0,tupleType>::type,
          typename TR1::tuple_element<1,tupleType>::type,
          typename TR1::tuple_element<2,tupleType>::type,
          typename TR1::tuple_element<3,tupleType>::type,
          typename TR1::tuple_element<4,tupleType>::type,
          XYZ...>::ResultType
	  go(const Nary& ff,const tupleType& abc,const XYZ&... xyz)
       {
	 /*	 std::cout << "binderHelper<4,XYZ...> has args "
                   << TR1::get<0>(abc) << ", "
                   << TR1::get<1>(abc) << ", "
                   << TR1::get<2>(abc) << ", "
                   << TR1::get<3>(abc)
                   << std::endl; */
	 return ff(TR1::get<0>(abc),TR1::get<1>(abc),
                   TR1::get<2>(abc),TR1::get<3>(abc),
                   TR1::get<4>(abc),xyz...);
       }
     };

   // Previous return type now replaced.
   // template <typename... XYZ>
   // typename RT<Nary,Args...,XYZ...>::ResultType
   // operator()(const XYZ&... xyz) const
   template <typename... XYZ>
   typename Sig<XYZ...>::ResultType operator()(const XYZ&... xyz) const
   {
     // Need a helper function to unpack the tuple.
     //return f(args..., xyz...);
     // Note that args here is a tuple object NOT an argument pack.
     return binderHelper<sizeof...(Args),XYZ...>::go(f,args,xyz...);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   // The type of the first argument is declared above as Arg1.
   // The object for the first argument has to be retrieved from the tuple.
   std::string name() const {
     if ( debug_traits_has_name<Nary>() ) {
       if (debug_traits_has_name<Arg1>() ) {
	  return std::string("binderMofN::")+debug_traits_name(f)
       		  + debug_traits_name(TR1::get<0>(args));
       } else
	  return std::string("binderMofN::")+ debug_traits_name(f);
   } else
        return std::string("binderMofN");
   }
#endif
};

////////////////////////////////////////////////////////////
// The next idea - binderAofN
// This arises from an idea of Mark Phillips for FC++.
// I am going to try it out in here first.
// The idea is to do binderAofN(F,Binder,A)
// where
// F is the functoid
// Binder is the location defined as a compile time constant
// A is the argument.
// My idea is to make the location and argument into a tuple
// which in this version can be variadic anyway.
// An FC++ version need not have the tuple, but would need
// to be done for each size.
///////////////////////////////////////////////////////////
// At the moment this code assumes that the next call will be a closure
// and this may not be the case. Some cases work but others do not.
// variadic_fcpp::bindAofN(variadic_fcpp::plusN,bind_second,2)(1,_abc)(3) is O.K.
// variadic_fcpp::bindAofN(variadic_fcpp::plusN,bind_third,3)(1,_abc)(2) is not.
// The reason is that the case is not properly catered for.
// The call to evaluate on bindAofN needs to know about the second parameter,
// which has not yet become available.
// This problem has now been sorted out.
//////////////////////////////////////////////////////////

 template <typename Binder,typename R,typename F,typename A,typename... Args>
struct binderAofNHelper;

 template <typename R,typename F,typename A,typename... Args>
   struct binderAofNHelper<Bind1,R,F,A,Args...>
{
  static inline R go( const F& f, const A& a, const Args&... args)
  {
    return f(a,args...);
  }
};

 template <typename R,typename F,typename A,typename B,typename... Args>
   struct binderAofNHelper<Bind2,R,F,A,B,Args...>
{
  static inline R go( const F& f, const A& a, const B& b, const Args&... args)
  {
    return f(b,a,args...);
  }
};
 template <typename R,typename F,typename A,typename B,typename C,typename... Args>
   struct binderAofNHelper<Bind3,R,F,A,B,C,Args...>
{
  static inline R go( const F& f, const A& a, const B& b, const C&c, const Args&... args)
  {
    return f(b,c,a,args...);
  }
};

// This is the special case bindAofN(F,bind_third,a)(b)(c).
 template <typename R,typename F,typename A,typename B>
   struct binderAofNHelper<Bind3,R,F,A,B>
{
  static inline R go( const F& f, const A& a, const B& b)
  {
    return makeFullN(binder1and3ofN<F,B,A>(f,b,a) );
  }
};

template <typename R,typename F,typename A,typename B,typename C,typename D,
          typename... Args>
  struct binderAofNHelper<Bind4,R,F,A,B,C,D,Args...>
{
  static inline R go( const F& f, const A& a, const B& b, const C&c, const D& d,
                      const Args&... args)
  {
    return f(b,c,d,a,args...);
  }
};

// This is the first special case bindAofN(F,bind_fourth,a)(b)(c,d).
 template <typename R,typename F,typename A,typename B>
   struct binderAofNHelper<Bind4,R,F,A,B>
{
  static inline R go( const F& f, const A& a, const B& b)
  {
    return makeFullN(binder1and4ofN<F,B,A>(f,b,a) );
  }
};

// This is the second special case bindAofN(F,bind_fourth,a)(b,c)(d).
 template <typename R,typename F,typename A,typename B, typename C>
   struct binderAofNHelper<Bind4,R,F,A,B,C>
{
  static inline R go( const F& f, const A& a, const B& b,const C& c)
  {
    return makeFullN(binder1and2and4ofN<F,B,C,A>(f,b,c,a) );
  }
};

template <typename R,typename F,typename A,typename B,typename C,typename D,
          typename E,typename... Args>
  struct binderAofNHelper<Bind5,R,F,A,B,C,D,E,Args...>
{
  static inline R go( const F& f, const A& a, const B& b, const C&c, const D& d,
                      const E& e, const Args&... args)
  {
    return f(b,c,d,e,a,args...);
  }
};

// This is the first special case bindAofN(F,bind_fifth,a)(b)(c,d,e).
 template <typename R,typename F,typename A,typename B>
   struct binderAofNHelper<Bind5,R,F,A,B>
{
  static inline R go( const F& f, const A& a, const B& b)
  {
    return makeFullN(binder1and5ofN<F,B,A>(f,b,a) );
  }
};

// This is the second special case bindAofN(F,bind_fifth,a)(b,c)(d,e).
 template <typename R,typename F,typename A,typename B, typename C>
   struct binderAofNHelper<Bind5,R,F,A,B,C>
{
  static inline R go( const F& f, const A& a, const B& b,const C& c)
  {
    return makeFullN(binder1and2and5ofN<F,B,C,A>(f,b,c,a) );
  }
};

// This is the third special case bindAofN(F,bind_fifth,a)(b,c,d)(e).
 template <typename R,typename F,typename A,typename B, typename C,typename D>
   struct binderAofNHelper<Bind5,R,F,A,B,C,D>
{
  static inline R go( const F& f, const A& a, const B& b,const C& c,const D& d)
  {
    return makeFullN(binder1and2and3and5ofN<F,B,C,D,A>(f,b,c,d,a) );
  }
};

template <class Nary, typename Binder,typename Arg1>
class binderAofN  : public SmartFunctoidN
{
   const Nary f;
   const TR1::tuple<Binder,Arg1> argA;
public:
   typedef typename  TR1::tuple<Binder,Arg1> tupleType;
public:
     binderAofN( const Nary& g,const  Binder& binder , const Arg1& a ) : f(g),
     argA(TR1::make_tuple(binder,a)) {}

// This does not work as the number of template args is wrong
// when it is used.
//    binderAofN( const Nary& g, TR1::tuple<Binder,Arg1> binder_a ) : f(g),
//     argA(binder_a) {}


     template <typename Bind, typename... XYZ>
     struct SigN;

     template <typename... XYZ>
     struct  SigN<Bind1,XYZ...>
       {
          typedef typename RT<Nary,
          typename TR1::tuple_element<1,tupleType>::type,
	    XYZ...>::ResultType ResultType;
       };

     template <typename B,typename... XYZ>
       struct  SigN<Bind2,B,XYZ...>
       {
	 typedef typename RT<Nary,B,
          typename TR1::tuple_element<1,tupleType>::type,
	    XYZ...>::ResultType ResultType;
       };

     // This is the special case bindAofN(F,bind_third,a)(b)(c)
     // There is no XYZ as there are not enough arguments
     // on the second call to give a type for C, so the case
     // below it fails.
     template <typename B>
       struct  SigN<Bind3,B>
       {
	 typedef FullN<binder1and3ofN<Nary,B,
	   typename TR1::tuple_element<1,tupleType>::type> > ResultType;
       };

     template <typename B,typename C,typename... XYZ>
       struct  SigN<Bind3,B,C,XYZ...>
       {
	 typedef typename RT<Nary,B,C,
          typename TR1::tuple_element<1,tupleType>::type,
	    XYZ...>::ResultType ResultType;
       };

     // This is the special case bindAofN(F,bind_fourth,a)(b)(c,d)
     // There is no XYZ as there are not enough arguments
     // on the second call to give a type for C or D, so the case
     // below it fails.
     // NOTE: The case bindAofN(F,bind_fourth,a)(b,c)(d)
     // will have a similar problem and is not yet handled.
     // The same applies even more so for bind_fifth.
     // Each new case needs a BinderAofNHelper as well as a Sig.
     template <typename B>
       struct  SigN<Bind4,B>
       {
	 typedef FullN<binder1and4ofN<Nary,B,
	   typename TR1::tuple_element<1,tupleType>::type> > ResultType;
       };

     template <typename B,typename C>
       struct  SigN<Bind4,B,C>
       {
	 typedef FullN<binder1and2and4ofN<Nary,B,C,
	   typename TR1::tuple_element<1,tupleType>::type> > ResultType;
       };

     template <typename B,typename C,typename D,typename... XYZ>
       struct  SigN<Bind4,B,C,D,XYZ...>
       {
	 typedef typename RT<Nary,B,C,D,
          typename TR1::tuple_element<1,tupleType>::type,
	    XYZ...>::ResultType ResultType;
       };

     template <typename B>
       struct  SigN<Bind5,B>
       {
	 typedef FullN<binder1and5ofN<Nary,B,
	   typename TR1::tuple_element<1,tupleType>::type> > ResultType;
       };

     template <typename B,typename C>
       struct  SigN<Bind5,B,C>
       {
	 typedef FullN<binder1and2and5ofN<Nary,B,C,
	   typename TR1::tuple_element<1,tupleType>::type> > ResultType;
       };

     template <typename B,typename C,typename D>
       struct  SigN<Bind5,B,C,D>
       {
	 typedef FullN<binder1and2and3and5ofN<Nary,B,C,D,
	   typename TR1::tuple_element<1,tupleType>::type> > ResultType;
       };

     template <typename B,typename C,typename D,typename E,typename... XYZ>
       struct  SigN<Bind5,B,C,D,E,XYZ...>
       {
	 typedef typename RT<Nary,B,C,D,E,
          typename TR1::tuple_element<1,tupleType>::type,
	    XYZ...>::ResultType ResultType;
       };

     // This Sig was incorrect and has been specialised to get the
     // type of the bound argument into the correct place in the sequence.
     // This has been done using the same method as in binderAofNHelper above.
   template <typename... XYZ>
     struct Sig : public SigN<Binder,XYZ...>
     //   public FunType<XYZ..., typename RT<Nary,Arg1,XYZ...>::ResultType >
     {
       typedef Void Arg1Type;
       typedef Void Arg2Type;
       typedef Void Arg3Type;
       typedef Void Arg4Type;
     };

   // There is a case here which was not being handled.
   // This is where the call is one where the second argument list
   // is not long enough to fill the gap left after the first call.
   //
   // e.g. bindAofN(variadic_fcpp::plusN,bind_third,3)(1)(2).
   //
   // This needs a way realise that there is a problem and to call
   // the next binder. This was not solved until now.
   // One idea is to have a separate operator() for some cases, starting
   // with single argument calls and then recognize different cases via
   // helper functions as is done in Full in FC++.
   //
   // Sketch:
   //
   // template <typename X>
   // typename Sig<X>::ResultType // handle result type issues there.
   // operator()(const X& x)
   // {
   //     if binder > 2 then process special case
   //       else do the normal case
   // }
   //
   // The answer is to define a special Sig and also a special version
   // of binderAofNHelper to cover this case. The case is then handled
   // through the same operator().
   //
   // It could be extended to cover more cases, but will also need special
   // binders.
   // Bindings for 4 would need binder1and4ofN and binders1and2and4ofN which
   // do not exist at present.
   //
   template <typename... XYZ>
     typename Sig<XYZ...>::ResultType operator()(const XYZ&... xyz) const
   {
     return binderAofNHelper<
            typename TR1::tuple_element<0,tupleType>::type,
       //            typename RT<Nary,Arg1,XYZ...>::ResultType,Nary,
            typename Sig<XYZ...>::ResultType,Nary,
            typename TR1::tuple_element<1,tupleType>::type,XYZ...>::go
     (f,TR1::get<1>(argA),xyz...);
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Nary>() ) {
       std::string temp = std::string("binderAofN::")
                        + debug_traits_name(f);
       if (TR1::get<0>(argA).n > 0)
          temp += std::string("(Arg") + argument_traits_value(TR1::get<0>(argA).n);
       temp += std::string(",") + argument_traits_value(TR1::get<1>(argA)) + std::string(")");
       return temp;
     } else
       return std::string("binderAofN");
    }
#endif
};

///////////////////////////////////////////////////////////////
// The next idea - binderXofN, based on the work in FC++ 1.5.2
// The number of arguments is reduced by putting the binder
// into a std::pair with the argument.
///////////////////////////////////////////////////////////////

template <class Nary, typename Pair>
class binderXofN  : public SmartFunctoidN
{
   const Nary f;
   Pair p;
   // These give the types for later
   typedef typename Pair::first_type Binder;
   typedef typename Pair::second_type ArgA;
   //   const TR1::tuple<Binder,Arg1> argA;
public:
   typedef Pair pairType; // Could just use Pair...
   //typedef typename  std::pair<Binder,ArgA> pairType; // Could just use Pair...
   typedef typename  TR1::tuple<Binder,ArgA> tupleType;
public:
 binderXofN( const Nary& g,const Pair& q) : f(g), p(q)  { }
   // Make explicit copy constructor which is needed
   // under new rules for C++11
   // No - not needed now that the bugs are fixed.
   /*binderXofN( const binderXofN& binder) : f(binder.f) {
     p = fcpp::makePair(binder.p.first,binder.p.second);
     } */
     template <typename Bind, typename... XYZ>
     struct SigN;

     template <typename... XYZ>
     struct  SigN<Bind1,XYZ...>
       {
          typedef typename RT<Nary,
          typename TR1::tuple_element<1,tupleType>::type,
	    XYZ...>::ResultType ResultType;
       };

     template <typename B,typename... XYZ>
       struct  SigN<Bind2,B,XYZ...>
       {
	 typedef typename RT<Nary,B,
          typename TR1::tuple_element<1,tupleType>::type,
	    XYZ...>::ResultType ResultType;
       };

     // This is the special case bindXofN(F,std::make_pair(bind_third,a))(b)(c)
     // There is no XYZ as there are not enough arguments
     // on the second call to give a type for C, so the case
     // below fails.
     template <typename B>
       struct  SigN<Bind3,B>
       {
	 typedef FullN<binder1and3ofN<Nary,B,
	   typename TR1::tuple_element<1,tupleType>::type> > ResultType;
       };

     template <typename B,typename C,typename... XYZ>
       struct  SigN<Bind3,B,C,XYZ...>
       {
	 typedef typename RT<Nary,B,C,
          typename TR1::tuple_element<1,tupleType>::type,
	    XYZ...>::ResultType ResultType;
       };

     template <typename B>
       struct  SigN<Bind4,B>
       {
	 typedef FullN<binder1and4ofN<Nary,B,
	   typename TR1::tuple_element<1,tupleType>::type> > ResultType;
       };

     template <typename B,typename C,typename D,typename... XYZ>
       struct  SigN<Bind4,B,C,D,XYZ...>
       {
	 typedef typename RT<Nary,B,C,D,
          typename TR1::tuple_element<1,tupleType>::type,
	    XYZ...>::ResultType ResultType;
       };

     template <typename B>
       struct  SigN<Bind5,B>
       {
	 typedef FullN<binder1and5ofN<Nary,B,
	   typename TR1::tuple_element<1,tupleType>::type> > ResultType;
       };

     template <typename B,typename C,typename D,typename E,typename... XYZ>
       struct  SigN<Bind5,B,C,D,E,XYZ...>
       {
	 typedef typename RT<Nary,B,C,D,E,
          typename TR1::tuple_element<1,tupleType>::type,
	    XYZ...>::ResultType ResultType;
       };

   template <typename... XYZ>
     struct Sig : public SigN<Binder,XYZ...>
     //   public FunType<XYZ..., typename RT<Nary,Arg1,XYZ...>::ResultType >
     {
       typedef Nary Arg1Type;
       typedef Pair Arg2Type;
       typedef Void Arg3Type;
       typedef Void Arg4Type;
     };

   template <typename... XYZ>
     typename Sig<XYZ...>::ResultType operator()(const XYZ&... xyz) const
   {
     return binderAofNHelper<Binder,
       //            typename RT<Nary,Arg1,XYZ...>::ResultType,Nary,
            typename Sig<XYZ...>::ResultType,Nary,ArgA,XYZ...>::go
                                  (f,p.second,xyz...);
   }

#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Nary>() ) {
       std::string temp = std::string("binderXofN::")
                        + debug_traits_name(f);
       if (p.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(p.first.n);
       temp += std::string(",") + debug_name_or_value(p.second)
            + std::string(")");
       return temp;
     } else
       return std::string("binderXofN");
    }
#endif
};

////////////////////////////////////////////////////////////////////////
// Attempt to build FullN on these foundations.
////////////////////////////////////////////////////////////////////////

template <class F>
class FullN : public SmartFunctoidN
{
   F f;
public:
   FullN() : f() {}
   FullN( const F& ff ) : f(ff) {}
  ~FullN() { }

   // This merges FullN with FC++ monads.
#ifdef FCPP_ENABLE_LAMBDA
   typedef FullN This;
   // For the moment make an explicit use of fcpp::
   template <class A> typename fcpp::fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
     { return fcpp::fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif

   /* For the time being I have put in these explicit unwrappings
      of the argument types. I expect these can be removed when
      FC++ is changed to not require them.
      I am now using ArgN for this.
   */
   //    template <int N, typename... Types> struct SigN;

#ifdef FCPP_BOOST_TYPE
   template <typename... XYZ>
   struct Sig : public fun_type<typename RT<F,XYZ...>::ResultType> {};
#else
   // The previous problem with this was the lack of typename before RT.
   template <typename... XYZ>
     struct Sig : public ArgN<sizeof...(XYZ),XYZ...>
   /* :  public FunType<XYZ..., typename RT<F,XYZ...>::ResultType > */
   {
     typedef typename RT<F,XYZ...>::ResultType ResultType;
   };
#endif
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Currying for FullN
// Every pattern needs a Sig entry and also a helper function.
// These need to have the same signature and lead to an appropriate binder.
// Examples of use of currying and the binders are in fcppideas9.cpp
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   // Thinking to set up currying,
   // but will this be distinguishable from FullN with 1 arg?
   // The problem is to distinguish e.g. PlusN(1) from PlusN(1)(2)
   // I can now do it with the explicit binder e.g. bind1ofN(plusN,1)(2)
   // The solution is to use a trailing object to show more to come.
   // e.g. plusN(1,_abc)(2) which is implemented here for 1 and 2 arguments.
#ifdef FCPP_CONCEPTS_LITE
   // Making this live messes up CurryableNHelper<2,R,F,X,Y> calls.
   // I have not been able to sort out why this happens.
   // I think it is picking up this return type incorrectly...
   // It picks it up even when the type here does not exist (Rubbish).
   //template <class X,is_rubbish T>
   template <class X,is_more_to_come T>
   struct Sig<X,T> : public FunType<X,FullN<binder1ofN<F,X> > > {
      typedef X Arg1Type;
      typedef T Arg2Type;
      typedef Void Arg3Type;
      typedef Void Arg4Type;
   };

   /*
   template <class X,class Y>
     struct Sig<X,Y> : public RT<F,X,Y> {
      typedef X Arg1Type;
      typedef Y Arg2Type;
      typedef Void Arg3Type;
      typedef Void Arg4Type;
   };
   */
#else
   template <class X>
   struct Sig<X,MoreToCome> : public FunType<X,FullN<binder1ofN<F,X> > > {
      typedef X Arg1Type;
      typedef MoreToCome Arg2Type;
      typedef Void Arg3Type;
      typedef Void Arg4Type;
   };
#endif

   template <class Y>
     struct Sig<OneMoreHere,Y> :
     public FunType<Bind2,Y,FullN<binderAofN<F,Bind2,Y> > > {
     typedef OneMoreHere Arg1Type;
     typedef Y Arg2Type;
     typedef Void Arg3Type;
     typedef Void Arg4Type;
   };

   template <class Y>
     struct Sig<OneMoreHere,Y,MoreToCome> :
     public FunType<Bind2,Y,FullN<binderAofN<F,Bind2,Y> > > {
     typedef OneMoreHere Arg1Type;
     typedef Y Arg2Type;
     typedef MoreToCome Arg3Type;
     typedef Void Arg4Type;
   };
   /*
   template <class Y>
     struct Sig<fcpp::AutoCurryType,Y> :
     public FunType<Bind2,Y,FullN<binderAofN<F,Bind2,Y> > > {
      typedef Y Arg2Type;
      };
   */
   template <class Z>
     struct Sig<OneMoreHere,OneMoreHere,Z> :
     public FunType<Bind3,Z,FullN<binderAofN<F,Bind3,Z> > > {};

   template <class U>
     struct Sig<OneMoreHere,OneMoreHere,OneMoreHere,U> :
     public FunType<Bind4,U,FullN<binderAofN<F,Bind4,U> > > {};

   template <class V>
     struct Sig<OneMoreHere,OneMoreHere,OneMoreHere,OneMoreHere,V> :
     public FunType<Bind5,V,FullN<binderAofN<F,Bind5,V> > > {};

   template <class X, class Y>
     struct Sig<X,Y,MoreToCome> : public FunType<X,Y,FullN<binder1and2ofN<F,X,Y> > > {};

   template <class X, class Z>
     struct Sig<X,OneMoreHere,Z,MoreToCome> : public FunType<X,Z,FullN<binder1and3ofN<F,X,Z> > > {};

   template <class X, class Y, class Z>
     struct Sig<X,Y,Z,MoreToCome> :
     public FunType<X,Y,Z,FullN<binder1and2and3ofN<F,X,Y,Z> > > {};

   template <class W,class X, class Y, class Z>
     struct Sig<W,X,Y,Z,MoreToCome> :
     public FunType<W,X,Y,Z,FullN<binderMofN<F,W,X,Y,Z> > > {};

   template <class V,class W,class X, class Y, class Z>
     struct Sig<V,W,X,Y,Z,MoreToCome> :
     public FunType<V,W,X,Y,Z,FullN<binderMofN<F,V,W,X,Y,Z> > > {};

/* Cannot generalise like this - parameter pack must be at the end of the list.
   template <typename... XYZ>
     struct Sig<XYZ...,MoreToCome> :
     public FunType<XYZ...,FullN<binderMofN<F,XYZ...> > > {};
 */

   template <class X, class Z>
     struct Sig<X,OneMoreHere,Z> : public FunType<X,Z,FullN<binder1and3ofN<F,X,Z> > > {};

   template <class X, class U>
     struct Sig<X,OneMoreHere,OneMoreHere,U> :
     public FunType<X,U,FullN<binder1and4ofN<F,X,U> > > {};

   template <class X, class V>
     struct Sig<X,OneMoreHere,OneMoreHere,OneMoreHere,V> :
     public FunType<X,V,FullN<binder1and5ofN<F,X,V> > > {};

   template <class Y, class Z>
     struct Sig<OneMoreHere,Y,Z> : public FunType<Y,Z,FullN<binder2and3ofN<F,Y,Z> > > {};

   template <class Y, class Z>
     struct Sig<OneMoreHere,Y,OneMoreHere,Z> : public FunType<Y,Z,FullN<binder2and4ofN<F,Y,Z> > > {};

   template <class Y, class Z>
     struct Sig<OneMoreHere,OneMoreHere,Y,Z> : public FunType<Y,Z,FullN<binder3and4ofN<F,Y,Z> > > {};

   template <class X, class Y,class U>
     struct Sig<X,Y,OneMoreHere,U> : public FunType<X,Y,U,FullN<binder1and2and4ofN<F,X,Y,U> > > {};

   template <class X, class Y,class V>
     struct Sig<X,Y,OneMoreHere,OneMoreHere,V> : public FunType<X,Y,V,FullN<binder1and2and5ofN<F,X,Y,V> > > {};

   template <class X, class Z,class U>
     struct Sig<X,OneMoreHere,Z,U> : public FunType<X,Z,U,FullN<binder1and3and4ofN<F,X,Z,U> > > {};

   template <class X, class Z, class V>
     struct Sig<X,OneMoreHere,Z,OneMoreHere,V> : 
     public FunType<X,Z,V,FullN<binder1and3and5ofN<F,X,Z,V> > > {};

   template <class X, class U, class V>
     struct Sig<X,OneMoreHere,OneMoreHere,U,V> : 
     public FunType<X,U,V,FullN<binder1and4and5ofN<F,X,U,V> > > {};

   template <class Y, class Z,class U>
     struct Sig<OneMoreHere,Y,Z,U> : public FunType<Y,Z,U,FullN<binder2and3and4ofN<F,Y,Z,U> > > {};

   template <class X, class Y,class Z,class V>
     struct Sig<X,Y,Z,OneMoreHere,V> : public FunType<X,Y,Z,V,FullN<binder1and2and3and5ofN<F,X,Y,Z,V> > > {};

   template <class Y,class Z,class U,class V>
     struct Sig<OneMoreHere,Y,Z,U,V> : public FunType<Y,Z,U,V,FullN<binder2and3and4and5ofN<F,Y,Z,U,V> > > {};

   template <class Y,class Z,class U,class V,class W>
     struct Sig<OneMoreHere,Y,Z,U,V,W> : 
     public FunType<Y,Z,U,V,W,FullN<binder2and3and4and5and6ofN<F,Y,Z,U,V,W> > > {};

    /* also for experiments. - see below.
   template <typename... Args>
     struct Sig<MoreToCome,Args...> :
     public FunType<Args...,FullN<binder1and2and3ofN<F,Args...> > > {};
     */

   /*  Taken out as it does not work properly like this.
   template <typename... Args>
     struct Sig<MoreToCome,Args...> :
     public FunType<Args...,FullN<binderMofN<F,Args...> > > {};
   */

   // These examples use concepts to identify the type.
   // This does not for these cases add much, as the concept
   // identifies a single type.
   // These cases are straight alternatives to the code to do
   // exactly the same function below.
   // It does show how the concepts lite ideas can be used.
   // The effect is to move the testing for the type to the template
   // from the specialization.
   // The type is determined by a property which could be held
   // by more than one type.
   // The resulting code is more abstract and compact.
#if (defined (FCPP_CONCEPTS_LITE) || defined(FCPP_TICK) )
#ifdef FCPP_CONCEPTS_LITE
   template <class X,is_more_to_come T>
   typename Sig<X,T>::ResultType operator()
      ( const X& x, const T& ) const
   {
     //std::cout << "makeFullN(binder1ofN<F,X>(f,x) ) called" << std::endl;
      return makeFullN(binder1ofN<F,X>(f,x) );
   }

   template <fcpp::is_auto_curry T,class Y>
     typename Sig<T,Y>::ResultType operator()
      ( const T& , const Y& y) const
   {
     return makeFullN(binderAofN<F,Bind2,Y>(f,bind_second,y) );
   }

   template <fcpp::is_auto_curry X,class Y,is_more_to_come Z>
     typename Sig<X,Y,Z>::ResultType operator()
     ( const X& , const Y& y,const Z& ) const
   {
     //std::cout << "makeFullN(binder1ofN<F,X>(f,x) ) called" << std::endl;
     return makeFullN(binderAofN<F,Bind2,Y>(f,bind_second,y));
   }

   template <fcpp::is_auto_curry T,class Z>
     typename Sig<T,T,Z>::ResultType operator()
      ( const T& , const T& , const Z& z) const
   {
     return makeFullN(binderAofN<F,Bind3,Z>(f,bind_third,z) );
   }

   template <fcpp::is_auto_curry T,typename A, typename B>
     typename Sig<A,T,T,B>::ResultType operator()
     ( const A& a, const T&,const T&, const B&b) const
   {
     return makeFullN(binder1and4ofN<F,A,B>(f,a,b) );
   }

   template <fcpp::is_auto_curry T,typename A, typename B>
     typename Sig<T,A,B>::ResultType operator()
     ( const T& ,const A& a, const B&b) const
   {
     return makeFullN(binder2and3ofN<F,A,B>(f,a,b) );
   }

   template <fcpp::is_auto_curry T,class U>
     typename Sig<T,T,T,U>::ResultType operator()
      (  const T&, const T&, const T&, const U& u) const
   {
     return makeFullN(binderAofN<F,Bind4,U>(f,bind_fourth,u) );
   }

   template <fcpp::is_auto_curry T,class V>
     typename Sig<T,T,T,T,V>::ResultType operator()
      (  const T&, const T&, const T&, const T&, const V& v) const
   {
     return makeFullN(binderAofN<F,Bind5,V>(f,bind_fifth,v) );
   }

   template <class X, class Y,is_more_to_come T>
     typename Sig<X,Y,T>::ResultType operator()
     ( const X& x, const Y& y,const T& ) const
   {
     return makeFullN(binder1and2ofN<F,X,Y>(f,x,y) );
   }

   template <class X, class Y, class Z,is_more_to_come T>
     typename Sig<X,Y,Z,T>::ResultType operator()
     ( const X& x, const Y& y,const Z& z,const T& ) const
   {
     return makeFullN(binder1and2and3ofN<F,X,Y,Z>(f,x,y,z) );
   }

   template <typename A,fcpp::is_auto_curry T, typename B>
     typename Sig<A,T,B>::ResultType operator()
     ( const A& a, const T& , const B&b) const
   {
     return makeFullN(binder1and3ofN<F,A,B>(f,a,b) );
   }

   // New non-FCPP_TICK code
   template <fcpp::is_auto_curry T, typename A, typename B>
     typename Sig<T,A,T,B>::ResultType operator()
     (const T&, const A& a, const T&, const B&b) const
   {
     return makeFullN(binder2and4ofN<F,A,B>(f,a,b) );
   }

   // New non-FCPP_TICK code
   template <fcpp::is_auto_curry T, typename A, typename B>
     typename Sig<T,T,A,B>::ResultType operator()
     (const T&, const T&, const A& a, const B&b) const
   {
     return makeFullN(binder3and4ofN<F,A,B>(f,a,b) );
   }

   template <class W,class X, class Y, class Z,is_more_to_come T>
     typename Sig<W,X,Y,Z,T>::ResultType operator()
     ( const W& w,const X& x, const Y& y,const Z& z,const T& ) const
   {
     return makeFullN(binderMofN<F,W,X,Y,Z>(f,w,x,y,z) );
   }

#else // FCPP_TICK
   // There are now some things defined here not yet done for concepts_lite.
   // First use of FCPP_TICK in VFC++.
   // These are taken in preference to this:
   // operator()( const XYZ&... xyz ) const {
   // Testing on two different concepts, one for auto curry
   // and one for more to come.
   // In some cases now there have to be tests of both concepts.
   // I find this out by resolving ambiguity which shows in compilation failure.
   // This works with clang 3.6 and gcc 6.0.1
   template <class X,class Y,TICK_REQUIRES(!is_auto_curry<X>() and !is_more_to_come<X>() and is_more_to_come<Y>())>
   typename Sig<X,Y>::ResultType operator()
      ( const X& x, const Y& ) const
   {
     //std::cout << "makeFullN(binder1ofN<F,X>(f,x) ) called" << std::endl;
      return makeFullN(binder1ofN<F,X>(f,x) );
   }

   template <class X,class Y,class Z,TICK_REQUIRES(!is_auto_curry<X>() and !is_more_to_come<X>() and !is_more_to_come<Y>() and is_more_to_come<Z>())>
     typename Sig<X,Y,Z>::ResultType operator()
     ( const X& x, const Y& y,const Z& ) const
   {
     //std::cout << "makeFullN(binder1ofN<F,X>(f,x) ) called" << std::endl;
     return makeFullN(binder1and2ofN<F,X,Y>(f,x,y) );
   }

   template <class X, class Y, class Z,class M,TICK_REQUIRES(!is_auto_curry<X>() and !is_more_to_come<X>() and !is_more_to_come<Y>() and !is_more_to_come<Z>() and is_more_to_come<M>())>
     typename Sig<X,Y,Z,M>::ResultType operator()
     ( const X& x, const Y& y,const Z& z,const M& ) const
   {
     return makeFullN(binder1and2and3ofN<F,X,Y,Z>(f,x,y,z) );
   }

   template <class X,class Y,class Z,TICK_REQUIRES(is_auto_curry<X>() and !is_more_to_come<Y>() and is_more_to_come<Z>())>
     typename Sig<X,Y,Z>::ResultType operator()
     ( const X& , const Y& y,const Z& ) const
   {
     //std::cout << "makeFullN(binder1ofN<F,X>(f,x) ) called" << std::endl;
     return makeFullN(binderAofN<F,Bind2,Y>(f,bind_second,y));
   }

   template <class X,class Y,TICK_REQUIRES(is_auto_curry<X>() and !is_auto_curry<Y>())>
     typename Sig<X,Y>::ResultType operator()
      ( const X&, const Y& y) const
   {
     //std::cout << "makeFullN(binderAofN<F,Bind2,Y>(f,bind_second,y) ) called"
     //          << std::endl;
     return makeFullN(binderAofN<F,Bind2,Y>(f,bind_second,y) );
   }

   template <class X,class Z,TICK_REQUIRES(is_auto_curry<X>() and !is_auto_curry<Z>())>
     typename Sig<X,X,Z>::ResultType operator()
      ( const X&, const X&, const Z& z) const
   {
     //std::cout << "makeFullN(binderAofN<F,Bind3,Y>(f,bind_third,z) ) called"
     //          << std::endl;
     return makeFullN(binderAofN<F,Bind3,Z>(f,bind_third,z) );
   }

   template <typename X, typename Y, typename Z,TICK_REQUIRES(!is_auto_curry<X>()and is_auto_curry<Y>() and !is_auto_curry<Z>())>
     typename Sig<X,Y,Z>::ResultType operator()
     ( const X& x, const Y&, const Z& z) const
   {
     return makeFullN(binder1and3ofN<F,X,Z>(f,x,z) );
   }

   template <typename X, typename Y, typename Z,TICK_REQUIRES(is_auto_curry<X>()and !is_auto_curry<Y>() and !is_auto_curry<Z>() and !is_more_to_come<Z>())>
     typename Sig<X,Y,Z>::ResultType operator()
     ( const X&, const Y& y, const Z& z) const
   {
     return makeFullN(binder2and3ofN<F,Y,Z>(f,y,z) );
   }

   //This case was ambiguous with a number of other cases not yet done for
   //concepts lite, the other ones with 4 arguments and
   //one or more autocurry arguments. These have been disabled for FCPP_TICK
   //and adapted versions have now been installed here.
   //This has been extended to cover some cases not previously available.
   template <class X, class U,TICK_REQUIRES(is_auto_curry<X>() and !is_auto_curry<U>())>
     typename Sig<X,X,X,U>::ResultType operator()
      (  const X&, const X&, const X&, const U& u) const
   {
     return makeFullN(binderAofN<F,Bind4,U>(f,bind_fourth,u) );
   }

   // This case is ambiguous with a lot of 5 argument cases.
   // These have been disabled below for FCPP_TICK and if needed
   // have to be adapted and installed for FCPP_TICK.
   // This is leading to fairly large expansion of the requirements.
   template <class X,class V,TICK_REQUIRES(is_auto_curry<X>() and !is_auto_curry<V>())>
     typename Sig<X,X,X,X,V>::ResultType operator()
      (  const X&, const X&, const X&, const X&, const V& v) const
   {
     return makeFullN(binderAofN<F,Bind5,V>(f,bind_fifth,v) );
   }

   template <typename X, typename Y,typename Z, typename U,TICK_REQUIRES(is_auto_curry<X>()and !is_auto_curry<Y>() and !is_auto_curry<Z>() and !is_auto_curry<U>())>
     typename Sig<X,Y,Z,U>::ResultType operator()
     ( const X& , const Y& y, const Z& z, const U& u) const
   {
     return makeFullN(binder2and3and4ofN<F,Y,Z,U>(f,y,z,u) );
   }

   template <typename X, typename Y,typename Z, typename U,TICK_REQUIRES(!is_auto_curry<X>()and is_auto_curry<Y>() and !is_auto_curry<Z>() and !is_auto_curry<U>())>
     typename Sig<X,Y,Z,U>::ResultType operator()
     ( const X& x, const Y& , const Z& z, const U& u) const
   {
     return makeFullN(binder1and3and4ofN<F,X,Z,U>(f,x,z,u) );
   }

   template <typename X, typename Y,typename Z, typename U,TICK_REQUIRES(!is_auto_curry<X>()and !is_auto_curry<Y>() and is_auto_curry<Z>() and !is_auto_curry<U>())>
     typename Sig<X,Y,Z,U>::ResultType operator()
     ( const X& x, const Y& y, const Z&, const U& u) const
   {
     return makeFullN(binder1and2and4ofN<F,X,Y,U>(f,x,y,u) );
   }

   template <typename X, typename Y,typename Z, typename U,TICK_REQUIRES(!is_auto_curry<X>()and is_auto_curry<Y>() and is_auto_curry<Z>() and !is_auto_curry<U>())>
     typename Sig<X,Y,Z,U>::ResultType operator()
     ( const X& x, const Y&, const Z&, const U& u) const
   {
     return makeFullN(binder1and4ofN<F,X,U>(f,x,u) );
   }

   // This is a new binder.
   template <typename X, typename Y,typename Z, typename U,TICK_REQUIRES(is_auto_curry<X>()and !is_auto_curry<Y>() and is_auto_curry<Z>() and !is_auto_curry<U>())>
     typename Sig<X,Y,Z,U>::ResultType operator()
     ( const X&, const Y& y, const Z&, const U& u) const
   {
     return makeFullN(binder2and4ofN<F,Y,U>(f,y,u) );
   }

   // This is a new binder.
   template <typename X, typename Y,typename Z, typename U,TICK_REQUIRES(is_auto_curry<X>() and is_auto_curry<Y>() and !is_auto_curry<Z>() and !is_auto_curry<U>())>
     typename Sig<X,Y,Z,U>::ResultType operator()
     ( const X&, const Y&, const Z& z, const U& u) const
   {
     return makeFullN(binder3and4ofN<F,Z,U>(f,z,u) );
   }

   template <class W,class X, class Y, class Z, class M,TICK_REQUIRES(!is_auto_curry<W>() and !is_more_to_come<W>()  and !is_more_to_come<X>() and !is_more_to_come<Y>() and !is_more_to_come<Z>() and is_more_to_come<M>())>
     typename Sig<W,X,Y,Z,M>::ResultType operator()
     ( const W& w,const X& x, const Y& y,const Z& z,const M& ) const
   {
     return makeFullN(binderMofN<F,W,X,Y,Z>(f,w,x,y,z) );
   }

#endif
#else
   // This is the code without both FCPP_CONCEPTS_LITE and FCPP_TICK

   template <class X>
   typename Sig<X,MoreToCome>::ResultType operator()
      ( const X& x, const MoreToCome&  ) const
   {
     //std::cout << "makeFullN(binder1ofN<F,X>(f,x) ) called" << std::endl;
      return makeFullN(binder1ofN<F,X>(f,x) );
   }

   template <class Y>
     typename Sig<OneMoreHere,Y>::ResultType operator()
      ( const OneMoreHere& , const Y& y) const
   {
     return makeFullN(binderAofN<F,Bind2,Y>(f,bind_second,y) );
   }
   /*
   template <class Y>
     typename Sig<fcpp::AutoCurryType,Y>::ResultType operator()
     ( const fcpp::AutoCurryType& , const Y& y) const
   {
     return makeFullN(binderAofN<F,Bind2,Y>(f,bind_second,y) );
   }
   */
   template <class Z>
     typename Sig<OneMoreHere,OneMoreHere,Z>::ResultType operator()
      ( const OneMoreHere&, const OneMoreHere& , const Z& z) const
   {
     return makeFullN(binderAofN<F,Bind3,Z>(f,bind_third,z) );
   }

   template <class U>
     typename Sig<OneMoreHere,OneMoreHere,OneMoreHere,U>::ResultType operator()
      (  const OneMoreHere&, const OneMoreHere&, const OneMoreHere&, const U& u) const
   {
     return makeFullN(binderAofN<F,Bind4,U>(f,bind_fourth,u) );
   }

   template <class V>
     typename Sig<OneMoreHere,OneMoreHere,OneMoreHere,OneMoreHere,V>::ResultType operator()
      (  const OneMoreHere&, const OneMoreHere&, const OneMoreHere&, const OneMoreHere&, const V& v) const
   {
     return makeFullN(binderAofN<F,Bind5,V>(f,bind_fifth,v) );
   }

   template <class X, class Y>
     typename Sig<X,Y,MoreToCome>::ResultType operator()
     ( const X& x, const Y& y,const MoreToCome& m ) const
   {
     return makeFullN(binder1and2ofN<F,X,Y>(f,x,y) );
   }

   template <typename A, typename B>
     typename Sig<A,OneMoreHere,B>::ResultType operator()
     ( const A& a, const OneMoreHere& m, const B&b) const
   {
     return makeFullN(binder1and3ofN<F,A,B>(f,a,b) );
   }

   template <typename A, typename B>
     typename Sig<OneMoreHere,A,B>::ResultType operator()
     ( const OneMoreHere& m,const A& a, const B&b) const
   {
     return makeFullN(binder2and3ofN<F,A,B>(f,a,b) );
   }

   template <class Y>
     typename Sig<OneMoreHere,Y,MoreToCome>::ResultType operator()
     ( const OneMoreHere& , const Y& y, const MoreToCome& ) const
   {
     //std::cout << "makeFullN(binder1ofN<F,X>(f,x) ) called" << std::endl;
     return makeFullN(binderAofN<F,Bind2,Y>(f,bind_second,y));
   }


   template <typename A, typename B>
     typename Sig<A,OneMoreHere,B,MoreToCome>::ResultType operator()
     ( const A& a, const OneMoreHere&, const B&b, const MoreToCome& ) const
   {
     return makeFullN(binder1and3ofN<F,A,B>(f,a,b) );
   }

   template <typename A, typename B>
     typename Sig<A,OneMoreHere,OneMoreHere,B>::ResultType operator()
     ( const A& a, const OneMoreHere&,const OneMoreHere&, const B&b) const
   {
     return makeFullN(binder1and4ofN<F,A,B>(f,a,b) );
   }

   // New non-FCPP_TICK code
   template <typename A, typename B>
     typename Sig<OneMoreHere,A,OneMoreHere,B>::ResultType operator()
     (const OneMoreHere&, const A& a,const OneMoreHere&, const B&b) const
   {
     return makeFullN(binder2and4ofN<F,A,B>(f,a,b) );
   }

   // New non-FCPP_TICK code
   template <typename A, typename B>
     typename Sig<OneMoreHere,OneMoreHere,A,B>::ResultType operator()
     (const OneMoreHere&, const OneMoreHere&, const A& a, const B&b) const
   {
     return makeFullN(binder3and4ofN<F,A,B>(f,a,b) );
   }

   template <class X, class Y, class Z>
     typename Sig<X,Y,Z,MoreToCome>::ResultType operator()
     ( const X& x, const Y& y,const Z& z,const MoreToCome& m ) const
   {
     return makeFullN(binder1and2and3ofN<F,X,Y,Z>(f,x,y,z) );
   }

   template <class W,class X, class Y, class Z>
     typename Sig<W,X,Y,Z,MoreToCome>::ResultType operator()
     ( const W& w,const X& x, const Y& y,const Z& z,const MoreToCome& m ) const
   {
     return makeFullN(binderMofN<F,W,X,Y,Z>(f,w,x,y,z) );
   }

   // End of code without both FCPP_CONCEPTS_LITE and FCPP_TICK
#endif
   // This code is used in all cases at present.

   template <class V,class W,class X, class Y, class Z>
     typename Sig<V,W,X,Y,Z,MoreToCome>::ResultType operator()
     ( const V& v,const W& w,const X& x, const Y& y,const Z& z,const MoreToCome& m ) const
   {
     return makeFullN(binderMofN<F,V,W,X,Y,Z>(f,v,w,x,y,z) );
   }

/* This does not work. Parameter packs must be at the end of the list.
   template <typename... XYZ>
     typename Sig<XYZ...,MoreToCome>::ResultType operator()
     ( const XYZ&... xyz,const MoreToCome& m ) const
   {
     return makeFullN(binderMofN<F,XYZ...>(f,xyz) );
   }
*/
#ifndef FCPP_TICK
   // All this code is used with concepts_lite.
   // If concepts_lite versions are made, the corresponding code
   // here needs to be moved up to the section for code executed
   // without both FCPP_CONCEPTS_LITE and FCPP_TICK

   template <typename A, typename B>
     typename Sig<A,OneMoreHere,OneMoreHere,OneMoreHere,B>::ResultType operator()
     ( const A& a, const OneMoreHere&,const OneMoreHere&,const OneMoreHere&, const B&b) const
   {
     return makeFullN(binder1and5ofN<F,A,B>(f,a,b) );
   }

   template <typename A, typename B,typename C>
     typename Sig<A,B,OneMoreHere,C>::ResultType operator()
     ( const A& a, const B&b, const OneMoreHere& m, const C&c) const
   {
     return makeFullN(binder1and2and4ofN<F,A,B,C>(f,a,b,c) );
   }

   template <typename A, typename B,typename C>
     typename Sig<A,B,OneMoreHere,OneMoreHere,C>::ResultType operator()
     ( const A& a, const B&b, const OneMoreHere& , const OneMoreHere& , const C&c) const
   {
     return makeFullN(binder1and2and5ofN<F,A,B,C>(f,a,b,c) );
   }

   template <typename A, typename B,typename C>
     typename Sig<A,OneMoreHere,B,C>::ResultType operator()
     ( const A& a, const OneMoreHere& m, const B&b, const C&c) const
   {
     return makeFullN(binder1and3and4ofN<F,A,B,C>(f,a,b,c) );
   }

   template <typename A, typename B,typename C>
     typename Sig<A,OneMoreHere,B,OneMoreHere,C>::ResultType operator()
     ( const A& a, const OneMoreHere& , const B&b,const OneMoreHere& , const C&c) const
   {
     return makeFullN(binder1and3and5ofN<F,A,B,C>(f,a,b,c) );
   }

   template <typename A, typename B,typename C>
     typename Sig<A,OneMoreHere,OneMoreHere,B,C>::ResultType operator()
     ( const A& a, const OneMoreHere& , const OneMoreHere& , const B&b, const C&c) const
   {
     return makeFullN(binder1and4and5ofN<F,A,B,C>(f,a,b,c) );
   }

   template <typename A, typename B,typename C>
     typename Sig<OneMoreHere,A,B,C>::ResultType operator()
     ( const OneMoreHere& m, const A&a, const B&b, const C&c) const
   {
     return makeFullN(binder2and3and4ofN<F,A,B,C>(f,a,b,c) );
   }
   template <typename A, typename B,typename C, typename D>
     typename Sig<A,B,C,OneMoreHere,D>::ResultType operator()
     ( const A& a, const B&b, const C&c, const OneMoreHere& , const D&d) const
   {
     return makeFullN(binder1and2and3and5ofN<F,A,B,C,D>(f,a,b,c,d) );
   }

   template <typename A, typename B,typename C, typename D>
     typename Sig<OneMoreHere,A,B,C,D>::ResultType operator()
     ( const OneMoreHere& ,const A& a, const B&b, const C&c,  const D&d) const
   {
     return makeFullN(binder2and3and4and5ofN<F,A,B,C,D>(f,a,b,c,d) );
   }
#endif

   template <typename A, typename B,typename C, typename D, typename E>
     typename Sig<OneMoreHere,A,B,C,D,E>::ResultType operator()
     ( const OneMoreHere& ,const A& a, const B&b, const C&c,  const D&d, const E&e) const
   {
     return makeFullN(binder2and3and4and5and6ofN<F,A,B,C,D,E>(f,a,b,c,d,e) );
   }

#ifdef FCPP_USE_BOOST_LAMBDA
   // Special for boost operations
   // This will only work for up to three placeholders because of the limit
   // in boost lambda.
   template <class T> struct sig
   {
     typedef typename boost::tuples::element<1, T>::type type;
   };
#endif


   template <typename... XYZ>
   // I can now go back to trusting the Sig as it now includes the return type.
   // inline typename Sig<XYZ...>::ResultType
   //inline typename Result<XYZ...>::Type
   // I am now using RT for the return type everywhere and not the Sig.
   // This works and needs some further thought.
   // Both now work and this is one level less of decoding....
   //Change back for FCPP_CXX11
   // The RT with argument types gets things wrong for curried cases.
#ifndef FCPP_CXX11
    inline typename RT<F,XYZ...>::ResultType
#else
     //inline typename RT<F,XYZ...>::ResultType
    inline typename Sig<XYZ...>::ResultType
#endif
   operator()( const XYZ&... xyz ) const {
     // Attempt to define for use of tr1::bind.
     // typedef typename Sig<XYZ...>::ResultType result_type;
     //std::cout << "FullN go()" << std::endl;
     return CurryableNHelper<sizeof... (XYZ),
     //  typename RT<F,XYZ...>::ResultType,F,XYZ...>::go(f,xyz...);
     // Change here from Sig to RT.
     // changed back for consistency although RT works here. Why?
#ifndef FCPP_CXX11
     typename RT<F,XYZ...>::ResultType,F,XYZ...>::go(f,xyz...);
#else
     //typename RT<F,XYZ...>::ResultType,F,XYZ...>::go(f,xyz...);
     typename Sig<XYZ...>::ResultType,F,XYZ...>::go(f,xyz...);
#endif
   }

#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("FullN::",f,"FullN");
   }
#endif
};

// Moved earlier.
//template <class F> FullN<F> makeFullN( const F& f ) { return FullN<F>(f); }

////////////////////////////////////////////////////////////////////////
// Attempt to build FullNR on these foundations.
// This is like FullN with explict return type.
// This is now used in association with FunN.
// There is now a separate ptr_to_funN.
////////////////////////////////////////////////////////////////////////

template <class F,class ResultType>
class FullNR : public SmartFunctoidN
{
   F f;
public:
   FullNR() : f() {}
   FullNR( const F& ff ) : f(ff) {}

// This  renders unnecessary the set of specialisations of SigN.
#ifdef FCPP_BOOST_TYPE
   template <typename... XYZ>
   struct Sig : public fun_type<ResultType> {};
#else
   template <typename... XYZ>
     struct Sig : public FunType<XYZ..., ResultType> {};
#endif

   template <typename... XYZ>
   inline ResultType
   operator()( const XYZ&... xyz ) const {
     //std::cout << "FullNR go()" << std::endl;
     return CurryableNHelper<sizeof... (XYZ),
     ResultType,F,XYZ...>::go(f,xyz...);
   }
   // Defined for use with tr1::bind.
   typedef ResultType result_type;
};


template <class F,class R> FullNR<F,R> makeFullNR( const F& f )
{ return FullNR<F,R>(f); }

////////////////////////////////////////////////////////////////////////
// ptr_to_funN.
// The strategy is different from FullNR.
// It decodes the no of arguments and calls the corresponding
// part of FC++ ptr_to_fun.
////////////////////////////////////////////////////////////////////////

// Note: This is needed for the default constructor in FunN.
// This changed and it now does something different - see FunNGuts.

// As a result I am not sure what use this is.

// The only reason for using this would be if there is a variadic function
// and I want to take a pointer to it.
// In all other cases use ptr_to_fun from FC++.

// This now uses the appropriate cases from ptr_to_fun.
// I have not dealt with the const case.

struct PtrToFunN  : public SmartFunctoidN {

  // Originally I selected SigN on sizeof(Types...).
  // The selection is now based on mapping of the function type.

  //#ifndef FCPP_USE_SIGNR
  template <typename F> struct SigN;

  template <typename R>
  struct SigN<R (*)()> : public FunType<R (*)(),
  Full0<impl::Xptr_to_nullary_function<R> > > {};

  template <typename R,typename A1>
    struct SigN<R (*)(A1)> : public FunType<R (*)(A1),
    Full1<impl::Xptr_to_unary_function<A1,R> > > {};

  template <typename R,typename A1,typename A2>
    struct SigN<R (*)(A1,A2)> : public FunType<R (*)(A1,A2),
    Full2<impl::Xptr_to_binary_function<A1,A2,R> > > {};

  template <typename R,typename A1,typename A2, typename A3>
    struct SigN<R (*)(A1,A2,A3)> : public FunType<R (*)(A1,A2,A3),
    Full3<impl::Xptr_to_ternary_function<A1,A2,A3,R> > > {};

#ifdef FCPP_USE_SIGNR
  // These only work with HelperN

  template <typename R,typename A1>
    struct SigN<R (A1::*)()> : public FunType<R (A1::*)(),
    Full1<impl::Xptr_to_mem_nullary_function<A1,R> > > {};

  template <typename R,typename A1>
    struct SigN<R (A1::*)() const> : public FunType<R (A1::*)() const,
    Full1<impl::Xptr_to_const_mem_nullary_function<A1,R> > > {};

  template <typename R,typename A1,typename A2>
    struct SigN<R (A1::*)(A2)> : public FunType<R (A1::*)(A2),
    Full2<impl::Xptr_to_mem_unary_function<A1,A2,R> > > {};

  template <typename R,typename A1,typename A2>
    struct SigN<R (A1::*)(A2) const> : public FunType<R (A1::*)(A2) const,
    Full2<impl::Xptr_to_const_mem_unary_function<A1,A2,R> > > {};

  template <typename R,typename A1,typename A2,typename A3>
    struct SigN<R (A1::*)(A2,A3)> : public FunType<R (A1::*)(A2,A3),
    Full3<impl::Xptr_to_mem_binary_function<A1,A2,A3,R> > > {};

  template <typename R,typename A1,typename A2,typename A3>
  struct SigN<R (A1::*)(A2,A3) const > :
    public FunType<R (A1::*)(A2,A3) const,
    Full3<impl::Xptr_to_const_mem_binary_function<A1,A2,A3,R> > > {};

  template <typename R,typename A1,typename A2,typename A3,typename A4>
    struct SigN<R (A1::*)(A2,A3,A4)> : public FunType<R (A1::*)(A2,A3,A4),
    Full4<impl::Xptr_to_mem_ternary_function<A1,A2,A3,A4,R> > > {};

  template <typename R,typename A1,typename A2,typename A3,typename A4>
    struct SigN<R (A1::*)(A2,A3,A4) const> :
    public FunType<R (A1::*)(A2,A3,A4) const,
    Full4<impl::Xptr_to_const_mem_ternary_function<A1,A2,A3,A4,R> > > {};

  template <typename R,typename A1,typename A2,typename A3,typename A4,
                       typename A5>
    struct SigN<R (A1::*)(A2,A3,A4,A5)> : public FunType<R (A1::*)(A2,A3,A4,A5),
    Full5<impl::Xptr_to_mem_quaternary_function<A1,A2,A3,A4,A5,R> > > {};

  template <typename R,typename A1,typename A2,typename A3,typename A4,
                       typename A5>
    struct SigN<R (A1::*)(A2,A3,A4,A5) const> :
    public FunType<R (A1::*)(A2,A3,A4,A5) const,
    Full5<impl::Xptr_to_const_mem_quaternary_function<A1,A2,A3,A4,A5,R> > > {};

  template <typename R,typename A1,typename A2,typename A3,typename A4,
    typename A5, typename A6>
    struct SigN<R (A1::*)(A2,A3,A4,A5,A6)> :
    public FunType<R (A1::*)(A2,A3,A4,A5,A6),
    Full6<impl::Xptr_to_mem_quintinary_function<A1,A2,A3,A4,A5,A6,R> > > {};

  template <typename R,typename A1,typename A2,typename A3,typename A4,
    typename A5, typename A6>
    struct SigN<R (A1::*)(A2,A3,A4,A5,A6) const> :
    public FunType<R (A1::*)(A2,A3,A4,A5,A6) const,
    Full6<impl::Xptr_to_const_mem_quintinary_function<A1,A2,A3,A4,A5,A6,R> > >
    {};

  template <typename R,typename A1,typename A2,typename A3,typename A4,
    typename A5, typename A6, typename A7>
    struct SigN<R (A1::*)(A2,A3,A4,A5,A6,A7) > :
    public FunType<R (A1::*)(A2,A3,A4,A5,A6,A7),
    Full7<impl::Xptr_to_mem_sextinary_function<A1,A2,A3,A4,A5,A6,A7,R> > >
    {};

  template <typename R,typename A1,typename A2,typename A3,typename A4,
    typename A5, typename A6, typename A7>
    struct SigN<R (A1::*)(A2,A3,A4,A5,A6,A7) const> :
    public FunType<R (A1::*)(A2,A3,A4,A5,A6,A7) const,
    Full7<impl::Xptr_to_const_mem_sextinary_function<A1,A2,A3,A4,A5,A6,A7,R> > >
    {};

#endif

  template <typename F>
    struct Sig : public SigN<F> {};

#ifndef FCPP_USE_SIGNR

  template <int N,typename F,typename R,typename... Types> struct Helper;

  template <typename F,typename R,typename... Types>
    struct Helper<0,F,R,Types...> {
    static inline Full0<impl::Xptr_to_nullary_function<R> >
       go(const F& f) {
       return makeFull0( impl::Xptr_to_nullary_function<R>(f) );
       }
    };

  template <typename F,typename R,typename A1>
    struct Helper<1,F,R,A1> {
    static inline Full1<impl::Xptr_to_unary_function<A1,R> >
       go(const F& f) {
       return makeFull1( impl::Xptr_to_unary_function<A1,R>(f) );
       }
    };
 
  template <typename F,typename R,typename A1, typename A2>
    struct Helper<2,F,R,A1,A2> {
    static inline Full2<impl::Xptr_to_binary_function<A1,A2,R> >
      go(const F& f) {
      return makeFull2( impl::Xptr_to_binary_function<A1,A2,R>(f) );
       }
    };

  template <typename F,typename R,typename A1, typename A2,typename A3>
    struct Helper<3,F,R,A1,A2,A3> {
    static inline Full3<impl::Xptr_to_ternary_function<A1,A2,A3,R> >
      go(const F& f) {
      return makeFull3( impl::Xptr_to_ternary_function<A1,A2,A3,R>(f) );
       }
    };

#else

  template <typename F> struct HelperN;

    template <typename R>
      struct HelperN<R (*)()> {
    static inline Full0<impl::Xptr_to_nullary_function<R> >
       go(R (*f)() ) {
       return makeFull0( impl::Xptr_to_nullary_function<R>(f) );
       }
    };

    template <typename R,typename A1>
      struct HelperN<R (*)(A1)> {
    static inline Full1<impl::Xptr_to_unary_function<A1,R> >
       go(R (*f)(A1) ) {
       return makeFull1( impl::Xptr_to_unary_function<A1,R>(f) );
       }
    };
 
    template <typename R,typename A1, typename A2>
      struct HelperN<R (*)(A1,A2)> {
    static inline Full2<impl::Xptr_to_binary_function<A1,A2,R> >
      go(R (*f)(A1,A2) ) {
      return makeFull2( impl::Xptr_to_binary_function<A1,A2,R>(f) );
       }
    };

    template <typename R,typename A1, typename A2,typename A3>
      struct HelperN<R (*)(A1,A2,A3)> {
    static inline Full3<impl::Xptr_to_ternary_function<A1,A2,A3,R> >
      go(R (*f)(A1,A2,A3) ) {
      return makeFull3( impl::Xptr_to_ternary_function<A1,A2,A3,R>(f) );
       }
    };

    // Member functions
    template <typename R,typename A1>
      struct HelperN<R (A1::*)()> {
    static inline Full1<impl::Xptr_to_mem_nullary_function<A1,R> >
      go(R (A1::*f)() ) {
       return makeFull1( impl::Xptr_to_mem_nullary_function<A1,R>(f) );
       }
    };

    template <typename R,typename A1>
      struct HelperN<R (A1::*)() const> {
    static inline Full1<impl::Xptr_to_const_mem_nullary_function<A1,R> >
      go(R (A1::*f)() const ) {
       return makeFull1( impl::Xptr_to_const_mem_nullary_function<A1,R>(f) );
       }
    };

    template <typename R,typename A1,typename A2>
      struct HelperN<R (A1::*)(A2)> {
      static inline Full2<impl::Xptr_to_mem_unary_function<A1,A2,R> >
      go(R (A1::*f)(A2) ) {
	return makeFull2( impl::Xptr_to_mem_unary_function<A1,A2,R>(f) );
       }
    };

    template <typename R,typename A1,typename A2>
      struct HelperN<R (A1::*)(A2) const> {
      static inline Full2<impl::Xptr_to_const_mem_unary_function<A1,A2,R> >
      go(R (A1::*f)(A2) const) {
	return makeFull2( impl::Xptr_to_const_mem_unary_function<A1,A2,R>(f) );
       }
    };

    template <typename R,typename A1,typename A2,typename A3>
      struct HelperN<R (A1::*)(A2,A3)> {
      static inline Full3<impl::Xptr_to_mem_binary_function<A1,A2,A3,R> >
	go(R (A1::*f)(A2,A3) ) {
	return makeFull3( impl::Xptr_to_mem_binary_function<A1,A2,A3,R>(f) );
       }
    };

    template <typename R,typename A1,typename A2,typename A3>
      struct HelperN<R (A1::*)(A2,A3) const> {
      static inline Full3<impl::Xptr_to_const_mem_binary_function<A1,A2,A3,R> >
	go(R (A1::*f)(A2,A3) const ) {
	return makeFull3( impl::Xptr_to_const_mem_binary_function<A1,A2,A3,R>(f) );
       }
    };

    template <typename R,typename A1,typename A2,typename A3,typename A4>
      struct HelperN<R (A1::*)(A2,A3,A4)> {
      static inline Full4<impl::Xptr_to_mem_ternary_function<A1,A2,A3,A4,R> >
	go(R (A1::*f)(A2,A3,A4) ) {
	return makeFull4( impl::Xptr_to_mem_ternary_function<A1,A2,A3,A4,R>(f));
       }
    };

    template <typename R,typename A1,typename A2,typename A3,typename A4>
      struct HelperN<R (A1::*)(A2,A3,A4) const> {
      static inline Full4<
        impl::Xptr_to_const_mem_ternary_function<A1,A2,A3,A4,R> >
	go(R (A1::*f)(A2,A3,A4) const ) {
	return makeFull4(
        impl::Xptr_to_const_mem_ternary_function<A1,A2,A3,A4,R>(f));
       }
    };

    template <typename R,typename A1,typename A2,typename A3,typename A4,
                         typename A5>
      struct HelperN<R (A1::*)(A2,A3,A4,A5)> {
      static inline Full5<
        impl::Xptr_to_mem_quaternary_function<A1,A2,A3,A4,A5,R> >
	go(R (A1::*f)(A2,A3,A4,A5) ) {
          return makeFull5(
          impl::Xptr_to_mem_quaternary_function<A1,A2,A3,A4,A5,R>(f));
        }
    };

    template <typename R,typename A1,typename A2,typename A3,typename A4,
                         typename A5>
      struct HelperN<R (A1::*)(A2,A3,A4,A5) const> {
     static inline Full5<
       impl::Xptr_to_const_mem_quaternary_function<A1,A2,A3,A4,A5,R> >
       go(R (A1::*f)(A2,A3,A4,A5) const ) {
          return makeFull5(
          impl::Xptr_to_const_mem_quaternary_function<A1,A2,A3,A4,A5,R>(f));
       }
    };

    template <typename R,typename A1,typename A2,typename A3,typename A4,
      typename A5,typename A6>
      struct HelperN<R (A1::*)(A2,A3,A4,A5,A6)> {
      static inline Full6<
        impl::Xptr_to_mem_quintinary_function<A1,A2,A3,A4,A5,A6,R> >
	go(R (A1::*f)(A2,A3,A4,A5,A6) ) {
	   return makeFull6(
           impl::Xptr_to_mem_quintinary_function<A1,A2,A3,A4,A5,A6,R>(f));
       }
    };

    template <typename R,typename A1,typename A2,typename A3,typename A4,
      typename A5,typename A6>
      struct HelperN<R (A1::*)(A2,A3,A4,A5,A6) const> {
      static inline Full6<
        impl::Xptr_to_const_mem_quintinary_function<A1,A2,A3,A4,A5,A6,R> >
	go(R (A1::*f)(A2,A3,A4,A5,A6) const ) {
	  return makeFull6(
          impl::Xptr_to_const_mem_quintinary_function<A1,A2,A3,A4,A5,A6,R>(f));
       }
    };

    template <typename R,typename A1,typename A2,typename A3,typename A4,
      typename A5,typename A6,typename A7>
      struct HelperN<R (A1::*)(A2,A3,A4,A5,A6,A7)> {
      static inline Full7<
        impl::Xptr_to_mem_sextinary_function<A1,A2,A3,A4,A5,A6,A7,R> >
	go(R (A1::*f)(A2,A3,A4,A5,A6,A7) ) {
	   return makeFull7(
           impl::Xptr_to_mem_sextinary_function<A1,A2,A3,A4,A5,A6,A7,R>(f));
       }
    };

    template <typename R,typename A1,typename A2,typename A3,typename A4,
      typename A5,typename A6,typename A7>
      struct HelperN<R (A1::*)(A2,A3,A4,A5,A6,A7) const> {
      static inline Full7<
        impl::Xptr_to_const_mem_sextinary_function<A1,A2,A3,A4,A5,A6,A7,R> >
	go(R (A1::*f)(A2,A3,A4,A5,A6,A7) const ) {
	   return makeFull7(
           impl::Xptr_to_const_mem_sextinary_function<A1,A2,A3,A4,A5,A6,A7,R>(f));
       }
    };

#endif

#ifndef FCPP_USE_SIGNR
  // Note that the Sig decodes the number of arguments via SigN.
  template <typename R,typename... Types >
  typename Sig<R(*)(Types...)>::ResultType operator() (R (*f)(Types...)) const
  {
    return Helper<sizeof...(Types),R (*)(Types...),R,Types...>::go(f);
  }

#else
    template <typename F>
      typename Sig<F>::ResultType operator() (const F& f) const
  {
    return HelperN<F>::go(f);
  }
#endif

  static FullN<PtrToFunN>& full() {static FullN<PtrToFunN> f; return f;}

#ifdef FCPP_DEBUG
   std::string name() const {
     return std::string("ptr_to_funN");
   }
#endif
 };

static FullN<PtrToFunN>& ptr_to_funN=PtrToFunN::full();


////////////////////////////////////////////////////////////////////////
// The binders using FullN are defined before FullN
// The functoids are defined here using FullN.
////////////////////////////////////////////////////////////////////////

/*
These functoids bind the first 1 or 2 arguments and return an Nary object
which can have a variable number of arguments.
*/

struct Bind1ofN : public SmartFunctoidN
{
   template <class Nary, class Arg1>
   struct Sig : public FunType< Nary, Arg1, FullN<binder1ofN<Nary,Arg1> > > {};

   template <class Nary, class Arg1>
   inline FullN<binder1ofN<Nary,Arg1> >
     operator()( const Nary& f, const Arg1& x) const {
     return makeFullN( binder1ofN<Nary,Arg1>(f,x) );
   }

   static FullN<Bind1ofN>& full() {static FullN<Bind1ofN> f; return f;}

#ifdef FCPP_DEBUG
   std::string name() const {
     return std::string("Bind1ofN");
   }
#endif
};

static FullN<Bind1ofN>& bind1ofN=Bind1ofN::full();

struct Bind1and2ofN : public SmartFunctoidN
{
  template <class Nary, class Arg1, class Arg2>
    struct Sig : public FunType< Nary, Arg1, Arg2, FullN<binder1and2ofN<Nary,Arg1,Arg2> > > {};

  template <class Nary, class Arg1, class Arg2>
    inline FullN<binder1and2ofN<Nary,Arg1,Arg2> >
    operator()( const Nary& f, const Arg1& x, const Arg2& y) const {
    return makeFullN( binder1and2ofN<Nary,Arg1,Arg2>(f,x,y) );
   }

   static FullN<Bind1and2ofN>& full() {static FullN<Bind1and2ofN> f; return f;}

};

static FullN<Bind1and2ofN>& bind1and2ofN=Bind1and2ofN::full();

struct Bind1and3ofN : public SmartFunctoidN
{
  template <class Nary, class Arg1, class Arg3>
    struct Sig : public FunType< Nary, Arg1, Arg3, FullN<binder1and3ofN<Nary,Arg1,Arg3> > > {};

  template <class Nary, class Arg1, class Arg3>
    inline FullN<binder1and3ofN<Nary,Arg1,Arg3> >
    operator()( const Nary& f, const Arg1& x, const Arg3& z) const {
    return makeFullN( binder1and3ofN<Nary,Arg1,Arg3>(f,x,z) );
   }

   static FullN<Bind1and3ofN>& full() {static FullN<Bind1and3ofN> f; return f;}

};

static FullN<Bind1and3ofN>& bind1and3ofN=Bind1and3ofN::full();

struct Bind2and3ofN : public SmartFunctoidN
{
  template <class Nary, class Arg2, class Arg3>
    struct Sig : public FunType< Nary, Arg2, Arg3, FullN<binder2and3ofN<Nary,Arg2,Arg3> > > {};

  template <class Nary, class Arg2, class Arg3>
    inline FullN<binder2and3ofN<Nary,Arg2,Arg3> >
    operator()( const Nary& f, const Arg2& y, const Arg3& z) const {
    return makeFullN( binder2and3ofN<Nary,Arg2,Arg3>(f,y,z) );
   }

   static FullN<Bind2and3ofN>& full() {static FullN<Bind2and3ofN> f; return f;}

};

static FullN<Bind2and3ofN>& bind2and3ofN=Bind2and3ofN::full();

struct Bind1and4ofN : public SmartFunctoidN
{
  template <class Nary, class Arg1, class Arg4>
    struct Sig : public FunType< Nary, Arg1, Arg4, FullN<binder1and4ofN<Nary,Arg1,Arg4> > > {};

  template <class Nary, class Arg1, class Arg4>
    inline FullN<binder1and4ofN<Nary,Arg1,Arg4> >
    operator()( const Nary& f, const Arg1& x, const Arg4& u) const {
    return makeFullN( binder1and4ofN<Nary,Arg1,Arg4>(f,x,u) );
   }

   static FullN<Bind1and4ofN>& full() {static FullN<Bind1and4ofN> f; return f;}

};

static FullN<Bind1and4ofN>& bind1and4ofN=Bind1and4ofN::full();

struct Bind1and5ofN : public SmartFunctoidN
{
  template <class Nary, class Arg1, class Arg5>
    struct Sig : public FunType< Nary, Arg1, Arg5, FullN<binder1and5ofN<Nary,Arg1,Arg5> > > {};

  template <class Nary, class Arg1, class Arg5>
    inline FullN<binder1and5ofN<Nary,Arg1,Arg5> >
    operator()( const Nary& f, const Arg1& x, const Arg5& v) const {
    //std::cout << "Bind1and5ofN operator() called." << std::endl;
    return makeFullN( binder1and5ofN<Nary,Arg1,Arg5>(f,x,v) );
   }

   static FullN<Bind1and5ofN>& full() {static FullN<Bind1and5ofN> f; return f;}

};

static FullN<Bind1and5ofN>& bind1and5ofN=Bind1and5ofN::full();

struct Bind2and4ofN : public SmartFunctoidN
{
  template <class Nary, class Arg2, class Arg4>
    struct Sig : public FunType< Nary, Arg2, Arg4, FullN<binder2and4ofN<Nary,Arg2,Arg4> > > {};

  template <class Nary, class Arg2, class Arg4>
    inline FullN<binder2and3ofN<Nary,Arg2,Arg4> >
    operator()( const Nary& f, const Arg2& y, const Arg4& z) const {
        //std::cout << "Bind2and3ofN operator() called." << std::endl;
    return makeFullN( binder2and4ofN<Nary,Arg2,Arg4>(f,y,z) );
   }

   static FullN<Bind2and4ofN>& full() {static FullN<Bind2and4ofN> f; return f;}

};

static FullN<Bind2and4ofN>& bind2and4ofN=Bind2and4ofN::full();

struct Bind3and4ofN : public SmartFunctoidN
{
  template <class Nary, class Arg3, class Arg4>
    struct Sig : public FunType< Nary, Arg3, Arg4, FullN<binder3and4ofN<Nary,Arg3,Arg4> > > {};

  template <class Nary, class Arg3, class Arg4>
    inline FullN<binder3and4ofN<Nary,Arg3,Arg4> >
    operator()( const Nary& f, const Arg3& y, const Arg4& z) const {
        //std::cout << "Bind2and3ofN operator() called." << std::endl;
    return makeFullN( binder3and4ofN<Nary,Arg3,Arg4>(f,y,z) );
   }

   static FullN<Bind3and4ofN>& full() {static FullN<Bind3and4ofN> f; return f;}

};

static FullN<Bind3and4ofN>& bind3and4ofN=Bind3and4ofN::full();

struct Bind1and2and3ofN : public SmartFunctoidN
{
  template <class Nary, class Arg1, class Arg2, class Arg3>
    struct Sig : public FunType< Nary, Arg1, Arg2, Arg3,
    FullN<binder1and2and3ofN<Nary,Arg1,Arg2,Arg3> > > {};

  template <class Nary, class Arg1, class Arg2, class Arg3>
    inline FullN<binder1and2and3ofN<Nary,Arg1,Arg2,Arg3> >
    operator()( const Nary& f, const Arg1& x, const Arg2& y, const Arg3& z) const {
    return makeFullN( binder1and2and3ofN<Nary,Arg1,Arg2,Arg3>(f,x,y,z) );
   }

   static FullN<Bind1and2and3ofN>& full() {static FullN<Bind1and2and3ofN> f; return f;}

};

static FullN<Bind1and2and3ofN>& bind1and2and3ofN=Bind1and2and3ofN::full();

struct Bind1and2and4ofN : public SmartFunctoidN
{
  template <class Nary, class Arg1, class Arg2, class Arg4>
    struct Sig : public FunType< Nary, Arg1, Arg2, Arg4,
    FullN<binder1and2and4ofN<Nary,Arg1,Arg2,Arg4> > > {};

  template <class Nary, class Arg1, class Arg2, class Arg4>
    inline FullN<binder1and2and4ofN<Nary,Arg1,Arg2,Arg4> >
    operator()( const Nary& f, const Arg1& x, const Arg2& y, const Arg4& u) const {
    return makeFullN( binder1and2and4ofN<Nary,Arg1,Arg2,Arg4>(f,x,y,u) );
   }

   static FullN<Bind1and2and4ofN>& full() {static FullN<Bind1and2and4ofN> f; return f;}

};

static FullN<Bind1and2and4ofN>& bind1and2and4ofN=Bind1and2and4ofN::full();


struct Bind1and2and5ofN : public SmartFunctoidN
{
  template <class Nary, class Arg1, class Arg2, class Arg5>
    struct Sig : public FunType< Nary, Arg1, Arg2, Arg5,
    FullN<binder1and2and5ofN<Nary,Arg1,Arg2,Arg5> > > {};

  template <class Nary, class Arg1, class Arg2, class Arg5>
    inline FullN<binder1and2and5ofN<Nary,Arg1,Arg2,Arg5> >
    operator()( const Nary& f, const Arg1& x, const Arg2& y, const Arg5& v) const {
    //std::cout << "Bind1and2and5ofN operator() called." << std::endl;
    return makeFullN( binder1and2and5ofN<Nary,Arg1,Arg2,Arg5>(f,x,y,v) );
   }

   static FullN<Bind1and2and5ofN>& full() {static FullN<Bind1and2and5ofN> f; return f;}

};

static FullN<Bind1and2and5ofN>& bind1and2and5ofN=Bind1and2and5ofN::full();

struct Bind1and3and4ofN : public SmartFunctoidN
{
  template <class Nary, class Arg1, class Arg3, class Arg4>
    struct Sig : public FunType< Nary, Arg1, Arg3, Arg4,
    FullN<binder1and3and4ofN<Nary,Arg1,Arg3,Arg4> > > {};

  template <class Nary, class Arg1, class Arg3, class Arg4>
    inline FullN<binder1and3and4ofN<Nary,Arg1,Arg3,Arg4> >
    operator()( const Nary& f, const Arg1& x, const Arg3& z, const Arg4& u) const {
    return makeFullN( binder1and3and4ofN<Nary,Arg1,Arg3,Arg4>(f,x,z,u) );
   }

   static FullN<Bind1and3and4ofN>& full() {static FullN<Bind1and3and4ofN> f; return f;}

};

static FullN<Bind1and3and4ofN>& bind1and3and4ofN=Bind1and3and4ofN::full();

struct Bind1and3and5ofN : public SmartFunctoidN
{
  template <class Nary, class Arg1, class Arg3, class Arg5>
    struct Sig : public FunType< Nary, Arg1, Arg3, Arg5,
    FullN<binder1and3and5ofN<Nary,Arg1,Arg3,Arg5> > > {};

  template <class Nary, class Arg1, class Arg3, class Arg5>
    inline FullN<binder1and3and5ofN<Nary,Arg1,Arg3,Arg5> >
    operator()( const Nary& f, const Arg1& x, const Arg3& z, const Arg5& v) const {
    return makeFullN( binder1and3and5ofN<Nary,Arg1,Arg3,Arg5>(f,x,z,v) );
   }

   static FullN<Bind1and3and5ofN>& full() {static FullN<Bind1and3and5ofN> f; return f;}

};

static FullN<Bind1and3and5ofN>& bind1and3and5ofN=Bind1and3and5ofN::full();

struct Bind1and4and5ofN : public SmartFunctoidN
{
  template <class Nary, class Arg1, class Arg4, class Arg5>
    struct Sig : public FunType< Nary, Arg1, Arg4, Arg5,
    FullN<binder1and4and5ofN<Nary,Arg1,Arg4,Arg5> > > {};

  template <class Nary, class Arg1, class Arg4, class Arg5>
    inline FullN<binder1and4and5ofN<Nary,Arg1,Arg4,Arg5> >
    operator()( const Nary& f, const Arg1& x, const Arg4& u, const Arg5& v) const {
    return makeFullN( binder1and4and5ofN<Nary,Arg1,Arg4,Arg5>(f,x,u,v) );
   }

   static FullN<Bind1and4and5ofN>& full() {static FullN<Bind1and4and5ofN> f; return f;}

};

static FullN<Bind1and4and5ofN>& bind1and4and5ofN=Bind1and4and5ofN::full();

struct Bind2and3and4ofN  : public SmartFunctoidN
{
  template <class Nary, class Arg2, class Arg3, class Arg4>
    struct Sig : public FunType< Nary, Arg2, Arg3, Arg4,
    FullN<binder2and3and4ofN<Nary,Arg2,Arg3,Arg4> > > {};

  template <class Nary, class Arg2, class Arg3, class Arg4>
    inline FullN<binder2and3and4ofN<Nary,Arg2,Arg3,Arg4> >
    operator()( const Nary& f, const Arg2& y, const Arg3& z, const Arg4& u) const {
    return makeFullN( binder2and3and4ofN<Nary,Arg2,Arg3,Arg4>(f,y,z,u) );
   }

   static FullN<Bind2and3and4ofN>& full() {static FullN<Bind2and3and4ofN> f; return f;}

};

static FullN<Bind2and3and4ofN>& bind2and3and4ofN=Bind2and3and4ofN::full();

struct Bind1and2and3and5ofN : public SmartFunctoidN
{
  template <class Nary, class Arg1, class Arg2, class Arg3, class Arg5>
    struct Sig : public FunType< Nary, Arg1, Arg2, Arg3, Arg5,
    FullN<binder1and2and3and5ofN<Nary,Arg1,Arg2,Arg3,Arg5> > > {};

  template <class Nary, class Arg1, class Arg2, class Arg3, class Arg5>
    inline FullN<binder1and2and3and5ofN<Nary,Arg1,Arg2,Arg3,Arg5> >
    operator()( const Nary& f, const Arg1& x, const Arg2& y, const Arg3& z, const Arg5& v) const {
    return makeFullN( binder1and2and3and5ofN<Nary,Arg1,Arg2,Arg3,Arg5>(f,x,y,z,v) );
   }

   static FullN<Bind1and2and3and5ofN>& full() {static FullN<Bind1and2and3and5ofN> f; return f;}

};

static FullN<Bind1and2and3and5ofN>& bind1and2and3and5ofN=Bind1and2and3and5ofN::full();

struct Bind2and3and4and5ofN : public SmartFunctoidN
{
  template <class Nary, class Arg2, class Arg3, class Arg4, class Arg5>
    struct Sig : public FunType< Nary, Arg2, Arg3, Arg4, Arg5,
    FullN<binder2and3and4and5ofN<Nary,Arg2,Arg3,Arg4,Arg5> > > {};

  template <class Nary, class Arg2, class Arg3, class Arg4, class Arg5>
    inline FullN<binder2and3and4and5ofN<Nary,Arg2,Arg3,Arg4,Arg5> >
    operator()( const Nary& f, const Arg2& y, const Arg3& z, const Arg4& u, const Arg5& v) const {
    return makeFullN( binder2and3and4and5ofN<Nary,Arg2,Arg3,Arg4,Arg5>(f,y,z,u,v) );
   }

   static FullN<Bind2and3and4and5ofN>& full() {static FullN<Bind2and3and4and5ofN> f; return f;}

};

static FullN<Bind2and3and4and5ofN>& bind2and3and4and5ofN=Bind2and3and4and5ofN::full();

struct Bind2and3and4and5and6ofN : public SmartFunctoidN
{
  template <class Nary, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
    struct Sig : public FunType< Nary, Arg2, Arg3, Arg4, Arg5, Arg6,
    FullN<binder2and3and4and5and6ofN<Nary,Arg2,Arg3,Arg4,Arg5,Arg6> > > {};

  template <class Nary, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
    inline FullN<binder2and3and4and5and6ofN<Nary,Arg2,Arg3,Arg4,Arg5,Arg6> >
    operator()( const Nary& f, const Arg2& y, const Arg3& z, const Arg4& u, const Arg5& v, const Arg6& w) const {
    return makeFullN( binder2and3and4and5and6ofN<Nary,Arg2,Arg3,Arg4,Arg5,Arg6>(f,y,z,u,v,w) );
   }

   static FullN<Bind2and3and4and5and6ofN>& full() {static FullN<Bind2and3and4and5and6ofN> f; return f;}

};

static FullN<Bind2and3and4and5and6ofN>& bind2and3and4and5and6ofN=Bind2and3and4and5and6ofN::full();

struct BindMofN : public SmartFunctoidN
{
  template <class Nary, typename... Args>
    struct Sig : public FunType< Nary, Args...,
    FullN<binderMofN<Nary,Args...> > > {};

  template <class Nary, typename... Args>
    inline FullN<binderMofN<Nary,Args...> >
    operator()( const Nary& f, const Args&... xyz) const {
    return makeFullN( binderMofN<Nary,Args...>(f,xyz...) );
   }

   static FullN<BindMofN>& full() {static FullN<BindMofN> f; return f;}

};

static FullN<BindMofN>& bindMofN=BindMofN::full();

struct BindAofN : public SmartFunctoidN
{
  template <class Nary, class Binder, class Arg1>
    struct Sig : public FunType< Nary, Arg1, FullN<binderAofN<Nary,Binder,Arg1> > > {};

  template <class Nary, class Binder, class Arg1>
     inline FullN<binderAofN<Nary,Binder,Arg1> >
     operator()( const Nary& f,const Binder& binder ,const Arg1& x) const {
    return makeFullN( binderAofN<Nary,Binder,Arg1>(f,binder,x) );
   }

   static FullN<BindAofN>& full() {static FullN<BindAofN> f; return f;}

};

static FullN<BindAofN>& bindAofN=BindAofN::full();

struct BindXofN : public SmartFunctoidN
{
  template <class Nary, class Pair>
    struct Sig : public FunType< Nary, Pair, FullN<binderXofN<Nary,Pair> > > {};

  template <class Nary, class Pair>
     inline FullN<binderXofN<Nary,Pair> >
     operator()( const Nary& f,const Pair &p) const {
    return makeFullN( binderXofN<Nary,Pair>(f,p) );
   }

   static FullN<BindXofN>& full() {static FullN<BindXofN> f; return f;}

};

static FullN<BindXofN>& bindXofN=BindXofN::full();

////////////////////////////////////////////////////////////////////////
// Attempt to build FunN on these foundations. (needs FullN)
////////////////////////////////////////////////////////////////////////
template <int N,typename Result,typename... Args>
struct FunNImpl;

template <typename Result,typename... Args>
struct FunN;

/*template <typename... TwoSetsOfTypes>
FunNImpl<typename FindFirstHalf<sizeof...(TwoSetsOfTypes),TwoSetsOfTypes...>::First,
         typename FindFirstHalf<sizeof...(TwoSetsOfTypes),TwoSetsOfTypes...>::Second,
	 typename FindFirstHalf<sizeof...(TwoSetsOfTypes),TwoSetsOfTypes...>::Third,
         typename FindFirstHalf<sizeof...(TwoSetsOfTypes),TwoSetsOfTypes...>::Fourth>*
convertN( const fcpp::IRef<const FunNImpl<
         typename FindSecondHalf<sizeof...(TwoSetsOfTypes),TwoSetsOfTypes...>::First,
         typename FindSecondHalf<sizeof...(TwoSetsOfTypes),TwoSetsOfTypes...>::Second,
	 typename FindSecondHalf<sizeof...(TwoSetsOfTypes),TwoSetsOfTypes...>::Third,
         typename FindSecondHalf<sizeof...(TwoSetsOfTypes),TwoSetsOfTypes...>::Fourth>
         >& f );
*/

template <int N,typename... TwoSetsOfTypes>
struct convertN;

/*template <typename R1,typename R2>
struct convertN<2>
{

  R1 operator()(
};
*/

  //  template <int N,typename Rd,typename DF,typename... Args>
  template <typename Rd,typename DF,typename... Args>
struct FunNConstructor;

// Definition of the special function for the default constructor to call.

 template<typename R,typename... Args>
  struct nullfunN
  {
    R operator()(const Args&... x) { return R(); }
  };

  nullfunN<int> nullfunNint;
  nullfunN<int,int> nullfunNintint;

/*template<int N,typename R,typename... Args>
  struct nullfunN
  {
    R operator()(const Args&... x) { return R(); }
  };
*/
// Comment from FC++
// The "Guts" class helps us implement currying; FunN floats gently atop
// FunNGuts and adds currying.

// Result is now explicit here.
template <typename Result,typename... Args>
  class FunNGuts : public FunArgs<Args...> /*, SmartFunctoidN */ {
  typedef fcpp::IRef<const FunNImpl<sizeof... (Args),Result,Args...> > RefImpl;
   RefImpl ref;
  template <typename R,typename... ABC> friend class FunNGuts;
  template <typename R,typename... ABC> friend class FunN;

  //template <int N,typename Rd, typename DF, typename... Ads>
  template <typename Rd, typename DF, typename... Ads>
   friend struct FunNConstructor;

  //   template <class A1d, class A2d, class A3d, class Rd,
  //           class A1s, class A2s, class A3s, class Rs>
  // friend Fun3<A1d,A2d,A3d,Rd>
  // explicit_convert3( const Fun3<A1s,A2s,A3s,Rs>& f );

  template <int N,typename... TwoSetsOfTypes>
  friend struct explicit_convertN;

  // Member implementation so that there is something concrete to call.
  nullfunN<Result,Args...> *nullfunNImpl;
public:
  typedef FunNImpl<sizeof...(Args),Result,Args...>* Impl;
   FunNGuts( int, Impl i ) : ref(i) {}

  template <typename... XYZ>
  inline Result operator()(const XYZ&... xyz) const
  {
    // std::cout << "FunNGuts operator()" << std::endl;
     return ref->operator()(xyz...);
  }

   template <class DF>
   FunNGuts( const DF& df )
     : ref( FunNConstructor<Result,DF,Args...>::make(df) ) {}

   FunNGuts( const FunNGuts& x ) : ref(x.ref) {}
   FunNGuts& operator=( const FunNGuts& x ) { ref = x.ref; return *this; }
   FunNGuts() {
     // Experimental and unsafe as it is not deleted.
     // Not in use as it does not work...
     nullfunNImpl = new nullfunN<Result,Args...>;
     FunNGuts  tmp =
       makeFullNR<Result(*),Result> ( (this->nullfunNImpl) );
       //makeFullNR ( &(nullfunNImpl) );
     ref = tmp.ref;
   }
   // added October 2009
   ~FunNGuts() {
       delete nullfunNImpl;
   }
};

// Result is now explicit here.
template <typename Result,typename... Args>
class FunN : public SmartFunctoidN
{
  template <typename Rd, typename DF,typename... Ads>
   friend struct FunNConstructor;

  //   template <class A1d, class A2d, class A3d, class Rd,
  //           class A1s, class A2s, class A3s, class Rs>
  // friend Fun3<A1d,A2d,A3d,Rd>
  // explicit_convert3( const Fun3<A1s,A2s,A3s,Rs>& f );

   FunNGuts<Result,Args...> rep;
public:
   typedef FunNImpl<sizeof...(Args),Result,Args...>* Impl;

   FunN( int, Impl i ) : rep(1,i) {}
   template <class DF>
   FunN( const DF& df ) : rep(df) {}

   FunN( const FunN& x ) : rep(x.rep) {}
   FunN& operator=( const FunN& x ) { rep = x.rep; return *this; }
   FunN() {
     //Actual implementation is in default constructor for FunNGuts
   }

  template <typename... ABC>
  Result
  operator()( const ABC&... xyz ) const
  {
    //std::cout << "FunN operator()" << std::endl;
     return (rep).operator()(xyz...);
  }

};

/* This section not yet implemented.
template <class A1d, class A2d, class A3d, class Rd,
          class A1s, class A2s, class A3s, class Rs>
Fun3Impl<A1d,A2d,A3d,Rd>*
convert3( const IRef<const Fun3Impl<A1s,A2s,A3s,Rs> >& f ) {
   return new Fun3Converter<A1d,A2d,A3d,Rd,A1s,A2s,A3s,Rs>( f );
}
template <class A1d, class A2d, class A3d, class Rd,
          class A1s, class A2s, class A3s, class Rs>
class Fun3ExplicitConverter : public Fun3Impl<A1d,A2d,A3d,Rd> {
   typedef IRef<const Fun3Impl<A1s,A2s,A3s,Rs> > MyFun;
   MyFun f;
public:
   Fun3ExplicitConverter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y, const A3d& z ) const {
      return static_cast<Rd>( f->operator()(  static_cast<A1s>(x),
         static_cast<A2s>(y), static_cast<A3s>(z)  ) );
   }
};

template <class A1d, class A2d, class A3d, class Rd,
          class A1s, class A2s, class A3s, class Rs>
Fun3<A1d,A2d,A3d,Rd> explicit_convert3( const Fun3<A1s,A2s,A3s,Rs>& f ) {
   return Fun3<A1d,A2d,A3d,Rd>( 1,
      new Fun3ExplicitConverter<A1d,A2d,A3d,Rd,A1s,A2s,A3s,Rs>(f.rep.ref) );
}
*/

// Modified this to be dependent on the no of args.
// template <int N,typename... Types> and specialise for different sizes.
// This is a model for how to implement a number of other things as well.

// Result is now explicit here.
// It is possible that the specialisations are not needed now.
// except perhaps when N = 0.
// This is now true and implemented.
template <int N,typename Result,typename... Args>
  struct FunNImpl
 : public FunArgs<Args...>, public fcpp::IRefable {
public:
  FunNImpl() {}

  virtual Result operator()( const Args&... ) const =0;
  virtual ~FunNImpl() {}
};

template <typename Result,typename... Args>
struct FunNImpl<0,Result,Args...>
 : public FunArgs<Args...>, public fcpp::IRefable {
public:
  FunNImpl() {}

  virtual Result operator()() const =0;
  virtual ~FunNImpl() {}
};

template <class Gen,typename Result,typename... Args>
  class GenN : public FunNImpl<sizeof...(Args),Result,Args...>
 {   Gen g;
public:
   GenN( Gen x ) : g(x) {}
   inline Result
   operator()(const Args&... xyz) const {
     //std::cout << "GenN operator()" << std::endl;
      return g(xyz...);
   }
};

/* Not yet implemented.
template <class Ternary>
Fun3<typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg1Type,
     typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg2Type,
     typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg3Type,
     typename Ternary::template Sig<AnyType,AnyType,AnyType>::ResultType>
makeFun3( const Ternary& g ) {
   return
   Fun3<typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg1Type,
        typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg2Type,
        typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg3Type,
        typename Ternary::template Sig<AnyType,AnyType,AnyType>::ResultType>
        (1,new Gen3<Ternary>(g));
}
*/

template <class Nary,typename Result,typename... Args>
GenN<Nary,Result,Args...>* makeFunNRef( const Nary& g ) {
  return new GenN<Nary,Result,Args...>(g);
}

// With reordering of template arguments, specialization is not needed at all.
  template <typename Rd,typename DF,typename... Args>
  struct FunNConstructor {
    static FunNImpl<sizeof...(Args),Rd,Args...>* make( const DF& df ) {
      //std::cout << "FunNConstructor<Rd,DF,Args...>" << std::endl;
      return makeFunNRef<DF,Rd,Args...>( (df) );
    }

    /* Not yet implemented.
template <class A1d, class A2d, class A3d, class Rd,
          class A1s, class A2s, class A3s, class Rs>
struct Fun3Constructor<A1d,A2d,A3d,Rd,Fun3<A1s,A2s,A3s,Rs> > {
   static Fun3Impl<A1d,A2d,A3d,Rd>* make( const Fun3<A1s,A2s,A3s,Rs>& f ) {
      return convert3<A1d,A2d,A3d,Rd>(f.rep.ref);
   }
};
    */
  };

////////////////////////////////////////////////////////////////////////
// Bringing in FC++ prelude ideas.
////////////////////////////////////////////////////////////////////////
// These are needed for interoperation between VFC++ and FC++,
// because the FC++ implementation uses knowledge of the
// maximum number of arguments, via the FunctoidTrait mechanism set up
// in smart.h.
// Special action thus needs to be taken so that VFC++ functoids will work,
// and this has been done by using a value of -1 for the maximum no of
// arguments in some of the implementations.
// This is detected (see examples below) and handled in VFC++ code.
// Other values are passed to FC++.  Note that this part of FC++ (prelude.h)
// has not been updated fully and so the maximum number of functoid parameters
// which can be handled is not as many as in e.g. full.h.
// This maximum is 4 or 5 for most of these items.
// What is needed is a VFC++ implementation (adding N at the end)
// which passes through
// to the FC++ code. This is therefore dependent on FC++.
//
// So far done: beforeN afterN
//              composeN composeNN (FC++ compose2)
//              ignoreN duplicateN (see prelude.h)
//
// Could be done:  emptifyN thunkFuncToFuncN
// (I am not going to bother with these for now)
//                 (in prelude.h)
// also bLiftN (in monad.h)
//
// I think these are quite important in expanding the usefulness of VFC++
// which up to now has not many functoids.
// These, and particularly composeN and composeNN,
// provide an easy way to build a lot more functoids.
//
// John Fletcher November 2009
// (All files mentioned are part of FC++, version 1.5.2)
////////////////////////////////////////////////////////////////////////

template <class F, class G>
class XBeforerN  : public SmartFunctoidN
{
   F f;
   G g;
public:
   XBeforerN( const F& ff, const G& gg ) : f(ff), g(gg) {}
   template <typename... Args> 
   struct Sig : public FunType<Args...,typename RT<G,Args...>::ResultType> {};
   template <typename... Args> 
   typename Sig<Args...>::ResultType 
     operator()( const Args&... args) const 
   { 
     //typedef typename Sig<Args...>::ResultType Result_Type;
     f(); return g(args...); 
   }
};

// This inherits from the FC++ definition of the functoid before.
  struct BeforeN : public fcpp::impl::XBefore, public SmartFunctoidN
{
   template <int i, class F, class G> struct HelperN;

   // This helper uses the value of -1 arguments to signal a FullN functoid
   // I wanted to make this a specialisation of the FC++ Helper but it is in the wrong namespace.
   // So I made it HelperN instead.
   template <class F, class G> 
   struct HelperN<-1,F,G> {
      typedef FullN<XBeforerN<F,G> > Result;
      static Result go( const F& f, const G& g ) 
      { return makeFullN( XBeforerN<F,G>(f,g) ); }
   };

   // These enable beforeN to process FC++ functoids by passing through to FC++ implementation.
   template <class F, class G> 
     struct HelperN<0,F,G> : public fcpp::impl::XBefore::Helper<0,F,G> { };
   template <class F, class G> 
     struct HelperN<1,F,G> : public fcpp::impl::XBefore::Helper<1,F,G> { };
   template <class F, class G> 
     struct HelperN<2,F,G> : public fcpp::impl::XBefore::Helper<2,F,G> { };
   template <class F, class G> 
     struct HelperN<3,F,G> : public fcpp::impl::XBefore::Helper<3,F,G> { };
   template <class F, class G> 
     struct HelperN<4,F,G> : public fcpp::impl::XBefore::Helper<4,F,G> { };
  
   template <class F, class G> struct Sig : public FunType<F,G,
     typename HelperN<fcpp::FunctoidTraits<G>::max_args,F,G>::Result> {};
   template <class F, class G>
   typename Sig<F,G>::ResultType 
   operator()( const F& f, const G& g ) const 
   {
     return HelperN<fcpp::FunctoidTraits<G>::max_args,F,G>::go( f, g );
   }
  
     static FullN<BeforeN>& full() {static FullN<BeforeN> f; return f;}
};

static FullN<BeforeN>& beforeN=BeforeN::full();

template <class F, class G>
class XAftererN  : public SmartFunctoidN
{
   F f;
   G g;
public:
   XAftererN( const F& ff, const G& gg ) : f(ff), g(gg) {}
   template <typename... Args> 
   struct Sig : public FunType<Args...,typename RT<F,Args...>::ResultType> {};
   template <typename... Args> 
   typename Sig<Args...>::ResultType 
     operator()( const Args&... args) const 
   { 
     typedef typename Sig<Args...>::ResultType Result_Type;
     Result_Type tmp = f(args...); g(); return tmp;
   }
};
// This inherits from the FC++ definition of the functoid after.
struct AfterN  : public fcpp::impl::XAfter ,  public SmartFunctoidN
{

   template <int i, class F, class G> struct HelperN;

   // This helper uses the value of -1 arguments to signal a FullN functoid
   // I wanted to make this a specialisation of the FC++ Helper but it is in the wrong namespace.
   // So I made it HelperN instead.
   template <class F, class G> 
   struct HelperN<-1,F,G> {
      typedef FullN<XAftererN<F,G> > Result;
      static Result go( const F& f, const G& g ) 
      { return makeFullN( XAftererN<F,G>(f,g) ); }
   };

   // These enable afterN to process FC++ functoids by passing through to FC++ implementation.
   template <class F, class G> 
     struct HelperN<0,F,G> : public fcpp::impl::XAfter::Helper<0,F,G> { };
   template <class F, class G> 
     struct HelperN<1,F,G> : public fcpp::impl::XAfter::Helper<1,F,G> { };
   template <class F, class G> 
     struct HelperN<2,F,G> : public fcpp::impl::XAfter::Helper<2,F,G> { };
   template <class F, class G> 
     struct HelperN<3,F,G> : public fcpp::impl::XAfter::Helper<3,F,G> { };
   template <class F, class G> 
     struct HelperN<4,F,G> : public fcpp::impl::XAfter::Helper<4,F,G> { };

   template <class F, class G> struct Sig : public FunType<F,G,
   typename HelperN<fcpp::FunctoidTraits<F>::max_args,F,G>::Result> {};
   template <class F, class G>
   typename Sig<F,G>::ResultType 
   operator()( const F& f, const G& g ) const 
   {
     return HelperN<fcpp::FunctoidTraits<F>::max_args,F,G>::go( f, g );
   }
  
     static FullN<AfterN>& full() {static FullN<AfterN> f; return f;}

};

static FullN<AfterN>& afterN=AfterN::full();

// This is used in ComposeN and ComposeNN
template <class F, class G>
class XComposeN  : public SmartFunctoidN
{
   F f;
   G g;
public:
#ifdef FCPP_DEBUG
  std::string name() const
  {
      // The complexity is to cover the case where
      // both, either, or neither of F and G have
      // the name defined.
    if ( debug_traits_has_name<F>() ) {
      if ( debug_traits_has_name<G>() ) {
	    return std::string("XComposeN::(")
                    + debug_traits_name(f)
                    + std::string(",")
       		    + debug_traits_name(g)
   	            + std::string(")");
         } else {
            return std::string("XComposeN::(")
                    + debug_traits_name(f)
   	            + std::string(",g)");
         }
      } else {
      if ( debug_traits_has_name<G>() ) {
            return std::string("XComposeN::(f,")
                    + debug_traits_name(g)
   	            + std::string(")");
	 } else {
             return std::string("XComposeN::(f,g)");
	 }
      }
  }
#endif
   XComposeN( const F& ff, const G& gg ) : f(ff), g(gg) {}
   template <typename... Args>
     struct Sig : public FunType<Args...,
     typename RT<F,typename RT<G,Args...>::ResultType>::ResultType> {};
   template <typename... Args>
   typename Sig<Args...>::ResultType
     operator()( const Args&... args) const
   {
     return f( g(args...) );
   }

};

// This inherits from the FC++ definition of the functoid compose.
struct ComposeN  : public fcpp::impl::XCompose,  public SmartFunctoidN
{
   template <int i, class F, class G> struct HelperN;

   template <class F, class G>
   struct HelperN<-1,F,G> {
      typedef FullN<XComposeN<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFullN( XComposeN<F,G>(f,g) ); }
   };

   // These enable composeN to process FC++ functoids by
   // passing through to FC++ implementation.
   template <class F, class G>
     struct HelperN<0,F,G> : public fcpp::impl::XCompose::Helper<0,F,G> { };
   template <class F, class G>
     struct HelperN<1,F,G> : public fcpp::impl::XCompose::Helper<1,F,G> { };
   template <class F, class G>
     struct HelperN<2,F,G> : public fcpp::impl::XCompose::Helper<2,F,G> { };
   template <class F, class G>
     struct HelperN<3,F,G> : public fcpp::impl::XCompose::Helper<3,F,G> { };
   template <class F, class G>
     struct HelperN<4,F,G> : public fcpp::impl::XCompose::Helper<4,F,G> { };

   template <class F, class G> struct Sig : public FunType<F,G,
   typename HelperN<fcpp::FunctoidTraits<G>::max_args,F,G>::Result> {};

   template <class F, class G>
   typename Sig<F,G>::ResultType
   operator()( const F& f, const G& g ) const
   {
     return HelperN<fcpp::FunctoidTraits<G>::max_args,F,G>::go( f, g );
   }
  
     static FullN<ComposeN>& full() {static FullN<ComposeN> f; return f;}

};

static FullN<ComposeN>& composeN=ComposeN::full();

template <class F, class G, class H>
class XComposeN2   : public SmartFunctoidN
{
   F f;
   G g;
   H h;
public:
#ifdef FCPP_DEBUG
  std::string name() const
  {
      // The complexity is to cover the case where
      // all or some of F G and H have the name defined.
      std::string temp = std::string("XComposeN2::(");
      if ( debug_traits_has_name<F>() ) {
        temp += debug_traits_name(f) + std::string(",");
	if ( debug_traits_has_name<G>() ) {
	    temp += debug_traits_name(g)
   	          + std::string(",");
         } else {
            temp += std::string("g,");
         }
	if ( debug_traits_has_name<H>() ) {
	    return  temp
       		    + debug_traits_name(h)
   	            + std::string(")");
         } else {
            return  temp + std::string("h)");
         }
      } else {
	if ( debug_traits_has_name<G>() ) {
            return std::string("XComposeN2::(f,")
                    + debug_traits_name(g)
   	            + std::string(",h)");
	 } else {
             return std::string("XComposeN2::(f,g,h)");
	 }
      }
   }
#endif
 XComposeN2( const F& ff, const G& gg, const H& hh ) : f(ff), g(gg), h(hh) {}
   template <typename... Args>
     struct Sig : public FunType<Args...,
     typename RT<F,typename RT<G,Args...>::ResultType,
     typename RT<H,Args...>::ResultType>::ResultType> {};

   template <typename... Args>
   typename Sig<Args...>::ResultType
     operator()( const Args&... args) const
   {
     return f( g(args...), h(args...) );
   }
};

// This separates out the two argument cases.
// It now exists as a separate functoid.
// I created it to resolve the problem of supporting the
// zero argument case, which I did not manage to do in XComposeN2.
// Its internals are used in ComposeNN to resolve the case
//     composeNN(f,g,h)() == f( g(), h() )
struct ComposeN2 : public SmartFunctoidN
{
 public:
   template <int i, int j, class F, class G, class H> struct HelperN;

   template <class F, class G, class H>
     struct HelperN<-1,-1,F,G,H> {
     typedef FullN<XComposeN2<F,G,H> > Result;
     static Result go( const F& f, const G& g, const H& h )
     { return makeFullN( XComposeN2<F,G,H>(f,g,h) ); }
   };

   // Special case where g and h have zero args (Full0 type).
   template <class F, class G, class H>
    struct HelperN<0,0,F,G,H> {
    typedef fcpp::Full0<fcpp::impl::XXCompose0Helper<F,G,H> > Result;
    static Result go( const F& f, const G& g, const H& h )
    { return fcpp::makeFull0(fcpp::impl::XXCompose0Helper<F,G,H>(f,g,h)); }
   };

   template <class F, class G, class H>
    struct HelperN<1,1,F,G,H> {
     typedef fcpp::Full1<fcpp::impl::XXCompose1Helper<F,G,H> > Result;
      static Result go( const F& f, const G& g, const H& h )
      { return fcpp::makeFull1(fcpp::impl::XXCompose1Helper<F,G,H>(f,g,h)); }
   };

   template <class F, class G, class H>
    struct HelperN<2,2,F,G,H> {
     typedef fcpp::Full2<fcpp::impl::XXCompose2Helper<F,G,H> > Result;
      static Result go( const F& f, const G& g, const H& h )
      { return fcpp::makeFull2(fcpp::impl::XXCompose2Helper<F,G,H>(f,g,h)); }
   };

   template <class F, class G, class H> struct Sig : public FunType<F,G,H,
     typename HelperN<fcpp::FunctoidTraits<G>::max_args,
     fcpp::FunctoidTraits<H>::max_args,F,G,H>::Result> {};

   template <class F, class G, class H>
     typename Sig<F,G,H>::ResultType
     operator()( const F& f, const G& g, const H &h ) const
   {
     return HelperN<fcpp::FunctoidTraits<G>::max_args,
       fcpp::FunctoidTraits<H>::max_args,F,G,H>::go( f, g, h );
   }
  
     static FullN<ComposeN2>& full() {static FullN<ComposeN2> f; return f;}

#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("composeN2");
  }
#endif
};

static FullN<ComposeN2>& composeN2=ComposeN2::full();

template <class F, class G, class H, class I>
class XComposeN3   : public SmartFunctoidN
{
   F f;
   G g;
   H h;
   I i;
public:
#ifdef FCPP_DEBUG
  std::string name() const
  {
      std::string temp = std::string("XComposeN3::(");
      if ( debug_traits_has_name<F>() ) {
        temp += debug_traits_name(f) + std::string(",");
	if ( debug_traits_has_name<G>() ) {
	    temp += debug_traits_name(g)
   	          + std::string(",");
        } else {
            temp += std::string("g,");
        }
	if ( debug_traits_has_name<H>() ) {
	    temp += debug_traits_name(h)
   	          + std::string(",");
        } else {
            temp += std::string("h,");
        }
	if ( debug_traits_has_name<I>() ) {
	    return  temp
       		    + debug_traits_name(i)
   	            + std::string(")");
        } else {
            return  temp + std::string("i)");
        }
      } else {
	if ( debug_traits_has_name<G>() ) {
            return std::string("XComposeN3::(f,")
                    + debug_traits_name(g)
   	            +std::string(",h,i)");
	 } else {
             return std::string("XComposeN3::(f,g,h,i)");
	 }
      }
      //return std::string("XComposeN3::");
  }
#endif
 XComposeN3( const F& ff, const G& gg, const H& hh, const I& ii ) : f(ff), g(gg), h(hh), i(ii) {}
   template <typename... Args>
     struct Sig : public FunType<Args...,
     typename RT<F,typename RT<G,Args...>::ResultType,typename RT<H,Args...>::ResultType,
                   typename RT<I,Args...>::ResultType>::ResultType>
   {
     typedef G Arg1Type;
     typedef H Arg2Type;
     typedef I Arg3Type;
   };

   template <typename... Args>
   typename Sig<Args...>::ResultType
     operator()( const Args&... args) const
   {
     return f( g(args...), h(args...), i(args...) );
   }
};

/* Moved to prelude.h where it works.
template <class F, class G, class H, class I>
class X3Compose0Helper : public CFunType<typename F::template
Sig<typename RT<G>::ResultType,typename RT<H>::ResultType,
    typename RT<I>::ResultType>::ResultType>
{
// contents removed.
};
*/

template <class F, class G, class H, class I, class J>
class XComposeN4   : public SmartFunctoidN
{
   F f;
   G g;
   H h;
   I i;
   J j;
public:
#ifdef FCPP_DEBUG
  std::string name() const
  {
      std::string temp = std::string("XComposeN4::(");
      if ( debug_traits_has_name<F>() ) {
        temp += debug_traits_name(f) + std::string(",");
	if ( debug_traits_has_name<G>() ) {
	    temp += debug_traits_name(g)
   	          + std::string(",");
        } else {
            temp += std::string("g,");
        }
	if ( debug_traits_has_name<H>() ) {
	    temp += debug_traits_name(h)
   	          + std::string(",");
        } else {
            temp += std::string("h,");
        }
	if ( debug_traits_has_name<I>() ) {
	    temp += debug_traits_name(i)
   	          + std::string(",");
        } else {
            temp += std::string("i,");
        }
	if ( debug_traits_has_name<J>() ) {
	    return  temp
       		    + debug_traits_name(i)
   	            + std::string(")");
        } else {
            return  temp
   	            + std::string("j)");
        }
      } else {
	if ( debug_traits_has_name<G>() ) {
            return std::string("XComposeN4::(f,")
                    + debug_traits_name(g)
   	            + std::string(",h,i,j)");
	 } else {
             return std::string("XComposeN4::(f,g,h,i,j)");
	 }
      }
      //return std::string("XComposeN4::");
  }
#endif
 XComposeN4( const F& ff, const G& gg, const H& hh, const I& ii,const J& jj ) :
   f(ff), g(gg), h(hh), i(ii), j(jj) {}
   template <typename... Args>
     struct Sig : public FunType<Args...,
     typename RT<F,typename RT<G,Args...>::ResultType,typename RT<H,Args...>::ResultType,
     typename RT<I,Args...>::ResultType,typename RT<J,Args...>::ResultType>::ResultType> {};

   template <typename... Args>
   typename Sig<Args...>::ResultType
     operator()( const Args&... args) const
   {
     return f( g(args...), h(args...), i(args...), j(args...) );
   }
};

// =====================================================================
// composeNN(f,f1,f2,f3,...,fn)(args) == f( f1(args),f2(args),f3(args),...,fn(args) )
// based on  compose2(f,g,h)(x) == f( g(x), h(x) )
// ====================== FC++ comment =================================
// compose2 composes a two argument function with two one-argument
// functions (taking the same type). This is quite useful for the
// common case of binary operators.  Use lambda for more-complicated stuff.
// I have extended compose2 to cover the case of g and h being fully bound.
//     compose2(f,g,h)() == f( g(), h() )
// This has now been extended to 3 and 4 function arguments.
// Some of the supporting code is located in prelude.h with compose2.
// =====================================================================
// composeNN goes much further and can apply an arbitrary set of args to
// any number of functoids.
// These results are then passed as arguments to the first named functoid.
// The implementation limit is currently 4 for the number of functions called.
// =====================================================================
// composeNN now makes use of composeN and composeN2 to handle the
// simple cases which in where possible are passed to compose or compose2.
// This simplifies things from the point of view of the user.
// This is carried out by the HelperNN struct calling HelperN structs
// in the other places as needed.
// =====================================================================
// Implementation note:
// ComposeNN has two levels of helpers which are used as follows.
//
// HelperNN declared as:
//   template <int i, class F, typename... Funs> struct HelperNN;
//
// The value of i is the number of function arguments from 1 to 4.
//
// HelperN3 and HelperN4 declared as:
//
//   template <int i, int j, int k, class F, class G, class H, class I>
//   struct HelperN3
//
//   template <int i, int j, int k, int l,
//     class F, class G, class H, class I, class J>
//   struct HelperN4
//
// These are used to specialise the case where the function arguments
// have zero arguments, i.e. they are fully bound as Full0.
//
// The only specializations are for the cases where i=j=k=0 (N3)
// and 1=j=k=l=0 (N4).
//
// The cases with 1 and 2 arguments are handled similarly using ComposeN
// and ComposeN2.
//
// Note that structs with names starting with X are not intended to be
// called directly by the user.
// =====================================================================

struct ComposeNN   : public SmartFunctoidN
{
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("ComposeNN");
  }
#endif

   template <int i, class F, typename... Funs> struct HelperNN;

   // This enables the use of the special cases which are in ComposeN.
   template <class F, class G>
   struct HelperNN<1,F,G> {
      typedef typename ComposeN::HelperN<fcpp::FunctoidTraits<G>::max_args,
              F,G>::Result Result;
      static Result go( const F& f, const G& g )
      {
        return ComposeN::HelperN<fcpp::FunctoidTraits<G>::max_args,
              F,G>::go( f, g );
      }
   };

   // This enables the use of the special cases for 0 and 1 args
   // which are in ComposeN2.
   template <class F, class G, class H>
   struct HelperNN<2,F,G,H> {
      typedef typename ComposeN2::HelperN<fcpp::FunctoidTraits<G>::max_args,
      fcpp::FunctoidTraits<H>::max_args,F,G,H>::Result Result;
      static Result go( const F& f, const G& g, const H& h)
      {
        return ComposeN2::HelperN<fcpp::FunctoidTraits<G>::max_args,
        fcpp::FunctoidTraits<H>::max_args,F,G,H>::go( f, g, h );
      }
   };

   // This is instead of defining ComposeN3 and using its
   // internals here.
   template <int i, int j, int k, class F, class G, class H, class I>
   struct HelperN3
   {
     //template <class F, class G, class H, class I>
     //struct HelperN3<-1,-1,-1,F,G,H,I> {
     typedef FullN<XComposeN3<F,G,H,I> > Result;
     static Result go( const F& f, const G& g, const H& h, const I& ii )
     {
       return makeFullN( XComposeN3<F,G,H,I>(f,g,h,ii) );
     }
   };

   // Special case where g h and i have zero args (Full0 type).
   template <class F, class G, class H, class I>
     struct HelperN3<0,0,0,F,G,H,I> {
     typedef fcpp::Full0<fcpp::impl::X3Compose0Helper<F,G,H,I> > Result;
     static Result go( const F& f, const G& g, const H& h, const I& i )
     {
       return fcpp::makeFull0(fcpp::impl::X3Compose0Helper<F,G,H,I>(f,g,h,i));
     }
   };

   template <class F, class G, class H, class I>
    struct HelperNN<3,F,G,H,I> {
      typedef typename HelperN3<fcpp::FunctoidTraits<G>::max_args,
      fcpp::FunctoidTraits<H>::max_args,fcpp::FunctoidTraits<I>::max_args,
	F,G,H,I>::Result Result;
    static Result go( const F& f, const G& g, const H& h, const I& i)
    {
        return HelperN3<fcpp::FunctoidTraits<G>::max_args,
        fcpp::FunctoidTraits<H>::max_args,fcpp::FunctoidTraits<I>::max_args,
	  F,G,H,I>::go( f, g, h, i );
    }
   };

   // This is instead of defining ComposeN4 and using its
   // internals here.
   template <int i, int j, int k, int l,
     class F, class G, class H, class I, class J>
   struct HelperN4
   {
     //template <class F, class G, class H, class I, class J>
     //struct HelperN4<-1,-1,-1,-1,F,G,H,I,J> {
     typedef FullN<XComposeN4<F,G,H,I,J> > Result;
     static Result go( const F& f, const G& g, const H& h,
                       const I& ii, const J& jj )
     {
       return makeFullN( XComposeN4<F,G,H,I,J>(f,g,h,ii,jj) );
     }
   };

   // Special case where g h i and j have zero args (Full0 type).
   template <class F, class G, class H, class I, class J>
     struct HelperN4<0,0,0,0,F,G,H,I,J> {
     typedef fcpp::Full0<fcpp::impl::X4Compose0Helper<F,G,H,I,J> > Result;
     static Result go( const F& f, const G& g, const H& h,
                       const I& i, const J& j )
     {
       return fcpp::makeFull0
       (fcpp::impl::X4Compose0Helper<F,G,H,I,J>(f,g,h,i,j));
     }
   };

   template <class F, class G, class H, class I, class J>
    struct HelperNN<4,F,G,H,I,J> {
      typedef typename HelperN4<fcpp::FunctoidTraits<G>::max_args,
      fcpp::FunctoidTraits<H>::max_args,fcpp::FunctoidTraits<I>::max_args,
      fcpp::FunctoidTraits<J>::max_args,F,G,H,I,J>::Result Result;
     static Result go( const F& f, const G& g, const H& h, const I& i, const J& j)
     {
        return HelperN4<fcpp::FunctoidTraits<G>::max_args,
        fcpp::FunctoidTraits<H>::max_args,fcpp::FunctoidTraits<I>::max_args,
	fcpp::FunctoidTraits<J>::max_args,F,G,H,I,J>::go
          ( f, g, h, i, j );
     }
   };

   template <int i,class F, typename... Funs> struct SigN;

   template <class F, typename G>
   struct SigN<1,F,G>  {
       typedef typename HelperNN<1,F,G>::Result ResultType;
       typedef G Arg1Type;
       typedef Void Arg2Type;
       typedef Void Arg3Type;
       typedef Void Arg4Type;
   };

   template <class F, typename G, typename H>
   struct SigN<2,F,G,H>  {
       typedef typename HelperNN<2,F,G,H>::Result ResultType;
       typedef G Arg1Type;
       typedef H Arg2Type;
       typedef Void Arg3Type;
       typedef Void Arg4Type;
   };

   template <class F, typename G, typename H, typename I>
   struct SigN<3,F,G,H,I>  {
       typedef typename HelperNN<3,F,G,H,I>::Result ResultType;
       typedef G Arg1Type;
       typedef H Arg2Type;
       typedef I Arg3Type;
       typedef Void Arg4Type;
    };

    template <class F, typename G, typename H, typename I, typename J>
    struct SigN<4,F,G,H,I,J>  {
       typedef typename HelperNN<4,F,G,H,I,J>::Result ResultType;
       typedef G Arg1Type;
       typedef H Arg2Type;
       typedef I Arg3Type;
       typedef J Arg4Type;
    };

    template <class F, typename... Funs> struct Sig :
    public SigN<sizeof...(Funs),F,Funs...> {};

    template <class F, typename... Funs>
     typename Sig<F,Funs...>::ResultType
     operator()(const F& f, const Funs&... funs) const {
     return  HelperNN<sizeof...(funs),F,Funs...>::go( f, funs... );
    }

    static FullN<ComposeNN>& full() {static FullN<ComposeNN> f; return f;}

};

static FullN<ComposeNN>& composeNN=ComposeNN::full();


template <class F>
class XTFTFN  : public SmartFunctoidN {
   F f;
public:
   XTFTFN(const F& ff) : f(ff) {}
    // Note the need to do two levels of FunType to get the result type.
    template <typename... Args>
    struct Sig : public FunType<FunType<Args...,typename RT<F,Args...>::ResultType>
                               >::ResultType {};
    template <typename... Args>
     typename Sig<Args...>::ResultType
     operator()(const Args&... args) const
   {
     //typedef typename Sig<Args...>::ResultType Result_Type;
     return  f()(args...);
   }
};

// This inherits from the FC++ definition of the functoid thunkFuncToFunc
struct ThunkFuncToFuncN  : public fcpp::impl::XThunkFuncToFunc,
                           public SmartFunctoidN
{

   template <int i, class F> struct HelperN;

   template <class F>
   struct HelperN<-1,F> {
      typedef FullN<XTFTFN<F> > Result;
      static Result go( const F& f)
      { return makeFullN( XTFTFN<F>(f) ); }
   };

   // These enable TFTFN to process FC++ functoids by passing through to
   // the FC++ implementation.
   template <class F>
     struct HelperN<0,F> : public fcpp::impl::XThunkFuncToFunc::Helper<0,F> { };
   template <class F>
     struct HelperN<1,F> : public fcpp::impl::XThunkFuncToFunc::Helper<1,F> { };
   template <class F>
     struct HelperN<2,F> : public fcpp::impl::XThunkFuncToFunc::Helper<2,F> { };
   template <class F>
     struct HelperN<3,F> : public fcpp::impl::XThunkFuncToFunc::Helper<3,F> { };
   template <class F>
     struct HelperN<4,F> : public fcpp::impl::XThunkFuncToFunc::Helper<4,F> { };
   template <class F>
     struct HelperN<5,F> : public fcpp::impl::XThunkFuncToFunc::Helper<5,F> { };
   template <class F>
     struct HelperN<6,F> : public fcpp::impl::XThunkFuncToFunc::Helper<6,F> { };

   // Note the need for both the Sig and the eventual call to have an extra
   // level of indirection to get the correct argument number and hence
   // call the correct next level of code.
   template <class F> struct Sig : public FunType<F,
     typename HelperN<fcpp::FunctoidTraits<typename fcpp::RT<F>::ResultType>::max_args,F>::Result> {};
   template <class F>
   typename Sig<F>::ResultType
   operator()( const F& f) const
     { // This works with fcpp::RT but not variadic_fcpp::RT.
     return HelperN<fcpp::FunctoidTraits<typename fcpp::RT<F>::ResultType>
            ::max_args,F>::go( f);
   }

   static FullN<ThunkFuncToFuncN>& full()
   {
        static FullN<ThunkFuncToFuncN> f;
        return f;
   }

};

static FullN<ThunkFuncToFuncN>& thunkFuncToFuncN=ThunkFuncToFuncN::full();


template <class F>
class XIgnorerN  : public SmartFunctoidN {
   F f;
public:
   XIgnorerN(const F& ff) : f(ff) {}
   template <typename X,typename... Args>
   struct Sig : public FunType<Args...,typename RT<F,Args...>::ResultType> {};
   template <typename X,typename... Args>
     typename Sig<X,Args...>::ResultType
     operator()( const X& x, const Args&... args) const
   {
     //typedef typename Sig<Args...>::ResultType Result_Type;
     return  f(args...);
   }
};

// This inherits from the FC++ definition of the functoid ignore.
struct IgnoreN  : public fcpp::impl::XIgnore,  public SmartFunctoidN
{

   template <int i, class F> struct HelperN;

   // This helper uses the value of -1 arguments to signal a FullN functoid
   // I wanted to make this a specialisation of the FC++ Helper
   // but it is in the wrong namespace.
   // So I made it HelperN instead.
   template <class F>
   struct HelperN<-1,F> {
      typedef FullN<XIgnorerN<F> > Result;
      static Result go( const F& f)
      { return makeFullN( XIgnorerN<F>(f) ); }
   };

   // These enable ignoreN to process FC++ functoids by passing through to
   // the FC++ implementation.
   template <class F>
     struct HelperN<0,F> : public fcpp::impl::XIgnore::Helper<0,F> { };
   template <class F>
     struct HelperN<1,F> : public fcpp::impl::XIgnore::Helper<1,F> { };
   template <class F>
     struct HelperN<2,F> : public fcpp::impl::XIgnore::Helper<2,F> { };
   template <class F>
     struct HelperN<3,F> : public fcpp::impl::XIgnore::Helper<3,F> { };
   template <class F>
     struct HelperN<4,F> : public fcpp::impl::XIgnore::Helper<4,F> { };

   template <class F> struct Sig : public FunType<F,
   typename HelperN<fcpp::FunctoidTraits<F>::max_args,F>::Result> {};
   template <class F>
   typename Sig<F>::ResultType 
   operator()( const F& f) const 
   {
     return HelperN<fcpp::FunctoidTraits<F>::max_args,F>::go( f);
   }
  
     static FullN<IgnoreN>& full() {static FullN<IgnoreN> f; return f;}

};

static FullN<IgnoreN>& ignoreN=IgnoreN::full();

template <class F>
class XDuplicaterN  : public SmartFunctoidN {
   F f;
public:
   XDuplicaterN( const F& ff ) : f(ff) {}

   template <class X, typename... Args>
     struct Sig : public FunType<X,Args...,typename RT<F,X,X,Args...>::ResultType > {};
   template <class X, typename... Args>
     typename Sig<X,Args...>::ResultType
     operator()( const X& x , const Args&... args) const {
     return f(x,x,args...);
   }
};

struct DuplicateN  : public SmartFunctoidN {
   template <class F>
   struct Sig : public FunType<F,FullN<XDuplicaterN<F> > > {};

   template <class F>
   FullN<XDuplicaterN<F> > operator()( const F& f ) const {
      return makeFullN( XDuplicaterN<F>(f) );
   }

     static FullN<DuplicateN>& full() {static FullN<DuplicateN> f; return f;}

};

static FullN<DuplicateN>& duplicateN=DuplicateN::full();

} // end namespace variadic_fcpp

///////////////////////////////////////////////////////////////////////////////
// Monad related code now in a separate file
//////////////////////////////////////////////////////////////////////////////
#ifdef FCPP_ENABLE_LAMBDA
#include "variadic_monad.h"
#endif

////////////////////////////////////////////////////////////////////////
// End of variadic FC++ (so far).
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// From here it is examples of uses.
// Note that uses do have to be  : public SmartFunctoidN
// to be able to be in composeNN.
// There is also a need to define extra arguments as Void
// for RT and RTN.
////////////////////////////////////////////////////////////////////////
//
// Code moved:
//
// MultipliesN
// PlusN
// TupleN
//
////////////////////////////////////////////////////////////////////////
#include "variadic_operator.h"


namespace variadic_fcpp {

  //template <typename R>
  // These cannot be declared generically. (Yet?)
  static FullNR<PlusN,int>& plusNint = PlusN::fullR<int>();
  static FullNR<PlusN,double>& plusNdouble = PlusN::fullR<double>();

} // end namespace variadic_fcpp


template <typename F>
std::ostream & operator << (std::ostream &o, const variadic_fcpp::FullN<F> &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a FullN functoid ****";
#endif
  return o;
}

template <typename F, typename A>
std::ostream & operator << (std::ostream &o, const variadic_fcpp::binder1ofN<F,A> &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a binder1ofN functoid ****";
#endif
  return o;
}

template <typename... Args>
std::ostream & operator << (std::ostream &o, const TR1::tuple<Args...> &f)
{
  o << "**** ERROR: Attempt to output a TR1::tuple ****";
  return o;
}

// Includes argument traits needed without FCPP_DEBUG
#include "variadic_debug.h"
//#endif

#endif // #ifndef FCPP_VARIADIC_FCPP_H

#endif // #ifdef VARIADIC


