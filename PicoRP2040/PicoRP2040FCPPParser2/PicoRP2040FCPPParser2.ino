//////////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Parser 2.
// This is for developments from the previous file based on the old code.
// That has some problems. I am going to leave it there for comparison.
// I have now moved most of the Parser monad code into namespace fcpp
// following the FC++ standard and using an inner namespace impl for the implementations.
// I have now tested this and it runs unchanged.
// I am changing most printf to print or println to avoid the need to specify output formats.
//////////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Parser example from old parser.cpp
// Changing from Arduino RP2040 FCPP Parser to get the output sorted out.
//////////////////////////////////////////////////////////////////////
// I think this is a significant comment from the original file.
// The point is that the origin of this code is older than the Monad implementation.
/* This is from the client_index file.

parser              Monadic parser combinators.  A bit of a mess, but
                       they work.  This program inspired Brian to implement
                       monads/lambda, but he never went back to clean up
                       the program much after lambda/monads were implemented.
*/
//
//////////////////////////////////////////////////////////////////////
// What I am finding is that there is confusion in the implementation between the ParserM monad
// and a Parser defined using sat (predicate function).
// I want to transfer the Parser to many which does not work, as it wants a ParserM.
// This distinction does not I think exist in Haskell.
// I am exploring some ways around this problem.
//////////////////////////////////////////////////////////////////////
// Comment from the old version:
// This code is going to be kept as close as possible to parser.cpp
// I have now finished porting the monad code and functions.
// I still need to do the example code.
// The main puzzle is why I have to replace IntP xintp() with  auto xintp()
// as some typedefs are not recognised although others are.
// There are about 6 examples which need the change.
// Most cases are now solved with
// using "X = type_expression;" instead of "typedef type_expression X;"
// There is still one case which is a problem.
// New ideas will be in a new version.
//////////////////////////////////////////////////////////////////////
// The example parser deals correctly with expressions like this
// "123+45-6"
// which have + and - and multi digit integers.
// It does NOT deal correctly with * and () and does not discard spaces.
//////////////////////////////////////////////////////////////////////

#include <string>
#define FCPP152
#define FCPP_ENABLE_LAMBDA
#include "fcpp_prelude.h"

//#include <Streaming.h>

using namespace fcpp;


/// I have renamed String to StringL to avoid a name clash with the Arduino use of String.
typedef List<char> StringL;

namespace fcpp {
/// Parser monad which is based on the work of Hutton and Meijer.
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

   struct XBind { // The use of  ReRep here is different from other Monad instances.
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

// item is specific to parsing char. The ParserM code does not enforce this.
namespace impl {
struct XItem : public CFunType<StringL,OddList<std::pair<char,StringL> > > {
   OddList<std::pair<char,StringL> > operator()( const StringL& s ) const {
      if( null(s) )
         return NIL;
      else
         return cons( makePair( head(s), tail(s) ), NIL );
   }
};

}
typedef Full1<impl::XItem> Item;
Item item;

namespace impl {
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

}
typedef Full3<impl::XPlusP> PlusP;
PlusP plusP;

namespace impl {
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
}
// Just using parser version here:    Parser a -> Parser [a]
typedef Full1<impl::XManyM<ParserM> > Many;
Many many;

/* This is an idea to have a version of many which takes a parser intstead of ParserM. 
   It is not working at present. */
/*
namespace impl {
struct XManyP {
   // Parser a -> Parser [a]
   template <class P>
   struct Sig : public FunType<P,OddList<std::pair<char,StringL> >
   {};
   template <class P>
   typename Sig<P>::ResultType operator()( const P& p ) const {

   }
};
}
*/

namespace impl {
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
}
typedef Full1<impl::XSat> Sat;
Sat sat;

namespace impl {
struct XCharP : public CFunType<char, 
   RT<Sat,RT<Equal,char>::ResultType>::ResultType> {
   RT<Sat,RT<Equal,char>::ResultType>::ResultType
   operator()( char c ) const {
      return sat( equal(c) );
   }
};
}
typedef Full1<impl::XCharP> CharP;
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

namespace impl {
struct XBetween2 {
   template <class T, class U, class V> struct Sig
      : public FunType<T,T,T,bool> {};
   template <class T>
   bool operator()( const T& goal, const T& lower, const T& upper ) const {
      return lessEqual(goal,upper) && greaterEqual(goal,lower);
   }
};
}
typedef Full3<impl::XBetween2> Between2;
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

/////////////////////////////////////////////////////
// New parsers
auto space = charP(' ');
/////////////////////////////////////////////////////

namespace impl {

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
}
// Just using parser version here:    Parser a -> Parser [a]
typedef Full1<impl::XMany1M<ParserM> > Many1;
Many1 many1;


namespace impl {
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
}
typedef Full2<impl::XChainl1> Chainl1;
Chainl1 chainl1;

namespace impl {
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
}
typedef Full2<impl::XChainr1> Chainr1;
Chainr1 chainr1;

namespace impl {
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
}
typedef Full3<impl::XChainl> Chainl;
Chainl chainl;

namespace impl {
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
}
typedef Full3<impl::XChainr> Chainr;
Chainr chainr;


// This is not in the paper. I think it is constructing a number from digits.
typedef RT<LEType<LAM<LET<BIND<1,LAM<LV<2>,LV<3>,CALL<Plus,
   CALL<Multiplies,int,LV<2> >,LV<3> > > >,CALL<Chainl1,COMP<ParserM,
   CALL<Construct1<int>::Type,CALL<Minus,LV<4>,char> >,GETS<4,Digit> >,
   CALL<UnitM<ParserM>::Type,LV<1> > > > > >::Type>::ResultType Nat;
namespace impl {
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
}
Nat nat = impl::xnat();

/* This will not compile: 'IntP' does not name a type
 *  There must be something wrong with the typedef although it does compile.
 *  Experiment shows that it does not work as a type for  IntP xintp() {  }
 *  and it does work as a type for IntP intP = xintp();
 * I do not have an explanation for this.
 */

using IntP =  RT<LEType<LAM<LET<BIND<3,CALL<PlusP,COMP<ParserM,CALL<
   Construct1<Fun1<int,int> >::Type,fcpp::Negate>,CALL<CharP,char> >,CALL<
   UnitM<ParserM>::Type,CALL<Construct1<Fun1<int,int> >::Type,fcpp::Id> > > >, 
   COMP<ParserM,CALL<LV<1>,LV<2> >,GETS<1,LV<3> >,GETS<2,Nat> 
   > > > >::Type>::ResultType;
/*
typedef RT<LEType<LAM<LET<BIND<3,CALL<PlusP,COMP<ParserM,CALL<
   Construct1<Fun1<int,int> >::Type,fcpp::Negate>,CALL<CharP,char> >,CALL<
   UnitM<ParserM>::Type,CALL<Construct1<Fun1<int,int> >::Type,fcpp::Id> > > >, 
   COMP<ParserM,CALL<LV<1>,LV<2> >,GETS<1,LV<3> >,GETS<2,Nat> 
   > > > >::Type>::ResultType IntP;
*/
 /* attempt at simplification which made no difference.
typedef Construct1<Fun1<int,int> >::Type Con1;
typedef UnitM<ParserM>::Type Unit1;
typedef RT<LEType<LAM<LET<BIND<3,CALL<PlusP,COMP<ParserM,CALL<
   Con1,fcpp::Negate>,CALL<CharP,char> >,CALL<
   Unit1,CALL<Con1,fcpp::Id> > > >, 
   COMP<ParserM,CALL<LV<1>,LV<2> >,GETS<1,LV<3> >,GETS<2,Nat> 
   > > > >::Type>::ResultType IntP;
*/
namespace impl {
// I have no idea why using auto makes this work when the line IntP intP = xintp();
// The code was written for C++ and is now being compiled with C++17.
IntP xintp() {
//auto xintp() {
   LambdaVar<1> F;
   LambdaVar<2> N;
   LambdaVar<3> O;  // O was OP
   Construct1<Fun1<int,int> >::Type cf = construct1<Fun1<int,int> >();
   return lambda()[ let[ O == compM<ParserM>()[ cf[fcpp::negate] | charP['-'] ]
                               %plusP% unitM<ParserM>()[ cf[id] ] ] 
      .in[ compM<ParserM>()[ F[N] | F<=O, N<=nat ] ]   ]();
}
}
IntP intP = impl::xintp();

namespace impl {
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
}
typedef Full2<impl::XSepBy1> SepBy1;
SepBy1 sepBy1;

namespace impl {
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
}
typedef Full3<impl::XBracket> Bracket;
Bracket bracket;

namespace impl {
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
}
typedef Full2<impl::XSepBy> SepBy;
SepBy sepBy;

namespace impl {
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
}
typedef Full1<impl::XOps> Ops;
Ops ops;


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
//using ExprP = typename ParserM::Rep<int>::Type;
extern ExprP exprP;

using AddOp =  RT<Ops,List<std::pair<RT<CharP,char>::ResultType,Fun2<int,int,int> 
   > > >::ResultType;
//typedef RT<Ops,List<std::pair<RT<CharP,char>::ResultType,Fun2<int,int,int> 
//   > > >::ResultType AddOp;
namespace impl {
AddOp xaddOp() { //we have another one.
//auto xaddOp() {
   typedef Fun2<int,int,int> F2; // was F which clashes
   return ops( list_with( 
      makePair( charP('+'), F2(fcpp::plus)  ), 
      makePair( charP('-'), F2(fcpp::minus) )   ) );
}
}
AddOp addOp = impl::xaddOp();

using ExpOp = AddOp ;
//typedef AddOp ExpOp;
namespace impl {
ExpOp xexpOp() {
//auto xexpOp() {
   typedef Fun2<int,int,int> F2;
   return ops( list_with( makePair(charP('^'),F2(ptr_to_fun(&my_pow))) ) );
}
}
ExpOp expOp = impl::xexpOp();


//typedef RT<PlusP,Nat,RT<Bracket,RT<CharP,char>::ResultType,
//   ExprP,RT<CharP,char>::ResultType>::ResultType>::ResultType Factor;
using Factor = RT<PlusP,Nat,RT<Bracket,RT<CharP,char>::ResultType,
   ExprP,RT<CharP,char>::ResultType>::ResultType>::ResultType;
//namespace impl {
Factor xfactor() {
//auto xfactor() {
   static Factor result = nat ^plusP^ bracket( charP('('), exprP, charP(')') );
   return result;
}

using Term = ExprP;
//typedef ExprP Term;  // I am too lazy to direct-type this
Term xterm() {
//auto xterm() {
   static Term result = thunkFuncToFunc(ptr_to_fun(&xfactor)) ^chainr1^ expOp;
   return result;
}

//ExprP xexprP() {
//Term xexprP() { // This does not work here.
auto xexprP() {
   return thunkFuncToFunc(ptr_to_fun(&xterm)) ^chainl1^ addOp;
}
ExprP exprP = xexprP();
}

//////////////////////////////////////////////////////////////////////
// Here I just want to show the straightforward way using indirect
// functoid types:

typedef ParserM::Rep<int>::Type P_int;
P_int dummy = ignore(const_(cons(makePair(0,StringL()),NIL)));
P_int group=dummy, factor=dummy, term=dummy, expression=dummy;

//////////////////////////////////////////////////////////
void outStringL (const StringL& s)
{
   StringL::iterator si;
   for (si = s.begin(); si != s.end(); ++si) {
      //Serial.printf("%c",*si);
      Serial.print(*si);
   }
}


void parser_example()
{
   LambdaVar<91> S;
   factor     = lambda(S)[ (nat %plusP% dereference[&group])[S] ];
   term       = factor ^chainr1^ expOp;
   expression = term ^chainl1^ addOp;
   group      = bracket( charP('('), expression, charP(')') );
//////////////////////////////////////////////////////////////////////
   // This example does not deal with spaces.
   Serial.println("Test of ParserM monad");
   // Strings with + and - and no spaces are processed correctly.
   std::string ss("123+45-6");
   //Serial << "string is " << ss << endl;
   StringL s( ss.begin(), ss.end() );
   StringL::iterator si;
   Serial.print("Input string is ");
   outStringL(s);
   //for (si = s.begin(); si != s.end(); ++si) {
   //   Serial.printf("%c",*si);
   //}
   Serial.println(" ");
   int l = length(s); // force evaluation
   Serial.print("Length of s is ");
   Serial.println(l);

   typedef ParserM P;
   LambdaVar<1> X;
   List<std::pair<char,StringL> > lpcs = lambda()[ compM<P>()
      [ X | X <= item ] ]()(s);
   int lp = length(lpcs);
   Serial.printf("Length of lpcs is %d \n",lp);
   List<std::pair<char,StringL> >::iterator lpi;
   for (lpi = lpcs.begin(); lpi != lpcs.end(); ++lpi) {
      Serial.print((*lpi).first);
      Serial.print(" ");
      outStringL((*lpi).second);
      Serial.printf("\n");      
   }
   auto exprp2 = exprP(s);
   int lrp = length(exprp2);
   Serial.printf("Length of exprP(s) is %d \n",lrp);
   List<std::pair<int,StringL> >::iterator lpisi;
   for (lpisi = exprp2.begin(); lpisi != exprp2.end(); ++lpisi) {
      Serial.print((*lpisi).first);
      Serial.print(" ");
      outStringL((*lpisi).second);
      Serial.printf("\n");      
   }
   auto expr = expression(s);
   int lr = length(expr);
   Serial.printf("Length of expression(s) is %d \n",lr);
   for (lpisi = expr.begin(); lpisi != expr.end(); ++lpisi) {
      Serial.print((*lpisi).first);
      Serial.print(" ");
      outStringL((*lpisi).second);
      Serial.printf("\n");      
   }
   //Serial.printf("expr.head().first %c \n",(expr.head()).first);
   List<std::pair<int,StringL> > lpis;
   //List<std::pair<int,StringL> >::iterator lpisi;
   lpis = nat(s);
   int ln = length(lpis);
   Serial.printf("Length of nat(s) is %d \n",ln);
   //Serial.printf("lpis.head().first %d \n",(lpis.head()).first);
   for (lpisi = lpis.begin(); lpisi != lpis.end(); ++lpisi) {
      Serial.print((*lpisi).first);
      Serial.print(" ");
      outStringL((*lpisi).second);
      Serial.printf("\n");      
   }
   //Serial.printf("\n");
   Serial.println("=============================");
   Serial.println("Test of some of the functions");
   //if (digit('1')()) Serial.println("1 is a digit");
   if (between2('1','0','9')) Serial.println("1 is a digit"); // This works.
   if (between2(_,'0','9')('1')) Serial.println("1 is a digit"); // This works.
   auto what = item(s);
   Serial.print(what.head().first);
   Serial.print(" ");
   outStringL(what.head().second);
   Serial.printf("\n");      
   if (between2(_,'0','9')(what.head().first)) Serial.println("1 is a digit"); // This works.
   // Sat returns a parser which can be applied to the string.
   auto what2 = sat(between2(_,'0','9'));
   auto what3 = what2(s);
   if (length(what3) > 0 ) { Serial.print(what3.head().first); Serial.println(" is a digit"); }
   auto what4 = digit(s);
   if (length(what4) > 0 ) { Serial.print(what4.head().first); Serial.println(" is a digit"); }
   //auto what5 = many(digit);
   //auto what6 = many(sat(between2(_,'0','9')));
   //auto what7 = many(dummy); 
   ParserM pm;
   //auto what8 = many(p);
   auto what9 = plusP(digit,digit)(s);
   if (length(what9) > 0 ) { Serial.print(what9.head().first); Serial.println(" is a digit"); }
   auto what10 = space(s);
   if (length(what10) == 0 ) { Serial.println("what10: There is no space"); }
   //auto spaceM = liftM<ParserM>()(space);
   // This works though I am not sure what it means in practice
   auto spaceP = unitM<ParserM>()(space);
   auto what11 = spaceP(s);
   if (length(what11) == 0 ) { Serial.println("what11: There is no space"); }
   else { Serial.print(length(what11)); Serial.println(" is length of what11"); 
   Serial.print("What is found is "); Serial.println(what11.head().second); }
   auto what12 = what11.head().first(s); // This is where there is an empty list.
   if (length(what12) == 0 ) { Serial.println("what12: There is no space"); }
   auto what13 = spaceP(s).head().first(s);
   if (length(what13) == 0 ) { Serial.println("what13: There is no space"); }
   else { Serial.print(length(what13)); Serial.println(" is length of what13"); }
   //auto what14 = many(spaceP(s));
   //List<std::pair<StringL,StringL> > lpss;
   //lpss = many(letter)(s);
   //List<std::pair<List<int>,String> > lplis;
   //lplis = bracket( charP('['), intP ^sepBy^ charP(','), charP(']') )(s);

}

//////////////////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  while (!Serial) { }
  //::delay(5000);
  Serial.printf("Pico RP2040 FC++ operations\n");
  parser_example();
  //int x = 1;
  // These work as alternatives to printf.
  //Serial.print("x = ");
  //Serial.print(x);
  //Serial.println(" end of line");
}

void loop() {
  // put your main code here, to run repeatedly:

}
