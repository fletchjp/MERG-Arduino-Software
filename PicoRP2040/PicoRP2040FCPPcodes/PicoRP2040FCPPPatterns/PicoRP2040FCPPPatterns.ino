//////////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Patterns
// Examples from patternsinfp.cpp and other sources
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// I have found a new source of inspiration.
// The reason I found it was that I was looking for a lost file
// of my own, translator.cpp.
//
// This is for a start from one of many pages.
//
// http://patternsinfp.wordpress.com/2010/12/31/stream-monad/
//
// I have also now been in contact with Jeremy Gibbons.
//
// See also
// http://zenzike.com/posts/2010-10-21-streams-and-monad-laws
//
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// What is coming into being is a new set of FC++ functoids
// which are versions of what Jeremy is talking about in his
// pages. I am starting with his work on the Stream monad
// where the Stream translates in FC++ terms into a List,
// often an infinite List e.g. enumFrom(1).
// He calls his work Patterns in Functional Programming.
// These functoids are going to form the patterns.
// I am not sticking strictly to his names and I explain that
// as I am going along.
//
// These functoids are now being located in either
// fcpp/prelude.h for those of general applicability or
// fcpp/pattern.h for those specific to the patterns.
//
// I think that the set of tools I am building will lead also
// to comonads and arrows, particularly as I already have
// first and second defined from my previous work on arrows.
//
// Also I think it is going to help me with the State monad in
// FC++ which I have not done much with yet.

// Monads
// ------
// Real World Haskell (p.329) defines the monad class in terms
// of bind(>>=) and return.
// FC++ uses bind and unit for these.
//
// The Stream Monad is defined in terms of join and return as follows.
//
//  Making streams a monad amounts to defining functions
//
//     return :: a -> Stream a
//     join   :: Stream( Stream a) -> Stream a
//  satisfying the monad laws:
//
//     join . return      = id
//     join . mapS return = id
//     join . mapS join   = join . join
//
//  It then defines
//     return = repeat         where
//     repeat = unfold double  and
//     double = fork(id,id)
//
// This is not consistent with FC++ where in the ListM monad
//
//     unit a  returns a List with only a, not a repeat.
//
// I am not sure what to do about that.
//
// I am now defining the Stream monad as StreamM.
//
// I have also defined Stream<T> as inheriting from List<T>.
// I can then add overloaded operators to do arithmetic.
//
//////////////////////////////////////////////////////////////
// ORGANISATION
// I am going to put the remaining general purpose functoids
// into fcpp/prelude.h
// (I think that is only applyp, the rest are there already.)
//
// I am going to make fcpp/pattern.h as a new file
// and put patterns in fp functoids there. It makes it easier
// for editing the tests to be able to see the functoids in a
// different file. I will also move first and second there
// from arrow.h.
//
// When complete, the Stream monad will go into fcpp/monad.h.
//
// This has developed differently, and fcpp/pattern.h is now
// the location of a lot of new work on Monads.
/////////////////////////////////////////////////////////////

// fork_(f,g,a) -> (f(a),g(a))
// Note: I have named it fork_ as there is something else called fork.

// Also the same thing for 2 arg functoids
// fork2(f,g,a,b) -> (f(a,b),g(a,b))

// There are related things already
// hCurry(f,a,b)     -> f((a,b)) makes arguments into a pair.
// hUncurry(f,(a,b)) -> f(a,b)   takes arguments as a pair.

// I have used hUncurry in arrows_examples.cpp
// I have not seen examples of hCurry.
// I don't know of a functoid taking a pair.

// I already have these which are in arrow.h
// first(f,p) and second(f,p) with p a pair (x,y)
// first  applies f to x and returns a pair f(x) and y
// second applies f to y and returns a pair x and f(y)
// first(f,(a,b))  -> (f(a),b)
// second(f,(a,b)) -> (a,f(b))
// Example of use:
// hUncurry(plus,compose(first(inc),second(dec))(makePair(x,x)))
// This takes an argument (x) and makes a pair of two copies.
// It increments the first and decrements the second.
// It then uncurries the pair and feeds it to plus,
// adding them together.

// I could now do something with parallel((f,g),(a,b))

// What about this  something(f,a,b) -> (f(a),f(b)) ??
// Is there a something?? It is like map, which works with lists.
// How about apply or apply2 ?
// apply(f,a)      -> f(a)           (hardly needed)
// apply2(f,a,b)   -> (f(a),f(b))

// Further ideas
// applyp(f,(a,b)) -> (f(a),f(b)) (DONE)
// parallel((f,g),(a,b)) -> (f(a),g(b)) (DONE) (called prod by Jeremy)
// parallel2((f,g),(a,b),(c,d)) -> (f(a,c),g(b,d)) (added 2022)

// related: compose2(f,g,h)(x)   -> f( g(x), h(x) )
// also:    combine (f,g,h)(x,y) -> f( g(x), h(y) )
// (also known as compose2xy)
     
// How about forkp( (f,g), a) -> (f(a),g(a))  DONE

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
