//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FULL5_DOT_H
#define FCPP_FULL5_DOT_H

//////////////////////////////////////////////////////////////////////
// Implementation of Full5. (Moved from full.h)
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Full5 is now complete.
//
// Full5 is uses some binders in the style used in Full4,
// but the number needed would be very large.
// Many are supplied instead using BindXof5, BindXandYof5, etc.
// In these the choice of which parameter is to be bound is 
// controlled by objects of type e.g. Bind1.
// These binders can be called directly but are expected to be 
// used via currying with underscore, which hides the details 
// from the user. See fcpptest5.cpp for examples.
//////////////////////////////////////////////////////////////////////
#ifdef FCPP_CONCEPTS
//////////////////////////////////////////////////////////////////////
// Space for the concept maps for Full5
//////////////////////////////////////////////////////////////////////
template <class F,class X>
concept_map ConstCallable5<F,X>
{
  typedef Full4<impl::binder1of5<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable5<F,X,AutoCurryType>
{
  typedef Full4<impl::binder1of5<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable5<F,X,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binder1of5<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable5<F,X,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binder1of5<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable5<F,X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binder1of5<F,X> > result_type;
};

template <class F,class Y>
  concept_map ConstCallable5<F,AutoCurryType,Y>
{
  typedef Full4<impl::binder2of5<F,Y> > result_type;
};

template <class F,class Y>
  concept_map ConstCallable5<F,AutoCurryType,Y,AutoCurryType>
{
  typedef Full4<impl::binder2of5<F,Y> > result_type;
};

template <class F,class Y>
  concept_map ConstCallable5<F,AutoCurryType,Y,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binder2of5<F,Y> > result_type;
};

template <class F,class Y>
  concept_map ConstCallable5<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binder2of5<F,Y> > result_type;
};

template <class F,class Z>
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,Z>
{
  typedef Full4<impl::binderAof5<F,Bind3,Z> > result_type;
};

template <class F,class Z>
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,Z,AutoCurryType>
{
  typedef Full4<impl::binderAof5<F,Bind3,Z> > result_type;
};

template <class F,class Z>
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binderAof5<F,Bind3,Z> > result_type;
};

template <class F,class V>
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,AutoCurryType,V>
{
  typedef Full4<impl::binderAof5<F,Bind4,V> > result_type;
};

template <class F,class V>
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,AutoCurryType,V,AutoCurryType>
{
  typedef Full4<impl::binderAof5<F,Bind4,V> > result_type;
};

template <class F,class W>
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,W>
{
  typedef Full4<impl::binderAof5<F,Bind5,W> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable5<F,X,Y>
{
  typedef Full3<impl::binder1and2of5<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable5<F,X,Y,AutoCurryType>
{
  typedef Full3<impl::binder1and2of5<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable5<F,X,Y,AutoCurryType,AutoCurryType>
{
  typedef Full3<impl::binder1and2of5<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable5<F,X,Y,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full3<impl::binder1and2of5<F,X,Y> > result_type;
};

// Start made on using binderXandYof5 for the general two parameter binding.
// This involves changes to the corresponding Curryable5Helper and
// also to the Sig definitions in Full5 for backward compatibility.
template <class F,class X, class Z>
  concept_map ConstCallable5<F,X,AutoCurryType,Z>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class X, class Z>
  concept_map ConstCallable5<F,X,AutoCurryType,Z,AutoCurryType>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class X, class Z>
  concept_map ConstCallable5<F,X,AutoCurryType,Z,AutoCurryType,AutoCurryType>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class X, class V>
  concept_map ConstCallable5<F,X,AutoCurryType,AutoCurryType,V>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind4,V> > > result_type;
};

template <class F,class X, class V>
  concept_map ConstCallable5<F,X,AutoCurryType,AutoCurryType,V,AutoCurryType>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind4,V> > > result_type;
};

template <class F,class X, class W>
  concept_map ConstCallable5<F,X,AutoCurryType,AutoCurryType,AutoCurryType,W>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind5,W> > > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable5<F,AutoCurryType,Y,Z>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable5<F,AutoCurryType,Y,Z,AutoCurryType>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable5<F,AutoCurryType,Y,Z,AutoCurryType,AutoCurryType>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Y, class V>
  concept_map ConstCallable5<F,AutoCurryType,Y,AutoCurryType,V>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind4,V> > > result_type;
};

template <class F,class Y, class V>
  concept_map ConstCallable5<F,AutoCurryType,Y,AutoCurryType,V,AutoCurryType>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind4,V> > > result_type;
};

template <class F,class Y, class W>
  concept_map ConstCallable5<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,W>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind5,W> > > result_type;
};

template <class F,class Z, class V>
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,Z,V>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind3,Z>,std::pair<Bind4,V> > > result_type;
};

template <class F,class Z, class V>
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,Z,V,AutoCurryType>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind3,Z>,std::pair<Bind4,V> > > result_type;
};

template <class F,class Z, class W>
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,W>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind3,Z>,std::pair<Bind5,W> > > result_type;
};

template <class F,class V, class W> 
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,AutoCurryType,V,W>
{
  typedef Full3<impl::binderXandYof5<F,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

template <class F,class X, class Y, class Z>
   concept_map ConstCallable5<F,X,Y,Z>
{
  typedef Full2<impl::binder1to3of5<F,X,Y,Z> > result_type;
};

template <class F,class X, class Y, class Z>
   concept_map ConstCallable5<F,X,Y,Z,AutoCurryType>
{
  typedef Full2<impl::binder1to3of5<F,X,Y,Z> > result_type;
};

template <class F,class X, class Y, class Z>
   concept_map ConstCallable5<F,X,Y,Z,AutoCurryType,AutoCurryType>
{
  typedef Full2<impl::binder1to3of5<F,X,Y,Z> > result_type;
};

template <class F,class X, class Y, class V>
  concept_map ConstCallable5<F,X,Y,AutoCurryType,V>
{
  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V> > > result_type;
};

template <class F,class X, class Y, class V>
  concept_map ConstCallable5<F,X,Y,AutoCurryType,V,AutoCurryType>
{
  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V> > > result_type;
};

template <class F,class X, class Y, class W>
  concept_map ConstCallable5<F,X,Y,AutoCurryType,AutoCurryType,W>
{
  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind5,W> > > result_type;
};

template <class F,class X, class Z, class V>
  concept_map ConstCallable5<F,X,AutoCurryType,Z,V>
{
  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > result_type;
};

template <class F,class X, class Z, class V>
  concept_map ConstCallable5<F,X,AutoCurryType,Z,V,AutoCurryType>
{
  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > result_type;
};

template <class F,class X, class Z, class W>
  concept_map ConstCallable5<F,X,AutoCurryType,Z,AutoCurryType,W>
{
  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind5,W> > > result_type;
};

template <class F,class X, class V, class W>
  concept_map ConstCallable5<F,X,AutoCurryType,AutoCurryType,V,W>
{
  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

template <class F,class Y, class Z, class V>
  concept_map ConstCallable5<F,AutoCurryType,Y,Z,V>
{

  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > result_type;
};

template <class F,class Y, class Z, class V>
  concept_map ConstCallable5<F,AutoCurryType,Y,Z,V,AutoCurryType>
{

  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > result_type;
};

template <class F,class Y, class Z, class W>
  concept_map ConstCallable5<F,AutoCurryType,Y,Z,AutoCurryType,W>
{
  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,W> > > result_type;
};

template <class F,class Y, class V, class W>
  concept_map ConstCallable5<F,AutoCurryType,Y,AutoCurryType,V,W>
{
  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

template <class F,class Z, class V, class W>
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,Z,V,W>
{
  typedef Full2<impl::binderXtoZof5<F,std::pair<Bind3,Z>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

 template <class F,class X, class Y, class Z, class V>
   concept_map ConstCallable5<F,X,Y,Z,V>
{
  typedef Full1<impl::binder1to4of5<F,X,Y,Z,V> > result_type;
};

 template <class F,class X, class Y, class Z, class V>
   concept_map ConstCallable5<F,X,Y,Z,V,AutoCurryType>
{
  typedef Full1<impl::binder1to4of5<F,X,Y,Z,V> > result_type;
};

 template <class F,class X, class Y, class Z, class W>
   concept_map ConstCallable5<F,X,Y,Z,AutoCurryType,W>
{
  typedef Full1<impl::binderWtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,W> > > result_type;
};

 template <class F,class X, class Y, class V, class W>
   concept_map ConstCallable5<F,X,Y,AutoCurryType,V,W>
{
  typedef Full1<impl::binderWtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

 template <class F,class X, class Z, class V, class W>
   concept_map ConstCallable5<F,X,AutoCurryType,Z,V,W>
{
  typedef Full1<impl::binderWtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

 template <class F,class Y, class Z, class V, class W>
   concept_map ConstCallable5<F,AutoCurryType,Y,Z,V,W>
{
  typedef Full1<impl::binderWtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

#endif


template <class F>
class Full5 : public SmartFunctoid5 {
   F f;
public:
   Full5() : f() {}
   Full5( const F& ff ) : f(ff) {}
#ifdef FCPP_ENABLE_LAMBDA
   typedef Full5 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
       {  
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "Full5 operator[] called" << std::endl;
#endif
      return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
/////////////  copied from curryable4; added impl:: to all binders  //
#ifndef FCPP_CONCEPTS
   template <class X, class Y=void, class Z=void, class V=void, class W=void>
   struct Sig
   : public FunType<
                    typename F::template Sig<X,Y,Z,V,W>::Arg1Type,
                    typename F::template Sig<X,Y,Z,V,W>::Arg2Type,
                    typename F::template Sig<X,Y,Z,V,W>::Arg3Type,
                    typename F::template Sig<X,Y,Z,V,W>::Arg4Type,
                    typename F::template Sig<X,Y,Z,V,W>::Arg5Type,
                    typename RT<F,X,Y,Z,V,W>::ResultType> {};

  // These are needed to get bind1to4of4 to work.
    template <class X> struct Sig<X>
   : public FunType<X,Full4<impl::binder1of5<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType>
   : public FunType<X,AutoCurryType,
                    Full4<impl::binder1of5<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType>
   : public FunType<X,AutoCurryType,AutoCurryType,
                    Full4<impl::binder1of5<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType>
   : public FunType<X,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full4<impl::binder1of5<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
   : public FunType<X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full4<impl::binder1of5<F,X> > > {};


    template <class Y> struct Sig<AutoCurryType,Y> 
   : public FunType<Y,Full4<impl::binder2of5<F,Y> > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType>
     : public FunType<AutoCurryType,Y,AutoCurryType,
                    Full4<impl::binder2of5<F,Y> > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,Y,AutoCurryType,AutoCurryType,
                    Full4<impl::binder2of5<F,Y> > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full4<impl::binder2of5<F,Y> > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z> 
     : public FunType<Z,Full4<impl::binderAof5<F,Bind3,Z> > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType> 
     : public FunType<Z,Full4<impl::binderAof5<F,Bind3,Z> > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType> 
     : public FunType<Z,Full4<impl::binderAof5<F,Bind3,Z> > > {};

   template <class V> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,V> 
     : public FunType<V,Full4<impl::binderAof5<F,Bind4,V> > > {};

   template <class V> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,V,AutoCurryType> 
     : public FunType<V,Full4<impl::binderAof5<F,Bind4,V> > > {};

   template <class W> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,W> 
     : public FunType<W,Full4<impl::binderAof5<F,Bind5,W> > > {};


   template <class X, class Y> struct Sig<X,Y> 
   : public FunType<X,Y,Full3<impl::binder1and2of5<F,X,Y> > > {};

   template <class X, class Y> struct Sig<X,Y,AutoCurryType> 
   : public FunType<X,Y,Full3<impl::binder1and2of5<F,X,Y> > > {};

   template <class X, class Y> struct Sig<X,Y,AutoCurryType,AutoCurryType> 
   : public FunType<X,Y,Full3<impl::binder1and2of5<F,X,Y> > > {};

   template <class X, class Y> struct Sig<X,Y,AutoCurryType,AutoCurryType,AutoCurryType> 
   : public FunType<X,Y,Full3<impl::binder1and2of5<F,X,Y> > > {};

   // NOTE: The calls can be simplified as follows.
   // binderXandYof5    for the two parameter calls (completed)
   // binderXtoZof5     for the three parameter calls
   // binderWtoZof5     for the four parameter calls (completed)
   // These need to implemented here and also in the 
   // corresponding Curryable5Helper functions.
   // For concepts the concept maps also have to be changed (or put in).
   // Examples of all now exist but the work is incomplete.
   template <class X, class Z> struct Sig<X,AutoCurryType,Z>
     : public FunType<X,Z,Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > > {};

   template <class X, class Z> struct Sig<X,AutoCurryType,Z,AutoCurryType>
    : public FunType<X,Z,Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > > {};

   template <class X, class Z> struct Sig<X,AutoCurryType,Z,AutoCurryType,AutoCurryType>
     : public FunType<X,Z,Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > > {};

   template <class X, class V> struct Sig<X,AutoCurryType,AutoCurryType,V>
     : public FunType<X,V,Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind4,V> > > > {};

   template <class X, class V> struct Sig<X,AutoCurryType,AutoCurryType,V,AutoCurryType>
     : public FunType<X,V,Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind4,V> > > > {};

   template <class X, class W> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,W>
     : public FunType<X,W,Full3<impl::binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind5,W> > > > {};

   template <class Y, class Z> struct Sig<AutoCurryType,Y,Z>
     : public FunType<Y,Z,Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class Y, class Z> struct Sig<AutoCurryType,Y,Z,AutoCurryType>
     : public FunType<Y,Z,Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class Y, class Z> struct Sig<AutoCurryType,Y,Z,AutoCurryType,AutoCurryType>
     : public FunType<Y,Z,Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class Y, class V> struct Sig<AutoCurryType,Y,AutoCurryType,V>
     : public FunType<Y,V,Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind4,V> > > > {};

   template <class Y, class V> struct Sig<AutoCurryType,Y,AutoCurryType,V,AutoCurryType>
     : public FunType<Y,V,Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind4,V> > > > {};

   template <class Y, class W> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,W>
     : public FunType<Y,W,Full3<impl::binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind5,W> > > > {};

   template <class Z, class V> struct Sig<AutoCurryType,AutoCurryType,Z,V>
     : public FunType<Z,V,Full3<impl::binderXandYof5<F,std::pair<Bind3,Z>,std::pair<Bind4,V> > > > {};

   template <class Z, class V> struct Sig<AutoCurryType,AutoCurryType,Z,V,AutoCurryType>
     : public FunType<Z,V,Full3<impl::binderXandYof5<F,std::pair<Bind3,Z>,std::pair<Bind4,V> > > > {};

   template <class Z, class W> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,W>
     : public FunType<Z,W,Full3<impl::binderXandYof5<F,std::pair<Bind3,Z>,std::pair<Bind5,W> > > > {};

   template <class V, class W> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,V,W>
     : public FunType<V,W,Full3<impl::binderXandYof5<F,std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z>
     : public FunType<X,Y,Z,Full2<impl::binder1to3of5<F,X,Y,Z> > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z,AutoCurryType>
     : public FunType<X,Y,Z,Full2<impl::binder1to3of5<F,X,Y,Z> > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z,AutoCurryType,AutoCurryType>
     : public FunType<X,Y,Z,Full2<impl::binder1to3of5<F,X,Y,Z> > > {};

   template <class X, class Y, class V> struct Sig<X,Y,AutoCurryType,V>
     : public FunType<X,Y,V,Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V> > > > {};

   template <class X, class Y, class V> struct Sig<X,Y,AutoCurryType,V,AutoCurryType>
     : public FunType<X,Y,V,Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V> > > > {};

   template <class X, class Y, class W> struct Sig<X,Y,AutoCurryType,AutoCurryType,W>
     : public FunType<X,Y,W,Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind5,W> > > > {};

   template <class X, class Z, class V> struct Sig<X,AutoCurryType,Z,V>
     : public FunType<X,Z,V,Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > > {};

   template <class X, class Z, class V> struct Sig<X,AutoCurryType,Z,V,AutoCurryType>
     : public FunType<X,Z,V,Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > > {};

   template <class X, class Z, class W> struct Sig<X,AutoCurryType,Z,AutoCurryType,W>
     : public FunType<X,Z,W,Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind5,W> > > > {};

   template <class X, class V, class W> struct Sig<X,AutoCurryType,AutoCurryType,V,W>
     : public FunType<X,V,W,Full2<impl::binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};

   template <class Y, class Z, class V> struct Sig<AutoCurryType,Y,Z,V>
     : public FunType<Y,Z,V,Full2<impl::binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > > {};

   template <class Y, class Z, class V> struct Sig<AutoCurryType,Y,Z,V,AutoCurryType>
     : public FunType<Y,Z,V,Full2<impl::binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > > {};

   template <class Y, class Z, class W> struct Sig<AutoCurryType,Y,Z,AutoCurryType,W>
     : public FunType<Y,Z,W,Full2<impl::binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,W> > > > {};

   template <class Y, class V, class W> struct Sig<AutoCurryType,Y,AutoCurryType,V,W>
     : public FunType<Y,V,W,Full2<impl::binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};

   template <class Z, class V, class W> struct Sig<AutoCurryType,AutoCurryType,Z,V,W>
     : public FunType<Z,V,W,Full2<impl::binderXtoZof5<F,std::pair<Bind3,Z>,std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};

   // Use one binder where it is available.
   template <class X, class Y, class Z, class V> struct Sig<X,Y,Z,V,AutoCurryType>
     : public FunType<X,Y,Z,V,Full1<impl::binder1to4of5<F,X,Y,Z,V> > > {};

   template <class X, class Y, class Z, class W> struct Sig<X,Y,Z,AutoCurryType,W>
     : public FunType<X,Y,Z,W,Full1<impl::binderWtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,
                                                                   std::pair<Bind3,Z>,std::pair<Bind5,W> > > > {};

   template <class X, class Y, class V, class W> struct Sig<X,Y,AutoCurryType,V,W>
     : public FunType<X,Y,V,W,Full1<impl::binderWtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,
                                                                   std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};

   template <class X, class Z, class V, class W> struct Sig<X,AutoCurryType,Z,V,W>
     : public FunType<X,Z,V,W,Full1<impl::binderWtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,
                                                                   std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};

   template <class Y, class Z, class V, class W> struct Sig<AutoCurryType,Y,Z,V,W>
     : public FunType<Y,Z,V,W,Full1<impl::binderWtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,
                                                                   std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};

   template <class X, class Y, class Z, class V> struct Sig<X,Y,Z,V>
     : public FunType<X,Y,Z,V,Full1<impl::binder1to4of5<F,X,Y,Z,V> > > {};

#endif


   // This is needed to get bind1to4of4 to work.
   template <class X>
#ifdef FCPP_CONCEPTS
   inline ConstCallable5<F,X>::result_type
#else
   typename Sig<X>::ResultType
#endif
    operator()( const X& x ) const {
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "full5 binder1of5" << std::endl;
#endif
      return makeFull4(impl::binder1of5<F,X>(f,x));
   }

   template <class X, class Y>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<F,X,Y>::result_type
#else
   typename Sig<X,Y>::ResultType 
#endif
   operator()( const X& x, const Y& y ) const {
     // Curryable5Helper4
#ifdef FCPP_CONCEPTS
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper4 used with concepts" << std::endl;
#endif
     return impl::Curryable5Helper4<typename ConstCallable5<F,X,Y>::result_type,F,X,Y>::go(f,x,y);
#else
     return impl::Curryable5Helper4<typename Sig<X,Y>::ResultType,F,X,Y>    
      ::go(f,x,y);
#endif
     //return makeFull3(impl::binder1and2of5<F,X,Y>(f,x,y));
   }

   template <class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<F,X,Y,Z>::result_type
#else
     typename Sig<X,Y,Z>::ResultType 
#endif
   operator()( const X& x, const Y& y,
       const Z& z) const {
     // Curryable5Helper3
#ifdef FCPP_CONCEPTS
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper3 used with concepts" << std::endl;
#endif
     return impl::Curryable5Helper3<typename ConstCallable5<F,X,Y,Z>::result_type,F,X,Y,Z>::go(f,x,y,z);
#else
     return impl::Curryable5Helper3<typename Sig<X,Y,Z>::ResultType,F,X,Y,Z>
          ::go(f,x,y,z);
#endif
   }

   template <class X, class Y, class Z, class W>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<F,X,Y,Z,W>::result_type
#else
     typename Sig<X,Y,Z,W>::ResultType 
#endif
   operator()( const X& x, const Y& y,
				   const Z& z,const W& w) const {
     // Curryable5Helper2
#ifdef FCPP_CONCEPTS
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 used with concepts" << std::endl;
#endif
     return impl::Curryable5Helper2<typename ConstCallable5<F,X,Y,Z,W>::result_type,F,X,Y,Z,W>::go(f,x,y,z,w);
#else
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 used without concepts" << std::endl;
#endif
     return impl::Curryable5Helper2<typename Sig<X,Y,Z,W>::ResultType,F,X,Y,Z,W>
      ::go(f,x,y,z,w);
#endif
   }

   template <class X, class Y, class Z, class V, class W>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<F,X,Y,Z,V,W>::result_type
#else
   inline typename Sig<X,Y,Z,V,W>::ResultType
#endif
   operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w ) const {
      // need partial specialization, so defer to a class helper
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Full5 operator() called with  5 args" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "ConstCallable5 used" << std::endl;
#endif
     return impl::Curryable5Helper<typename ConstCallable5<F,X,Y,Z,V,W>::result_type,F,X,Y,Z,V,W>::go(f,x,y,z,v,w);
#else
      return impl::Curryable5Helper<typename Sig<X,Y,Z,V,W>::ResultType,F,X,Y,Z,V,W>
      ::go(f,x,y,z,v,w);
#endif
   }
#ifdef FCPP_DEBUG
   std::string name() const {
      return debug_string_and_name_or_string("Full5::",f,"Full5");
   }
#endif

};




#endif
