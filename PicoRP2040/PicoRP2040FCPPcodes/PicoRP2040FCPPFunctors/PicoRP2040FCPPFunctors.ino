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

////////////////////////////////////////////////////////////////////////////////
/// New note: this is where Endo and MonoidEndo will be of help.
/////////////////////////////////////////////////////////////////////////////////


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

}
//////////////////////////////////////////////////////////


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
  Serial.printf("Pico RP2040 FC++ operations\n");
  fcpp_examples();
  Serial.println("after fcpp_examples");
  Serial.println("--------------------------");
 }

int i = 0;
 
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(i);
  //i++;
}
