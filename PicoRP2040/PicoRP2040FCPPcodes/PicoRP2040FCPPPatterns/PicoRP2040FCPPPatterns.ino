//////////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Patterns
// Examples from patternsinfp.cpp and other sources
// It may be that new work on monoids will be relevant.
// I have already noted the addition of parallel2.
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

/////////////////////////////////////////////////////////////////
// I think some of these will be useful for work with
// objects with state, in particular
// fork_(head,tail,some_list) -> (head(some_list),tail(some_list)
/////////////////////////////////////////////////////////////////
//
// I have gone back to the blog on the stream monad.
// I have some of the names different as follows.
//
// fork(f,g)a = (fa,ga)     is my forkp (fork from a pair)
//
// prod(f,g)(a,b) = (fa,gb) is my parallel though I can have two names.

// double = fork (id,id)    is forkp(makePair(id,id))
// I have called this double1.

// What is needed is double(a) -> (a,double(a)) where head() will always
// return a. which I cannot get to compile until I have defined it
// as out(repeat) which works fine. I could not get the Sig
// to define with the Reuser in a std::pair.

// repeat is alreade in prelude and does what is needed.

// unfold needs to be figured out.
// unfold :: (b->(a,b))->b->Stream a
// unfold f b = a : unfold f b' where (a,b') = f b
// Here : puts a at the front of a list.
// This is a recursive definition!
// See Real World Haskell p.203

// unfold f generates a stream from a seed, using a body f
// that transforms a seed b into an element a and a new
// seed b'.
// I have made this unfold2 and also unfold.

// out = fork(head,tail) on a list.  I can do this one.

// So unfold takes the pair (element,list) and puts the list together.

// unfold(pair(element,list)) -> cons(element,list)
// I have made that unfold1 (A one argment version).

// mapS ::
// mapS f = unfold (fork (f.head,tail))

//////////////////////////////////////////////////////////////
// Unrelated to the above, I am building exceptions into
// operators on List<T> which would not terminate on a
// lazy infinite list e.g. EnumFrom(1).
// This has lead me to look at Reusers, which are in fcpp/reuse.h
// and which I seem only to have modified once to add some
// concept code, now out of use.
// I have never extended them beyond three parameters.
// Operators so far protected:
// last, length, reverse.
// Although reverse itself is lazy, use of the result
// e.g. head(reverse(l)) is risky, so I have put a call
// to length() inside reverse to force a fail if needed.
// init is lazy and is O.K.
//////////////////////////////////////////////////////////////
// John Fletcher November 2013.
// NOTE: exceptions are not available in the RP2040 work.

// I think fork_ is useful enough to go into fcpp/prelude.h
// so I have made fork2 and forkp as well.
// These are also now in fcpp/prelude.h
// The remaining functoids are now in fcpp/pattern.h

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
void testing_patterns()
{
  Serial << "fork_ functoid" << endl;
  Serial << "--------------" << endl;
  Serial << "fork_(f,g,a) -> (f(a),g(a))" << endl;
  std::pair<int,int> res = fork_(inc,dec,1);
  Serial << "fork_(inc,dec,1) = ";
  Serial << "(" << res.first << "," << res.second << ")" << endl;
  Serial << "--------------" << endl;
  Serial << "fork2 functoid" << endl;
  Serial << "fork2(f,g,a,b) -> (f(a,b),g(a,b))" << endl;
  Serial << "--------------" << endl;
  std::pair<int,int> res2 = fork2(plus,minus,4,2);
  Serial << "fork2(plus,minus,4,2) = ";
  Serial << "(" << res2.first << "," << res2.second << ")" << endl;
  std::pair<int,int> res2a = fork2(plus,minus,4,3);
  Serial << "fork2(plus,minus,4,3) = ";
  Serial << "(" << res2a.first << "," << res2a.second << ")" << endl;
  Serial << "---------------" << endl;
  Serial << "forkp functoid receives a pair of functoids." << endl;
  Serial << "forkp((f,g),a) -> (f(a),g(a))" << endl;
  Serial << "--------------" << endl;
  std::pair<int,int> resp = forkp(std::make_pair(inc,dec),4);
  Serial << "forkp(std::make_pair(inc,dec),4) = ";
  Serial << "(" << resp.first << "," << resp.second << ")" << endl;
  Serial << "---------------" << endl;
  Serial << "apply functoid" << endl;
  Serial << "apply(f,a) -> f(a)" << endl;
  Serial << "---------------" << endl;
  int y = apply(inc,4);
  Serial << "apply(inc,4) = " << y << endl;  
  Serial << "---------------" << endl;
  Serial << "apply2 functoid" << endl;
  Serial << "apply2(f,a,b) -> (f(a),f(b))" << endl;
  Serial << "---------------" << endl;
  std::pair<int,int> res3 = apply2(inc,4,2);
  Serial << "apply2(inc,4,2) = ";
  Serial << "(" << res3.first << "," << res3.second << ")" << endl;
  Serial << "---------------" << endl;
  Serial << "applyp functoid" << endl;
  Serial << "applyp(f,(a,b) ) -> (f(a),f(b))" << endl;
  Serial << "---------------" << endl;
  std::pair<int,int> res4 = applyp(inc,std::make_pair(4,2));
  Serial << "applyp(inc,std::make_pair(4,2)) = ";
  Serial << "(" << res4.first << "," << res4.second << ")" << endl;
  Serial << "---------------" << endl;
  Serial << "parallel functoid" << endl;
  Serial << "parallel((f,g),(a,b) ) -> (f(a),g(b))" << endl;
  Serial << "---------------" << endl;
  Serial << "parallel(makePair(inc,dec),std::make_pair(4,4)) = ";
  std::pair<int,int> res5 = parallel(makePair(inc,dec),std::make_pair(4,4));
  Serial << "(" << res5.first << "," << res5.second << ")" << endl;
  Serial << "---------------" << endl;
  Serial << "Compare with this using first and second" << endl;
  Serial << "compose(first(inc),second(dec))(makePair(4,4)) = ";
  std::pair<int,int> res6 = compose(first(inc),second(dec))(makePair(4,4));
  Serial << "(" << res6.first << "," << res6.second << ")" << endl;
  Serial << "---------------" << endl;
  Serial << "NOTE: makePair is the FC++ functoid for std::pair."
            << endl;
  Serial << "==================================" << endl;
  Serial << "Comparison with hCurry and hUncurry " << endl;
  Serial << "==================================" << endl;
  Serial << "hCurry(f,a,b) -> f( (a,b) ) makes a pair for f " << endl;
  Serial << "hUncurry(f, (a,b) ) -> f(a,b) takes a pair " << endl;
  int x = hCurry(hUncurry(plus),1,2);
  Serial << "hCurry(hUncurry(plus),1,2) = " << x << endl;
  Serial << "hUncurry with fork_" << endl;
  x = hUncurry(plus,res); // using previous result.
  x = hUncurry(plus,fork_(inc,dec,1));
  Serial << "hUncurry(plus,fork_(inc,dec,1)) = " << x << endl;
  Serial << "==================================" << endl;
  Serial << "Experiments with lists" << endl;
  Serial << "==================================" << endl;
  List<int> l = enumFrom(1);
  Serial << "List<int> l = enumFrom(1);" << endl;
  std::pair<int,List<int> > il = fork_(head,tail,l);
  Serial << "fork_(head,tail,a list(1,2,3,...))" << endl;
  Serial << "( first: " << il.first
            << ", head of second: " << head(il.second) << ")" << endl;
  std::pair<int,List<int> > il2 = fork_(inc ^dot^ head,tail,l);
  Serial << "fork_(inc ^dot^ head,tail,a list(1,2,3,...))" << endl;
  Serial << "( first: " << il2.first
            << ", head of second: " << head(il2.second) << ")" << endl;
  Serial << "---------------" << endl;
  std::pair<int,List<int> > il3 = out(l);
  Serial << "std::pair<int,List<int> > il3 = out(l);" << endl;
  Serial << "out(a list(1,2,3,...))" << endl;
  Serial << "( first: " << il3.first
            << ", head of second: " << head(il3.second) << ")" << endl;
  Serial << "unfold1 takes a std::pair<T,List<T> >" << endl;
  List<int> l2 = unfold1(il3);
  Serial << "unfold1(1,a list(2,3,...)) makes original list" << endl;
  Serial << "( first: " << head(l2)
            << ", second: " << head(tail(l2)) << ")" << endl;
  List<int> l3 = unfold1(first(inc,il3));
  Serial << "unfold1(first(inc,pair(1,list))) makes new list" << endl;
  Serial << "( first: " << head(l3)
            << ", second: " << head(tail(l3)) << ")" << endl;

  List<int> l4 = unfold1(first(inc)(il3)); // The same as l3.
  Serial << "==================================" << endl;
  Serial << "The Stream monad" << endl;
  Serial << "==================================" << endl;
  // This is 'double' now implemented as 'double_' see below.
  std::pair<int,int> pi = forkp(makePair(id,id))(1);
  Serial << "pi = forkp(makePair(id,id))(1);" << endl;
  Serial << "pi.first = " << pi.first
            << ", pi.second = " << pi.second << endl;
  // My first take on double is more general
  // There is no need to describe the functoid f twice.
  std::pair<int,int> pi2 = doublef(id)(1);
  Serial << "pi2 = doublef(id)(1);" << endl;
  Serial << "pi2.first = " << pi2.first
            << ", pi2.second = " << pi2.second << endl;
  std::pair<int,int> pi3 = doublef(inc)(1);
  Serial << "pi3 = doublef(inc)(1);" << endl;
  Serial << "pi3.first = " << pi3.first
            << ", pi3.second = " << pi3.second << endl;
  std::pair<int,int> pi4 = double1(1);
  Serial << "pi4 = double1(1);" << endl;
  Serial << "pi4.first = " << pi4.first
            << ", pi4.second = " << pi4.second << endl;
  // This is supposed to be 'unfold double'
  // but it has the wrong type.
  //List<int> l5 = unfold1(pi);
  // I had the wrong idea about this. This is what is needed.
  // It won't compile unless defined as out(repeat)!!
  Serial << "-------------------------------------" << endl;
  std::pair<int,List<int> > il4 = double_(1);
  std::pair<int,List<int> > il5 = out(repeat(1));
  // Now I need diag for the bind of StreamM.
  Serial << "-------------------------------------" << endl;
  List<List<int> > lli = repeatS(enumFrom(1));
  Serial << "List<List<int> > lli = repeatS(enumFrom(1));" << endl;
  List<int> ild = diag(lli);
  Serial << "List<int> ild = diag(lli);" << endl;
  Serial << "head(ild) = " << head(ild) << ","
            << " head(tail(ild)) = " << head(tail(ild)) << endl;
  //List<List<int> > lli2 = map(repeatS,ild);
  List<int> ild2 = diag(repeatS(enumFrom(1)));
  Serial << "List<int> ild2 = diag(repeatS(enumFrom(1)));" << endl;
  Serial << "head(ild2) = " << head(ild2) << ","
            << " head(tail(ild2)) = " << head(tail(ild2)) << endl;
  Serial << "head(tail(tail(ild2))) = " << head(tail(tail(ild2))) << endl;
  Serial << "-------------------------------------" << endl;
  List<int> ild3 = fcpp::map(inc,enumFrom(1));
  Serial << "List<int> ild3 = map(inc,enumFrom(1));" << endl;
  Serial << "head(ild3) = " << head(ild3) << ","
            << " head(tail(ild3)) = " << head(tail(ild3)) << endl;
  Serial << "head(tail(tail(ild3))) = " << head(tail(tail(ild3))) << endl;
  List<int> ild4 = diag(repeatS(mapS(inc,enumFrom(1))));
  Serial << "List<int> ild4 = diag(repeatS(mapS(inc,enumFrom(1))));"
            << endl;
  Serial << "head(ild4) = " << head(ild4) << ","
            << " head(tail(ild4)) = " << head(tail(ild4)) << endl;
  Serial << "head(tail(tail(ild4))) = " << head(tail(tail(ild4))) << endl;
  Serial << "-------------------------------------" << endl;
  List<int> ild5 = diag(mapS(repeatS,enumFrom(1)));
  Serial << "List<int> ild5 = diag(mapS(repeatS,enumFrom(1)));"
            << endl;
  Serial << "head(ild5) = " << head(ild5) << ","
            << " head(tail(ild5)) = " << head(tail(ild5)) << endl;
  Serial << "head(tail(tail(ild5))) = " << head(tail(tail(ild5))) << endl;
  Serial << "-------------------------------------" << endl;
  Serial << "FC++ ListM monad " << endl;
  Serial << "-------------------------------------" << endl;
  LambdaVar<1> X;
  //LambdaVar<2> Y;

  List<int> l5 = lambda(X) [unitM<ListM>() [ X ] ](1);
  Serial << "List<int> l5 = lambda(X) [unitM<ListM>() [ X ] ](1);"
      << endl;
  Serial << "head(l5) = " << head(l5)
            << ", length(l5) = " << length(l5) << endl;
  List<List<int> > ll1 = lambda(X) [unitM<ListM>() [ X ] ](l5);
  Serial << "List<List<int> > ll1 = lambda(X) [unitM<ListM>() [ X ] ](l5);"
      << endl;
  Serial << "head(head(ll1)) = " << head(head(ll1))
            << ", length(ll1) = " << length(ll1) << endl;
  List<int> l6 = repeat(1);
  Serial << "List<int> l6 = repeat(1);" << endl;
  Serial << "head(l6) = " << head(l6)
            << ", head(tail(l6)) = " << head(tail(l6)) << endl;
  // I cannot find any examples of bindM<ListM> !!
  // It needs a functiod of the type a -> [a].
  Serial << "-------------------------------------" << endl;
  List<int> l6a = lambda(X) [bindM<ListM>() [ X, repeat ] ](enumFromTo(1,2));
  Serial << "l6a = lambda(X) [bindM<ListM>() [ X , repeat ] ](enumFromTo(1,2));"
            << endl;
  List<int>::iterator i;
  int j = 0;
  for (i = l6a.begin(); i != l6a.end(); ++i) {
    ++j;
    Serial << j << " " << *i << endl;
    if (j > 5) break;
  }
 Serial << "head(l6a) = " << head(l6a)
            << ", head(tail(l6a)) = " << head(tail(l6a)) << endl;
  Serial << "head(tail(tail(l6a))) = " << head(tail(tail(l6a))) << endl;
  Serial << "-------------------------------------" << endl;
  Serial << "This is the body of the previous one and they are the same"
            << endl;
  List<int> l6b = concat(mapS(repeat,enumFromTo(1,2)));
  Serial << "List<int> l6b = concat(mapS(repeat,enumFromTo(1,2)));" << endl;
  Serial << "head(l6b) = " << head(l6b)
            << ", head(tail(l6b)) = " << head(tail(l6b)) << endl;
  Serial << "head(tail(tail(l6b))) = " << head(tail(tail(l6b))) << endl;
  j = 0;
  for (i = l6b.begin(); i != l6b.end(); ++i) {
    ++j;
    Serial << j << " " << *i << endl;
    if (j > 5) break;
  }
  Serial << "-------------------------------------" << endl;
  Serial << "This is the body of the previous one modified to do what I want."
            << endl;
  List<int> l6bb = concat(repeat(fcpp::map(id,enumFromTo(1,2))));
  Serial << "List<int> l6bb = concat(repeat(mapS(id,enumFromTo(1,2))));"
            << endl;
  Serial << "head(l6bb) = " << head(l6bb)
            << ", head(tail(l6bb)) = " << head(tail(l6bb)) << endl;
  Serial << "head(tail(tail(l6bb))) = " << head(tail(tail(l6bb))) << endl;
  j = 0;
  for (i = l6bb.begin(); i != l6bb.end(); ++i) {
    ++j;
    Serial << j << " " << *i << endl;
    if (j > 5) break;
  }
  Serial << "-------------------------------------" << endl;
  List<int> l6bc = concat(mapS(id,repeat(enumFromTo(1,2))));
  Serial << "List<int> l6bc = concat(mapS(id,repeat(enumFromTo(1,2))));"
            << endl;
  j = 0;
  for (i = l6bc.begin(); i != l6bc.end(); ++i) {
    ++j;
    Serial << j << " " << *i << endl;
    if (j > 5) break;
  }
  Serial << "-------------------------------------" << endl;
  List<int> l6bd = lambda(X) [bindM<ListM>() [ X, id ] ](repeat(enumFromTo(1,2)));
  Serial << "l6a = lambda(X) [bindM<ListM>() [ X , id ] ](repeat(enumFromTo(1,2)));"
            << endl;
  j = 0;
  for (i = l6bd.begin(); i != l6bd.end(); ++i) {
    ++j;
    Serial << j << " " << *i << endl;
    if (j > 5) break;
  }
  Serial << "head(l6bd) = " << head(l6bd)
            << ", head(tail(l6bd)) = " << head(tail(l6bd)) << endl;
  Serial << "head(tail(tail(l6bd))) = " << head(tail(tail(l6bd))) << endl;
  Serial << "-------------------------------------" << endl;
  List<int> l6c = fcpp::map(inc,enumFromTo(1,2));
  Serial << "List<int> l6c = map(inc,enumFromTo(1,2));" << endl;
  j = 0;
  for (i = l6c.begin(); i != l6c.end(); ++i) {
    ++j;
    Serial << j << " " << *i << endl;
    if (j > 5) break;
  }
  Serial << "-------------------------------------" << endl;
  List<int> l6d = concat(repeat(l6c));
  Serial << "List<int> l6d = concat(repeat(l6c));" << endl;
  j = 0;
  for (i = l6d.begin(); i != l6d.end(); ++i) {
    ++j;
    Serial << j << " " << *i << endl;
    if (j > 5) break;
  }
  Serial << "-------------------------------------" << endl;
  Serial << "New FC++ StreamM monad " << endl;
  Serial << "-------------------------------------" << endl;
  Serial << "Correction to bug in diag functoid means that results are correct now."
      << endl;
  List<int> l7 = lambda(X) [unitM<StreamM>() [ X ] ](1);
  Serial << "List<int> l7 = lambda(X) [unitM<StreamM>() [ X ] ](1);"
      << endl;
  Serial << "head(l7) = " << head(l7)
            << ", head(tail(l7)) = " << head(tail(l7)) << endl;
  List<int> l8 = lambda(X) [bindM<StreamM>() [ X , id] ](l7);
  Serial << "List<int> l8 = lambda(X) [bindM<StreamM>() [ X , id] ](l7);"
            << endl;
  Serial << "head(l8) = " << head(l8)
            << ", head(tail(l8)) = " << head(tail(l8)) << endl;
  List<int> l8a = lambda(X) [bindM<StreamM>() [ X , id] ](enumFrom(1));
  Serial << "l8a = lambda(X) [bindM<StreamM>() [ X , id ] ](enumFrom(1));"
            << endl;
  Serial << "head(l8a) = " << head(l8a)
            << ", head(tail(l8a)) = " << head(tail(l8a)) << endl;
  List<int> l8b = lambda(X) [bindM<StreamM>() [ X , inc ] ](enumFrom(1));
  Serial << "List<int> l8b = lambda(X) [bindM<StreamM>() [ X , inc ] ](enumFrom(1));"
            << endl;
  Serial << "head(l8b) = " << head(l8b)
            << ", head(tail(l8b)) = " << head(tail(l8b)) << endl;
  j = 0;
  for (i = l8b.begin(); i != l8b.end(); ++i) {
    ++j;
    Serial << j << " " << *i << endl;
    if (j > 5) break;
  }

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
  Serial.printf("Pico RP2040 FC++ pattern operations\n");
  fcpp_examples();
  Serial.println("after fcpp_examples");
  Serial.println("--------------------------");
  testing_patterns();
  Serial.println("--------------------------");
  Serial.println("after testing_patterns");
  Serial.println("--------------------------");
 }

int i = 0;
 
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(i);
  //i++;
}
