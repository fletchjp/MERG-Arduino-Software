//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_PRE_LAMBDA_DOT_H
#define FCPP_PRE_LAMBDA_DOT_H

// Some of the stuff here is needed as forward declarations for full.h.
// A lot of the stuff here is just handy helpers for lambda.h.  

#ifdef FCPP_ENABLE_LAMBDA
namespace fcpp {
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
  namespace impl {
    struct XInc;
  }
  typedef Full1<impl::XInc> Inc;
#endif
// Here is a 'cute' kludge to make "_*_" represent a 'nothing' argument
struct PlaceholderForZeroArguments {};
inline PlaceholderForZeroArguments operator*( AutoCurryType, AutoCurryType ) {
   return PlaceholderForZeroArguments();
}

namespace fcpp_lambda {

//////////////////////////////////////////////////////////////////////
// A lot of small helpers that will be useful throughout lambda...
//////////////////////////////////////////////////////////////////////
template <class T, class U>
struct ImplicitlyConvertibleHelper {
   typedef char Small;
   struct Big { char dummy[2]; };
   static Small Test( const U );
   static Big   Test(...);
   static const T MakeT();
};
template <class T, class U>   // T converts to U
struct ImplicitlyConvertible {
   typedef ImplicitlyConvertibleHelper<T,U> H;
   //static const int lhs = sizeof(H::Test((const T) H::MakeT()));
   static const int lhs = sizeof(H::Test((T) H::MakeT()));
   static const int rhs = sizeof(typename H::Small);
   static const bool value = (lhs==rhs);
};
//////////////////////////////////////////////////////////////////////
// See operator, overloads in lambda.h for namespace explanation
namespace exp {
   struct NIL {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("NIL");
   }
#endif
   };
   template <class H, class T> struct CONS {
      H head;
      T tail;
      CONS( const H& h ) : head(h) {}
      CONS( const H& h, const T& t ) : head(h), tail(t) {}
   };
}
using exp::NIL;
using exp::CONS;
//////////////////////////////////////////////////////////////////////
template <class LA, class LB> struct AppendList;
template <class LB> struct AppendList<NIL,LB> { 
   typedef LB Result; 
   static inline Result go( const NIL&, const LB& x ) { return x; }
};
template <class H, class T, class LB> struct AppendList<CONS<H,T>,LB> { 
   typedef CONS<H,typename AppendList<T,LB>::Result> Result; 
   static inline Result go( const CONS<H,T>& x, const LB& y ) 
   { return Result( x.head, AppendList<T,LB>::go(x.tail,y) ); }
};
//////////////////////////////////////////////////////////////////////
template <class L> struct Length;
template <> struct Length<NIL> { static const int value = 0; };
template <class H, class T> struct Length< CONS<H,T> >
{ static const int value = 1 + Length<T>::value; };
//////////////////////////////////////////////////////////////////////
// remove every T element from the list A
template <class A, class T> struct Remove;
template <class T> struct Remove<NIL,T> { typedef NIL Result; };
template <class AH, class AT, class T> struct Remove<CONS<AH,AT>,T> 
{ typedef CONS<AH,typename Remove<AT,T>::Result> Result; };
template <class AT, class T> struct Remove<CONS<T,AT>,T> 
{ typedef typename Remove<AT,T>::Result Result; };
///////////////////////////////////////////////////////////////////////
// for each element in the list B, remove that element from list A
template <class A, class B> struct ListDifference;
template <class A> struct ListDifference<A,NIL> { typedef A Result; };
template <class A, class T, class Rest> 
struct ListDifference<A,CONS<T,Rest> > {
   typedef typename Remove<A,T>::Result APrime;
   typedef typename ListDifference<APrime,Rest>::Result Result;
};
//////////////////////////////////////////////////////////////////////
template <class L> struct RemoveDuplicates;
template <> struct RemoveDuplicates<NIL> { typedef NIL Result; };
template <class H, class Rest> struct RemoveDuplicates<CONS<H,Rest> > 
{ typedef CONS<H,typename Remove<Rest,H>::Result> Result; };
//////////////////////////////////////////////////////////////////////
struct LEBase {};     // Base type for all LEs
//////////////////////////////////////////////////////////////////////
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
namespace exp { 
   template <int i> class LambdaVar; 
   template <typename T> class LambdaType;
}
using exp::LambdaVar;
using exp::LambdaType;
using fcpp::impl::XInc;
#ifdef FCPP_LAMBDA_AUTOCURRY
namespace exp {
  class AutoCurry;
}
 using exp::AutoCurry;
#endif 
#endif

template <class T, bool b> struct EnsureLEHelper 
{ static inline void go() 
  {
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
      if (is_same_type<LambdaVar<1>,T>::value) 
        std::cout << "T is LambdaVar<1> in EnsureLEHelper" << std::endl;
      if (is_same_type<LambdaVar<2>,T>::value) 
        std::cout << "T is LambdaVar<2> in EnsureLEHelper" << std::endl;
      if (is_same_type<LambdaVar<4>,T>::value) 
        std::cout << "T is LambdaVar<4> in EnsureLEHelper" << std::endl;
      if (is_same_type<LambdaType<Inc>,T>::value) 
        std::cout << "T is LambdaType<Inc> > in EnsureLEHelper" << std::endl;
#ifdef FCPP_LAMBDA_AUTOCURRY
      if (is_same_type<AutoCurry,T>::value) 
        std::cout << "T is AutoCurry in EnsureLEHelper" << std::endl;
#endif
#endif
  } 
};
template <class T> struct EnsureLEHelper<T,false> {};
template <class T> struct EnsureLE { 
   static inline void go() 
   { 
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
      std::cout << "EnsureLE.go called" << std::endl;
      if (is_same_type<LambdaVar<1>,T>::value) 
        std::cout << "T is LambdaVar<1> in EnsureLE" << std::endl;
      if (is_same_type<LambdaVar<2>,T>::value) 
        std::cout << "T is LambdaVar<2> in EnsureLE" << std::endl;
      if (is_same_type<LambdaVar<4>,T>::value) 
        std::cout << "T is LambdaVar<4> in EnsureLE" << std::endl;
      if (is_same_type<AutoCurryType,T>::value) 
        std::cout << "T is AutoCurryType in EnsureLE" << std::endl;
      if (is_same_type<LambdaType<Inc>,T>::value) 
        std::cout << "T is LambdaType<Inc> > in EnsureLE" << std::endl;
#ifdef FCPP_LAMBDA_AUTOCURRY
      if (is_same_type<AutoCurry,T>::value) 
        std::cout << "T is AutoCurry in EnsureLE" << std::endl;
#endif
#endif
      EnsureLEHelper<T,Inherits<T,LEBase>::value>::go(); 
   } 
};
//////////////////////////////////////////////////////////////////////
template <class LEL> struct EnsureLEList;
template <> struct EnsureLEList<NIL> { static inline void go() {} };
template <class H, class T> struct EnsureLEList<CONS<H,T> > 
{ static inline void go() { EnsureLE<H>::go(); EnsureLEList<T>::go(); } };
//////////////////////////////////////////////////////////////////////
template <int i, class TypeThunk> struct TEPair {
   static const int my_lv = i;
   typedef TypeThunk MyTypeThunk;
};
// TE is a type environment; a list of TEPair<i,T>
template <int i, class LE> struct BEPair { 
   static const int my_lv = i;
   typedef LE MyLE;
   LE value;  
   BEPair( const LE& x ) : value(x) { EnsureLE<LE>::go(); } 
};
// BE is a value environment; a list of BEPair<i,LE>
//////////////////////////////////////////////////////////////////////
template <class LEL> struct AccumFreeVars;
template <> struct AccumFreeVars<NIL> { typedef NIL Result; };
template <class H, class T> struct AccumFreeVars<CONS<H,T> > { 
   typedef typename AppendList<typename H::FreeVars,
      typename AccumFreeVars<T>::Result>::Result Result; 
};
//////////////////////////////////////////////////////////////////////
// forward decls
namespace exp { 
  //#ifdef FCPP_USE_BOOST_MPL_MESSAGES
  // template <int i> class LambdaVar; 
  //#endif
   template <class T> struct Value; 
   template <class Fun, class Args> struct Call;
}
//////////////////////////////////////////////////////////////////////
template <int i, class LE> struct Binder { static const int lvnum = i; 
   LE exp; Binder( const LE& e ) : exp(e) { EnsureLE<LE>::go(); } };
//////////////////////////////////////////////////////////////////////
template <class BL> struct EnsureBinderList;
template <> struct EnsureBinderList<NIL> { static inline void go() {} };
template <int i, class LE, class Rest> 
struct EnsureBinderList<CONS<Binder<i,LE>,Rest> > 
{ static inline void go() { EnsureBinderList<Rest>::go(); } };
//////////////////////////////////////////////////////////////////////
template <int i, class LE> struct Gets { static const int lvnum = i; 
   LE exp; Gets( const LE& e ) : exp(e) { EnsureLE<LE>::go(); } };
//////////////////////////////////////////////////////////////////////
template <class X, class Y> struct AlwaysFirst { typedef X Type; };
//////////////////////////////////////////////////////////////////////
typedef enum { IfNormal, IfTrue, IfFalse } IfKind;
//////////////////////////////////////////////////////////////////////

#ifdef FCPP_USE_BOOST_MPL_MESSAGES
 namespace impl {
   struct XInc;
 }
#endif

template <bool b, class T>
struct LEifyHelper {
   typedef T Type;
   static inline Type go( const T& x ) 
   { 
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
     if (is_same_type<impl::XInc,T>::value) 
        std::cout << "T is XInc in LEifyHelper<true>" << std::endl;
     if (is_same_type<LambdaVar<1>,T>::value) 
        std::cout << "T is LambdaVar<1> in LEifyHelper<true>" << std::endl;
     if (is_same_type<LambdaVar<2>,T>::value) 
        std::cout << "T is LambdaVar<2> in LEifyHelper<true>" << std::endl;
     if (is_same_type<LambdaVar<4>,T>::value) 
        std::cout << "T is LambdaVar<4> in LEifyHelper<true>" << std::endl;
     if (is_same_type<AutoCurryType,T>::value) 
        std::cout << "T is AutoCurryType in LEifyHelper<true>" << std::endl;
     if (is_same_type<LambdaType<Inc>,T>::value) 
        std::cout << "T is LambdaType<Inc> > in LEifyHelper<true>" << std::endl;
#ifdef FCPP_LAMBDA_AUTOCURRY
      if (is_same_type<AutoCurry,T>::value) 
        std::cout << "T is AutoCurry in LEifyHelper<true>" << std::endl;
#endif
#endif
      return x; 
   }
};
template <class T>
struct LEifyHelper<false,T> {
   typedef exp::Value<T> Type;
   static inline Type go( const T& x ) 
   { 
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
     if (is_same_type<impl::XInc,T>::value) 
        std::cout << "T is XInc in LEifyHelper<false>" << std::endl;
     if (is_same_type<LambdaVar<1>,T>::value) 
        std::cout << "T is LambdaVar<1> in LEifyHelper<false>" << std::endl;
     if (is_same_type<LambdaVar<2>,T>::value) 
        std::cout << "T is LambdaVar<2> in LEifyHelper<false>" << std::endl;
     if (is_same_type<LambdaVar<4>,T>::value) 
        std::cout << "T is LambdaVar<4> in LEifyHelper<false>" << std::endl;
     if (is_same_type<AutoCurryType,T>::value) {
        std::cout << "T is AutoCurryType in LEifyHelper<false>" << std::endl;
     if (is_same_type<LambdaType<Inc>,T>::value) 
        std::cout << "T is LambdaType<Inc> > in LEifyHelper<false>" << std::endl;
        //return x; // Idea doesn't seem to make any difference.
     }
#ifdef FCPP_LAMBDA_AUTOCURRY
      if (is_same_type<AutoCurry,T>::value) 
        std::cout << "T is AutoCurry in LEifyHelper<false>" << std::endl;
#endif
#endif
       return Type(x); 
   }
};
template <class T>
struct LEify {
   static const bool b = Inherits<T,LEBase>::value;
   typedef typename LEifyHelper<b,T>::Type Type;
   static inline Type go( const T& x ) { 
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
     if (is_same_type<impl::XInc,T>::value) 
        std::cout << "T is XInc in LEify" << std::endl;
     if (is_same_type<LambdaVar<1>,T>::value) 
        std::cout << "T is LambdaVar<1> in LEify" << std::endl;
     if (is_same_type<LambdaVar<2>,T>::value) 
        std::cout << "T is LambdaVar<2> in LEify" << std::endl;
     if (is_same_type<LambdaVar<4>,T>::value) 
        std::cout << "T is LambdaVar<4> in LEify" << std::endl;
     if (is_same_type<AutoCurryType,T>::value) 
        std::cout << "T is AutoCurryType in LEify" << std::endl;
     if (is_same_type<LambdaType<Inc>,T>::value) 
        std::cout << "T is LambdaType<Inc> > in LEify" << std::endl;
#ifdef FCPP_LAMBDA_AUTOCURRY
      if (is_same_type<AutoCurry,T>::value) 
        std::cout << "T is AutoCurry in LEify" << std::endl;
#endif
#endif
      return LEifyHelper<b,T>::go(x); 
   }
};


//////////////////////////////////////////////////////////////////////
// 4 cases: normal value (LE or otherwise), _*_, NIL, CONS
template <class T> struct LEListify {
   typedef typename LEify<T>::Type LE;
   typedef CONS<LE,NIL> Type;
   static inline Type go( const T& x ) 
   { 
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
     if (is_same_type<impl::XInc,T>::value) 
        std::cout << "T is XInc in LEListify" << std::endl;
     if (is_same_type<LambdaVar<1>,T>::value) 
        std::cout << "T is LambdaVar<1> in LEListify" << std::endl;
     if (is_same_type<LambdaVar<2>,T>::value) 
        std::cout << "T is LambdaVar<2> in LEListify" << std::endl;
     if (is_same_type<LambdaVar<4>,T>::value) 
        std::cout << "T is LambdaVar<4> in LEListify" << std::endl;
     if (is_same_type<AutoCurryType,T>::value) 
        std::cout << "T is AutoCurryType in LEListify" << std::endl;
     if (is_same_type<LambdaType<Inc>,T>::value) 
        std::cout << "T is LambdaType<Inc> > in LEListify" << std::endl;
#ifdef FCPP_LAMBDA_AUTOCURRY
      if (is_same_type<AutoCurry,T>::value) 
        std::cout << "T is AutoCurry in LEListify" << std::endl;
#endif
#endif
      return Type( LEify<T>::go(x) ); 
   }
};
template <> struct LEListify<PlaceholderForZeroArguments> {
   typedef NIL Type;
   static inline Type go( const PlaceholderForZeroArguments& ) 
   { return Type(); }
};
template <> struct LEListify<NIL> {
   typedef NIL Type;
   static inline Type go( const NIL& ) { return Type(); }
};
/*
#ifdef FCPP_CONCEPTS
// Guess as to what to do next....
// This is a first attempt to correct the behaviour with 
// AutoCurry - not a success.
template <> struct LEListify<AutoCurry> {
  //typedef AutoCurry Type;
   typedef LEify<AutoCurry>::Type LE;
   typedef CONS<LE,NIL> Type;
   static inline Type go( const AutoCurry& x ) { 
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
     if (is_same_type<AutoCurry,Type>::value) 
      std::cout << "Type is AutoCurry in LEListify<AutoCurry>" << std::endl;
#endif
      return Type( LEify<AutoCurry>::go(x)); 
   }
};
#endif
*/
template <class LE, class Rest> struct LEListify<CONS<LE,Rest> > {
   typedef CONS<LE,Rest> Type;
   static inline Type go( const Type& x ) 
   { 
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
      if (is_same_type<impl::XInc,Type>::value) 
        std::cout << "Type is XInc in LEListify<CONS>" << std::endl;
      if (is_same_type<LambdaVar<1>,Type>::value) 
        std::cout << "Type is LambdaVar<1> in LEListify<CONS>" << std::endl;
      if (is_same_type<LambdaVar<2>,Type>::value) 
        std::cout << "Type is LambdaVar<2> in LEListify<CONS>" << std::endl;
      if (is_same_type<LambdaVar<4>,Type>::value) 
        std::cout << "Type is LambdaVar<4> in LEListify<CONS>" << std::endl;
      if (is_same_type<AutoCurryType,Type>::value) 
        std::cout << "Type is AutoCurryType in LEListify<CONS>" << std::endl;
      if (is_same_type<LambdaType<Inc>,Type>::value) 
        std::cout << "Type is LambdaType<Inc> > in LEListify<CONS>" << std::endl;
#endif
      EnsureLEList<Type>::go(); return x; 
   }
};
//////////////////////////////////////////////////////////////////////
template <class B> struct BinderListify;
template <int i, class LE> struct BinderListify<Binder<i,LE> > {
   typedef CONS<Binder<i,LE>,NIL> Type;
   static inline Type go( const Binder<i,LE>& b ) { return Type(b); }
};
template <class B, class Rest> struct BinderListify<CONS<B,Rest> > {
   typedef CONS<B,Rest> Type;
   static inline Type go( const Type& x ) { return x; }
};
//////////////////////////////////////////////////////////////////////
template <class This, class Arg>
struct BracketCallable {
   typedef typename LEify<This>::Type ThisLE;
   typedef exp::Call<ThisLE,typename LEListify<Arg>::Type> Result;
   static inline Result go( const This& me, const Arg& arg ) 
	{   //std::cout << "BracketCallable::go called" << std::endl;
      return Result( LEify<This>::go(me), LEListify<Arg>::go(arg) ); }
};
//////////////////////////////////////////////////////////////////////
template <bool b, class List> struct FilterHelp;
template <class H, class T> struct FilterHelp<true,CONS<H,T> > {
   typedef CONS<H,T> Result;
   static inline Result go( const CONS<H,T>& x ) { return x; }
};
template <class H, class T> struct FilterHelp<false,CONS<H,T> > {
   typedef T Result;
   static inline Result go( const CONS<H,T>& x ) { return x.tail; }
};
template <class PredThunk, class List> struct Filter;
template <class PT> struct Filter<PT,NIL> {
   typedef NIL Result;
   static inline Result go( const NIL& x ) { return x; }
};
template <class PT, class H, class T> struct Filter<PT,CONS<H,T> > {
   static const bool b = PT::template Go<int,H>::value;
   typedef FilterHelp<b,CONS<H,T> > Help;
   typedef typename Help::Result Result;
   static inline Result go( const CONS<H,T>& x ) { return Help::go(x); }
};
//////////////////////////////////////////////////////////////////////
template <class List, class E> struct Contains;
template <class E> struct Contains<NIL,E> 
{ static const bool value = false; };
template <class H, class T, class E> struct Contains<CONS<H,T>,E> 
{ static const bool value = Contains<T,E>::value; };
template <class T, class E> struct Contains<CONS<E,T>,E> 
{ static const bool value = true; };
//////////////////////////////////////////////////////////////////////
template <class F, class List> struct Map;
template <class F> struct Map<F,NIL> { 
   typedef NIL Result;
   static inline Result go( const NIL& x ) { return x; }
};
template <class F, class H, class T> struct Map<F,CONS<H,T> > { 
   typedef CONS<typename F::template Go<H>::Result,
                typename Map<F,T>::Result> Result;
   static inline Result go( const CONS<H,T>& x ) {
      return Result( F::template Go<H>::go( x.head ),
                     Map<F,T>::go( x.tail ) );
   }
};
//////////////////////////////////////////////////////////////////////
template <class Op, class E, class List> struct Foldr;
template <class Op, class E> struct Foldr<Op,E,NIL> 
{ typedef E Result; };
template <class Op, class E, class H, class T> struct Foldr<Op,E,CONS<H,T> > { 
   typedef typename Foldr<Op,E,T>::Result Tmp;
   typedef typename Op::template Go<H,Tmp>::Result Result;
};
//////////////////////////////////////////////////////////////////////
   
} // end namespace fcpp_lambda
} // end namespace fcpp

#endif
#endif
