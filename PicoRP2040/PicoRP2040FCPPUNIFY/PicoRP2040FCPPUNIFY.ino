////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Unify from Pico RP2040 FC++ Monad
// I am working to bring in things from the unify.cpp example file.
// I have modified more of the FC++ headers to work in the Arduino/Pico environment.
// This involves disabling exceptions.
////////////////////////////////////////////////////////////////
// was Arduino RP2040 Monad
////////////////////////////////////////////////////////////////
// was DUEMONAD

#include <string>

// Demo of FC++ Maybe Monad operations
// This will work on ARDUINO DUE but not on AVR boards.
// 5 parameter functoids and more for example working.
// Function 6, 7 and 8 now operational.
#define FCPP152
#define FCPP_ENABLE_LAMBDA
#define FCPP_EXTRA_STRONG_CURRY
#define FCPP_PATTERN // define this to include pattern.h
//#define FCPP_ARDUINO // Take out arguement traits - defined in prelude for Arduino etc
// All headers modified to work in the Arduino/Pico environment.
// Some exceptions guarding in patter.h have had to be removed.
#define FCPP_UNIFY // Unification of Monads and Functors in functors.h
#include "fcpp_prelude.h"
#include "fcpp/functors.h"
#include "fcpp/comonad.h"
#include "fcpp/arrow.h"


#include <Streaming.h>


using namespace fcpp;

/// I have renamed String to StringL to avoid a name clash with the Arduino use of String.
typedef List<char> StringL;

//- templated version does not compile.
// It seems to regard T as an undeclared class.
// I think the problem is that there is already a generic templated operator<< in Streaming.
// template <typename T>
/*
Print &operator <<( Print &obj, const Maybe<int> &arg)
{
    if (arg.is_nothing()) { 
       obj.print("nothing");
    } else {
       obj.print(arg.value());
    }
    return obj; 
}

Print &operator <<( Print &obj, const Either<int> &arg)
{
    if (arg.is_error()) { 
       obj.print(arg.left());
    } else {
       obj.print(arg.right());
    }
    return obj; 
}
*/

// This does compile.
template <class T>
T what(const Maybe<T> &mt) { return mt.value(); }

// Failure example from https://people.cs.umass.edu/~yannis/fc++/fcpp-lambda.pdf

int f(int x) {
  if (x >= 0) return x;
  else return -1;
}

int g(int x)
{
  return f(x);
}

int h(int x, int y)
{
  if (f(x) < 0) return f(x);
  else
  return f(x) + g(y);
}

// Note use of auto to avoid having to sort out the type.
// The type had to be specified before C++11.
auto pg = ptr_to_fun(&g);

// Failure example adapted to return Maybe<int>.
// Note that these are not used directly, instead using ptr_to_fun to make an object for FC++.
Maybe<int> mf(int x) {
  if (x >= 0) return just(x);
  else return NOTHING;
}

Maybe<int> mg(int x)
{
  return mf(x);
}

Maybe<int> mh(int x, int y)
{
  if (mf(x).is_nothing()) return mf(x);
  else
  return just(mf(x).value() + mg(y).value());
}

auto pmf = ptr_to_fun(&mf);
auto pmg = ptr_to_fun(&mg);
auto pmh = ptr_to_fun(&mh);

// Code from bindm_tests.cpp
void explore_bindm()
{
   LambdaVar<1> X;
   LambdaVar<2> Y;
   LambdaVar<3> Z;
   LambdaVar<4> W;
   LambdaVar<5> A;
   LambdaVar<6> P;
   LambdaVar<7> F;
   LambdaVar<8> G;
   LambdaVar<9> H;
   List<int> l = list_with(1,2,3);
   List<int>::iterator i;
   List<int> l2 = list_with(2,3,4);
   List<List<int> > ll = list_with(l,l2);
   List<List<List<int> > > lll = list_with(ll,ll);
   bind(lll,id); // This works.
   //

    Serial.println("---------------------");
    Serial.println("bind on List<int> ");
    Serial.println("---------------------");
    List<int> lb = bindM<IdentityM>()(l,id);
    Serial.println("List<int> lb = bindM<IdentityM>()(l,id);");
    Serial.print("lb =  : ");
    for (i = lb.begin(); i != lb.end(); ++i) {
      Serial << *i; Serial.print(" ");
    }
    Serial << endl;
    Serial.println("---------------------");
    List<int> lc = bindM<IdentityM>()(l,fcpp::map(inc));
    Serial << "List<int> lc = bindM<IdentityM>()(l,map(inc));" << endl;
    Serial << "lc =  : ";
    for (i = lc.begin(); i != lc.end(); ++i) {
      Serial << *i; Serial.print(" ");
    }
    Serial << endl;
    Serial.println("---------------------");
    Serial << "bind on List<List<int> > " << endl;
    Serial.println("---------------------");
    List<int> lq = bind(ll,tail);
    List<int> lid = bind(ll,id);
    List<int> linc = bind(ll,fcpp::map(inc));
    Serial.println("---------------------");
    Serial << "l = list_with(1,2,3) : ";
    for (i = l.begin(); i != l.end(); ++i) {
      Serial << *i; Serial.print(" ");
    }
    Serial << endl;
    Serial << "l2 = list_with(2,3,4) : ";
    for (i = l2.begin(); i != l2.end(); ++i) {
      Serial << *i; Serial.print(" ");
    }
    Serial << endl;
    Serial.println("---------------------");
    Serial << "List<List<int> > ll = list_with(l,l2);" << endl;
    Serial << "This takes the tail of both lists and concatenates the result."
         << endl;
    Serial.println("---------------------");
    Serial << "bind(ll,tail) : "; // << endl;
    for (i = lq.begin(); i != lq.end(); ++i) {
      Serial << *i; Serial.print(" ");
    }
    Serial << endl;
    Serial.println("---------------------");
    Serial << "bind(ll,id) : "; // << endl;
    for (i = lid.begin(); i != lid.end(); ++i) {
      Serial << *i; Serial.print(" ");
    }
    Serial << endl;
    Serial.println("---------------------");
    //Serial << "bind_(ll,id) : "; // << endl;
    //for (i = lid2.begin(); i != lid2.end(); ++i) {
    //  Serial << *i << " ";
    //}
    //Serial << endl;
    //Serial << "---------------------" << endl;
    Serial << "bind(ll,map(inc)) : "; // << endl;
    for (i = linc.begin(); i != linc.end(); ++i) {
      Serial << *i; Serial.print(" ");
    }
    Serial << endl;
    Serial.println("---------------------");
    List<int> lr = joinM<ListM>()(ll);
    Serial << "joinM<ListM>()(ll) : "; // << endl;
    for (i = lr.begin(); i != lr.end(); ++i) {
       Serial << *i; Serial.print(" ");
    }
    Serial << endl;
    Serial.println("---------------------");
    List<int> ls = bindM<ListM>()(ll,id);
    Serial << "bindM<ListM>()(ll,id) : "; // << endl;
    for (i = ls.begin(); i != ls.end(); ++i) {
       Serial << *i; Serial.print(" ");
    }
    Serial << endl;
    Serial.println("---------------------");
    List<int> lt = bindM<ListM>()(ll,fcpp::map(inc));
    Serial << "bindM<ListM>()(ll,map(inc)) : "; // << endl;
    for (i = lt.begin(); i != lt.end(); ++i) {
       Serial << *i; Serial.print(" ");
    }
    Serial << endl;
    Serial.println("---------------------");
    List<int> lu = ListM::bind()(ll,fcpp::map(inc));
    Serial << "ListM::bind()(ll,map(inc)) : "; // << endl;
    for (i = lu.begin(); i != lu.end(); ++i) {
       Serial << *i; Serial.print(" ");
    }
    Serial << endl;
    Serial.println("---------------------");
    Serial << "Here is an example of bindM_ in action." << endl;
    Serial << "It takes the head of the list and discards it." << endl;
    Serial << "F is then applied to the second item in the list." << endl;
    Serial.println("---------------------");
    Serial<< "l : [ "; // << endl;
    for (i = l.begin(); i != l.end(); ++i) {
       Serial << *i; Serial.print(" ");
    }
    Serial << "]" << endl;
    Serial << "lambda(X,F)[bindM_<IdentityM>()[head[X], F] [head[tail[X]]] ](l,inc) = ";
    int res = lambda(X,F)[bindM_<IdentityM>()[head[X], F] [head[tail[X]]] ](l,inc);
    Serial << res << endl;
    Serial.println("---------------------");
    Serial << "lres = lambda(X,F)[bindM_<IdentityM>()[head[X], map[F] ] [tail[X]] ](l,inc);" << endl;
    List<int> lres = lambda(X,F)[bindM_<IdentityM>()[head[X], fcpp::map[F] ] [tail[X]] ](l,inc);
    Serial<< "lres : [ "; // << endl;
    for (i = lres.begin(); i != lres.end(); ++i) {
      Serial << *i; Serial.print(" ");
    }
    Serial << "]" << endl;
    Serial.println("---------------------");
    //IdentityM::bind_()(head(l),fcpp::map(inc));
    // I cannot do the above because I need the list at two places (as X in the line below.
    Serial << "These examples all do the same thing with different arrangements of lambda arguments." << endl; 
    Serial.println("---------------------");
    Serial << "List<int> lv = lambda(X,F)[bindM_<IdentityM>()[head[X], F] [tail[X]] ](l,fcpp::map(inc));" << endl;
    List<int> lv = lambda(X,F)[bindM_<IdentityM>()[head[X], F] [tail[X]] ](l,fcpp::map(inc));
    Serial.print("lv : [ ");
    for (i = lv.begin(); i != lv.end(); ++i) {
      Serial << *i; Serial.print(" ");
    }
    Serial.println("]");
    Serial.println("---------------------");
    Serial << "List<int> lw = lambda(X,F,G)[bindM_<IdentityM>()[F[X], G] [tail[X]] ](l,head,fcpp::map(inc));" << endl;
    List<int> lw = lambda(X,F,G)[bindM_<IdentityM>()[F[X], G] [tail[X]] ](l,head,fcpp::map(inc));
    Serial.print("lw : [ ");
    for (i = lw.begin(); i != lw.end(); ++i) {
      Serial << *i; Serial.print(" ");
    }
    Serial.println("]");
    Serial.println("---------------------");
    Serial << "List<int> lx = lambda(X,F,G,H)[bindM_<IdentityM>()[F[X], H] [G[X]] ](l,head,tail,fcpp::map(inc));" << endl;
    List<int> lx = lambda(X,F,G,H)[bindM_<IdentityM>()[F[X], H] [G[X]] ](l,head,tail,fcpp::map(inc));
    Serial.print("lx : [ ");
    for (i = lx.begin(); i != lx.end(); ++i) {
      Serial << *i; Serial.print(" ");
    }
    Serial.println("]");
    Serial.println("---------------------");
    //lambda(X,F,G,H )[ bindM_<ListM>()[F[X], H] [ G [X] ] ](ll, head, tail,fcpp::map(inc) );
    Serial.println("Exploring use of join with MaybeM and EitherM monads and associated values.");
    Serial.println("---------------------");
    //bindM<MaybeM>()(ll,id); fails as expected
/*
    // Exploring use of join with MaybeM and EitherM monads and associated values. 
    Maybe<int> mj = joinM<MaybeM>()( just (just (2) ) );
    Serial << "joinM<MaybeM>()( just (just (2) ) ) : ";
    // This is using the operator<< which I have defined.
    Serial << mj << endl;
    Serial << "MaybeM::join()( just (just (2) ) ) : ";
    Maybe<int> mj2 = MaybeM::join()( just (just (2) ) );
    Serial << mj2 << endl;
    Serial << "join( just (just (2) ) ) : ";
    Maybe<int> mj3 = join( just (just (2) ) );
    Serial << mj3 << endl;
    Serial << "joinM<EitherM>() ( right (right (2) ) ) : "; 
    Either<int> ej = joinM<EitherM>()( right (right (2) ) );
    Serial << ej << endl;
    Serial.print("EitherM::join()( right (right (2) ) ) : "); 
    Either<int> ej2 = EitherM::join() ( right (right (2) ) ); 
    Serial << ej2 << endl;
    Serial << "join ( right (right (2) ) ) : "; 
    Either<int> ej3 = join ( right (right (2) ) ); 
    Serial << ej3 << endl;
    Serial.println("---------------------");
*/
}

template <typename T>
Print &operator <<( Print &obj, const Maybe<T> &arg)
{
    if (arg.is_nothing()) { 
       obj.print("nothing");
    } else {
       obj.print("Just ");
       obj.print(arg.value());
    }
    return obj; 
}

template <typename T>
Print &operator <<( Print &obj, const Either<T> &arg)
{
    if (arg.is_error()) {
       obj.print(arg.left());
    } else {
       obj.print("Right ");
       obj.print(arg.right());
    }
    return obj; 
}
//////////////////////////////////////////////////////////
void unify_examples()
{
  List<int>::iterator it;
  Serial << "===============================" << endl;
  Serial << "Here is fmap actually working!!" << endl;
  Serial << "===============================" << endl;
  auto what = id ( just(3) );
  Serial << "id ( just(3) )                 : " << what
         << " (page 224)"<< endl;
  Serial << "liftM<MaybeM>()(id)( just(3) ) : "
         <<  liftM<MaybeM>()(id)( just(3) ) << endl;
  Serial << "liftM<EitherM>()(id)(right(3)) : "
         <<  liftM<EitherM>()(id)( right(3) ) << endl;
  Serial << "===============================" << endl;
  Serial << "Functor Law 1        (page 223)" << endl;
  Serial << "     fmap id = id" << endl;
  Serial << "===============================" << endl;
  Serial << "id ( just(3) )                 : "
            <<  id ( just(3) ) << endl;
  Serial << "fmap (id) ( just(3) )          : "
            <<  fmap (id)( just(3) ) << endl;
  Serial << "fmap (id) ( right(3) )         : "
            <<  fmap (id)( right(3) ) << endl;
  Serial << "fmap (id) ( ident(3) )         : "
            <<  fmap (id)( ident(3) ) << endl;
  Serial << "fmap (inc)( just(3) )          : "
            <<  fmap (inc)( just(3) ) << endl;
  Serial << "fmap (inc)( right(3) )         : "
            <<  fmap (inc)( right(3) ) << endl;
  Serial << "fmap (inc) ( ident(3) )        : "
            <<  fmap (inc)( ident(3) ) << endl;
  Serial << "(inc) ^fmap^ ( ident(3) )      : "
            <<  ((inc) ^fmap^ ( ident(3) )) << endl;
  Serial << "fmap( _, ident(3) )(inc)       : "
            <<  fmap( _, ident(3) )(inc) << endl;
  Serial << "fmap( _, just(3) )(inc)        : "
            <<  fmap( _, just(3) )(inc) << endl;
  Serial << "=================================" << endl;
  Serial << "Functor Law 2          (page 224)" << endl;
  Serial << "fmap (f . g ) = fmap(f) . fmap(g)" << endl;
  Serial << "=================================" << endl;
  Serial << " fmap (inc  ^dot^ dec)( ident(3) )     : "
            <<  fmap (inc ^dot^ dec)( ident(3) ) << endl;
  Serial << "(fmap (inc) ^dot^ fmap(dec))(ident(3)) : "
            <<  (fmap (inc) ^dot^ fmap(dec)) (ident(3) ) << endl;
  Serial << " fmap (inc  ^dot^ dec)( just(3) )      : "
            <<  fmap (inc ^dot^ dec)( just(3) ) << endl;
  Serial << "(fmap (inc) ^dot^ fmap(dec))(just(3))  : "
            <<  (fmap (inc) ^dot^ fmap(dec)) (just(3) ) << endl;
  Serial << " fmap (inc  ^dot^ dec)( right(3) )     : "
            <<  fmap (inc ^dot^ dec)( right(3) ) << endl;
  Serial << "(fmap (inc) ^dot^ fmap(dec))(right(3)) : "
            <<  (fmap (inc) ^dot^ fmap(dec)) (right(3) ) << endl;
  List<int> fl2_1 = makeList1(3);
  List<int> fl2_2 = fmap (inc ^dot^ dec)( fl2_1 );
  List<int> fl2_3 = (fmap (inc) ^dot^ fmap(dec)) (fl2_1 );
  Serial << " fmap (inc ^dot^ dec)( fl2_1 ) = [ ";
  for (it = fl2_2.begin(); it != fl2_1.end(); ++it) {
     Serial << *it << " ";
  }
  Serial << "]" << endl;
  Serial << "(fmap (inc) ^dot^ fmap(dec))(fl2_1) = [ ";
  for (it = fl2_3.begin(); it != fl2_3.end(); ++it) {
     Serial << *it << " ";
  }
  Serial << "]" << endl;
  Serial << "=================================" << endl;
  Serial << "More examples. p.229" << endl;
  Serial << "=================================" << endl;
  Serial << "(just(plus(3)) ^star^ just(9)) : "
            <<  (just(plus(3)) ^star^ just(9)) << endl;  // p.229
  Serial << "====================================" << endl;
  Serial << "Applicative Functor Law 1 (page 238)" << endl;
  Serial << " pure f <*> x = fmap f x" << endl;
  Serial << "====================================" << endl;
  Serial << "fmap (inc) ( just(3) )         : "
            <<  fmap (inc)( just(3) ) << endl;
  Maybe<int> mx1 = pure (inc) ^star^ ( just(3) );
  Serial << "pure (inc) ^star^ ( just(3) )  : "
            <<  mx1 << endl;
  Maybe<int> mx1a = MaybeA::pure()(inc) ^star^ (3);
  Serial << "MaybeA::pure()(inc) ^star^ (3) : "
            <<  mx1a << endl;
  // pure is actually optional here as pure == id when on its own.
  Maybe<int> mx1b = inc ^star^ ( just(3) );
  Serial << "inc ^star^ (3) : " <<  mx1b << endl;
  Serial << "====================================" << endl;
  Serial << "Applicative Functor Law 2 (page 238)" << endl;
  Serial << " pure id <*> v = v" << endl;
  Serial << "====================================" << endl;
  Maybe<int> mx2 = pure (id) ^star^ ( just(3) );
  Serial << "pure (id) ^star^ ( just(3) )    : "
            <<  mx2 << endl;
  Serial << "====================================" << endl;
  Serial << "Applicative Functor Law 3 (page 238)" << endl;
  Serial << "pure (.)<*>u<*>v<*>w = u<*>(v<*>w)" << endl;
  Serial << "Note: in the example one of the operators cannot be inferred."
            << endl;
  Serial << "====================================" << endl;
  Maybe<int> mx3a = pure (dot) ^star^ pureA<MaybeA>()(inc)
                    ^star^ dec ^star^ ( just(3) );
  Maybe<int> mx3b = inc ^star^ (dec ^star^ ( just(3) ));
  Serial << "pure(dot) ^star^ pureA<MaybeA>()(inc) ^star^ dec ^star^ ( just(3) )    : "
            <<  mx3a << endl;
  Serial << "inc ^star^ (dec ^star^ ( just(3) )) : "
            <<  mx3b << endl;
  Serial << "====================================" << endl;
  Serial << "Applicative Functor Law 4 (page 238)" << endl;
  Serial << " pure f <*> pure x = pure ( f x )" << endl;
  Serial << "====================================" << endl;
  Maybe<int> mx4a = pure (id) ^star^ ( just(3) );
  Maybe<int> mx4a2 = pure (id) ^star^ pure ( just(3) );
  Maybe<int> mx4b = pure (id ( just(3) ) );
  Serial << "pure (id) ^star^ ( just(3) )    : "
            <<  mx4a << endl;
  Serial << "pure (id) ^star^ pure ( just(3) )    : "
            <<  mx4a2 << endl;
  Serial << "pure (id ( just(3) ) )          : "
            <<  mx4b << endl;
  Maybe<int> mx4c = pure (inc) ^star^ ( just(3) );
  // These are all equivalent. MaybeA::pure provides just.
  // The problem is how to get pure to know what it is doing.
  // At the moment the action is taken in star.
  // That does not work for pure on its own.
  // There is a comment on p.228 that there is no default implementation
  // of 'pure' so it needs to be defined in the applicative functor.
  // These examples do this for the 'pure' in MaybeA which equates to 'just'.
  Maybe<int> mx4c2 = (just(inc)) ^star^ ( just(3) );
  Maybe<int> mx4c3 = (pureA<MaybeA>()(inc)) ^star^ ( just(3) );
  Maybe<int> mx4c4 = (MaybeA::pure()(inc)) ^star^ ( just(3) );
  //Maybe<int> mx4d = pure (pureA<MaybeA>()(inc) ( just(3) ) );
  Maybe<int> mx4d = pure (liftM<MaybeM>()(inc) ( just(3) ) );
  Serial << "pure (inc) ^star^ ( just(3) )    : "
            <<  mx4c << endl;
  Serial << "pure (liftM<MaybeM>()(inc) ( just(3) ) ) : "
            <<  mx4d << endl;
#ifdef FCPP_UNIFY
  // MaybeA now behaving like MaybeM.
  Maybe<int> mx4du = pure (liftM<MaybeA>()(inc) ( just(3) ) );
  Serial << "---------------------------" << endl;
  Serial << "MaybeA now acting as MaybeM" << endl;
  Serial << "pure (liftM<MaybeA>()(inc) ( just(3) ) ) : "
            <<  mx4du << endl;
  Serial << "---------------------------" << endl;
#endif
  Maybe<int> mx4d2 = pure ( fmap(inc) ( just(3) ) );
  Serial << "pure ( fmap(inc) ( just(3) ) ) : "
            <<  mx4d2 << endl;
  Maybe<int> mx4d3 = fmap(inc) ( just(3) );
  Serial << "fmap(inc) ( just(3) ) : "
            <<  mx4d3 << endl;
  // This is the correct action for the RHS of the law.
  Maybe<int> mx4d4 = MaybeA::pure()(inc (3));
  Maybe<int> mx4d5 = just(inc (3));
  // At the moment 'pure' == 'id' so this is not surprising.
  Maybe<int> mx4e = pure (inc (3) );
  Serial << "mx4e = pure (inc (3) )  : "
            <<  mx4e << endl;
  // The result is created from the assignment.
  Either<int> ex4e = pure (inc (3) );
  Serial << "ex4e = pure (inc (3) )  : "
            <<  ex4e << endl;
  Serial << "pure (inc (3) )  : " << pure (inc (3) ) << endl;
  Serial << "====================================" << endl;
  Serial << "Applicative Functor Law 5 (page 238)" << endl;
  Serial << " u <*> pure y = pure ( $ y ) <*> u" << endl;
  Serial << "Note: $ is the infix operator for fmap." << endl;
  Serial << "Notation not reproduced completely" << endl;
  Serial << "====================================" << endl;
  // One side or the other of star must give the type information.
  // Note: pureA<MaybeA>() == MaybeA::pure() == just. All equivalent.
  Maybe<int> mx5a = pureA<MaybeA>()(inc) ^star^ ( pure(3) );
  Maybe<int> mx5a2 = pure(inc) ^star^ ( just(3) );
  // This fails as it cannot deduce a type from pure alone.
  //Maybe<int> mx5a3 = pure(inc) ^star^ ( pure(3) );
  Maybe<int> mx5b = fmap(_,just(3)) (inc);
  Maybe<int> mx5c = (_ ^fmap^ just(3)) (inc);
  // This one fails because inc does not need to be lifted here.
  //Maybe<int> mx5d = (_ ^fmap^ just(3)) ( pureA<MaybeA>()(inc) );
  Serial << "pureA<MaybeA>()(inc) ^star^ ( pure(3) ) : "
            <<  mx5a << endl;
  Serial << "pure( inc ) ^star^ ( just(3) ) : "
            <<  mx5a2 << endl;
  Serial << "(_ ^fmap^ just(3)) (inc)        : "
            <<  mx5c << endl;
  Serial << "====================================" << endl;
  Serial << "Application of the various laws" << endl;
  Serial << "====================================" << endl;
  Serial << " pure f <*> x = fmap f x" << endl;
  Serial << "This example implements the RHS of the law." << endl;
  Serial << "That is equivalent to operating the LHS of the law"
            << endl;
  Serial << "=================================================="
            << endl;
  Fun1<int,int> finc(inc), fdec(dec); // Monomorphic versions
  List<int> l1 = list_with(1,2);
  List<int> l2 = pureA<ListA>()(finc) ^star^ l1;
  List<int> l3 = fcpp::map(finc,l1);
  List<int> l4 = fmap(finc,l1);
  List<Fun1<int,int> > lf1 = list_with(finc);
  List<int> l5 = star(lf1)(l1);
  //List<int> l6 = star(pure(finc),l1); Cannot deduce list type.

  List<Maybe<int> >::iterator imt;
  List<Maybe<int> > lm1 = unitM<MaybeT<ListM> >()(just(1));
  Serial << "unitM<MaybeT<ListM> >()(just(1))    = [ ";
  for (imt = lm1.begin(); imt != lm1.end(); ++imt) {
     Serial << *imt << " ";
  }
  Serial << "]" << endl;
#ifdef FCPP_UNIFY
  Serial << "-----------------------------------------------"
            << endl;
  List<Maybe<int> > lm1a = unitM<MaybeT<ListA> >()(just(1));
  Serial << "unitM<MaybeT<ListA> >()(just(1))    = [ ";
  for (imt = lm1a.begin(); imt != lm1a.end(); ++imt) {
     Serial << *imt << " ";
  }
  Serial << "]" << endl;
  Serial << "-----------------------------------------------"
            << endl;
#endif
  List<Maybe<int> > lm2 = fmap (id)( lm1 );
  Serial << "fmap (id)  (lm1)                    = [ ";
  for (imt = lm2.begin(); imt != lm2.end(); ++imt) {
     Serial << *imt << " ";
  }
  Serial << "]" << endl;
  List<Maybe<int> > lm3 = fmap (liftM<MaybeM>()(inc))( lm1 );
  Serial << "fmap (liftM<MaybeM>()(inc)) (lm1)   = [ ";
  for (imt = lm3.begin(); imt != lm3.end(); ++imt) {
     Serial << *imt << " ";
  }
  Serial << "]" << endl;
  List<Maybe<int> > lm4 = (liftM<MaybeM>()(inc)) ^fmap^ ( lm1 );
  Serial << "(liftM<MaybeM>()(inc)) ^fmap^ (lm1) = [ ";
  for (imt = lm4.begin(); imt != lm4.end(); ++imt) {
     Serial << *imt << " ";
  }
  Serial << "]" << endl;
#ifdef FCPP_UNIFY
  Serial << "-----------------------------------------------"
            << endl;
  List<Maybe<int> > lm5 = (liftM<MaybeA>()(inc)) ^fmap^ ( lm1 );
  Serial << "(liftM<MaybeA>()(inc)) ^fmap^ (lm1) = [ ";
  for (imt = lm5.begin(); imt != lm4.end(); ++imt) {
     Serial << *imt << " ";
  }
  Serial << "]" << endl;
#endif

}
//////////////////////////////////////////////////////////

void monad_examples() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  while (!Serial) { }

  Serial.println("Pico RP2040 FC++ Monads ");
  Serial.println("--------------------------");
  Serial.println("Some simple FC++ operations");
  Serial.print("plus(1,2) = "); Serial.println(plus(1,2));
  Serial.print("plus(1.5,2.3) = "); Serial.println(plus(1.5,2.3));
  Serial.print("plus(1)(2) = "); Serial.println(plus(1)(2));
  Serial.print("inc(1) = "); Serial.println(inc(1));
  Serial.println("--------------------------");
  Serial.println("Some simple FC++ Monad operations");
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
  // Use of Monad MaybeM with liftM to move inc into Maybe.
  mz = liftM<MaybeM>()(inc)(mx);
  Serial.println("mz = liftM<MaybeM>()(inc)(mx);");
  if (mz.is_nothing()) { 
     Serial.println("mz is nothing");
  } else {
     Serial.print("mz has "); Serial.println(mz.value());
  }
  Serial.println("--------------------------");
/*
  Serial << "--------------------------" << endl;
  LambdaVar<4> W;
  LambdaVar<1> X;
  LambdaVar<2> Y;
  LambdaVar<3> Z;
  Serial << "h(-1,2) = " << h(-1,2) << endl;
  Maybe<int> mhr = mh(-1,2);
  if (mhr.is_nothing()) { 
     Serial << "mhr is nothing" << endl;
  } else {
     Serial << "mhr has " << mhr.value() << endl;
  }
  Maybe<int> mgr = liftM<MaybeM>()(pg)(mf(2));
  if (mgr.is_nothing()) { 
     Serial << "mgr is nothing" << endl;
  } else {
     Serial << "mgr has " << mgr.value() << endl;
  }
  mz = lambda()[ compM<MaybeM>()[ inc[Z] | Z <= just[2] ] ]();
  if (mz.is_nothing()) { 
     Serial << "mz is nothing" << endl;
  } else {
     Serial << "mz has " << mz.value() << endl;
  }
  mhr = lambda()[ compM<MaybeM>()[ Z | Z <= just[2] ] ]();
  // Putting (X) makes it a parameter.
  mhr = lambda(X)[ compM<MaybeM>()[ Z | Z <= just[X] ] ](2);
  mhr = lambda(X)[ compM<MaybeM>()[ Z | Z <= pmf[X] ] ](2);
  mhr = lambda(X)[ compM<MaybeM>()[ Z | Y <= pmf[X], W <=  pmg[Y], Z <=  pmh[Y,W] ] ](2);
  if (mhr.is_nothing()) { 
     Serial << "mhr is nothing" << endl;
  } else {
     Serial << "mhr has " << mhr.value() << endl;
  }
  // Failure case
  mhr = lambda(X)[ compM<MaybeM>()[ Z | Y <= pmf[X], W <=  pmg[Y], Z <=  pmh[Y,W] ] ](-2);
  if (mhr.is_nothing()) { 
     Serial << "mhr is nothing" << endl;
  } else {
     Serial << "mhr has " << mhr.value() << endl;
  }
  Serial << "--------------------------" << endl;


  List<double>::iterator idx1, idx2;
  List<double> x1 = list_with(0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0);
  List<double> x2 = fcpp::map(fcpp::minus(1.0),x1);
  double sumx1 = foldl(plus,0.,x1);
  for (idx1 = x1.begin(), idx2 = x2.begin(); idx1 != x1.end(); ++idx1, ++idx2) {
     Serial << *idx1 << " " << *idx2 << endl;
  }
  Serial << "Sum of x1 = " << sumx1 << endl;
  Serial << "--------------------------" << endl;

  List<int> ix1 = enumFromTo(0,10);
  List<bool> bodds = fcpp::map(odd,ix1);
  List<bool> bevens = fcpp::map(even,ix1);
  List<int> odds = filter(odd,ix1);
  int sum_odds = foldl(plus,0,odds);
  int sum_odds2 = foldl(plus,0,filter(odd,ix1));
  List<int>::iterator iix1;
  List<bool>::iterator ib1, ib2;
  Serial << "Length of bodds is " << length(bodds) << endl;
  for (iix1 = ix1.begin(), ib1 = bodds.begin(), ib2 = bevens.begin(); iix1 != ix1.end(); ++iix1, ++ib1, ++ib2) {
     Serial << *iix1 << " " << *ib1 << " " << *ib2 << endl;
  }

  Serial << "Length of odds is " << length(odds) << endl;
  Serial << "sum of the odds is " << sum_odds << endl;
  explore_bindm();
  while (!delay_without_delaying(5000) ) { };
*/
}

void setup() {
  monad_examples();
  unify_examples();
}
void loop() {
  // put your main code here, to run repeatedly:

}
