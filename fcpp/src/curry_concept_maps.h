
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_CONCEPT_MAPS_DOT_H
#define FCPP_CURRY_CONCEPT_MAPS_DOT_H

////////////////////////////////////////////////////////////////////////////////
// Adapting the binders to work with FCPP_CONCEPTS
// 
// The binders have been adapted in the same way as other code in full.h 
// and function.h using the concept maps defined in full.h.
//
// To adapt binderAof5 and binderXof5 it is necessary to define some further 
// concept maps which are introduced below.
//
// Concept maps have to be in the same namespace as the concept which 
// is being mapped.
//
// This means that namespace fcpp::impl has to be closed to define them.
//
// John Fletcher January 2008
////////////////////////////////////////////////////////////////////////////////

#ifdef FCPP_CONCEPTS

template <class F,class ArgA,class A,class B,class C,class D>
concept_map ConstCallable6<F,impl::Bind1,ArgA,A,B,C,D>
{
  typedef ConstCallable5<F,ArgA,A,B,C,D>::result_type result_type;
};

template <class F,class ArgA,class A,class B,class C,class D>
concept_map ConstCallable6<F,impl::Bind2,ArgA,A,B,C,D>
{
  typedef ConstCallable5<F,A,ArgA,B,C,D>::result_type result_type;
};

template <class F,class ArgA,class A,class B,class C,class D>
concept_map ConstCallable6<F,impl::Bind3,ArgA,A,B,C,D>
{
  typedef ConstCallable5<F,A,B,ArgA,C,D>::result_type result_type;
};

template <class F,class ArgA,class A,class B,class C,class D>
concept_map ConstCallable6<F,impl::Bind4,ArgA,A,B,C,D>
{
  typedef ConstCallable5<F,A,B,C,ArgA,D>::result_type result_type;
};

template <class F,class ArgA,class A,class B,class C,class D>
  concept_map ConstCallable6<F,impl::Bind5,ArgA,A,B,C,D>
{
  typedef ConstCallable5<F,A,B,C,D,ArgA>::result_type result_type;
};

// These are used by binderXandYof5 - more are needed.
template <class F,class ArgX,class ArgY,class A,class B,class C>
  concept_map ConstCallable7<F,impl::Bind1,impl::Bind2,ArgX,ArgY,A,B,C>
{
  typedef ConstCallable5<F,ArgX,ArgY,A,B,C>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C>
  concept_map ConstCallable7<F,impl::Bind1,impl::Bind3,ArgX,ArgY,A,B,C>
{
  typedef ConstCallable5<F,ArgX,A,ArgY,B,C>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C>
  concept_map ConstCallable7<F,impl::Bind1,impl::Bind4,ArgX,ArgY,A,B,C>
{
  typedef ConstCallable5<F,ArgX,A,B,ArgY,C>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C>
  concept_map ConstCallable7<F,impl::Bind1,impl::Bind5,ArgX,ArgY,A,B,C>
{
  typedef ConstCallable5<F,ArgX,A,B,C,ArgY>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C>
  concept_map ConstCallable7<F,impl::Bind2,impl::Bind3,ArgX,ArgY,A,B,C>
{
  typedef ConstCallable5<F,A,ArgX,ArgY,B,C>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C>
  concept_map ConstCallable7<F,impl::Bind2,impl::Bind4,ArgX,ArgY,A,B,C>
{
  typedef ConstCallable5<F,A,ArgX,B,ArgY,C>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C>
  concept_map ConstCallable7<F,impl::Bind2,impl::Bind5,ArgX,ArgY,A,B,C>
{
  typedef ConstCallable5<F,A,ArgX,B,C,ArgY>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C>
  concept_map ConstCallable7<F,impl::Bind3,impl::Bind4,ArgX,ArgY,A,B,C>
{
  typedef ConstCallable5<F,A,B,ArgX,ArgY,C>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C>
  concept_map ConstCallable7<F,impl::Bind3,impl::Bind5,ArgX,ArgY,A,B,C>
{
  typedef ConstCallable5<F,A,B,ArgX,C,ArgY>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C>
  concept_map ConstCallable7<F,impl::Bind4,impl::Bind5,ArgX,ArgY,A,B,C>
{
  typedef ConstCallable5<F,A,B,C,ArgX,ArgY>::result_type result_type;
};

// These are the ones for bindXtoZof5
// 10 needed in total to match the helper functions.
template <class F,class ArgX,class ArgY,class ArgZ,class A,class B>
  concept_map ConstCallable8<F,impl::Bind1,impl::Bind2,impl::Bind3,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable5<F,ArgX,ArgY,ArgZ,A,B>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B>
  concept_map ConstCallable8<F,impl::Bind1,impl::Bind2,impl::Bind4,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable5<F,ArgX,ArgY,A,ArgZ,B>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B>
  concept_map ConstCallable8<F,impl::Bind1,impl::Bind2,impl::Bind5,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable5<F,ArgX,ArgY,A,B,ArgZ>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B>
  concept_map ConstCallable8<F,impl::Bind1,impl::Bind3,impl::Bind4,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable5<F,ArgX,A,ArgY,ArgZ,B>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B>
  concept_map ConstCallable8<F,impl::Bind1,impl::Bind3,impl::Bind5,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable5<F,ArgX,A,ArgY,B,ArgZ>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B>
  concept_map ConstCallable8<F,impl::Bind1,impl::Bind4,impl::Bind5,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable5<F,ArgX,A,B,ArgY,ArgZ>::result_type result_type;
};
template <class F,class ArgX,class ArgY,class ArgZ,class A,class B>
  concept_map ConstCallable8<F,impl::Bind2,impl::Bind3,impl::Bind4,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable5<F,A,ArgX,ArgY,ArgZ,B>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B>
  concept_map ConstCallable8<F,impl::Bind2,impl::Bind3,impl::Bind5,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable5<F,A,ArgX,ArgY,B,ArgZ>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B>
  concept_map ConstCallable8<F,impl::Bind2,impl::Bind4,impl::Bind5,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable5<F,A,ArgX,B,ArgY,ArgZ>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B>
  concept_map ConstCallable8<F,impl::Bind3,impl::Bind4,impl::Bind5,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable5<F,A,B,ArgX,ArgY,ArgZ>::result_type result_type;
};

// These are the ones for bindWtoZof5
// 5 needed in total to match the helper functions.
 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A>
   concept_map ConstCallable9<F,impl::Bind1,impl::Bind2,impl::Bind3,impl::Bind4,ArgW,ArgX,ArgY,ArgZ,A>
{
  typedef ConstCallable5<F,ArgW,ArgX,ArgY,ArgZ,A>::result_type result_type;
};

 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A>
   concept_map ConstCallable9<F,impl::Bind1,impl::Bind2,impl::Bind3,impl::Bind5,ArgW,ArgX,ArgY,ArgZ,A>
{
  typedef ConstCallable5<F,ArgW,ArgX,ArgY,A,ArgZ>::result_type result_type;
};

 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A>
   concept_map ConstCallable9<F,impl::Bind1,impl::Bind2,impl::Bind4,impl::Bind5,ArgW,ArgX,ArgY,ArgZ,A>
{
  typedef ConstCallable5<F,ArgW,ArgX,A,ArgY,ArgZ>::result_type result_type;
};

 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A>
   concept_map ConstCallable9<F,impl::Bind1,impl::Bind3,impl::Bind4,impl::Bind5,ArgW,ArgX,ArgY,ArgZ,A>
{
  typedef ConstCallable5<F,ArgW,A,ArgX,ArgY,ArgZ>::result_type result_type;
};

 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A>
   concept_map ConstCallable9<F,impl::Bind2,impl::Bind3,impl::Bind4,impl::Bind5,ArgW,ArgX,ArgY,ArgZ,A>
{
  typedef ConstCallable5<F,A,ArgW,ArgX,ArgY,ArgZ>::result_type result_type;
};

// These are the ones for bindXof6 - 6 needed in total to match the helper functions.
 template <class F,class ArgX,class A,class B,class C,class D, class E>
   concept_map ConstCallable7<F,impl::Bind1,ArgX,A,B,C,D,E>
{
  typedef ConstCallable6<F,ArgX,A,B,C,D,E>::result_type result_type;
};

 template <class F,class ArgX,class A,class B,class C,class D, class E>
   concept_map ConstCallable7<F,impl::Bind2,ArgX,A,B,C,D,E>
{
  typedef ConstCallable6<F,A,ArgX,B,C,D,E>::result_type result_type;
};

 template <class F,class ArgX,class A,class B,class C,class D, class E>
   concept_map ConstCallable7<F,impl::Bind3,ArgX,A,B,C,D,E>
{
  typedef ConstCallable6<F,A,B,ArgX,C,D,E>::result_type result_type;
};

 template <class F,class ArgX,class A,class B,class C,class D, class E>
   concept_map ConstCallable7<F,impl::Bind4,ArgX,A,B,C,D,E>
{
  typedef ConstCallable6<F,A,B,C,ArgX,D,E>::result_type result_type;
};

 template <class F,class ArgX,class A,class B,class C,class D, class E>
   concept_map ConstCallable7<F,impl::Bind5,ArgX,A,B,C,D,E>
{
  typedef ConstCallable6<F,A,B,C,D,ArgX,E>::result_type result_type;
};

 template <class F,class ArgX,class A,class B,class C,class D, class E>
   concept_map ConstCallable7<F,impl::Bind6,ArgX,A,B,C,D,E>
{
  typedef ConstCallable6<F,A,B,C,D,E,ArgX>::result_type result_type;
};

// These are used by binderXandYof6 - more are needed.
 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind1,impl::Bind2,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,ArgX,ArgY,A,B,C,D>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind1,impl::Bind3,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,ArgX,A,ArgY,B,C,D>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind1,impl::Bind4,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,ArgX,A,B,ArgY,C,D>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind1,impl::Bind5,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,ArgX,A,B,C,ArgY,D>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind1,impl::Bind6,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,ArgX,A,B,C,D,ArgY>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind2,impl::Bind3,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,A,ArgX,ArgY,B,C,D>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind2,impl::Bind4,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,A,ArgX,B,ArgY,C,D>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind2,impl::Bind5,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,A,ArgX,B,C,ArgY,D>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind2,impl::Bind6,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,A,ArgX,B,C,D,ArgY>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind3,impl::Bind4,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,A,B,ArgX,ArgY,C,D>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind3,impl::Bind5,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,A,B,ArgX,C,ArgY,D>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind3,impl::Bind6,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,A,B,ArgX,C,D,ArgY>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind4,impl::Bind5,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,A,B,C,ArgX,ArgY,D>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind4,impl::Bind6,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,A,B,C,ArgX,D,ArgY>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class A,class B,class C,class D>
   concept_map ConstCallable8<F,impl::Bind5,impl::Bind6,ArgX,ArgY,A,B,C,D>
{
  typedef ConstCallable6<F,A,B,C,D,ArgX,ArgY>::result_type result_type;
};

// These are used by binderXtoZof6 - more are needed.
 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind1,impl::Bind2,impl::Bind3,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,ArgX,ArgY,ArgZ,A,B,C>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind1,impl::Bind2,impl::Bind4,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,ArgX,ArgY,A,ArgZ,B,C>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind1,impl::Bind2,impl::Bind5,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,ArgX,ArgY,A,B,ArgZ,C>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind1,impl::Bind2,impl::Bind6,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,ArgX,ArgY,A,B,C,ArgZ>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind1,impl::Bind3,impl::Bind4,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,ArgX,A,ArgY,ArgZ,B,C>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind1,impl::Bind3,impl::Bind5,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,ArgX,A,ArgY,B,ArgZ,C>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind1,impl::Bind3,impl::Bind6,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,ArgX,A,ArgY,B,C,ArgZ>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind2,impl::Bind3,impl::Bind4,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,A,ArgX,ArgY,ArgZ,B,C>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind2,impl::Bind3,impl::Bind5,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,A,ArgX,ArgY,B,ArgZ,C>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind2,impl::Bind3,impl::Bind6,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,A,ArgX,ArgY,B,C,ArgZ>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind2,impl::Bind4,impl::Bind5,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,A,ArgX,B,ArgY,ArgZ,C>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind2,impl::Bind4,impl::Bind6,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,A,ArgX,B,ArgY,C,ArgZ>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind2,impl::Bind5,impl::Bind6,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,A,ArgX,B,C,ArgY,ArgZ>::result_type result_type;
};

 template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable9<F,impl::Bind3,impl::Bind4,impl::Bind5,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable6<F,A,B,ArgX,ArgY,ArgZ,C>::result_type result_type;
};


// These are the ones for bindWtoZof6
// ? needed in total to match the helper functions.
 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A,class B>
   concept_map ConstCallable10<F,impl::Bind1,impl::Bind2,impl::Bind3,impl::Bind4,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable6<F,ArgW,ArgX,ArgY,ArgZ,A,B>::result_type result_type;
};

 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A,class B>
   concept_map ConstCallable10<F,impl::Bind1,impl::Bind2,impl::Bind3,impl::Bind5,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable6<F,ArgW,ArgX,ArgY,A,ArgZ,B>::result_type result_type;
};

 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A,class B>
   concept_map ConstCallable10<F,impl::Bind1,impl::Bind2,impl::Bind4,impl::Bind5,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable6<F,ArgW,ArgX,A,ArgY,ArgZ,B>::result_type result_type;
};

 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A,class B>
   concept_map ConstCallable10<F,impl::Bind1,impl::Bind3,impl::Bind4,impl::Bind5,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable6<F,ArgW,A,ArgX,ArgY,ArgZ,B>::result_type result_type;
};

 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A,class B>
   concept_map ConstCallable10<F,impl::Bind2,impl::Bind3,impl::Bind4,impl::Bind5,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable6<F,A,ArgW,ArgX,ArgY,ArgZ,B>::result_type result_type;
};

 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A,class B>
   concept_map ConstCallable10<F,impl::Bind1,impl::Bind2,impl::Bind3,impl::Bind6,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable6<F,ArgW,ArgX,ArgY,A,B,ArgZ>::result_type result_type;
};

 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A,class B>
   concept_map ConstCallable10<F,impl::Bind1,impl::Bind2,impl::Bind4,impl::Bind6,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable6<F,ArgW,ArgX,A,ArgY,B,ArgZ>::result_type result_type;
};

 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A,class B>
   concept_map ConstCallable10<F,impl::Bind1,impl::Bind3,impl::Bind4,impl::Bind6,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable6<F,ArgW,A,ArgX,ArgY,B,ArgZ>::result_type result_type;
};

// These are the ones for bindVtoZof6 
// 6 needed in total to match the helper functions.
 template <class F,class ArgV,class ArgW,class ArgX,class ArgY,class ArgZ,class A>
   concept_map ConstCallable11<F,impl::Bind1,impl::Bind2,impl::Bind3,impl::Bind4,impl::Bind5,
                               ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  typedef ConstCallable6<F,ArgV,ArgW,ArgX,ArgY,ArgZ,A>::result_type result_type;
};

 template <class F,class ArgV,class ArgW,class ArgX,class ArgY,class ArgZ,class A>
   concept_map ConstCallable11<F,impl::Bind1,impl::Bind2,impl::Bind3,impl::Bind4,impl::Bind6,
                               ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  typedef ConstCallable6<F,ArgV,ArgW,ArgX,ArgY,A,ArgZ>::result_type result_type;
};

// These are the ones for bindXof7 - 7 needed in total to match the helper functions.
 template <class F,class ArgX,class A,class B,class C,class D, class E, class G>
   concept_map ConstCallable8<F,impl::Bind1,ArgX,A,B,C,D,E,G>
{
  typedef ConstCallable7<F,ArgX,A,B,C,D,E,G>::result_type result_type;
};

 template <class F,class ArgX,class A,class B,class C,class D, class E, class G>
   concept_map ConstCallable8<F,impl::Bind2,ArgX,A,B,C,D,E,G>
{
  typedef ConstCallable7<F,A,ArgX,B,C,D,E,G>::result_type result_type;
};

 template <class F,class ArgX,class A,class B,class C,class D, class E, class G>
   concept_map ConstCallable8<F,impl::Bind3,ArgX,A,B,C,D,E,G>
{
  typedef ConstCallable7<F,A,B,ArgX,C,D,E,G>::result_type result_type;
};

 template <class F,class ArgX,class A,class B,class C,class D, class E, class G>
   concept_map ConstCallable8<F,impl::Bind4,ArgX,A,B,C,D,E,G>
{
  typedef ConstCallable7<F,A,B,C,ArgX,D,E,G>::result_type result_type;
};

 template <class F,class ArgX,class A,class B,class C,class D, class E, class G>
   concept_map ConstCallable8<F,impl::Bind5,ArgX,A,B,C,D,E,G>
{
  typedef ConstCallable7<F,A,B,C,D,ArgX,E,G>::result_type result_type;
};

 template <class F,class ArgX,class A,class B,class C,class D, class E, class G>
   concept_map ConstCallable8<F,impl::Bind6,ArgX,A,B,C,D,E,G>
{
  typedef ConstCallable7<F,A,B,C,D,E,ArgX,G>::result_type result_type;
};

 template <class F,class ArgX,class A,class B,class C,class D, class E, class G>
   concept_map ConstCallable8<F,impl::Bind7,ArgX,A,B,C,D,E,G>
{
  typedef ConstCallable7<F,A,B,C,D,E,G,ArgX>::result_type result_type;
};

// These are used by binderXandYof7 - more are needed.
template <class F,class ArgX,class ArgY,class A,class B,class C,class D, class E>
  concept_map ConstCallable9<F,impl::Bind1,impl::Bind2,ArgX,ArgY,A,B,C,D,E>
{
  typedef ConstCallable7<F,ArgX,ArgY,A,B,C,D,E>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C,class D, class E>
  concept_map ConstCallable9<F,impl::Bind1,impl::Bind3,ArgX,ArgY,A,B,C,D,E>
{
  typedef ConstCallable7<F,ArgX,A,ArgY,B,C,D,E>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C,class D, class E>
  concept_map ConstCallable9<F,impl::Bind1,impl::Bind4,ArgX,ArgY,A,B,C,D,E>
{
  typedef ConstCallable7<F,ArgX,A,B,ArgY,C,D,E>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C,class D, class E>
  concept_map ConstCallable9<F,impl::Bind1,impl::Bind5,ArgX,ArgY,A,B,C,D,E>
{
  typedef ConstCallable7<F,ArgX,A,B,C,ArgY,D,E>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C,class D, class E>
  concept_map ConstCallable9<F,impl::Bind1,impl::Bind6,ArgX,ArgY,A,B,C,D,E>
{
  typedef ConstCallable7<F,ArgX,A,B,C,D,ArgY,E>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C,class D, class E>
  concept_map ConstCallable9<F,impl::Bind1,impl::Bind7,ArgX,ArgY,A,B,C,D,E>
{
  typedef ConstCallable7<F,ArgX,A,B,C,D,E,ArgY>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C,class D, class E>
  concept_map ConstCallable9<F,impl::Bind2,impl::Bind3,ArgX,ArgY,A,B,C,D,E>
{
  typedef ConstCallable7<F,A,ArgX,ArgY,B,C,D,E>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C,class D, class E>
  concept_map ConstCallable9<F,impl::Bind6,impl::Bind7,ArgX,ArgY,A,B,C,D,E>
{
  typedef ConstCallable7<F,A,B,C,D,E,ArgX,ArgY>::result_type result_type;
};

// These are used by binderXtoZof7 - more are needed.
template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C,class D>
  concept_map ConstCallable10<F,impl::Bind1,impl::Bind2,impl::Bind3,ArgX,ArgY,ArgZ,A,B,C,D>
{
  typedef ConstCallable7<F,ArgX,ArgY,ArgZ,A,B,C,D>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C,class D>
  concept_map ConstCallable10<F,impl::Bind1,impl::Bind2,impl::Bind4,ArgX,ArgY,ArgZ,A,B,C,D>
{
  typedef ConstCallable7<F,ArgX,ArgY,A,ArgZ,B,C,D>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C,class D>
  concept_map ConstCallable10<F,impl::Bind1,impl::Bind2,impl::Bind5,ArgX,ArgY,ArgZ,A,B,C,D>
{
  typedef ConstCallable7<F,ArgX,ArgY,A,B,ArgZ,C,D>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C,class D>
  concept_map ConstCallable10<F,impl::Bind1,impl::Bind2,impl::Bind6,ArgX,ArgY,ArgZ,A,B,C,D>
{
  typedef ConstCallable7<F,ArgX,ArgY,A,B,C,ArgZ,D>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C,class D>
  concept_map ConstCallable10<F,impl::Bind1,impl::Bind2,impl::Bind7,ArgX,ArgY,ArgZ,A,B,C,D>
{
  typedef ConstCallable7<F,ArgX,ArgY,A,B,C,D,ArgZ>::result_type result_type;
};

// These are the ones for bindWtoZof7
// ? needed in total to match the helper functions.
 template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A,class B,class C>
   concept_map ConstCallable11<F,impl::Bind1,impl::Bind2,impl::Bind3,impl::Bind4,
   ArgW,ArgX,ArgY,ArgZ,A,B,C>
{
  typedef ConstCallable7<F,ArgW,ArgX,ArgY,ArgZ,A,B,C>::result_type result_type;
};

// These are the ones for bindVtoZof7 - 
// ? needed in total to match the helper functions.
template <class F,class ArgV,class ArgW,class ArgX,class ArgY,class ArgZ,class A, class B>
   concept_map ConstCallable12<F,impl::Bind1,impl::Bind2,impl::Bind3,impl::Bind4,impl::Bind5,
  ArgV,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  typedef ConstCallable7<F,ArgV,ArgW,ArgX,ArgY,ArgZ,A,B>::result_type result_type;
};

// These are the ones for bindUtoZof7 - 
// 7 needed in total to match the helper functions.
template <class F,class ArgU,class ArgV,class ArgW,class ArgX,class ArgY,class ArgZ,class A>
  concept_map ConstCallable13<F,impl::Bind1,impl::Bind2,impl::Bind3,impl::Bind4,impl::Bind5,impl::Bind6,
  ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  typedef ConstCallable7<F,ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A>::result_type result_type;
};

// These are the ones for bindXof8 - 8 needed in total to match the helper functions.
template <class F,class ArgX,class A,class B,class C,class D, class E, class G, class H>
  concept_map ConstCallable9<F,impl::Bind1,ArgX,A,B,C,D,E,G,H>
{
  typedef ConstCallable8<F,ArgX,A,B,C,D,E,G,H>::result_type result_type;
};

template <class F,class ArgX,class A,class B,class C,class D, class E, class G, class H>
  concept_map ConstCallable9<F,impl::Bind2,ArgX,A,B,C,D,E,G,H>
{
  typedef ConstCallable8<F,A,ArgX,B,C,D,E,G,H>::result_type result_type;
};

template <class F,class ArgX,class A,class B,class C,class D, class E, class G, class H>
  concept_map ConstCallable9<F,impl::Bind3,ArgX,A,B,C,D,E,G,H>
{
  typedef ConstCallable8<F,A,B,ArgX,C,D,E,G,H>::result_type result_type;
};

// These are used by binderXandYof8 - more are needed.
template <class F,class ArgX,class ArgY,class A,class B,class C,class D, class E, class G>
  concept_map ConstCallable10<F,impl::Bind1,impl::Bind2,ArgX,ArgY,A,B,C,D,E,G>
{
  typedef ConstCallable8<F,ArgX,ArgY,A,B,C,D,E,G>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C,class D, class E, class G>
  concept_map ConstCallable10<F,impl::Bind1,impl::Bind3,ArgX,ArgY,A,B,C,D,E,G>
{
  typedef ConstCallable8<F,ArgX,A,ArgY,B,C,D,E,G>::result_type result_type;
};

template <class F,class ArgX,class ArgY,class A,class B,class C,class D, class E,class G>
  concept_map ConstCallable10<F,impl::Bind2,impl::Bind3,ArgX,ArgY,A,B,C,D,E,G>
{
  typedef ConstCallable8<F,A,ArgX,ArgY,B,C,D,E,G>::result_type result_type;
};

// These are used by binderXtoZof8 - more are needed.
template <class F,class ArgX,class ArgY,class ArgZ,class A,class B,class C,class D,class E>
  concept_map ConstCallable11<F,impl::Bind1,impl::Bind2,impl::Bind3,ArgX,ArgY,ArgZ,A,B,C,D,E>
{
  typedef ConstCallable8<F,ArgX,ArgY,ArgZ,A,B,C,D,E>::result_type result_type;
};

// These are used by binderWtoZof8 - more are needed.
template <class F,class ArgW,class ArgX,class ArgY,class ArgZ,class A,class B,class C,class D>
  concept_map ConstCallable12<F,impl::Bind1,impl::Bind2,impl::Bind3,impl::Bind4,ArgW,ArgX,ArgY,ArgZ,A,B,C,D>
{
  typedef ConstCallable8<F,ArgW,ArgX,ArgY,ArgZ,A,B,C,D>::result_type result_type;
};


#endif

#endif
