////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Monad
// I want to extend this to explore some of the things about Monads
// See https://wiki.haskell.org/Typeclassopedia for some explanation.
// There are some things there which I have never seen before.
// I think that some of them were not around when the original FC++
// was written.
// I have npw found that what I have done in the past is to add things
// to the applicative examples in fcpp/functors.h to make them act as monads.
// This is done with FCPP_UNIFY.
// I did not do the reverse, which is to make the monads in fcpp/monads.h applicative.
// What that means is that MaybeA in fcpp/functors.h can act as MaybeM
// but not the opposite.
// This would be difficult to do given the order of the header files.
// I think what I will do is leave fcpp/monads.h unchanged and do the new things
// in fcpp/functors.h.
////////////////////////////////////////////////////////////////
// was Arduino RP2040 Monad
////////////////////////////////////////////////////////////////
// was DUEMONAD
// Demo of FC++ Maybe Monad operations
// This will work on ARDUINO DUE but not on AVR boards.
// 5 parameter functoids and more for example working.
// Function 6, 7 and 8 now operational.
#define FCPP152
#define FCPP_ENABLE_LAMBDA
#define FCPP_EXTRA_STRONG_CURRY
#define FCPP_PATTERN // define this to include pattern.h
#define FCPP_UNIFY // Unification of Monads and Functors in functors.h
#include "fcpp_prelude.h"
#include "fcpp/monoids.h"
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
   //LambdaVar<2> Y;
   //LambdaVar<3> Z;
   //LambdaVar<4> W;
   //LambdaVar<5> A;
   //LambdaVar<6> P;
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

//////////////////////////////////////////////////////////
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

template <class T>
class B {
protected:
  int x_;
public:
  B() : x_(0) { }
  B(int x) : x_(x) { }
  B(B const& b) : x_(b.x_) { }
  B<T> &operator= (const B& b) { this->x_ = b.x_; return *this; }
};

template <class T>
class A : public B<T> {
public:
  using B<T>::x_;
  A() { }
  A(int y) : B<T>(y) { }
  A(const B<T> &b) : B<T>(b) { }
  A(const A<T> &a) : B<T>(a) { /*this->x_ = a.x_;*/ /* *this = a; */ } 
  A<T> &operator= (const A<T>& a) { B<T>::operator=(a); return *this; }
  A<T> &operator= (const B<T>& b) { B<T>::operator=(b); return *this; }
};



void setup() {
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
  List<int> li0;
  List<int> li1(li0);
  List<int> li2 = li1;
  Mlist<int> mli0;
  //Mlist<int> mli1(mli0);
  Mlist<int> mli2(li0);
  Mlist<int> mli3 = mli0;
  A<int> a0;
  B<int> b0;
  A<int> a1(a0);
  A<int> a2(b0);
  A<int> a3 = a0;
  A<int> a4 = b0;
  Serial << "--------------------------" << endl;
/*  LambdaVar<4> W;
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
*/
  explore_bindm();
  //while (!delay_without_delaying(5000) ) { };
}

void loop() {
  // put your main code here, to run repeatedly:

}
