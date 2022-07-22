//////////////////////////////////////////////////////////////////////
// Arduino RP2040 FC++ Parser example from old parser.cpp
//////////////////////////////////////////////////////////////////////
// I think this is a significant comment from the original file.
// The point is that the origin of this code is older than the Monad implementation.
/* This is from the client_index file.

parser              Monadic parser combinators.  A bit of a mess, but
                       they work.  This program inspired Brian to implement
                       monads/lambda, but he never went back to clean up
                       the program much after lambda/monads were implemented.
*/
//////////////////////////////////////////////////////////////////////
// This code is going to be kept as close as possible to parser.cpp
// New ideas will be in a new version.
//////////////////////////////////////////////////////////////////////

#include <string>
#define FCPP152
#define FCPP_ENABLE_LAMBDA
#include "fcpp_prelude.h"

#include <Streaming.h>

using namespace fcpp;

/// I have renamed String to StringL to avoid a name clash with the Arduino use of String.
typedef List<char> StringL;

/// Parser monad which is based on the work of Hutten and Meijer.
/// I have the paper.
/// This is a translation of the Haskell in the paper into FC++
/// It is useful to compare the two.
/// The paper does not mention Unit and Bind.
struct ParserM {
   // M a = StringL -> [(a,StringL)]

   // We use indirect functoids as a representation type, since we will
   // often need two functions with different behaviors (but the same
   // signatures) to appear to have the same type.
   template <class A> struct Rep
      { typedef Fun1<StringL,List<std::pair<A,StringL> > > Type; };
   template <class MA> struct UnRep { typedef typename
      RT<MA,StringL>::ResultType::ElementType::first_type Type; };

   // ReRep is a type-identity in Haskell; here it "indirect"ifies the
   // type, so direct functoids are turned into indirect ones so that
   // only the signature information appears in the type.
   template <class MA> struct ReRep
      { typedef typename Rep<typename UnRep<MA>::Type>::Type Type; };

   struct XUnit {
      template <class A> struct Sig : public FunType<A,
         typename Rep<A>::Type> {};
      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a ) const {
         LambdaVar<1> S;
         return lambda(S)[ cons[makePair[a,S],NIL] ];
      }
   };
   typedef Full1<XUnit> Unit;
  // This change and the one for Bind got rid of many errors.
   static Unit& unit() {static Unit f; return f;}
  //static Unit unit;

   struct XBind {
      template <class M, class K> struct Sig : public FunType<M,K,typename
         ReRep<typename RT<K,typename UnRep<M>::Type>::ResultType>::Type> {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         LambdaVar<1> P;
         LambdaVar<2> S;
         return lambda(S)[ concat[ compM<ListM>()
            [ k[fst[P]][snd[P]] | P <= m[S] ] ] ];
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}
  //static Bind bind;

   typedef Fun1<StringL,AUniqueTypeForNil> Zero;
   static Zero& zero() {static Zero f; return f;}
  //static Zero zero;
};
//ParserM::Unit ParserM::unit;
//ParserM::Bind ParserM::bind;
//ParserM::Zero ParserM::zero; //= ignore( const_(NIL) );

struct XItem : public CFunType<StringL,OddList<std::pair<char,StringL> > > {
   OddList<std::pair<char,StringL> > operator()( const StringL& s ) const {
      if( null(s) )
         return NIL;
      else
         return cons( makePair( head(s), tail(s) ), NIL );
   }
};
typedef Full1<XItem> Item;
Item item;

// I think this is the (+++) in the paper page 4.
struct XPlusP {
   template <class P, class Q, class S> struct Sig : public 
      FunType<P,Q,StringL,typename RT<Cat,typename RT<P,StringL>::ResultType,
      typename RT<Curry1,Q,StringL>::ResultType>::ResultType> {};
   template <class P, class Q>
   typename Sig<P,Q,StringL>::ResultType
   operator()( const P& p, const Q& q, const StringL& s ) const {
      return p(s) ^cat^ curry1(q,s);
   }
};
typedef Full3<XPlusP> PlusP;
PlusP plusP;

/// Note that in the paper many uses many1 and many1 uses many.
/// In this implementation many does not use many1.
template <class Monad>
struct XManyM {
   // Monad a -> Monad [a]
   template <class MA>
   struct Sig : public FunType<MA, typename RT<typename
      PlusM<Monad>::Type, typename LEType<LAM<COMP<CALL<Cons,LV<1>,LV<2>
      >,GETS<1,MA>,GETS<2,CALL<Full1<XManyM<Monad> >,MA> > > > >::Type,
      typename RT<typename UnitM<Monad>::Type, List<typename
      Monad::template UnRep<MA>::Type> >::ResultType>::ResultType> {};

   template <class MA>
   typename Sig<MA>::ResultType operator()( const MA& ma ) const {
      typedef typename Monad::template UnRep<MA>::Type AA;
      LambdaVar<1> A;
      LambdaVar<2> AS;
      return lambda()[ compM<Monad>()[ cons(A,AS) | 
         A <= ma, AS <= makeFull1(*this)[ma] ] ]() 
         ^plusM<Monad>()^ unitM<Monad>()( List<AA>() );
   }
};
// Just using parser version here:    Parser a -> Parser [a]
typedef Full1<XManyM<ParserM> > Many;
Many many;

// sat :: (char -> bool) -> Parser char
struct XSat {
   template <class P> struct Sig : public FunType<P,
      typename RT<typename LEType<LAM<COMP<ParserM,LV<1>,GETS<1,Item>,
      GUARD<CALL<P,LV<1> > > > > >::Type>::ResultType> {};
   template <class P>
   typename Sig<P>::ResultType
   operator()( const P& p ) const {
      LambdaVar<1> C;
      return lambda()[ compM<ParserM>()[ C | C<=item, guard[p[C]] ] ]();
   }
};
typedef Full1<XSat> Sat;
Sat sat;

struct XCharP : public CFunType<char, 
   RT<Sat,RT<Equal,char>::ResultType>::ResultType> {
   RT<Sat,RT<Equal,char>::ResultType>::ResultType
   operator()( char c ) const {
      return sat( equal(c) );
   }
};
typedef Full1<XCharP> CharP;
CharP charP;

// OLD COMMENTS
// I have not compiled this for a long time as it now fails.
// The reason is that prelude.h now has an implementation of between
// which clashes with this.
// There are several reasons.
// First, this is not in namespace impl.
// Second, the arguments are in a different order.
// Here goal is first and in prelude.h it is last.
// I am going to call this Between2 and adjust any cases of use.

struct XBetween2 {
   template <class T, class U, class V> struct Sig
      : public FunType<T,T,T,bool> {};
   template <class T>
   bool operator()( const T& goal, const T& lower, const T& upper ) const {
      return lessEqual(goal,upper) && greaterEqual(goal,lower);
   }
};
typedef Full3<XBetween2> Between2;
Between2 between2;

// These are some very useful implementations.
typedef RT<Sat,RT<Between2,AutoCurryType,char,char>
   ::ResultType>::ResultType Digit;
Digit digit = sat( between2(_,'0','9') );

typedef Digit Lower;
Lower lower = sat( between2(_,'a','z') );

typedef Digit Upper;
Upper upper = sat( between2(_,'A','Z') );

typedef RT<PlusP,Lower,Upper>::ResultType Letter;
Letter letter = lower ^plusP^ upper;

typedef RT<PlusP,Letter,Digit>::ResultType AlphaNum;
AlphaNum alphaNum = letter ^plusP^ digit;

template <class Monad>
struct XMany1M {
   // Monad a -> Monad [a]
   template <class MA>
   struct Sig : public FunType<MA, typename LEType<LAM<COMP<
      CALL<Delay,CALL<Cons,LV<1>,LV<2> > >,GETS<1,MA>,GETS<2,
      CALL<Full1<XManyM<Monad> >,MA> > > > >::Type> {};

   template <class MA>
   typename Sig<MA>::ResultType
   operator()( const MA& ma ) const {
      LambdaVar<1> A;
      LambdaVar<2> AS;
      return lambda()[ compM<Monad>() [ fcpp::delay[cons[A,AS]] | 
         A <= ma, AS <= makeFull1(XManyM<Monad>())[ma] ] ]();
   }
};
// Just using parser version here:    Parser a -> Parser [a]
typedef Full1<XMany1M<ParserM> > Many1;
Many1 many1;

struct XChainl1 {
   // Parser a -> Parser (a->a->a) -> Parser a
   // parses a series of items separated by left-associative operators

   typedef BindM<ParserM>::Type BIND;
   typedef UnitM<ParserM>::Type UNIT;
   template <class P, class O>
   struct XRest {
      P p;
      O op;
      XRest( const P& pp, const O& oo ) : p(pp), op(oo) {}

      template <class X> struct Sig : public FunType<X,
         typename ParserM::Rep<X>::Type> {};
      template <class X>
      typename Sig<X>::ResultType
      operator()( const X& x ) const {
         LambdaVar<1> F;
         LambdaVar<2> Y;
         return (op ^BIND()^ lambda(F)[ 
                 p  %BIND()% lambda(Y)[
                 makeFull1(*this)[ F[x,Y] ] ] ]) ^plusP^ UNIT()(x);
      }
   };

   template <class P, class O> struct Sig : public FunType<P,O,
      typename RT<BIND,P,Full1<XRest<P,O> > >::ResultType> {};
   template <class P, class O>
   typename Sig<P,O>::ResultType
   operator()( const P& p, const O& op ) const {
      return p ^BIND()^ makeFull1(XRest<P,O>(p,op));
   }
};
typedef Full2<XChainl1> Chainl1;
Chainl1 chainl1;

// This is mentioned in the paper and not implemented.
struct XChainr1 {
   // Parser a -> Parser (a->a->a) -> Parser a
   // parses a series of items separated by right-associative operators

   template <class P, class O> struct Sig : public FunType<P,O,
      typename ParserM::Rep<typename ParserM::UnRep<P>::Type>::Type> {};
   template <class P, class O>
   typename Sig<P,O>::ResultType
   operator()( const P& p, const O& op ) const {
      LambdaVar<1> F;
      LambdaVar<2> X;
      LambdaVar<3> Y;
      return p ^bindM<ParserM>()^ lambda(X)[ compM<ParserM>()
         [ F[X,Y] | F <= op, Y <= makeFull2(*this)[p][op] ]
         %plusP% unitM<ParserM>()[X] ];
   }
};
typedef Full2<XChainr1> Chainr1;
Chainr1 chainr1;

struct XChainl {
   template <class P, class O, class V> struct Sig : public FunType<P,O,V,
      typename RT<PlusP,typename RT<Chainl1,P,O>::ResultType,
      typename RT<typename UnitM<ParserM>::Type,V>::ResultType>::ResultType> {};
   template <class P, class O, class V>
   typename Sig<P,O,V>::ResultType
   operator()( const P& p, const O& op, const V& v ) const {
      return (p ^chainl1^ op) ^plusP^ unitM<ParserM>()(v);
   }
};
typedef Full3<XChainl> Chainl;
Chainl chainl;

// This is mentioned in the paper and not implemented.
struct XChainr {
   template <class P, class O, class V> struct Sig : public FunType<P,O,V,
      typename RT<PlusP,typename RT<Chainr1,P,O>::ResultType,
      typename RT<typename UnitM<ParserM>::Type,V>::ResultType>::ResultType> {};
   template <class P, class O, class V>
   typename Sig<P,O,V>::ResultType
   operator()( const P& p, const O& op, const V& v ) const {
      return (p ^chainr1^ op) ^plusP^ unitM<ParserM>()(v);
   }
};
typedef Full3<XChainr> Chainr;
Chainr chainr;

// This is not in the paper. I think it is constructing a number from digits.
typedef RT<LEType<LAM<LET<BIND<1,LAM<LV<2>,LV<3>,CALL<Plus,
   CALL<Multiplies,int,LV<2> >,LV<3> > > >,CALL<Chainl1,COMP<ParserM,
   CALL<Construct1<int>::Type,CALL<Minus,LV<4>,char> >,GETS<4,Digit> >,
   CALL<UnitM<ParserM>::Type,LV<1> > > > > >::Type>::ResultType Nat;
Nat xnat() {
   LambdaVar<1> O; // was OP
   LambdaVar<2> M;
   LambdaVar<3> N;
   LambdaVar<4> X;
   return lambda()[ let[ 
      O == lambda(M,N)[ fcpp::plus[fcpp::multiplies[10,M],N] ]
      ].in[ compM<ParserM>()[ construct1<int>()[fcpp::minus[X,'0']] | X<=digit ]
            %chainl1% unitM<ParserM>()[O] ] ]();
}
Nat nat = xnat();

/* This will not compile: 'IntP' does not name a type
 *  There must be something wrong with the typedef although it does compile.
 *  Experiment shows that it does not work as a type for  IntP xintp() {  }
 *  and it does work as a type for IntP intP = xintp();
 * I do not have an explanation for this.
 */

typedef RT<LEType<LAM<LET<BIND<3,CALL<PlusP,COMP<ParserM,CALL<
   Construct1<Fun1<int,int> >::Type,fcpp::Negate>,CALL<CharP,char> >,CALL<
   UnitM<ParserM>::Type,CALL<Construct1<Fun1<int,int> >::Type,fcpp::Id> > > >, 
   COMP<ParserM,CALL<LV<1>,LV<2> >,GETS<1,LV<3> >,GETS<2,Nat> 
   > > > >::Type>::ResultType IntP;

 /* attempt at simplification which made no difference.
typedef Construct1<Fun1<int,int> >::Type Con1;
typedef UnitM<ParserM>::Type Unit1;
typedef RT<LEType<LAM<LET<BIND<3,CALL<PlusP,COMP<ParserM,CALL<
   Con1,fcpp::Negate>,CALL<CharP,char> >,CALL<
   Unit1,CALL<Con1,fcpp::Id> > > >, 
   COMP<ParserM,CALL<LV<1>,LV<2> >,GETS<1,LV<3> >,GETS<2,Nat> 
   > > > >::Type>::ResultType IntP;
*/
// I have no idea why using auto makes this work when the line IntP intP = xintp();
// The code was written for C++ and is now being compiled with C++17.
//IntP xintp() {
auto xintp() {
   LambdaVar<1> F;
   LambdaVar<2> N;
   LambdaVar<3> O;  // O was OP
   Construct1<Fun1<int,int> >::Type cf = construct1<Fun1<int,int> >();
   return lambda()[ let[ O == compM<ParserM>()[ cf[fcpp::negate] | charP['-'] ]
                               %plusP% unitM<ParserM>()[ cf[id] ] ] 
      .in[ compM<ParserM>()[ F[N] | F<=O, N<=nat ] ]   ]();
}
IntP intP = xintp();


struct XSepBy1 {
   // Parser a -> Parser b -> Parser [a]
   // parses "p (sep p)*", throwing away the separators
   template <class P, class S> struct Sig : public FunType<P,S,
      typename RT<typename LEType<LAM<COMP<ParserM,CALL<Delay,
      CALL<Cons,LV<1>,LV<2> > >,GETS<1,P>,GETS<2,CALL<Many,
      COMP<ParserM,LV<3>,S,GETS<3,P> > > > > > >::Type>::ResultType> {};
   template <class P, class S>
   typename Sig<P,S>::ResultType
   operator()( const P& p, const S& sep ) const {
      LambdaVar<1> X;
      LambdaVar<2> XS;
      LambdaVar<3> Y;
      return lambda()[ compM<ParserM>()[ fcpp::delay[cons[X,XS]] |
         X <= p, XS <= many[ compM<ParserM>()[ Y | sep, Y <= p ] ] ] ]();
   }
};
typedef Full2<XSepBy1> SepBy1;
SepBy1 sepBy1;

struct XBracket {
   template <class O, class P, class C> struct Sig : public FunType<O,P,C,
      typename RT<typename LEType<LAM<COMP<ParserM,LV<1>,O,GETS<1,P>,C>
      > >::Type>::ResultType> {};
   template <class O, class P, class C>
   typename Sig<O,P,C>::ResultType
   operator()( const O& open, const P& p, const C& close ) const {
      LambdaVar<1> X;
      return lambda()[ compM<ParserM>()[ X | open, X<=p, close ] ]();
   }
};
typedef Full3<XBracket> Bracket;
Bracket bracket;

struct XSepBy {
   template <class P, class S> struct Sig : public FunType<P,S,
      typename RT<PlusP,typename RT<SepBy1,P,S>::ResultType,
      typename RT<UnitM<ParserM>::Type,List<typename
      ParserM::UnRep<P>::Type> >::ResultType>::ResultType> {};
   template <class P, class S>
   typename Sig<P,S>::ResultType
   operator()( const P& p, const S& sep ) const {
      typedef typename ParserM::UnRep<P>::Type A;
      List<A> l = NIL;
      return (p ^sepBy1^ sep) ^plusP^ unitM<ParserM>()( l );
   }
};
typedef Full2<XSepBy> SepBy;
SepBy sepBy;

struct XOps {
   // [(Parser a, b)] -> Parser b
   // given a list of pair<parser to parse op,op>, returns a parser
   template <class X> struct Sig : public FunType<X,
      typename RT<Foldr1,PlusP,typename RT<typename LEType<LAM<
      COMP<ListM,COMP<ParserM,CALL<Snd,LV<1> >,CALL<Fst,LV<1> > >,
      GETS<1,X> > > >::Type>::ResultType>::ResultType> {};
   template <class X>
   typename Sig<X>::ResultType
   operator()( const X& xs ) const {
      LambdaVar<1> P;
      return foldr1( plusP, lambda()[ compM<ListM>()[ 
         compM<ParserM>()[ snd[P] | fst[P] ]   | P <= xs ] ]() );
   }
};
typedef Full1<XOps> Ops;
Ops ops;


// More to come from line 414 onwards in parser.cpp

//////////////////////////////////////////////////////////
// Output operators have to be done differently.
//////////////////////////////////////////////////////////

int my_pow( int x, int y ) {
   int r = 1;
   while(y) {
      r *= x;
      --y;
   }
   return r;
}

typedef ParserM::Rep<int>::Type ExprP;
extern ExprP exprP;

typedef RT<Ops,List<std::pair<RT<CharP,char>::ResultType,Fun2<int,int,int> 
   > > >::ResultType AddOp;
//AddOp xaddOp() { we have another one.
auto xaddOp() {
   typedef Fun2<int,int,int> F2; // was F which clashes
   return ops( list_with( 
      makePair( charP('+'), F2(fcpp::plus)  ), 
      makePair( charP('-'), F2(fcpp::minus) )   ) );
}
AddOp addOp = xaddOp();

//////////////////////////////////////////////////////////
// This comes from the cdc_multi example
// Helper: non-blocking "delay" alternative.
boolean delay_without_delaying(unsigned long time) {
  // return false if we're still "delaying", true if time ms has passed.
  // this should look a lot like "blink without delay"
  static unsigned long previousmillis = 0;
  unsigned long currentmillis = millis();
  if (currentmillis - previousmillis >= time) {
    previousmillis = currentmillis;
    return true;
  }
  return false;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 10000));
  t2 = millis() - t1;
  Serial.print("Waited for ");
  Serial.print(t2);
  Serial.println(" millis");
  while (!delay_without_delaying(5000) ) { };
  Serial << endl << endl << " Arduino RP2040 FC++ " << __FILE__ << endl;
  Serial << "Some simple FC++ operations" << endl;
  Serial << "plus(1,2) = " << plus(1,2) << endl;
  Serial << "plus(1.5,2.3) = " << plus(1.5,2.3) << endl;
  Serial << "plus(1)(2) = " << plus(1)(2) << endl;
  Serial << "inc(1) = " << inc(1) << endl;
  Serial << "--------------------------" << endl;

  Maybe<int> mx(2), my(3);
  Maybe<int> mz;
  if (mz.is_nothing()) Serial << "mz is nothing" << endl;
  mz = just(plus(mx.value(),my.value()));
  if (mz.is_nothing()) { 
     Serial << "mz is nothing" << endl;
  } else {
     Serial << "mz has " << mz.value() << endl;
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
  while (!delay_without_delaying(5000) ) { };

}

void loop() {
  // put your main code here, to run repeatedly:

}
