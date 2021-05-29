//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FULL7_DOT_H
#define FCPP_FULL7_DOT_H

//////////////////////////////////////////////////////////////////////
// Implementation of Full7. (Moved from full.h)
//////////////////////////////////////////////////////////////////////

#ifdef FCPP_CONCEPTS
//////////////////////////////////////////////////////////////////////
// Space for the concept maps for Full7
//////////////////////////////////////////////////////////////////////
template <class F,class X>
concept_map ConstCallable7<F,X>
{
  typedef Full6<impl::binder1of7<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable7<F,X,AutoCurryType>
{
  typedef Full6<impl::binder1of7<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable7<F,X,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binder1of7<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable7<F,X,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binder1of7<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable7<F,X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binder1of7<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable7<F,X,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binder1of7<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable7<F,X,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binder1of7<F,X> > result_type;
};

template <class F,class Y>
  concept_map ConstCallable7<F,AutoCurryType,Y>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable7<F,AutoCurryType,Y,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable7<F,AutoCurryType,Y,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable7<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,
                               AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable7<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,
                               AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable7<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,
                               AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Z>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,Z>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > result_type;
};


template <class F,class Z>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,Z,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Z>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,
                               AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Z>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,
                               AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Z>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,
                               AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class T>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,T>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind4,T> > > result_type;
};

template <class F,class T>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,T,
                               AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind4,T> > > result_type;
};

template <class F,class T>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,T,
                               AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind4,T> > > result_type;
};

template <class F,class T>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,T,
                               AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind4,T> > > result_type;
};

template <class F,class U>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,U>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind5,U> > > result_type;
};

template <class F,class U>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,U,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind5,U> > > result_type;
};

template <class F,class U>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,U,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind5,U> > > result_type;
};

template <class F,class V>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,AutoCurryType,V>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind6,V> > > result_type;
};

template <class F,class V>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,AutoCurryType,V,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind6,V> > > result_type;
};

template <class F,class W>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,AutoCurryType,AutoCurryType,W>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind7,W> > > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable7<F,X,Y>
{
  typedef Full5<impl::binder1and2of7<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable7<F,X,Y,AutoCurryType>
{
  typedef Full5<impl::binder1and2of7<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable7<F,X,Y,AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binder1and2of7<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable7<F,X,Y,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binder1and2of7<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable7<F,X,Y,AutoCurryType,AutoCurryType,
                                    AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binder1and2of7<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable7<F,X,Y,AutoCurryType,AutoCurryType,
                                    AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binder1and2of7<F,X,Y> > result_type;
};

template <class F,class X, class Z>
  concept_map ConstCallable7<F,X,AutoCurryType,Z>
{
  typedef Full5<impl::binderXandYof7<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable7<F,AutoCurryType,Y,Z>
{
  typedef Full5<impl::binderXandYof7<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class V, class W>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,AutoCurryType,V,W>
{
  typedef Full5<impl::binderXandYof7<F,std::pair<Bind6,V>,std::pair<Bind7,W> > > result_type;
};


 template <class F,class X, class Y, class Z>
   concept_map ConstCallable7<F,X,Y,Z>
{
  typedef Full4<impl::binder1to3of7<F,X,Y,Z> > result_type;
};

 template <class F,class X, class Y, class Z>
   concept_map ConstCallable7<F,X,Y,Z,AutoCurryType>
{
  typedef Full4<impl::binder1to3of7<F,X,Y,Z> > result_type;
};

 template <class F,class X, class Y, class Z>
   concept_map ConstCallable7<F,X,Y,Z,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binder1to3of7<F,X,Y,Z> > result_type;
};

 template <class F,class X, class Y, class T>
   concept_map ConstCallable7<F,X,Y,AutoCurryType,T>
{
  typedef Full4<impl::binderXtoZof7<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,T> > > result_type;
};

 template <class F,class X, class Y, class T>
   concept_map ConstCallable7<F,X,Y,AutoCurryType,T,AutoCurryType>
{
  typedef Full4<impl::binderXtoZof7<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,T> > > result_type;
};

 template <class F,class X, class Y, class U>
   concept_map ConstCallable7<F,X,Y,AutoCurryType,AutoCurryType,U>
{
  typedef Full4<impl::binderXtoZof7<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind5,U> > > result_type;
};

 template <class F, class W, class X, class Y, class Z>
   concept_map ConstCallable7<F,W,X,Y,Z>
{
  typedef Full3<impl::binder1to4of7<F,W,X,Y,Z> > result_type;
};


 template <class F, class W, class X, class Y, class Z>
   concept_map ConstCallable7<F,W,X,Y,Z,AutoCurryType>
{
  typedef Full3<impl::binder1to4of7<F,W,X,Y,Z> > result_type;
};

 template <class F, class V, class W, class X, class Y, class Z>
   concept_map ConstCallable7<F,V,W,X,Y,Z>
{
  typedef Full2<impl::binder1to5of7<F,V,W,X,Y,Z> > result_type;
};

 template <class F, class U, class V, class W, class X, class Y, class Z>
   concept_map ConstCallable7<F,U,V,W,X,Y,Z>
{
  typedef Full1<impl::binder1to6of7<F,U,V,W,X,Y,Z> > result_type;
};

#endif

//////////////////////////////////////////////////////////////////////
// Full7
//////////////////////////////////////////////////////////////////////

template <class F>
class Full7 : public SmartFunctoid7 {
   F f;
public:
   Full7() : f() {}
   Full7( const F& ff ) : f(ff) {}
#ifdef FCPP_ENABLE_LAMBDA
   typedef Full7 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif

#ifndef FCPP_CONCEPTS
   template <class X, class Y=void, class Z=void, class T=void, class U=void,class V=void, class W=void>
   struct Sig
   : public FunType<
     typename F::template Sig<X,Y,Z,T,U,V,W>::Arg1Type,
     typename F::template Sig<X,Y,Z,T,U,V,W>::Arg2Type,
     typename F::template Sig<X,Y,Z,T,U,V,W>::Arg3Type,
     typename F::template Sig<X,Y,Z,T,U,V,W>::Arg4Type,
     typename F::template Sig<X,Y,Z,T,U,V,W>::Arg5Type,
     typename F::template Sig<X,Y,Z,T,U,V,W>::Arg6Type,
     typename F::template Sig<X,Y,Z,T,U,V,W>::Arg7Type,
     typename RT<F,X,Y,Z,T,U,V,W>::ResultType> {};

    template <class X> struct Sig<X> 
   : public FunType<X,Full6<impl::binder1of7<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType>
   : public FunType<X,Full6<impl::binder1of7<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType>
   : public FunType<X,Full6<impl::binder1of7<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType>
   : public FunType<X,Full6<impl::binder1of7<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,
                                   AutoCurryType>
   : public FunType<X,Full6<impl::binder1of7<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,
                                   AutoCurryType,AutoCurryType>
   : public FunType<X,Full6<impl::binder1of7<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,
                                   AutoCurryType,AutoCurryType,AutoCurryType>
   : public FunType<X,Full6<impl::binder1of7<F,X> > > {};

   template <class Y> struct Sig<AutoCurryType,Y>
     : public FunType<Y,Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType>
     : public FunType<Y,Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType>
     : public FunType<Y,Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,
                                 AutoCurryType>
     : public FunType<Y,Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,
                                 AutoCurryType,AutoCurryType>
     : public FunType<Y,Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,
                                 AutoCurryType,AutoCurryType,AutoCurryType>
     : public FunType<Y,Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z>
     : public FunType<Z,Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType>
     : public FunType<Z,Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,
                                 AutoCurryType>
     : public FunType<Z,Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,
                                 AutoCurryType,AutoCurryType>
     : public FunType<Z,Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > > {};

   template <class T> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,T>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,T,
                    Full6<impl::binderXof7<F,std::pair<Bind4,T> > > > {};

   template <class T> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,T,
                                 AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,T,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind4,T> > > > {};

   template <class T> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,T,
                                 AutoCurryType,AutoCurryType>
     : public FunType<T,Full6<impl::binderXof7<F,std::pair<Bind4,T> > > > {};

   template <class T> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,T,
                                 AutoCurryType,
                                 AutoCurryType,AutoCurryType>
     : public FunType<T,Full6<impl::binderXof7<F,std::pair<Bind4,T> > > > {};

   template <class U> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,
                                 AutoCurryType,U>
     : public FunType<U,Full6<impl::binderXof7<F,std::pair<Bind5,U> > > > {};

   template <class U> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,
                                 AutoCurryType,U,AutoCurryType>
     : public FunType<U,Full6<impl::binderXof7<F,std::pair<Bind5,U> > > > {};

   template <class U> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,
                                 AutoCurryType,U,AutoCurryType,AutoCurryType>
     : public FunType<U,Full6<impl::binderXof7<F,std::pair<Bind5,U> > > > {};

   template <class V> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,
                                 AutoCurryType,AutoCurryType,V>
     : public FunType<V,Full6<impl::binderXof7<F,std::pair<Bind6,V> > > > {};

   template <class V> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,
                                 AutoCurryType,AutoCurryType,V,AutoCurryType>
     : public FunType<V,Full6<impl::binderXof7<F,std::pair<Bind6,V> > > > {};

   template <class W> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,
                                 AutoCurryType,AutoCurryType,AutoCurryType,W>
     : public FunType<W,Full6<impl::binderXof7<F,std::pair<Bind7,W> > > > {};

   template <class X, class Y>
   struct Sig<X,Y>
   : public FunType<X,Y,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Y>
   struct Sig<X,Y,AutoCurryType>
   : public FunType<X,Y,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Y>
   struct Sig<X,Y,AutoCurryType,AutoCurryType>
   : public FunType<X,Y,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Y>
   struct Sig<X,Y,AutoCurryType,AutoCurryType,AutoCurryType>
   : public FunType<X,Y,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Y>
   struct Sig<X,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
   : public FunType<X,Y,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Y>
   struct Sig<X,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,
                  AutoCurryType>
   : public FunType<X,Y,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Z>
   struct Sig<X,AutoCurryType,Z>
   : public FunType<X,Z,Full5<impl::binderXandYof7<F,std::pair<Bind1,X>,
                              std::pair<Bind3,Z> > > > {};

   template <class X, class Z>
     struct Sig<X,AutoCurryType,Z,AutoCurryType>
   : public FunType<X,Z,Full5<impl::binderXandYof7<F,std::pair<Bind1,X>,
                              std::pair<Bind3,Z> > > > {};

   template <class X, class Z>
     struct Sig<X,AutoCurryType,AutoCurryType,Z>
   : public FunType<X,Z,Full5<impl::binderXandYof7<F,std::pair<Bind1,X>,
                              std::pair<Bind4,Z> > > > {};

   template <class Y, class Z>
   struct Sig<AutoCurryType,Y,Z>
   : public FunType<Y,Z,Full5<impl::binderXandYof7<F,std::pair<Bind2,Y>,
                              std::pair<Bind3,Z> > > > {};

   template <class Y, class Z>
   struct Sig<AutoCurryType,AutoCurryType,Y,Z>
   : public FunType<Y,Z,Full5<impl::binderXandYof7<F,std::pair<Bind3,Y>,
                              std::pair<Bind4,Z> > > > {};

   template <class V, class W>
   struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,
              AutoCurryType,V,W>
   : public FunType<V,W,Full5<impl::binderXandYof7<F,std::pair<Bind6,V>,
                              std::pair<Bind7,W> > > > {};

   template <class X, class Y, class Z>
   struct Sig<X,Y,Z>
   : public FunType<X,Y,Z,Full4<impl::binder1to3of7<F,X,Y,Z> > > {};

   template <class X, class Y, class Z>
   struct Sig<X,Y,Z,AutoCurryType>
   : public FunType<X,Y,Z,Full4<impl::binder1to3of7<F,X,Y,Z> > > {};
   //   : public FunType<X,Y,Z,Full4<impl::binderXtoZof7<F,std::pair<Bind1,X>,
   //         std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class X, class Y, class Z>
   struct Sig<X,Y,Z,AutoCurryType,AutoCurryType>
   : public FunType<X,Y,Z,Full4<impl::binder1to3of7<F,X,Y,Z> > > {};

   template <class X, class Z, class T>
     struct Sig<X,AutoCurryType,Z,T>
   : public FunType<X,Z,T,Full4<impl::binderXtoZof7<F,std::pair<Bind1,X>,
            std::pair<Bind3,Z>,std::pair<Bind4,T> > > > {};

   template <class X, class Y, class T>
   struct Sig<X,Y,AutoCurryType,T>
   : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind1,X>,
            std::pair<Bind2,Y>,std::pair<Bind4,T> > > > {};

   template <class X, class Y, class T>
   struct Sig<X,Y,AutoCurryType,T,AutoCurryType>
     : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind4,T> > > > {};

   template <class X, class Y, class U>
   struct Sig<X,Y,AutoCurryType,AutoCurryType,U>
     : public FunType<X,Y,U,Full4<impl::binderXtoZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind5,U> > > > {};

   template <class X, class Z, class U>
     struct Sig<X,AutoCurryType,AutoCurryType,Z,U>
     : public FunType<X,Z,U,Full4<impl::binderXtoZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind4,Z>,std::pair<Bind5,U> > > > {};

   template <class X, class Y, class T>
   struct Sig<AutoCurryType,X,Y,T>
   : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind2,X>,
            std::pair<Bind3,Y>,std::pair<Bind4,T> > > > {};

   template <class X, class Y, class T>
   struct Sig<AutoCurryType,AutoCurryType,X,Y,T>
   : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind3,X>,
            std::pair<Bind4,Y>,std::pair<Bind5,T> > > > {};

   template <class X, class Y, class T>
   struct Sig<AutoCurryType,AutoCurryType,X,Y,AutoCurryType,T>
   : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind3,X>,
            std::pair<Bind4,Y>,std::pair<Bind6,T> > > > {};

   template <class X, class Y, class T>
     struct Sig<AutoCurryType,AutoCurryType,X,AutoCurryType,Y,T>
   : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind3,X>,
            std::pair<Bind5,Y>,std::pair<Bind6,T> > > > {};

   template <class X, class Y, class T>
     struct Sig<AutoCurryType,AutoCurryType,X,AutoCurryType,Y,AutoCurryType,T>
   : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind3,X>,
            std::pair<Bind5,Y>,std::pair<Bind7,T> > > > {};

   template <class X, class Y, class T>
     struct Sig<AutoCurryType,AutoCurryType,X,AutoCurryType,AutoCurryType,Y,T>
   : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind3,X>,
            std::pair<Bind6,Y>,std::pair<Bind7,T> > > > {};

   template <class X, class Y, class T>
   struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,X,Y,T>
   : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind4,X>,
            std::pair<Bind5,Y>,std::pair<Bind6,T> > > > {};

   template <class X, class Y, class T>
   struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,X,Y,AutoCurryType,T>
   : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind4,X>,
            std::pair<Bind5,Y>,std::pair<Bind7,T> > > > {};

   template <class X, class Y, class T>
     struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,X,AutoCurryType,Y,T>
   : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind4,X>,
            std::pair<Bind6,Y>,std::pair<Bind7,T> > > > {};

   template <class X, class Y, class T>
   struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,X,Y,T>
   : public FunType<X,Y,T,Full4<impl::binderXtoZof7<F,std::pair<Bind5,X>,
            std::pair<Bind6,Y>,std::pair<Bind7,T> > > > {};

   template <class W,class X, class Y, class Z>
   struct Sig<W,X,Y,Z>
     : public FunType<W,X,Y,Z,Full3<impl::binder1to4of7<F,W,X,Y,Z> > > {};

   template <class W,class X, class Y, class Z>
   struct Sig<W,X,Y,Z,AutoCurryType>
     : public FunType<W,X,Y,Z,Full3<impl::binder1to4of7<F,W,X,Y,Z> > > {};

   template <class W,class X, class Y, class Z>
   struct Sig<W,AutoCurryType,X,Y,Z>
       : public FunType<W,X,Y,Z,Full3<impl::binderWtoZof7<F,std::pair<Bind1,W>,
         std::pair<Bind3,X>,std::pair<Bind4,Y>,std::pair<Bind5,Z> > > > {};

   template <class W,class X, class Y, class Z>
   struct Sig<AutoCurryType,W,X,Y,Z>
       : public FunType<W,X,Y,Z,Full3<impl::binderWtoZof7<F,std::pair<Bind2,W>,
         std::pair<Bind3,X>,std::pair<Bind4,Y>,std::pair<Bind5,Z> > > > {};

   template <class W,class X, class Y, class Z>
     struct Sig<W,X,AutoCurryType,Y,Z>
       : public FunType<W,X,Y,Z,Full3<impl::binderWtoZof7<F,std::pair<Bind1,W>,
         std::pair<Bind2,X>,std::pair<Bind4,Y>,std::pair<Bind5,Z> > > > {};

   template <class W,class X, class Y, class Z>
     struct Sig<W,X,Y,AutoCurryType,Z>
       : public FunType<W,X,Y,Z,Full3<impl::binderWtoZof7<F,std::pair<Bind1,W>,
         std::pair<Bind2,X>,std::pair<Bind3,Y>,std::pair<Bind5,Z> > > > {};

   template <class W,class X, class Y, class Z>
   struct Sig<W,AutoCurryType,AutoCurryType,X,Y,Z>
       : public FunType<W,X,Y,Z,Full3<impl::binderWtoZof7<F,std::pair<Bind1,W>,
         std::pair<Bind4,X>,std::pair<Bind5,Y>,std::pair<Bind6,Z> > > > {};

   template <class V,class W,class X, class Y, class Z>
   struct Sig<V,W,X,Y,Z>
     : public FunType<V,W,X,Y,Z,Full2<impl::binder1to5of7<F,V,W,X,Y,Z> > > {};

   template <class V,class W,class X, class Y, class Z>
   struct Sig<AutoCurryType,V,W,X,Y,Z>
   : public FunType<V,W,X,Y,Z,Full2<impl::binderVtoZof7<F,std::pair<Bind2,V>,
                       std::pair<Bind3,W>,std::pair<Bind4,X>,
                       std::pair<Bind5,Y>,std::pair<Bind6,Z> > > >{};

   template <class V,class W,class X, class Y, class Z>
     struct Sig<V,AutoCurryType,W,X,Y,Z>
   : public FunType<V,W,X,Y,Z,Full2<impl::binderVtoZof7<F,std::pair<Bind1,V>,
                       std::pair<Bind3,W>,std::pair<Bind4,X>,
                       std::pair<Bind5,Y>,std::pair<Bind6,Z> > > >{};

   template <class V,class W,class X, class Y, class Z>
     struct Sig<V,AutoCurryType,AutoCurryType,W,X,Y,Z>
   : public FunType<V,W,X,Y,Z,Full2<impl::binderVtoZof7<F,std::pair<Bind1,V>,
                       std::pair<Bind4,W>,std::pair<Bind5,X>,
                       std::pair<Bind6,Y>,std::pair<Bind7,Z> > > >{};

   template <class U,class V,class W,class X, class Y, class Z>
     struct Sig<U,V,W,X,Y,Z>
   : public FunType<U,V,W,X,Y,Z,Full1<impl::binder1to6of7<F,U,V,W,X,Y,Z> > > {};

   template <class U,class V,class W,class X, class Y, class Z>
   struct Sig<AutoCurryType,U,V,W,X,Y,Z>
   : public FunType<U,V,W,X,Y,Z,Full1<impl::binderUtoZof7<F,std::pair<Bind2,U>,
                       std::pair<Bind3,V>,std::pair<Bind4,W>,std::pair<Bind5,X>,
                       std::pair<Bind6,Y>,std::pair<Bind7,Z> > > >{};

   template <class U,class V,class W,class X, class Y, class Z>
     struct Sig<U,AutoCurryType,V,W,X,Y,Z>
   : public FunType<U,V,W,X,Y,Z,Full1<impl::binderUtoZof7<F,std::pair<Bind1,U>,
                       std::pair<Bind3,V>,std::pair<Bind4,W>,std::pair<Bind5,X>,
                       std::pair<Bind6,Y>,std::pair<Bind7,Z> > > >{};

#endif

   template <class X>
#ifdef FCPP_CONCEPTS
   inline ConstCallable7<F,X>::result_type
#else
   typename Sig<X>::ResultType
#endif
   operator()( const X& x ) const {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "full7 binder1of7" << std::endl;
#endif
      return makeFull6(impl::binder1of7<F,X>(f,x));
   }

   template <class X, class Y>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<F,X,Y>::result_type
#else
   typename Sig<X,Y>::ResultType
#endif
   operator()( const X& x, const Y& y ) const {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "full7 Curryable7Helper6" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable7Helper6<typename ConstCallable7<F,X,Y>::result_type,F,X,Y>
      ::go(f,x,y);
#else
     return impl::Curryable7Helper6<typename Sig<X,Y>::ResultType,F,X,Y>
      ::go(f,x,y);
#endif
   }

   template <class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<F,X,Y,Z>::result_type
#else
     typename Sig<X,Y,Z>::ResultType
#endif
     operator()( const X& x, const Y& y, const Z& z) const {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "full7 Curryable7Helper5" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable7Helper5<typename ConstCallable7<F,X,Y,Z>::result_type,F,X,Y,Z>
       ::go(f,x,y,z);
#else
     return impl::Curryable7Helper5<typename Sig<X,Y,Z>::ResultType,F,X,Y,Z>
      ::go(f,x,y,z);
#endif
   }

   template <class W,class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<F,W,X,Y,Z>::result_type
#else
     typename Sig<W,X,Y,Z>::ResultType
#endif
     operator()( const W& w,const X& x, const Y& y,
       const Z& z) const {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "full7 Curryable7Helper4" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable7Helper4<typename ConstCallable7<F,W,X,Y,Z>::result_type,F,W,X,Y,Z>
       ::go(f,w,x,y,z);
#else
     return impl::Curryable7Helper4<typename Sig<W,X,Y,Z>::ResultType,F,W,X,Y,Z>
       ::go(f,w,x,y,z);
#endif
   }

   template < class V, class W,class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<F,V,W,X,Y,Z>::result_type
#else
     typename Sig<V,W,X,Y,Z>::ResultType 
#endif
     operator()( const V& v,const W& w,const X& x, const Y& y,
       const Z& z) const {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "full7 Curryable7Helper3" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable7Helper3<typename ConstCallable7<F,V,W,X,Y,Z>::result_type,F,V,W,X,Y,Z>
       ::go(f,v,w,x,y,z);
#else
     return impl::Curryable7Helper3<typename Sig<V,W,X,Y,Z>::ResultType,F,V,W,X,Y,Z>
      ::go(f,v,w,x,y,z);
#endif
   }

   template < class U, class V, class W,class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<F,U,V,W,X,Y,Z>::result_type
#else
     typename Sig<U,V,W,X,Y,Z>::ResultType 
#endif
     operator()( const U& u,const V& v,const W& w,const X& x, const Y& y,
       const Z& z) const {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "full7 Curryable7Helper2" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable7Helper2<typename ConstCallable7<F,U,V,W,X,Y,Z>::result_type,F,U,V,W,X,Y,Z>
       ::go(f,u,v,w,x,y,z);
#else
     return impl::Curryable7Helper2<typename Sig<U,V,W,X,Y,Z>::ResultType,F,U,V,W,X,Y,Z>
       ::go(f,u,v,w,x,y,z);
#endif
   }


   template <class X, class Y, class Z, class T, class U, class V, class W>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<F,X,Y,Z,T,U,V,W>::result_type
#else
     inline typename Sig<X,Y,Z,T,U,V,W>::ResultType
#endif
     operator()( const X& x, const Y& y, const Z& z, const T& t, 
                 const U& u, const V& v, const W& w ) const {
      // need partial specialization, so defer to a class helper
#ifdef FCPP_CONCEPTS
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "ConstCallable7 used in Full7" << std::endl;
#endif
     return impl::Curryable7Helper<typename ConstCallable7<F,X,Y,Z,T,U,V,W>::result_type,F,X,Y,Z,T,U,V,W>
                ::go(f,x,y,z,t,u,v,w);
#else
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper call in Full7" << std::endl;
#endif
     return impl::Curryable7Helper<typename Sig<X,Y,Z,T,U,V,W>::ResultType,F,X,Y,Z,T,U,V,W>
                ::go(f,x,y,z,t,u,v,w);
#endif
   }

#ifdef FCPP_DEBUG
   std::string name() const {
      return debug_string_and_name_or_string("Full7::",f,"Full7");
   }
#endif

};


#endif
