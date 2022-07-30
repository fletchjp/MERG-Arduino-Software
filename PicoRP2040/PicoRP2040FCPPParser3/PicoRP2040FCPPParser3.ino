//////////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Parser3
// This one is for new ideas about Parser.
// I am going to have two versions.
// Parser3 will have Parser and ParserM   with std::pair<A,StringL>
// Parser4 will have ParserL and ParserLM with std::pair<List<A>,StringL>
// This is because I am not clear which one works best.
//////////////////////////////////////////////////////////////////////
// Thinking.  In HASKELL the monad and an instance have the same name.
// In FC++ the two are distinct e.g. MaybeM and Maybe which is an instance.
// I think that ParserM was written before the monad ideas crystallised, so there is no Parser object.
// I intend to explore that idea here.
//////////////////////////////////////////////////////////////////////
// Mysteriously, this gave no output until I put in prove_a_point()
// and the code needed for it.
// I can now add the previous examples and get output.
// The main thing I want to do is to gain confidence with use in the Pico.
// There have been some strange issues with typedefs needing to become using
// for some things to work.
//////////////////////////////////////////////////////////////////////
// was DUEFCPP and Arduino RP2040 FC++
// Demo of FC++ Maybe operation
// This will work on ARDUINO DUE but not on AVR boards.

#include <string>
#define FCPP152
#define FCPP_ENABLE_LAMBDA
#include "fcpp_prelude.h"

#include <Streaming.h>

using namespace fcpp;

/// I have renamed String to StringL to avoid a name clash with the Arduino use of String.
/// This is used with the Parser monad and other codes.
typedef List<char> StringL;

namespace fcpp {

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

/// Parser object - experimental adapting Maybe<T>
/// I am calling it Parser although it will have a value of the type and a string.
/// This is a start. I think it also needs a constructor from a string and something to parse the string.
/// Also something to run a parser over the string.
template <class A>
class Parser
{
   OddList<std::pair<A,StringL> > rep;
public:
   typedef A ElementType;
   typedef std::pair<A,StringL> ParserType;
   Parser() { }
   // Constructor with no result yet.
   // Not feasible without assumption of a default value for A.
   /*
   Parser(const StringL &s)
   {  ParserType pt = std::make_pair(NIL,s); 
      rep = cons(pt,NIL); }
   */
   Parser(const A &a, const StringL &s) //: rep (cons (std::make_pair(cons(a,NIL),s),NIL) )
   {  ParserType pt = std::make_pair(a,s); 
      rep = cons(pt,NIL); }
   // Make a Parser object by applying a function to parse the head of the string.
   template <typename P> 
   Parser(const P &p, const StringL &s) : rep (cons (std::make_pair(cons(p(s),NIL),s.tail()),NIL) )
   {   }
   // Make a new ParserL object by applying p to the string in an existing Parser and adding the output. 
   template <typename P> 
   Parser(const P &p, const Parser &par) {
     StringL s = par.rep.head().second;
     ParserType pt = std::make_pair(p(s),s.tail());
     rep = cons (pt,NIL);
   }
   bool is_nothing() const { return null(rep); }
   bool no_result() const { return null(rep.head().first); }
   bool no_string() const { return null(rep.head().second); }
   // Do not use these if is_nothing() returns true.
   A value() const { return head(rep).first; }
   //List<A> value_list() const { return head(rep).first; }
   StringL stringL() { return head(rep).second; }
};

/// Parser monad which is based on the work of Hutton and Meijer.
/// I have the paper.
/// This is a translation of the Haskell in the paper into FC++
/// It is useful to compare the two.
/// The paper does not mention Unit and Bind.
////////////////////////////////////////////////////////////////////////////////
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
}

//////////////////////////////////////////////////////////

void outStringL (const StringL& s)
{
   StringL::iterator si;
   for (si = s.begin(); si != s.end(); ++si) {
      //Serial.printf("%c",*si);
      Serial.print(*si);
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

}

void new_ideas()
{
   Serial.println("Some operations with ParserL<A>");
   Parser<int> pint;
   std::string ss("123+45-6");
   //Serial << "string is " << ss << endl;
   StringL s( ss.begin(), ss.end() );
   StringL::iterator si;
   Serial.print("Input string is ");
   outStringL(s); Serial.println(" ");
   //Parser<int> pints(s); not implemented
   Parser<int> pintex(1,s);
   if (pint.is_nothing()) { Serial.println("pint has no data"); }
   //if (pints.no_result()) { Serial.println("pints has no result"); }
   Serial.print("pintex has the value "); Serial.println(pintex.value());
}

void prove_a_point() {
/*
This works.  I did it just to prove a point.
*/
   typedef ParserM P;
   LambdaVar<1> X;
   std::string ss("abcABC");
   //Serial << "string is " << ss << endl;
   StringL s( ss.begin(), ss.end() );
   List<std::pair<char,StringL> > lpcs;
   List<std::pair<StringL,StringL> > lpss;
   LambdaVar<12> lower;
   LambdaVar<13> upper;
   LambdaVar<14> letter;
   LambdaVar<15> words;
   lpcs = lambda()[ let[
      lower == compM<P>()[ X | X<=item, guard[logicalAnd[greaterEqual[X,'a'],
                                                         lessEqual[X,'z']]]],
      upper == compM<P>()[ X | X<=item, guard[logicalAnd[greaterEqual[X,'A'],
                                                         lessEqual[X,'Z']]]],
      letter == lower %plusP% upper
   ].in[ letter[s] ] ]();
   //   , words == many[letter] 
   //  ].in[ words[s] ] ]();
//   cout << lpss << endl;
     int ln = length(lpcs);
     Serial.printf("Length of lpcs is %d \n",ln);

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  while (!Serial) { }
  //::delay(5000);
  Serial.printf("Pico RP2040 FC++ operations\n");
  //Serial << "Test streaming output" << endl;
  fcpp_examples();
  Serial.println("after fcpp_examples");
  prove_a_point();
  Serial.println("--------------------------");
  new_ideas();
  Serial.println("--------------------------");
  //Serial.flush();
}

int i = 0;
 
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(i);
  //i++;
}
