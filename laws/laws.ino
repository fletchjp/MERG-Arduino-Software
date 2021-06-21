// laws 

// A first attempt to get FC++ functor laws operational

// A lot of the examples come from "Learn You a Haskell for Great Good"
// by Miran Lipovaca, No Starch Press, 2011

// This comes from the old file unify.cpp which has a lot of explanations.

// This will now work with FC++ as an Arduino library - just include "prelude.h" and it will find it.

// I have quite a lot of things working.

// There are examples where I had to replace pure with pureM.
// This was because I had to change the type deduction in ChooseApplicable
// because I did not have boost/mpl. There may be some other way to do the type deduction.

// Now that I have a version of boost available I have been able to restore the previous operation.

// There is a lot of information in functors.h

#include<ArduinoSTL.h>

#include <boost_1_51_0.h>
// This is used in FC++ to allow for boost usage.
#define FCPP_ARDUINO_BOOST
#include <iostream>
// Need 5 parameter functoids for example.
#define FCPP152
#define FCPP_ENABLE_LAMBDA
#define FCPP_USE_PROMOTE
#define FCPP_DEBUG
#define FCPP_EXTRA_STRONG_CURRY
#define FCPP_PATTERN // define this to include pattern.h
#define FCPP_UNIFY
#include "prelude.h"
#include "functors.h"
//#include "comonad.h"

using namespace fcpp;

//using std::cout;
//using std::endl;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  std::cout << "==================================" << std::endl;
  std::cout << "Functor laws examples" << std::endl;
  std::cout << "Page numbers refer to the book" << std::endl;
  std::cout << "Learn You a Haskell for Great Good" << std::endl;
  std::cout << "==================================" << std::endl;
  List<int>::iterator it;
  std::cout << "===============================" << std::endl;
  std::cout << "Here is fmap actually working!!" << std::endl;
  std::cout << "===============================" << std::endl;
  std::cout << "id ( just(3) )                 : " << id ( just(3) )
            << " (page 224)"<< std::endl;  
  std::cout << "===============================" << std::endl;
  std::cout << "Functor Law 1        (page 223)" << std::endl;
  std::cout << "     fmap id = id" << std::endl;
  std::cout << "===============================" << std::endl;
  std::cout << "id ( just(3) )                 : "
            <<  id ( just(3) ) << std::endl;
  std::cout << "fmap (id) ( just(3) )          : "
            <<  fmap (id)( just(3) ) << std::endl;
  std::cout << "fmap (id) ( right(3) )         : "
            <<  fmap (id)( right(3) ) << std::endl;
  std::cout << "fmap (id) ( ident(3) )         : "
            <<  fmap (id)( ident(3) ) << std::endl;
  std::cout << "=================================" << std::endl;
  std::cout << "Functor Law 2          (page 224)" << std::endl;
  std::cout << "fmap (f . g ) = fmap(f) . fmap(g)" << std::endl;
  std::cout << "=================================" << std::endl;
  std::cout << " fmap (inc  ^dot^ dec)( ident(3) )     : "
            <<  fmap (inc ^dot^ dec)( ident(3) ) << std::endl;
  std::cout << "(fmap (inc) ^dot^ fmap(dec))(ident(3)) : "
            <<  (fmap (inc) ^dot^ fmap(dec)) (ident(3) ) << std::endl;
  std::cout << " fmap (inc  ^dot^ dec)( just(3) )      : "
            <<  fmap (inc ^dot^ dec)( just(3) ) << std::endl;
  std::cout << "(fmap (inc) ^dot^ fmap(dec))(just(3))  : "
            <<  (fmap (inc) ^dot^ fmap(dec)) (just(3) ) << std::endl;
  std::cout << " fmap (inc  ^dot^ dec)( right(3) )     : "
            <<  fmap (inc ^dot^ dec)( right(3) ) << std::endl;
  std::cout << "(fmap (inc) ^dot^ fmap(dec))(right(3)) : "
            <<  (fmap (inc) ^dot^ fmap(dec)) (right(3) ) << std::endl;
  std::cout << "=================================" << std::endl;
  List<int> fl2_1 = makeList1(3);
  std::cout << " List<int> fl2_1 = makeList1(3);" << std::endl;
  List<int> fl2_2 = fmap (inc ^dot^ dec)( fl2_1 );
  List<int> fl2_3 = (fmap (inc) ^dot^ fmap(dec)) (fl2_1 );
  std::cout << " fmap (inc ^dot^ dec)( fl2_1 ) = [ ";
  for (it = fl2_2.begin(); it != fl2_1.end(); ++it) {
     std::cout << *it << " ";
  }
  std::cout << "]" << std::endl;
  std::cout << " (fmap (inc) ^dot^ fmap(dec))(fl2_1) = [ ";
  for (it = fl2_3.begin(); it != fl2_3.end(); ++it) {
     std::cout << *it << " ";
  }
  std::cout << "]" << std::endl;
  std::cout << "=================================" << std::endl;
  std::cout << "More examples. p.229" << std::endl;
  std::cout << "=================================" << std::endl;
  std::cout << "(just(plus(3)) ^star^ just(9)) : "
            <<  (just(plus(3)) ^star^ just(9)) << std::endl;  // p.229
  std::cout << "====================================" << std::endl;
  std::cout << "Applicative Functor Law 1 (page 238)" << std::endl;
  std::cout << " pure f <*> x = fmap f x" << std::endl;
  std::cout << "Note use of pureM which matches the Monad type." << std::endl;
  std::cout << "This is a current limitation in the mechanism to infer the argument type." << std::endl;
  std::cout << "====================================" << std::endl;
  std::cout << "fmap (inc) ( just(3) )         : "
            <<  fmap (inc)( just(3) ) << std::endl;
  Maybe<int> mx1 = pure (inc) ^star^ ( just(3) );  // pure now works here.
  std::cout << "pure (inc) ^star^ ( just(3) )  : "
            <<  mx1 << std::endl;
  Maybe<int> mx1a = MaybeA::pure()(inc) ^star^ (3);
  std::cout << "MaybeA::pure()(inc) ^star^ (3) : "
            <<  mx1a << std::endl;
  // pure is actually optional here as pure == id when on its own.
  Maybe<int> mx1b = inc ^star^ ( just(3) );
  std::cout << "====================================" << std::endl;
  std::cout << "Applicative Functor Law 2 (page 238)" << std::endl;
  std::cout << " pure id <*> v = v" << std::endl;
  std::cout << "====================================" << std::endl;
  std::cout << "Note use of pureM which matches the Monad type is not needed." << std::endl;
  Maybe<int> mx2 = pure (id) ^star^ ( just(3) );
  std::cout << "pure (id) ^star^ ( just(3) )    : "
            <<  mx2 << std::endl;
  std::cout << "====================================" << std::endl;
  std::cout << "Applicative Functor Law 3 (page 238)" << std::endl;
  std::cout << "pure (.)<*>u<*>v<*>w = u<*>(v<*>w)" << std::endl;
  std::cout << "Note: in the example one of the operators cannot be inferred."
            << std::endl;
  std::cout << "====================================" << std::endl;
  Maybe<int> mx3a = pure (dot) ^star^ pureA<MaybeA>()(inc)
                    ^star^ dec ^star^ ( just(3) );
  Maybe<int> mx3b = pure(inc) ^star^ (pureM(dec) ^star^ ( just(3) ));
  std::cout << "pure(dot) ^star^ pureA<MaybeA>()(inc) ^star^ dec ^star^ ( just(3) )    : "
            <<  mx3a << std::endl;
  std::cout << "pure(inc) ^star^ (pureM(dec) ^star^ ( just(3) )) : "
            <<  mx3b << std::endl;
  Maybe<int> mx3c = pure(inc) ^star^ (dec ^star^ ( just(3) ) );

  std::cout << "====================================" << std::endl;
  std::cout << "Applicative Functor Law 4 (page 238)" << std::endl;
  std::cout << "pure f <*> pure x = pure ( f x )" << std::endl;
  std::cout << "====================================" << std::endl;
  Maybe<int> mx4a = pure (id) ^star^ ( just(3) );
  Maybe<int> mx4a2 = pure (id) ^star^ pure ( just(3) );
  Maybe<int> mx4b = pure (id ( just(3) ) );
  std::cout << "pure (id) ^star^ ( just(3) )    : "
            <<  mx4a << std::endl;
  std::cout << "pure (id) ^star^ pure ( just(3) )    : "
            <<  mx4a2 << std::endl;
  std::cout << "pure (id ( just(3) ) )          : "
            <<  mx4b << std::endl;
  Maybe<int> mx4c = pure (inc) ^star^ ( just(3) );
  std::cout << "pure (inc) ^star^ ( just(3) )   : "
            <<  mx4c << std::endl;
  // These are all equivalent. MaybeA::pure provides just.
  // The problem is how to get pure to know what it is doing.
  // At the moment the action is taken in star.
  // That does not work for pure on its own.
  // There is a comment on p.228 that there is no default implementation
  // of 'pure' so it needs to be defined in the applicative functor.
  // These examples do this for the 'pure' in MaybeA which equates to 'just'.
  Maybe<int> mx4c2 = (just(inc)) ^star^ ( just(3) );
  std::cout << "(just(inc)) ^star^ ( just(3) )  : "
            <<  mx4c2 << std::endl;
  Maybe<int> mx4c3 = (pureA<MaybeA>()(inc)) ^star^ ( just(3) );
  std::cout << "(pureA<MaybeA>()(inc)) ^star^ ( just(3) )  : "
            <<  mx4c3 << std::endl;
  Maybe<int> mx4c4 = (MaybeA::pure()(inc)) ^star^ ( just(3) );
  std::cout << "(MaybeA>::pure()(inc)) ^star^ ( just(3) )  : "
            <<  mx4c4 << std::endl;
  //Maybe<int> mx4d = pure (pureA<MaybeA>()(inc) ( just(3) ) );
  Maybe<int> mx4d = pure (liftM<MaybeM>()(inc) ( just(3) ) );
  std::cout << "pure (liftM<MaybeM>()(inc))( just(3) )  : "
            <<  mx4d << std::endl;
  std::cout << "====================================" << std::endl;
  std::cout << "Applicative Functor Law 5 (page 238)" << std::endl;
  std::cout << " u <*> pure y = pure ( $ y ) <*> u" << std::endl;
  std::cout << "Note: $ is the infix operator for fmap." << std::endl;
  std::cout << "Notation not reproduced completely" << std::endl;
  std::cout << "====================================" << std::endl;
  // One side or the other of star must give the type information.
  // Note: pureA<MaybeA>() == MaybeA::pure() == just. All equivalent.
  Maybe<int> mx5a = pureA<MaybeA>()(inc) ^star^ ( pure(3) );
  Maybe<int> mx5a2 = pure(inc) ^star^ ( just(3) );
  // This fails as it cannot deduce a type from pure alone.
  //Maybe<int> mx5a3 = pure(inc) ^star^ ( pure(3) );
  Maybe<int> mx5a3 = pure(inc) ^star^ ( pureM(3) ); // This works.
  Maybe<int> mx5b = fmap(_,just(3)) (inc);
  Maybe<int> mx5c = (_ ^fmap^ just(3)) (inc);
  // This one fails because inc does not need to be lifted here.
  //Maybe<int> mx5d = (_ ^fmap^ just(3)) ( pureA<MaybeA>()(inc) );
  std::cout << "pureA<MaybeA>()(inc) ^star^ ( pure(3) ) : "
            <<  mx5a << std::endl;
  std::cout << "pure( inc ) ^star^ ( just(3) ) : "
            <<  mx5a2 << std::endl;
  std::cout << "pure( inc ) ^star^ ( pureM(3) ) : "
            <<  mx5a3 << std::endl;
  std::cout << "(_ ^fmap^ just(3)) (inc)        : "
            <<  mx5c << std::endl;

  
  std::cout << "=======================================" << std::endl;
  std::cout << "Some more examples from functors.cpp" << std::endl;
  std::cout << "=======================================" << std::endl;
  std::cout << "dot(multiplies(3),plus(100))(1)    : "
            << dot(multiplies(3),plus(100))(1) << " (page 221)" << std::endl;
  std::cout << "(multiplies(3) ^dot^ plus(100))(1)  : "
            << (multiplies(3) ^dot^ plus(100))(1) << std::endl;
  std::cout << "dot (id, id ) ( just(3)) : " << dot (id, id ) ( just(3))
            << " needs repeat of id!" << std::endl;
  std::cout << "id ( just(3) )            : " << id ( just(3) )
            << " (page 224)"<< std::endl;
  std::cout << "----------------------------------------------------"
            << std::endl;
  std::cout << "MaybeM and EitherM are the only FC++ monads with an explicit join."
            << std::endl;
  std::cout << "----------------------------------------------------"
            << std::endl;
  Maybe<int> mj = joinM<MaybeM>()( just (just (2) ) );
  std::cout << "joinM<MaybeM>()( just (just (2) ) ) : " << mj << std::endl;
  Maybe<int> mj2 = MaybeM::join()( just (just (2) ) );
  std::cout << "MaybeM::join()( just (just (2) ) )  : " << mj2 << std::endl;
  Maybe<int> mj3 = join( just (just (2) ) );
  std::cout << "join( just (just (2) ) )            : " << mj3 << std::endl;
  std::cout << "--------------------------------------" << std::endl;
  Either<int> ej = (joinM<EitherM>())( right (right (2) ) );
  std::cout << "joinM<EitherM>()( right (right (2) ) ) : " << ej
            << std::endl;
  Either<int> ej2 = EitherM::join()( right (right (2) ) );
  std::cout << "EitherM::join()( right (right (2) ) )  : " << ej2
            << std::endl;
  Either<int> ej3 = join( right (right (2) ) );
  std::cout << "join( right (right (2) ) )             : " << ej3
            << std::endl;
  Either<int> el = liftM<EitherM>()(inc) (right (2) );
  std::cout << "liftM<EitherM>()(inc) (right (2) )     : " << el
            << std::endl;
  std::cout << "--------------------------------------" << std::endl;
  Maybe<int> mx4e = pure (inc (3) );
  std::cout << "mx4e = pure (inc (3) )  : "
            <<  mx4e << std::endl;
  // The result is created from the assignment.
  Either<int> ex4e = pure (inc (3) );
  std::cout << "ex4e = pure (inc (3) )  : "
            <<  ex4e << std::endl;
  std::cout << "pure (inc (3) )  : " << pure (inc (3) ) << std::endl;


  std::cout << "=======================================" << std::endl;
#ifdef FCPP_UNIFY
  std::cout << "These are monad uses being unified into MaybeA" << std::endl;
  std::cout << "--------------------------------------" << std::endl;
  std::cout << "liftM<MaybeA>()(inc)( just(3) ) : "
            <<  liftM<MaybeA>()(inc)( just(3) ) << std::endl;
  Maybe<int> mja = joinM<MaybeA>()( just (just (2) ) );
  std::cout << "joinM<MaybeA>()( just (just (2) ) ) : " << mja << std::endl;
  Maybe<int> mj2a = MaybeA::join()( just (just (2) ) );
  std::cout << "MaybeA::join()( just (just (2) ) )  : " << mj2a << std::endl;
  std::cout << "--------------------------------------" << std::endl;
  Either<int> eja = joinM<EitherA>()( right (right (2) ) );
  std::cout << "joinM<EitherA>()( right (right (2) ) ) : " << eja
            << std::endl;
  Either<int> ela = liftM<EitherA>()(inc) (right (2) );
  std::cout << "liftM<EitherA>()(inc) (right (2) )     : " << ela
            << std::endl;
  std::cout << "--------------------------------------" << std::endl;
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

}
