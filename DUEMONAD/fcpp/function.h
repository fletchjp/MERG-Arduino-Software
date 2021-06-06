//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FUNCTION_DOT_H
#define FCPP_FUNCTION_DOT_H

//////////////////////////////////////////////////////////////////////////
// Adding default constructors to FunN so that 
// they can be put into a std::map.
// For each FunN there needs to be a default function,
// which I have called nullfunN, It takes the correct number 
// of arguments and returns a value using the default constructor
// for that type ( Result() ). This will work unless the Result type
// does not have a default constructor.
// For Fun3 and above, there also has to be a default constructor
// for FunNGuts, which does the actual work.
//
// John Fletcher May 2006
//
// Further experiment. I am adding member functions swap(ThisType &f)
// to Fun objects which swap two functoids with the same descriptions.
// e.g.
//   Fun2<int,int,void> writetwof = ptr_to_fun(&writetwo<int,int>);
//   Fun2<int,int,void> writetwofr = ptr_to_fun(&writetwo_rev<int,int>);
//   writetwof.swap(writetwofr);
// See example usage in interesting.cpp
// This is so far added to Fun1 and Fun2.
//
// John Fletcher November 2008
//
// operator== for comparison of functoids.
//
// This is needed for work on the observer pattern.
// So far added to Fun0 only.
// 
// John Fletcher December 2009
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// This version has an experimental pointer class being developed.
// The tests are in class Fun4, as that was where it was evident 
// from some comments how this version differs from the boost version.
// It does nothing different at the moment, and all modified code is within
// conditional compilation as follows. 
//#ifdef FCPP_POINTERCLASSEXPERIMENT
//#endif
//
// John Fletcher March 2006
//
// For more information see the start of the Fun4 section.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Here is where FunN and FunNImpl classes are declared.  Also here are
//  - makeFunN    turn a monomorphic direct functoid into an indirect 
//                functoid
//  - convertN    for implicit conversions (subtype polymorphism)
//  - explicit_convertN   like convertN, but uses casts (non-implicit)
//
// Note: makeFunN is deprecated and removed in the boost version.
// Here it is used in currying of e.g. Fun2 and is 
// therefore retained (for now).
//////////////////////////////////////////////////////////////////////////

#include "ref_count.h"
#include "operator.h"

#ifndef FCPP_NO_USE_NAMESPACE
namespace fcpp {
#endif

// AnyType is the type to use when we don't care about the
// instantiation of a template. This usually happens when we are
// reading the signature of a monomorphic function as if it were
// polymorphic.
class AnyType {};

//////////////////////////////////////////////////////////////////////
// monomorphic traits. 
// These are defined in the boost version of FC++
// in full.hpp and used in function.hpp
// I want to introduce them here as part of implementing 
// concepts, to provide a way to get at the argument types.
// This has now been used to convert Fun1 to work with 
// concepts, using code copied from function.hpp of the boost
// version of FC++. I also had to adapt makeFun1.
// John Fletcher  June 2007.
//
// Note: These compile unconditionally as they could be used anywhere.
// Similar code is unconditional in the boost version.
//
// These have now been used to implement Sig free behaviour with 
// concepts for Fun1 and Fun2. In addition, Fun2 uses 
// concept maps defined for Full2 to implement currying 
// for which the returned objects are now Full1 objects with no use of 
// makeFun1. I think this fulfils a previous idea.
// 
// I have implemented currying of Full3 in full.h and used this to
// extending currying to Fun3. This has been tricky as Fun3 is implemented
// using Fun3Guts. This work has identified and solved a bug in 
// autocurrying of Fun3 with two parameters e.g. (_,2) and (1,_).
//
// Fun4 is different again and currying for that needs to follow 
// implementation first in Full4. Monmorphic traits are also needed.
//
// Fun5 largely follows Fun4.
//
// Fun6 and Fun7 are being implemented, following Fun5.
// These are now in separate files function6.h function7.h and function8.h
//
// These traits have now been made more useful to detect that
// an object is actually monomorphic and not compile if it is not.
// This follows to some extent the work in function.hpp and signature.hpp
// of the old boost version of FC++.
//
// Some of this will need further work to be compatible with concepts.
//
// See examples of use in newmonad.cpp, fcpptest4.cpp, fcpptest5.cpp and
// fcpptest6.cpp
//
// John Fletcher, October 2007, January and February 2008
//
// I am refactoring the code and adding more debug information.
// See examples in fcpptest8.cpp
//
// October 2016
//
//////////////////////////////////////////////////////////////////////

template <class F>
struct monomorphic_traits : public F {
   static std::string testit() { return "monomorphic_traits : public F"; }
   static const int max_args = FunctoidTraits<F>::max_args;
   typedef typename F::monomorphic monomorphic;
};
template <class F>
struct monomorphic_traits<Full0<F> > {
   static std::string testit() { return "monomorphic_traits<Full0<F> >"; }
   typedef typename F::result_type result_type;
   static const int max_args = FunctoidTraits<F>::max_args;
   typedef typename F::monomorphic monomorphic;
};
template <class F>
struct monomorphic_traits<Full1<F> > {
   static std::string testit() { return "monomorphic_traits<Full1<F> >"; }
   typedef typename F::first_argument_type first_argument_type;
   typedef typename F::argument_type argument_type;
   typedef typename F::result_type result_type;
   static const int max_args = FunctoidTraits<F>::max_args;
   typedef typename F::monomorphic monomorphic;
};
template <class F>
struct monomorphic_traits<Full2<F> > {
   static std::string testit() { return "monomorphic_traits<Full2<F> >"; }
   typedef typename F::first_argument_type first_argument_type;
   typedef typename F::second_argument_type second_argument_type;
   typedef typename F::result_type result_type;
   static const int max_args = FunctoidTraits<F>::max_args;
   typedef typename F::monomorphic monomorphic;
};
template <class F>
struct monomorphic_traits<Full3<F> > {
   static std::string testit() { return "monomorphic_traits<Full3<F> >"; }
   typedef typename F::first_argument_type first_argument_type;
   typedef typename F::second_argument_type second_argument_type;
   typedef typename F::third_argument_type third_argument_type;
   typedef typename F::result_type result_type;
   static const int max_args = FunctoidTraits<F>::max_args;
   typedef typename F::monomorphic monomorphic;
};
#ifdef FCPP152
/* These are now operational.
   Type definitions have now been added to CFunType definitions
   in signature.h.*/

template <class F>
struct monomorphic_traits<Full4<F> > {
   static std::string testit() { return "monomorphic_traits<Full4<F> >"; }
   typedef typename F::first_argument_type first_argument_type;
   typedef typename F::second_argument_type second_argument_type;
   typedef typename F::third_argument_type third_argument_type;
   typedef typename F::fourth_argument_type fourth_argument_type;
   typedef typename F::result_type result_type;
   static const int max_args = FunctoidTraits<F>::max_args;
};
template <class F>
struct monomorphic_traits<Full5<F> > {
   static std::string testit() { return "monomorphic_traits<Full5<F> >"; }
   typedef typename F::first_argument_type first_argument_type;
   typedef typename F::second_argument_type second_argument_type;
   typedef typename F::third_argument_type third_argument_type;
   typedef typename F::fourth_argument_type fourth_argument_type;
   typedef typename F::fifth_argument_type fifth_argument_type;
   typedef typename F::result_type result_type;
   static const int max_args = FunctoidTraits<F>::max_args;
};
template <class F>
struct monomorphic_traits<Full6<F> > {
   static std::string testit() { return "monomorphic_traits<Full6<F> >"; }
   typedef typename F::first_argument_type first_argument_type;
   typedef typename F::second_argument_type second_argument_type;
   typedef typename F::third_argument_type third_argument_type;
   typedef typename F::fourth_argument_type fourth_argument_type;
   typedef typename F::fifth_argument_type fifth_argument_type;
   typedef typename F::sixth_argument_type sixth_argument_type;
   typedef typename F::result_type result_type;
   static const int max_args = FunctoidTraits<F>::max_args;
};
template <class F>
struct monomorphic_traits<Full7<F> > {
   static std::string testit() { return "monomorphic_traits<Full7<F> >"; }
   typedef typename F::first_argument_type first_argument_type;
   typedef typename F::second_argument_type second_argument_type;
   typedef typename F::third_argument_type third_argument_type;
   typedef typename F::fourth_argument_type fourth_argument_type;
   typedef typename F::fifth_argument_type fifth_argument_type;
   typedef typename F::sixth_argument_type sixth_argument_type;
   typedef typename F::seventh_argument_type seventh_argument_type;
   typedef typename F::result_type result_type;
   static const int max_args = FunctoidTraits<F>::max_args;
};
template <class F>
struct monomorphic_traits<Full8<F> > {
   static std::string testit() { return "monomorphic_traits<Full8<F> >"; }
   typedef typename F::first_argument_type first_argument_type;
   typedef typename F::second_argument_type second_argument_type;
   typedef typename F::third_argument_type third_argument_type;
   typedef typename F::fourth_argument_type fourth_argument_type;
   typedef typename F::fifth_argument_type fifth_argument_type;
   typedef typename F::sixth_argument_type sixth_argument_type;
   typedef typename F::seventh_argument_type seventh_argument_type;
   typedef typename F::eighth_argument_type eighth_argument_type;
   typedef typename F::result_type result_type;
   static const int max_args = FunctoidTraits<F>::max_args;
};

#endif

//////////////////////////////////////////////////////////////////////
// Ok, this file has been a mess, so I'm trying to clean it up.  The
// file is divided into 4 sections, for Fun0, Fun1, Fun2, and Fun3.
// The sections are all pretty redundant, except that
//  - Fun2 implements its own currying (it was written before Curryables were)
//  - Fun3 uses Curryable3 to implement its currying
// As a result, I'm removing some of the redundant comments from all the
// sections except Fun0.  So basically, use the comments in the Fun0
// section as a reference for the corresponding structures in the other
// FunN classes.
//////////////////////////////////////////////////////////////////////

template <class Result>
struct Fun0Impl;

template <class Result>
struct Fun0;

template <class Rd, class Rs>   // result of dest, result of src
  Fun0Impl<Rd>* convert0( const IRef<const Fun0Impl<Rs> >& f );

template <class Rd, class DF>
struct Fun0Constructor;

// Special function for the default constructor to call.
template <class Result> 
Result nullfun0() { return Result(); }

template <class Result>
  class Fun0 : public ::fcpp::SmartFunctoid0 
// Added ::fcpp::SmartFunctoid0  - missing from way back.
{
   typedef IRef<const Fun0Impl<Result> > RefImpl;

   RefImpl ref;
   template <class T> friend class Fun0; 
   template <class Rd, class Rs>
   friend Fun0<Rd> explicit_convert0( const Fun0<Rs>& f );

   template <class Rd, class DF>
   friend struct Fun0Constructor;
public:
   // See comment in Fun0Impl about g++2.95.2
   typedef Result ResultType;
   template <class Dummy1=Void, class Dummy2=Void>
   struct Sig : public FunType<WrongNumberOfSigArgs> {};
   template <class Dummy>
   struct Sig<Void,Dummy> : public FunType<Result> {};

   template <class Dummy, int i> struct crazy_accepts {
      static const bool args = false;
   };
   template <class Dummy> struct crazy_accepts<Dummy,0> {
      static const bool args = true;
   };
   static const int crazy_max_args = 0;

   typedef const Fun0Impl<Result>* Impl;
   // int is dummy arg to differentiate from the template constructor
   Fun0( int, Impl i ) : ref(i) {}

   Result operator()() const { return ref->operator()(); }

   template <class DF>   // direct functoid (or subtype polymorphism)
   Fun0( const DF& f ) : ref( Fun0Constructor<Result,DF>::make(f) ) {}

   Fun0( const Fun0& x ) : ref(x.ref) {}
   Fun0& operator=( const Fun0& x ) { ref = x.ref; return *this; }
   // Experimental - never tried this before.
   // This is used in the observer implementation.
   bool operator==( const Fun0& x ) const { return ref == x.ref; }
#ifndef FCPP_OLD_GCC
   // Defined specially for the old gcc on research computer.
   Fun0() { 
       Fun0 tmp = ptr_to_fun( &nullfun0<Result>);
       ref = tmp.ref;
   }
#endif
#ifdef FCPP_ENABLE_LAMBDA
   typedef Fun0 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
#ifdef FCPP_DEBUG
   std::string name() const {
     // When a Fun0 object is made from a Full0 object,
     // it uses Fun0Constructor (see below). This does not store
     // the Full0 object so it would be necessary to store it
     // to get access to its name.
     // RefImpl is NOT the actual type of any object.
     // It is defined in this way above:
     //  typedef IRef<const Fun0Impl<Result> > RefImpl;
     // Fun0Impl<Result> is a struct inherited by various things
     // in order to convert input functions to the type needed.
     // It has a name() function to provide a default.
     // Fun0Impl<Result> is inherited by Gen0 which deals with a lot of cases.
     // XEFH and XEFTH also inherit Fun0Impl<Result>.
     // These are helpers for EnumFrom and EnumFromTo.
     // The complexity is now hidden using this function from debug_traits.h
     return debug_string_and_name_or_string("Fun0::",*ref,"Fun0(something)");
   }
#endif
};


// See comments below
template <class R> struct Inherits<Fun0<R>,CallableWithoutArguments>
{ static const bool value = true; };
template <class R> struct Inherits<Fun0<R>,::fcpp::SmartFunctoid>
{ static const bool value = true; };

template <class Result>
struct Fun0Impl : public IRefable {
   // g++2.95.2 doesn't implement the empty-base-class-optimization, so we
   // 'hand code' the Sig information rather than just inheriting it from a
   // CFunType class.  Fun0s are on the 'critical path' for lists, and need 
   // to be extra efficient.
   typedef Result ResultType;
   template <class Dummy1=Void, class Dummy2=Void>
   struct Sig : public FunType<WrongNumberOfSigArgs> {};
   template <class Dummy>
   struct Sig<Void,Dummy> : public FunType<Result> {};

   virtual Result operator()() const =0;
   virtual ~Fun0Impl() {}
#ifdef FCPP_DEBUG
   // Gen0 inherits from Fun0Impl<Result> so that its name()
   // replaces this one - good.
   // I have not yet managed that within the List context.
   virtual std::string name() const {
        return std::string("Fun0Impl<Result>(something)");
   }
#endif
};
// Since we cheated inheritance above, we need to inform our inheritance
// detector for the particular case of importance.
template <class R> struct Inherits<Fun0Impl<R>,CallableWithoutArguments>
{ static const bool value = true; };


template <class Rd, class Rs>
struct Fun0Converter : public Fun0Impl<Rd> {
  //public:
  typedef ::fcpp::IRef<const Fun0Impl<Rs> > MyFun;
   MyFun f;
public:
   Fun0Converter( const MyFun& g ) : f(g) {}
   Rd operator()() const {
#ifndef FCPP_CUDA_COMPATIBLE
      return f->operator()();
#else
/* 
Translation in stlexample.cu as the following:      
# 357 "/home/fletcher/FC++/fcpp/function.h"
return ((*(f)).__T7::operator()());
Why it is doing this I do not know.
In one case it is T7 and in another T16 but consistently.
All cases is one file are the same.
See Fun1 for discussion of the solution.
*/
      return f.unwrap()->template Fun0Impl<Rs>::operator()();
#endif
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Fun0Converter::",f,"Fun0Converter");
   }
#endif
};

template <class Rd, class Rs>
Fun0Impl<Rd>* convert0( const IRef<const Fun0Impl<Rs> >& f ) {
   return new Fun0Converter<Rd,Rs>( f );
}

template <class Rd, class Rs>
class Fun0ExplicitConverter : public Fun0Impl<Rd> {
  typedef fcpp::IRef<const Fun0Impl<Rs> > MyFun;
   MyFun f;
public:
   Fun0ExplicitConverter( const MyFun& g ) : f(g) {}
   Rd operator()() const {
#ifndef FCPP_CUDA_COMPATIBLE
      return static_cast<Rd>( f->operator()() );
#else
      return static_cast<Rd>( f.unwrap()->template Fun0Impl<Rd>::operator()() );
#endif
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Fun0ExplicitConverter::",f,
                                            "Fun0ExplicitConverter");
   }
#endif
};

template <class Rd, class Rs>
Fun0<Rd> explicit_convert0( const Fun0<Rs>& f ) {
   return Fun0<Rd>( 1, new Fun0ExplicitConverter<Rd,Rs>( f.ref ) );
}

template <class Gen>
class Gen0 
#ifdef FCPP_CONCEPTS
: public Fun0Impl<typename monomorphic_traits<Gen>::result_type>
#else
#ifdef FCPP_CUDA_COMPATIBLE
: public Fun0Impl<typename RT0<Gen>::ResultType> 
#else
: public Fun0Impl<typename RT<Gen>::ResultType> 
#endif
#endif
{
   Gen g;
public:
   Gen0( Gen x ) : g(x) {}
#ifdef FCPP_CONCEPTS
   inline ConstCallable0<Gen>::result_type
#else
#ifdef FCPP_CUDA_COMPATIBLE
   typename RT0<Gen>::ResultType 
#else
   typename RT<Gen>::ResultType 
#endif
#endif
   operator()() const { return g(); }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Gen0::",g,"Gen0(something)");
   }
#endif
};
   
template <class Gen>
#ifdef FCPP_CONCEPTS
Fun0<typename ConstCallable0<Gen>::result_type>
#else
#ifdef FCPP_CUDA_COMPATIBLE
Fun0<typename RT0<Gen>::ResultType>
#else
Fun0<typename RT<Gen>::ResultType>
#endif
#endif
makeFun0( const Gen& g ) 
{
#ifdef FCPP_CONCEPTS
   return Fun0<typename ConstCallable0<Gen>::result_type>(1,new Gen0<Gen>(g));
#else
#ifdef FCPP_CUDA_COMPATIBLE
   return Fun0<typename RT0<Gen>::ResultType>(1,new Gen0<Gen>(g));
#else
   return Fun0<typename RT<Gen>::ResultType>(1,new Gen0<Gen>(g));
#endif
#endif
}

template <class Nullary>
Gen0<Nullary>* makeFun0Ref( const Nullary& g ) {
   return new Gen0<Nullary>(g);
}

// Note: conversion-from-direct-functoid and subtype-polymorphism are
// really two different kinds of functionality.  However, if we try to
// create two separate constructors in the Fun0 class, we end up with
// ambiguity (C++ can't tell which one to call).  As a result, there is
// one constructor that handles both cases by forwarding the work to
// this class, which uses partial specialization to distinguish between
// the two cases.

// NOTE: DF stands for direct functoid.
template <class Rd, class DF>
struct Fun0Constructor {
    static Fun0Impl<Rd>* make( const DF& df ) {
     // I think this layer of wrapping is not needed.
      return makeFun0Ref( ::fcpp::monomorphize0<Rd>(df) );
      //return makeFun0Ref( df );
   }
};

template <class Rd, class Rs>
struct Fun0Constructor<Rd,Fun0<Rs> > {
   static Fun0Impl<Rd>* make( const Fun0<Rs>& f ) {
      return convert0<Rd>(f.ref);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("Fun0Constructor2");
   }
#endif
};

/////////////////////////////////////////////////////////////////////
// MakeN are moved from ecoop2b.cpp
/////////////////////////////////////////////////////////////////////
template <class T>
struct Make0 : public CFunType<T*> {
   T* operator()() const { return new T(); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("Make0");
   }
#endif
};

// Changed argument type to const ref.
template <class T, class A1>
struct Make1 : public CFunType<A1,T*> {
   T* operator()(const A1& x) const { return new T(x); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("Make1");
   }
#endif
};

// Made a new version with two arguments.
template <class T, class A1, class A2>
struct Make2 : public CFunType<A1,A2,T*> {
  T* operator()(const A1& x,const A2& y) const { return new T(x,y); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("Make2");
   }
#endif
};

// Made a new version with three arguments.
template <class T, class A1, class A2, class A3>
struct Make3 : public CFunType<A1,A2,A3,T*> {
  T* operator()(const A1& x,const A2& y, const A3 & z) const { return new T(x,y,z); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("Make3");
   }
#endif
};

#ifdef FCPP152
// Made a new version with four arguments.
template <class T, class A1, class A2, class A3, class A4>
  struct Make4 : public CFunType<A1,A2,A3,A4,T*> {
  T* operator()(const A1& x,const A2& y, const A3 & z, const A4 & w) const
  { return new T(x,y,z,w); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("Make4");
   }
#endif
};

#endif


/////////////////////////////////////////////////////////////////////
// LazyPtrProxy is quite general and could be included in FC++     //
// anywhere after Fun0 and makefunO have been defined.             //
// This is moved here from example ecoop2b.cc in FC++-clients.1.5  //
// There is a new example using this - ecoop2b.cpp                 //
// This was in the examples provided with FC++ 1.5 and is mentioned//
// in one of the papers. It has not been included in the headers   //
// until this version. It now has classes Proxy1, Proxy2 and Proxy3//
// provided below which wrap the lazy pointer.                     //
// These were not in FC++ 1.5.                                     //
//                                                                 //
// John Fletcher December 2015                                     //
/////////////////////////////////////////////////////////////////////
template <class T>
class LazyPtrProxy {
   Fun0<T*> f;
   mutable T* p;
   void cache() const { if(!p) p = f(); }
public:
   LazyPtrProxy( const Fun0<T*> ff ) : f(ff), p(0) {}
   template <class F> LazyPtrProxy( F ff ) : f(makeFun0(ff)), p(0) {}
   T& operator*() const { cache(); return *p; }
   T* operator->() const { cache(); return p; }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("LazyPtrProxy::",f,"LazyPtrProxy");
   }
#endif
};

////////////////////////////////////////////////////////////////////////
// These are useful as they are the start of generalised components
// for the proxy classes which otherwise have to be built from scratch.
// The question is, what interface to provide for these.
// At the moment it is operator() and constructor and converter
// for the LazyPtrProxy<F>.
// This makes no assumptions about what is provided by F
// except a constructor with the correct arguments.
// operator LazyPtrProxy<F>() and f() are provided for access to the
// structure of F which is not assumed here.
// See examples in ecoop2b.cpp and messages.cpp
////////////////////////////////////////////////////////////////////////
// Generalised Proxy class with template parameters F and A1.
template <class F, class A1>
class Proxy1
{
  LazyPtrProxy<F> f_inside;
public:
  Proxy1 (const A1& a )
    : f_inside(curry1(Make1<F,A1>(),a)) {}
  Proxy1 (const LazyPtrProxy<F>& lpf)
    : f_inside(lpf) {}
  void operator()() { (*f_inside)(); }
  operator LazyPtrProxy<F>() const { return f_inside; }
  LazyPtrProxy<F> f() const {  return f_inside; }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Proxy1::",f_inside,"Proxy1");
   }
#endif
};

// Generalised Proxy class with template parameters F A1 and A2.
template <class F, class A1, class A2>
class Proxy2
{
  LazyPtrProxy<F> f2_inside;
public:
  Proxy2 (const A1& a1, const A2& a2)
    : f2_inside(curry2(Make2<F,A1,A2>(),a1,a2)) {}
  Proxy2 (const LazyPtrProxy<F>& lpf2)
    : f2_inside(lpf2) {}
  void operator()() { (*f2_inside)(); }
  operator LazyPtrProxy<F>() const { return f2_inside; }
  LazyPtrProxy<F> f() const {  return f2_inside; }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Proxy2::",f2_inside,"Proxy2");
   }
#endif
};

// Generalised Proxy class with template parameters F A1 A2 and A3.
template <class F, class A1, class A2, class A3>
class Proxy3
{
  LazyPtrProxy<F> f3_inside;
public:
  Proxy3 (const A1& a1, const A2& a2, const A3& a3)
    : f3_inside(curry3(Make3<F,A1,A2,A3>(),a1,a2,a3)) {}
  Proxy3 (const LazyPtrProxy<F>& lpf3)
    : f3_inside(lpf3) {}
  void operator()() { (*f3_inside)(); }
  operator LazyPtrProxy<F>() const { return f3_inside; }
  LazyPtrProxy<F> f() const {  return f3_inside; }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Proxy3::",f3_inside,"Proxy3");
   }
#endif
};

#ifdef FCPP152
// Generalised Proxy class with template parameters F A1 A2 A3 and A4.
template <class F, class A1, class A2, class A3, class A4>
class Proxy4
{
  LazyPtrProxy<F> f4_inside;
public:
  Proxy4 (const A1& a1, const A2& a2, const A3& a3, const A4& a4)
    : f4_inside(curry4(Make4<F,A1,A2,A3,A4>(),a1,a2,a3,a4)) {}
  Proxy4 (const LazyPtrProxy<F>& lpf4)
    : f4_inside(lpf4) {}
  void operator()() { (*f4_inside)(); }
  operator LazyPtrProxy<F>() const { return f4_inside; }
  LazyPtrProxy<F> f() const {  return f4_inside; }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Proxy4::",f4_inside,"Proxy4");
   }
#endif
};

#endif

//////////////////////////////////////////////////////////////////////
// Now code for Fun1.
//////////////////////////////////////////////////////////////////////

template <class Arg1, class Result>
struct Fun1Impl;

template <class Gen>
class Gen1;

template <class A1d, class Rd, class A1s, class Rs>
Fun1Impl<A1d,Rd>* convert1( const IRef<const Fun1Impl<A1s,Rs> >& f );

template <class Ad, class Rd, class DF>
struct Fun1Constructor;

// Special function for the default constructor to call.
template <class Arg1, class Result>
Result nullfun1(Arg1 a) { return Result(); }

template <class Arg1, class Result>
class Fun1 : public CFunType<Arg1,Result>, public ::fcpp::SmartFunctoid1 {
  //  public:  // Added
   typedef IRef<const Fun1Impl<Arg1,Result> > RefImpl;
   RefImpl ref;
   template <class A, class B> friend class Fun1;
   
   template <class Ad, class Rd, class DF>
   friend struct Fun1Constructor;

   template <class A1d, class Rd, class A1s, class Rs> 
   friend Fun1<A1d,Rd> explicit_convert1( const Fun1<A1s,Rs>& f );
public:
   typedef Fun1Impl<Arg1,Result>* Impl;

   Fun1( int, Impl i ) : ref(i) {}
   typedef Fun1<Arg1,Result> ThisType;

#ifndef FCPP_CUDA_COMPATIBLE
   Result operator()( const Arg1& x ) const {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Fun1 operator() called with "
               << argument_traits_value(x) << std::endl;
#endif
      return ref->operator()(x);
   }
   Result operator()( const Arg1& x )  {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Fun1 operator() called with "
               << argument_traits_value(x) << std::endl;
#endif
      return ref->operator()(x);
   }
#else
   // This code does not compile with CUDA but I have not been able
   // to find out how to work around this and still have the code
   // work properly. The error message looks like this:
   // error: ‘__T7’ has not been declared
   // __T7 appears to be an internal thing in CUDA which has not been
   // correctly resolved.
   // So I have put in the type explicitly but now both CUDA and CLANG
   // do not link as they cannot find the operator at link time.
   // The operator in Fun1Impl is virtual and I am attempting to figure out
   // where it is really provided, and therefore what the proper type is.
   Result operator()( const Arg1& x ) const {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Fun1 operator() const called with "
               << argument_traits_value(x) << std::endl;
#endif
     // I thought this a solution, but it does not find it either.
     return ref.unwrap()->template Fun1Impl<Arg1,Result>::operator()(x);
     // The following is the one which compiles but cannot find the function.
     // return ref->template Fun1Impl<Arg1,Result>::operator()(x);
     // This is because Fun1Impl does not define the operator.
     // It is defined in a Gen1 object which is created by the constructor.
     // Fun1 is at this point sitting on top of a Full1 object.
     // This can be seen from the call sequence exposed when compiled with
     // both FCPP_DEBUG and FCPP_PRINT_HELPERS.
     // return ref->RefImpl::operator()(x);
     //       return ref->Gen1<G>::operator()(x);
     //     return ref->Gen1<ThisType>::operator()(x);
   }
   Result operator()( const Arg1& x )  {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Fun1 operator() called with "
               << argument_traits_value(x) << std::endl;
#endif
     //return ref.naked().operator()(x);
     return ref.unwrap()->template Fun1Impl<Arg1,Result>::operator()(x);
     // return ref->template Fun1Impl<Arg1,Result>::operator()(x);
     //   return ref->RefImpl::operator()(x);
     //   return ref->Gen1<G>::operator()(x);
     //   return ref->Gen1<ThisType>::operator()(x);
   }
#endif
   template <class DF>
   Fun1( const DF& df ) : ref( Fun1Constructor<Arg1,Result,DF>::make(df) ) {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Fun1 constructor(df) returns " << std::endl;
#endif
   }
   // added to sort out the bug with parser.cc example - I don't like it.
   //Fun1() {
   //  cout << "Did the dirty and called the null constructor  Fun1()" << endl;
   // } // and I get a segmentation fault
   Fun1( const Fun1& x ) : ref(x.ref) {}

   // Experimental swap routine following boost function
   // See boost/function/function_template.hpp
   void swap(ThisType &f) {
     if(f.ref == ref) return;
     RefImpl temp = ref;
     ref = f.ref;
     f.ref = temp;
   }

   Fun1& operator=( const Fun1& x ) {
#ifdef FCPP_PRINT_HELPERS
       std::cout << "Fun1 operator=  called " << std::endl;
#endif
       ref = x.ref; return *this;
   }
   Fun1() {
       Fun1 tmp = ptr_to_fun( &nullfun1<Arg1,Result>);
       ref = tmp.ref;
   }
#ifdef FCPP_ENABLE_LAMBDA
   typedef Fun1 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Fun1::",*ref,"Fun1");
   }
#endif
};


template <class Arg1, class Result>
struct Fun1Impl : public CFunType<Arg1,Result>, public IRefable {
// Attempt to sort out problems with translator.cpp
// causes other problems - needs investigation.
//#ifdef FCPP_USE_BOOST_TRAITS
//   typedef typename boost::call_traits<Arg1>::param_type A1;
//   virtual Result operator()( A1 ) const =0;
///#else
   virtual Result operator()( const Arg1& ) const =0;
//#endif
   virtual ~Fun1Impl() {}
#ifdef FCPP_DEBUG
   virtual std::string name() const {
        return std::string("Fun1Impl<Arg1,Result>(something)");
   }
#endif
};

template <class A1d, class Rd, class A1s, class Rs>
class Fun1Converter : public Fun1Impl<A1d,Rd> {
   typedef IRef<const Fun1Impl<A1s,Rs> > MyFun;
   MyFun f;
public:
   Fun1Converter( const MyFun& g ) : f(g)
   {
#ifdef FCPP_PRINT_HELPERS
       std::cout << "Fun1Converter  called " << std::endl;
#endif
   }
   Rd operator()( const A1d& x ) const {
#ifndef FCPP_CUDA_COMPATIBLE
      return f->operator()( x );
#else
      return f.unwrap()->template Fun1Impl<A1d,Rd>::operator()( x );
#endif
   }
};

template <class A1d, class Rd, class A1s, class Rs>
Fun1Impl<A1d,Rd>* convert1( const IRef<const Fun1Impl<A1s,Rs> >& f ) {
   return new Fun1Converter<A1d,Rd,A1s,Rs>( f );
}

template <class A1d, class Rd, class A1s, class Rs>
class Fun1ExplicitConverter : public Fun1Impl<A1d,Rd> {
   typedef IRef<const Fun1Impl<A1s,Rs> > MyFun;
   MyFun f;
public:
   Fun1ExplicitConverter( const MyFun& g ) : f(g) {
#ifdef FCPP_PRINT_HELPERS
       std::cout << "Fun1ExplicitConverter  called " << std::endl;
#endif
   }
   Rd operator()( const A1d& x ) const {
#ifndef FCPP_CUDA_COMPATIBLE
      return static_cast<Rd>( f->operator()(  static_cast<A1s>(x)  ) );
#else
      return static_cast<Rd>(
           f.unwrap()->template Fun1Impl<A1s,Rs>::operator()
           (  static_cast<A1s>(x)  ) );
#endif
   }
   //#ifdef FCPP_CUDA_COMPATIBLE
   //MyFun pointer() { return f;}
   //#endif
};

template <class A1d, class Rd, class A1s, class Rs>
Fun1<A1d,Rd> explicit_convert1( const Fun1<A1s,Rs>& f ) {
   return Fun1<A1d,Rd>( 1, new Fun1ExplicitConverter<A1d,Rd,A1s,Rs>(f.ref) );
}


#ifdef FCPP_USE_BOOST_VOID
template <class G,class A1,bool test_void> struct Gen1_impl;

template <class G, class A1>
  struct Gen1_impl<G,A1,true>
{
    static void do_it(const G& g, const A1& a1)
    {  g(a1); }
};

template <class G, class A1>
  struct Gen1_impl<G,A1,false>
  {
    // Defining a typename saves multiple #ifdef's within this
    // and some other function bodies.
#ifdef FCPP_CONCEPTS
    typedef typename ConstCallable1<G,A1>::result_type Result_Type;
#else
    typedef typename RT<G,A1>::ResultType Result_Type;
#endif
    static Result_Type do_it(const G& g, const A1& a1)
   {
     return g(a1);
    }
  };

#endif

template <class Gen>
#ifdef FCPP_CONCEPTS
class Gen1 : public Fun1Impl<
      typename monomorphic_traits<Gen>::first_argument_type,
      typename monomorphic_traits<Gen>::result_type> 
#else
class Gen1 : public Fun1Impl<typename Gen::template Sig<AnyType>::Arg1Type, 
                             typename Gen::template Sig<AnyType>::ResultType> 
#endif
{
   Gen g;
public:
   Gen1( Gen x ) : g(x) {
#ifdef FCPP_PRINT_HELPERS
       std::cout << "Gen1 called " << std::endl;
#endif
   }
#ifdef FCPP_CONCEPTS
   typedef typename monomorphic_traits<Gen>::result_type Result_Type;
   typedef typename monomorphic_traits<Gen>::first_argument_type Arg1Type;
   Result_Type operator()( const Arg1Type& x ) const {
#ifdef FCPP_PRINT_HELPERS
       std::cout << "Gen1 operator() called with " << x << std::endl;
#endif
#ifdef FCPP_USE_BOOST_VOID
#ifdef FCPP_PRINT_HELPERS
       std::cout << "Gen1_impl::do_it(g,x) called" << std::endl;
#endif
       return Gen1_impl<Gen,Arg1Type,
         boost::is_void<Result_Type>::value >::do_it(g,x);
#else
       return g(x);
#endif
   }
#else
   typedef typename Gen::template Sig<AnyType>::ResultType Result_Type;
   typedef typename Gen::template Sig<AnyType>::Arg1Type Arg1Type;
   Result_Type operator()( const Arg1Type& x ) const {
#ifdef FCPP_PRINT_HELPERS
       std::cout << "Gen1 operator() called with "
                 << argument_traits_value(x) << std::endl;
#endif
#ifdef FCPP_USE_BOOST_VOID
#ifdef FCPP_PRINT_HELPERS
       std::cout << "Gen1_impl::do_it(g,x) called" << std::endl;
#endif
       return Gen1_impl<Gen,Arg1Type,
	 boost::is_void<Result_Type>::value >::do_it(g,x);
#else
       return g(x);
#endif
   }
#endif
   Gen fun_g() { return g; }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Gen1::",g,"Gen1");
   }
#endif
};
   

#ifdef FCPP_CONCEPTS
// Probably not needed once concepts are implemented for currying.
// This may in fact be unusable.
template <class Unary>
Fun1<
      typename monomorphic_traits<Unary>::first_argument_type,
      typename monomorphic_traits<Unary>::result_type>
makeFun1( const Unary& g ) {
#ifdef FCPP_PRINT_HELPERS
	std::cout << "Used new makeFun1" << std::endl;
#endif
   return
     Fun1<
      typename monomorphic_traits<Unary>::first_argument_type,
      typename monomorphic_traits<Unary>::result_type>
        (1,new Gen1<Unary>(g));
}
#else
template <class Unary>
Fun1<typename Unary::template Sig<AnyType>::Arg1Type,
     typename Unary::template Sig<AnyType>::ResultType>
makeFun1( const Unary& g ) {
#ifdef FCPP_PRINT_HELPERS
	std::cout << "Used old makeFun1" << std::endl;
#endif
   return
     Fun1<typename Unary::template Sig<AnyType>::Arg1Type,
          typename Unary::template Sig<AnyType>::ResultType>
        (1,new Gen1<Unary>(g));
}
#endif

template <class Unary>
Gen1<Unary>* makeFun1Ref( const Unary& g ) {
#ifdef FCPP_PRINT_HELPERS
	std::cout << "Used makeFun1Ref" << std::endl;
#endif
   return new Gen1<Unary>(g);
}

template <class Ad, class Rd, class DF>
struct Fun1Constructor {
   static Fun1Impl<Ad,Rd>* make( const DF& df ) {
#ifdef FCPP_PRINT_HELPERS
	std::cout << "Used Fun1Constructor(A) make()" << std::endl;
#endif
	// Remove a layer of monomporhize which is not needed.
      return makeFun1Ref( ::fcpp::monomorphize1<Ad,Rd>(df) );
      //return makeFun1Ref( df );
   }
};
template <class Ad, class Rd, class As, class Rs>
struct Fun1Constructor<Ad,Rd,Fun1<As,Rs> > {
   static Fun1Impl<Ad,Rd>* make( const Fun1<As,Rs>& f ) {
#ifdef FCPP_PRINT_HELPERS
	std::cout << "Used Fun1Constructor(B) make()" << std::endl;
#endif
      return convert1<Ad,Rd>(f.ref);
   }
};

//////////////////////////////////////////////////////////////////////

template <class Arg1, class Arg2, class Result>
struct Fun2Impl;

template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
Fun2Impl<A1d,A2d,Rd>* convert2( const IRef<const Fun2Impl<A1s,A2s,Rs> >& f );

template <class A1d, class A2d, class Rd, class DF>
struct Fun2Constructor;

// Special function for the default constructor to call.
template <class Arg1, class Arg2, class Result> 
Result nullfun2(Arg1 a, Arg2 b) { return Result(); }

// Note that this class has two signatures: it can be used either as
// a two argument function or as a single argument function (currying).
template <class Arg1, class Arg2, class Result>
class Fun2 : public CFunType<Arg1,Arg2,Result>, public ::fcpp::SmartFunctoid2 {
   typedef IRef<const Fun2Impl<Arg1, Arg2, Result> > RefImpl;
   RefImpl ref;
   template <class A1, class A2, class R> friend class Fun2;
   template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
   friend Fun2<A1d,A2d,Rd> explicit_convert2( const Fun2<A1s,A2s,Rs>& f );

   template <class A1d, class A2d, class Rd, class DF>
   friend struct Fun2Constructor;

   // kludge while this file not in namepsace (FCPP_NO_USE_NAMESPACE)
   typedef fcpp::AutoCurryType AutoCurryType;   
public:
   // typedef i_am_monomorphic monomorphic;
#ifndef FCPP_CONCEPTS
   // Signature for normal use of the functoid (2 args)
   template <class P1, class P2 = void>
   struct Sig : public FunType<Arg1, Arg2, Result> {};

   // Signature for using this function with automatic currying (1-arg)
   template <class P1>
   struct Sig<P1,void> : public FunType<Arg1, Fun1<Arg2, Result> > {};

   // Signatures for using this function with underscore currying (1-arg)
   template <class P2>
   struct Sig<AutoCurryType,P2> 
   : public FunType<AutoCurryType, P2, Fun1<Arg1, Result> > {};
   template <class P1>
   struct Sig<P1,AutoCurryType> 
   : public FunType<P1, AutoCurryType, Fun1<Arg2, Result> > {};
#endif

   typedef Fun2Impl<Arg1,Arg2,Result>* Impl;
   Fun2( int, Impl i ) : ref(i) {}
   
   template <class DF>
   Fun2( const DF& df ) 
   : ref( Fun2Constructor<Arg1,Arg2,Result,DF>::make(df) ) {}

   Fun2( const Fun2& x ) : ref(x.ref) {}
   Fun2& operator=( const Fun2& x ) { ref = x.ref; return *this; }
   Fun2() { 
     Fun2 tmp = ptr_to_fun( &nullfun2<Arg1,Arg2,Result>);
     ref = tmp.ref;
   }

   typedef Fun2<Arg1,Arg2,Result> ThisType;
   // Experimental swap routine following boost function
   // See boost/function/function_template.hpp
   void swap(ThisType &f) {
     if(f.ref == ref) return;
     RefImpl temp = ref;
     ref = f.ref;
     f.ref = temp;
   }

   // normal call
   Result operator()( const Arg1& x, const Arg2& y ) const {
#ifndef FCPP_CUDA_COMPATIBLE
      return ref->operator()(x,y);
#else
      return ref.unwrap()->template Fun2Impl<Arg1,Arg2,Result>::operator()(x,y);
#endif
   }
  
   // inheritable underscore currying!
#ifdef FCPP_CONCEPTS
   // This is an important change as the result is a full functoid
   // not a function object.
  ConstCallable2<Fun2<Arg1,Arg2,Result>,AutoCurryType,Arg2>::result_type
   operator()( const AutoCurryType&, const Arg2& y ) const { 
      return bind2of2(*this, y);
   }
#else
   Fun1<Arg1, Result> 
   operator()( const AutoCurryType&, const Arg2& y ) const { 
      return makeFun1(bind2of2(*this, y));
   }
#endif

#ifdef FCPP_CONCEPTS
  ConstCallable2<Fun2<Arg1,Arg2,Result>,Arg1,AutoCurryType>::result_type
   operator()( const Arg1& x, const AutoCurryType& ) const { 
      return bind1of2(*this, x);
   }
#else
   Fun1<Arg2, Result> 
   operator()( const Arg1& x, const AutoCurryType& ) const { 
      return makeFun1(bind1of2(*this, x));
   }
#endif
 

   // REVIEW: Note that this could return a direct functoid, too, which
   // might be more efficient.  Same with other currying calls.

   // inheritable automatic currying!
#ifdef FCPP_CONCEPTS
   // This is an important change as the result is a full functoid
   // not a function object.
   ConstCallable2<Fun2<Arg1,Arg2,Result>,Arg1 >::result_type
   operator()( const Arg1& x) const {
     return bind1of2(*this, x);
   }
#else
   Fun1<Arg2,Result> operator()( const Arg1& x) const {
      return makeFun1(bind1of2(*this, x));
   }
#endif

#ifdef FCPP_ENABLE_LAMBDA
   typedef Fun2 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Fun2::",*ref,"Fun2");
   }
#endif
};

template <class Arg1, class Arg2, class Result>
struct Fun2Impl : public CFunType<Arg1,Arg2,Result>, public IRefable {
//#ifdef FCPP_USE_BOOST_TRAITS
//    typedef typename boost::call_traits<Arg1>::param_type A1;
//    typedef typename boost::call_traits<Arg2>::param_type A2;
//    virtual Result operator()( A1, A2) const = 0;
//#else
   virtual Result operator()( const Arg1&, const Arg2& ) const =0;
//#endif
   virtual ~Fun2Impl() {}
#ifdef FCPP_DEBUG
   virtual std::string name() const {
        return std::string("Fun2Impl<Arg1,Arg2,Result>(something)");
   }
#endif
};

template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
class Fun2Converter : public Fun2Impl<A1d,A2d,Rd> {
   typedef IRef<const Fun2Impl<A1s,A2s,Rs> > MyFun;
   MyFun f;
public:
   Fun2Converter( const MyFun& g ) : f(g) {}
//#ifdef FCPP_USE_BOOST_TRAITS
//    typedef typename boost::call_traits<A1d>::param_type A1;
//    typedef typename boost::call_traits<A2d>::param_type A2;
//   Rd operator()( A1 x, A2 y ) const {
//#else
   Rd operator()( const A1d& x, const A2d&  y ) const {
//#endif
#ifndef FCPP_CUDA_COMPATIBLE
      return f->operator()( x, y );
#else
      return f.unwrap()->template Fun2Impl<A1d,A2d,Rd>::operator()( x, y );
#endif
   }
};

template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
Fun2Impl<A1d,A2d,Rd>* convert2( const IRef<const Fun2Impl<A1s,A2s,Rs> >& f ) {
   return new Fun2Converter<A1d,A2d,Rd,A1s,A2s,Rs>( f );
}

template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
class Fun2ExplicitConverter : public Fun2Impl<A1d,A2d,Rd> {
   typedef IRef<const Fun2Impl<A1s,A2s,Rs> > MyFun;
   MyFun f;
public:
   Fun2ExplicitConverter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y ) const {
#ifndef FCPP_CUDA_COMPATIBLE
      return static_cast<Rd>( f->operator()( static_cast<A1s>(x),
                                             static_cast<A2s>(y) ) );
#else
      return static_cast<Rd>(
        f.unwrap()->template Fun2Impl<A1s,A2s,Rs>::operator()
                ( static_cast<A1s>(x), static_cast<A2s>(y) ) );
#endif
   }
};

template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
Fun2<A1d,A2d,Rd> explicit_convert2( const Fun2<A1s,A2s,Rs>& f ) {
   return Fun2<A1d,A2d,Rd>( 1, 
      new Fun2ExplicitConverter<A1d,A2d,Rd,A1s,A2s,Rs>(f.ref) );
}

template <class Gen>
class Gen2 : public Fun2Impl<
#ifdef FCPP_CONCEPTS
      typename monomorphic_traits<Gen>::first_argument_type,
      typename monomorphic_traits<Gen>::second_argument_type,
      typename monomorphic_traits<Gen>::result_type>
#else
      typename Gen::template Sig<AnyType, AnyType>::Arg1Type, 
      typename Gen::template Sig<AnyType, AnyType>::Arg2Type,
      typename Gen::template Sig<AnyType, AnyType>::ResultType>
#endif
{
   Gen g;
public:
   Gen2( Gen x ) : g(x) {}
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::result_type
   operator()(
      const typename monomorphic_traits<Gen>::first_argument_type& x,
      const typename monomorphic_traits<Gen>::second_argument_type& y
#else
   typename Gen::template Sig<AnyType, AnyType>::ResultType
   operator()(
      const typename Gen::template Sig<AnyType,AnyType>::Arg1Type& x,
      const typename Gen::template Sig<AnyType,AnyType>::Arg2Type& y
#endif
) const
   {
      return g(x,y);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Gen2::",g,"Gen2");
   }
#endif
};

#ifdef FCPP_CONCEPTS
template <class Binary>
Fun2<
      typename monomorphic_traits<Binary>::first_argument_type,
      typename monomorphic_traits<Binary>::second_argument_type,
      typename monomorphic_traits<Binary>::result_type>
makeFun2( const Binary& g ) {
#ifdef FCPP_PRINT_HELPERS
	std::cout << "Used new makeFun2" << std::endl;
#endif
   return Fun2<
      typename monomorphic_traits<Binary>::first_argument_type,
      typename monomorphic_traits<Binary>::second_argument_type,
      typename monomorphic_traits<Binary>::result_type>
     (1,new Gen2<Binary>(g));
}
#else
template <class Binary>
Fun2<typename Binary::template Sig<AnyType, AnyType>::Arg1Type, 
     typename Binary::template Sig<AnyType, AnyType>::Arg2Type,
     typename Binary::template Sig<AnyType, AnyType>::ResultType> 
makeFun2( const Binary& g ) {
#ifdef FCPP_PRINT_HELPERS
	std::cout << "Used old makeFun2" << std::endl;
#endif
   return Fun2<typename Binary::template Sig<AnyType, AnyType>::Arg1Type,
               typename Binary::template Sig<AnyType, AnyType>::Arg2Type,
               typename Binary::template Sig<AnyType, AnyType>::ResultType> 
     (1,new Gen2<Binary>(g));
}
#endif

template <class Binary>
Gen2<Binary>* makeFun2Ref( const Binary& g ) {
   return new Gen2<Binary>(g);
}

template <class A1d, class A2d, class Rd, class DF>
struct Fun2Constructor {
   static Fun2Impl<A1d,A2d,Rd>* make( const DF& df ) {
     return makeFun2Ref( ::fcpp::monomorphize2<A1d,A2d,Rd>(df) );
     //return makeFun2Ref( df ); // Take out monomorphize2.
   }
};
template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
struct Fun2Constructor<A1d,A2d,Rd,Fun2<A1s,A2s,Rs> > {
   static Fun2Impl<A1d,A2d,Rd>* make( const Fun2<A1s,A2s,Rs>& f ) {
      return convert2<A1d,A2d,Rd>(f.ref);
   }
};


//////////////////////////////////////////////////////////////////////
// Fun3 implementation
//////////////////////////////////////////////////////////////////////

template <class Arg1, class Arg2, class Arg3, class Result>
struct Fun3Impl;

template <class Arg1, class Arg2, class Arg3, class Result>
struct Fun3;

template <class A1d, class A2d, class A3d, class Rd, 
          class A1s, class A2s, class A3s, class Rs> 
Fun3Impl<A1d,A2d,A3d,Rd>* 
convert3( const IRef<const Fun3Impl<A1s,A2s,A3s,Rs> >& f );

template <class A1d, class A2d, class A3d, class Rd, class DF>
struct Fun3Constructor;

// Special function for the default constructor to call.
template <class Arg1, class Arg2, class Arg3, class Result> 
Result nullfun3(Arg1 a, Arg2 b, Arg3 c) { return Result(); }

// The "Guts" class helps us implement currying; Fun3 floats gently atop
// Fun3Guts and adds currying.  
template <class Arg1, class Arg2, class Arg3, class Result>
class Fun3Guts : public CFunType<Arg1,Arg2,Arg3,Result> {
   typedef IRef<const Fun3Impl<Arg1,Arg2,Arg3,Result> > RefImpl;
   RefImpl ref;
   template <class A, class B, class C, class D> friend class Fun3Guts;
   template <class A, class B, class C, class D> friend class Fun3;
   
   template <class A1d, class A2d, class A3d, class Rd, class DF>
   friend struct Fun3Constructor;

   template <class A1d, class A2d, class A3d, class Rd, 
             class A1s, class A2s, class A3s, class Rs> 
   friend Fun3<A1d,A2d,A3d,Rd> 
   explicit_convert3( const Fun3<A1s,A2s,A3s,Rs>& f );
public:
   typedef Fun3Impl<Arg1,Arg2,Arg3,Result>* Impl;

   Fun3Guts( int, Impl i ) : ref(i) {}
   Result operator()( const Arg1& x, const Arg2& y, const Arg3& z ) const 
#ifndef FCPP_CUDA_COMPATIBLE
      { return ref->operator()(x,y,z); }
#else
   {
        return ref.unwrap()->template Fun3Impl<Arg1,Arg2,Arg3,Result>
                           ::operator()(x,y,z);
   }
#endif

   template <class DF>
   Fun3Guts( const DF& df )
   : ref( Fun3Constructor<Arg1,Arg2,Arg3,Result,DF>::make(df) ) {}

   Fun3Guts( const Fun3Guts& x ) : ref(x.ref) {}
   Fun3Guts& operator=( const Fun3Guts& x ) { ref = x.ref; return *this; }
   Fun3Guts() { 
     Fun3Guts tmp = 
     ptr_to_fun( &nullfun3<Arg1,Arg2,Arg3,Result>);
     ref = tmp.ref;
   }
   // Put in for tests with LC++
   bool operator==( const Fun3Guts& x ) { return ref == x.ref; }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Fun3Guts");
   }
#endif
};

template <class Arg1, class Arg2, class Arg3, class Result>
  class Fun3 : public CFunType<Arg1,Arg2,Arg3,Result>, public ::fcpp::SmartFunctoid3 {
   template <class A1d, class A2d, class A3d, class Rd, class DF>
   friend struct Fun3Constructor;

   template <class A1d, class A2d, class A3d, class Rd, 
             class A1s, class A2s, class A3s, class Rs> 
   friend Fun3<A1d,A2d,A3d,Rd> 
   explicit_convert3( const Fun3<A1s,A2s,A3s,Rs>& f );

   Fun3Guts<Arg1,Arg2,Arg3,Result> rep;
public:
   typedef Fun3Impl<Arg1,Arg2,Arg3,Result>* Impl;

   Fun3( int, Impl i ) : rep(1,i) {}

   template <class DF> 
   Fun3( const DF& df ) : rep(df) {}

   Fun3( const Fun3& x ) : rep(x.rep) {}
   Fun3& operator=( const Fun3& x ) { rep = x.rep; return *this; }
   Fun3() {
     //Actual implementation is in default constructor for Fun3Guts
     //Fun3 tmp = ptr_to_fun( &nullfun3<Arg1,Arg2,Arg3,Result>);
     //rep = tmp.rep;
   }
   // Put in for tests with LC++
   bool operator==( const Fun3& x ) { return rep == x.rep; }
   
#ifndef FCPP_CONCEPTS
   typedef fcpp::Curryable3<Fun3Guts<Arg1,Arg2,Arg3,Result> > SigHelp;
   // This would be in line with Fun4:
   //typedef Full3<Fun3Guts<Arg1,Arg2,Arg3,Result> > SigHelp;
   template <class A, class B=void, class C=void>
   struct Sig : public SigHelp::template Sig<A,B,C> {};
#endif

   template <class A, class B, class C>
#ifdef FCPP_CONCEPTS
   inline ConstCallable3<Fun3Guts<Arg1,Arg2,Arg3,Result>,A,B,C >::result_type
   operator()( const A& x, const B& y, const C& z ) const 
      { 
#ifdef FCPP_PRINT_HELPERS
	std::cout << "ConstCallable3 used in Fun3" << std::endl;
#endif
 	// Use helper to cover the cases with autocurrying.
     return impl::Curryable3Helper<typename ConstCallable3<
            Fun3Guts<Arg1,Arg2,Arg3,Result>,A,B,C>::result_type,
            Fun3Guts<Arg1,Arg2,Arg3,Result>,A,B,C>::go(rep,x,y,z);
        //return ::fcpp::makeFull3(rep)(x,y,z); 
      }
#else 
   typename Sig<A,B,C>::ResultType
   operator()( const A& x, const B& y, const C& z ) const 
      { return ::fcpp::makeFull3(rep)(x,y,z); }
#endif

   template <class A, class B>
#ifdef FCPP_CONCEPTS
   inline ConstCallable3<Fun3Guts<Arg1,Arg2,Arg3,Result>,A,B >::result_type
   operator()( const A& x, const B& y ) const 
      { 
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable3Helper2<typename ConstCallable3<
            Fun3Guts<Arg1,Arg2,Arg3,Result>,A,B>::result_type,
            Fun3Guts<Arg1,Arg2,Arg3,Result>,A,B>::go(rep,x,y);
     //return bind1and2of3(rep,x,y); 
      }
#else
   typename Sig<A,B>::ResultType
   operator()( const A& x, const B& y ) const 
     { return ::fcpp::makeFull3(rep)(x,y); }
   // Altered from the line below to enable autocurrying.
   // NOTE: This is a bug in FC++ 1.5.
   //      { return ::fcpp::curry3(rep,x,y); }
#endif

#ifdef FCPP_CONCEPTS
   // This is an important change as the result is a full functoid
   // not a function object.
   inline ConstCallable3<Fun3Guts<Arg1,Arg2,Arg3,Result>,Arg1 >::result_type
   operator()( const Arg1& x) const {
     return bind1of3(rep, x);
   }
#else
   template <class A>
   typename Sig<A>::ResultType
   operator()( const A& x ) const 
     {
       return bind1of3(rep, x);
     }
   // { return ::fcpp::curry3(rep,x); }
#endif

#ifdef FCPP_ENABLE_LAMBDA
   typedef Fun3 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Fun3::",*(rep.ref),"Fun3");
   }
#endif
};

template <class Arg1, class Arg2, class Arg3, class Result>
struct Fun3Impl : public CFunType<Arg1,Arg2,Arg3,Result>, public IRefable {
public:
  Fun3Impl() {}

   virtual Result operator()( const Arg1&, const Arg2&, const Arg3& ) const =0;
   virtual ~Fun3Impl() {}
#ifdef FCPP_DEBUG
   virtual std::string name() const {
        return std::string("Fun3Impl<Arg1,Arg2,Arg3,Result>(something)");
   }
#endif
};

//bool operator==( const Fun3Impl<Arg1,Arg2,Arg3,Result>& x ) { return ref == x.ref; }

template <class A1d, class A2d, class A3d, class Rd,
          class A1s, class A2s, class A3s, class Rs>
class Fun3Converter : public Fun3Impl<A1d,A2d,A3d,Rd> {
   typedef IRef<const Fun3Impl<A1s,A2s,A3s,Rs> > MyFun;
   MyFun f;
public:
   Fun3Converter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y, const A3d& z ) const {
#ifndef FCPP_CUDA_COMPATIBLE
      return f->operator()( x, y, z );
#else
      return f.unwrap()->template Fun3Impl<A1d,A2d,A3d,Rd>::operator()( x, y, z );
#endif
   }
};

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
#ifndef FCPP_CUDA_COMPATIBLE
      return static_cast<Rd>( f->operator()(  static_cast<A1s>(x),
         static_cast<A2s>(y), static_cast<A3s>(z)  ) );
#else
      return static_cast<Rd>(
          f.unwrap()->template Fun3Impl<A1s,A2s,A3s,Rs>::operator()
       (  static_cast<A1s>(x), static_cast<A2s>(y), static_cast<A3s>(z)  ) );
#endif
   }
};

template <class A1d, class A2d, class A3d, class Rd,
          class A1s, class A2s, class A3s, class Rs>
Fun3<A1d,A2d,A3d,Rd> explicit_convert3( const Fun3<A1s,A2s,A3s,Rs>& f ) {
   return Fun3<A1d,A2d,A3d,Rd>( 1,
      new Fun3ExplicitConverter<A1d,A2d,A3d,Rd,A1s,A2s,A3s,Rs>(f.rep.ref) );
}

template <class Gen>
class Gen3 : public Fun3Impl<
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::first_argument_type,
   typename monomorphic_traits<Gen>::second_argument_type,
   typename monomorphic_traits<Gen>::third_argument_type,
   typename monomorphic_traits<Gen>::result_type>
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType>::Arg1Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType>::Arg2Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType>::Arg3Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType>::ResultType>
#endif
   {
   Gen g;
public:
   Gen3( Gen x ) : g(x) {}
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::result_type operator()(
      const typename monomorphic_traits<Gen>::first_argument_type& x,
      const typename monomorphic_traits<Gen>::second_argument_type& y,
      const typename monomorphic_traits<Gen>::third_argument_type& z ) const
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType>::ResultType
   operator()(
   const typename Gen::template Sig<AnyType,AnyType,AnyType>::Arg1Type& x,
   const typename Gen::template Sig<AnyType,AnyType,AnyType>::Arg2Type& y,
   const typename Gen::template Sig<AnyType,AnyType,AnyType>::Arg3Type& z
             ) const
#endif
   {
      return g(x,y,z);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Gen3::",g,"Gen3");
   }
#endif
};

template <class Ternary>
Fun3<typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg1Type,
     typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg2Type,
     typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg3Type,
     typename Ternary::template Sig<AnyType,AnyType,AnyType>::ResultType>
makeFun3( const Ternary& g ) {
#ifdef FCPP_PRINT_HELPERS
	std::cout << "Used old makeFun3" << std::endl;
#endif
   return 
   Fun3<typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg1Type,
        typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg2Type,
        typename Ternary::template Sig<AnyType,AnyType,AnyType>::Arg3Type,
        typename Ternary::template Sig<AnyType,AnyType,AnyType>::ResultType>
        (1,new Gen3<Ternary>(g));
}

template <class Ternary>
Gen3<Ternary>* makeFun3Ref( const Ternary& g ) {
   return new Gen3<Ternary>(g);
}

template <class A1d, class A2d, class A3d, class Rd, class DF>
struct Fun3Constructor {
   static Fun3Impl<A1d,A2d,A3d,Rd>* make( const DF& df ) {
     return makeFun3Ref( ::fcpp::monomorphize3<A1d,A2d,A3d,Rd>(df) );
     //return makeFun3Ref( df ); // remove monomorphize3
   }
};
template <class A1d, class A2d, class A3d, class Rd,
          class A1s, class A2s, class A3s, class Rs>
struct Fun3Constructor<A1d,A2d,A3d,Rd,Fun3<A1s,A2s,A3s,Rs> > {
   static Fun3Impl<A1d,A2d,A3d,Rd>* make( const Fun3<A1s,A2s,A3s,Rs>& f ) {
      return convert3<A1d,A2d,A3d,Rd>(f.rep.ref);
   }
};

// FC++.1.5.2
#ifdef FCPP152

#ifdef FCPP_POINTERCLASSEXPERIMENT

// ++++++++++++++++++++++++++++++++++++++++++
// Experimental Pointer Policy Classes
// ++++++++++++++++++++++++++++++++++++++++++

// These are generic but at present applied only in Fun4.

// Not implemented yet
//template <class B>
//class Boost_Pointer_Class {
// public:
//  typedef boost::intrusive_ptr<B> Pointer_Type;
//};

/* Not implemented yet.
template <typename T>
class IRefTraits
{
 private:
 template <class U> struct testIRef
 {
   enum { result = false };
 };
 template <class U> struct testIRef<typename U::inner>
 {
   enum { result = true };
 };
 public: 
 enum { isIRef = testIRef<T>::result }; 
};
*/

// Simple pointer class not used in this experiment.
//template <class B>
//class IRef_Pointer_Class {
// public:
//  typedef IRef<B> Pointer_Type;
//};

// This pointer class implementation
// provided the functionality to use IRef.
// It does this by providing the member functions incref and decref
// which IRef needs. These are located in a class called Inner,
// which also contains a member function
// unwrap() which returns the original pointer.

// Originally I designed Inner to inherit from the class of the pointer
// passed in.  This turned out to be unnecessary and also did not work
// for pointers, such as Fun4Impl*, which point to objects which cannot be
// instantiated as they are abstract.

// Doing it this way works as the unwrap() function allows access to the
// underlying operator() without having the instantiate the Fun4Impl,
// which would fail.

// Usage:

// The class definition must include the template parameter
// template <class> class Pointer_Class = Another_IRef_Pointer_Class
// The top level class (here Fun4) has the default parameter, the others do not.
//
// Define the types of the pointers within the classes as needed.
// typedef typename Pointer_Class<const Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result> >
//                               ::Pointer_Type RefImpl;
// typedef Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result>* Impl;

// Make a pointer.
// RefImpl ref;
// Impl j;
// ref =  Pointer_Class<const Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result> >
//        ::makeRefable(j);

// Retrieve the original pointer using either of these:
// ref->unwrap()->operator()(x,y,z,v);
// (*ref>->operator()(x,y,z,v);

// This interface should be implementable for other pointer types
// and also for the other FunN functions.

// John Fletcher  March 2006.

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <class B>
class Another_IRef_Pointer_Class {
 public:
// Modified so that it does not inherit, but just holds the pointer.
  template <class C>
      class Inner //: public C  
  {
    mutable RefCountType refC_;
    C* c_;
  public:
    //Inner() { }
    enum { in = true } inner;
    Inner(const C* c) : c_(c) , refC_(0) { }
    void incref() const { ++refC_; }
    void decref() const { if (!--refC_) delete this; }
    operator C*() const { return c_; }
    // When the pointer is needed call this.
    C* unwrap() const { return c_; }
    // Don't use this - wrong argument list.
    //C* operator->() const { return c_; }
  };
  typedef IRef<Inner<B> > Pointer_Type;
  static Pointer_Type makeRefable(const B* b) 
  { 
      Inner<B> *bp = new Inner<B>(b);
       return IRef<Inner<B> >(bp); 
  }
};


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif

//////////////////////////////////////////////////////////////////////
// Fun4 implementation.
//////////////////////////////////////////////////////////////////////

// ++++++++++++++++++++++++++++++++++++++++++
// Experimental Pointer Policy Class for Fun4
// ++++++++++++++++++++++++++++++++++++++++++

// This is a first stage towards a pointer policy for Fun4. 
// It is contained withing the following conditional compilation:
//#ifdef FCPP_POINTERCLASSEXPERIMENT
//#endif

// At the moment there is no implementation for the alternative boost pointers
// used in the boost version, which have a different interface.
// More thought is needed to sort this out.

// I have taken this a stage further in using Another_IRef_Pointer_Class
// to handle the IRef business, only to find out that there is more to be
// tackled lower down in the convert processes.  I think that some more code
// is going to have to know about the Pointer_Class.

// I have made considerable progress, but there is some more sorting out
// to be done.

// The final point was that the class Inner in the Pointer_Class
// implementation does not need to inherit from the class it is receiving,
// just store the pointer.

// More testing is needed, but the examples in ideas5.cc all work now.

// John Fletcher (J.P.Fletcher@aston.ac.uk) 9th March 2005.

#ifdef FCPP_POINTERCLASSEXPERIMENT
template <class Arg1, class Arg2, class Arg3, class Arg4, class Result>
class Fun4Impl : public CFunType<Arg1,Arg2,Arg3,Arg4,Result> {
#else
template <class Arg1, class Arg2, class Arg3, class Arg4, class Result>
class Fun4Impl : public CFunType<Arg1,Arg2,Arg3,Arg4,Result> , public IRefable {
#endif
  //   mutable RefCountType refC;
public:
   Fun4Impl() {}
   virtual Result operator() ( const Arg1&, const Arg2&, const Arg3&, const Arg4&) const =0;
   virtual ~Fun4Impl() {}

   //template <class X, class Y, class Z, class V, class W>
   //friend void intrusive_ptr_add_ref( const Fun4Impl<X,Y,Z,V,W>* p );
   //template <class X, class Y, class Z, class V, class W>
   //friend void intrusive_ptr_release( const Fun4Impl<X,Y,Z,V,W>* p );
#ifdef FCPP_DEBUG
   virtual std::string name() const {
        return std::string("Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result>(something)");
   }
#endif
};


#ifdef FCPP_POINTERCLASSEXPERIMENT

template <class Arg1, class Arg2, class Arg3, class Arg4, class Result,
          template <class> class Pointer_Class>
struct Fun4;

template <class A1d, class A2d, class A3d, class A4d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class Rs> 
Fun4Impl<A1d,A2d,A3d,A4d,Rd>* 
convert4(  const Fun4Impl<A1s,A2s,A3s,A4s,Rs>* f
	   //const IRef<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> >& f 
);

template <class A1d, class A2d, class A3d, class A4d, class Rd, class DF, 
           template <class> class Pointer_Class>
struct Fun4Constructor;

#else
template <class Arg1, class Arg2, class Arg3, class Arg4, class Result>
class Fun4Impl;
template <class Arg1, class Arg2, class Arg3, class Arg4, class Result>
struct Fun4;

template <class A1d, class A2d, class A3d, class A4d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class Rs> 
Fun4Impl<A1d,A2d,A3d,A4d,Rd>* 
convert4( const IRef<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> >& f
//const boost::intrusive_ptr<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> >& f 
       );

template <class A1d, class A2d, class A3d, class A4d, class Rd, class DF>
struct Fun4Constructor;

#endif

// Special function for the default constructor to call.
template <class Arg1, class Arg2, class Arg3, class Arg4, class Result> 
Result nullfun4(Arg1 a, Arg2 b, Arg3 c, Arg4 d) { return Result(); }

// The "Guts" class helps us implement currying; fun4 floats gently atop
// Fun4Guts and adds currying.  
#ifdef FCPP_POINTERCLASSEXPERIMENT
template <class Arg1, class Arg2, class Arg3, class Arg4, class Result,
          template <class> class Pointer_Class >
#else
template <class Arg1, class Arg2, class Arg3, class Arg4, class Result>
#endif
class Fun4Guts : public CFunType<Arg1,Arg2,Arg3,Arg4,Result> 
{
#ifdef FCPP_POINTERCLASSEXPERIMENT
  typedef typename Pointer_Class<const Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result> >::Pointer_Type RefImpl;
#else
  typedef IRef<const Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result> > RefImpl;
#endif
   // This is the actual pointer.
   RefImpl ref;

   // This does not seem necessary. Why does a class need to made a friend of itself?
   //template <class A, class B, class C, class D, class E> friend class Fun4Guts;

#ifdef FCPP_POINTERCLASSEXPERIMENT
   template <class A, class B, class C, class D, class E,
            template <class> class P> friend class Fun4;
   template <class A1d, class A2d, class A3d, class A4d, class Rd, class DF, 
             template <class> class P>
   friend struct Fun4Constructor;
#else
   template <class A, class B, class C, class D, class E> friend class Fun4;
   template <class A1d, class A2d, class A3d, class A4d, class Rd, class DF>
   friend struct Fun4Constructor;
#endif

#ifdef FCPP_POINTERCLASSEXPERIMENT
   template <class A1d, class A2d, class A3d, class A4d, class Rd, 
             class A1s, class A2s, class A3s, class A4s, class Rs, template <class> class P> 
   friend Fun4<A1d,A2d,A3d,A4d,Rd,P> 
   explicit_convert4( const Fun4<A1s,A2s,A3s,A4s,Rs,P>& f );
#else
   template <class A1d, class A2d, class A3d, class A4d, class Rd, 
             class A1s, class A2s, class A3s, class A4s, class Rs> 
   friend Fun4<A1d,A2d,A3d,A4d,Rd> 
   explicit_convert4( const Fun4<A1s,A2s,A3s,A4s,Rs>& f );
#endif
public:
#ifdef FCPP_POINTERCLASSEXPERIMENT
   typedef Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result>* Impl;
   Fun4Guts( int, Impl i ) : ref(Pointer_Class<const Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result> >::makeRefable(i)) {}
   // Note use of the unwrap member function to get back to the pointer needed for the operator().
   Result operator()( const Arg1& x, const Arg2& y, const Arg3& z, const Arg4& v) const 
         { return (*ref)->operator()(x,y,z,v); }
   //{ return ref->unwrap()->operator()(x,y,z,v); } // alternative

#else
   typedef Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result>* Impl;
   Fun4Guts( int, Impl i ) : ref(i) {}
   Result operator()( const Arg1& x, const Arg2& y, const Arg3& z, const Arg4& v) const 
      { return ref->operator()(x,y,z,v); }
#endif


#ifdef FCPP_POINTERCLASSEXPERIMENT
   template <class DF>
   Fun4Guts( const DF& df )
   {   
       Impl j = (Fun4Constructor<Arg1,Arg2,Arg3,Arg4,Result,DF,Pointer_Class>::make(df));
       ref =  Pointer_Class<const Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result> >::makeRefable(j);
   }                                                                      
#else
   template <class DF>
   Fun4Guts( const DF& df )
   : ref( Fun4Constructor<Arg1,Arg2,Arg3,Arg4,Result,DF>::make(df) ) { 
  }
#endif

   Fun4Guts( const Fun4Guts& x ) : ref(x.ref) {}
   Fun4Guts& operator=( const Fun4Guts& x ) { ref = x.ref; return *this; }
   Fun4Guts() { 
     Fun4Guts tmp = 
       ptr_to_fun( &nullfun4<Arg1,Arg2,Arg3,Arg4,Result>);
     ref = tmp.ref;
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Fun4Guts");
   }
#endif
};

//template <class A1d, class A2d, class A3d, class A4d, class Rd,
//          class A1s, class A2s, class A3s, class A4s, class Rs>
//Fun4Impl<A1d,A2d,A3d,A4d,Rd>*
//convert4( const IRef<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> >& f ) {
//   return new Fun4Converter<A1d,A2d,A3d,A4d,Rd,A1s,A2s,A3s,A4s,Rs>( f );
//}

//template <class A1d, class A2d, class A3d, class A4d, class Rd, 
//          class A1s, class A2s, class A3s, class A4s, class Rs> 
//class Fun4ExplicitConverter : public Fun4Impl<A1d,A2d,A3d,A4d,Rd> {
//   typedef IRef<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> > MyFun;
//   MyFun f;
//public:
//   Fun4ExplicitConverter( const MyFun& g ) : f(g) {}
//   Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& w ) const {
//      return static_cast<Rd>( f->operator()(  static_cast<A1s>(x),
//         static_cast<A2s>(y), static_cast<A3s>(z) , static_cast<A4s>(w)  ) );
//   }
//};

#ifdef FCPP_POINTERCLASSEXPERIMENT
// Here a default policy class is assigned. This is the ONLY place this is done.
template <class Arg1, class Arg2, class Arg3, class Arg4, class Result,
          template <class> class Pointer_Class = Another_IRef_Pointer_Class>
#else
template <class Arg1, class Arg2, class Arg3, class Arg4, class Result>
#endif
  class Fun4 : public CFunType<Arg1,Arg2,Arg3,Arg4,Result>, public SmartFunctoid4 {

#ifdef FCPP_POINTERCLASSEXPERIMENT
   template <class A1d, class A2d, class A3d, class A4d, class Rd, class DF, template <class> class P>
   friend struct Fun4Constructor;
#else
   template <class A1d, class A2d, class A3d, class A4d, class Rd, class DF>
   friend struct Fun4Constructor;
#endif

#ifdef FCPP_POINTERCLASSEXPERIMENT
   template <class A1d, class A2d, class A3d, class A4d, class Rd, 
             class A1s, class A2s, class A3s, class A4s, class Rs, template <class> class P> 
   friend Fun4<A1d,A2d,A3d,A4d,Rd,P> 
   explicit_convert4( const Fun4<A1s,A2s,A3s,A4s,Rs,P>& f );
#else
   template <class A1d, class A2d, class A3d, class A4d, class Rd, 
             class A1s, class A2s, class A3s, class A4s, class Rs> 
   friend Fun4<A1d,A2d,A3d,A4d,Rd> 
   explicit_convert4( const Fun4<A1s,A2s,A3s,A4s,Rs>& f );
#endif

#ifdef FCPP_POINTERCLASSEXPERIMENT
   Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result,Pointer_Class> rep;
#else
   Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result> rep;
#endif
public:
#ifdef FCPP_POINTERCLASSEXPERIMENT
   typedef Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result>* Impl;
#else
   typedef Fun4Impl<Arg1,Arg2,Arg3,Arg4,Result>* Impl;
#endif

   Fun4( int, Impl i ) : rep(1,i) {}

   template <class DF> 
   Fun4( const DF& df ) : rep(df) {}

   Fun4( const Fun4& x ) : rep(x.rep) {}
   Fun4& operator=( const Fun4& x ) { rep = x.rep; return *this; }
   Fun4() {
     //Actual implementation is in default constructor for Fun4Guts
     //Fun4 tmp = ptr_to_fun( &nullfun4<Arg1,Arg2,Arg3,Arg4,Result>);
     //rep = tmp.rep;
   }
    
#ifndef FCPP_CONCEPTS
#ifdef FCPP_POINTERCLASSEXPERIMENT
   typedef Full4<Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result,Pointer_Class> > SigHelp;
#else
   typedef Full4<Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result> > SigHelp;
#endif
   template <class A, class B=void, class C=void, class D=void>
   struct Sig : public SigHelp::template Sig<A,B,C,D> {};
#endif

   template <class A, class B, class C, class D>
#ifdef FCPP_CONCEPTS
   inline ConstCallable4<Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result>,A,B,C,D >::result_type
   operator()( const A& x, const B& y, const C& z, const D& v ) const 
      { 
#ifdef FCPP_PRINT_HELPERS
	std::cout << "ConstCallable4 used in Fun4" << std::endl;
#endif
 	// Use helper to cover the cases with autocurrying.
     return impl::Curryable4Helper<typename ConstCallable4<
       Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result>,A,B,C,D>::result_type,
       Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result>,A,B,C,D>::go(rep,x,y,z,v);
      }
#else 
   typename Sig<A,B,C,D>::ResultType
   operator()( const A& x, const B& y, const C& z, const D& v ) const 
	{ return makeFull4(rep)(x,y,z,v); } // 3 is now 4
#endif

   template <class A, class B, class C>
#ifdef FCPP_CONCEPTS
   inline ConstCallable4<Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result>,A,B,C >::result_type
   operator()( const A& x, const B& y, const C& z ) const 
      { 
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable4Helper2<typename ConstCallable4<
     Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result>,A,B,C>::result_type,
     Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result>,A,B,C>::go(rep,x,y,z);
     //return bind1and2of3(rep,x,y); 
      }
#else
   typename Sig<A,B,C>::ResultType
   operator()( const A& x, const B& y, const C& v ) const 
      { return ::fcpp::makeFull4(rep)(x,y,v); }
//      { return ::fcpp::curry4(rep,x,y,v); }
#endif

   template <class A, class B>
#ifdef FCPP_CONCEPTS
   inline ConstCallable4<Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result>,A,B >::result_type
   operator()( const A& x, const B& y ) const 
      { 
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable4Helper3<typename ConstCallable4<
     Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result>,A,B>::result_type,
     Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result>,A,B>::go(rep,x,y);
     //return bind1and2of3(rep,x,y); 
      }
#else
   typename Sig<A,B>::ResultType
   operator()( const A& x, const B& y ) const 
      { return ::fcpp::makeFull4(rep)(x,y); }
//      { return ::fcpp::curry4(rep,x,y); }
#endif

#ifdef FCPP_CONCEPTS
   // This is an important change as the result is a full functoid
   // not a function object.
   //inline ConstCallable4<Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result>,Arg1 >::result_type
   //operator()( const Arg1& x) const {
   // Changed to make argument a templated type.
   template <class A>
   inline ConstCallable4<Fun4Guts<Arg1,Arg2,Arg3,Arg4,Result>,A >::result_type
   operator()( const A& x) const {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Used modified operator() " << std::endl;
#endif
     return bind1of4(rep, x);
   }
#else
   template <class A>
   typename Sig<A>::ResultType
   operator()( const A& x ) const 
      { return bind1of4(rep,x); }
#endif

#ifdef FCPP_ENABLE_LAMBDA
   typedef Fun4 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
   // result_of
   //template <class F> struct result;
   //template <class Me, class X, class Y, class Z, class V>
   //struct result<Me(X,Y,Z,V)> :public Type<typename sig<X,Y,Z,V>::result_type> {};
   //template <class Me, class X, class Y, class Z>
   //struct result<Me(X,Y,Z)> :public Type<typename sig<X,Y,Z,void>::result_type> {};
   //template <class Me, class X, class Y>
   //struct result<Me(X,Y)> :public Type<typename sig<X,Y,void,void>::result_type> {};
   //template <class Me, class X>
   //struct result<Me(X)> :public Type<typename sig<X,void,void,void>::result_type> {};
#ifdef FCPP_DEBUG
   std::string name() const {
      return debug_string_and_name_or_string("Fun4::",*(rep.ref),"Fun4");
   }
#endif
};

//template <class T, class U, class V, class W, class X> 
//void intrusive_ptr_add_ref( const Fun4Impl<T,U,V,W,X>* p ) {
//   ++ (p->refC);
//}  
//template <class T, class U, class V, class W, class X> 
//void intrusive_ptr_release( const Fun4Impl<T,U,V,W,X>* p ) {
//   if( !--(p->refC) ) delete p;
//}  

#ifdef FCPP_POINTERCLASSEXPERIMENT
template <class A1d, class A2d, class A3d, class A4d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class Rs> 
class Fun4Converter : public Fun4Impl<A1d,A2d,A3d,A4d,Rd> {
   typedef const Fun4Impl<A1s,A2s,A3s,A4s,Rs>*  MyFun;
#else
template <class A1d, class A2d, class A3d, class A4d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class Rs> 
class Fun4Converter : public Fun4Impl<A1d,A2d,A3d,A4d,Rd> {
   typedef IRef<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> > MyFun;
#endif
   //   typedef boost::intrusive_ptr<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> > MyFun;
   MyFun f;
public:
   Fun4Converter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& v ) const {
      return f->operator()( x, y, z, v );
   }
};

#ifdef FCPP_POINTERCLASSEXPERIMENT
template <class A1d, class A2d, class A3d, class A4d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class Rs> 
Fun4Impl<A1d,A2d,A3d,A4d,Rd>* 
convert4(  const Fun4Impl<A1s,A2s,A3s,A4s,Rs>* f
   //const boost::intrusive_ptr<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> >& f 
         ) {
   return new Fun4Converter<A1d,A2d,A3d,A4d,Rd,A1s,A2s,A3s,A4s,Rs>( f );
}

template <class A1d, class A2d, class A3d, class A4d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class Rs> 
class Fun4ExplicitConverter : public Fun4Impl<A1d,A2d,A3d,A4d,Rd> {
   typedef const Fun4Impl<A1s,A2s,A3s,A4s,Rs> MyFun;
  //typedef boost::intrusive_ptr<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> > MyFun;
   MyFun f;
public:
   Fun4ExplicitConverter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& v ) const {
      return static_cast<Rd>( f->operator()(  static_cast<A1s>(x),
         static_cast<A2s>(y), static_cast<A3s>(z), static_cast<A4s>(v)  ) );
   }
};

template <class A1d, class A2d, class A3d, class A4d, class Rd, 
    class A1s, class A2s, class A3s, class A4s, class Rs, template <class> class P > 
Fun4<A1d,A2d,A3d,A4d,Rd,P> explicit_convert4( const Fun4<A1s,A2s,A3s,A4s,Rs,P>& f ) {
  // was explicit_convert3
   return Fun4<A1d,A2d,A3d,A4d,Rd,P>( 1, 
      new Fun4ExplicitConverter<A1d,A2d,A3d,A4d,Rd,A1s,A2s,A3s,A4s,Rs>(f.rep.ref) );
}

template <class Gen>
class Gen4 : public Fun4Impl<
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::first_argument_type,
   typename monomorphic_traits<Gen>::second_argument_type,
   typename monomorphic_traits<Gen>::third_argument_type,
   typename monomorphic_traits<Gen>::fourth_argument_type,
   typename monomorphic_traits<Gen>::result_type>
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg1Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg2Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg3Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg4Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::ResultType>
#endif
{
   Gen g;
public:
   Gen4( Gen x ) : g(x) {}
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::result_type operator()(
      const typename monomorphic_traits<Gen>::first_argument_type& x,
      const typename monomorphic_traits<Gen>::second_argument_type& y,
      const typename monomorphic_traits<Gen>::third_argument_type& z,
      const typename monomorphic_traits<Gen>::fourth_argument_type& v ) const
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::ResultType
     //typename monomorphic_traits<Gen>::result_type
   operator()(
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg1Type& x,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg2Type& y,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg3Type& z,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg4Type& v
 ) const
#endif
   {
      return g(x,y,z,v);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Gen4::",g,"Gen4");
   }
#endif
};


#ifndef FCPP_CONCEPTS
template <class Quaternary>
Fun4<typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg1Type, 
     typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg2Type,
     typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg3Type,
     typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg4Type,
     typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::ResultType> 
makeFun4( const Quaternary& g ) {
   return 
   Fun4<typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg1Type,
        typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg2Type,
        typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg3Type,
        typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg4Type,
        typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::ResultType> 
        (1,new Gen4<Quaternary>(g));
}
#endif

template <class Quaternary>
Gen4<Quaternary>* makeFun4Ref( const Quaternary& g ) {
   return new Gen4<Quaternary>(g);
}


template <class A1d, class A2d, class A3d, class A4d, class Rd, class DF, template <class> class Pointer_Class >
struct Fun4Constructor {
   static Fun4Impl<A1d,A2d,A3d,A4d,Rd>* 
   make( const DF& df ) {
      return  makeFun4Ref( ::fcpp::monomorphize4<A1d,A2d,A3d,A4d,Rd>(df));
   }
};

template <class A1d, class A2d, class A3d, class A4d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class Rs, template <class> class Pointer_Class > 
struct Fun4Constructor<A1d,A2d,A3d,A4d,Rd,Fun4<A1s,A2s,A3s,A4s,Rs,Pointer_Class>,Pointer_Class > {
   static Fun4Impl<A1d,A2d,A3d,A4d,Rd>* make( const Fun4<A1s,A2s,A3s,A4s,Rs,Pointer_Class>& f ) {
      return convert4<A1d,A2d,A3d,A4d,Rd>(f.rep.ref);
   }
};

#else // Alternative to FCPP_POINTERCLASSEXPERIMENT
// So there are two versions of large parts of Fun4.
template <class A1d, class A2d, class A3d, class A4d, class Rd,
          class A1s, class A2s, class A3s, class A4s, class Rs>
Fun4Impl<A1d,A2d,A3d,A4d,Rd>*
convert4(  const IRef<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> >& f
   //const boost::intrusive_ptr<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> >& f
         ) {
   return new Fun4Converter<A1d,A2d,A3d,A4d,Rd,A1s,A2s,A3s,A4s,Rs>( f );
}

template <class A1d, class A2d, class A3d, class A4d, class Rd,
          class A1s, class A2s, class A3s, class A4s, class Rs>
class Fun4ExplicitConverter : public Fun4Impl<A1d,A2d,A3d,A4d,Rd> {
   typedef IRef<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> > MyFun;
  //typedef boost::intrusive_ptr<const Fun4Impl<A1s,A2s,A3s,A4s,Rs> > MyFun;
   MyFun f;
public:
   Fun4ExplicitConverter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& v ) const {
      return static_cast<Rd>( f->operator()(  static_cast<A1s>(x),
         static_cast<A2s>(y), static_cast<A3s>(z), static_cast<A4s>(v)  ) );
   }
};


template <class A1d, class A2d, class A3d, class A4d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class Rs> 
Fun4<A1d,A2d,A3d,A4d,Rd> explicit_convert4( const Fun4<A1s,A2s,A3s,A4s,Rs>& f ) {
  // was explicit_convert3
   return Fun4<A1d,A2d,A3d,A4d,Rd>( 1, 
      new Fun4ExplicitConverter<A1d,A2d,A3d,A4d,Rd,A1s,A2s,A3s,A4s,Rs>(f.rep.ref) );
}


template <class Gen>
class Gen4 : public Fun4Impl<
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::first_argument_type,
   typename monomorphic_traits<Gen>::second_argument_type,
   typename monomorphic_traits<Gen>::third_argument_type,
   typename monomorphic_traits<Gen>::fourth_argument_type,
   typename monomorphic_traits<Gen>::result_type>
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg1Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg2Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg3Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg4Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::ResultType>
#endif
{
   Gen g;
public:
   Gen4( Gen x ) : g(x) {}
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::result_type operator()(
      const typename monomorphic_traits<Gen>::first_argument_type& x,
      const typename monomorphic_traits<Gen>::second_argument_type& y,
      const typename monomorphic_traits<Gen>::third_argument_type& z,
      const typename monomorphic_traits<Gen>::fourth_argument_type& v ) const
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::ResultType
     //typename monomorphic_traits<Gen>::result_type
   operator()(
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg1Type& x,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg2Type& y,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg3Type& z,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg4Type& v
 ) const
#endif
   {
      return g(x,y,z,v);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Gen4::",g,"Gen4");
   }
#endif
};


template <class Quaternary>
Fun4<typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg1Type, 
     typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg2Type,
     typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg3Type,
     typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg4Type,
     typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::ResultType> 
makeFun4( const Quaternary& g ) {
#ifdef FCPP_PRINT_HELPERS
	std::cout << "Used old makeFun4" << std::endl;
#endif
   return 
   Fun4<typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg1Type,
        typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg2Type,
        typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg3Type,
        typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::Arg4Type,
        typename Quaternary::template Sig<AnyType,AnyType,AnyType,AnyType>::ResultType> 
        (1,new Gen4<Quaternary>(g));
}



template <class Quaternary>
Gen4<Quaternary>* makeFun4Ref( const Quaternary& g ) {
   return new Gen4<Quaternary>(g);
}

template <class A1d, class A2d, class A3d, class A4d, class Rd, class DF>
struct Fun4Constructor {
   static Fun4Impl<A1d,A2d,A3d,A4d,Rd>* make( const DF& df ) {
     return makeFun4Ref( ::fcpp::monomorphize4<A1d,A2d,A3d,A4d,Rd>(df) );
     // return makeFun4Ref( df );
   }
};
template <class A1d, class A2d, class A3d, class A4d, class Rd,
          class A1s, class A2s, class A3s, class A4s, class Rs>
struct Fun4Constructor<A1d,A2d,A3d,A4d,Rd,Fun4<A1s,A2s,A3s,A4s,Rs> > {
   static Fun4Impl<A1d,A2d,A3d,A4d,Rd>* make( const Fun4<A1s,A2s,A3s,A4s,Rs>& f ) {
      return convert4<A1d,A2d,A3d,A4d,Rd>(f.rep.ref);
   }
};

#endif

//////////////////////////////////////////////////////////////////////
// Fun5 implementation (under development).
// The basic structures exist as enlargement of Fun4 
// without the pointer class experiment.
// The same then applies to Fun6 and Fun7 which have been constructed
// from the start with the option of use with concepts, which has been 
// fitted to the earlier code.
// Only some of the total range of currying has been done as yet.
// In general, the higher the number N is FunN the less complete it is.
// 
// A note on testing. The testing so far has been done mainly with arguments
// all of type int. This will not find cases where the argument types have
// been mixed up. The tests will find scrambling of the values.
// This needs to be remedied by more thorough testing with a mix of types.
//
// John Fletcher February 2008
//////////////////////////////////////////////////////////////////////

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result>
class Fun5Impl;

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result>
struct Fun5;

template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class A5s, class Rs> 
Fun5Impl<A1d,A2d,A3d,A4d,A5d,Rd>* 
convert5( const IRef<const Fun5Impl<A1s,A2s,A3s,A4s,A5s,Rs> >& f);

template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd, class DF>
struct Fun5Constructor;

// Special function for the default constructor to call.
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5,class Result> 
Result nullfun5(Arg1 a, Arg2 b, Arg3 c, Arg4 d, Arg5 e) { return Result(); }


// The "Guts" class helps us implement currying; Fun5 floats gently atop
// Fun5Guts and adds currying.  
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result>
class Fun5Guts : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Result> {
   typedef IRef<const Fun5Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Result> > RefImpl;
   RefImpl ref;
   //template <class A, class B, class C, class D, class E, class F> friend class Fun5Guts;
   template <class A, class B, class C, class D, class E, class F> friend class Fun5;
   
   template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd, class DF>
   friend struct Fun5Constructor;

   template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd, 
             class A1s, class A2s, class A3s, class A4s, class A5s, class Rs> 
   friend Fun5<A1d,A2d,A3d,A4d,A5d,Rd> 
   explicit_convert5( const Fun5<A1s,A2s,A3s,A4s,A5s,Rs>& f );
public:
   typedef Fun5Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Result>* Impl;

   Fun5Guts( int, Impl i ) : ref(i) {}
   Result operator()( const Arg1& x, const Arg2& y, const Arg3& z, const Arg4& v, const Arg5& w) const
      { return ref->operator()(x,y,z,v,w); }

   template <class DF>
   Fun5Guts( const DF& df )
   : ref( Fun5Constructor<Arg1,Arg2,Arg3,Arg4,Arg5,Result,DF>::make(df) ) {}

   Fun5Guts( const Fun5Guts& x ) : ref(x.ref) {}
   Fun5Guts& operator=( const Fun5Guts& x ) { ref = x.ref; return *this; }
   Fun5Guts() { 
     Fun5Guts tmp = 
       ptr_to_fun( &nullfun5<Arg1,Arg2,Arg3,Arg4,Arg5,Result>);
     ref = tmp.ref;
   }
};


template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result>
  class Fun5 :  public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Result>, public SmartFunctoid5 {
   template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd, class DF>
   friend struct Fun5Constructor;

   template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd, 
             class A1s, class A2s, class A3s, class A4s, class A5s, class Rs> 
   friend Fun5<A1d,A2d,A3d,A4d,A5d,Rd> 
   explicit_convert5( const Fun5<A1s,A2s,A3s,A4s,A5s,Rs>& f );

   Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result> rep;
public:
   typedef Fun5Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Result>* Impl;

   Fun5() {} // Lars Schouw added 2005.01.31
   Fun5( int, Impl i ) : rep(1,i) {}

   template <class DF> 
   Fun5( const DF& df ) : rep(df) {}

   Fun5( const Fun5& x ) : rep(x.rep) {}
   Fun5& operator=( const Fun5& x ) { rep = x.rep; return *this; }

#ifndef FCPP_CONCEPTS
   typedef Full5<Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result> > SigHelp;
   template <class A, class B=void, class C=void, class D=void, class E=void>
   struct Sig : public SigHelp::template Sig<A,B,C,D,E> {};
#endif
 
   template <class A, class B, class C, class D, class E>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B,C,D,E >::result_type
     operator()( const A& x, const B& y, const C& z, const D& v, const E& w ) const 
      { 
#ifdef FCPP_PRINT_HELPERS5
	std::cout << "ConstCallable5 used in Fun5" << std::endl;
#endif
 	// Use helper to cover the cases with autocurrying.
     return impl::Curryable5Helper<typename ConstCallable5<
       Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B,C,D,E>::result_type,
       Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B,C,D,E>::go(rep,x,y,z,v,w);
      }
#else 
   typename Sig<A,B,C,D,E>::ResultType
   operator()( const A& x, const B& y, const C& z, const D& v, const E& w ) const 
	{ return makeFull5(rep)(x,y,z,v,w); } 
#endif

   // These calls to curry5 are needed by lambda operations.
   template <class A, class B, class C, class D>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B,C,D >::result_type
   operator()( const A& x, const B& y, const C& z, const D& v ) const 
      { 
#ifdef FCPP_PRINT_HELPERS5
	std::cout << "ConstCallable5 used in Fun5" << std::endl;
#endif
 	// Use helper to cover the cases with autocurrying.
     return impl::Curryable5Helper2<typename ConstCallable5<
       Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B,C,D>::result_type,
       Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B,C,D>::go(rep,x,y,z,v);
      }
#else 
   typename Sig<A,B,C,D>::ResultType
   operator()( const A& x, const B& y, const C& v, const D& w ) const 
     {  
#ifdef FCPP_PRINT_HELPERS5
       std::cout << "calling curry5(rep,x,y,v,w)" << std::endl;
#endif
       return ::fcpp::makeFull5(rep)(x,y,v,w);
        //return ::fcpp::curry5(rep,x,y,v,w); 
     }
#endif

   template <class A, class B, class C>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B,C >::result_type
   operator()( const A& x, const B& y, const C& z ) const 
      { 
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable5Helper3<typename ConstCallable5<
     Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B,C>::result_type,
     Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B,C>::go(rep,x,y,z);
     //return bind1and2of3(rep,x,y); 
      }
#else
   typename Sig<A,B,C>::ResultType
   operator()( const A& x, const B& y, const C& v ) const 
     { 
#ifdef FCPP_PRINT_HELPERS5
       std::cout << "calling makeFull5(rep)(x,y,v)" << std::endl;
#endif
       return ::fcpp::makeFull5(rep)(x,y,v);
        //return ::fcpp::curry5(rep,x,y,v); 
     }
#endif

   template <class A, class B>
#ifdef FCPP_CONCEPTS
   inline ConstCallable5<Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B >::result_type
   operator()( const A& x, const B& y ) const 
      { 
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable5Helper4<typename ConstCallable5<
       Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B>::result_type,
       Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A,B>::go(rep,x,y);
     //return bind1and2of3(rep,x,y); 
      }
#else
   typename Sig<A,B>::ResultType
   operator()( const A& x, const B& y ) const 
     { 
#ifdef FCPP_PRINT_HELPERS5
        std::cout << "calling makeFull5(rep)(x,y)" << std::endl;
#endif
        return ::fcpp::makeFull5(rep)(x,y);
        //return ::fcpp::curry5(rep,x,y); 
     }
#endif

#ifdef FCPP_CONCEPTS
   // This is an important change as the result is a full functoid
   // not a function object.
   //inline ConstCallable5<Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,Arg1 >::result_type
   //operator()( const Arg1& x) const {
   // Changed to make argument a templated type.
   template <class A>
   inline ConstCallable5<Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,A >::result_type
   operator()( const A& x) const 
     {
#ifdef FCPP_PRINT_HELPERS5
       std::cout << "Used modified operator() " << std::endl;
#endif
       return bind1of5(rep, x);
     }
#else
   template <class A>
   typename Sig<A>::ResultType
   operator()( const A& x ) const 
     {
       // std::cout << "calling curry5(rep,x)" << std::endl;
       //return curry5(rep,x); 
       return bind1of5(rep, x);
     }
#endif
   
#ifdef FCPP_ENABLE_LAMBDA
   typedef Fun5 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Fun5::",*(rep.ref),"Fun5");
   }
#endif
};

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result>
class Fun5Impl : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Result> , public IRefable {
  //   mutable RefCountType refC;
public:
   Fun5Impl() {}

   virtual Result operator()( const Arg1&, const Arg2&, const Arg3&, const Arg4&, const Arg5&) const =0;
   virtual ~Fun5Impl() {}
#ifdef FCPP_DEBUG
   virtual std::string name() const {
        return std::string("Fun5Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Result>(something)");
   }
#endif
};

template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd,
          class A1s, class A2s, class A3s, class A4s, class A5s, class Rs>
class Fun5Converter : public Fun5Impl<A1d,A2d,A3d,A4d,A5d,Rd> {
   typedef IRef<const Fun5Impl<A1s,A2s,A3s,A4s,A5s,Rs> > MyFun;
   MyFun f;
public:
   Fun5Converter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& v, const A5d& w ) const {
      return f->operator()( x, y, z, v, w );
   }
};

template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd,
          class A1s, class A2s, class A3s, class A4s, class A5s, class Rs>
Fun5Impl<A1d,A2d,A3d,A4d,A5d,Rd>*
convert5(  const IRef<const Fun5Impl<A1s,A2s,A3s,A4s,A5s,Rs> >& f
         ) {
   return new Fun5Converter<A1d,A2d,A3d,A4d,A5d,Rd,A1s,A2s,A3s,A4s,A5s,Rs>( f );
}

template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd,
          class A1s, class A2s, class A3s, class A4s, class A5s, class Rs>
class Fun5ExplicitConverter : public Fun5Impl<A1d,A2d,A3d,A4d,A5d,Rd> {
   typedef IRef<const Fun5Impl<A1s,A2s,A3s,A4s,A5s,Rs> > MyFun;
   MyFun f;
public:
   Fun5ExplicitConverter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& v , const A5d& w) const {
      return static_cast<Rd>( f->operator()(  static_cast<A1s>(x),
         static_cast<A2s>(y), static_cast<A3s>(z), static_cast<A4s>(v), static_cast<A5s>(w)  ) );
   }
};

template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class A5s, class Rs> 
Fun5<A1d,A2d,A3d,A4d,A5d,Rd> explicit_convert5( const Fun5<A1s,A2s,A3s,A4s,A5s,Rs>& f ) {
   return Fun5<A1d,A2d,A3d,A4d,A5d,Rd>( 1, 
      new Fun5ExplicitConverter<A1d,A2d,A3d,A4d,A5d,Rd,A1s,A2s,A3s,A4s,A5s,Rs>(f.rep.ref) );
}

template <class Gen>
class Gen5 : public Fun5Impl<
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::first_argument_type,
   typename monomorphic_traits<Gen>::second_argument_type,
   typename monomorphic_traits<Gen>::third_argument_type,
   typename monomorphic_traits<Gen>::fourth_argument_type,
   typename monomorphic_traits<Gen>::fifth_argument_type,
   typename monomorphic_traits<Gen>::result_type>
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type, 
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType> 
#endif
{
   Gen g;
public:
   Gen5( Gen x ) : g(x) {}
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::result_type operator()( 
      const typename monomorphic_traits<Gen>::first_argument_type&  x,
      const typename monomorphic_traits<Gen>::second_argument_type& y,
      const typename monomorphic_traits<Gen>::third_argument_type&  z,
      const typename monomorphic_traits<Gen>::fourth_argument_type& v,
      const typename monomorphic_traits<Gen>::fifth_argument_type&  w ) const
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType
   operator()( 
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type& x,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type& y,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type& z, 
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type& v, 
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type& w 
 ) const
#endif
   {
      return g(x,y,z,v,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
      return debug_string_and_name_or_string("Gen5::",g,"Gen5");
   }
#endif
};

#ifndef FCPP_CONCEPTS
template <class Quintinary>
Fun5<typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type, 
     typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
     typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
     typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
     typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
     typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType> 
makeFun5( const Quintinary& g ) {
#ifdef FCPP_PRINT_HELPERS5
	std::cout << "Used old makeFun5" << std::endl;
#endif
   return 
   Fun5<typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type,
        typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
        typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
        typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
        typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
        typename Quintinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType> 
        (1,new Gen5<Quintinary>(g));
}
#endif

template <class Quintinary>
Gen5<Quintinary>* makeFun5Ref( const Quintinary& g ) {
   return new Gen5<Quintinary>(g);
}

template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd, class DF>
struct Fun5Constructor {
   static Fun5Impl<A1d,A2d,A3d,A4d,A5d,Rd>* make( const DF& df ) {
      return makeFun5Ref( ::fcpp::monomorphize5<A1d,A2d,A3d,A4d,A5d,Rd>(df) );
   }
};

template <class A1d, class A2d, class A3d, class A4d, class A5d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class A5s, class Rs> 
struct Fun5Constructor<A1d,A2d,A3d,A4d,A5d,Rd,Fun5<A1s,A2s,A3s,A4s,A5s,Rs> > {
   static Fun5Impl<A1d,A2d,A3d,A4d,A5d,Rd>* make( const Fun5<A1s,A2s,A3s,A4s,A5s,Rs>& f ) {
      return convert5<A1d,A2d,A3d,A4d,A5d,Rd>(f.rep.ref);
   }
};

//////////////////////////////////////////////////////////////////////
// Fun6 implementation (under development).
//////////////////////////////////////////////////////////////////////

//#include "function6.h"

//////////////////////////////////////////////////////////////////////
// Fun7 implementation (under development).
//////////////////////////////////////////////////////////////////////

//#include "function7.h"

//////////////////////////////////////////////////////////////////////
// Fun8 implementation (under development).
//////////////////////////////////////////////////////////////////////

//#include "function8.h"

//////////////////////////////////////////////////////////////////////
// Space for the concept maps for Fun6
// This is an attempt to solve the binder resolution problem.
/*
 template <class U,class V,class W, class X, class Y, class Z, class R>
   concept_map ConstCallable6<Fun6<U,V,W,X,Y,Z,R>,U,V,W,X,Y,Z>
{
  typedef Full0<impl::binder1and2and3and4and5and6of6<Fun6<U,V,W,X,Y,Z,R>,U,V,W,X,Y,Z> > result_type;
};
*/

//////////////////////////////////////////////////////////////////////

#endif
 /* This and others defined in seq.h.
   namespace traits {
     // argument traits now defined outside FCPP_DEBUG
       template<class R>
       class argument_traits<Fun0<R> >
       {
       public:
	 //static const bool has_name = true;
	static std::string value(const Fun0<R> &f)
	{ std::string null; return null;}
       };
   }
 */

#ifndef FCPP_NO_USE_NAMESPACE
} // end namespace fcpp
#endif

// Includes argument traits needed without FCPP_DEBUG
#include "function_debug.h"

#endif
