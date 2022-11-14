//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FULL_DOT_H
#define FCPP_FULL_DOT_H

#include "smart.h"
#include "curry.h"
#include "pre_lambda.h"
#ifdef FCPP_DEBUG
#include "debug_traits.h"
#endif

//////////////////////////////////////////////////////////////////////
// FCPP_CONCEPTS implemented as an option.
// Implemented in Full0 to Full5 (including some currying)
// Full0 - special as Sig is via CFunType, unchanged.
// Full1 - no Sig
// Full2 - no Sig, currying implemented using concept maps. (tests in fcpptest2.cpp)
// Full3 - no Sig, currying implemented using concept maps. (tests in fcpptest4.cpp)
// Full4 - no Sig, currying implemented using concept maps. (tests in fccptest4.cpp)
// Full5 - no Sig, currying implemented using concept maps. (tests in fcpptest5.cpp)
//                 (There are some deficiencies in Full5 currying.)
// Full6 - no Sig, some implementation using concept maps (tests in fcpptest6.cpp)
// Full7 - no Sig, some implementation using concept maps (tests in fcpptest7.cpp)
// Full8 - no Sig, some implementation using concept maps (tests in fcpptest7.cpp)
//
// Notes: (An attempt to gather all information 
//         relating to concepts from this and other files)
//
// 1.  Each currying implementation requires a concept map which specialises
//     the concept defined in concepts.h. This is equivalent to specialising the
//     Sig except that as it is not internal to the class it is also available
//     for use by the corresponding Fun object in function.h.
//
// 2.  The objects which are defined elsewhere and use the Full definitions 
//     do not need to be modified to work with the new scheme UNLESS they
//     use the Sig explicitly to define the return type from operator().
//     Some do this but most do not.  When these are modified all the Sig
//     definitions can be converted to be NOT defined when FCPP_CONCEPTS is defined.
//
// 3.  The process of conversion has thrown up a few bugs in the original 
//     FC++.1.5 implementation, mostly only partial implementation of autocurrying
//     in Fun3 (and also in Fun4, which is not part of the original 1.5).
//
// 4.  All the binder functions (in curry.h) have Sigs which are being dealt with 
//     as well to complete the activity. These use the same concept maps
//     from full.h and some additional ones where needed for the binders of type 
//     bindAof and bindXof.  These were in curry.h, now spilt up, see curry.h.
//
// 5.  ALL CONCEPT MAPS have to be in the same namespace as the concept which they map.
//     Here that is fcpp and not fcpp::impl which is used 
//     for the implementation of binders.
//
// 6.   All the binders up to and including N=4 have been modified
//      to use concepts instead of the Sig when FCPP_CONCEPTS is defined.
// 
// 7.   This action is now also complete for N=5 including bindAof5,
//      bindXof5 and bindXandYof5 and a start has been made on N=6 and N=7. 
// 
// 8.   Binders for 5 arguments are being completed using new 
//      bindXandYandZof5 and bindWandXandYandZof5, which work 
//      for direct calls with and without concepts.
//      This leads to some simplification to the 2, 3 and 4 parameter 
//      curried calls in Full5. 
//
// 9.   Similarly new binders bindWandXandYandZof6 and bindVandWandXandYandZof6 
//      have been implemented, which work for direct calls with and without concepts.
//
// 10.  Similar new binders bindXandYof7, bindXandYandZof7, 
//      bindWandXandYandZof7 and bindVandWandXandYandZof7 are being implemented, 
//      which work for direct calls with and without concepts.
//
// 11.  Full6 and Full7 are being extended to use the new binders.
//      Fun6 and Fun7 are also being extended. Both now work for currying for all 
//      combinations of the first three parameters.
//
// 12.  To support this work the Const callable concept has been extended 
//      to 14 parameters. All these concepts have a default parameter defined 
//      for all parameters after the first, to support the variable number 
//      of parameters occurring in currying.
//
// 13.  See curry.h for a description of the breakdown of that file 
//      into smaller files. This was because it had got so big 
//      that it was inconvenient to edit it.
// 
// 14.  function.h has also been subdivided by putting Fun6 and Fun7 in separate files.
//
// 15.  Full8 has been started in order to be able to bind all 7 arguments in 
//      Full7 and Fun7.
//
// 16.  Full7 and Fun7 now work for all cases of binding the leading parameters.
//
// 17.  Restored Sigs in Full1 and Full2 in order to work with transfoldc.cpp
//      This eliminates most of the errors with Map but not all of them.
//      That required restoration of Sig in binder1of2 and binder2of2.
//      Clearly List and Map need a lot more work to work Sig free with concepts.
// 
// 18.  April 2008. Work now covered by use of svn to log all changes.
//      Removed Sigs in Full1 and Full2 as a result of work on numerous things.
//      testcontm.cpp works fully without them.
//
// 19.  May 2008. Added extra code as part of the work on the lambda problem.
//      This allows extra curry cases (_) with Full1, (_) and (_,_) with Full2
//      controlled by defining FCPP_EXTRA_STRONG_CURRY. 
//      [r101] - refs like this are to the relevant svn update
//
// 20.  May 2008. Added code so that strong curry (see 19) now works with 
//      concepts for Full1 and Full2. Initially the Full1 version did not work.
//      Some old code causing this has been removed and all now works.
//      Also corrected Flip to work with concepts - not yet tested.
//      [r111]
//
// 21.  October 2008. Work resumed adding in more currying examples in Full7 and Fun7.
//      These examples now work with the concepts development compiler as well 
//      as the previous version (boost_con). [r163][r168][r175]
//
//      Note: Each added autocurry combination needs a new helper specialisation in 
//      curry_helpers7.h as well as a Sig in Full7 and a concept map before Full7.
//      If a new binding to a binder of the type with XYZ etc in the name is needed, 
//      then new entries in curry_binders7.h and curry_concept_maps.h are also needed.
//      The test file fcpptest7.cpp is given added tests of Full7 and 
//      Fun7 autocurrying. If these work for all three compilers 
//      then the new element is considered complete.
//      The new helper function is calling to a particular binder function. 
//      All the binders needed are now implemented for Full7 
//      although some have yet to be tested and do not cover all 
//      combinations of parameters.
//      Further code is needed in curry_binders7.h  and curry_concept_maps.h.
//      There is a full list of binder names at the end of this file 
//      in the external defintion
//
// 22.  November 2008. Discussion about C++0x and templates has raised the 
//      question of the use of swap in FC++. There is none at all, 
//      apart from swap members in the pointer 
//      reference class IRef, which do not seem to be used anywhere. 
//      I have added swap as a class member function in Fun1 and Fun2 
//      (see function.h) following the same method
//      as that used in boost function (see boost/function/function_template.hpp). 
//      I have generalized this to swap two polymorphic functoids with 
//      the same instantiation template e.g. examples of plus and minus. 
//      This needs a swap functoid which would be 
//      different from the others in that its arguments would be references 
//      rather than const references. See example at the end of operator.h. 
//      which does not work.
//      This is achieved in boost::bind by using explicit ref(x)
//      in the argument call.
//      Swap member does not work in Full2 as the different objects 
//      have different types.
//
// 23.  March 2009 Edited comments here. [r180]
// 
// SPECIAL NOTE: The repository is located on usbdrive-files and there is a link
//      to this from my default home directory. [r181]
//
// 24.  Added some new concept maps to Full6 to complete single parameter curry
//      cases [r184]. Tests are in fcpptest6.cpp and run for four compilers.
//
// 25.  hUncurry (prelude.h) has a concept map but retains a Sig. Why?
//
// 26.  Added some new cases to Full6 to add two actual parameter hashes for 
//      4 and 5 parameter calls, concentrating on ones without trailing _.
//      One case to start use of BindXandYandZof6, which includes additions
//      in both binders and helpers. [r186]
//  
// 27.  Sorted out incomplete conditional printing in curry_helpers5.h. [r192]
//
// 28.  Identifying some deficiencies in Full5 currying.
//      Use of two binders where one will now do and some concept maps missing. 
// 
// 29.  Adding and correcting 3 parameter currying for Full5. [r196] 
//      Completed Full5 currying [r198]
//
// 30.  The time has come to break up full.h which is now very large.
//      I will put Full6 into full6.h and Full7 into full7.h. 
//
//      This work is incomplete.   
// 
//      John Fletcher    January 2008 - March 2009
//////////////////////////////////////////////////////////////////////

namespace fcpp {

//////////////////////////////////////////////////////////////////////
// Full functoids
//////////////////////////////////////////////////////////////////////
// Just as Curryable2/Curryable3 serve as wrappers which decorate
// functoids to give them 'curryability', FullN are wrappers to give
// functoids _all_ the 'features' that functoids can have.  The current
// "extra feature set" for functoids is
//   - curryability     (ability to call with fewer args)
//   - lambda-awareness (operator[] for use inside lambda)
//   - smartness        (inherited typedefs to answer introspective questions)
// The FullN classes just combine all of the features into one
// uber-wrapper which does it all.

// Don't forget that any features added here need to be added to 
// the indirect functoids in function.h, too.
// And also to the Uncurryable class in prelude.h.
// FIX THIS take a good look at Uncurry; it may have pre-dated smartness
// and can now be written more sanely...?
template <class F>
class Full0 
: public SmartFunctoid0
#ifndef FCPP_CONCEPTS
, public CFunType<typename RT<F>::ResultType> 
#endif
{
   F f;
public:
   Full0() : f() {}
   Full0( const F& ff ) : f(ff) {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Full0(ff) called" << std::endl;
#endif
   }
#ifdef FCPP_ENABLE_LAMBDA
   typedef Full0 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
#ifdef FCPP_CONCEPTS
   //  requires ConstCallable0<F>
  inline ConstCallable0<F>::result_type
#else
   inline typename RT<F>::ResultType
#endif
   operator()() const {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "f() called" << std::endl;
#endif
      return f();
   }
#ifdef FCPP_DEBUG
   std::string name() const { 
      if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("Full0::")+fcpp::traits::debug_traits<F>::name(f); 
      else
        return std::string("Full0"); 
   }
#endif
};

// Full1 and Full2 now do not have Sig members 
// when FCPP_CONCEPTS is defined.

#ifdef FCPP_EXTRA_STRONG_CURRY
#ifdef FCPP_CONCEPTS

// This enables f(_) to work -> f
template <class F>
concept_map ConstCallable1<F,AutoCurryType>
{
  // This change removes an extra level of call to helpers when using concepts.
  // The same change made in Full2.
  typedef F result_type;
  //typedef Full1<F> result_type;
};

#else
// Note that some extra helpers are needed in curry.h as well.
// This code allows cases like f(_) and f(_,_)

// This helper is needed because the code will not work inside Full2.
template <class F,class T>
struct Full1SigHelper 
   : public FunType<typename F::template Sig<T>::Arg1Type,
                    typename RT<F,T>::ResultType> {};

// This enables f(_) to work -> f
template <class F>
struct Full1SigHelper <F,AutoCurryType> : public FunType<AutoCurryType,Full1<F> >
{};   

#endif
#endif


template <class F>
class Full1 : public SmartFunctoid1 {
   F f;
public:
   Full1() : f() {}
   Full1( const F& ff ) : f(ff) {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     // This is a different way to use the name() members.
     std::cout << "Full1(" ;
     if (fcpp::traits::debug_traits<F>::has_name) {
       std::cout << fcpp::traits::debug_traits<F>::name(f);
     } else {
       std::cout << "f";
     }
     std::cout << ") called." << std::endl;
#else
     std::cout << "Full1(ff) called" << std::endl;
#endif
#endif
   }
#ifdef FCPP_ENABLE_LAMBDA
   typedef Full1 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif

   // Experimental swap routine following boost function
   // See boost/function/function_template.hpp
   /* void swap(F &ff) {
     if(f == ff) return;
     F temp = f;
     f = ff;
     ff = temp;
   } 
   */
#ifndef FCPP_CONCEPTS
#ifdef FCPP_EXTRA_STRONG_CURRY
   // This allows cases like f(_) and f(_,_)
   template <class T>
     struct Sig : public Full1SigHelper<F,T>
   {};

#else
   template <class T> struct Sig 
      : public FunType<typename RT<F,T>::Arg1Type,
                       typename RT<F,T>::ResultType> {};
#endif
#endif

#ifdef FCPP_EXTRA_STRONG_CURRY
   template <class X>
#ifdef FCPP_CONCEPTS
   inline ConstCallable1<F,X>::result_type
#else
   inline typename Sig<X>::ResultType 
#endif
   operator()( const X& x ) const {
#ifdef FCPP_PRINT_HELPERS
      std::cout << "Curryable1Helper0 called" << std::endl;
#endif
      // Switch to use helper for this case only.
#ifdef FCPP_CONCEPTS
      return impl::Curryable1Helper0<typename ConstCallable1<F,X>::result_type,F,X>::go(f,x);
#else
      return impl::Curryable1Helper0<typename Sig<X>::ResultType,F,X>::go(f,x);
#endif
   }
#else
  template <class T>
#ifdef FCPP_CONCEPTS
  inline ConstCallable1<F,T>::result_type
#else
  inline typename Sig<T>::ResultType 
#endif
  operator()( const T& x ) const {
#ifdef FCPP_PRINT_HELPERS
    std::cout << "f(x) called" << std::endl;
#endif
     return f(x);
   }
#endif

#ifdef FCPP_DEBUG
   std::string name() const { 
      if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("Full1::")+fcpp::traits::debug_traits<F>::name(f); 
      else
        return std::string("Full1"); 
   }
#endif
};

#ifdef FCPP_CONCEPTS
/* 
These concept maps are the equivalent of the different Sig 
definitions which remain below. These deal with the cases
with the full number of template parameters.
I have now sorted out how to deal with the case with only one parameter.
This involves a default parameter in the concepts.h file.
*/
template <class F,class X>
concept_map ConstCallable2<F,X>
{
  typedef Full1<impl::binder1of2<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable2<F,X,AutoCurryType>
{
  typedef Full1<impl::binder1of2<F,X> > result_type;
};

template <class F,class Y>
  concept_map ConstCallable2<F,AutoCurryType,Y>
{
  //typedef Full1<impl::binder2of2<Full2<F>,Y> > result_type;
  typedef Full1<impl::binder2of2<F,Y> > result_type;
};

#endif

#ifdef FCPP_EXTRA_STRONG_CURRY
// Extra strong currying implies a reordering of the arguments.

// f(_,a) -> bind2of2(f,a) 

// Thus f(_)(a) should mean the same but at the moment

// f(_) -> f so that f(_)(a) -> bind1of2(f,a) which is NOT the same.

// What is needed is that f(_) -> cycle2(f) which binds the arguments in a reverse order.
// This needs to be a new functoid which can be in turn curried 
// as the arguments are not available when f(_) is called.

// This has now been defined in curry.h after Bind1and2of2.

// It is used here and in Curryable2Helper0.

#ifdef FCPP_CONCEPTS

// This enables f(_) to work -> f
template <class F>
concept_map ConstCallable2<F,AutoCurryType>
{
  //typedef F result_type;
  // This version is needed in the non-lambda context.
  // In the lambda context it gives an extra level of call.
  // Could this be resolved with traits?
  // Above comment may not now be relevant.
  // Code now changed to use cycler2
  typedef Full2<impl::cycler2<F> > result_type;
};

// This enables f(_,_) to work -> f
template <class F>
concept_map ConstCallable2<F,AutoCurryType,AutoCurryType>
{
  //typedef F result_type;
  typedef Full2<F> result_type;
};

#else

// This allows cases like f(_) and f(_,_)

// This helper is needed because the code will not work inside Full2.
template <class F,class X,class Y>
struct Full2SigHelper 
   : public FunType<typename F::template Sig<X,Y>::Arg1Type,
                    typename F::template Sig<X,Y>::Arg2Type,
                    typename RT<F,X,Y>::ResultType> {};


template <class F,class X>
  struct Full2SigHelper<F,X,void> : public FunType<X,Full1<impl::binder1of2<F,X> > > 
{};   

template <class F,class Y>
  struct Full2SigHelper<F,AutoCurryType,Y> : public FunType<AutoCurryType,Y,Full1<impl::binder2of2<F,Y> > > 
{};   

template <class F,class X>
  struct Full2SigHelper<F,X,AutoCurryType> : public FunType<X,AutoCurryType,Full1<impl::binder1of2<F,X> > > 
{};   

// This enables f(_) to work -> f
template <class F>
struct Full2SigHelper <F,AutoCurryType,void> : 
  public FunType<AutoCurryType,Full2<impl::cycler2<F> > >
{};   

// This enables f(_,_) to work -> f
template <class F>
struct Full2SigHelper <F,AutoCurryType,AutoCurryType> : 
  public FunType<AutoCurryType,AutoCurryType,Full2<F> >
{};   

#endif
#endif

template <class F>
class Full2 : public SmartFunctoid2 {
   F f;
public:
   Full2() : f() {}
   Full2( const F& ff ) : f(ff) {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     // This is a different way to use the name() members.
     std::cout << "Full2(" ;
     if (fcpp::traits::debug_traits<F>::has_name) {
       std::cout << fcpp::traits::debug_traits<F>::name(f);
     } else {
       std::cout << "f";
     }
     std::cout << ") called." << std::endl;
#else
     std::cout << "Full2(ff) called" << std::endl;
#endif
#endif
   }

   // Experimental swap routine following boost function
   // See boost/function/function_template.hpp
   /*
   void swap(F &ff) {
     if(f == ff) return;
     F temp = f;
     f = ff;
     ff = temp;
   } 
   */
#ifdef FCPP_ENABLE_LAMBDA
   typedef Full2 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
/////////////  copied from Curryable2; added impl:: to binders  //////
#ifndef FCPP_CONCEPTS

   // Extra defs for the monomorphic traits does not make sense.
   //typedef typename F::template Sig<X,Y>::Arg1Type first_argument_type;
   //typedef typename F::template Sig<X,Y>::Arg1Type second_argument_type;
   //typedef typename RT<F,X,Y>::ResultType result_type;

#ifdef FCPP_EXTRA_STRONG_CURRY

   // This allows cases like f(_) and f(_,_)
   template <class X, class Y=void>
     struct Sig : public Full2SigHelper<F,X,Y>
   {};

#else

   template <class X, class Y=void>
   struct Sig
   : public FunType<typename F::template Sig<X,Y>::Arg1Type,
                    typename F::template Sig<X,Y>::Arg2Type,
                    typename RT<F,X,Y>::ResultType> {};

   template <class X>
     struct Sig<X,void> : public FunType<X,Full1<impl::binder1of2<F,X> > > {};

   template <class Y>
   struct Sig<AutoCurryType,Y>
   : public FunType<AutoCurryType,Y,Full1<impl::binder2of2<F,Y> > > {};

   template <class X>
   struct Sig<X,AutoCurryType>
   : public FunType<X,AutoCurryType,Full1<impl::binder1of2<F,X> > > {};
#endif
#endif

#ifdef FCPP_EXTRA_STRONG_CURRY
   template <class X>
#ifdef FCPP_CONCEPTS
   inline ConstCallable2<F,X>::result_type
#else
   inline typename Sig<X>::ResultType 
#endif
   operator()( const X& x ) const {
#ifdef FCPP_PRINT_HELPERS
      std::cout << "Curryable2Helper0 called" << std::endl;
#endif
      // Switch to use helper for this case only.
#ifdef FCPP_CONCEPTS
      return impl::Curryable2Helper0<typename ConstCallable2<F,X>::result_type,F,X>::go(f,x);
#else
      return impl::Curryable2Helper0<typename Sig<X>::ResultType,F,X>::go(f,x);
#endif
   }
#else
   template <class X>
#ifdef FCPP_CONCEPTS
   inline ConstCallable2<F,X>::result_type
#else
   inline typename Sig<X>::ResultType 
#endif
   operator()( const X& x ) const {
#ifdef FCPP_PRINT_HELPERS
      std::cout << "makeFull1 called" << std::endl;
#endif
      return makeFull1( impl::binder1of2<F,X>(f,x) );
   }
#endif

   template <class X, class Y>
#ifdef FCPP_CONCEPTS
   inline ConstCallable2<F,X,Y>::result_type
#else
   inline typename Sig<X,Y>::ResultType
#endif
   operator()( const X& x, const Y& y ) const {
#ifdef FCPP_PRINT_HELPERS
    std::cout << "Curryable2Helper called" << std::endl;
#endif
#ifdef FCPP_CONCEPTS
return impl::Curryable2Helper<typename ConstCallable2<F,X,Y>::result_type,F,X,Y>::go(f,x,y);
#else
      // need partial specialization, so defer to a class helper
return impl::Curryable2Helper<typename Sig<X,Y>::ResultType,F,X,Y>::go(f,x,y);
#endif
   }
//////////////////////////////////////////////////////////////////////
#ifdef FCPP_DEBUG
   std::string name() const { 
      if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("Full2::")+fcpp::traits::debug_traits<F>::name(f); 
      else
        return std::string("Full2"); 
   }
#endif
};


#ifdef FCPP_CONCEPTS
// Space for the concept maps for Full3
template <class F,class X>
concept_map ConstCallable3<F,X>
{
  typedef Full2<impl::binder1of3<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable3<F,X,AutoCurryType>
{
  typedef Full2<impl::binder1of3<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable3<F,X,AutoCurryType,AutoCurryType>
{
  typedef Full2<impl::binder1of3<F,X> > result_type;
};

template <class F,class Y>
  concept_map ConstCallable3<F,AutoCurryType,Y>
{
  typedef Full2<impl::binder2of3<F,Y> > result_type;
};

template <class F,class Y>
  concept_map ConstCallable3<F,AutoCurryType,Y,AutoCurryType>
{
  typedef Full2<impl::binder2of3<F,Y> > result_type;
};

template <class F,class Z>
  concept_map ConstCallable3<F,AutoCurryType,AutoCurryType,Z>
{
  typedef Full2<impl::binder3of3<F,Z> > result_type;
};

 template <class F,class X, class Y>
   concept_map ConstCallable3<F,X,Y>
{
  typedef Full1<impl::binder1and2of3<F,X,Y> > result_type;
};

 template <class F,class X, class Y>
   concept_map ConstCallable3<F,X,Y,AutoCurryType>
{
  typedef Full1<impl::binder1and2of3<F,X,Y> > result_type;
};

 template <class F,class X, class Z>
   concept_map ConstCallable3<F,X,AutoCurryType,Z>
{
  typedef Full1<impl::binder1and3of3<F,X,Z> > result_type;
};

 template <class F,class Y, class Z>
   concept_map ConstCallable3<F,AutoCurryType,Y,Z>
{
  typedef Full1<impl::binder2and3of3<F,Y,Z> > result_type;
};

#endif

#ifdef FCPP_EXTRA_STRONG_CURRY
#ifdef FCPP_CONCEPTS

// This enables f(_) to work -> f
template <class F>
concept_map ConstCallable3<F,AutoCurryType>
{
  //typedef F result_type;
  typedef Full3<impl::cycler3r<F> > result_type;
};

// This enables f(_,_) to work -> f
template <class F>
concept_map ConstCallable3<F,AutoCurryType,AutoCurryType>
{
  //typedef F result_type;
  typedef Full3<impl::cycler3<F> > result_type;
};

// This enables f(_,_,_) to work -> f
template <class F>
concept_map ConstCallable3<F,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef F result_type;
  //typedef Full3<F> result_type;
};

#else

// This allows cases like f(_) and f(_,_)

// This helper is needed because the code will not work inside Full2.
template <class F,class X,class Y=void,class Z=void>
struct Full3SigHelper 
   : public FunType<typename F::template Sig<X,Y,Z>::Arg1Type,
                    typename F::template Sig<X,Y,Z>::Arg2Type,
                    typename F::template Sig<X,Y,Z>::Arg3Type,
   typename RT<F,X,Y,Z>::ResultType> {};


template <class F,class X>
  struct Full3SigHelper<F,X> 
   : public FunType<X,Full2<impl::binder1of3<F,X> > > 
{};   

template <class F,class X>
struct Full3SigHelper<F,X,AutoCurryType> 
   : public FunType<X,AutoCurryType,Full2<impl::binder1of3<F,X> > > 
{};   

template <class F,class X>
struct Full3SigHelper<F,X,AutoCurryType,AutoCurryType> 
   : public FunType<X,AutoCurryType,AutoCurryType,Full2<impl::binder1of3<F,X> > > 
{};   

template <class F,class Y>
struct Full3SigHelper<F,AutoCurryType,Y> 
   : public FunType<AutoCurryType,Y,Full2<impl::binder2of3<F,Y> > > 
{};   

template <class F,class Y>
struct Full3SigHelper<F,AutoCurryType,Y,AutoCurryType> 
   : public FunType<AutoCurryType,Y,AutoCurryType,Full2<impl::binder2of3<F,Y> > > 
{};   

template <class F,class Z>
struct Full3SigHelper<F,AutoCurryType,AutoCurryType,Z> 
   : public FunType<AutoCurryType,AutoCurryType,Z,Full2<impl::binder3of3<F,Z> > > 
{};   

template <class F,class X,class Y> 
struct Full3SigHelper<F,X,Y>
   : public FunType<X,Y,Full1<impl::binder1and2of3<F,X,Y> > > {};

template <class F,class X,class Y> 
struct Full3SigHelper<F,X,Y,AutoCurryType>
   : public FunType<X,Y,AutoCurryType,Full1<impl::binder1and2of3<F,X,Y> > > {};

template <class F,class X,class Z> 
struct Full3SigHelper<F,X,AutoCurryType,Z>
   : public FunType<X,AutoCurryType,Z,Full1<impl::binder1and3of3<F,X,Z> > > {};

template <class F,class Y,class Z> 
struct Full3SigHelper<F,AutoCurryType,Y,Z>
  : public FunType<AutoCurryType,Y,Z,Full1<impl::binder2and3of3<F,Y,Z> > > {};


// This enables f(_) to work -> cycler3r(f)
template <class F>
struct Full3SigHelper <F,AutoCurryType,void> : 
public FunType<AutoCurryType,Full3<impl::cycler3r<F> > >
{};   

// This enables f(_,_) to work -> cycler3(f)
template <class F>
struct Full3SigHelper <F,AutoCurryType,AutoCurryType> : 
  public FunType<AutoCurryType,AutoCurryType,Full3<impl::cycler3<F> > >
{};   

// This enables f(_,_,_) to work -> f
template <class F>
struct Full3SigHelper <F,AutoCurryType,AutoCurryType,AutoCurryType> : 
  public FunType<AutoCurryType,AutoCurryType,AutoCurryType,Full3<F> >
{};   

#endif
#endif

template <class F>
class Full3 : public SmartFunctoid3 {
   F f;
public:
   Full3() : f() {}
   Full3( const F& ff ) : f(ff) {}
#ifdef FCPP_ENABLE_LAMBDA
   typedef Full3 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
/////////////  copied from Curryable3; added impl:: to all binders  //
#ifndef FCPP_CONCEPTS

#ifdef FCPP_EXTRA_STRONG_CURRY

   // This allows cases like f(_) and f(_,_)
   template <class X, class Y=void, class Z=void>
     struct Sig : public Full3SigHelper<F,X,Y,Z>
   {};

#else


   template <class X, class Y=void, class Z=void>
   struct Sig
   : public FunType<typename F::template Sig<X,Y,Z>::Arg1Type,
                    typename F::template Sig<X,Y,Z>::Arg2Type,
                    typename F::template Sig<X,Y,Z>::Arg3Type,
                    typename RT<F,X,Y,Z>::ResultType> {};

   template <class X,class Y> struct Sig<X,Y,void>
   : public FunType<X,Y,Full1<impl::binder1and2of3<F,X,Y> > > {};

   template <class X> struct Sig<X,AutoCurryType,void>
   : public FunType<X,AutoCurryType,Full2<impl::binder1of3<F,X> > > {};

   template <class Y> struct Sig<AutoCurryType,Y,void>
   : public FunType<AutoCurryType,Y,Full2<impl::binder2of3<F,Y> > > {};

   template <class X> struct Sig<X,void,void>
   : public FunType<X,Full2<impl::binder1of3<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType>
   : public FunType<X,AutoCurryType,AutoCurryType,
                    Full2<impl::binder1of3<F,X> > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType>
   : public FunType<AutoCurryType,Y,AutoCurryType,
                    Full2<impl::binder2of3<F,Y> > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z>
   : public FunType<AutoCurryType,AutoCurryType,Z,
                    Full2<impl::binder3of3<F,Z> > > {};

   template <class X,class Z> struct Sig<X,AutoCurryType,Z>
   : public FunType<X,AutoCurryType,Z,Full1<impl::binder1and3of3<F,X,Z> > > {};

   template <class Y,class Z> struct Sig<AutoCurryType,Y,Z>
   : public FunType<AutoCurryType,Y,Z,Full1<impl::binder2and3of3<F,Y,Z> > > {};

   template <class X,class Y> struct Sig<X,Y,AutoCurryType>
   : public FunType<X,Y,AutoCurryType,Full1<impl::binder1and2of3<F,X,Y> > > {};
#endif
#endif

   template <class X,class Y>
#ifdef FCPP_CONCEPTS
   inline ConstCallable3<F,X,Y>::result_type
#else
   typename Sig<X,Y>::ResultType
#endif
   operator()( const X& x, const Y& y ) const {
      // need partial specialization, so defer to a class helper
#ifdef FCPP_CONCEPTS
     return impl::Curryable3Helper2<typename ConstCallable3<F,X,Y>::result_type,F,X,Y>::go(f,x,y);
#else
     return impl::Curryable3Helper2<typename Sig<X,Y>::ResultType,F,X,Y>::go(f,x,y);
#endif
   }

#ifdef FCPP_EXTRA_STRONG_CURRY
   template <class X>
#ifdef FCPP_CONCEPTS
   inline ConstCallable3<F,X>::result_type
#else
   inline typename Sig<X>::ResultType 
#endif
   operator()( const X& x ) const {
#ifdef FCPP_PRINT_HELPERS
      std::cout << "Curryable3Helper0 called" << std::endl;
#endif
      // Switch to use helper for this case only.
#ifdef FCPP_CONCEPTS
      return impl::Curryable3Helper0<typename ConstCallable3<F,X>::result_type,F,X>::go(f,x);
#else
      return impl::Curryable3Helper0<typename Sig<X>::ResultType,F,X>::go(f,x);
#endif
   }
#else
   template <class X>
#ifdef FCPP_CONCEPTS
   inline ConstCallable3<F,X>::result_type
#else
   inline typename Sig<X>::ResultType 
#endif
   operator()( const X& x ) const {
#ifdef FCPP_PRINT_HELPERS
      std::cout << "makeFull2 called" << std::endl;
#endif
      return makeFull2(impl::binder1of3<F,X>(f,x));
   }
#endif

   template <class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
     //  requires ConstCallable3<F,X,Y,Z>
     inline ConstCallable3<F,X,Y,Z>::result_type
#else
   inline typename Sig<X,Y,Z>::ResultType
#endif
   operator()( const X& x, const Y& y, const Z& z ) const {
      // need partial specialization, so defer to a class helper
#ifdef FCPP_CONCEPTS
#ifdef FCPP_PRINT_HELPERS
     std::cout << "ConstCallable3 used" << std::endl;
#endif
     return impl::Curryable3Helper<typename ConstCallable3<F,X,Y,Z>::result_type,F,X,Y,Z>::go(f,x,y,z);
#else
      return impl::Curryable3Helper<typename Sig<X,Y,Z>::ResultType,F,X,Y,Z>
      ::go(f,x,y,z);
#endif
   }
//////////////////////////////////////////////////////////////////////
#ifdef FCPP_DEBUG
   std::string name() const { 
      if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("Full3::")+fcpp::traits::debug_traits<F>::name(f); 
      else
        return std::string("Full3"); 
   }
#endif
};
// FC++.1.5.2
#ifdef FCPP152
//////////////////////////////////////////////////////////////////////
// Full4 is now complete.
//////////////////////////////////////////////////////////////////////
#ifdef FCPP_CONCEPTS
// Space for the concept maps for Full4
template <class F,class X>
concept_map ConstCallable4<F,X>
{
  typedef Full3<impl::binder1of4<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable4<F,X,AutoCurryType>
{
  typedef Full3<impl::binder1of4<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable4<F,X,AutoCurryType,AutoCurryType>
{
  typedef Full3<impl::binder1of4<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable4<F,X,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full3<impl::binder1of4<F,X> > result_type;
};

template <class F,class Y>
concept_map ConstCallable4<F,AutoCurryType,Y>
{
  typedef Full3<impl::binder2of4<F,Y> > result_type;
};

template <class F,class Y>
concept_map ConstCallable4<F,AutoCurryType,Y,AutoCurryType>
{
  typedef Full3<impl::binder2of4<F,Y> > result_type;
};

template <class F,class Y>
concept_map ConstCallable4<F,AutoCurryType,Y,AutoCurryType,AutoCurryType>
{
  typedef Full3<impl::binder2of4<F,Y> > result_type;
};

template <class F,class Z>
concept_map ConstCallable4<F,AutoCurryType,AutoCurryType,Z>
{
  typedef Full3<impl::binder3of4<F,Z> > result_type;
};

template <class F,class Z>
concept_map ConstCallable4<F,AutoCurryType,AutoCurryType,Z,AutoCurryType>
{
  typedef Full3<impl::binder3of4<F,Z> > result_type;
};

template <class F,class W>
  concept_map ConstCallable4<F,AutoCurryType,AutoCurryType,AutoCurryType,W>
{
  typedef Full3<impl::binder4of4<F,W> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable4<F,X,Y>
{
  typedef Full2<impl::binder1and2of4<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable4<F,X,Y,AutoCurryType>
{
  typedef Full2<impl::binder1and2of4<F,X,Y> > result_type;
};

template <class F,class X, class Z>
  concept_map ConstCallable4<F,X,AutoCurryType,Z>
{
  typedef Full2<impl::binder1and3of4<F,X,Z> > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable4<F,AutoCurryType,Y,Z>
{
  typedef Full2<impl::binder2and3of4<F,Y,Z> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable4<F,X,Y,AutoCurryType,AutoCurryType>
{
  typedef Full2<impl::binder1and2of4<F,X,Y> > result_type;
};

template <class F,class X, class Z>
  concept_map ConstCallable4<F,X,AutoCurryType,Z,AutoCurryType>
{
  typedef Full2<impl::binder1and3of4<F,X,Z> > result_type;
};

template <class F,class X, class W>
  concept_map ConstCallable4<F,X,AutoCurryType,AutoCurryType,W>
{
  typedef Full2<impl::binder1and4of4<F,X,W> > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable4<F,AutoCurryType,Y,Z,AutoCurryType>
{
  typedef Full2<impl::binder2and3of4<F,Y,Z> > result_type;
};

template <class F,class Y, class W>
  concept_map ConstCallable4<F,AutoCurryType,Y,AutoCurryType,W>
{
  typedef Full2<impl::binder2and4of4<F,Y,W> > result_type;
};

template <class F,class Z, class W>
  concept_map ConstCallable4<F,AutoCurryType,AutoCurryType,Z,W>
{
  typedef Full2<impl::binder3and4of4<F,Z,W> > result_type;
};

 template <class F,class X, class Y, class Z>
   concept_map ConstCallable4<F,X,Y,Z>
{
  typedef Full1<impl::binder1and2and3of4<F,X,Y,Z> > result_type;
};

 template <class F,class X, class Y, class Z>
   concept_map ConstCallable4<F,X,Y,Z,AutoCurryType>
{
  typedef Full1<impl::binder1and2and3of4<F,X,Y,Z> > result_type;
};

 template <class F,class X, class Y, class W>
   concept_map ConstCallable4<F,X,Y,AutoCurryType,W>
{
  typedef Full1<impl::binder1and2and4of4<F,X,Y,W> > result_type;
};

 template <class F,class X, class Z, class W>
   concept_map ConstCallable4<F,X,AutoCurryType,Z,W>
{
  typedef Full1<impl::binder1and3and4of4<F,X,Z,W> > result_type;
};

 template <class F,class Y, class Z, class W>
   concept_map ConstCallable4<F,AutoCurryType,Y,Z,W>
{
  typedef Full1<impl::binder2and3and4of4<F,Y,Z,W> > result_type;
};

#endif

template <class F>
class Full4 : public SmartFunctoid4 {
   F f;
public:
   Full4() : f() {}
   Full4( const F& ff ) : f(ff) {}
#ifdef FCPP_ENABLE_LAMBDA
   typedef Full4 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
/////////////  copied from curryable4; added impl:: to all binders  //
#ifndef FCPP_CONCEPTS
   template <class X, class Y=void, class Z=void, class V=void>
   struct Sig
   : public FunType<
                    typename F::template Sig<X,Y,Z,V>::Arg1Type,
                    typename F::template Sig<X,Y,Z,V>::Arg2Type,
                    typename F::template Sig<X,Y,Z,V>::Arg3Type,
                    typename F::template Sig<X,Y,Z,V>::Arg4Type,
                    typename RT<F,X,Y,Z,V>::ResultType> {};

   template <class X> struct Sig<X> 
   : public FunType<X,Full3<impl::binder1of4<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType>
   : public FunType<X,AutoCurryType,
                    Full3<impl::binder1of4<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType>
   : public FunType<X,AutoCurryType,AutoCurryType,
                    Full3<impl::binder1of4<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType>
   : public FunType<X,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full3<impl::binder1of4<F,X> > > {};

   template <class Y> struct Sig<AutoCurryType,Y>
   : public FunType<AutoCurryType,Y,Full3<impl::binder2of4<F,Y> > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType>
   : public FunType<AutoCurryType,Y,AutoCurryType,Full3<impl::binder2of4<F,Y> > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType>
   : public FunType<AutoCurryType,Y,AutoCurryType,AutoCurryType,Full3<impl::binder2of4<F,Y> > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z>
   : public FunType<AutoCurryType,AutoCurryType,Z,Full3<impl::binder3of4<F,Z> > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType>
   : public FunType<AutoCurryType,AutoCurryType,Z,AutoCurryType,Full3<impl::binder3of4<F,Z> > > {};

   template <class V> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,V>
   : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,V,Full3<impl::binder4of4<F,V> > > {};

   template <class X,class Y> struct Sig<X,Y> 
   : public FunType<X,Y,Full2<impl::binder1and2of4<F,X,Y> > > {};

   template <class X,class Y> struct Sig<X,Y,AutoCurryType> : public 
      FunType<X,Y,AutoCurryType,Full2<impl::binder1and2of4<F,X,Y> > > {};

   template <class X,class Y> struct Sig<X,Y,AutoCurryType,AutoCurryType> : public 
      FunType<X,Y,AutoCurryType,AutoCurryType,Full2<impl::binder1and2of4<F,X,Y> > > {};

   template <class X,class Z> struct Sig<X,AutoCurryType,Z> : public 
      FunType<X,AutoCurryType,Z,AutoCurryType,Full2<impl::binder1and3of4<F,X,Z> > > {};
   template <class X,class Z> struct Sig<X,AutoCurryType,Z,AutoCurryType> : public 
      FunType<X,AutoCurryType,Z,AutoCurryType,Full2<impl::binder1and3of4<F,X,Z> > > {};

   template <class X,class V> struct Sig<X,AutoCurryType,AutoCurryType,V> : public 
      FunType<X,AutoCurryType,AutoCurryType,V,Full2<impl::binder1and4of4<F,X,V> > > {};

   template <class Y,class Z> struct Sig<AutoCurryType,Y,Z> : public 
      FunType<AutoCurryType,Y,Z,AutoCurryType,Full2<impl::binder2and3of4<F,Y,Z> > > {};

   template <class Y,class Z> struct Sig<AutoCurryType,Y,Z,AutoCurryType> : public 
      FunType<AutoCurryType,Y,Z,AutoCurryType,Full2<impl::binder2and3of4<F,Y,Z> > > {};

   template <class Y,class V> struct Sig<AutoCurryType,Y,AutoCurryType,V> : public 
      FunType<AutoCurryType,Y,AutoCurryType,V,Full2<impl::binder2and4of4<F,Y,V> > > {};

   template <class Z,class V> struct Sig<AutoCurryType,AutoCurryType,Z,V> : public 
      FunType<AutoCurryType,AutoCurryType,Z,V,Full2<impl::binder3and4of4<F,Z,V> > > {};


   template <class X,class Y,class Z> struct Sig<X,Y,Z,AutoCurryType> : public 
      FunType<X,Y,Z,AutoCurryType,Full1<impl::binder1and2and3of4<F,X,Y,Z> > > {};

   template <class X,class Y,class Z> struct Sig<X,Y,Z> : public 
      FunType<X,Y,Z,Full1<impl::binder1and2and3of4<F,X,Y,Z> > > {};


   template <class X,class Z,class W> struct Sig<X,AutoCurryType,Z,W> : public 
      FunType<X,AutoCurryType,Z,W,Full1<impl::binder1and3and4of4<F,X,Z,W> > > {};
 
   template <class X,class Y,class V> struct Sig<X,Y,AutoCurryType,V> : public 
      FunType<X,Y,AutoCurryType,V,Full1<impl::binder1and2and4of4<F,X,Y,V> > > {};

   template <class Y,class Z,class V> struct Sig<AutoCurryType,Y,Z,V> : public 
      FunType<AutoCurryType,Y,Z,V,Full1<impl::binder2and3and4of4<F,Y,Z,V> > > {};
#endif

   template <class X>
#ifdef FCPP_CONCEPTS
   inline ConstCallable4<F,X>::result_type
#else
   typename Sig<X>::ResultType 
#endif
   operator()( const X& x ) const {
#ifdef FCPP_PRINT_HELPERS
      std::cout << "full4 binder1of4" << std::endl;
#endif
      return makeFull3(impl::binder1of4<F,X>(f,x));
   }

   template <class X,class Y>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<F,X,Y>::result_type
#else
   typename Sig<X,Y>::ResultType 
#endif
   operator()( const X& x, const Y& y ) const {
      // need partial specialization, so defer to a class helper
#ifdef FCPP_CONCEPTS
   return impl::Curryable4Helper3<typename ConstCallable4<F,X,Y>::result_type,F,X,Y>::go(f,x,y);
#else
   return impl::Curryable4Helper3<typename Sig<X,Y>::ResultType,F,X,Y>::go(f,x,y);
#endif
   }

   template <class X,class Y,class Z>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<F,X,Y,Z>::result_type
#else
   typename Sig<X,Y,Z>::ResultType 
#endif
   operator()( const X& x, const Y& y, const Z& z ) const {
      // need partial specialization, so defer to a class helper
#ifdef FCPP_CONCEPTS
   return impl::Curryable4Helper2<typename ConstCallable4<F,X,Y,Z>::result_type,F,X,Y,Z>::go(f,x,y,z);
#else
   return impl::Curryable4Helper2<typename Sig<X,Y,Z>::ResultType,F,X,Y,Z>::go(f,x,y,z);
#endif
   }

   template <class X, class Y, class Z, class V>
#ifdef FCPP_CONCEPTS
     // requires ConstCallable4<F,X,Y,Z,V>
     inline ConstCallable4<F,X,Y,Z,V>::result_type
#else
   inline typename Sig<X,Y,Z,V>::ResultType
#endif
   operator()( const X& x, const Y& y, const Z& z, const V& v  ) const {
      // need partial specialization, so defer to a class helper
#ifdef FCPP_CONCEPTS
#ifdef FCPP_PRINT_HELPERS
     std::cout << "ConstCallable4 used" << std::endl;
#endif
     return impl::Curryable4Helper<typename ConstCallable4<F,X,Y,Z,V>::result_type,F,X,Y,Z,V>::go(f,x,y,z,v);
#else
      return impl::Curryable4Helper<typename Sig<X,Y,Z,V>::ResultType,F,X,Y,Z,V>
      ::go(f,x,y,z,v);
#endif
   }
#ifdef FCPP_DEBUG
   std::string name() const { 
      if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("Full4::")+fcpp::traits::debug_traits<F>::name(f) ; 
      else
        return std::string("Full4"); 
   }
#endif

};
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Full5 is now complete.
//////////////////////////////////////////////////////////////////////
#ifdef FCPP_CONCEPTS
// Space for the concept maps for Full5
// These are not complete for three and four arguments.
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
  typedef Full2<impl::binder1and2and3of5<F,X,Y,Z> > result_type;
};

template <class F,class X, class Y, class Z>
   concept_map ConstCallable5<F,X,Y,Z,AutoCurryType>
{
  typedef Full2<impl::binder1and2and3of5<F,X,Y,Z> > result_type;
};

template <class F,class X, class Y, class Z>
   concept_map ConstCallable5<F,X,Y,Z,AutoCurryType,AutoCurryType>
{
  typedef Full2<impl::binder1and2and3of5<F,X,Y,Z> > result_type;
};

template <class F,class X, class Y, class V>
  concept_map ConstCallable5<F,X,Y,AutoCurryType,V>
{
  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V> > > result_type;
};

template <class F,class X, class Y, class V>
  concept_map ConstCallable5<F,X,Y,AutoCurryType,V,AutoCurryType>
{
  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V> > > result_type;
};

template <class F,class X, class Y, class W>
  concept_map ConstCallable5<F,X,Y,AutoCurryType,AutoCurryType,W>
{
  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind5,W> > > result_type;
};

template <class F,class X, class Z, class V>
  concept_map ConstCallable5<F,X,AutoCurryType,Z,V>
{
  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > result_type;
};

template <class F,class X, class Z, class V>
  concept_map ConstCallable5<F,X,AutoCurryType,Z,V,AutoCurryType>
{
  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > result_type;
};

template <class F,class X, class Z, class W>
  concept_map ConstCallable5<F,X,AutoCurryType,Z,AutoCurryType,W>
{
  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind5,W> > > result_type;
};

template <class F,class X, class V, class W>
  concept_map ConstCallable5<F,X,AutoCurryType,AutoCurryType,V,W>
{
  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

template <class F,class Y, class Z, class V>
  concept_map ConstCallable5<F,AutoCurryType,Y,Z,V>
{

  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > result_type;
};

template <class F,class Y, class Z, class V>
  concept_map ConstCallable5<F,AutoCurryType,Y,Z,V,AutoCurryType>
{

  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > result_type;
};

template <class F,class Y, class Z, class W>
  concept_map ConstCallable5<F,AutoCurryType,Y,Z,AutoCurryType,W>
{
  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,W> > > result_type;
};

template <class F,class Y, class V, class W>
  concept_map ConstCallable5<F,AutoCurryType,Y,AutoCurryType,V,W>
{
  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind2,Y>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

template <class F,class Z, class V, class W>
  concept_map ConstCallable5<F,AutoCurryType,AutoCurryType,Z,V,W>
{
  typedef Full2<impl::binderXandYandZof5<F,std::pair<Bind3,Z>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

 template <class F,class X, class Y, class Z, class V>
   concept_map ConstCallable5<F,X,Y,Z,V>
{
  typedef Full1<impl::binder1and2and3and4of5<F,X,Y,Z,V> > result_type;
};

 template <class F,class X, class Y, class Z, class V>
   concept_map ConstCallable5<F,X,Y,Z,V,AutoCurryType>
{
  typedef Full1<impl::binder1and2and3and4of5<F,X,Y,Z,V> > result_type;
};

 template <class F,class X, class Y, class Z, class W>
   concept_map ConstCallable5<F,X,Y,Z,AutoCurryType,W>
{
  typedef Full1<impl::binderWandXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,W> > > result_type;
};

 template <class F,class X, class Y, class V, class W>
   concept_map ConstCallable5<F,X,Y,AutoCurryType,V,W>
{
  typedef Full1<impl::binderWandXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

 template <class F,class X, class Z, class V, class W>
   concept_map ConstCallable5<F,X,AutoCurryType,Z,V,W>
{
  typedef Full1<impl::binderWandXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
};

 template <class F,class Y, class Z, class V, class W>
   concept_map ConstCallable5<F,AutoCurryType,Y,Z,V,W>
{
  typedef Full1<impl::binderWandXandYandZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V>,std::pair<Bind5,W> > > result_type;
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

  // These are needed to get bind1and2and3and4of4 to work.
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
   // binderXandYof5         for the two parameter calls (completed)
   // binderXandYandZof5     for the three parameter calls
   // binderWandXandYandZof5 for the four parameter calls (completed)
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
     : public FunType<X,Y,Z,Full2<impl::binder1and2and3of5<F,X,Y,Z> > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z,AutoCurryType> 
     : public FunType<X,Y,Z,Full2<impl::binder1and2and3of5<F,X,Y,Z> > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z,AutoCurryType,AutoCurryType> 
     : public FunType<X,Y,Z,Full2<impl::binder1and2and3of5<F,X,Y,Z> > > {};

   template <class X, class Y, class V> struct Sig<X,Y,AutoCurryType,V> 
     : public FunType<X,Y,V,Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V> > > > {};
     // : public FunType<X,Y,V,Full2<impl::binder1and3of4<impl::binder1of5<F,X>,Y,V > > > {};

   template <class X, class Y, class V> struct Sig<X,Y,AutoCurryType,V,AutoCurryType> 
     : public FunType<X,Y,V,Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V> > > > {};
   //: public FunType<X,Y,V,Full2<impl::binder1and3of4<impl::binder1of5<F,X>,Y,V > > > {};

   template <class X, class Y, class W> struct Sig<X,Y,AutoCurryType,AutoCurryType,W> 
     : public FunType<X,Y,W,Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind5,W> > > > {};
   //: public FunType<X,Y,W,Full2<impl::binder1and4of4<impl::binder1of5<F,X>,Y,W > > > {};

   template <class X, class Z, class V> struct Sig<X,AutoCurryType,Z,V> 
     : public FunType<X,Z,V,Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > > {};
   //: public FunType<X,Z,V,Full2<impl::binder2and3of4<impl::binder1of5<F,X>,Z,V > > > {};

   template <class X, class Z, class V> struct Sig<X,AutoCurryType,Z,V,AutoCurryType> 
     : public FunType<X,Z,V,Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > > {};
   //: public FunType<X,Z,V,Full2<impl::binder2and3of4<impl::binder1of5<F,X>,Z,V > > > {};

   template <class X, class Z, class W> struct Sig<X,AutoCurryType,Z,AutoCurryType,W> 
     : public FunType<X,Z,W,Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind5,W> > > > {};
  // : public FunType<X,Z,W,Full2<impl::binder3of3<impl::binder2of4<impl::binder1of5<F,X>,Z>,W > > > {};
   //: public FunType<X,Z,W,Full2<impl::binder2and4of4<impl::binder1of5<F,X>,Z,W > > > {};

   template <class X, class V, class W> struct Sig<X,AutoCurryType,AutoCurryType,V,W> 
     : public FunType<X,V,W,Full2<impl::binderXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};
   //: public FunType<X,V,W,Full2<impl::binder3and4of4<impl::binder1of5<F,X>,V,W > > > {};

   template <class Y, class Z, class V> struct Sig<AutoCurryType,Y,Z,V> 
     : public FunType<Y,Z,V,Full2<impl::binderXandYandZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > > {};
   //: public FunType<Y,Z,V,Full2<impl::binder2and3of4<impl::binder2of5<F,Y>,Z,V > > > {};

   template <class Y, class Z, class V> struct Sig<AutoCurryType,Y,Z,V,AutoCurryType> 
     : public FunType<Y,Z,V,Full2<impl::binderXandYandZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V> > > > {};
   //: public FunType<Y,Z,V,Full2<impl::binder2and3of4<impl::binder2of5<F,Y>,Z,V > > > {};

   template <class Y, class Z, class W> struct Sig<AutoCurryType,Y,Z,AutoCurryType,W> 
     : public FunType<Y,Z,W,Full2<impl::binderXandYandZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,W> > > > {};
   //: public FunType<Y,Z,W,Full2<impl::binder2and4of4<impl::binder2of5<F,Y>,Z,W > > > {};

   template <class Y, class V, class W> struct Sig<AutoCurryType,Y,AutoCurryType,V,W> 
     : public FunType<Y,V,W,Full2<impl::binderXandYandZof5<F,std::pair<Bind2,Y>,std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};
   //: public FunType<Y,V,W,Full2<impl::binder3and4of4<impl::binder2of5<F,Y>,V,W > > > {};

   template <class Z, class V, class W> struct Sig<AutoCurryType,AutoCurryType,Z,V,W> 
     : public FunType<Z,V,W,Full2<impl::binderXandYandZof5<F,std::pair<Bind3,Z>,std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};
   //: public FunType<Z,V,W,Full2<impl::binder3and4of4<impl::binderAof5<F,Bind3,Z>,V,W > > > {};

   // Use one binder where it is available.
   template <class X, class Y, class Z, class V> struct Sig<X,Y,Z,V,AutoCurryType> 
     : public FunType<X,Y,Z,V,Full1<impl::binder1and2and3and4of5<F,X,Y,Z,V> > > {};

   template <class X, class Y, class Z, class W> struct Sig<X,Y,Z,AutoCurryType,W> 
     : public FunType<X,Y,Z,W,Full1<impl::binderWandXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,
                                                                   std::pair<Bind3,Z>,std::pair<Bind5,W> > > > {};
   //: public FunType<X,Y,Z,W,Full1<impl::binder1and3of3<impl::binder1and2of5<F,X,Y>,Z,W > > > {};

   template <class X, class Y, class V, class W> struct Sig<X,Y,AutoCurryType,V,W> 
     : public FunType<X,Y,V,W,Full1<impl::binderWandXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,
                                                                   std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};
   //: public FunType<X,Y,V,W,Full1<impl::binder2and3of3<impl::binder1and2of5<F,X,Y>,V,W > > > {};

   template <class X, class Z, class V, class W> struct Sig<X,AutoCurryType,Z,V,W> 
     : public FunType<X,Z,V,W,Full1<impl::binderWandXandYandZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,
                                                                   std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};
   //: public FunType<X,Z,V,W,Full1<impl::binder2and3and4of4<impl::binder1of5<F,X>,Z,V,W > > > {};

   template <class Y, class Z, class V, class W> struct Sig<AutoCurryType,Y,Z,V,W> 
     : public FunType<Y,Z,V,W,Full1<impl::binderWandXandYandZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,
                                                                   std::pair<Bind4,V>,std::pair<Bind5,W> > > > {};
   //: public FunType<Y,Z,V,W,Full1<impl::binder2and3and4of4<impl::binder2of5<F,Y>,Z,V,W > > > {};

   template <class X, class Y, class Z, class V> struct Sig<X,Y,Z,V> 
     : public FunType<X,Y,Z,V,Full1<impl::binder1and2and3and4of5<F,X,Y,Z,V> > > {};

#endif


   // This is needed to get bind1and2and3and4of4 to work.
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
     //return makeFull2(impl::binder1and2and3of5<F,X,Y,Z>(f,x,y,z));
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
     //return makeFull1(impl::binder1and2and3and4of5<F,X,Y,Z,W>(f,x,y,z,w));
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
      if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("Full5::")+fcpp::traits::debug_traits<F>::name(f); 
      else
        return std::string("Full5"); 
   }
#endif

};


//////////////////////////////////////////////////////////////////////
// First try at Full6.
//////////////////////////////////////////////////////////////////////
#ifdef FCPP_CONCEPTS
// Space for the concept maps for Full6
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

template <class F,class X, class Z>
  concept_map ConstCallable6<F,X,AutoCurryType,Z>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class X, class U>
  concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,U>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind4,U> > > result_type;
};

template <class F,class X, class V>
  concept_map ConstCallable6<F,X,AutoCurryType,AutoCurryType,AutoCurryType,V>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind5,V> > > result_type;
};

template <class F,class Y, class Z>
  concept_map ConstCallable6<F,AutoCurryType,Y,Z>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Y, class U>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,U>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind4,U> > > result_type;
};

template <class F,class Y, class V>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,V>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind5,V> > > result_type;
};

template <class F,class Z, class U>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,U>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U> > > result_type;
};

template <class F,class Z, class V>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,V>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind5,V> > > result_type;
};

template <class F,class U, class V>
  concept_map ConstCallable6<F,AutoCurryType,AutoCurryType,AutoCurryType,U,V>
{
  typedef Full4<impl::binderXandYof6<F,std::pair<Bind4,U>,std::pair<Bind5,V> > > result_type;
};

 template <class F,class X, class Y, class Z>
   concept_map ConstCallable6<F,X,Y,Z>
{
  typedef Full3<impl::binder1and2and3of6<F,X,Y,Z> > result_type;
};

template <class F,class Y, class V, class W>
  concept_map ConstCallable6<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,V,W>
{
  typedef Full3<impl::binderXandYandZof6<F,std::pair<Bind2,Y>,std::pair<Bind5,V>,
                                           std::pair<Bind6,W> > > result_type;
};

 template <class F, class W, class X, class Y, class Z>
   concept_map ConstCallable6<F,W,X,Y,Z>
{
  typedef Full2<impl::binder1and2and3and4of6<F,W,X,Y,Z> > result_type;
};

 template <class F, class V, class W, class X, class Y, class Z>
   concept_map ConstCallable6<F,V,W,X,Y,Z>
{
  typedef Full1<impl::binder1and2and3and4and5of6<F,V,W,X,Y,Z> > result_type;
};

#endif

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

   template <class X, class Z> struct Sig<X,AutoCurryType,Z> 
   : public FunType<X,Z,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > > {};

   template <class X, class Z> struct Sig<X,AutoCurryType,Z,AutoCurryType> 
   : public FunType<X,Z,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> > > > {};

   template <class X, class U> struct Sig<X,AutoCurryType,AutoCurryType,U> 
   : public FunType<X,U,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind4,U> > > > {};

   template <class X, class V> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,V> 
   : public FunType<X,V,Full4<impl::binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind5,V> > > > {};

   template <class Y, class Z> struct Sig<AutoCurryType,Y,Z> 
   : public FunType<Y,Z,Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class Y, class Z> struct Sig<AutoCurryType,Y,Z,AutoCurryType> 
   : public FunType<Y,Z,Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class Y, class U> struct Sig<AutoCurryType,Y,AutoCurryType,U> 
   : public FunType<Y,U,Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind4,U> > > > {};

   template <class Y, class V> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,V> 
   : public FunType<Y,V,Full4<impl::binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind5,V> > > > {};

   template <class Z, class U> struct Sig<AutoCurryType,AutoCurryType,Z,U> 
   : public FunType<Z,U,Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U> > > > {};

   template <class Z, class V> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,V> 
   : public FunType<Z,V,Full4<impl::binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind5,V> > > > {};

   template <class U, class V> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,U,V> 
   : public FunType<U,V,Full4<impl::binderXandYof6<F,std::pair<Bind4,U>,std::pair<Bind5,V> > > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z> 
   : public FunType<X,Y,Z,Full3<impl::binder1and2and3of6<F,X,Y,Z> > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z,AutoCurryType> 
   : public FunType<X,Y,Z,Full3<impl::binder1and2and3of6<F,X,Y,Z> > > {};

   template <class X, class Y, class U> struct Sig<X,Y,AutoCurryType,U> 
   : public FunType<X,Y,U,Full3<impl::binderXandYandZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind4,U> > > > {};

   template <class X, class Z, class U> struct Sig<X,AutoCurryType,Z,U> 
   : public FunType<X,Z,U,Full3<impl::binderXandYandZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind3,Z>,std::pair<Bind4,U> > > > {};

   template <class Y, class Z, class U> struct Sig<AutoCurryType,Y,Z,U> 
   : public FunType<Y,Z,U,Full3<impl::binderXandYandZof6<F,std::pair<Bind2,Y>,
		      std::pair<Bind3,Z>,std::pair<Bind4,U> > > > {};

   template <class Y, class V, class W> struct Sig<AutoCurryType,Y,AutoCurryType,
                                                   AutoCurryType,V,W> 
   : public FunType<Y,V,W,Full3<impl::binderXandYandZof6<F,std::pair<Bind2,Y>,
		      std::pair<Bind5,V>,std::pair<Bind6,W> > > > {};

   template <class X, class Y, class Z, class U> struct Sig<X,Y,Z,U> 
   : public FunType<X,Y,Z,U,Full2<impl::binder1and2and3and4of6<F,X,Y,Z,U> > > {};

   template <class X, class Y, class Z, class U, class V> struct Sig<X,Y,Z,U,V> 
   : public FunType<X,Y,Z,U,V,Full1<impl::binder1and2and3and4and5of6<F,X,Y,Z,U,V> > > {};

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
     std::cout << "full6 binder1and2and3and4of6" << std::endl;
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
     std::cout << "full6 binder1and2and3and4and5of6" << std::endl;
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
      if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("Full6::")+fcpp::traits::debug_traits<F>::name(f); 
      else
        return std::string("Full6"); 
   }
#endif

};

//////////////////////////////////////////////////////////////////////
// First try at Full7.
//////////////////////////////////////////////////////////////////////
#ifdef FCPP_CONCEPTS
// Space for the concept maps for Full7
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
concept_map ConstCallable7<F,X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binder1of7<F,X> > result_type;
};

template <class F,class X>
concept_map ConstCallable7<F,X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
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
  concept_map ConstCallable7<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable7<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > result_type;
};

template <class F,class Y>
  concept_map ConstCallable7<F,AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
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
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Z>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class Z>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > result_type;
};

template <class F,class T>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,T>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind4,T> > > result_type;
};

template <class F,class T>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,T,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind4,T> > > result_type;
};

template <class F,class T>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,T,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind4,T> > > result_type;
};

template <class F,class T>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,T,AutoCurryType,
                               AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind4,T> > > result_type;
};

template <class F,class U>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,U>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind5,U> > > result_type;
};

template <class F,class U>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,U,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind5,U> > > result_type;
};

template <class F,class U>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,U,AutoCurryType,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind5,U> > > result_type;
};

template <class F,class V>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,V>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind6,V> > > result_type;
};

template <class F,class V>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,V,AutoCurryType>
{
  typedef Full6<impl::binderXof7<F,std::pair<Bind6,V> > > result_type;
};

template <class F,class W>
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,W>
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
   concept_map ConstCallable7<F,X,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
{
  typedef Full5<impl::binder1and2of7<F,X,Y> > result_type;
};

template <class F,class X, class Y>
   concept_map ConstCallable7<F,X,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
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
  concept_map ConstCallable7<F,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,V,W>
{
  typedef Full5<impl::binderXandYof7<F,std::pair<Bind6,V>,std::pair<Bind7,W> > > result_type;
};


 template <class F,class X, class Y, class Z>
   concept_map ConstCallable7<F,X,Y,Z>
{
  typedef Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

 template <class F,class X, class Y, class Z>
   concept_map ConstCallable7<F,X,Y,Z,AutoCurryType>
{
  typedef Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

 template <class F,class X, class Y, class Z>
   concept_map ConstCallable7<F,X,Y,Z,AutoCurryType,AutoCurryType>
{
  typedef Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind3,Z> > > result_type;
};

 template <class F,class X, class Y, class T>
   concept_map ConstCallable7<F,X,Y,AutoCurryType,T>
{
  typedef Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,T> > > result_type;
};

 template <class F,class X, class Y, class T>
   concept_map ConstCallable7<F,X,Y,AutoCurryType,T,AutoCurryType>
{
  typedef Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,T> > > result_type;
};

 template <class F,class X, class Y, class U>
   concept_map ConstCallable7<F,X,Y,AutoCurryType,AutoCurryType,U>
{
  typedef Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind5,U> > > result_type;
};

 template <class F, class W, class X, class Y, class Z>
   concept_map ConstCallable7<F,W,X,Y,Z>
{
  typedef Full3<impl::binderWandXandYandZof7<F,std::pair<Bind1,W>,std::pair<Bind2,X>,
                                               std::pair<Bind3,Y>,std::pair<Bind4,Z> > > result_type;
};


 template <class F, class W, class X, class Y, class Z>
   concept_map ConstCallable7<F,W,X,Y,Z,AutoCurryType>
{
  typedef Full3<impl::binderWandXandYandZof7<F,std::pair<Bind1,W>,std::pair<Bind2,X>,
                                               std::pair<Bind3,Y>,std::pair<Bind4,Z> > > result_type;
};

 template <class F, class V, class W, class X, class Y, class Z>
   concept_map ConstCallable7<F,V,W,X,Y,Z>
{
  typedef Full2<impl::binderVandWandXandYandZof7<F,std::pair<Bind1,V>,std::pair<Bind2,W>,
                            std::pair<Bind3,X>,std::pair<Bind4,Y>,std::pair<Bind5,Z> > > result_type;
};

 template <class F, class U, class V, class W, class X, class Y, class Z>
   concept_map ConstCallable7<F,U,V,W,X,Y,Z>
{
  typedef Full1<impl::binderUandVandWandXandYandZof7<F,std::pair<Bind1,U>,std::pair<Bind2,V>,std::pair<Bind3,W>,
                            std::pair<Bind4,X>,std::pair<Bind5,Y>,std::pair<Bind6,Z> > > result_type;
};


/*
 template <class F, class U,class V, class W, class X, class Y, class Z>
   concept_map ConstCallable7<F,U,V,W,X,Y,Z>
{
  typedef Full0<impl::binder1and2and3and4and5and6of6<F,U,V,W,X,Y,Z> > result_type;
};
*/
#endif

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
   : public FunType<X,AutoCurryType,
                    Full6<impl::binder1of7<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType>
   : public FunType<X,AutoCurryType,AutoCurryType,
                    Full6<impl::binder1of7<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType>
   : public FunType<X,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full6<impl::binder1of7<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
   : public FunType<X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full6<impl::binder1of7<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
   : public FunType<X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full6<impl::binder1of7<F,X> > > {};

   template <class X> struct Sig<X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
   : public FunType<X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full6<impl::binder1of7<F,X> > > {};

   template <class Y> struct Sig<AutoCurryType,Y>
     : public FunType<AutoCurryType,Y,
                    Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType>
     : public FunType<AutoCurryType,Y,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,Y,AutoCurryType,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Y> struct Sig<AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind2,Y> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z>
     : public FunType<AutoCurryType,AutoCurryType,Z,
                    Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,Z,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > > {};

   template <class Z> struct Sig<AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind3,Z> > > > {};

   template <class T> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,T>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,T,
                    Full6<impl::binderXof7<F,std::pair<Bind4,T> > > > {};

   template <class T> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,T,AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,T,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind4,T> > > > {};

   template <class T> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,T,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,T,AutoCurryType,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind4,T> > > > {};

   template <class T> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,T,AutoCurryType,
                                 AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,T,AutoCurryType,AutoCurryType,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind4,T> > > > {};

   template <class U> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,U>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,U,
                    Full6<impl::binderXof7<F,std::pair<Bind5,U> > > > {};

   template <class U> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,U,AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,U,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind5,U> > > > {};

   template <class U> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,U,AutoCurryType,AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,U,AutoCurryType,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind5,U> > > > {};

   template <class V> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,V>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,V,
                    Full6<impl::binderXof7<F,std::pair<Bind6,V> > > > {};

   template <class V> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,V,AutoCurryType>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,V,AutoCurryType,
                    Full6<impl::binderXof7<F,std::pair<Bind6,V> > > > {};

   template <class W> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,W>
     : public FunType<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,W,
                    Full6<impl::binderXof7<F,std::pair<Bind7,W> > > > {};

   template <class X, class Y> struct Sig<X,Y> 
   : public FunType<X,Y,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Y> struct Sig<X,Y,AutoCurryType> 
   : public FunType<X,Y,AutoCurryType,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Y> struct Sig<X,Y,AutoCurryType,AutoCurryType> 
   : public FunType<X,Y,AutoCurryType,AutoCurryType,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Y> struct Sig<X,Y,AutoCurryType,AutoCurryType,AutoCurryType> 
   : public FunType<X,Y,AutoCurryType,AutoCurryType,AutoCurryType,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Y> struct Sig<X,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType> 
   : public FunType<X,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Y> struct Sig<X,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType> 
   : public FunType<X,Y,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,Full5<impl::binder1and2of7<F,X,Y> > > {};

   template <class X, class Z> struct Sig<X,AutoCurryType,Z> 
   : public FunType<X,Z,Full5<impl::binderXandYof7<F,std::pair<Bind1,X>,
                              std::pair<Bind3,Z> > > > {};

   template <class Y, class Z> struct Sig<AutoCurryType,Y,Z> 
   : public FunType<Y,Z,Full5<impl::binderXandYof7<F,std::pair<Bind2,Y>,
                              std::pair<Bind3,Z> > > > {};

   template <class V, class W> struct Sig<AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,V,W> 
   : public FunType<V,W,Full5<impl::binderXandYof7<F,std::pair<Bind6,V>,
                              std::pair<Bind7,W> > > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z> 
   : public FunType<X,Y,Z,Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z,AutoCurryType> 
   : public FunType<X,Y,Z,AutoCurryType,Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class X, class Y, class Z> struct Sig<X,Y,Z,AutoCurryType,AutoCurryType> 
   : public FunType<X,Y,Z,AutoCurryType,AutoCurryType,Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind3,Z> > > > {};

   template <class X, class Y, class T> struct Sig<X,Y,AutoCurryType,T> 
     : public FunType<X,Y,AutoCurryType,T,Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind4,T> > > > {};

   template <class X, class Y, class T> struct Sig<X,Y,AutoCurryType,T,AutoCurryType> 
     : public FunType<X,Y,AutoCurryType,T,AutoCurryType,Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind4,T> > > > {};

   template <class X, class Y, class U> struct Sig<X,Y,AutoCurryType,AutoCurryType,U> 
     : public FunType<X,Y,AutoCurryType,AutoCurryType,U,Full4<impl::binderXandYandZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind5,U> > > > {};

   template <class W,class X, class Y, class Z> struct Sig<W,X,Y,Z> 
     : public FunType<W,X,Y,Z,Full3<impl::binderWandXandYandZof7<F,std::pair<Bind1,W>,
                     std::pair<Bind2,X>,std::pair<Bind3,Y>,std::pair<Bind4,Z> > > > {};

   template <class W,class X, class Y, class Z> struct Sig<W,X,Y,Z,AutoCurryType> 
     : public FunType<W,X,Y,Z,AutoCurryType,Full3<impl::binderWandXandYandZof7<F,std::pair<Bind1,W>,
                     std::pair<Bind2,X>,std::pair<Bind3,Y>,std::pair<Bind4,Z> > > > {};

   template <class V,class W,class X, class Y, class Z> struct Sig<V,W,X,Y,Z> 
     : public FunType<V,W,X,Y,Z,Full2<impl::binderVandWandXandYandZof7<F,std::pair<Bind1,V>,
       std::pair<Bind2,W>,std::pair<Bind3,X>,std::pair<Bind4,Y>,std::pair<Bind5,Z> > > > {};

   template <class U,class V,class W,class X, class Y, class Z> struct Sig<U,V,W,X,Y,Z> 
     : public FunType<U,V,W,X,Y,Z,Full1<impl::binderUandVandWandXandYandZof7<F,std::pair<Bind1,U>,
       std::pair<Bind2,V>,std::pair<Bind3,W>,std::pair<Bind4,X>,std::pair<Bind5,Y>,std::pair<Bind6,Z> > > > {};

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
     //return makeFull5(impl::binder1and2of7<F,X,Y>(f,x,y));
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
      if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("Full7::")+fcpp::traits::debug_traits<F>::name(f);
      else
        return std::string("Full7"); 
   }
#endif

};

//////////////////////////////////////////////////////////////////////
// First try at Full8.
//////////////////////////////////////////////////////////////////////
#ifdef FCPP_CONCEPTS
// Space for the concept maps for Full8
template <class F,class X>
concept_map ConstCallable8<F,X>
{
  typedef Full7<impl::binder1of8<F,X> > result_type;
};
#endif

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
      if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("Full8::")+fcpp::traits::debug_traits<F>::name(f);
      else
        return std::string("Full8"); 
   }
#endif


};

#endif


template <class F> Full0<F> makeFull0( const F& f ) { return Full0<F>(f); }
template <class F> Full1<F> makeFull1( const F& f ) { 
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     // This is a different way to use the name() members.
     std::cout << "makeFull1(" ;
     if (fcpp::traits::debug_traits<F>::has_name) {
       std::cout << fcpp::traits::debug_traits<F>::name(f);
     } else {
       std::cout << "f";
     }
     std::cout << ") called." << std::endl;
#else
      std::cout << "makeFull1() called" << std::endl;
#endif
#endif
      return Full1<F>(f); 
}
template <class F> Full2<F> makeFull2( const F& f ) { 
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     // This is a different way to use the name() members.
     std::cout << "makeFull2(" ;
     if (fcpp::traits::debug_traits<F>::has_name) {
       std::cout << fcpp::traits::debug_traits<F>::name(f);
     } else {
       std::cout << "f";
     }
     std::cout << ") called." << std::endl;
#else
      std::cout << "makeFull2() called" << std::endl;
#endif
#endif
    return Full2<F>(f); 
}
template <class F> Full3<F> makeFull3( const F& f ) { return Full3<F>(f); }
// FC++.1.5.2
#ifdef FCPP152
template <class F> Full4<F> makeFull4( const F& f ) {
#ifdef FCPP_PRINT_HELPERS
      std::cout << "makeFull4 called" << std::endl;
#endif
      return Full4<F>(f); }
template <class F> Full5<F> makeFull5( const F& f ) { 
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "makeFull5 called" << std::endl;
#endif
      return Full5<F>(f); }
template <class F> Full6<F> makeFull6( const F& f ) { 
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "makeFull6 called" << std::endl;
#endif
      return Full6<F>(f); }
template <class F> Full7<F> makeFull7( const F& f ) { 
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "makeFull7 called" << std::endl;
#endif
      return Full7<F>(f); }
template <class F> Full8<F> makeFull8( const F& f ) { 
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "makeFull8 called" << std::endl;
#endif
      return Full8<F>(f); }
#endif

#ifdef FCPP_DEBUG
   namespace traits {

      template <class F>
      class debug_traits<Full1<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Full1<F> &f) { return f.name(); }
      };

      template <class F>
      class debug_traits<Full2<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Full2<F> &f) { return f.name(); }
      };

      template <class F>
      class debug_traits<Full3<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Full3<F> &f) { return f.name(); }
      };

#ifdef FCPP152

      template <class F>
      class debug_traits<Full4<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Full4<F> &f) { return f.name(); }
      };

      template <class F>
      class debug_traits<Full5<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Full5<F> &f) { return f.name(); }
      };

      template <class F>
      class debug_traits<Full6<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Full6<F> &f) { return f.name(); }
      };

      template <class F>
      class debug_traits<Full7<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Full7<F> &f) { return f.name(); }
      };

      template <class F>
      class debug_traits<Full8<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Full8<F> &f) { return f.name(); }
      };

#endif

   }
#endif

//////////////////////////////////////////////////////////////////////
// Definitions of stuff heretofore put-off...
//////////////////////////////////////////////////////////////////////
// from curry.h:
typedef Full1<impl::Const> Const;
typedef Full2<impl::Bind1of1> Bind1of1;

typedef Full2<impl::Bind1of2> Bind1of2;
typedef Full2<impl::Bind2of2> Bind2of2;
typedef Full3<impl::Bind1and2of2> Bind1and2of2;

//#ifdef FCPP_EXTRA_STRONG_CURRY
typedef Full1<impl::Cycle2> Cycle2;
typedef Full1<impl::Cycle3> Cycle3;
typedef Full1<impl::Cycle3r> Cycle3r;
//#endif

// FC++.1.5.2
#ifdef FCPP152
typedef Full1<impl::Konst> Konst;
typedef Full5<impl::Bind1and2and3and4of4> Bind1and2and3and4of4;

typedef Full4<impl::Bind1and2and3of3> Bind1and2and3of3;
typedef Full4<impl::Bind1and2and3of4> Bind1and2and3of4;
typedef Full4<impl::Bind1and2and4of4> Bind1and2and4of4;
typedef Full4<impl::Bind1and3and4of4> Bind1and3and4of4;
typedef Full4<impl::Bind2and3and4of4> Bind2and3and4of4;

typedef Full3<impl::Bind1and2of4> Bind1and2of4;
typedef Full3<impl::Bind2and3of4> Bind2and3of4;
typedef Full3<impl::Bind1and3of4> Bind1and3of4;
typedef Full3<impl::Bind1and4of4> Bind1and4of4;
typedef Full3<impl::Bind2and4of4> Bind2and4of4;
typedef Full3<impl::Bind3and4of4> Bind3and4of4;

typedef Full2<impl::Bind1of4> Bind1of4;
typedef Full2<impl::Bind2of4> Bind2of4;
typedef Full2<impl::Bind3of4> Bind3of4;
typedef Full2<impl::Bind4of4> Bind4of4;

typedef Full2<impl::Bind1of5> Bind1of5;
typedef Full2<impl::Bind2of5> Bind2of5;
typedef Full3<impl::BindAof5> BindAof5;
typedef Full2<impl::BindXof5> BindXof5;
typedef Full3<impl::Bind1and2of5> Bind1and2of5;
typedef Full3<impl::BindXandYof5> BindXandYof5;
typedef Full4<impl::Bind1and2and3of5> Bind1and2and3of5;
typedef Full4<impl::BindXandYandZof5> BindXandYandZof5;
typedef Full5<impl::Bind1and2and3and4of5> Bind1and2and3and4of5;
typedef Full5<impl::BindWandXandYandZof5> BindWandXandYandZof5;
typedef Full6<impl::Bind1and2and3and4and5of5> Bind1and2and3and4and5of5;

typedef Full2<impl::Bind1of6> Bind1of6;
typedef Full2<impl::BindXof6> BindXof6;
typedef Full3<impl::Bind1and2of6> Bind1and2of6;
typedef Full3<impl::BindXandYof6> BindXandYof6;
typedef Full4<impl::Bind1and2and3of6> Bind1and2and3of6;
typedef Full4<impl::BindXandYandZof6> BindXandYandZof6;
typedef Full5<impl::Bind1and2and3and4of6> Bind1and2and3and4of6;
typedef Full5<impl::BindWandXandYandZof6> BindWandXandYandZof6;
typedef Full6<impl::Bind1and2and3and4and5of6> Bind1and2and3and4and5of6;
typedef Full6<impl::BindVandWandXandYandZof6> BindVandWandXandYandZof6;
typedef Full7<impl::Bind1and2and3and4and5and6of6> Bind1and2and3and4and5and6of6;

typedef Full2<impl::Bind1of7> Bind1of7;
typedef Full2<impl::BindXof7> BindXof7;
typedef Full3<impl::Bind1and2of7> Bind1and2of7;
typedef Full3<impl::BindXandYof7> BindXandYof7;
typedef Full4<impl::BindXandYandZof7> BindXandYandZof7;
typedef Full5<impl::BindWandXandYandZof7> BindWandXandYandZof7;
typedef Full6<impl::BindVandWandXandYandZof7> BindVandWandXandYandZof7;
typedef Full7<impl::BindUandVandWandXandYandZof7> BindUandVandWandXandYandZof7;
typedef Full8<impl::Bind1and2and3and4and5and6and7of7> Bind1and2and3and4and5and6and7of7;

//#ifdef FCPP_CONCEPTS
// The promotion of CurryN to be a full functoid -
// only when using concepts!!
// This has been removed because of problems with use with concepts.
// Back to the old definitions in curry.h
//typedef Full5<impl::Curry4> Curry4;
//typedef Full6<impl::Curry5> Curry5;
//typedef Full7<impl::Curry6> Curry6;
//#endif

#else
// FIX THIS? I never noticed: the only 4-arg functoid in the entire library
typedef impl::Bind1and2and3of3 Bind1and2and3of3;
#endif

//#ifdef FCPP_CONCEPTS
//typedef Full2<impl::Curry1> Curry1;
//typedef Full3<impl::Curry2> Curry2;
//#ifdef FCPP152
//typedef Full4<impl::Curry3> Curry3;
//#else
// Cannot do Curry3 without Full4.
//using impl::Curry3;
//#endif
//#endif

typedef Full3<impl::Bind1and2of3> Bind1and2of3;
typedef Full3<impl::Bind2and3of3> Bind2and3of3;
typedef Full3<impl::Bind1and3of3> Bind1and3of3;
typedef Full2<impl::Bind1of3> Bind1of3;
typedef Full2<impl::Bind2of3> Bind2of3;
typedef Full2<impl::Bind3of3> Bind3of3;

FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Const const_;   // C++ keyword, so add trailing underscore
FCPP_MAYBE_EXTERN Bind1of1 bind1of1;

FCPP_MAYBE_EXTERN Bind1of2 bind1of2;
FCPP_MAYBE_EXTERN Bind2of2 bind2of2;
FCPP_MAYBE_EXTERN Bind1and2of2 bind1and2of2;

//#ifdef FCPP_EXTRA_STRONG_CURRY
FCPP_MAYBE_EXTERN Cycle2 cycle2;
FCPP_MAYBE_EXTERN Cycle3 cycle3;
FCPP_MAYBE_EXTERN Cycle3r cycle3r;
//#endif

FCPP_MAYBE_EXTERN Bind1and2and3of3 bind1and2and3of3;
FCPP_MAYBE_EXTERN Bind1and2of3 bind1and2of3;
FCPP_MAYBE_EXTERN Bind2and3of3 bind2and3of3;
FCPP_MAYBE_EXTERN Bind1and3of3 bind1and3of3;
FCPP_MAYBE_EXTERN Bind1of3 bind1of3;
FCPP_MAYBE_EXTERN Bind2of3 bind2of3;
FCPP_MAYBE_EXTERN Bind3of3 bind3of3;
// FC++.1.5.2
#ifdef FCPP152
FCPP_MAYBE_EXTERN Konst konst;   

FCPP_MAYBE_EXTERN Bind1and2and3and4of4 bind1and2and3and4of4;

FCPP_MAYBE_EXTERN Bind1and2and3of4 bind1and2and3of4;
FCPP_MAYBE_EXTERN Bind1and2and4of4 bind1and2and4of4;
FCPP_MAYBE_EXTERN Bind1and3and4of4 bind1and3and4of4;
FCPP_MAYBE_EXTERN Bind2and3and4of4 bind2and3and4of4;

FCPP_MAYBE_EXTERN Bind1and2of4 bind1and2of4;
FCPP_MAYBE_EXTERN Bind2and3of4 bind2and3of4;
FCPP_MAYBE_EXTERN Bind1and3of4 bind1and3of4;
FCPP_MAYBE_EXTERN Bind1and4of4 bind1and4of4;
FCPP_MAYBE_EXTERN Bind2and4of4 bind2and4of4;
FCPP_MAYBE_EXTERN Bind3and4of4 bind3and4of4;

FCPP_MAYBE_EXTERN Bind1of4 bind1of4;
FCPP_MAYBE_EXTERN Bind2of4 bind2of4;
FCPP_MAYBE_EXTERN Bind3of4 bind3of4;
FCPP_MAYBE_EXTERN Bind4of4 bind4of4;

FCPP_MAYBE_EXTERN Bind1of5 bind1of5;
FCPP_MAYBE_EXTERN Bind2of5 bind2of5;
FCPP_MAYBE_EXTERN BindAof5 bindAof5;
FCPP_MAYBE_EXTERN BindXof5 bindXof5;
FCPP_MAYBE_EXTERN Bind1and2of5 bind1and2of5;
FCPP_MAYBE_EXTERN BindXandYof5 bindXandYof5;
FCPP_MAYBE_EXTERN Bind1and2and3of5 bind1and2and3of5;
FCPP_MAYBE_EXTERN BindXandYandZof5 bindXandYandZof5;
FCPP_MAYBE_EXTERN Bind1and2and3and4of5 bind1and2and3and4of5;
FCPP_MAYBE_EXTERN BindWandXandYandZof5 bindWandXandYandZof5;
FCPP_MAYBE_EXTERN Bind1and2and3and4and5of5 bind1and2and3and4and5of5;

FCPP_MAYBE_EXTERN Bind1of6 bind1of6;
FCPP_MAYBE_EXTERN BindXof6 bindXof6;
FCPP_MAYBE_EXTERN Bind1and2of6 bind1and2of6;
FCPP_MAYBE_EXTERN BindXandYof6 bindXandYof6;
FCPP_MAYBE_EXTERN Bind1and2and3of6 bind1and2and3of6;
FCPP_MAYBE_EXTERN BindXandYandZof6 bindXandYandZof6;
FCPP_MAYBE_EXTERN Bind1and2and3and4of6 bind1and2and3and4of6;
FCPP_MAYBE_EXTERN BindWandXandYandZof6 bindWandXandYandZof6;
FCPP_MAYBE_EXTERN Bind1and2and3and4and5of6 bind1and2and3and4and5of6;
FCPP_MAYBE_EXTERN BindVandWandXandYandZof6 bindVandWandXandYandZof6;
FCPP_MAYBE_EXTERN Bind1and2and3and4and5and6of6 bind1and2and3and4and5and6of6;

FCPP_MAYBE_EXTERN Bind1of7 bind1of7;
FCPP_MAYBE_EXTERN BindXof7 bindXof7;
FCPP_MAYBE_EXTERN Bind1and2of7 bind1and2of7;
FCPP_MAYBE_EXTERN BindXandYof7 bindXandYof7;
FCPP_MAYBE_EXTERN BindXandYandZof7 bindXandYandZof7;
FCPP_MAYBE_EXTERN BindWandXandYandZof7 bindWandXandYandZof7;
FCPP_MAYBE_EXTERN BindVandWandXandYandZof7 bindVandWandXandYandZof7;
FCPP_MAYBE_EXTERN BindUandVandWandXandYandZof7 bindUandVandWandXandYandZof7;
FCPP_MAYBE_EXTERN Bind1and2and3and4and5and6and7of7 bind1and2and3and4and5and6and7of7;


// When concepts are in use then the following are here instead of in curry.h
// They are being phased out in function.h in favour of calls to the correct binder.
//#ifdef FCPP_CONCEPTS
//FCPP_MAYBE_EXTERN Curry4 curry4; 
//FCPP_MAYBE_EXTERN Curry5 curry5; 
//FCPP_MAYBE_EXTERN Curry6 curry6; 
//#endif

#endif

//#ifdef FCPP_CONCEPTS
//FCPP_MAYBE_EXTERN Curry1 curry1, curry; // "curry" is the same as "curry1"
//FCPP_MAYBE_EXTERN Curry2 curry2;
//FCPP_MAYBE_EXTERN Curry3 curry3; 
//#endif

FCPP_MAYBE_NAMESPACE_CLOSE

} // end namespace fcpp

#endif
