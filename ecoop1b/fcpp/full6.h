//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FULL6_DOT_H
#define FCPP_FULL6_DOT_H

//////////////////////////////////////////////////////////////////////
// Implementation of Full6. (Moved from full.h)
//////////////////////////////////////////////////////////////////////

#ifdef FCPP_CONCEPTS
//////////////////////////////////////////////////////////////////////
// Space for the concept maps for Full6
//////////////////////////////////////////////////////////////////////
template <class F,class X>
concept_map ConstCallable6<F,X>
{
  typedef Full5<impl::binder1of6<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable6<F,X,AutoCurryType>
{
  typedef Full5<impl::binder1of6<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binder1of6<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binder1of6<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType>
{
  typedef Full5<impl::binder1of6<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binder1of6<F,X> > result_type;
};

template <class F,class Y>
  concept_map ConstCallable6<F,AutoCurryType,Y>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,
                               AutoCurryType>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,
                               AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Z>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Z>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,AutoCurryType>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Z>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,
                               AutoCurryType>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Z>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,
                               AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class U>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,AutoCurryType,U>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind4,U> > > result_type;
};

template <class F,class U>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,AutoCurryType,U,
                               AutoCurryType>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind4,U> > > result_type;
};

template <class F,class U>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,AutoCurryType,U,
                               AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind4,U> > > result_type;
};

template <class F,class V>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,V>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind5,V> > > result_type;
};

template <class F,class V>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,V,AutoCurryType>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind5,V> > > result_type;
};

template <class F,class W>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,AutoCurryType,W>
{
  typedef Full5<impl::binderXof6<F,std::pair<Bind6,W> > > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable6<F,X,Y>
{
  typedef Full4<impl::binder1and2of6<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable6<F,X,Y,AutoCurryType>
{
  typedef Full4<impl::binder1and2of6<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable6<F,X,Y,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binder1and2of6<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable6<F,X,Y,AutoCurryType,AutoCurryType,
                                    AutoCurryType>
{
  typedef Full4<impl::binder1and2of6<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable6<F,X,Y,AutoCurryType,AutoCurryType,
                                    AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binder1and2of6<F,X,Y> > result_type;
};

template <class F,class X, class Z>
  concept_map ConstCallable6<F,X,AutoCurryType,Z>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class X, class Z>
  concept_map ConstCallable6<F,X,AutoCurryType,Z,AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class X, class Z>
  concept_map ConstCallable6<F,X,AutoCurryType,Z,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class X, class U>
  concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,U>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind4,U> > > result_type;
};

template <class F,class X, class U>
  concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,U,AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind4,U> > > result_type;
};

template <class F,class X, class U>
  concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,U,
                                 AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind4,U> > > result_type;
};

template <class F,class X, class V>
  concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,AutoCurryType,V>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind5,V> > > result_type;
};

template <class F,class X, class V>
  concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,AutoCurryType,V,
                                 AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind5,V> > > result_type;
};

template <class F,class X, class W>
  concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,
                                 AutoCurryType,AutoCurryType,W>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind6,W> > > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable6<F,AutoCurryType,Y,Z>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable6<F,AutoCurryType,Y,Z,AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable6<F,AutoCurryType,Y,Z,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable6<F,AutoCurryType,Y,Z,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Y, class U>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,U>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind4,U> > > result_type;
};

template <class F,class Y, class U>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,U,AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind4,U> > > result_type;
};

template <class F,class Y, class U>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,U,AutoCurryType,
                               AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind4,U> > > result_type;
};

template <class F,class Y, class V>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,V>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind5,V> > > result_type;
};

template <class F,class Y, class V>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,V,
                               AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind5,V> > > result_type;
};

template <class F,class Y, class W>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,
                               AutoCurryType,W>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind6,W> > > result_type;
};

template <class F,class Z, class U>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,U>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U> > > result_type;
};

template <class F,class Z, class U>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,U,AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U> > > result_type;
};

template <class F,class Z, class U>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,U,
                               AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U> > > result_type;
};

template <class F,class Z, class V>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,V>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind5,V> > > result_type;
};

template <class F,class Z, class V>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,V,
                               AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind5,V> > > result_type;
};

template <class F,class Z, class W>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,
                               AutoCurryType,W>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind6,W> > > result_type;
};

template <class F,class U, class V>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,AutoCurryType,U,V>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind4,U>,std::pair<Bind5,V> > > result_type;
};

template <class F,class U, class V>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,AutoCurryType,U,V,
                               AutoCurryType>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind4,U>,std::pair<Bind5,V> > > result_type;
};

template <class F,class U, class W>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,AutoCurryType,U,
                               AutoCurryType,W>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind4,U>,std::pair<Bind6,W> > > result_type;
};

template <class F,class V, class W>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,AutoCurryType,
                               AutoCurryType,V,W>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind5,V>,std::pair<Bind6,W> > > result_type;
};

 template <class F,class X, class Y, class Z>
   concept_map ConstCallable6<F,X,Y,Z>
{
  typedef Full3<impl::binder1to3of6<F,X,Y,Z> > result_type;
};

 template <class F,class X, class Y, class Z>
   concept_map ConstCallable6<F,X,Y,Z,AutoCurryType>
{
  typedef Full3<impl::binder1to3of6<F,X,Y,Z> > result_type;
};

 template <class F,class X, class Y, class Z>
   concept_map ConstCallable6<F,X,Y,Z,AutoCurryType,AutoCurryType>
{
  typedef Full3<impl::binder1to3of6<F,X,Y,Z> > result_type;
};

 template <class F,class X, class Y, class Z>
   concept_map ConstCallable6<F,X,Y,Z,AutoCurryType,AutoCurryType,
                                      AutoCurryType>
{
  typedef Full3<impl::binder1to3of6<F,X,Y,Z> > result_type;
};

 template <class F,class X, class Y, class U>
  concept_map ConstCallable6<F,X,Y,AutoCurryType,U>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,
                                           std::pair<Bind4,U> > > result_type;
};

 template <class F,class X, class Y, class U>
  concept_map ConstCallable6<F,X,Y,AutoCurryType,U,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,
                                           std::pair<Bind4,U> > > result_type;
};

 template <class F,class X, class Y, class U>
  concept_map ConstCallable6<F,X,Y,AutoCurryType,U,AutoCurryType,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,
                                           std::pair<Bind4,U> > > result_type;
};

 template <class F,class X, class Y, class V>
   concept_map ConstCallable6<F,X,Y,AutoCurryType,AutoCurryType,V>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,
                                           std::pair<Bind5,V> > > result_type;
};

 template <class F,class X, class Y, class V>
   concept_map ConstCallable6<F,X,Y,AutoCurryType,AutoCurryType,V,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,
                                           std::pair<Bind5,V> > > result_type;
};

 template <class F,class X, class Z, class U>
  concept_map ConstCallable6<F,X,AutoCurryType,Z,U>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,
                                           std::pair<Bind4,U> > > result_type;
};

 template <class F,class X, class Z, class U>
  concept_map ConstCallable6<F,X,AutoCurryType,Z,U,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,
                                           std::pair<Bind4,U> > > result_type;
};

 template <class F,class X, class Z, class U>
  concept_map ConstCallable6<F,X,AutoCurryType,Z,U,AutoCurryType,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,
                                           std::pair<Bind4,U> > > result_type;
};

 template <class F,class X, class Z, class V>
   concept_map ConstCallable6<F,X,AutoCurryType,Z,AutoCurryType,V>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,
                                           std::pair<Bind5,V> > > result_type;
};

 template <class F,class X, class Z, class V>
   concept_map ConstCallable6<F,X,AutoCurryType,Z,AutoCurryType,V,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,
                                           std::pair<Bind5,V> > > result_type;
};

 template <class F,class X, class Z, class W>
   concept_map ConstCallable6<F,X,AutoCurryType,Z,AutoCurryType,AutoCurryType,W>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,
                                           std::pair<Bind6,W> > > result_type;
};

 template <class F,class X, class U, class V>
   concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,U,V>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind4,U>,
                                           std::pair<Bind5,V> > > result_type;
};
 template <class F,class X, class U, class V>
   concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,U,V,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,std::pair<Bind4,U>,
                                           std::pair<Bind5,V> > > result_type;
};

 template <class F,class Y, class Z, class U>
   concept_map ConstCallable6<F,AutoCurryType,Y,Z,U>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,
                                           std::pair<Bind4,U> > > result_type;
};

 template <class F,class Y, class Z, class U>
   concept_map ConstCallable6<F,AutoCurryType,Y,Z,U,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,
                                           std::pair<Bind4,U> > > result_type;
};

 template <class F,class Y, class Z, class U>
   concept_map ConstCallable6<F,AutoCurryType,Y,Z,U,AutoCurryType,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,
                                           std::pair<Bind4,U> > > result_type;
};

 template <class F,class Y, class Z, class V>
   concept_map ConstCallable6<F,AutoCurryType,Y,Z,AutoCurryType,V>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,
                                           std::pair<Bind5,V> > > result_type;
};

 template <class F,class Y, class Z, class V>
   concept_map ConstCallable6<F,AutoCurryType,Y,Z,AutoCurryType,V,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,
                                           std::pair<Bind5,V> > > result_type;
};

 template <class F,class Y, class U, class V>
   concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,U,V>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,std::pair<Bind4,U>,
                                           std::pair<Bind5,V> > > result_type;
};

 template <class F,class Y, class U, class V>
   concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,U,V,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,std::pair<Bind4,U>,
                                           std::pair<Bind5,V> > > result_type;
};

 template <class F,class Y, class V, class W>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,V,W>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,std::pair<Bind5,V>,
                                           std::pair<Bind6,W> > > result_type;
};

 template <class F,class Z, class U, class V>
   concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,U,V>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U>,
                                           std::pair<Bind5,V> > > result_type;
};

 template <class F,class Z, class U, class V>
   concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,U,V,AutoCurryType>
{
  typedef Full3<impl::binderXtoZof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U>,
                                           std::pair<Bind5,V> > > result_type;
};

 template <class F, class W, class X, class Y, class Z>
   concept_map ConstCallable6<F,W,X,Y,Z>
{
  typedef Full2<impl::binder1to4of6<F,W,X,Y,Z> > result_type;
};

 template <class F, class W, class X, class Y, class Z>
   concept_map ConstCallable6<F,W,X,Y,Z,AutoCurryType>
{
  typedef Full2<impl::binder1to4of6<F,W,X,Y,Z> > result_type;
};

 template <class F, class W, class X, class Y, class Z>
   concept_map ConstCallable6<F,W,X,Y,Z,AutoCurryType,AutoCurryType>
{
  typedef Full2<impl::binder1to4of6<F,W,X,Y,Z> > result_type;
};

 template <class F, class X, class Y, class Z, class V>
  concept_map ConstCallable6<F,X,Y,Z,AutoCurryType,V>
{
  typedef Full2<impl::binderWtoZof6<F,std::pair<Bind1,X>,
       std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,V> > > result_type;
};

 template <class F, class X, class Y, class Z, class V>
  concept_map ConstCallable6<F,X,Y,Z,AutoCurryType,V,AutoCurryType>
{
  typedef Full2<impl::binderWtoZof6<F,std::pair<Bind1,X>,
       std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,V> > > result_type;
};

 template <class F, class X, class Y, class U, class V>
   concept_map ConstCallable6<F,X,Y,AutoCurryType,U,V>
{
  typedef Full2<impl::binderWtoZof6<F,std::pair<Bind1,X>,
       std::pair<Bind2,Y>,std::pair<Bind4,U>,std::pair<Bind5,V> > > result_type;
};

 template <class F, class X, class Y, class U, class V>
   concept_map ConstCallable6<F,X,Y,AutoCurryType,U,V,AutoCurryType>
{
  typedef Full2<impl::binderWtoZof6<F,std::pair<Bind1,X>,
       std::pair<Bind2,Y>,std::pair<Bind4,U>,std::pair<Bind5,V> > > result_type;
};

 template <class F, class X, class Z, class U, class V>
   concept_map ConstCallable6<F,X,AutoCurryType,Z,U,V>
{
  typedef Full2<impl::binderWtoZof6<F,std::pair<Bind1,X>,
       std::pair<Bind3,Z>,std::pair<Bind4,U>,std::pair<Bind5,V> > > result_type;
};

 template <class F, class Y, class Z, class U, class V>
   concept_map ConstCallable6<F,AutoCurryType,Y,Z,U,V>
{
  typedef Full2<impl::binderWtoZof6<F,std::pair<Bind2,Y>,
       std::pair<Bind3,Z>,std::pair<Bind4,U>,std::pair<Bind5,V> > > result_type;
};

 template <class F, class V, class W, class X, class Y, class Z>
   concept_map ConstCallable6<F,V,W,X,Y,Z>
{
  typedef Full1<impl::binder1to5of6<F,V,W,X,Y,Z> > result_type;
};

 template <class F, class V, class W, class X, class Y, class Z>
   concept_map ConstCallable6<F,V,W,X,Y,Z,AutoCurryType>
{
  typedef Full1<impl::binder1to5of6<F,V,W,X,Y,Z> > result_type;
};

#endif


//////////////////////////////////////////////////////////////////////
// Full6
//////////////////////////////////////////////////////////////////////

template <class F>
class Full6 : public SmartFunctoid6 {
   F f;
public:
   Full6() : f() {}
   Full6( const F& ff ) : f(ff) {}
#ifdef FCPP_ENABLE_LAMBDA
   typedef Full6 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
       {  
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Full6 operator[] called" << std::endl;
#endif
      return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif

#ifndef FCPP_CONCEPTS
   template <class X, class Y=void, class Z=void, class U=void,class V=void, class W=void>
   struct Sig
   : public FunType<
                    typename F::template Sig<X,Y,Z,U,V,W>::Arg1Type,
                    typename F::template Sig<X,Y,Z,U,V,W>::Arg2Type,
                    typename F::template Sig<X,Y,Z,U,V,W>::Arg3Type,
                    typename F::template Sig<X,Y,Z,U,V,W>::Arg4Type,
                    typename F::template Sig<X,Y,Z,U,V,W>::Arg5Type,
                    typename F::template Sig<X,Y,Z,U,V,W>::Arg6Type,
                    typename RT<F,X,Y,Z,U,V,W>::ResultType> {};

   template <class X> struct Sig<X> 
   : public FunType<X,Full5<impl::binder1of6<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType> 
   : public FunType<X,Full5<impl::binder1of6<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType> 
   : public FunType<X,Full5<impl::binder1of6<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType> 
   : public FunType<X,Full5<impl::binder1of6<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,
                                   AutoCurryType> 
   : public FunType<X,Full5<impl::binder1of6<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,
                                   AutoCurryType,AutoCurryType> 
   : public FunType<X,Full5<impl::binder1of6<F,X> > > {};

   template <class Y> struct Sig<AutoCurryType,Y> 
   : public FunType<Y,Full5<impl::binderXof6<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType> 
   : public FunType<Y,Full5<impl::binderXof6<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType> 
   : public FunType<Y,Full5<impl::binderXof6<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,
                                 AutoCurryType> 
   : public FunType<Y,Full5<impl::binderXof6<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,
                                 AutoCurryType,AutoCurryType> 
   : public FunType<Y,Full5<impl::binderXof6<F,std::pair<Bind2,Y> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z> 
   : public FunType<Z,Full5<impl::binderXof6<F,std::pair<Bind3,Z> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType> 
   : public FunType<Z,Full5<impl::binderXof6<F,std::pair<Bind3,Z> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,
                                 AutoCurryType> 
   : public FunType<Z,Full5<impl::binderXof6<F,std::pair<Bind3,Z> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,
                                 AutoCurryType,AutoCurryType> 
   : public FunType<Z,Full5<impl::binderXof6<F,std::pair<Bind3,Z> > > > {};

   template <class U> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,U> 
   : public FunType<U,Full5<impl::binderXof6<F,std::pair<Bind4,U> > > > {};

   template <class U> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,U,
                                 AutoCurryType> 
   : public FunType<U,Full5<impl::binderXof6<F,std::pair<Bind4,U> > > > {};

   template <class U> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,U,
                                 AutoCurryType,AutoCurryType> 
   : public FunType<U,Full5<impl::binderXof6<F,std::pair<Bind4,U> > > > {};

   template <class V> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,
                                 AutoCurryType,V> 
   : public FunType<V,Full5<impl::binderXof6<F,std::pair<Bind5,V> > > > {};

   template <class V> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,
                                 AutoCurryType,V,AutoCurryType> 
   : public FunType<V,Full5<impl::binderXof6<F,std::pair<Bind5,V> > > > {};

   template <class W> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,
                                 AutoCurryType,AutoCurryType,W> 
   : public FunType<W,Full5<impl::binderXof6<F,std::pair<Bind6,W> > > > {};

   template <class X, class Y> struct Sig<X,Y> 
   : public FunType<X,Y,Full4<impl::binder1and2of6<F,X,Y> > > {};

   template <class X, class Y> struct Sig<X,Y,AutoCurryType> 
   : public FunType<X,Y,Full4<impl::binder1and2of6<F,X,Y> > > {};
//   : public FunType<X,Y,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind2,Y> > > > {};

   template <class X, class Y> struct Sig<X,Y,AutoCurryType,AutoCurryType> 
   : public FunType<X,Y,Full4<impl::binder1and2of6<F,X,Y> > > {};
   //   : public FunType<X,Y,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind2,Y> > > > {};

   template <class X, class Y> struct Sig<X,Y,AutoCurryType,AutoCurryType,
                                              AutoCurryType> 
   : public FunType<X,Y,Full4<impl::binder1and2of6<F,X,Y> > > {};
   //   : public FunType<X,Y,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind2,Y> > > > {};

   template <class X, class Z> struct Sig<X,AutoCurryType,Z> 
   : public FunType<X,Z,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > > {};

   template <class X, class Z> struct Sig<X,AutoCurryType,Z,AutoCurryType> 
   : public FunType<X,Z,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > > {};

   template <class X, class Z> struct Sig<X,AutoCurryType,Z,AutoCurryType,
                                            AutoCurryType> 
   : public FunType<X,Z,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > > {};

   template <class X, class U> struct Sig<X,AutoCurryType,AutoCurryType,U> 
   : public FunType<X,U,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind4,U> > > > {};

   template <class X, class U> struct Sig<X,AutoCurryType,AutoCurryType,U,
                                            AutoCurryType> 
   : public FunType<X,U,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind4,U> > > > {};

   template <class X, class V> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,V> 
   : public FunType<X,V,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind5,V> > > > {};

   template <class Y, class Z> struct Sig<AutoCurryType,Y,Z> 
   : public FunType<Y,Z,Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class Y, class Z> struct Sig<AutoCurryType,Y,Z,AutoCurryType> 
   : public FunType<Y,Z,Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class Y, class Z> struct Sig<AutoCurryType,Y,Z,AutoCurryType,
                                          AutoCurryType> 
   : public FunType<Y,Z,Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class Y, class U> struct Sig<AutoCurryType,Y,AutoCurryType,U> 
   : public FunType<Y,U,Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind4,U> > > > {};
   template <class Y, class U> struct Sig<AutoCurryType,Y,AutoCurryType,U,AutoCurryType> 
   : public FunType<Y,U,Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind4,U> > > > {};

   template <class Y, class V> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,V> 
   : public FunType<Y,V,Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind5,V> > > > {};

   template <class Z, class U> struct Sig<AutoCurryType,AutoCurryType,Z,U> 
   : public FunType<Z,U,Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U> > > > {};

   template <class Z, class U> struct Sig<AutoCurryType,AutoCurryType,Z,U,
                                          AutoCurryType> 
   : public FunType<Z,U,Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U> > > > {};

   template <class Z, class U> struct Sig<AutoCurryType,AutoCurryType,Z,U,
                                          AutoCurryType,AutoCurryType> 
   : public FunType<Z,U,Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U> > > > {};

   template <class Z, class V> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,V> 
   : public FunType<Z,V,Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind5,V> > > > {};

   template <class U, class V> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,U,V> 
   : public FunType<U,V,Full4<impl::binderXandYof6<F,std::pair<Bind4,U>,std::pair<Bind5,V> > > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z> 
   : public FunType<X,Y,Z,Full3<impl::binder1to3of6<F,X,Y,Z> > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z,AutoCurryType> 
   : public FunType<X,Y,Z,Full3<impl::binder1to3of6<F,X,Y,Z> > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z,AutoCurryType,AutoCurryType> 
   : public FunType<X,Y,Z,Full3<impl::binder1to3of6<F,X,Y,Z> > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z,AutoCurryType,
                                                 AutoCurryType,AutoCurryType> 
   : public FunType<X,Y,Z,Full3<impl::binder1to3of6<F,X,Y,Z> > > {};

   template <class X, class Y, class U> struct Sig<X,Y,AutoCurryType,U> 
   : public FunType<X,Y,U,Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind4,U> > > > {};

   template <class X, class Y, class U> struct Sig<X,Y,AutoCurryType,U,AutoCurryType> 
   : public FunType<X,Y,U,Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind4,U> > > > {};

   template <class X, class Y, class V> struct Sig<X,Y,AutoCurryType,AutoCurryType,V> 
   : public FunType<X,Y,V,Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind5,V> > > > {};

   template <class X, class Z, class U> struct Sig<X,AutoCurryType,Z,U> 
   : public FunType<X,Z,U,Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind3,Z>,std::pair<Bind4,U> > > > {};

   template <class X, class Z, class U> struct Sig<X,AutoCurryType,Z,U,AutoCurryType> 
   : public FunType<X,Z,U,Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind3,Z>,std::pair<Bind4,U> > > > {};

   template <class X, class Z, class V> struct Sig<X,AutoCurryType,Z,AutoCurryType,V> 
   : public FunType<X,Z,V,Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind3,Z>,std::pair<Bind5,V> > > > {};

   template <class X, class U, class V> struct Sig<X,AutoCurryType,AutoCurryType,U,V> 
   : public FunType<X,U,V,Full3<impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind4,U>,std::pair<Bind5,V> > > > {};

   template <class Y, class Z, class U> struct Sig<AutoCurryType,Y,Z,U> 
   : public FunType<Y,Z,U,Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,
		      std::pair<Bind3,Z>,std::pair<Bind4,U> > > > {};

   template <class Y, class Z, class U> struct Sig<AutoCurryType,Y,Z,U,AutoCurryType> 
   : public FunType<Y,Z,U,Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,
		      std::pair<Bind3,Z>,std::pair<Bind4,U> > > > {};

   template <class Y, class Z, class V> struct Sig<AutoCurryType,Y,Z,AutoCurryType,V> 
   : public FunType<Y,Z,V,Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,
		      std::pair<Bind3,Z>,std::pair<Bind5,V> > > > {};

   template <class Y, class Z, class V> struct Sig<AutoCurryType,Y,Z,AutoCurryType,V,AutoCurryType> 
   : public FunType<Y,Z,V,Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,
		      std::pair<Bind3,Z>,std::pair<Bind5,V> > > > {};

   template <class Y, class U, class V> struct Sig<AutoCurryType,Y,AutoCurryType,U,V> 
     : public FunType<Y,U,V,Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,
     std::pair<Bind4,U>,std::pair<Bind5,V> > > > {};

   template <class Y, class U, class V> struct Sig<AutoCurryType,Y,AutoCurryType,U,V,AutoCurryType> 
     : public FunType<Y,U,V,Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,
     std::pair<Bind4,U>,std::pair<Bind5,V> > > > {};

   template <class Y, class V, class W> struct Sig<AutoCurryType,Y,AutoCurryType,
                                                   AutoCurryType,V,W> 
   : public FunType<Y,V,W,Full3<impl::binderXtoZof6<F,std::pair<Bind2,Y>,
		      std::pair<Bind5,V>,std::pair<Bind6,W> > > > {};

   template <class Z, class U, class V> struct Sig<AutoCurryType,AutoCurryType,Z,U,V> 
     : public FunType<Z,U,V,Full3<impl::binderXtoZof6<F,std::pair<Bind3,Z>,
     std::pair<Bind4,U>,std::pair<Bind5,V> > > > {};

   template <class Z, class U, class V> struct Sig<AutoCurryType,AutoCurryType,Z,U,V,AutoCurryType> 
     : public FunType<Z,U,V,Full3<impl::binderXtoZof6<F,std::pair<Bind3,Z>,
     std::pair<Bind4,U>,std::pair<Bind5,V> > > > {};

   template <class X, class Y, class Z, class U> struct Sig<X,Y,Z,U>
   : public FunType<X,Y,Z,U,Full2<impl::binder1to4of6<F,X,Y,Z,U> > > {};

   template <class X, class Y, class Z, class U> struct Sig<X,Y,Z,U,AutoCurryType>
   : public FunType<X,Y,Z,U,Full2<impl::binder1to4of6<F,X,Y,Z,U> > > {};

   template <class X, class Y, class Z, class V> struct Sig<X,Y,Z,AutoCurryType,V>
     : public FunType<X,Y,Z,V,Full2<impl::binderWtoZof6<F,std::pair<Bind1,X>,
           std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,V> > > > {};

   template <class X, class Y, class Z, class V> struct Sig<X,Y,Z,AutoCurryType,V,AutoCurryType> 
     : public FunType<X,Y,Z,V,Full2<impl::binderWtoZof6<F,std::pair<Bind1,X>,
           std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,V> > > > {};

   template <class X, class Y, class U, class V> struct Sig<X,Y,AutoCurryType,U,V>
     : public FunType<X,Y,U,V,Full2<impl::binderWtoZof6<F,std::pair<Bind1,X>,
           std::pair<Bind2,Y>,std::pair<Bind4,U>,std::pair<Bind5,V> > > > {};

   template <class X, class Y, class U, class V> struct Sig<X,Y,AutoCurryType,U,V,AutoCurryType>
     : public FunType<X,Y,U,V,Full2<impl::binderWtoZof6<F,std::pair<Bind1,X>,
           std::pair<Bind2,Y>,std::pair<Bind4,U>,std::pair<Bind5,V> > > > {};

   template <class X, class Z, class U, class V> struct Sig<X,AutoCurryType,Z,U,V>
     : public FunType<X,Z,U,V,Full2<impl::binderWtoZof6<F,std::pair<Bind1,X>,
           std::pair<Bind3,Z>,std::pair<Bind4,U>,std::pair<Bind5,V> > > > {};

   template <class Y, class Z, class U, class V> struct Sig<AutoCurryType,Y,Z,U,V> 
     : public FunType<Y,Z,U,V,Full2<impl::binderWtoZof6<F,std::pair<Bind2,Y>,
           std::pair<Bind3,Z>,std::pair<Bind4,U>,std::pair<Bind5,V> > > > {};


   template <class X, class Y, class Z, class U, class V> struct Sig<X,Y,Z,U,V,AutoCurryType>
   : public FunType<X,Y,Z,U,V,Full1<impl::binder1to5of6<F,X,Y,Z,U,V> > > {};

   template <class X, class Y, class Z, class U, class V> struct Sig<X,Y,Z,U,V>
   : public FunType<X,Y,Z,U,V,Full1<impl::binder1to5of6<F,X,Y,Z,U,V> > > {};

   template <class X,class Y, class Z, class U, class V>
     struct Sig<AutoCurryType,X,Y,Z,U,V>
     : public FunType<Y,Z,U,V,Full1<impl::binderVtoZof6
          <F,std::pair<Bind2,X>,std::pair<Bind3,Y>,
           std::pair<Bind4,Z>,std::pair<Bind5,U>,std::pair<Bind6,V> > > > {};

#endif

   template <class X>
#ifdef FCPP_CONCEPTS
   inline ConstCallable6<F,X>::result_type
#else
   typename Sig<X>::ResultType
#endif
   operator()( const X& x ) const {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "full6 binder1of6" << std::endl;
#endif
      return makeFull5(impl::binder1of6<F,X>(f,x));
   }

   template <class X, class Y>
#ifdef FCPP_CONCEPTS
     inline ConstCallable6<F,X,Y>::result_type
#else
   typename Sig<X,Y>::ResultType
#endif
   operator()( const X& x, const Y& y ) const {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "full6 Curryable6Helper5" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable6Helper5<typename ConstCallable6<F,X,Y>::result_type,F,X,Y>
      ::go(f,x,y);
#else
     return impl::Curryable6Helper5<typename Sig<X,Y>::ResultType,F,X,Y>
      ::go(f,x,y);
#endif
   }

   template <class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
     inline ConstCallable6<F,X,Y,Z>::result_type
#else
     typename Sig<X,Y,Z>::ResultType 
#endif
     operator()( const X& x, const Y& y,
       const Z& z) const {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "full6 Curryable6Helper4" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable6Helper4<typename ConstCallable6<F,X,Y,Z>::result_type,F,X,Y,Z>    
       ::go(f,x,y,z);
#else
     return impl::Curryable6Helper4<typename Sig<X,Y,Z>::ResultType,F,X,Y,Z>    
      ::go(f,x,y,z);
#endif
   }

   template <class X, class Y, class Z, class U>
#ifdef FCPP_CONCEPTS
     inline ConstCallable6<F,X,Y,Z,U>::result_type
#else
     typename Sig<X,Y,Z,U>::ResultType
#endif
     operator()( const X& x, const Y& y,
       const Z& z, const U& u) const {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "full6 binder1to4of6" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable6Helper3<typename ConstCallable6<F,X,Y,Z,U>::result_type,F,X,Y,Z,U>
       ::go(f,x,y,z,u);
#else
     return impl::Curryable6Helper3<typename Sig<X,Y,Z,U>::ResultType,F,X,Y,Z,U>
       ::go(f,x,y,z,u);
#endif
   }

   template <class X, class Y, class Z, class U, class V>
#ifdef FCPP_CONCEPTS
     inline ConstCallable6<F,X,Y,Z,U,V>::result_type
#else
     typename Sig<X,Y,Z,U,V>::ResultType 
#endif
    operator()( const X& x, const Y& y,
       const Z& z, const U& u, const V& v) const {
#ifdef FCPP_PRINT_HELPERS6
     //std::cout << "full6 binder1and2and3and4and5of6" << std::endl;
     std::cout << "Curryable6Helper2 call in Full6" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable6Helper2<typename ConstCallable6<F,X,Y,Z,U,V>::result_type,F,X,Y,Z,U,V>
       ::go(f,x,y,z,u,v);
#else
    return impl::Curryable6Helper2<typename Sig<X,Y,Z,U,V>::ResultType,F,X,Y,Z,U,V>
       ::go(f,x,y,z,u,v);
#endif
   }

   template <class X, class Y, class Z, class U, class V, class W>
#ifdef FCPP_CONCEPTS
     inline ConstCallable6<F,X,Y,Z,U,V,W>::result_type
#else
   inline typename Sig<X,Y,Z,U,V,W>::ResultType
#endif
   operator()( const X& x, const Y& y, const Z& z, const U& u, const V& v, const W& w ) const {
      // need partial specialization, so defer to a class helper
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Full6 operator() called with  6 args" << std::endl;
     std::cout << "Curryable6Helper call in Full6" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable6Helper<typename ConstCallable6<F,X,Y,Z,U,V,W>::result_type,F,X,Y,Z,U,V,W>::go(f,x,y,z,u,v,w);
#else
     return impl::Curryable6Helper<typename Sig<X,Y,Z,U,V,W>::ResultType,F,X,Y,Z,U,V,W>
      ::go(f,x,y,z,u,v,w);
#endif
   }
#ifdef FCPP_DEBUG
   std::string name() const {
      return debug_string_and_name_or_string("Full6::",f,"Full6");
   }
#endif

};


#endif
