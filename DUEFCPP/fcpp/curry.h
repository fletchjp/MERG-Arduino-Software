//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_DOT_H
#define FCPP_CURRY_DOT_H

#include <cassert>

#include "signature.h"

#ifdef FCPP_DEBUG
//#include "debug_traits.h"
#include "debug_helper.h"
#endif

namespace fcpp {

//////////////////////////////////////////////////////////////////////
// This file implements currying for functoids.  Included here are
//  - bindMofN    for currying the Mth of N arguments
//  - Const       for turning a value into a constant function
//  - Konst       for turning a value into a unary constant function
//  - curryN      curries the first k arguments of an N-arg functoid,
//                where k is the number of arguments "curryN" was
//                called with
//  - CurryableN  new way to curry with underscores (e.g. f(_,y,_); )
//
// For more info, see
//    http://www.cc.gatech.edu/~yannis/fc++/currying.html
// which is now somewhat out-of-date, and
//    http://www.cc.gatech.edu/~yannis/fc++/New1.5/full.html
// which helps bring you up to date.
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// November 2016
//
// I have done a refactor and expansion of the binder codes.
// This has extended the debug facilities and put them into a standard
// form. Examples for this are in fcpptest8.cpp
// I have found some bugs in the previous code for binderAof5, binderXof5
// and all the later codes which have developed from that.
// These concerned the template types of the arguments and have now been
// corrected. I have also shortened the long names of some binders.
// e.g. binderXandYandZof7 has become binderXtoZof7.
// All names of helper functions have also been changed.
// Code also has to be changed in other files e.g. fullN.h files.
// Examples have been changed where necessary.
// This makes the code much more readable.
//
// John Fletcher
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// FCPP_CONCEPTS
// All the binders up to and including N=4 have been modified
// to use concepts instead of the Sig when FCPP_CONCEPTS is defined.
// I have also completed N=5 and have now solved the problem of dealing
// with bindAof5, bindXof5 and bindXandYof5.
// There are also added bindXtoZof5 and bindWtoZof5.
// Note that this needs ConstCallable in sizes up to 7.
// concept maps are located at about line 1630 in this file.
// I have also made a start on N=6
// John Fletcher January 2008 (updated November 2016)
// See note at the end of this file about CurryN.
//
// I have just realised that this file is now HUGE.
// 235,000 bytes and ~6500 lines.
// It needs to be divided up.
// I have made a backup called curry_undivided.h
// and this file curry_divided.h, which will be divided and then replace
// curry.h
// 
// Ideas
//
// Keep the external name curry.h so as not to disrupt anything else.
// Keep all of the orginal FC++ 1.5 in the main file (n = 1 to 3),
// plus the n = 4 which are similar.
// Put all the extra binders (N>4) in a separate files.      curry_binders5.h
//                                                           curry_binders6.h
//                                                           curry_binders7.h
//                                                           curry_binders8.h
// Put all the concept maps in a separate file.          curry_concept_maps.h
// Put all the extra helpers (N>4) in another separate file. curry_helpers5.h
//                                                           curry_helpers6.h
//                                                           curry_helpers7.h
//                                                           curry_helpers8.h
// Put all the debug traits in another separate file.        curry_debug.h
//
// The above has now been carried out and all files are of a reasonable size.
// It also makes it easier to work as several can be open at once for
// ease of transfer and cross reference.
//////////////////////////////////////////////////////////////////////

// Important to implementation of CurryableN classes
struct AutoCurryType {
#if (defined (FCPP_CONCEPTS_LITE) || defined(FCPP_TICK) )
  void auto_curry() const {};  // For detection purposes
  typedef AutoCurryType ElementType; // Dummy to fool TICK.
#endif
};
FCPP_MAYBE_EXTERN AutoCurryType _;   // this is a legal identifier as fcpp::_

// Forward declarations; curryability and fullness are now somewhat
// inextricably intertwined...
template <class F> struct Full0;
template <class F> struct Full1;
template <class F> struct Full2;
template <class F> struct Full3;
// FC++.1.5.2
#ifdef FCPP152
template <class F> struct Full4;
template <class F> struct Full5;
template <class F> struct Full6;
template <class F> struct Full7;
template <class F> struct Full8;
#endif
template <class F> Full0<F> makeFull0( const F& f );
template <class F> Full1<F> makeFull1( const F& f );
template <class F> Full2<F> makeFull2( const F& f );
template <class F> Full3<F> makeFull3( const F& f );
// FC++.1.5.2
#ifdef FCPP152
template <class F> Full4<F> makeFull4( const F& f );
template <class F> Full5<F> makeFull5( const F& f );
template <class F> Full6<F> makeFull6( const F& f );
template <class F> Full7<F> makeFull7( const F& f );
template <class F> Full8<F> makeFull8( const F& f );
#endif


namespace impl {

//////////////////////////////////////////////////////////////////////
// Const and Konst
//////////////////////////////////////////////////////////////////////
// Check out these unconditional sigs.

template <class T>
struct ConstHelper : public CFunType<T> {
#ifdef FCPP_DEBUG
     std::string name() const
    {
       return std::string("ConstHelper( ")
        + debug_name_or_value(x)
        + std::string(" )");
    }
#endif
  const T x;
public:
   ConstHelper( const T& a ) : x(a) {}
   T operator()() const { return x; }
};
struct Const {
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Const");
    }
#endif
   template <class T>
   struct Sig : public FunType<T,Full0<ConstHelper<T> > > {};

   template <class T>
   Full0<ConstHelper<T> > operator()( const T& x ) const {
      return makeFull0( ConstHelper<T>(x) );
   }
};


// FC++.1.5.2
// #ifdef FCPP152
// WHY is this 1.5.2??? Did I add it?
// It is not in 1.5.1.
// Did it come from the boost version? YES
// The next line was here before.
// This needs careful testing.

// It is so common to say ignore(const_(x)) that we make a composite
// function out of it, and call it konst.  That is
//    konst(x)(y) = x
template <class X>
struct XkonstHelp {
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("KonstHelp( ")
        + debug_name_or_value(x)
	+ std::string(" )");
    }
#endif
   X x;
   XkonstHelp( const X& xx ) : x(xx) {}
  template <class A> struct Sig : public FunType<A,X> {};
   template <class A>
   X operator()( const A& ) const {
      return x;
   }
};
struct Konst {
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Konst");
    }
#endif
  // X, needed here in FC++!
  // In the boost version the arguments are not needed.
  // This code was copied from there and never tested until now.
  template <class X> struct Sig : public FunType<X,
      Full1<XkonstHelp<X> > > {};
   template <class X>
   typename Sig<X>::ResultType operator()( const X& x ) const {
      return makeFull1( XkonstHelp<X>( x ) );
   }
};
//#endif

//////////////////////////////////////////////////////////////////////
// Binders (through "...of2")
// These have been changed to implement concepts.
//////////////////////////////////////////////////////////////////////

template <class Unary, class Arg>
class binder1of1 
: public CFunType<typename RT<Unary,Arg>::ResultType> {
   const Unary f;
   const Arg a;
public:
   binder1of1( const Unary& x, const Arg& y ) : f(x), a(y) {}
#ifdef FCPP_CONCEPTS
   inline ConstCallable1<Unary,Arg>::result_type
#else
   typename RT<Unary,Arg>::ResultType
#endif
   operator()() const {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "binder1of1 called" << std::endl;
#endif
   return f(a);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if (debug_traits_has_name<Unary>()) {
	 temp += std::string("binder1of1::")
	      + debug_traits_name(f);
	 if (debug_traits_has_name<Arg>()) {
	   temp += std::string("::")
       	        +  debug_traits_name(a);
         } else {
  	   temp += std::string("( ");
	   temp += argument_traits_value(a);
           temp += std::string(" )");
         }
         return temp;
     } else
         return std::string("binder1of1");
     }
#endif
};

struct Bind1of1 {
#ifndef FCPP_CONCEPTS
   template <class Unary, class Arg>
   struct Sig : public FunType< Unary, Arg, Full0<binder1of1<Unary,Arg> > > {};
#endif
   
   template <class Unary, class Arg>
   inline Full0<binder1of1<Unary,Arg> > 
   operator()( const Unary& f, const Arg& a ) const {
      return makeFull0( binder1of1<Unary,Arg>(f,a) );
   }
};

#ifdef FCPP_EXTRA_STRONG_CURRY
// This is the normal call - not usually needed like this.
template <class R, class F, class X>
struct Curryable1Helper0 {
   static inline R go( const F& f, const X& x) {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Curryable1Helper0 calls f(x)" << std::endl;
#endif
     return f(x);
   }
};

// Code for autocurry f(_) -> f
template <class R, class F>
struct Curryable1Helper0<R,F,AutoCurryType> {
   static inline R go( const F& f, const AutoCurryType&) {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Curryable1Helper0 returns f STRONG " << std::endl;
#endif
     return f;
     //return makeFull1( f );
   }
};
#endif


template <class Binary, class Arg1>
class binder1of2 {
   const Binary f;
   const Arg1 x;
public:
   binder1of2( const Binary& a, const Arg1& b ) : f(a), x(b) {}

     /* See Notes 17 and 18 at the start of full.h */
#ifndef FCPP_CONCEPTS
   template <class Arg2>
   struct Sig
   : public FunType<typename Binary::template Sig<Arg1,Arg2>::Arg2Type,
                    typename Binary::template Sig<Arg1,Arg2>::ResultType> {};
#endif

   template <class Arg2>
#ifdef FCPP_CONCEPTS
   inline ConstCallable2<Binary,Arg1,Arg2>::result_type
#else
   typename Binary::template Sig<Arg1,Arg2>::ResultType
#endif
   operator()( const Arg2& y ) const {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     std::cout << "binder1of2 called with " //<< f
       // Actual parameters supressed to avoid crash
       // - now using traits as a trial
       << "("
       << debug_name_or_value(x) << ","
       << debug_name_or_value(y) << ")"
       //  << "(x,y)"
       << std::endl;
#else
     std::cout << "binder1of2(x,y) called" << std::endl;
#endif
#endif
     return f(x,y);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Binary>() ) {
	 temp += std::string("binder1of2::")
	      + debug_traits_name(f);
	 if ( debug_traits_has_name<Arg1>() ) {
	 temp += std::string("::")
       	      +  debug_traits_name(x);
       } else {
  	 temp += std::string("( ");
	 temp += argument_traits_value(x);
         temp += std::string(" , _ )");
       }
       return temp;
     } else
         return std::string("binder1of2");
     }
#endif
};

struct Bind1of2 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("Bind1of2");
  }
#endif

#ifndef FCPP_CONCEPTS
   template <class Binary, class Arg1>
   struct Sig : public FunType<Binary,Arg1,Full1<binder1of2<Binary,Arg1> > > {};
#endif

   template <class Binary, class Arg1>
   inline Full1<binder1of2<Binary,Arg1> >
   operator()( const Binary& f, const Arg1& x ) const {
      return makeFull1( binder1of2<Binary,Arg1>(f,x) );
   }
};

template <class Binary, class Arg2>
class binder2of2 {
   const Binary f;
   const Arg2 y;
public:
 binder2of2( const Binary& a, const Arg2& b ) : f(a), y(b) {}

     /* See Notes 17 and 18 at the start of full.h */
#ifndef FCPP_CONCEPTS
   template <class Arg1>
   struct Sig
   : public FunType<typename Binary::template Sig<Arg1,Arg2>::Arg1Type,
                    typename Binary::template Sig<Arg1,Arg2>::ResultType> {};
#endif

   template <class Arg1>
#ifdef FCPP_CONCEPTS
   inline ConstCallable2<Binary,Arg1,Arg2>::result_type
#else
   typename Binary::template Sig<Arg1,Arg2>::ResultType
#endif
   operator()( const Arg1& x ) const {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     std::cout << "binder2of2 called with " //<< f
	       << "(" << debug_name_or_value(x)
               << "," << debug_name_or_value(y) << ")" << std::endl;
#else
     std::cout << "binder2of2 called " << std::endl;
#endif
#endif
      return f(x,y);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
      std::string temp;
      if ( debug_traits_has_name<Binary>() ) {
	temp += std::string("binder2of2::")
             + debug_traits_name(f);
	if ( debug_traits_has_name<Arg2>() ) {
          temp += std::string("::")
               + debug_traits_name(y);
       } else {
   	  temp += std::string("( _ , ");
	  temp += argument_traits_value(y);
          temp += std::string(" )");
       }
       return temp;
      } else
        return std::string("binder2of2");
   }
#endif
};

struct Bind2of2 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("Bind2of2");
  }
#endif
#ifndef FCPP_CONCEPTS
   template <class Binary, class Arg2>
   struct Sig : public FunType<Binary,Arg2,Full1<binder2of2<Binary,Arg2> > > {};
#endif

   template <class Binary, class Arg2>
   inline Full1<binder2of2<Binary,Arg2> >
   operator()( const Binary& f, const Arg2& y ) const {
      return makeFull1( binder2of2<Binary,Arg2>(f,y) );
   }
};

template <class Binary, class Arg1, class Arg2>
class binder1and2of2
#ifndef FCPP_CONCEPTS
: public CFunType<typename RT<Binary,Arg1,Arg2>::ResultType >
#endif
{
   const Binary f;
   const Arg1 a1;
   const Arg2 a2;
public:
   binder1and2of2( const Binary& x, const Arg1& y, const Arg2& z )
   : f(x), a1(y), a2(z) {}
#ifdef FCPP_CONCEPTS
   inline ConstCallable2<Binary,Arg1,Arg2>::result_type
#else
   inline typename RT<Binary,Arg1,Arg2>::ResultType
#endif
   operator()() const { return f(a1,a2); }

#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   // This is now extended to do the second name as well.
   // For two names the form ::(name1,name2) is generated.
   // Attempts to output the values as well now restored.
   // I have built argument_traits to avoid problems
   // when there is a name.
   std::string name() const {
      std::string temp;
      if ( debug_traits_has_name<Binary>() ) {
	temp +=  std::string("binder1and2of2::")
	     + debug_traits_name(f);
	if ( debug_traits_has_name<Arg1>() ) {
	  if ( debug_traits_has_name<Arg2>() ) {
       	     temp += std::string("(")
                  + debug_traits_name(a1)
	          + std::string(",")
       		  + debug_traits_name(a2)
	          + std::string(")");
          } else {
       	    temp += debug_traits_name(a1);
   	    temp += std::string("( ");
	    temp += argument_traits_value(a2);
            temp += std::string(" , ... )");
          }
       } else {
    	    temp += std::string("( ");
	    temp += argument_traits_value(a1);
    	    temp += std::string(" , ");
	    temp += argument_traits_value(a2);
            temp += std::string(" )");
      }
       return temp;
    } else
        return std::string("binder1and2of2");
   }
#endif

};

struct Bind1and2of2 {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1and2of2");
   }
 #endif
 #ifndef FCPP_CONCEPTS
   template <class Binary, class Arg1, class Arg2>
   struct Sig
   : public FunType< Binary, Arg1, Arg2,
      Full0<binder1and2of2<Binary,Arg1,Arg2> > > {};
#endif

   template <class Binary, class Arg1, class Arg2>
   inline Full0<binder1and2of2<Binary,Arg1,Arg2> >
   operator()( const Binary& f, const Arg1& a1, const Arg2& a2 ) const {
      return makeFull0( binder1and2of2<Binary,Arg1,Arg2>(f,a1,a2) );
   }
};


//#ifdef FCPP_EXTRA_STRONG_CURRY

// This code defines cycle2 as an FC++ functoid
// such that cycle2(f)(a,b) -> f(b,a), for example
// cycle2(fcpp::minus)(1,2) = 1
// This is used in strong currying so that 
// minus(_)(1,2) = 1 

// Notes: (1) Now available with concepts.
//        (2) It could be made available generally.
//        (3) Needs to be extended to cycle3 etc.
//        (4) This is needed because of comma operator problems
//            in lambda.
//        (5) Some lambda cases fail with concepts.

template <class Binary>
class cycler2 {
   const Binary f;
 
public:
 cycler2(const Binary &g) : f(g) { }

#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg2>
   struct Sig
   : public FunType<typename Binary::template Sig<Arg2,Arg1>::Arg1Type,
                    typename Binary::template Sig<Arg2,Arg1>::Arg2Type,
                    typename Binary::template Sig<Arg2,Arg1>::ResultType> {};
#endif

   template < class Arg1, class Arg2>
#ifdef FCPP_CONCEPTS
   inline ConstCallable2<Binary,Arg2,Arg1>::result_type
#else
   typename Binary::template Sig<Arg2,Arg1>::ResultType
#endif
   operator()( const Arg1& x, const Arg2& y ) const {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     std::cout << "cycler2 called with " //<< f
               << "(" << debug_name_or_value(x)
               << "," << debug_name_or_value(y) << ")" << std::endl;
#else
     std::cout << "cycler2 called " << std::endl;
#endif
#endif
     return f(y,x); // Note arguments are reversed.
   } 
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return debug_string_and_name_or_string("cycler2::",f,"cycler2");
   }
#endif
};

struct Cycle2 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Cycle2");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class F>
     struct Sig : public FunType<F,Full2<cycler2<F> > >{};
#endif

    template <class F>
    inline Full2<cycler2<F> >
    operator()(const F& f) const {
       return makeFull2(cycler2<F>(f));
    }

 };

//#endif

//////////////////////////////////////////////////////////////////////
// Now that bindNof2 are defined, we can define Curryable2, which then
// some of the later binders can use.
//////////////////////////////////////////////////////////////////////

#ifdef FCPP_EXTRA_STRONG_CURRY
// This is the normal call - not usually needed like this.
template <class R, class F, class X>
struct Curryable2Helper0 {
   static inline R go( const F& f, const X& x) {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     std::cout << "Curryable2helper0 calls binder1of2("
               << debug_name_or_string(f,"f") << ","
               << debug_name_or_value(x) << ")" << std::endl;
#else
     std::cout << "Curryable2helper0 calls binder1of2(f,x)" << std::endl;
#endif
#endif
     return makeFull1( binder1of2<F,X>(f,x) );
   }
};

template <class R, class F>
struct Curryable2Helper0<R,F,AutoCurryType> {
   static inline R go( const F& f, const AutoCurryType&) {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     std::cout << "Curryable2Helper0 returns cycler2("
	       << debug_name_or_string(f,"f") << ") STRONG" << std::endl;
#else
     std::cout << "Curryable2helper returns cycler2(f) STRONG" << std::endl;
#endif
#endif
     //return f;
     return makeFull2( cycler2<F>(f) );
   }
};
#endif


template <class R, class F, class X, class Y>
struct Curryable2Helper {
   static inline R go( const F& f, const X& x, const Y& y ) {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     std::cout << "Curryable2helper calls "
               << debug_name_or_string(f,"f") << "("
               << debug_name_or_value(x) << ","
               << debug_name_or_value(y)  << ")" << std::endl;
#else
     std::cout << "Curryable2helper calls f(x,y)" << std::endl;
#endif
#endif
     return f(x,y);
   }
};

template <class R, class F, class Y>
struct Curryable2Helper<R,F,AutoCurryType,Y> {
   static inline R go( const F& f, const AutoCurryType& , const Y& y ) {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     std::cout << "Curryable2helper calls binder2of2("
               << debug_name_or_string(f,"f") << ","
               << debug_name_or_value(y) << ")" << std::endl;
#else
     std::cout << "Curryable2helper calls binder2of2(f,y)" << std::endl;
#endif
#endif
      return makeFull1( binder2of2<F,Y>(f,y) );
   }
};

template <class R, class F, class X>
struct Curryable2Helper<R,F,X,AutoCurryType> {
   static inline R go( const F& f, const X& x, const AutoCurryType& ) {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     std::cout << "Curryable2helper calls binder1of2("
               << debug_name_or_string(f,"f") << ","
               << debug_name_or_value(x) << ")" << std::endl;
#else
     std::cout << "Curryable2helper calls binder1of2(f,x)" << std::endl;
#endif
#endif
      return makeFull1( binder1of2<F,X>(f,x) );
   }
};

#ifdef FCPP_EXTRA_STRONG_CURRY
template <class R, class F>
struct Curryable2Helper<R,F,AutoCurryType,AutoCurryType> {
   static inline R go( const F& f, const AutoCurryType , const AutoCurryType& ) {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     std::cout << "Curryable2Helper returns "
               <<  debug_name_or_string(f,"f") << " STRONG" << std::endl;
#else
     std::cout << "Curryable2helper returns f STRONG" << std::endl;
#endif
#endif
     //return f;
     return makeFull2( f );
   }
};

#endif

/*
 template <class R, class F, class G, class X>
   struct Curryable2Helper<R,F,Full1<binder2of2<G,X> >,X> {
   static inline R go( const F& f, const Full1<binder2of2<G,X> >& g , const X& x ) {
     //#ifdef FCPP_PRINT_HELPERS
     std::cout << "Curryable2Helper returns compose ( f(_,x),g )" << std::endl;
     //#endif
     //return f;
     return compose( f(_,x),g );
   }
};
*/


// NOTE: I would like to convert this to work with concepts.
// It is not in fact much used (unlike the helpers)
// To convert it would need the concept maps
// defined in full.h which are I think defined after this.
// One way around this would be to move the concept maps
// into concepts.h and have them defined earlier.
// Indeed, I noted that they could be moved when
// I first defined them for Full2 in full.h.
// NO in fact they cannot be moved.
// Maybe this class definition can be moved into full.h.
// DECISION: These are deprecated anyway so ignore and delete later.
template <class F>
class Curryable2 {
   const F f;
public:
   Curryable2( const F& ff ) : f(ff) {}

   template <class X, class Y=void>
   struct Sig
   : public FunType<typename F::template Sig<X,Y>::Arg1Type,
                    typename F::template Sig<X,Y>::Arg2Type,
                    typename RT<F,X,Y>::ResultType> {};

   template <class X>
   struct Sig<X,void> : public FunType<X,Full1<binder1of2<F,X> > > {};

   template <class Y>
   struct Sig<AutoCurryType,Y> 
   : public FunType<AutoCurryType,Y,Full1<binder2of2<F,Y> > > {};

   template <class X>
   struct Sig<X,AutoCurryType> 
   : public FunType<X,AutoCurryType,Full1<binder1of2<F,X> > > {};

   template <class X>
   typename Sig<X>::ResultType operator()( const X& x ) const {
      return makeFull1( binder1of2<F,X>(f,x) );
   }
   template <class X, class Y>
     //#ifdef FCPP_CONCEPTS
     //inline ConstCallable2<F,X,Y>::result_type
     //#else
   inline typename Sig<X,Y>::ResultType 
     //#endif
   operator()( const X& x, const Y& y ) const {
     //#ifdef FCPP_CONCEPTS
     //std::cout << "ConstCallable2 used" << std::endl;
     //return impl::Curryable2Helper<typename ConstCallable2<F,X,Y>::result_type,F,X,Y>::go(f,x,y);
     //#else
      // need partial specialization, so defer to a class helper
      return Curryable2Helper<typename Sig<X,Y>::ResultType,F,X,Y>::go(f,x,y);
      //#endif
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Curryable2::",f,"Curryable2");
   }
#endif
};

//////////////////////////////////////////////////////////////////////
// With Curryable2 out of the way, we can go back to the 3-arg binders.
//////////////////////////////////////////////////////////////////////

template <class Ternary, class A1, class A2, class A3>
class binder1and2and3of3
#ifndef FCPP_CONCEPTS
: public CFunType<typename RT<Ternary,A1,A2,A3>::ResultType> 
#endif
{
   const Ternary f;
   const A1 a1;
   const A2 a2;
   const A3 a3;
public:
   binder1and2and3of3( const Ternary& w, const A1& x, const A2& y, const A3& z )
   : f(w), a1(x), a2(y), a3(z) {}
#ifdef FCPP_CONCEPTS
   inline ConstCallable3<Ternary,A1,A2,A3>::result_type
#else
   typename RT<Ternary,A1,A2,A3>::ResultType 
#endif
   operator()() const { return f(a1,a2,a3); }

#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Ternary>() ) {
       std::string bindername = std::string("binder1and1and3of3::")+
              debug_traits_name(f);
       bindername += std::string("( ") + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1and2and3of3");
   }
#endif

};

struct Bind1and2and3of3 {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1and2and3of3");
   }
 #endif
#ifndef FCPP_CONCEPTS
   template <class Ternary, class A1, class A2, class A3>
   struct Sig
   : public FunType<Ternary,A1,A2,A3,
                    Full0<binder1and2and3of3<Ternary,A1,A2,A3> > > {};
#endif

   template <class Ternary, class A1, class A2, class A3>
   inline Full0<binder1and2and3of3<Ternary,A1,A2,A3> >
   operator()( const Ternary& f, const A1& a1, 
               const A2& a2, const A3& a3 ) const {
      return makeFull0( binder1and2and3of3<Ternary,A1,A2,A3>(f,a1,a2,a3) );
   }
};

template <class Ternary, class Arg1, class Arg2>
class binder1and2of3 {
   const Ternary f;
   const Arg1 a1;
   const Arg2 a2;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg3>
   struct Sig 
   : public FunType<typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg3Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::ResultType> {};
#endif

   binder1and2of3(const Ternary& w, const Arg1& x, const Arg2& y) : 
     f(w), a1(x), a2(y) {}
   template <class Arg3>
#ifdef FCPP_CONCEPTS
     inline ConstCallable3<Ternary,Arg1,Arg2,Arg3>::result_type
#else
   typename Sig<Arg3>::ResultType 
#endif
   operator()(const Arg3& z) const { return f(a1,a2,z); }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   // This is now extended to do the second name as well.
   // For two names the form ::(name1,name2) is generated.
   // Attempts to output the values as well now restored.
   // I have built argument_traits to avoid problems
   // when there is a name.
   std::string name() const {
      std::string temp;
      if ( debug_traits_has_name<Ternary>() ) {
	temp +=  std::string("binder1and2of3::")
	     + debug_traits_name(f);
	if ( debug_traits_has_name<Arg1>() ) {
	  if ( debug_traits_has_name<Arg2>() ) {
       	     temp += std::string("(")
                  + debug_traits_name(a1)
	          + std::string(",")
       		  + debug_traits_name(a2)
	          + std::string(")");
          } else {
       	    temp += debug_traits_name(a1);
   	    temp += std::string("( ");
	    temp += argument_traits_value(a2);
            temp += std::string(" )");
          }
       } else {
    	    temp += std::string("( ");
	    temp += argument_traits_value(a1);
    	    temp += std::string(" , ");
	    temp += argument_traits_value(a2);
            temp += std::string(" , _ )");
      }
       return temp;
    } else
        return std::string("binder1and2of3");
   }
#endif
};

struct Bind1and2of3 {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1and2of3");
   }
 #endif
#ifndef FCPP_CONCEPTS
   template <class Ternary, class A1, class A2>
   struct Sig
   : public FunType<Ternary,A1,A2,Full1<binder1and2of3<Ternary,A1,A2> > > {};
#endif

   template <class Ternary, class A1, class A2>
   Full1<binder1and2of3<Ternary,A1,A2> >
   operator()( const Ternary& f, const A1& a1, const A2& a2 ) const {
      return makeFull1( binder1and2of3<Ternary,A1,A2>(f,a1,a2) );
   }
};

template <class Ternary, class Arg2, class Arg3>
class binder2and3of3 {
   const Ternary f;
   const Arg2 a2;
   const Arg3 a3;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg1>
   struct Sig 
   : public FunType<typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg1Type,
                    typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType> {};
                    // need RT above due to g++ bug on line below
                    // typename Ternary::Sig<Arg1,Arg2,Arg3>::ResultType> {};
#endif

   binder2and3of3(const Ternary& w, const Arg2& y, const Arg3& z) : 
     f(w), a2(y), a3(z) {}
   template <class Arg1>
#ifdef FCPP_CONCEPTS
     inline ConstCallable3<Ternary,Arg1,Arg2,Arg3>::result_type
#else
   typename Sig<Arg1>::ResultType 
#endif
   operator()(const Arg1& x) const { return f(x,a2,a3); }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   // This is now extended to do the second name as well.
   // For two names the form ::(name1,name2) is generated.
   // Attempts to output the values as well now restored.
   // I have built argument_traits to avoid problems
   // when there is a name.
   std::string name() const {
      std::string temp;
      if ( debug_traits_has_name<Ternary>() ) {
	temp +=  std::string("binder2and3of3::")
	     + debug_traits_name(f);
	if ( debug_traits_has_name<Arg2>() ) {
	   temp += std::string("::");
           if ( debug_traits_has_name<Arg3>() ) {
       	     temp += std::string("(")
                  + debug_traits_name(a2)
	          + std::string(",")
       		  + debug_traits_name(a3)
	          + std::string(")");
          } else {
       	    temp += debug_traits_name(a2);
   	    temp += std::string("( ");
	    temp += argument_traits_value(a3);
            temp += std::string(" )");
          }
       } else {
    	    temp += std::string("( _ , ");
	    temp += argument_traits_value(a2);
    	    temp += std::string(" , ");
	    temp += argument_traits_value(a3);
            temp += std::string(" )");
      }
       return temp;
    } else
        return std::string("binder2and3of3");
   }
#endif
};

struct Bind2and3of3 {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind2and3of3");
   }
 #endif
#ifndef FCPP_CONCEPTS
   template <class Ternary, class A2, class A3>
   struct Sig
   : public FunType<Ternary,A2,A3,Full1<binder2and3of3<Ternary,A2,A3> > > {};
#endif

   template <class Ternary, class A2, class A3>
   Full1<binder2and3of3<Ternary,A2,A3> >
   operator()( const Ternary& f, const A2& a2, const A3& a3 ) const {
      return makeFull1( binder2and3of3<Ternary,A2,A3>(f,a2,a3) );
   }
};


template <class Ternary, class Arg1, class Arg3>
class binder1and3of3 {
   const Ternary f;
   const Arg1 a1;
   const Arg3 a3;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg2>
   struct Sig 
   : public FunType<typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg2Type,
                    typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType> {};
                    // need RT above due to g++ bug on line below
                    // typename Ternary::Sig<Arg1,Arg2,Arg3>::ResultType> {};
#endif

   binder1and3of3(const Ternary& w, const Arg1& x, const Arg3& z) : 
     f(w), a1(x), a3(z) {}
   template <class Arg2>
#ifdef FCPP_CONCEPTS
   inline ConstCallable3<Ternary,Arg1,Arg2,Arg3>::result_type
#else
   typename Sig<Arg2>::ResultType 
#endif
   operator()(const Arg2& y) const { return f(a1,y,a3); }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   // This is now extended to do the second name as well.
   // For two names the form ::(name1,name2) is generated.
   // Attempts to output the values as well now restored.
   // I have built argument_traits to avoid problems
   // when there is a name.
   std::string name() const {
      std::string temp;
      if ( debug_traits_has_name<Ternary>() ) {
	temp +=  std::string("binder1and3of3::")
	     + debug_traits_name(f);
	if ( debug_traits_has_name<Arg1>() ) {
	   temp += std::string("::");
           if ( debug_traits_has_name<Arg3>() ) {
       	     temp += std::string("(")
                  + debug_traits_name(a1)
	          + std::string(", _ ,")
       		  + debug_traits_name(a3)
	          + std::string(")");
          } else {
       	    temp += debug_traits_name(a1);
   	    temp += std::string("( _ ,");
	    temp += argument_traits_value(a3);
            temp += std::string(" )");
          }
       } else {
    	    temp += std::string("( ");
	    temp += argument_traits_value(a1);
    	    temp += std::string(" , _ , ");
	    temp += argument_traits_value(a3);
            temp += std::string(" )");
      }
       return temp;
    } else
        return std::string("binder1and3of3");
   }
#endif
};


struct Bind1and3of3 {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1and3of3");
   }
 #endif
#ifndef FCPP_CONCEPTS
   template <class Ternary, class A1, class A3>
   struct Sig
   : public FunType<Ternary,A1,A3,Full1<binder1and3of3<Ternary,A1,A3> > > {};
#endif

   template <class Ternary, class A1, class A3>
   Full1<binder1and3of3<Ternary,A1,A3> >
   operator()( const Ternary& f, const A1& a1, const A3& a3 ) const {
      return makeFull1( binder1and3of3<Ternary,A1,A3>(f,a1,a3) );
   }
};

template <class Ternary, class Arg1>
class binder1of3 {
   const Ternary f;
   const Arg1 x;
public:
   binder1of3( const Ternary& a, const Arg1& b ) : f(a), x(b) {}

#ifndef FCPP_CONCEPTS
   template <class Arg2, class Arg3>
   struct Sig 
   : public FunType<typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg2Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg3Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::ResultType> {};
#endif

   template <class Arg2, class Arg3>
#ifdef FCPP_CONCEPTS
   inline ConstCallable3<Ternary,Arg1,Arg2,Arg3>::result_type
#else
   typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType
#endif
   operator()( const Arg2& y, const Arg3& z ) const {
      return f(x,y,z);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Ternary>() ) {
	 temp += std::string("binder1of3::")
	      + debug_traits_name(f);
	 if ( debug_traits_has_name<Arg1>() ) {
	 temp += std::string("::")
       	      +  debug_traits_name(x);
       } else {
  	 temp += std::string("( ");
	 temp += argument_traits_value(x);
         temp += std::string(" , _ , _ )");
       }
       return temp;
     } else
         return std::string("binder1of3");
     }
#endif
};

struct Bind1of3 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1of3");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Ternary, class Arg1>
   struct Sig
   : public FunType<Ternary,Arg1,Full2<binder1of3<Ternary,Arg1> > > {};
#endif

   template <class Ternary, class Arg1>
#ifdef FCPP_CONCEPTS
   inline Full2<binder1of3<Ternary,Arg1> >
#else
   typename Sig<Ternary,Arg1>::ResultType
#endif
   operator()( const Ternary& f, const Arg1& x ) const {
      return makeFull2( binder1of3<Ternary,Arg1>(f,x) );
   }
};

template <class Ternary, class Arg2>
class binder2of3 {
   const Ternary f;
   const Arg2 x;
public:
   binder2of3( const Ternary& a, const Arg2& b ) : f(a), x(b) {}

#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg3>
   struct Sig 
   : public FunType<typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg1Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg3Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::ResultType> {};
#endif

   template <class Arg1, class Arg3>
#ifdef FCPP_CONCEPTS
   inline ConstCallable3<Ternary,Arg1,Arg2,Arg3>::result_type
#else
   typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType
#endif
   operator()( const Arg1& y, const Arg3& z ) const {
      return f(y,x,z);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Ternary>() ) {
	 temp += std::string("binder2of3::")
	      + debug_traits_name(f);
	 if ( debug_traits_has_name<Arg2>() ) {
	 temp += std::string("::")
       	      +  debug_traits_name(x);
       } else {
  	 temp += std::string("( _ ,");
	 temp += argument_traits_value(x);
         temp += std::string(" , _ )");
       }
       return temp;
     } else
         return std::string("binder2of3");
     }
#endif
};

struct Bind2of3 {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind2of3");
   }
 #endif
#ifndef FCPP_CONCEPTS
   template <class Ternary, class Arg2>
   struct Sig
   : public FunType<Ternary,Arg2,Full2<binder2of3<Ternary,Arg2> > > {};
#endif

   template <class Ternary, class Arg2>
#ifdef FCPP_CONCEPTS
   inline Full2<binder2of3<Ternary,Arg2> >
#else
   typename Sig<Ternary,Arg2>::ResultType
#endif
   operator()( const Ternary& f, const Arg2& x ) const {
      return makeFull2( binder2of3<Ternary,Arg2>(f,x) );
   }
};

template <class Ternary, class Arg3>
class binder3of3 {
   const Ternary f;
   const Arg3 x;
public:
   binder3of3( const Ternary& a, const Arg3& b ) : f(a), x(b) {}

#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg2>
   struct Sig 
   : public FunType<typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg1Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg2Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::ResultType> {};
#endif

   template <class Arg1, class Arg2>
#ifdef FCPP_CONCEPTS
   inline ConstCallable3<Ternary,Arg1,Arg2,Arg3>::result_type
#else
   typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType
#endif
   operator()( const Arg1& y, const Arg2& z ) const {
      return f(y,z,x);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     std::string temp;
     if ( debug_traits_has_name<Ternary>() ) {
	 temp += std::string("binder3of3::")
	      + debug_traits_name(f);
	 if ( debug_traits_has_name<Arg3>() ) {
	 temp += std::string("::")
       	      +  debug_traits_name(x);
       } else {
  	 temp += std::string("( _ , _ , ");
	 temp += argument_traits_value(x);
         temp += std::string(" )");
       }
       return temp;
     } else
         return std::string("binder3of3");
     }
#endif
};

struct Bind3of3 {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind3of3");
   }
 #endif
#ifndef FCPP_CONCEPTS
   template <class Ternary, class Arg3>
   struct Sig
   : public FunType<Ternary,Arg3,Full2<binder3of3<Ternary,Arg3> > > {};
#endif

   template <class Ternary, class Arg3>
#ifdef FCPP_CONCEPTS
   inline Full2<binder3of3<Ternary,Arg3> >
#else
   typename Sig<Ternary,Arg3>::ResultType
#endif
   operator()( const Ternary& f, const Arg3& x ) const {
      return makeFull2( binder3of3<Ternary,Arg3>(f,x) );
   }
};

//#ifdef FCPP_EXTRA_STRONG_CURRY

// This code defines cycle3 and cycle3r as an FC++ functoids such that 
//
// cycle3(f)(a,b,c) -> f(b,c,a)
// cycle3r(f)(a,b,c) -> f(c,a,b)

// cycle3r is used to make f(_)(a,b)(c) -> f(c,a,b)


template <class Ternary>
class cycler3 {
   const Ternary f;
 
public:
 cycler3(const Ternary &g) : f(g) { }

#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg2, class Arg3>
   struct Sig : 
     public FunType<typename Ternary::template Sig<Arg2,Arg3,Arg1>::Arg1Type,
     typename Ternary::template Sig<Arg2,Arg3,Arg1>::Arg2Type,
                  typename Ternary::template Sig<Arg2,Arg3,Arg1>::Arg3Type,
                  typename Ternary::template Sig<Arg2,Arg3,Arg1>::ResultType> {};
#endif

   template < class Arg1, class Arg2, class Arg3>
#ifdef FCPP_CONCEPTS
     inline ConstCallable3<Ternary,Arg2,Arg3,Arg1>::result_type
#else
     typename Ternary::template Sig<Arg2,Arg3,Arg1>::ResultType
#endif
     operator()( const Arg1& x, const Arg2& y, const Arg3& z ) const {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     std::cout << "cycler3 called with " //<< f
         << "(" << argument_traits_value(x)
         << "," << argument_traits_value(y)
         << "," << argument_traits_value(z) << ")" << std::endl;
#else
     std::cout << "cycler3 called " << std::endl;
#endif
#endif
     return f(y,z,x); // Note arguments are cycled.
   } 
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("cycler3");
   }
#endif
};



struct Cycle3 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Cycle3");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class F>
     struct Sig : public FunType<F,Full3<cycler3<F> > >{};
#endif

    template <class F>
    inline Full3<cycler3<F> >
    operator()(const F& f) const {
       return makeFull3(cycler3<F>(f));
    }

 };

template <class Ternary>
class cycler3r {
   const Ternary f;
 
public:
 cycler3r(const Ternary &g) : f(g) { }

#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg2, class Arg3>
   struct Sig : 
     public FunType<typename Ternary::template Sig<Arg3,Arg1,Arg2>::Arg1Type,
     typename Ternary::template Sig<Arg3,Arg1,Arg2>::Arg2Type,
                  typename Ternary::template Sig<Arg3,Arg1,Arg2>::Arg3Type,
                  typename Ternary::template Sig<Arg3,Arg1,Arg2>::ResultType> {};
#endif

   template < class Arg1, class Arg2, class Arg3>
#ifdef FCPP_CONCEPTS
     inline ConstCallable3<Ternary,Arg3,Arg1,Arg2>::result_type
#else
     typename Ternary::template Sig<Arg3,Arg1,Arg2>::ResultType
#endif
     operator()( const Arg1& x, const Arg2& y, const Arg3& z ) const {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     std::cout << "cycler3r called with " //<< f
         << "(" << argument_traits_value(x)
         << "," << argument_traits_value(y)
         << "," << argument_traits_value(z) << ")" << std::endl;
#else
     std::cout << "cycler3r called " << std::endl;
#endif
#endif
     return f(z,x,y); // Note arguments are cycled.
   } 
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("cycler3r");
   }
#endif
};



struct Cycle3r {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Cycle3r");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class F>
     struct Sig : public FunType<F,Full3<cycler3r<F> > >{};
#endif

    template <class F>
    inline Full3<cycler3r<F> >
    operator()(const F& f) const {
       return makeFull3(cycler3r<F>(f));
    }

 };

//#endif


// FC++.1.5.2
#ifdef FCPP152
//////////////////////////////////////////////////////////////////////
// This is going to be the meat of the modifications.
// Here are binders for 4 argument functions.
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// binders for 4 argument functions - first bind all 4.
//////////////////////////////////////////////////////////////////////

template <class Quaternary, class A1, class A2, class A3, class A4>
class binder1to4of4
#ifndef FCPP_CONCEPTS
: public CFunType<typename RT<Quaternary,A1,A2,A3,A4>::ResultType> 
#endif
{
   const Quaternary f;
   const A1 a1;
   const A2 a2;
   const A3 a3;
   const A4 a4;
public:
   binder1to4of4( const Quaternary& w, const A1& x, const A2& y, const A3& z, const A4& v )
   : f(w), a1(x), a2(y), a3(z), a4(v) {}
#ifdef FCPP_CONCEPTS
   inline ConstCallable4<Quaternary,A1,A2,A3,A4>::result_type
#else
   typename RT<Quaternary,A1,A2,A3,A4>::ResultType
#endif
   operator()() const { return f(a1,a2,a3,a4); }

#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quaternary>() ) {
       std::string bindername = std::string("binder1to4of4::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" , ") + debug_name_or_value(a4);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1to4of4");
   }
#endif

};

struct Bind1to4of4 {
  //#ifndef FCPP_CONCEPTS
   // No sig, as there are no 5-arg functoids - there is now.
   template <class Quaternary, class A1, class A2, class A3, class A4>
   struct Sig
     : public FunType<Quaternary,A1,A2,A3,A4,
      Full0<binder1to4of4<Quaternary,A1,A2,A3,A4> > > {};
  //#endif

   template <class Quaternary, class A1, class A2, class A3, class A4>
   Full0<binder1to4of4<Quaternary,A1,A2,A3,A4> >
   operator()( const Quaternary& f, const A1& a1,
               const A2& a2, const A3& a3, const A4& a4) const {
      return makeFull0( binder1to4of4<Quaternary,A1,A2,A3,A4>(f,a1,a2,a3,a4) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to4of4");
   }
#endif
};

//////////////////////////////////////////////////////////////////////
// bind 3 arguments - there are 4 possibilities.
//////////////////////////////////////////////////////////////////////

template <class Quaternary, class Arg1, class Arg2, class Arg3>
class binder1and2and3of4 {
   const Quaternary f;
   const Arg1 a1;
   const Arg2 a2;
   const Arg3 a3;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg4>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg4Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif

   binder1and2and3of4(const Quaternary& w, const Arg1& x, const Arg2& y, const Arg3& z) : 
     f(w), a1(x), a2(y), a3(z) {}
   template <class Arg4>
#ifdef FCPP_CONCEPTS
   inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename Sig<Arg4>::ResultType
#endif
   operator()(const Arg4& v) const { return f(a1,a2,a3,v); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quaternary>() ) {
       std::string bindername = std::string("binder1and2and3of4::")+
              debug_traits_name(f);
       bindername += std::string("( ") + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1and2and3of4");
   }
#endif
};

struct Bind1and2and3of4 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1and2and3of4");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class A1, class A2, class A3>
   struct Sig
   : public FunType<Quaternary,A1,A2,A3,Full1<binder1and2and3of4<Quaternary,A1,A2,A3> > > {};
#endif

   template <class Quaternary, class A1, class A2, class A3>
   inline Full1<binder1and2and3of4<Quaternary,A1,A2,A3> >
   operator()( const Quaternary& f, const A1& a1, const A2& a2, const A3& a3 ) const {
#ifdef FCPP_PRINT_HELPERS
      std::cout << "bind1and2and3of4 called" << std::endl;
#endif
      return makeFull1( binder1and2and3of4<Quaternary,A1,A2,A3>(f,a1,a2,a3) );
   }
};

template <class Quaternary, class Arg1, class Arg2, class Arg4>
class binder1and2and4of4 {
   const Quaternary f;
   const Arg1 a1;
   const Arg2 a2;
   const Arg4 a4;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg3>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg3Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif

   binder1and2and4of4(const Quaternary& w, const Arg1& x, const Arg2& y, const Arg4& v) : 
     f(w), a1(x), a2(y), a4(v) {}
   template <class Arg3>
#ifdef FCPP_CONCEPTS
   inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename Sig<Arg3>::ResultType 
#endif
   operator()(const Arg3& z) const { return f(a1,a2,z,a4); }
};

struct Bind1and2and4of4 {
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class A1, class A2, class A4>
   struct Sig
   : public FunType<Quaternary,A1,A2,A4,Full1<binder1and2and4of4<Quaternary,A1,A2,A4> > > {};
#endif

   template <class Quaternary, class A1, class A2, class A4>
   Full1<binder1and2and4of4<Quaternary,A1,A2,A4> >
   inline operator()( const Quaternary& f, const A1& a1, const A2& a2, const A4& a4 ) const {
      return makeFull1( binder1and2and4of4<Quaternary,A1,A2,A4>(f,a1,a2,a4) );
   }
};

template <class Quaternary, class Arg1, class Arg3, class Arg4>
class binder1and3and4of4 {
   const Quaternary f;
   const Arg1 a1;
   const Arg3 a3;
   const Arg4 a4;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg2>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg2Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif

   binder1and3and4of4(const Quaternary& w, const Arg1& x, const Arg3& z, const Arg4& v) : 
     f(w), a1(x), a3(z), a4(v) {}
   template <class Arg2>
#ifdef FCPP_CONCEPTS
   inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename Sig<Arg2>::ResultType 
#endif
   operator()(const Arg2& y) const { return f(a1,y,a3,a4); }
};

struct Bind1and3and4of4 {
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class A1, class A3, class A4>
   struct Sig
   : public FunType<Quaternary,A1,A3,A4,Full1<binder1and3and4of4<Quaternary,A1,A3,A4> > > {};
#endif

   template <class Quaternary, class A1, class A3, class A4>
   Full1<binder1and3and4of4<Quaternary,A1,A3,A4> >
   inline operator()( const Quaternary& f, const A1& a1, const A3& a3, const A4& a4 ) const {
      return makeFull1( binder1and3and4of4<Quaternary,A1,A3,A4>(f,a1,a3,a4) );
   }
};

template <class Quaternary, class Arg2, class Arg3, class Arg4>
class binder2and3and4of4 {
   const Quaternary f;
   const Arg2 a2;
   const Arg3 a3;
   const Arg4 a4;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg1>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg1Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif

   binder2and3and4of4(const Quaternary& w, const Arg2& y, const Arg3& z, const Arg4& v) : 
     f(w), a2(y), a3(z), a4(v) {}
   template <class Arg1>
#ifdef FCPP_CONCEPTS
   inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename Sig<Arg1>::ResultType 
#endif
   operator()(const Arg1& x) const { return f(x,a2,a3,a4); }
};

struct Bind2and3and4of4 {
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class A2, class A3, class A4>
   struct Sig 
   : public FunType<Quaternary,A2,A3,A4,Full1<binder2and3and4of4<Quaternary,A2,A3,A4> > > {};
#endif

   template <class Quaternary, class A2, class A3, class A4>
   Full1<binder2and3and4of4<Quaternary,A2,A3,A4> >
   inline operator()( const Quaternary& f, const A2& a2, const A3& a3, const A4& a4 ) const {
      return makeFull1( binder2and3and4of4<Quaternary,A2,A3,A4>(f,a2,a3,a4) );
   }
};

//////////////////////////////////////////////////////////////////////
// bind 2 arguments - there are 6 possibilities.
//////////////////////////////////////////////////////////////////////

template <class Quaternary, class Arg1, class Arg2>
class binder1and2of4 {
   const Quaternary f;
   const Arg1 a1;
   const Arg2 a2;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg3, class Arg4>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg3Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg4Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif

   binder1and2of4(const Quaternary& w, const Arg1& x, const Arg2& y) : 
     f(w), a1(x), a2(y) {}
   template <class Arg3,class Arg4>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename Sig<Arg3,Arg4>::ResultType 
#endif
   operator()(const Arg3& z, const Arg4& v) const { return f(a1,a2,z,v); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quaternary>() ) {
       std::string bindername = std::string("binder1and2of4::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1and2of4");
   }
#endif
};

struct Bind1and2of4 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1and2of4");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class A1, class A2>
   struct Sig
   : public FunType<Quaternary,A1,A2,Full2<binder1and2of4<Quaternary,A1,A2> > > {};
#endif

   template <class Quaternary, class A1, class A2>
   inline Full2<binder1and2of4<Quaternary,A1,A2> >
   operator()( const Quaternary& f, const A1& a1, const A2& a2 ) const {
      return makeFull2( binder1and2of4<Quaternary,A1,A2>(f,a1,a2) );
   }
};

template <class Quaternary, class Arg2, class Arg3>
class binder2and3of4 {
   const Quaternary f;
   const Arg2 a2;
   const Arg3 a3;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg4>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg1Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg4Type,
                    typename RT<Quaternary,Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif

   binder2and3of4(const Quaternary& w, const Arg2& y, const Arg3& z) : 
     f(w), a2(y), a3(z) {}
   template <class Arg1, class Arg4>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename Sig<Arg1,Arg4>::ResultType 
#endif
   operator()(const Arg1& x, const Arg4& v) const { return f(x,a2,a3,v); }
};

struct Bind2and3of4 {
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class A2, class A3>
   struct Sig
   : public FunType<Quaternary,A2,A3,Full2<binder2and3of4<Quaternary,A2,A3> > > {};
#endif

   template <class Quaternary, class A2, class A3>
   Full2<binder2and3of4<Quaternary,A2,A3> >
   operator()( const Quaternary& f, const A2& a2, const A3& a3) const {
      return makeFull2( binder2and3of4<Quaternary,A2,A3>(f,a2,a3) );
   }
};

template <class Quaternary, class Arg1, class Arg3>
class binder1and3of4 {
   const Quaternary f;
   const Arg1 a1;
   const Arg3 a3;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg2, class Arg4>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg1Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg3Type,
                    typename RT<Quaternary,Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif
                    
   binder1and3of4(const Quaternary& w, const Arg1& x, const Arg3& z) : 
     f(w), a1(x), a3(z) {}
   template <class Arg2, class Arg4>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename Sig<Arg2,Arg4>::ResultType 
#endif
   operator()(const Arg2& y, const Arg4& v) const { return f(a1,y,a3,v); }
};

struct Bind1and3of4 {
#ifndef FCPP_CONCEPTS
  template <class Quaternary, class A1, class A3>
   struct Sig
   : public FunType<Quaternary,A1,A3,Full2<binder1and3of4<Quaternary,A1,A3> > > {};
#endif

   template <class Quaternary, class A1, class A3>
   inline Full2<binder1and3of4<Quaternary,A1,A3> >
   operator()( const Quaternary& f, const A1& a1, const A3& a3 ) const {
     return makeFull2( binder1and3of4<Quaternary,A1,A3>(f,a1,a3) ); 
   }
};

template <class Quaternary, class Arg1, class Arg4>
class binder1and4of4 {
   const Quaternary f;
   const Arg1 a1;
   const Arg4 a4;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg2, class Arg3>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg2Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg3Type,
                    typename RT<Quaternary,Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif
                    
   binder1and4of4(const Quaternary& w, const Arg1& x, const Arg4& v) : 
     f(w), a1(x), a4(v) {}
   template <class Arg2, class Arg3>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename Sig<Arg2,Arg3>::ResultType 
#endif
   operator()(const Arg2& y, const Arg3& z) const { return f(a1,y,z,a4); }
};

struct Bind1and4of4 {
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class A1, class A4>
   struct Sig
   : public FunType<Quaternary,A1,A4,Full2<binder1and4of4<Quaternary,A1,A4> > > {};
#endif

   template <class Quaternary, class A1, class A4>
   inline Full2<binder1and4of4<Quaternary,A1,A4> >
   operator()( const Quaternary& f, const A1& a1, const A4& a4 ) const {
     return makeFull2( binder1and4of4<Quaternary,A1,A4>(f,a1,a4) ); 
   }
};

template <class Quaternary, class Arg2, class Arg4>
class binder2and4of4 {
   const Quaternary f;
   const Arg2 a2;
   const Arg4 a4;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg3>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg1Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg3Type,
                    typename RT<Quaternary,Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif
                    
   binder2and4of4(const Quaternary& w, const Arg2& y, const Arg4& v) : 
     f(w), a2(y), a4(v) {}
   template <class Arg1, class Arg3>
#ifdef FCPP_CONCEPTS
   inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename Sig<Arg1,Arg3>::ResultType 
#endif
   operator()(const Arg1& x, const Arg3& z) const { return f(x,a2,z,a4); }
};

struct Bind2and4of4 {
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class A2, class A4>
   struct Sig
   : public FunType<Quaternary,A2,A4,Full2<binder2and4of4<Quaternary,A2,A4> > > {};
#endif

   template <class Quaternary, class A2, class A4>
   inline Full2<binder2and4of4<Quaternary,A2,A4> >
   operator()( const Quaternary& f, const A2& a2, const A4& a4 ) const {
     return makeFull2( binder2and4of4<Quaternary,A2,A4>(f,a2,a4) ); 
   }
};

template <class Quaternary, class Arg3, class Arg4>
class binder3and4of4 {
   const Quaternary f;
   const Arg3 a3;
   const Arg4 a4;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg2>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg1Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg2Type,
                    typename RT<Quaternary,Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif
                    
   binder3and4of4(const Quaternary& w, const Arg3& z, const Arg4& v) : 
     f(w), a3(z), a4(v) {}
   template <class Arg1, class Arg2>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename Sig<Arg1,Arg2>::ResultType 
#endif

   operator()(const Arg1& x, const Arg2& y) const { return f(x,y,a3,a4); }
};

struct Bind3and4of4 {
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class A3, class A4>
   struct Sig
   : public FunType<Quaternary,A3,A4,Full2<binder3and4of4<Quaternary,A3,A4> > > {};
#endif

   template <class Quaternary, class A3, class A4>
   Full2<binder3and4of4<Quaternary,A3,A4> >
   inline operator()( const Quaternary& f, const A3& a3, const A4& a4 ) const {
     return makeFull2( binder3and4of4<Quaternary,A3,A4>(f,a3,a4) ); 
   }
};

//////////////////////////////////////////////////////////////////////
// bind 1 argument - there are 4 possibilities.
//////////////////////////////////////////////////////////////////////

template <class Quaternary, class Arg1>
class binder1of4 {
   const Quaternary f;
   const Arg1 x;
public:
   binder1of4( const Quaternary& a, const Arg1& b ) : f(a), x(b) {}

#ifndef FCPP_CONCEPTS
   template <class Arg2, class Arg3, class Arg4> struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg2Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg3Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg4Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif

   template <class Arg2, class Arg3, class Arg4>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename RT<Quaternary,Arg1,Arg2,Arg3,Arg4>::ResultType
#endif
   operator()( const Arg2& y, const Arg3& z, const Arg4& v ) const {
      return f(x,y,z,v);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quaternary>() ) {
       std::string bindername = std::string("binder1of4::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(x);
       bindername += std::string(" )");
       return bindername;
     } else
        return std::string("binder1of4");
   }
#endif
};

struct Bind1of4 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1of4");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class Arg1>
   struct Sig
   : public FunType<Quaternary,Arg1,Full3<binder1of4<Quaternary,Arg1> > > {};
#endif

   template <class Quaternary, class Arg1>
#ifdef FCPP_CONCEPTS
   inline Full3<binder1of4<Quaternary,Arg1> >
#else
   typename Sig<Quaternary,Arg1>::ResultType
#endif
   operator()( const Quaternary& f, const Arg1& x ) const {
      return makeFull3( binder1of4<Quaternary,Arg1>(f,x) );
   }
};

template <class Quaternary, class Arg2>
class binder2of4 {
   const Quaternary f;
   const Arg2 x;
public:
   binder2of4( const Quaternary& a, const Arg2& b ) : f(a), x(b) {}

#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg3, class Arg4>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg1Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg3Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg4Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif

   template <class Arg1, class Arg3, class Arg4>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename RT<Quaternary,Arg1,Arg2,Arg3,Arg4>::ResultType
#endif
   operator()( const Arg1& y, const Arg3& z, const Arg4& v ) const {
      return f(y,x,z,v);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quaternary>() ) {
       std::string bindername = std::string("binder2of4::")+
              debug_traits_name(f);
       bindername += std::string("( _ , ")  + debug_name_or_value(x);
       bindername += std::string(" )");
       return bindername;
     } else
        return std::string("binder2of4");
   }
#endif
};

struct Bind2of4 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind2of4");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class Arg2>
   struct Sig
   : public FunType<Quaternary,Arg2,Full3<binder2of4<Quaternary,Arg2> > > {};
#endif

   template <class Quaternary, class Arg2>
#ifdef FCPP_CONCEPTS
   inline Full3<binder2of4<Quaternary,Arg2> >
#else
   typename Sig<Quaternary,Arg2>::ResultType
#endif
   operator()( const Quaternary& f, const Arg2& x ) const {
      return makeFull3( binder2of4<Quaternary,Arg2>(f,x) );
   }
};

template <class Quaternary, class Arg3>
class binder3of4 {
   const Quaternary f;
   const Arg3 x;
public:
   binder3of4( const Quaternary& a, const Arg3& b ) : f(a), x(b) {}

#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg2, class Arg4>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg1Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg2Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg4Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif

   template <class Arg1, class Arg2, class Arg4>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename RT<Quaternary,Arg1,Arg2,Arg3,Arg4>::ResultType
#endif
   operator()( const Arg1& y, const Arg2& z, const Arg4& v ) const {
      return f(y,z,x,v);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quaternary>() ) {
       std::string bindername = std::string("binder3of4::")+
              debug_traits_name(f);
       bindername += std::string("( _ , _ , ")  + debug_name_or_value(x);
       bindername += std::string(" )");
       return bindername;
     } else
        return std::string("binder3of4");
   }
#endif
};

struct Bind3of4 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind3of4");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class Arg3>
   struct Sig
     : public FunType<Quaternary,Arg3,Full3<binder3of4<Quaternary,Arg3> > > {};
#endif

   template <class Quaternary, class Arg3>
#ifdef FCPP_CONCEPTS
   inline Full3<binder3of4<Quaternary,Arg3> >
#else
   typename Sig<Quaternary,Arg3>::ResultType
#endif
   operator()( const Quaternary& f, const Arg3& x ) const {
      return makeFull3( binder3of4<Quaternary,Arg3>(f,x) );
   }
};

template <class Quaternary, class Arg4>
class binder4of4 {
   const Quaternary f;
   const Arg4 v;
public:
   binder4of4( const Quaternary& a, const Arg4& b ) : f(a), v(b) {}

#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg2, class Arg3>
   struct Sig 
   : public FunType<typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg1Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg2Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::Arg3Type,
                    typename Quaternary::template Sig<Arg1,Arg2,Arg3,Arg4>::ResultType> {};
#endif

   template <class Arg1, class Arg2, class Arg3>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<Quaternary,Arg1,Arg2,Arg3,Arg4>::result_type
#else
   typename RT<Quaternary,Arg1,Arg2,Arg3,Arg4>::ResultType
#endif
   operator()( const Arg1& y, const Arg2& z, const Arg3& x ) const {
      return f(y,z,x,v);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quaternary>() ) {
       std::string bindername = std::string("binder4of4::")+
              debug_traits_name(f);
       bindername += std::string("( _ , _ , _ , ")  + debug_name_or_value(v);
       bindername += std::string(" )");
       return bindername;
     } else
        return std::string("binder4of4");
   }
#endif
};

struct Bind4of4 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind4of4");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Quaternary, class Arg4>
   struct Sig
     : public FunType<Quaternary,Arg4,Full3<binder4of4<Quaternary,Arg4> > > {};
#endif

   template <class Quaternary, class Arg4>
#ifdef FCPP_CONCEPTS
   inline Full3<binder4of4<Quaternary,Arg4> >
#else
   typename Sig<Quaternary,Arg4>::ResultType
#endif
   operator()( const Quaternary& f, const Arg4& x ) const {
      return makeFull3( binder4of4<Quaternary,Arg4>(f,x) );
   }
};


////////////////////////////////////////////////////////////////////////////////
// Adapting the binders to work with FCPP_CONCEPTS
// 
// The binders have been adapted in the same way as other code in full.h and function.h
// using the concept maps defined in full.h.
//
// To adapt binderAof5 and binderXof5 it is necessary to define some further 
// concept maps which are introduced below.
// Concept maps have to be in the same namespace as the concept which is being mapped.
// This means that namespace fcpp::impl has to be closed to define them.
//
// John Fletcher January 2008
////////////////////////////////////////////////////////////////////////////////

// These must be defined before the concept maps are defined.
// These objects control the choice of which parameter to bind.
// The values of n have been added for use with FCPP_DEBUG, initially in variadic_fcpp.h
struct Bind1 { static int n; };
Bind1 bind_first;
int Bind1::n = 1;

struct Bind2 { static int n; };
Bind2 bind_second;
int Bind2::n = 2;

struct Bind3 { static int n; };
Bind3 bind_third;
int Bind3::n = 3;

struct Bind4 { static int n; };
Bind4 bind_fourth;
int Bind4::n = 4;

struct Bind5 { static int n; };
Bind5 bind_fifth;
int Bind5::n = 5;

struct Bind6 { static int n; };
Bind6 bind_sixth;
int Bind6::n = 6;

struct Bind7 { static int n; };
Bind7 bind_seventh;
int Bind7::n = 7;

struct Bind8 { static int n; };
Bind8 bind_eighth;
int Bind8::n = 8;

#ifdef FCPP_CONCEPTS
// Space for the concept maps needed.
// These cannot be in namespace impl.
} // end namespace impl

#include "curry_concept_maps.h"

namespace impl { // resume namespace impl

#endif
// FC++.1.5.2
#ifdef FCPP152
//////////////////////////////////////////////////////////////////////
// Binders for Full4, Full5, Full7 and Full8
//////////////////////////////////////////////////////////////////////
#include "curry_binders5.h"
#include "curry_binders6.h"
#include "curry_binders7.h"
#include "curry_binders8.h"
#endif


#endif

//////////////////////////////////////////////////////////////////////
// For future reference I have been attempting to establish more
// items in the following list.
//
//  1. Unary     (Primary)
//  2. Binary    (Secondary)
//  3. Tertiary  (Ternary or trinary)
//  4. Quaternary
//  5. Quintinary
//  6. Sextinary
//  7. Septimary
//  8. Octavary
//  9. Nonary
// 10. Decimary
// 
// See for example the following discussion.
// http://blogs.msdn.com/larryosterman/archive/2004/11/19/259032.aspx
// 
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// "curry" versions. Now that we have auto-currying, you rarely need to
// call curry() explicitly, unless you are trying to get lazy evaulation
// in examples like
//    curry2( map, f, l ).
// It used to be the case that you might also do
//    curry2( map, f ),
// but nowadays you can say the same thing with just
//    map(f).
//////////////////////////////////////////////////////////////////////

// FC++.1.5.2
#ifdef FCPP152

}

 namespace impl {


struct Curry4 {

#ifndef FCPP_CONCEPTS
  template <class Quaternary, class A1, class A2 = void, class A3 = void, class A4 = void>
  struct Sig
  : public FunType<Quaternary, A1, A2, A3, A4,
                   Full0<binder1to4of4<Quaternary,A1,A2,A3,A4> > > {};

  template <class Quaternary, class A1, class A2, class A3> struct Sig<Quaternary, A1, A2, A3>
  : public FunType<Quaternary, A1, A2, A3, void,
                   Full1<binder1and2and3of4<Quaternary,A1,A2,A3> > > {};

  template <class Quaternary, class A1, class A2> struct Sig<Quaternary, A1, A2>
  : public FunType<Quaternary, A1, A2, void, void,
                   Full2<binder1and2of4<Quaternary,A1,A2> > > {};

  template <class Quaternary, class A1> struct Sig<Quaternary, A1>
  : public FunType<Quaternary, A1, void, void, void,
                   Full3<binder1of4<Quaternary,A1> > > {};

  template <class Quaternary>
  struct Sig<Quaternary, void, void, void, void>
    : public Full4<Quaternary> {};

#endif

  template <class Quaternary, class A1, class A2, class A3, class A4>
  inline Full0<binder1to4of4<Quaternary,A1,A2,A3,A4> >
  operator()( const Quaternary& f, const A1& a1, const A2& a2,
	      const A3& a3, const A4& a4 ) const {
    return makeFull0( binder1to4of4<Quaternary,A1,A2,A3,A4>(f,a1,a2,a3,a4) );
  }

  template <class Quaternary, class A1, class A2, class A3>
  inline Full1<binder1and2and3of4<Quaternary,A1,A2,A3> > // of3 to be of4
  operator()( const Quaternary& f, const A1& a1, const A2& a2, const A3& a3 ) const {
    return makeFull1( binder1and2and3of4<Quaternary,A1,A2,A3>(f,a1,a2,a3) );
  }

  template <class Quaternary, class A1, class A2>
  inline Full2<binder1and2of4<Quaternary,A1,A2> > // of3 to be of4
    //typename Sig<Quaternary, A1, A2>::ResultType
  operator()( const Quaternary& f, const A1& a1, const A2& a2 ) const {
    return makeFull2( binder1and2of4<Quaternary,A1,A2>(f,a1,a2) );
  }

  template <class Quaternary, class A1>
  inline Full3<binder1of4<Quaternary,A1> > // of3 to be of4
    //typename Sig<Quaternary, A1>::ResultType
  operator()( const Quaternary& f, const A1& a1 ) const {
    return makeFull3( binder1of4<Quaternary,A1>(f,a1) );
  }

  template <class Quaternary>
  inline Quaternary
  operator()( const Quaternary& f) const {
    bool b = &FunctoidTraits<Quaternary>::template ensure_accepts<4>::args;
    //assert(FunctoidTraits<Ternary>::max_args == 3);
    assert(FunctoidTraits_max_args<Quaternary>() == 4);
    return f;
  }

#ifdef FCPP_DEBUG
   std::string name() const {
     // This does not work as the code has no access to "Quaternary".
     // Instead the debug drops through to the binder.
     // The only use of this function is to report the name e.g.
     // sprint(curry4). The same applies to curry1, curry2 and curry3.
     //      if (fcpp::traits::debug_traits<Quaternary>::has_name)
     //   return std::string("Curry4::")+fcpp::traits::debug_traits<F>::name(f);
     // else
        return std::string("Curry4");
   }
#endif


};

#endif

struct Curry3 {

#ifndef FCPP_CONCEPTS
  template <class Ternary, class A1 = void, class A2 = void, class A3 = void>
  struct Sig
    : public FunType<Ternary, A1, A2, A3,
                     Full0<binder1and2and3of3<Ternary,A1,A2,A3> > > {};

  template <class Ternary, class A1, class A2>
  struct Sig<Ternary, A1, A2, void>
  : public FunType<Ternary, A1, A2, Full1<binder1and2of3<Ternary,A1,A2> > > {};

  template <class Ternary, class A1>
  struct Sig<Ternary, A1, void, void>
    : public FunType<Ternary, A1, Full2<binder1of3<Ternary,A1> > > {};

  template <class Ternary>
  struct Sig<Ternary, void, void, void>
    : public Full3<Ternary> {};

#endif

  template <class Ternary, class A1, class A2, class A3>
    //typename Sig<Ternary, A1, A2, A3>::ResultType
  inline Full0<binder1and2and3of3<Ternary,A1,A2,A3> >
  operator()( const Ternary& f, const A1& a1, const A2& a2, 
	      const A3& a3 ) const {
    return makeFull0( binder1and2and3of3<Ternary,A1,A2,A3>(f,a1,a2,a3) );
  }

  template <class Ternary, class A1, class A2>
    //typename Sig<Ternary, A1, A2>::ResultType
  inline Full1<binder1and2of3<Ternary,A1,A2> >
  operator()( const Ternary& f, const A1& a1, const A2& a2 ) const {
    return makeFull1( binder1and2of3<Ternary,A1,A2>(f,a1,a2) );
  }

  template <class Ternary, class A1>
    //typename Sig<Ternary, A1>::ResultType
  inline Full2<binder1of3<Ternary,A1> >
  operator()( const Ternary& f, const A1& a1 ) const {
    return makeFull2( binder1of3<Ternary,A1>(f,a1) );
  }

  template <class Ternary>
  inline Ternary
  operator()( const Ternary& f) const {
    bool b = &FunctoidTraits<Ternary>::template ensure_accepts<3>::args;
    //assert(FunctoidTraits<Ternary>::max_args == 3);
    assert(FunctoidTraits_max_args<Ternary>() == 3);
    return f;
  }
#ifdef FCPP_DEBUG
   std::string name() const {
     //if (fcpp::traits::debug_traits<Ternary>::has_name)
     //   return std::string("Curry3::")+fcpp::traits::debug_traits<Ternary>::name(f);
     //else
        return std::string("Curry3");
   }
#endif
};

struct Curry2 {
#ifndef FCPP_CONCEPTS
  template <class Binary, class A1 = void, class A2 = void>
  struct Sig
  : public FunType<Binary, A1, A2, Full0<binder1and2of2<Binary,A1,A2> > > {};

  template <class Binary, class A1>
  struct Sig<Binary, A1, void>
    : public FunType<Binary, A1, Full1<binder1of2<Binary,A1> > > {};

  template <class Binary>
  struct Sig<Binary, void, void>
    : public Full2<Binary> {};
#endif

  template <class Binary, class A1, class A2>
    //typename Sig<Binary, A1, A2>::ResultType
  inline Full0<binder1and2of2<Binary,A1,A2> >
  operator()( const Binary& f, const A1& a1, const A2& a2 ) const {
    return makeFull0( binder1and2of2<Binary,A1,A2>(f,a1,a2) );
  }

  template <class Binary, class A1>
    //typename Sig<Binary, A1>::ResultType
  inline Full1<binder1of2<Binary,A1> >
  operator()( const Binary& f, const A1& a1 ) const {
    return makeFull1( binder1of2<Binary,A1>(f,a1) );
  }

  template <class Binary>
    //typename Sig<Binary, A1>::ResultType
  inline Binary
  operator()( const Binary& f) const {
    bool b = &FunctoidTraits<Binary>::template ensure_accepts<2>::args;
    //assert(FunctoidTraits<Binary>::max_args == 2);
    assert(FunctoidTraits_max_args<Binary>() == 2);
   return f;
  }
#ifdef FCPP_DEBUG
   std::string name() const {
     //if (fcpp::traits::debug_traits<Binary>::has_name)
     //    return std::string("Curry2::")+fcpp::traits::debug_traits<Binary>::name(f);
     //else
        return std::string("Curry2");
   }
#endif
};

// This handles the case  curry1(function,arg)
// It does not handle the cases curry1(function) or curry1(function,_)
// which should return the function.
// Note that function(_) does work with EXTRA_STRONG_CURRY.
// The question is whether this matters enough to implement it.
// I want to make it so that curry(function, ...) can figure out the
// max no of args of function and act appropriately.
struct Curry1 /* : public SmartFunctoid1 */ {
#ifndef FCPP_CONCEPTS
  template <class Unary, class A1 = void>
  struct Sig : public FunType<Unary, A1, Full0<binder1of1<Unary,A1> > > {};

  template <class Unary>
  struct Sig<Unary, void>
    : public Full1<Unary> {};
#endif

  template <class Unary, class A1>
    //typename Sig<Unary, A1>::ResultType
    inline Full0<binder1of1<Unary,A1> >
  operator()( const Unary& f, const A1& a1 ) const {
    return makeFull0( binder1of1<Unary,A1>(f,a1) );
  }

  template <class Unary>
    //typename Sig<Binary, A1>::ResultType
  inline Unary
  operator()( const Unary& f) const {
    bool b = &FunctoidTraits<Unary>::template ensure_accepts<1>::args;
    //assert(FunctoidTraits<Unary>::max_args == 1);
    assert(FunctoidTraits_max_args<Unary>() == 1);
    return f;
  }

#ifdef FCPP_DEBUG
  //   template <class Unary>
   std::string name() const {
     //if (fcpp::traits::debug_traits<Unary>::has_name)
     //   return std::string("Curry1::")+fcpp::traits::debug_traits<Unary>::name(f);
     //else
        return std::string("Curry1");
   }
#endif
};

// This is not possible here because Full1 has been declared
// but not yet defined as full.h is only called after this file.
// (curry.h is included at the top of full.h)
//typedef Full1<impl::Curry1> Curry1;
//FCPP_MAYBE_NAMESPACE_OPEN
//FCPP_MAYBE_EXTERN Curry1 curry1;
//FCPP_MAYBE_NAMESPACE_CLOSE

// So if I want to make Curry1 etc full functoids
// I have to do this at the end of full.h
// See the comments on this at the end of the file.

// Local copies needed for generic Curry.
 Curry1 curry1;
 Curry2 curry2;
 Curry3 curry3;

// New attempt to make curry work on any no of args.
// This enables the following:
// curry(f0)
// curry(f1)  curry(f1,a1)
// curry(f2)  curry(f2,a1) curry(f2,a1,a2)
// curry(f3)  curry(f3,a1) curry(f3,a1,a2) curry(f3,a1,a2,a3)
// However it cannot itself be a functoid
// because it does not have a fixed number of arguments.
// This means there are some locations where curry1, curry2, curry3
// work but curry will not compile.
struct Curry {
   template <int i, class F> struct Helper;

  // These are all the same but I will leave them.
   template <class F>
   struct Helper<0,F> {
     typedef F Result;
      static inline Result go( const F& f )
      { return f; }
   };

   template <class F>
   struct Helper<1,F> {
     typedef F Result;
      static inline Result go( const F& f )
      { return f; }
   };

   template <class F>
   struct Helper<2,F> {
     typedef F Result;
      static inline Result go( const F& f )
      { return f; }
   };

   template <class F>
   struct Helper<3,F> {
     typedef F Result;
      static inline Result go( const F& f )
      { return f; }
   };

  template <int i, class F, class A1> struct Helper1;

  template <class F, class A1>
    struct Helper1<1,F,A1> {
#ifndef FCPP_CONCEPTS
    typedef typename Curry1::template Sig<F,A1>::ResultType Result;
#else
    typedef Full0<binder1of1<F,A1> > Result;
#endif
    static inline Result go( const F& f, const A1& a1 )
    { return curry1(f,a1); }
   };

  template <class F, class A1>
    struct Helper1<2,F,A1> {
#ifndef FCPP_CONCEPTS
    typedef typename Curry2::template Sig<F,A1>::ResultType Result;
#else
    typedef Full1<binder1of2<F,A1> > Result;
#endif
    static inline Result go( const F& f, const A1& a1 )
    { return curry2(f,a1); }
   };

  template <class F, class A1>
    struct Helper1<3,F,A1> {
#ifndef FCPP_CONCEPTS
    typedef typename Curry3::template Sig<F,A1>::ResultType Result;
#else
    typedef Full2<binder1of3<F,A1> > Result;
#endif
    static inline Result go( const F& f, const A1& a1 )
    { return curry3(f,a1); }
   };

  template <int i, class F, class A1, class A2> struct Helper2;

  template <class F, class A1, class A2>
    struct Helper2<2,F,A1,A2> {
#ifndef FCPP_CONCEPTS
    typedef typename Curry2::template Sig<F,A1,A2>::ResultType Result;
#else
    typedef Full0<binder1and2of2<F,A1,A2> > Result;
#endif
    static inline Result go( const F& f, const A1& a1, const A2& a2 )
    { return curry2(f,a1,a2); }
   };

  template <class F, class A1, class A2>
    struct Helper2<3,F,A1,A2> {
#ifndef FCPP_CONCEPTS
    typedef typename Curry3::template Sig<F,A1,A2>::ResultType Result;
#else
    typedef Full1<binder1and2of3<F,A1,A2> > Result;
#endif
    static inline Result go( const F& f, const A1& a1, const A2& a2 )
    { return curry3(f,a1,a2); }
   };

  template <int i, class F, class A1, class A2, class A3> struct Helper3;

  template <class F, class A1, class A2, class A3>
    struct Helper3<3,F,A1,A2,A3> {
#ifndef FCPP_CONCEPTS
    typedef typename Curry3::template Sig<F,A1,A2,A3>::ResultType Result;
#else
    typedef Full0<binder1and2and3of3<F,A1,A2,A3> > Result;
#endif
    static inline Result go( const F& f, const A1& a1, const A2& a2, const A3& a3 )
    { return curry3(f,a1,a2,a3); }
   };

#ifndef FCPP_CONCEPTS
  template <class F, class A1 = void, class A2 = void, class A3 = void>
  struct Sig : public
  FunType<F,typename Helper3<FunctoidTraits<F>::max_args,F,A1,A2,A3>::Result>
    {};

  template <class F, class A1, class A2> struct Sig<F,A1,A2,void> : public
  FunType<F,typename Helper2<FunctoidTraits<F>::max_args,F,A1,A2>::Result> {};

  template <class F, class A1> struct Sig<F,A1,void,void> : public
  FunType<F,typename Helper1<FunctoidTraits<F>::max_args,F,A1>::Result> {};

  template <class F> struct Sig<F,void,void,void> : public
  FunType<F,typename Helper<FunctoidTraits<F>::max_args,F>::Result> {};
#endif

   template <class F>
#ifndef FCPP_CONCEPTS
   inline typename Sig<F>::ResultType
#else
   inline typename Helper<FunctoidTraits<F>::max_args,F>::Result
#endif
   operator()( const F& f) const
   {
      return Helper<FunctoidTraits<F>::max_args,F>::go( f );
   }

  template <class F, class A1>
#ifndef FCPP_CONCEPTS
   inline typename Sig<F, A1>::ResultType
#else
     inline typename Helper1<FunctoidTraits<F>::max_args,F,A1>::Result
#endif
     operator()( const F& f, const A1& a1) const
   {
     return Helper1<FunctoidTraits<F>::max_args,F,A1>::go( f, a1 );
   }

  template <class F, class A1, class A2>
#ifndef FCPP_CONCEPTS
    inline typename Sig<F, A1, A2>::ResultType
#else
    inline typename Helper2<FunctoidTraits<F>::max_args,F,A1,A2>::Result
#endif
   operator()( const F& f, const A1& a1, const A2& a2) const
   {
     return Helper2<FunctoidTraits<F>::max_args,F,A1,A2>::go( f, a1, a2 );
   }

  template <class F, class A1, class A2, class A3>
#ifndef FCPP_CONCEPTS
    inline typename Sig<F, A1, A2, A3>::ResultType
#else
    inline typename Helper3<FunctoidTraits<F>::max_args,F,A1,A2,A3>::Result
#endif
    operator()( const F& f, const A1& a1, const A2& a2, const A3& a3) const
   {
     return Helper3<FunctoidTraits<F>::max_args,F,A1,A2,A3>::go( f,a1,a2,a3);
   }

#ifdef FCPP_DEBUG
   std::string name() const {
       return std::string("Curry");
   }
#endif
};

//////////////////////////////////////////////////////////////////////
// Finally, Curryable3 (what a huge beast)
//////////////////////////////////////////////////////////////////////

#ifdef FCPP_EXTRA_STRONG_CURRY
// This is the normal call - not usually needed like this.
template <class R, class F, class X>
struct Curryable3Helper0 {
   static inline R go( const F& f, const X& x) {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Curryable3helper0 calls binder1of2(f,x)" << std::endl;
#endif
     return makeFull2( binder1of3<F,X>(f,x) );
   }
};

template <class R, class F>
struct Curryable3Helper0<R,F,AutoCurryType> {
   static inline R go( const F& f, const AutoCurryType&) {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Curryable3Helper0 returns cycler3r<F>(f) STRONG" << std::endl;
#endif
     //   return f;
     return makeFull3(cycler3r<F>(f) );
   }
};
#endif

template <class R, class F, class X, class Y, class Z>
struct Curryable3Helper {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z ) {
      return f(x,y,z); 
   }
};

template <class R, class F, class X>
struct Curryable3Helper<R,F,X,AutoCurryType,AutoCurryType> {
   static inline R go( const F& f, const X& x, const AutoCurryType&,
                 const AutoCurryType& ) {
      return makeFull2( binder1of3<F,X>(f,x) );
   }
};

template <class R, class F, class Y>
struct Curryable3Helper<R,F,AutoCurryType,Y,AutoCurryType> {
   static inline R go( const F& f, const AutoCurryType&, const Y& y,
                 const AutoCurryType& ) {
      return makeFull2( binder2of3<F,Y>(f,y) );
   }
};

template <class R, class F, class Z>
struct Curryable3Helper<R,F,AutoCurryType,AutoCurryType,Z> {
   static inline R go( const F& f, const AutoCurryType&, const AutoCurryType&,
                 const Z& z ) {
      return makeFull2( binder3of3<F,Z>(f,z) );
   }
};

template <class R, class F, class Y, class Z>
struct Curryable3Helper<R,F,AutoCurryType,Y,Z> {
   static inline R go( const F& f, const AutoCurryType&, const Y& y,
                 const Z& z ) {
      return makeFull1( binder2and3of3<F,Y,Z>(f,y,z) );
   }
};

template <class R, class F, class X, class Z>
struct Curryable3Helper<R,F,X,AutoCurryType,Z> {
   static inline R go( const F& f, const X& x, const AutoCurryType&,
                 const Z& z ) {
      return makeFull1( binder1and3of3<F,X,Z>(f,x,z) );
   }
};

template <class R, class F, class X, class Y>
struct Curryable3Helper<R,F,X,Y,AutoCurryType> {
   static inline R go( const F& f, const X& x, const Y& y,
                 const AutoCurryType& ) {
      return makeFull1( binder1and2of3<F,X,Y>(f,x,y) );
   }
};

#ifdef FCPP_EXTRA_STRONG_CURRY
template <class R, class F>
struct Curryable3Helper<R,F,AutoCurryType,AutoCurryType,AutoCurryType> {
   static inline R go( const F& f, const AutoCurryType , const AutoCurryType&,
                       const AutoCurryType& ) {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Curryable3Helper returns f STRONG" << std::endl;
#endif
     return f;
     //return makeFull3( f );
   }
};

#endif

template <class R, class F, class X, class Y>
struct Curryable3Helper2 {
   static inline R go( const F& f, const X& x, const Y& y ) {
      return makeFull1( binder1and2of3<F,X,Y>(f,x,y) );
   }
};

template <class R, class F, class Y>
struct Curryable3Helper2<R,F,AutoCurryType,Y> {
   static inline R go( const F& f, const AutoCurryType&, const Y& y ) {
      return makeFull2(binder2of3<F,Y>(f,y));
   }
};

template <class R, class F, class X>
struct Curryable3Helper2<R,F,X,AutoCurryType> {
   static inline R go( const F& f, const X& x, const AutoCurryType& ) {
      return makeFull2(binder1of3<F,X>(f,x));
   }
};

#ifdef FCPP_EXTRA_STRONG_CURRY
template <class R, class F>
struct Curryable3Helper2<R,F,AutoCurryType,AutoCurryType> {
  static inline R go( const F& f, const AutoCurryType, const AutoCurryType& ) {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Curryable3Helper2 returns cycler3<F>(f) STRONG" << std::endl;
#endif
     //return f;
     return makeFull3( cycler3<F>(f) );
   }
};

#endif

template <class F>
class Curryable3 {
   const F f;
public:
   Curryable3( const F& ff ) : f(ff) {}

   template <class X, class Y=void, class Z=void>
   struct Sig
   : public FunType<typename F::template Sig<X,Y,Z>::Arg1Type,
                    typename F::template Sig<X,Y,Z>::Arg2Type,
                    typename F::template Sig<X,Y,Z>::Arg3Type,
                    typename RT<F,X,Y,Z>::ResultType> {};

   template <class X,class Y> struct Sig<X,Y,void> 
   : public FunType<X,Y,Full1<binder1and2of3<F,X,Y> > > {};

   template <class X> struct Sig<X,AutoCurryType,void> 
   : public FunType<X,AutoCurryType,Full2<binder1of3<F,X> > > {};

   template <class Y> struct Sig<AutoCurryType,Y,void> 
   : public FunType<AutoCurryType,Y,Full2<binder2of3<F,Y> > > {};

   template <class X> struct Sig<X,void,void> 
   : public FunType<X,Full2<binder1of3<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType> 
   : public FunType<X,AutoCurryType,AutoCurryType,
                    Full2<binder1of3<F,X> > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType> 
   : public FunType<AutoCurryType,Y,AutoCurryType,
                    Full2<binder2of3<F,Y> > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z> 
   : public FunType<AutoCurryType,AutoCurryType,Z,
                    Full2<binder3of3<F,Z> > > {};

   template <class X,class Z> struct Sig<X,AutoCurryType,Z> 
   : public FunType<X,AutoCurryType,Z,Full1<binder1and3of3<F,X,Z> > > {};

   template <class Y,class Z> struct Sig<AutoCurryType,Y,Z> 
   : public FunType<AutoCurryType,Y,Z,Full1<binder2and3of3<F,Y,Z> > > {};

   template <class X,class Y> struct Sig<X,Y,AutoCurryType> 
   : public FunType<X,Y,AutoCurryType,Full1<binder1and2of3<F,X,Y> > > {};

   template <class X,class Y>
   typename Sig<X,Y>::ResultType operator()( const X& x, const Y& y ) const {
      // need partial specialization, so defer to a class helper
      return Curryable3Helper2<typename Sig<X,Y>::ResultType,F,X,Y>::go(f,x,y);
   }
   template <class X>
   typename Sig<X>::ResultType operator()( const X& x ) const {
      return makeFull2(binder1of3<F,X>(f,x));
   }
   template <class X, class Y, class Z>
   inline typename Sig<X,Y,Z>::ResultType 
   operator()( const X& x, const Y& y, const Z& z ) const {
      // need partial specialization, so defer to a class helper
return Curryable3Helper<typename Sig<X,Y,Z>::ResultType,F,X,Y,Z>::go(f,x,y,z);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
      if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("Curryable3::")+fcpp::traits::debug_traits<F>::name(f);
      else
        return std::string("Curryable3");
   }
#endif
};


// FC++.1.5.2
#ifdef FCPP152
//////////////////////////////////////////////////////////////////////
// curryable4 (what a huge beast)
//////////////////////////////////////////////////////////////////////

// Temporary to sort out multiple needs to change it.
#define auto_curry_type AutoCurryType 

template <class R, class F, class X, class Y, class Z, class V>
struct Curryable4Helper {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z, const V& v ) {
#ifdef FCPP_PRINT_HELPERS
       std::cout << "Curryable4Helper called with 4 args" << std::endl;
#endif
       //        <<  x << " " << y 
       //         << " " << z << " " << v << std::endl;
      return f(x,y,z,v); 
   }
};

template <class R, class F, class X>
struct Curryable4Helper<R,F,X,AutoCurryType,AutoCurryType,AutoCurryType> {
   static R go( const F& f, const X& x, const AutoCurryType&, const AutoCurryType&, const AutoCurryType& ) {
      return makeFull3( binder1of4<F,X>(f,x) );
   }
};

template <class R, class F, class Y>
struct Curryable4Helper<R,F,AutoCurryType,Y,AutoCurryType,AutoCurryType> {
   static R go( const F& f, const AutoCurryType&, const Y& y, const AutoCurryType&, const AutoCurryType& ) {
      return makeFull3( binder2of4<F,Y>(f,y) );
   }
};

template <class R, class F, class Z>
struct Curryable4Helper<R,F,AutoCurryType,AutoCurryType,Z,AutoCurryType> {
   static R go( const F& f, const AutoCurryType&, const AutoCurryType&, const Z& z, const AutoCurryType& ) {
      return makeFull3( binder3of4<F,Z>(f,z) );
   }
};

template <class R, class F, class V>
struct Curryable4Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,V> {
   static R go( const F& f, const auto_curry_type&, const auto_curry_type&, const auto_curry_type&, const V& v ) {
      return makeFull3( binder4of4<F,V>(f,v) );
   }
};


template <class R, class F, class X, class Y>
struct Curryable4Helper<R,F,X,Y,auto_curry_type,auto_curry_type> {
   static R go( const F& f, const X& x, const Y& y, const auto_curry_type&, const auto_curry_type& ) {
      return makeFull2( binder1and2of4<F,X,Y>(f,x,y) );
   }
};

template <class R, class F, class X, class Z>
  struct Curryable4Helper<R,F,X,auto_curry_type,Z,auto_curry_type> {
    static R go( const F& f, const X& x, const auto_curry_type&, const Z& z,const auto_curry_type& ) {
      return makeFull2( binder1and3of4<F,X,Z>(f,x,z) );
   }
};

template <class R, class F, class X, class V>
  struct Curryable4Helper<R,F,X,auto_curry_type,auto_curry_type,V> {
    static R go( const F& f, const X& x, const auto_curry_type&, const auto_curry_type&, const V& v ) {
      return makeFull2( binder1and4of4<F,X,V>(f,x,v) );
   }
};


template <class R, class F, class Y, class Z>
struct Curryable4Helper<R,F,auto_curry_type,Y,Z,auto_curry_type> {
   static R go( const F& f, const auto_curry_type&, const Y& y, const Z& z, const auto_curry_type& ) {
      return makeFull2( binder2and3of4<F,Y,Z>(f,y,z) );
   }
};

template <class R, class F, class Y, class V>
  struct Curryable4Helper<R,F,auto_curry_type,Y,auto_curry_type,V> {
    static R go( const F& f, const auto_curry_type&, const Y& y, const auto_curry_type&, const V& v ) {
      return makeFull2( binder2and4of4<F,Y,V>(f,y,v) );
   }
};

 template <class R, class F, class Z, class V>
   struct Curryable4Helper<R,F,auto_curry_type,auto_curry_type,Z,V> {
     static R go( const F& f, const auto_curry_type&, const auto_curry_type& , const Z& z, const V& v) {
       return makeFull2( binder3and4of4<F,Z,V>(f,z,v) );
   }
};



template <class R, class F, class X, class Y, class Z>
struct Curryable4Helper<R,F,X,Y,Z,AutoCurryType> {
   static R go( const F& f, const X& x, const Y& y, const Z& z, const auto_curry_type& ) {
      return makeFull1( binder1and2and3of4<F,X,Y,Z>(f,x,y,z) );
   }
};

template <class R, class F, class X, class Y, class V>
struct Curryable4Helper<R,F,X,Y,AutoCurryType,V> {
   static R go( const F& f, const X& x, const Y& y, const auto_curry_type&, const V& v ) {
      return makeFull1( binder1and2and4of4<F,X,Y,V>(f,x,y,v) );
   }
};

template <class R, class F, class X, class Z, class V>
struct Curryable4Helper<R,F,X,AutoCurryType,Z,V> {
   static R go( const F& f, const X& x,  const auto_curry_type&,const Z& z, const V& v ) {
      return makeFull1( binder1and3and4of4<F,X,Z,V>(f,x,z,v) );
   }
};

template <class R, class F, class Y, class Z, class V>
struct Curryable4Helper<R,F,AutoCurryType,Y,Z,V> {
   static R go( const F& f, const auto_curry_type&, const Y& y, const Z& z, const V& v ) {
      return makeFull1( binder2and3and4of4<F,Y,Z,V>(f,y,z,v) );
   }
};


template <class R, class F, class X, class Y, class V>
struct Curryable4Helper2 {
   static R go( const F& f, const X& x, const Y& y, const V& v ) {
#ifdef FCPP_PRINT_HELPERS
      std::cout << "Curryable4Helper2  "
                << argument_traits_value(x) << " "
                << argument_traits_value(y) << " "
                << argument_traits_value(v) << std::endl;
#endif
      return makeFull1( binder1and2and3of4<F,X,Y,V>(f,x,y,v) );
   }
};


template <class R, class F, class X>
struct Curryable4Helper2<R,F,X,auto_curry_type,auto_curry_type> {
   static R go( const F& f, const X& x, const auto_curry_type&, const auto_curry_type) {
      return makeFull3(binder1of4<F,X>(f,x));
   }
};

template <class R, class F, class Y>
struct Curryable4Helper2<R,F,auto_curry_type,Y,auto_curry_type> {
   static R go( const F& f, const auto_curry_type&, const Y& y, const auto_curry_type&) {
      return makeFull3(binder2of4<F,Y>(f,y));
   }
};

template <class R, class F, class Z>
  struct Curryable4Helper2<R,F,auto_curry_type,auto_curry_type,Z> {
    static R go( const F& f, const auto_curry_type&, const auto_curry_type, const Z& z) {
      return makeFull3(binder3of4<F,Z>(f,z));
   }
};

template <class R, class F, class X, class Y>
struct Curryable4Helper2<R,F,X,Y,auto_curry_type> {
   static R go( const F& f, const X& x, const Y& y, const auto_curry_type&) {
      return makeFull2( binder1and2of4<F,X,Y>(f,x,y) );
   }
};

template <class R, class F, class X, class Z>
  struct Curryable4Helper2<R,F,X,auto_curry_type,Z> {
    static R go( const F& f, const X& x, const auto_curry_type&, const Z& z) {
      return makeFull2( binder1and3of4<F,X,Z>(f,x,z) );
   }
};

template <class R, class F, class Y, class Z>
struct Curryable4Helper2<R,F,auto_curry_type,Y,Z> {
   static R go( const F& f, const auto_curry_type&, const Y& y, const Z& z) {
      return makeFull2( binder2and3of4<F,Y,Z>(f,y,z) );
   }
};

template <class R, class F, class X, class Y>
struct Curryable4Helper3 {
   static R go( const F& f, const X& x, const Y& y) {
#ifdef FCPP_PRINT_HELPERS
      std::cout << "Curryable4Helper3" << std::endl;
#endif
      return makeFull2( binder1and2of4<F,X,Y>(f,x,y) );
   }
};

template <class R, class F, class X>
struct Curryable4Helper3<R,F,X,auto_curry_type> {
   static R go( const F& f, const X& x, const auto_curry_type&) {
      return makeFull3(binder1of4<F,X>(f,x));
   }
};

template <class R, class F, class Y>
struct Curryable4Helper3<R,F,auto_curry_type,Y> {
   static R go( const F& f, const auto_curry_type&, const Y& y) {
      return makeFull3(binder2of4<F,Y>(f,y));
   }
};


// Start to build this which has not been built before.
// This is incomplete - compare with Curryable3
// Use makeFull4.
template <class F>
class Curryable4 {
   const F f;
public:
   Curryable4( const F& ff ) : f(ff) {}

     template <class X, class Y=void, class Z=void, class W=void>
   struct Sig
       : public FunType<typename F::template Sig<X,Y,Z,W>::Arg1Type,
                        typename F::template Sig<X,Y,Z,W>::Arg2Type,
                        typename F::template Sig<X,Y,Z,W>::Arg3Type,
                        typename F::template Sig<X,Y,Z,W>::Arg4Type,
                        typename RT<F,X,Y,Z,W>::ResultType> {};
 
   template <class X, class Y, class Z, class W>
     inline typename Sig<X,Y,Z,W>::ResultType 
     operator()( const X& x, const Y& y, const Z& z, const W& w ) const {
      // need partial specialization, so defer to a class helper
     return Curryable4Helper<typename Sig<X,Y,Z,W>::ResultType,F,X,Y,Z,W>::go(f,x,y,z,w);
   }

};

///////////////////////////////////////////////////////////////////
// helpers for Full5, Full6, Full7 and Full8
///////////////////////////////////////////////////////////////////

#include "curry_helpers5.h"
#include "curry_helpers6.h"
#include "curry_helpers7.h"
#include "curry_helpers8.h"

#endif

} // end namespace impl

using impl::Curryable2;
using impl::Curryable3;
// FC++.1.5.2
#ifdef FCPP152
//typedef impl::Curry4 Thunk4;
//typedef impl::Curry3 Thunk3;
//typedef impl::Curry2 Thunk2;
//typedef impl::Curry1 Thunk1;
using impl::Curryable4;
using impl::Curryable5;
// See note below why these are being moved to full.h
//typedef Full7<impl::Curry6> Curry6;
//#ifndef FCPP_CONCEPTS
using impl::Curry7;
using impl::Curry6;
using impl::Curry5;
using impl::Curry4;
//#endif
#endif
  //#ifndef FCPP_CONCEPTS
using impl::Curry3;
using impl::Curry2;
using impl::Curry1;
using impl::Curry;
//#endif

#ifdef FCPP152
// These are used for bindAof5, bindXof5, bindXandYof5, bindXof6
// These declare the types of the binders.
using impl::Bind1;
using impl::Bind2;
using impl::Bind3;
using impl::Bind4;
using impl::Bind5;
using impl::Bind6;
using impl::Bind7;
// These are instances for use.
using impl::bind_first;
using impl::bind_second;
using impl::bind_third;
using impl::bind_fourth;
using impl::bind_fifth;
using impl::bind_sixth;
using impl::bind_seventh;
#endif

// FIX THIS: the CurryNs are not Full functoids.  Oh well.
// This can be fixed but the definitions have to be done in full.h
// after the FullN have been defined. They are not defined here.
// This is why all the definitions of the binders are there and not here.
// For the moment I have left the original ones here.
//////////////////////////////////////////////////////////////////////////////
// Comment from John Fletcher February 2008
// I have attempted to make the CurryN into full functoids when using concepts.
// I have run into problems with this and have reverted to non-full functoid
// behaviour for CurryN objects. This has cured some problems with CurryN and
// also with the binders which bind all the parameters to create a Full0 object.
// These are related to the CurryN and indeed called by a CurryN object
// when called with N arguements.  I was getting failure of function lookup at
// compile time and some wrong linking.
// I also think there is no point in makeing CurryN a functoid as the
// alternative of binding the required arguments is available via the
// binders and the methods using underscore for the unbound parameters.
// It also became clear during testing that there are multiple layers
// of indirection involved in this code. I am attempting to inline
// all the operators so that this is dealt with at compile time,
// in particular when optimizing when the code does reduce
// in size a lot.
//
// Example fcpptest6.cpp demonstrates all of this and more.
///////////////////////////////////////////////////////////////////////////////
//#ifndef FCPP_CONCEPTS
FCPP_MAYBE_EXTERN Curry  curry;   // "curry" was the same as "curry1"
FCPP_MAYBE_EXTERN Curry1 curry1;  // "curry" was the same as "curry1"
FCPP_MAYBE_EXTERN Curry2 curry2;
FCPP_MAYBE_EXTERN Curry3 curry3;
// FC++.1.5.2
#ifdef FCPP152
FCPP_MAYBE_EXTERN Curry4 curry4;
FCPP_MAYBE_EXTERN Curry5 curry5;
FCPP_MAYBE_EXTERN Curry6 curry6;
FCPP_MAYBE_EXTERN Curry7 curry7;
//FCPP_MAYBE_EXTERN Thunk1 thunk1;
//FCPP_MAYBE_EXTERN Thunk2 thunk2;
//FCPP_MAYBE_EXTERN Thunk3 thunk3;
//FCPP_MAYBE_EXTERN Thunk4 thunk4;
#endif
//#endif
// FIX THIS These are deprecated; use makeFullN instead
template <class F>
Curryable2<F> makeCurryable2( const F& f ) {
   return Curryable2<F>( f );
}
template <class F>
Curryable3<F> makeCurryable3( const F& f ) {
   return Curryable3<F>( f );
}

// Debug traits now in a separate file.

//#ifdef FCPP_DEBUG
// Includes argument traits needed without FCPP_DEBUG
#include "curry_debug.h"
//#endif

} // end namespace fcpp

#endif
