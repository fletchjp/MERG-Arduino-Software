//////////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Functors
// Moving code from functors.cpp
// This is being done in the knowledge of some things I did not know then.
// In particular I now have monoid code available and in particular
// Endo and MonoidEndo which enable lists of polymorphic functoids.
// Examples are in PicoRP2040FCPPMonoids.
// I have copied all the previous comments here unchanged from functors.cpp
// This relates closely to things about Monads in
// https://wiki.haskell.org/Typeclassopedia
// which has been a source of much enlightenment for me.
//////////////////////////////////////////////////////////////////////
//
// Developments of FC++ functors.
//
/////////////////////////////////////////////////////////////////////////////

// See also the file transformers.cpp which has monad transformers.

// There are several starting points for the new ideas.

// My own is my wish to integrate more of my previous work on arrows and
// comonads. I had done some things a few years ago and did not really
// understand what I had been doing.

// In recent work I came across some papers by among others Ross Patterson
// of City University London. He used the word "Idioms" which are also
// referred to as "Applicative Functors".  This work has Haskell examples.

// I also have one of many papers from Bartosz Milewski under the name
// "C++ Ranges are Pure Monadic Goodness". This discusses a series of steps.
// Functor -> Pointed Functor -> Applicative Functor -> Monad
// with examples in Haskell and C++.
// I was considering starting a file which would implement the examples from
// Bartosz' paper.

// I then found "Learn You a Haskell for Great Good!" by Miran Lipovaca.
// This book is much easier to read than "Real World Haskell".
// Reading it I am making much more sense of the Haskell notation.
// I also found an explanation of Applicative Functors (Chapter 11) which
// made sense for me and helped with understanding the other material.

// This gives considerable scope to add more things to FC++ to round it out.
// It turns out that Maybe<T> and Either<T> are particular instances
// of an Applicable<T> type which is more general.
// It turns out that the <T> need not be a data type but can also be a functor.
// This is potentially powerful and needs thought in the FC++ context.

// This is one example of how the FC++ system could be made both more complete
// and more consistent.

// I have started this by building a join for inferrable monads which is now
// available for use in fcpp/monad.h

// Page references are to "Learn You a Haskell for Great Good!"
// unless otherwise noted.

//////////////////////////////////////////////////////////////////////////
// fmap was added as a synonym for FC++ compose and dot (page 221)
// # I now think this may not be correct.
// # I think it needs to be resolved with the functor laws.
// # I have now removed this definition of fmap.

// I am exploring the interaction between fmap and Maybe<T>.
// I need to do this:
// dot (id, id ) ( just(3))
//
// rather than this which also compiles:
// liftM<MaybeM>() (id) ( just(3))

// See examples at the start of output from this file.

// This means that fmap is a version of liftM<MaybeM>() which can infer the
// type of the input. I do not have this currently.

// I have found a sketch of what it could be in sugar.cpp
// and copied it to unify.cpp.
//
// Light dawns. fmap is a generalisation of map which applies only to
// List<T>.
//
//        fmap :: (a -> b) -> f a -> f b
//
// f here is the type of the first argument and gives the type to be made.
// So what is needed is a generalisation of map to cover different types.
//
// I think that implies that anything to be such should inherit from a new
// type e.g. Functor which provides the property.
//
// I think if I can manage this I will have something powerful and general
// which will make a lot of other things easier to do.
//
// This section added 2/3/2016. What is below follows on from above this.
/////////////////////////////////////////////////////////////////////////

// That has lead me to reflect on the definition of Maybe<T> which is not
// a functoid and explore MaybeA as an Applicative Functor.
// I have given this an internal functoid 'pure' and want to create
// the inferrable 'pure' as well. (See more notes below.)

// One question is how to handle the 'nothing' case.
// I have defined a functoid 'nothing' which ignores its argument
// and returns 'NOTHING'. That helps with a polymorphic environment.

// I need polymorphism if the Maybe type is to hold functoids e.g (plus(3))
// which has not been the case before.

// I also need a C++ name for the Haskell <*> which is a 'beefed up fmap'.
// I have chosen 'star' for this name.

// "Something between a large bathing machine and a very small second class
// carriage." (G&S patter song in Iolanthe)

//////////////////////////////////////////////////////////////////////////
// Inferrable pure is a problem.
//////////////////////////////////////////////////////////////////////////
// The problem is that in many cases it is not known what it should infer.
// This can be remedied in most cases when it is used around an argument
// to star, when what it is can be inferred from the other argument.
// To get to that I have put in a placeholder of id.
// This works in many cases but not with ListA.
// New idea. Make id a default case with a hidden argument.
// Experiment with pure2 carried out.
// There is now a version called pureL which works with ListA.
// Unfortunately I do not yet have a way in which it can be called
// silently. I will think about that.
//
//     example    :    pureL(inc) ^star^ (l12)
//     equivalent :    map(inc,l12)
//
// I have now done pureM and pureE for MaybeA and EitherA.
//
// So I now have a situation where 'pure' will work in some cases where
// the correct action can be deduced. Where the action needed is known the
// user can give a steer by using the correct one, as follows:
//
// pureE  for EitherA and Either<T>
// pureL  for ListA   and List<T>
// pureM  for MaybeA  and Maybe<T>
//
/////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// functors.cpp
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// This file contains a lot of examples of the use of the
// applicative functors which I have developed as an extension of FC++.
// It also has examples of the use of the functors as FC++ monads.
// These actions are activated when FCPP_UNIFY is defined.
// The aim of this is to make it such that the applicative functor
// can be used wherever the monad can be used  e.g. MaybeA for MaybeM
//
// John Fletcher December 2016
//////////////////////////////////////////////////////////////////////////

// I now have a star functoid which works both with a named applicative functor
// and can infer the applicative functor.

// There are now three ways to access the star in a functor:
// 1. ApplicativeA::star()(arg1,arg2)
// 2. starA<ApplicativeA>()(arg1,arg2)
// 3. star (arg1,arg2)

// Only for the last one, the functoid can be inferred from one of the
// arguments and will be applied to the other one.

// A lot of functoids have now been moved to a new file fcpp/functors.h
// which means they can now be used in other files.

// I have now moved MaybeA to  fcpp/functors.h.

// I now also made and moved ListA.

// There is an important limitation on ListA. It is necessary to make a list
// of functors to use it. FC++ List<T> only supports a list of items of all
// the same type. Things like inc and dec are different types so that
// e.g. makeList2(inc,dec) will not compile.

// They can however be used in the monomorphic form by declaring e.g.
// Fun1<int,int> finc(inc), fdec(dec);

// Then this will work:
// List<Fun1<int,int> > flist = makeList2(finc,fdec);

// See examples lower down this file.

// This limitation will not be a problem as the arguments have to be lists
// of all the same type anyway.

////////////////////////////////////////////////////////////////////////////////
/// New note: this is where Endo and MonoidEndo will be of help.
/// This is one of the things I plan to explore.
/////////////////////////////////////////////////////////////////////////////////

// I have also constructed EitherA.

// Note that in my implementation the left value is of type EitherError.
// This is a wrapper around std::string, needed in case the real data is also
// std::string.

// I am looking at some ways of using it which involve pairs of functors.

// I think the theory should then lead me to Arrows and Comonads.

// Work in progress.

// In particular, the proposed transformer for applicative functors,
// maybeF, is incomplete in that I have not been able to define a
// transformation for the star operation.

// I am wondering whether I need the separate implementations of
// Applicable Functors or whether with a bit more thought and
// understanding I can integrate this with FC++ Monads.

// See files unify.cpp and category_fcpp.cpp for more examples.

// John Fletcher  February and March 2016

// November 2016 : I have done some updates to solve a problem with the
// debug output from inside the functor transformers MaybeF and EitherF.
// The following did not give any useful output.
// sprint(MaybeF<ListA>::pure())
// The solution is to provide a debug_trait which will call the correct
// name function.
// The trait passes to the name function the name of the functor.
// This is needed as the functor cannot be decoded from a nested
// template.
/*
    template<>
    class debug_traits<typename MaybeF<ListA>::XPure >
       {
       public:
         static const bool has_name = true;
         static std::string name(const typename MaybeF<ListA>::XPure &f) { return f.name("ListA"); }
       };

      std::string name(const std::string& functor) const
      {
         return std::string("MaybeF<") + functor + std::string(">::Pure");
      }
*/

////////////////////////////////////////////////////////////////
// NEW NOTE: I am not using the traits yet in the RP2040 work.
////////////////////////////////////////////////////////////////

// ========================
// headers
// ========================

#include <string>
#include <vector>
#include <algorithm>

// ========================
// FC++ headers
// ========================

#define FCPP152
#define FCPP_ENABLE_LAMBDA
#define FCPP_EXTRA_STRONG_CURRY
#define FCPP_PATTERN // define this to include pattern.h
//#define FCPP_USE_EXCEPTIONS
#define FCPP_UNIFY // Unification of Monads and Functors in functors.h
#include "fcpp_prelude.h"
#include "fcpp/monoids.h"
#include "fcpp/functors.h"
#include "fcpp/comonad.h"
#include "fcpp/arrow.h"

#include <Streaming.h>

using namespace fcpp;

/// I have renamed String to StringL to avoid a name clash with the Arduino use of String.
/// This is used with the Parser monad and other codes.
typedef List<char> StringL;

namespace fcpp {
/// Space for new functoids.

  ////////////////////////////////////////////////////////////////////////
  // This is an adaption of the code for inferrable monads.
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // This allows star to be inferred for the MaybeA applicative functor.
  //
  // Idea: Use either argument to infer pure for the other one!
  //       This would allow such things as:
  //
  //       star( pure(plus(2)), just(4))
  //
  //       rather than this which already works.
  //
  //       star ((MaybeA::pure()(plus(2))), just(4))
  //
  // These have now been implemented using a series of helper structs.
  // These are in order with the deepest level first.
  // I have attempted to keep the top level code as clean as possible.
  ////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////
    // I want this to detect whether one of the arguments to star
    // is an applicative functor or a suitable type.
    // If the other argument is pure then convert it.
    // e.g.
    // star (pure inc, just(2) )         -> star(MaybeA::pure()(inc),just(2))
    // star (MaybeA::pure()(inc),pure 2) -> star(MaybeA::pure()(inc),just(2))
    //
    // If this works it could also be inferred within
    // an applicative functor's star as well.
    //
    // Note that Applicative::pure applies a functor and could be useful.
    //
    ///////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////
    // StarTypeHelper sorts out applicative types.
    ////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////
    //  struct StarHelper
    // This replaces ApplicativeHelper and give the same result
    // for two arguments neither of which is pure(x).
    // For (a,pure(x)) it will use the type of a.
    // for (pure(f),b) it will use the type of b.
    // for (a,b) it will us Ids on both (not yet available).
    //
    // Note the availability of Applicative::pure()(f,x)
    //
    // The Type calculation needs to be like this:
    // Given A and K
    // if both A and K are applicative -> use A
    // if only A       is  applicative -> use A
    // if only K       is  applicative -> use K
    //
    // The return type is the same in each case.
    //
    // StarHelper works with Applicative types.
    //
    ////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////
  // pureA<Functor>() and starA<Functor>() implemented as in Monads.
  ////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  // Applicative - see p.228
  //
  // class (Functor f) => Applicative f where
  //   pure :: a -> f a
  //  (<*>) :: f (a -> b) -> f a -> f b
  //
  // Note: fmap :: (a -> b) -> f a -> f b
  //
  // I need a name for <*> which I will call 'star'
  // This is difficult to construct in general as what is needed
  // is a way to get to (a -> b) from within f (a -> b)
  // (I think that this is a comonad!!)
  //
  // Here MaybeA is an applicable version of Maybe.
  //
  // The most significant aspect of this is the idea to store
  // functoids (functors) in Maybe types and then apply them to other
  // Maybe objects.
  //
  // This works (see examples) but the grammar is clunky.
  // I have now made this inferrable for 'star'.
  // I want to do the same for 'pure' if a context can be established.
  //
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  // Working with Either/EitherM/EitherA
  //
  // In all cases my implementation uses EitherError for the left()
  // value. In the file testeither.cpp there are a lot of examples of
  // functoids to implement various examples. These predate the current work
  // on applicable functors. I think what is needed is something more general
  // which will take both an operator and a predicate to test the result.
  //
  // constraint(op,test)
  //
  // This will apply op to the argument(s) and test the result with (op).
  // There could be a version which did:
  //
  // constraint(op,test,message) where the message is the failure message.
  //
  // The result is an Either containing an operation and a message.
  // Perhaps what I want is a standard pair with an operand and a test.
  //
  /////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// The code described above has been moved to fcpp/functors.h
//////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Copied from testeither.cpp, named as follows,
// the no indicates the no of arguments
// and the letters indicates a test on the first (x) or second (y) argument.
//
// These are modified here so that return types are not assumed to be the
// same as the inputs.
// The input parameters are assumed to be all the same type for 2 and 3
// parameter cases.
//
// Versions defined:
// constrain1x   (modified RTs and argument order)
// constrain2x   (modified RTs)
// constrain2y   (modified RTs)
// constrain2xy  (modified RTs)
// constrain3xyz (modified RTs)
// Also change name to constrain as that is what it does to the
// functoid which is called.
// Note that constrain3xyz is a Full6 and needs a 7 parameter lambda.
// This is approaching or on the limit of what is available in FC++.

namespace impl {
  struct XConstrain1x {
     // OP will be an FC++ functoid taking one operand of type T
     // B=boolean function with operand x
     // and S=std::string
     // If the boolean function is true this indicates failure
     // and the string will be returned as an error message.
     // I have moved T to the end to ease currying.
     // I have also made the return type be
     // Either<Type of the result of op(x)> instead of Either<T>.
     template <class OP, class B, class E, class T > struct Sig :
      public FunType<OP,B,E,T,Either<typename RT<OP,T>::ResultType> > {};
   
     template <class OP, class B, class E, class T >
     typename Sig<OP,B,E,T>::ResultType
      operator()(const OP& op, const B& test,
                 const E& message, const T& x) const {
       typedef typename Sig<OP,B,E,T>::ResultType R;
       if ( test(x) ) {
          R result ( message );
          return result;
        } else {
          typename RT<OP,T>::ResultType r = op(x);
          R result ( r );
          return result;
        }
      }
   };
}
typedef Full4<impl::XConstrain1x> Constrain1x;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Constrain1x constrain1x;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
  struct XConstrain2y {
     // OP will be an FC++ functoid taking two operands of type T
     // B=boolean function with operand y
     // and E=EitherError
     // If the boolean function is true this indicates failure
     // and the string will be returned as an error message.
     // I have made the return type be
     // Either<Type of the result of op(x,y)> instead of Either<T>.
     template <class OP, class T, class U, class B, class E> struct Sig;
     template <class OP, class T, class B, class E > struct Sig<OP,T,T,B,E> :
      public FunType<OP,T,T,B,E,Either<typename RT<OP,T,T>::ResultType> > {};
   
     template <class OP, class T, class B, class E >
     typename Sig<OP,T,T,B,E>::ResultType
      operator()(const OP& op, const T& x,const T& y, const B& test,
        const E& message) const {
       typedef typename Sig<OP,T,T,B,E>::ResultType R;
       if ( test(y) ) {
          R result ( message );
          return result;
        } else {
          typename RT<OP,T,T>::ResultType r = op(x,y);
          R result ( r );
          return result;
        }
      }
   };
}
typedef Full5<impl::XConstrain2y> Constrain2y;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Constrain2y constrain2y;
FCPP_MAYBE_NAMESPACE_CLOSE


namespace impl {
  struct XConstrain2x {
     // OP will be an FC++ functoid taking two operands of type T
     // B=boolean function with operand x
     // and E=EitherError
     // If the boolean function is true this indicates failure
     // and the string will be returned as an error message.
     template <class OP, class T, class U, class B, class E> struct Sig;
     template <class OP, class T, class B, class E > struct Sig<OP,T,T,B,E> :
       public FunType<OP,T,T,B,E,Either<typename RT<OP,T,T>::ResultType> > {};
   
     template <class OP, class T, class B, class E >
     typename Sig<OP,T,T,B,E>::ResultType
      operator()(const OP& op, const T& x,const T& y, const B& test,
        const E& message) const {
       typedef typename Sig<OP,T,T,B,E>::ResultType R;
       if ( test(x) ) {
          R result ( message );
          return result;
        } else {
          typename RT<OP,T,T>::ResultType r = op(x,y);
          R result ( r );
          return result;
        }
      }
   };
}
typedef Full5<impl::XConstrain2x> Constrain2x;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Constrain2x constrain2x;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
  struct XConstrain2xy {
     // OP will be an FC++ functoid taking two operands of type T
     // B=boolean function with operands x and y
     // and E=EitherError
     // If the boolean function is true this indicates failure
     // and the string will be returned as an error message.
     template <class OP, class T, class U, class B, class E> struct Sig;
     template <class OP, class T, class B, class E > struct Sig<OP,T,T,B,E> :
       public FunType<OP,T,T,B,E,Either<typename RT<OP,T,T>::ResultType> > {};
   
     template <class OP, class T, class B, class E >
     typename Sig<OP,T,T,B,E>::ResultType
      operator()(const OP& op, const T& x,const T& y, const B& test,
        const E& message) const {
       typedef typename Sig<OP,T,T,B,E>::ResultType R;
       if ( test(x,y) ) {
          R result ( message );
          return result;
        } else {
          typename RT<OP,T,T>::ResultType r = op(x,y);
          R result ( r );
          return result;
        }
      }
   };
}
typedef Full5<impl::XConstrain2xy> Constrain2xy;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Constrain2xy constrain2xy;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
  struct XConstrain3xyz {
     // OP will be an FC++ functoid taking three operands of type T
     // B=boolean function with operands x y and z
     // and E=EitherError
     // If the boolean function is true this indicates failure
     // and the string will be returned as an error message.
    template <class OP, class T, class U, class V, class B, class E> struct Sig;
    template <class OP, class T, class B, class E > struct Sig<OP,T,T,T,B,E> :
      public FunType<OP,T,T,T,B,E,Either<typename RT<OP,T,T,T>::ResultType> > {};
   
     template <class OP, class T, class B, class E >
     typename Sig<OP,T,T,T,B,E>::ResultType
     operator()(const OP& op, const T& x,const T& y, const T& z,const B& test,
        const E& message) const {
       typedef typename Sig<OP,T,T,T,B,E>::ResultType R;
       if ( test(x,y,z) ) {
          R result ( message );
          return result;
        } else {
          typename RT<OP,T,T,T>::ResultType r = op(x,y,z);
          R result ( r );
          return result;
        }
      }
   };
}
typedef Full6<impl::XConstrain3xyz> Constrain3xyz;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Constrain3xyz constrain3xyz;
FCPP_MAYBE_NAMESPACE_CLOSE


  /////////////////////////////////////////////////////////////////////////
  // EitherA is an applicable version of Either. (moved to fcpp/functors.h)
  /////////////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////
  // Functor Transformer MaybeF puts Maybe on the inside.
  // Usage is MaybeF<Functor>
  // which is a version of MaybeT transformer for monads.
  // (moved to fcpp/functors.h)
  ///////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////
  // EitherF functor transformer following MaybeF
  // (moved to fcpp/functors.h)
  //////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  // ListA Applicative operations on List<T>.
  // The functors in lists will have to be monomorphic.
  // The type will be Fun1<T,T> where T is the element type.
  // Otherwise the types are different and cannot be in a List type.
  // (moved to fcpp/functors.h)
  ///////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  // Applicative - see p.228
  //
  // class (Functor f) => Applicative f where
  //   pure :: a -> f a
  //  (<*>) :: f (a -> b) -> f a -> f b
  //
  // Note: fmap :: (a -> b) -> f a -> f b
  //
  // I need a name for <*> which I will call 'star'
  // This is difficult to construct in general as what is needed
  // is a way to get to (a -> b) from within f (a -> b)
  // (I think that this is a comonad!!)
  //
  /////////////////////////////////////////////////////////////////////////

  // If templated could it make functors applicative on the fly?
  // Should this be templated or inherited?
  //template <typename Functor>
    struct Applicative {
#ifdef FCPP_DEBUG
     std::string name() const
     {
       return std::string("Applicative");
     }
#endif
      //typedef Functor F;

      //
      //template <typename F,typename A>
      struct XPure {
        template <typename F,class A>
        struct Sig : public FunType<F,A,typename RT<F,A>::ResultType> {};
        template <typename F, typename A>
        typename Sig<F,A>::ResultType
        operator() (const F& f,const A& a) const
        {
          return f(a);
        }
      };
      typedef Full2<XPure> Pure;
      static Pure& pure() {static Pure f; return f;}

      // I want to put something generic in here as well.
      struct XStar {

      };


    };

} // namespace fcpp

Print &operator <<( Print &obj, const std::string &arg)
{
    obj.print(arg.c_str());
    return obj; 
}

Print &operator <<( Print &obj, const Maybe<int> &arg)
{
    if (arg.is_nothing()) { 
       obj.print(" nothing ");
    } else {
       Serial << "just ( ";
       obj.print(arg.value());
       Serial << " )";
    }
    return obj; 
}

Print &operator <<( Print &obj, const Either<int> &arg)
{
    if (arg.is_error()) { 
       Serial << "left ( ";
       obj.print(arg.left().c_str());
       Serial << " )";
     } else {
       Serial << "right ( ";
       obj.print(arg.right());
       Serial << " )";
    }
    return obj; 
}

//////////////////////////////////////////////////////////

void functor_examples()
{
  Serial << "dot is a synonym for FC++ compose" << endl;
  Serial << "----------------------------------------------------"
            << endl;
  Serial << "dot(multiplies(3),plus(100))(1)    : "
            << dot(multiplies(3),plus(100))(1) << " (page 221)" << endl;
  Serial << "(multiplies(3) ^dot^ plus(100))(1)  : "
            << (multiplies(3) ^dot^ plus(100))(1) << endl;
  Serial << "dot (id, id ) ( just(3)) : " << dot (id, id ) ( just(3))
            << " needs repeat of id!" << endl;
  Serial << "id ( just(3) )            : " << id ( just(3) )
            << " (page 224)"<< endl;
  Serial << "liftM<MaybeM>()(id)( just(3) )  : "
            <<  liftM<MaybeM>()(id)( just(3) ) << endl;
  Serial << "liftM<MaybeM>()(inc)( just(3) ) : "
            <<  liftM<MaybeM>()(inc)( just(3) ) << endl;
  Maybe<int> mj = joinM<MaybeM>()( just (just (2) ) );
  Serial << "joinM<MaybeM>()( just (just (2) ) ) : " << mj << endl;
  Serial << "----------------------------------------------------"
            << endl;
  Serial << "MaybeM was the only FC++ monad with an explicit join."
            << endl;
  Serial << "There is now an explicit join for EitherM."
            << endl;
  Serial << "----------------------------------------------------"
            << endl;
  Maybe<int> mj2 = MaybeM::join()( just (just (2) ) );
  Serial << "MaybeM::join()( just (just (2) ) )  : " << mj2 << endl;
  Serial << "--------------------------------------" << endl;
  Either<int> ej = (joinM<EitherM>())( right (right (2) ) );
  Serial << "joinM<EitherM>()( right (right (2) ) ) : " << ej
            << endl;
  Either<int> el = liftM<EitherM>()(inc) (right (2) );
  Serial << "liftM<EitherM>()(inc) (right (2) )     : " << el
            << endl;
}

void functor_examples2()
{
  Serial << "===============================================" << endl;
  Serial << "First use of Applicative Functors." << endl;
  Serial << "===============================================" << endl;
#ifdef FCPP_UNIFY
  Serial << "These are monad uses being unified into MaybeA" << endl;
  Serial << "--------------------------------------" << endl;
  Serial << "liftM<MaybeA>()(inc)( just(3) ) : "
            <<  liftM<MaybeA>()(inc)( just(3) ) << endl;
  Maybe<int> mja = joinM<MaybeA>()( just (just (2) ) );
  Serial << "joinM<MaybeA>()( just (just (2) ) ) : " << mja << endl;
  Maybe<int> mj2a = MaybeA::join()( just (just (2) ) );
  Serial << "MaybeA::join()( just (just (2) ) )  : " << mj2a << endl;
  Serial << "--------------------------------------" << endl;
  Either<int> eja = joinM<EitherA>()( right (right (2) ) );
  Serial << "joinM<EitherA>()( right (right (2) ) ) : " << eja
            << endl;
  Either<int> ela = liftM<EitherA>()(inc) (right (2) );
  Serial << "liftM<EitherA>()(inc) (right (2) )     : " << ela
            << endl;
  Serial << "--------------------------------------" << endl;
#endif
  Serial << "MaybeA::pure()(2)         : " << MaybeA::pure()(2) << endl;
  Serial << "MaybeA::star()((MaybeA::pure()(inc)),just(4) : "
            << MaybeA::star()((MaybeA::pure()(inc)),just(4)) << endl;
  Serial << "MaybeA::star()(MaybeA::pure()(plus(2)),just(4)) : "
            << MaybeA::star()(MaybeA::pure()(plus(2)),just(4)) << endl;
  Serial << "starA<MaybeA>()(pureA<MaybeA>()(plus(2)),just(4)) : "
            << starA<MaybeA>()(pureA<MaybeA>()(plus(2)),just(4)) << endl;
  //Serial << "starA<MaybeA>()(pureA<MaybeA>()(plus(2)),pure(4)) : "
  //          << starA<MaybeA>()(pureA<MaybeA>()(plus(2)),pure(4)) << endl;
  Serial << "===============================================" << endl;
  Serial << "First use of inferred Applicative Functors." << endl;
  Serial << "===============================================" << endl;
  Serial << "star ((MaybeA::pure()(inc)),just(4) : "
            << star ((MaybeA::pure()(inc)),just(4)) << endl;
  Serial << "star (pureA<MaybeA>()(inc),just(4)) : "
            << star (pureA<MaybeA>()(inc),just(4)) << endl;
  Serial << "nothing(2)                  : " << nothing(2) << endl;
  Serial << "Applicative::pure()(just,2) : " << Applicative::pure()(just,2)
            << endl;
  Serial << "pure(just(2))               : " << pure(just(2)) << endl;
  // These are now implemented.
  Serial << "star (MaybeA::pure()(inc),pure(4)) : "
            <<  star (MaybeA::pure()(inc),pure(4)) << endl;
  Serial << "star (pureA<MaybeA>()(inc),pure(4)) : "
            <<  star (pureA<MaybeA>()(inc),pure(4)) << endl;
  Serial << "star (pure(inc), just(4)) : "
            << star ( pure(inc), just(4)) << endl;
  Serial << "star can infer that inc should be in MaybeA!" << endl;
  Serial << "star (inc, just(4)) : "
            << star ( inc, just(4)) << endl;
  Serial << "MaybeA::star()((MaybeA::pure()(plus(2))),just(4)) : "
             << MaybeA::star()((MaybeA::pure()(plus(2))),just(4)) << endl;
  Serial << "MaybeA::star()((MaybeA::pure()(plus(2))),pure2(just,4)) : "
      << MaybeA::star()((MaybeA::pure()(plus(2))),pure2(just,4))
            << endl;
  Serial << "MaybeA::star()((MaybeA::pure()(plus(2))),pure2(just)(4)) : "
      << MaybeA::star()((MaybeA::pure()(plus(2))),pure2(just)(4))
            << endl;
  Serial << "===============================================" << endl;
  Serial << "Examples from p.230 and extensions" << endl;
  Serial << "===============================================" << endl;
  Serial << "star (pure(plus(3)),just(10))   : "
            << star (pure(plus(3)),just(10)) << endl;
  Serial << "===============================================" << endl;
  Serial << "Brackets needed to chain from the right with functors."
            << endl;
  Serial << "===============================================" << endl;
  Serial << "star(pure(plus(3)),star(pureA<MaybeA>()(inc),just(10))) : "
            <<  star(pure(plus(3)),star(pureA<MaybeA>()(inc),just(10)))
            << endl;
  Serial << "star2a(pure(plus(3)),pureA<MaybeA>()(inc),just(10)) : "
            <<  star2a(pure(plus(3)),pureA<MaybeA>()(inc),just(10))
            << endl;
   // This cannot be output directly (operator precedence?)
  Maybe<int> mx = pure(plus(3)) ^star^ (just(10));
  Serial << "pure(plus(3)) ^star^ (just(10)) : " << mx << endl;
  Serial << "star(pure(plus(3)),just(10))    : "
            << star(pure(plus(3)),just(10)) << endl;
  // InfixOpThingy cannot cope with this.
  // The grouping must be from the right.
  //Maybe<int> my0 = (pure(plus(3)) ^star^ pureA<MaybeA>()(inc)) ^star^ (just(10));
  Maybe<int> my = pure(plus(3)) ^star^ (pureA<MaybeA>()(inc) ^star^ (just(10)));
  Serial << "pure(plus(3)) ^star^ (pureA<MaybeA>()(inc)) ^star^ (just(10))) : "
            << my << endl;
  Maybe<int> mz = (pure(plus(3)) ^star^ nothing(2));
  Serial << "pure(plus(3)) ^star^ nothing(2) : "
            << mz << endl;

  Maybe<int> my2 = star(star(pure(plus),just(2)),just(4));
  Serial << "star(star(pure(plus),just(2)),just(4)) : "
            << my2 << endl;
  Serial << "===============================================" << endl;
  Serial << "O.K. to chain from the left with values." << endl;
  Serial << "pure and pureM both work in expressions." << endl;
  Serial << "===============================================" << endl;
  Maybe<int> my3 =(pure(plus) ^star^ just(2)) ^star^ (just(4));
  Serial << "(pure(plus) ^star^ just(2)) ^star^ (just(4) : "
            << my3 << endl;
  Maybe<int> my4 = pure(plus) ^star^ just(2) ^star^ (just(4));
  Serial << " pure(plus) ^star^ just(2)  ^star^ (just(4) : "
            << my4 << endl;
  Maybe<int> my5 = (pure(plus) ^star2^ just(2)) (just(4));
  Serial << "(pure(plus) ^star2^ just(2)) (just(4)) : "
            << my5 << endl;
  Serial << "===============================================" << endl;
  Serial << "Experiments with ListA." << endl;
  Serial << "===============================================" << endl;
  List<int> l1 = ListA::pure()(1);
  List<int> l2 = pureA<ListA>()(2);
  //List<int> l3 = starA<ListA>() (pureA<ListA>()(inc),l2);
  List<int> l12 = list_with(1,2);
  Serial << "===============================================" << endl;
  Serial << "Development of a version of pure called pureL"   << endl;
  Serial << "pureL replaces pure for cases involving ListA"   << endl;
  Serial << "NOTE: delay and map have to be fcpp::delay and fcpp::map" << endl;
  Serial << "===============================================" << endl;
  List<int>::iterator li;
  List<int> l23 = pureA<ListA>()(inc) ^star^ l12;
  Serial << "List<int> l23    = pureA<ListA>()(inc) ^star^ l12 : [ ";
  for (li= l23.begin(); li!=l23.end(); ++li)
    {
      Serial << *li << " ";
    }
  Serial << "]" << endl;
  List<int> l23aa = star(pureA<ListA>()(inc),l12);
  List<int> l23ab = star(ListA::pure()(inc),l12);
  // List is a special case as pure does id while the others make a List
  // This is a special version of pure called pure2 which takes an argument
  // to tell pure what to apply internally.
  List<int> l23ac = star(pure2(pureA<ListA>())(inc),l12);
  Serial << "List<int> l23ac  = star(pure2(pureA<ListA>())(inc),l12) : [ ";
  for (li= l23ac.begin(); li!=l23ac.end(); ++li)
    {
      Serial << *li << " ";
    }
  Serial << "]" << endl;
  // No member named delay in binder2of2.
  // The problem is that cons returns OddList not List.
  //List<int> l23ad = star(pure2(cons(_,NIL).delay())(inc),l12);
  // .delay() has to come after the binder is satisfied.
  List<int> l23ad = star((pure2(cons(_,NIL))(inc)).delay(),l12);
  List<int> l23ae = star(cons(inc,NIL).delay(),l12);
  List<int> l23af = star(fcpp::delay(cons(inc,NIL)),l12);
  List<int> l23ag = star((fcpp::delay ^dot^ cons)(inc,NIL),l12);
  List<int> l23ag2 = star((fcpp::delay ^dot^ cons)(_,NIL)(inc),l12);
  List<int> l23ag3 = (fcpp::delay ^dot^ cons)(_,NIL)(inc) ^star^ (l12);
  Serial << "List<int> l23ag3 = (fcpp::delay ^dot^ cons)(_,NIL)(inc)^star^(l12) : [ ";
  for (li= l23ag3.begin(); li!=l23ag3.end(); ++li)
    {
      Serial << *li << " ";
    }
  Serial << "]" << endl;
  // I have made pureL which should be used with ListA
  // instead of just pure.
  List<int> l23ag4 = pureL(inc) ^star^ (l12);
  Serial << "List<int> l23ag4 = pureL(inc) ^star^ (l12) : [ ";
  for (li= l23ag4.begin(); li!=l23ag4.end(); ++li)
    {
      Serial << *li << " ";
    }
  Serial << "]" << endl;
  // This is the easy way to do this.
  List<int> l23ah = fcpp::map(inc,l12);
  Serial << "List<int> l23ah  = map(inc,l12) : [ ";
  for (li= l23ah.begin(); li!=l23ah.end(); ++li)
    {
      Serial << *li << " ";
    }
  Serial << "]" << endl;
  Maybe<int> m2a = star(pure(inc),just(3));
  // Use fmap for things other than lists as well as lists.
  List<int> l23aj = fmap(inc,l12);
  Maybe<int> m2b  = fmap(inc,just(3));
  Either<int> e2b = fmap(inc,right(3));
  List<int> l2a = pureL(2);
  Serial << "===============================================" << endl;
  Serial << "Experiments with pureA<MaybeF<ListA> >()." << endl;
  Serial << "This uses a functor transformer on a functors" << endl;
  Serial << "===============================================" << endl;
  List<Maybe<int> > lm2 = pureA<MaybeF<ListA> >()(just(2));
  List<Maybe<int> > lm2a = pureA<MaybeF<ListA> >()(pureM(2));
  List<Maybe<int> >::iterator lmi;
  Serial << "pureA<MaybeF<ListA> >()(just(2))  : [ ";
  for (lmi= lm2.begin(); lmi!=lm2.end(); ++lmi)
    {
      Serial << *lmi << " ";
    }
  Serial << "]" << endl;
  Serial << "pureA<MaybeF<ListA> >()(pureM(2)) : [ ";
  for (lmi= lm2a.begin(); lmi!=lm2a.end(); ++lmi)
    {
      Serial << *lmi << " ";
    }
  Serial << "]" << endl;
  List<Maybe<int> > lm12 = list_with(just(1),just(2));
  Fun1<int,int> finc(inc), fdec(dec), fp2(plus(2));
  Serial << "===============================================" << endl;
  Serial << "Experiments with starA<ListA>()." << endl;
  Serial << "This uses a list of monomorphic functors." << endl;
  Serial << "===============================================" << endl;
  //Serial << sprint(makeList2(inc,dec)) << endl; This fails.
  List<Fun1<int,int> > lf2 = makeList3(finc,fdec,fp2);
  List<int> res = starA<ListA>() (pureA<ListA>()(finc))(l12);
  int res2 = head(tail(res));
  Serial << "Fun1<int,int> finc(inc), fdec(dec), fp2(plus(2));"
            << endl;
  Serial << "List<Fun1<int,int> > lf2 = makeList3(finc,fdec,fp2);"
            << endl;
  Serial << "starA<ListA>() (pureA<ListA>()(finc))(l12) : ";
  for (li= res.begin(); li!=res.end(); ++li)
    {
      Serial << *li << " ";
    }
  Serial << endl;
  List<int> res3 = fcpp::map(finc,l12);
  Serial << "map(finc,l12) : ";
  for (li= res3.begin(); li!=res3.end(); ++li)
    {
      Serial << *li << " ";
    }
  Serial << endl;
  Serial << "(*lfi)(2) : ";
  List<Fun1<int,int> >::iterator lfi;
  for (lfi= lf2.begin(); lfi!=lf2.end(); ++lfi)
    {
      Serial << (*lfi)(2) << " ";
    }
  Serial << endl;
  Serial << "lres = cat(lres,map(*lfi)(l12)) : ";
  List<int> lres;
  for (lfi= lf2.begin(); lfi!=lf2.end(); ++lfi)
    {
      lres = cat(lres,fcpp::map(*lfi)(l12));
    }
  for (li= lres.begin(); li!=lres.end(); ++li)
    {
      Serial << *li << " ";
    }
  Serial << endl;
  List<int> lres2 = starA<ListA>()(lf2)(l12);
  Serial << "starA<ListA>()(lf2)(l12) : ";
  for (li= lres2.begin(); li!=lres2.end(); ++li)
    {
      Serial << *li << " ";
    }
  Serial << endl;
  List<int> lres3 = star (pureA<ListA>()(finc),l12);
  Serial << "star (pureA<ListA>()(finc),l12) : ";
  for (li= lres3.begin(); li!=lres3.end(); ++li)
    {
      Serial << *li << " ";
    }
  Serial << endl;
  Serial << "star (lf2)(l12) : ";
  List<int> lres4 = star (lf2)(l12);
  for (li= lres4.begin(); li!=lres4.end(); ++li)
    {
      Serial << *li << " ";
    }
  Serial << endl;
  Serial << "===============================================" << endl;
  Serial << "Experiments with starA<MaybeF<ListA> >()." << endl;
  Serial << "This uses a list of monomorphic functors." << endl;
  Serial << "WARNING: this code is not functional at the moment."
            << endl;
  Serial << "===============================================" << endl;
  Serial << "===============================================" << endl;
  Serial << "Test use of EitherA" << endl;
  Serial << "===============================================" << endl;
  Either<int> e1(1,std::string("This is one") );
  Serial << e1 << endl;
  Either<int> e2, e3;
  Either<int> e1p = star(pureA<EitherA>()(inc), e1 );
  Serial << e1p << endl;
  Either<int> e1pp = star ( inc, e1p );
  Serial << e1pp << endl;
  int x = -1;
  EitherError xbelowzero(std::string("x below zero"));
  Either<int> test1x = constrain1x(inc,less(_,0),xbelowzero,x);
  if (test1x.is_error() )
       Serial << "test1x.left() = " << test1x.left() << endl;
  else Serial << "test1x.right() = " << test1x.right() << endl;
  // This applies the operator twice.
  Either<int> test1xa = constrain1x(inc,greater(0),xbelowzero,inc(x));
  if (test1xa.is_error() )
       Serial << "test1xa.left() = " << test1xa.left() << endl;
  else Serial << "test1xa.right() = " << test1xa.right() << endl;
  Serial << "===============================================" << endl;
  Serial << "This is beginning to look very interesting." << endl;
  Serial << "It is a question of how to hold the information" << endl;
  Serial << "for a case handled by an Either." << endl;
  Serial << "===============================================" << endl;
  // This is beginning to look like an arrow...
  typedef std::pair<int,Fun1<int,bool> > Pip;
  Serial << "typedef std::pair<int,Fun1<int,bool> > Pip;" << endl;
  // Perhaps it should be std::pair<Either<int>,Fun1<int,bool> >
  Either<Pip> e1b(makePair(x,greater(0)));
  Serial << "Either<Pip> e1b(makePair(x,greater(0)));" << endl;
  Either<Pip> e1c = first(inc,e1b.right());
  Serial << "Either<Pip> e1c = first(inc,e1b.right()) = ";
  Serial << fst(e1c.right()) << endl;
  typedef std::pair<Fun1<int,int>,Fun1<int,bool> > Pfp;
  Either<Pfp> efp(makePair(finc,greater(0)));
  // Define a pair of operators.
  Pfp fp = makePair(finc,greater(0));
  typedef std::pair<int,bool> Pib;
  Serial << "typedef std::pair<int,bool> Pib;" << endl;
  Pib ib  = makePair(fst(fp)(x),snd(fp)(x));
  Serial << "Pib ib  = makePair(fst(fp)(x),snd(fp)(x)) : ";
  if (snd(ib) ) Serial << fst(ib) << endl;
  // This compose is similar to one I have used in fcpp/arrow.h
  Pib ib2 = compose(first(fst(fp)),second(snd(fp)))(makePair(x,inc(x)));
  Serial << "Pib ib2 = compose(first(fst(fp)),second(snd(fp)))(makePair(x,inc(x))) : ";
  if (snd(ib) ) Serial << fst(ib) << endl;
  // This provides a way to move from the pair of functors to an Either.
  // What needs to be sorted also is how to handle the message.
  Either<int> eres = snd(ib) ? xbelowzero : right(fst(ib));
  Serial << "Either<int> eres = snd(ib) ? xbelowzero : right(fst(ib)) : ";
  Serial << eres << endl;
  Either<int> eres2 = snd(ib2) ? xbelowzero : right(fst(ib2));
  Serial << "Either<int> eres2 = snd(ib2) ? xbelowzero : right(fst(ib2)) : ";
  Serial << eres2 << endl;
  Serial << "===============================================" << endl;
  Serial << "Experiments with pureA<EitherF<ListA> >()." << endl;
  Serial << "This uses a functor transformer on functors" << endl;
  Serial << "===============================================" << endl;
  List<Either<int> > le2 = pureA<EitherF<ListA> >()(right(2));
  List<Either<int> > le2a = pureA<EitherF<ListA> >()(pureE(2));
  List<Either<int> >::iterator lei;
  Serial << "pureA<EitherF<ListA> >()(right(2)) : [ ";
  for (lei= le2.begin(); lei!=le2.end(); ++lei)
    {
      Serial << *lei << " ";
    }
  Serial << "]" << endl;
  Serial << "pureA<EitherF<ListA> >()(pureE(2)) : [ ";
  for (lei= le2a.begin(); lei!=le2a.end(); ++lei)
    {
      Serial << *lei << " ";
    }
  Serial << "]" << endl;

}

void fcpp_examples()
{
  Serial.println("--------------------------");
  Serial.println("Some simple FC++ operations");
  Serial.print("plus(1,2) = "); Serial.println(plus(1,2));
  Serial.print("plus(1.5,2.3) = "); Serial.println(plus(1.5,2.3));
  Serial.print("plus(1)(2) = "); Serial.println(plus(1)(2));
  Serial.print("inc(1) = "); Serial.println(inc(1));
  Serial.println("--------------------------");

  Maybe<int> mx(2), my(3);
  Maybe<int> mz;
  if (mz.is_nothing()) Serial.println("mz is nothing");
  mz = just(plus(mx.value(),my.value()));
  if (mz.is_nothing()) { 
     Serial.println("mz is nothing");
  } else {
     Serial.print("mz has "); Serial.println(mz.value());
  }
  Serial.println("--------------------------");

}

// I have put these examples here to show how some things work and also to avoid some warnings
// that things are defined and not used.
auto bar() {
   LambdaVar<1> X;
   LambdaVar<2> Y;
   return lambda()[
      doM[ X <= list_with(1,2), Y <= list_with(3,4),
           unitM<ListM>()[ makePair[X,Y] ] ] ];
}

auto qux() {
   LambdaVar<1> X;
   LambdaVar<2> Y;
   return lambda()[ compM<ListM>()[ makePair[X,Y] | 
          X<=list_with(1,2), guard[true], Y<=list_with(3,4), 
          guard[equal[divides[Y,X],3] ] ]
      ];
}
auto whoa() {
   LambdaVar<1> even;
   LambdaVar<2> odd;
   LambdaVar<3> X;
   return lambda()[
      letrec[ even==lambda(X)[ if1[fcpp::equal[X,0],true,odd[fcpp::minus[X,1]]] ],
              odd ==lambda(X)[ if2[notEqual[X,0],even[fcpp::minus[X,1]],false] ]
      ].in[ even[3] ] ];
}

void lambda_examples()
{
   LambdaVar<1> F;
//   LambdaVar<2> G;
   //LambdaVar<3> H;
   LambdaVar<4> X;
   Serial << "lambda(F,X)[ if0[true,F[X,2],X] ](plus,1) = ";
   Serial << lambda(F,X)[ if0[true,F[X,2],X] ](plus,1) << endl;
  
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  while (!Serial) { }
  //::delay(5000);
  Serial.printf("Pico RP2040 FC++ Functor operations\n");
  fcpp_examples();
  Serial.println("after fcpp_examples");
  Serial.println("--------------------------");
  functor_examples();
  functor_examples2();
  Serial.println("--------------------------");
  Serial.println("after functor_examples");
  Serial.println("--------------------------");
 }

int i = 0;
 
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(i);
  //i++;
}
