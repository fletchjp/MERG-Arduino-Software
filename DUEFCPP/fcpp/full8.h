//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FULL8_DOT_H
#define FCPP_FULL8_DOT_H

//////////////////////////////////////////////////////////////////////
// Implementation of Full8. (Moved from full.h)
//////////////////////////////////////////////////////////////////////

#ifdef FCPP_CONCEPTS
//////////////////////////////////////////////////////////////////////
// Space for the concept maps for Full8
//////////////////////////////////////////////////////////////////////
template <class F,class X>
concept_map ConstCallable8<F,X>
{
  typedef Full7<impl::binder1of8<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable8<F,X,AutoCurryType>
{
  typedef Full7<impl::binder1of8<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable8<F,X,AutoCurryType,AutoCurryType>
{
  typedef Full7<impl::binder1of8<F,X> > result_type;
};

template <class F,class Y>
  concept_map ConstCallable8<F,AutoCurryType,Y>
{
  typedef Full7<impl::binderXof8<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable8<F,AutoCurryType,Y,AutoCurryType>
{
  typedef Full7<impl::binderXof8<F,std::pair<Bind2,Y> > > result_type;
};


template <class F,class Z>
  concept_map ConstCallable8<F,AutoCurryType,AutoCurryType,Z>
{
  typedef Full7<impl::binderXof8<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable8<F,X,Y>
{
  typedef Full6<impl::binder1and2of8<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable8<F,X,Y,AutoCurryType>
{
  typedef Full6<impl::binder1and2of8<F,X,Y> > result_type;
};

template <class F,class X, class Z>
  concept_map ConstCallable8<F,X,AutoCurryType,Z>
{
  typedef Full6<impl::binderXandYof8<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable8<F,AutoCurryType,Y,Z>
{
  typedef Full6<impl::binderXandYof8<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class X, class Y, class Z>
   concept_map ConstCallable8<F,X,Y,Z>
{
  typedef Full5<impl::binderXtoZof8<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};


 template <class F, class W, class X, class Y, class Z>
   concept_map ConstCallable8<F,W,X,Y,Z>
{
  typedef Full4<impl::binderWtoZof8<F,std::pair<Bind1,W>,std::pair<Bind2,X>,
                std::pair<Bind3,Y>,std::pair<Bind4,Z> > > result_type;
};

#endif

//////////////////////////////////////////////////////////////////////
// Full8
//////////////////////////////////////////////////////////////////////

template <class F>
class Full8 : public SmartFunctoid8 {
   F f;
public:
   Full8() : f() {}
   Full8( const F& ff ) : f(ff) {}
#ifdef FCPP_ENABLE_LAMBDA
   typedef Full8 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif

#ifndef FCPP_CONCEPTS
   template <class X, class Y=void, class Z=void, class S=void, class T=void, class U=void,class V=void, class W=void>
   struct Sig
   : public FunType<
     typename F::template Sig<X,Y,Z,S,T,U,V,W>::Arg1Type,
     typename F::template Sig<X,Y,Z,S,T,U,V,W>::Arg2Type,
     typename F::template Sig<X,Y,Z,S,T,U,V,W>::Arg3Type,
     typename F::template Sig<X,Y,Z,S,T,U,V,W>::Arg4Type,
     typename F::template Sig<X,Y,Z,S,T,U,V,W>::Arg5Type,
     typename F::template Sig<X,Y,Z,S,T,U,V,W>::Arg6Type,
     typename F::template Sig<X,Y,Z,S,T,U,V,W>::Arg7Type,
     typename F::template Sig<X,Y,Z,S,T,U,V,W>::Arg8Type,
     typename RT<F,X,Y,Z,S,T,U,V,W>::ResultType> {};

    template <class X> struct Sig<X>
   : public FunType<X,Full7<impl::binder1of8<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType>
   : public FunType<X,Full7<impl::binder1of8<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType>
   : public FunType<X,Full7<impl::binder1of8<F,X> > > {};

   template <class Y> struct Sig<AutoCurryType,Y>
     : public FunType<Y,Full7<impl::binderXof8<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType>
     : public FunType<Y,Full7<impl::binderXof8<F,std::pair<Bind2,Y> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z>
     : public FunType<Z,Full7<impl::binderXof8<F,std::pair<Bind3,Z> > > > {};

   template <class X, class Y>
   struct Sig<X,Y>
   : public FunType<X,Y,Full6<impl::binder1and2of8<F,X,Y> > > {};

   template <class X, class Y>
   struct Sig<X,Y,AutoCurryType>
   : public FunType<X,Y,Full6<impl::binder1and2of8<F,X,Y> > > {};

   template <class X, class Z>
   struct Sig<X,AutoCurryType,Z>
   : public FunType<X,Z,Full6<impl::binderXandYof8<F,std::pair<Bind1,X>,
                              std::pair<Bind3,Z> > > > {};
   template <class Y, class Z>
   struct Sig<AutoCurryType,Y,Z>
   : public FunType<Y,Z,Full6<impl::binderXandYof8<F,std::pair<Bind2,Y>,
                              std::pair<Bind3,Z> > > > {};

   template <class X, class Y, class Z>
   struct Sig<X,Y,Z>
   : public FunType<X,Y,Z,Full5<impl::binderXtoZof8<F,std::pair<Bind1,X>,
            std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class W,class X, class Y, class Z>
   struct Sig<W,X,Y,Z>
     : public FunType<W,X,Y,Z,Full4<impl::binderWtoZof8<F,std::pair<Bind1,W>,
                     std::pair<Bind2,X>,std::pair<Bind3,Y>,std::pair<Bind4,Z> > > > {};

#endif

   template <class X>
#ifdef FCPP_CONCEPTS
   inline ConstCallable8<F,X>::result_type
#else
   typename Sig<X>::ResultType
#endif
   operator()( const X& x ) const {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "full8 binder1of8" << std::endl;
#endif
      return makeFull7(impl::binder1of8<F,X>(f,x));
   }


   template <class X, class Y>
#ifdef FCPP_CONCEPTS
     inline ConstCallable8<F,X,Y>::result_type
#else
   typename Sig<X,Y>::ResultType
#endif
   operator()( const X& x, const Y& y ) const {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "full8 Curryable8Helper7" << std::endl;
#endif

#ifdef FCPP_CONCEPTS
     return impl::Curryable8Helper7<typename ConstCallable8<F,X,Y>::result_type,F,X,Y>
      ::go(f,x,y);
#else
     return impl::Curryable8Helper7<typename Sig<X,Y>::ResultType,F,X,Y>
      ::go(f,x,y);
#endif
      // Temporarily use binder direct, without helper for other cases.
      //return makeFull6(impl::binder1and2of8<F,X,Y>(f,x,y));
   }

   template <class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
     inline ConstCallable8<F,X,Y,Z>::result_type
#else
     typename Sig<X,Y,Z>::ResultType
#endif
     operator()( const X& x, const Y& y, const Z& z) const {
#ifdef FCPP_PRINT_HELPERS8
     std::cout << "full8 Curryable8Helper6" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable8Helper6<typename ConstCallable8<F,X,Y,Z>::result_type,F,X,Y,Z>
       ::go(f,x,y,z);
#else
     return impl::Curryable8Helper6<typename Sig<X,Y,Z>::ResultType,F,X,Y,Z>
      ::go(f,x,y,z);
#endif
   }

   template <class W,class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
     inline ConstCallable8<F,W,X,Y,Z>::result_type
#else
     typename Sig<W,X,Y,Z>::ResultType
#endif
     operator()( const W& w,const X& x, const Y& y,
       const Z& z) const {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "full8 Curryable8Helper5" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
     return impl::Curryable8Helper5<typename ConstCallable8<F,W,X,Y,Z>::result_type,F,W,X,Y,Z>
       ::go(f,w,x,y,z);
#else
     return impl::Curryable8Helper5<typename Sig<W,X,Y,Z>::ResultType,F,W,X,Y,Z>
       ::go(f,w,x,y,z);
#endif
   }
   template <class X, class Y, class Z, class S, class T, class U, class V, class W>
#ifdef FCPP_CONCEPTS
     inline ConstCallable8<F,X,Y,Z,S,T,U,V,W>::result_type
#else
     inline typename Sig<X,Y,Z,S,T,U,V,W>::ResultType
#endif
     operator()( const X& x, const Y& y, const Z& z, const S&s, const T& t,
                 const U& u, const V& v, const W& w ) const {
      // need partial specialization, so defer to a class helper
#ifdef FCPP_CONCEPTS
#ifdef FCPP_PRINT_HELPERS8
     std::cout << "ConstCallable8 used in Full8" << std::endl;
#endif
     return impl::Curryable8Helper<typename ConstCallable8<F,X,Y,Z,S,T,U,V,W>::result_type,F,X,Y,Z,S,T,U,V,W>
       ::go(f,x,y,z,s,t,u,v,w);
#else
#ifdef FCPP_PRINT_HELPERS8
     std::cout << "Curryable8Helper call in Full8" << std::endl;
#endif
     return impl::Curryable8Helper<typename Sig<X,Y,Z,S,T,U,V,W>::ResultType,F,X,Y,Z,S,T,U,V,W>
       ::go(f,x,y,z,s,t,u,v,w);
#endif
   }


#ifdef FCPP_DEBUG
   std::string name() const {
      return debug_string_and_name_or_string("Full8::",f,"Full8");
   }
#endif


};

#endif
