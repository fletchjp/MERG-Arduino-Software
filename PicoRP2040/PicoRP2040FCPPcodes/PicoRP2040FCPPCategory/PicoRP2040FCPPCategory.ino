//////////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Category
// New example to look at code from category_fcpp.cpp
// I have added a lot of the code and I have started to look at the examples.
// This example is also being used for the development of the monoid code
// which have now been moved to PicoRP2040FCPPMonoids. 
// I now have an example working applying two different operators to paired data.
// I have built bimap2 and parallel2 to operate on pairs of two argument functoids.
// As I invent new FC++ functoids I am moving them to be in the most relevant location.
// I have managed to make e.g. mmappend(inc,inc) by making a specialization for it.
// I have now also extended that to monoporphic versions.
// I think that is a clue to me that there are things which are Monoid implementations
// in FC++ which I have not recognised as such as I did not understand Monoid. 
//////////////////////////////////////////////////////////////////////
// I think that the above work on monoids will help me sort out the next section too.
// See https://wiki.haskell.org/Typeclassopedia for some explanation.
// The first result of this is the Monoid Mendo for endmorphic transformations.
// I am not sure that this has any practical use I have the FC++ compose (aka dot)
// which does it without the structure.
// It does help me with understanding.
// I have attempted to extend Mendo to monomorphic Fun1<T,T> items.
// This has now been taken out of use as compose(f,g) crashes for such items.
// Cases just do not terminate. This is an FC++ issue and not in the monoid code.
// The whole of the next section was written before I understood monoids.
//////////////////////////////////////////////////////////////////////
// File for developments following Bartosz Milewski's work
// "Category Theory for Programmers" and related material
//
// I am looking at his examples in Section 4 "Kleisli Categories".
// His examples use auto so I am going to implement this as C++14
// to get the extensions I need.
//
// Note that on the RP2040 I have gcc 10 and C++17 which should be alright for this code.

// In this version I am going to use FC++.

// The experiments on the Writer have shown up a link with the
// bifunctor operator bimap which has examples in unify.cpp
// The examples below show that FC++ functoids can be lifted to Writer
// without any special code using either bimap or parallel.

// To do Bartosz example I have to implement toWords and toUpper as FC++
// functoids which do not store any message text.

// I have implemented a functoid trail(a) equivalent to plus(_,a)
// for the purpose of appending to a string or Message.
// Message is new structure wrapping a std::string.

// The implementation using bimap or parallel is polymorphic and can
// support other operations on the Message and even other types.
// One thing which can be done is to reset the message string with
// konst(Message).

// I have now put this into a ParallelM monad which works on pairs
// of data and pairs of functions.
// I have now worked out how to make it inferrable and implemented it.
// I could add first and second operators to it.

// I intended to do a WriterM monad which still needs some thought.
// It would work with the same pairs of data but take a function object
// which did both operations, something like writew().

// There is code for this in "Learn you a Haskell.."

// I have added some code as a result of more reading in
// "Learn you a Haskell.." relating to the Reader monad which contains
// functions. This is not directly implementable, but FC++ compose2 does
// something similar and FC++ lambda allows this to be generalised.

// The C++11 'auto' makes it much easier to use the FC++ lambda as the
// type of the lambda can be deduced and does not impinge on the user at all.

// I have now moved the monad ParallelM to patterns.h to make it more
// widely available. There may be other code which can be harvested too.

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
//#define FCPP_ARDUINO // Take out arguement traits - defined in prelude for Arduino etc
// All headers modified to work in the Arduino/Pico environment.
// Some exceptions guarding in pattern.h have had to be removed.
#define FCPP_UNIFY // Unification of Monads and Functors in functors.h
#include "fcpp_prelude.h"
#include "fcpp/functors.h"
#include "fcpp/comonad.h"
#include "fcpp/arrow.h"
#include "fcpp/monoids.h"


#include <Streaming.h>

using namespace fcpp;

/// I have renamed String to StringL to avoid a name clash with the Arduino use of String.
/// This is used with the Parser monad and other codes.
typedef List<char> StringL;

namespace fcpp {

// New XOR operator for FC++ it has been moved to fcpp/prelude.h

// New Bimap2 operator for FC++ it has been moved to fcpp/functors.h

// New Parallel2 operator for FC++ it has been moved to fcpp/pattern.h
// This extends parallel to work on two argument functions - parallel2(makePair(f,g),p1,p2).
// This returns std::make_pair(f(p1.first,p2.first),g(p1.second,p2.second)).
// Note that makePair is an FC++ operator.

//////////////////////////////////////////////////////////////////////////
/// Output operators for the Monoid types.
/////////////////////////////////////////////////////////////////////////

Print &operator <<( Print &obj, const std::string &arg)
{
    Serial << arg.c_str();
    return obj; 
}

Print &operator <<( Print &obj, Mstring &arg)
{
    Serial << arg.get_string();
    return obj; 
}

template <class T>
Print &operator << ( Print &obj, const List<T> &arg)
{
    Serial << "[ ";
    for (auto i = arg.begin(); i != arg.end(); ++i)
    {
        Serial << *i << " ";
    }
    Serial << "]";
    return obj; 
}

template <>
Print &operator << ( Print &obj, const List<int> &arg)
{
    Serial << "[ ";
    for (auto i = arg.begin(); i != arg.end(); ++i)
    {
        Serial << *i << " ";
    }
    Serial << "]";
    return obj; 
}

template <>
Print &operator << ( Print &obj, const List<Mstring> &arg)
{
    Serial << "[ ";
    for (auto i = arg.begin(); i != arg.end(); ++i)
    {
        Serial << (*i).get_string() << " ";
    }
    Serial << "]";
    return obj; 
}


template <class T>
Print &operator << ( Print &obj, const Mlist<T> &arg)
{
    Serial << "[ ";
    for (auto i = arg.begin(); i != arg.end(); ++i)
    {
        Serial << *i << " ";
    }
    Serial << "]";
    return obj; 
}

template <class T,class Op>
Print &operator << ( Print &obj, const MonoidType<T,Op> &arg)
{
    Serial << arg.value;
    return obj; 
}

////////////////////////////////////////////////////////////////////////
/// I am going to look at this in the light of the comments
/// in Typeclassopedia. I am going to look at the Writer monad as well.
////////////////////////////////////////////////////////////////////////
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
/*
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

/////////////////////////////////////////////////////////////
//
// http://bartoszmilewski.com/2014/12/23/kleisli-categories/
//
/////////////////////////////////////////////////////////////

std::string toUpper_a(std::string s) {
  std::string result;
    int (*toupperp)(int) = &toupper; // toupper is overloaded
    std::transform(begin(s), end(s), back_inserter(result), toupperp);
    return result;
}

std::vector<std::string> words(std::string s) {
  std::vector<std::string> result{""};
    for (auto i = begin(s); i != end(s); ++i)
    {
        if (isspace(*i))
            result.push_back("");
        else
            result.back() += *i;
    }
    return result;
}

std::vector<std::string> toWords_a(std::string s) {
    return words(s);
}

/////////////////////////////////
// The Writer Category in FC++ //
/////////////////////////////////

// Space here for new functoids!!
namespace fcpp {

////////////////////////////////////////////////////////
// These implement toUpper and toWord as FC++ functoids.
////////////////////////////////////////////////////////
  namespace impl {
   struct XToUpper {
     template <class A> struct Sig :
       public FunType<A,A> {};
   
     template <class A>
     typename Sig<A>::ResultType
     operator()(const A& a) const {
       return toUpper_a(a);
     }
   };

   struct XToWords {
     template <class A> struct Sig :
       public FunType<A,std::vector<A> > {};
   
     template <class A>
     typename Sig<A>::ResultType
     operator()(const A& a) const {
       return toWords_a(a);
     }
   };

  }
typedef Full1<impl::XToUpper> ToUpper;
typedef Full1<impl::XToWords> ToWords;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN ToUpper toUpper;
FCPP_MAYBE_EXTERN ToWords toWords;
FCPP_MAYBE_NAMESPACE_CLOSE

  namespace impl {
    // Trail adds at the end, which is useful for std::string.
    // It returns a one parameter functoid.
   struct XTrail {
  #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Trail");
   }
 #endif
     // This is how to tell it the A is the second argument for Plus.
    template <class A> struct Sig :
       public FunType<A,typename RT<Plus,AutoCurryType,A>::ResultType > {};
   
     template <class A>
     typename Sig<A>::ResultType
     operator()(const A& a) const {
       return plus(_,a);
     }
   };
}
typedef Full1<impl::XTrail> Trail;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Trail trail;
 FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XTrail>
       {
       public:
  static const bool has_name = true;
  static std::string name(const impl::XTrail &f) { return f.name(); }
       };
    }
 #endif

////////////////////////////////////////////////////////
// I am going to review this in the light of the things
// in Typeclassopedia.
////////////////////////////////////////////////////////
// General structure for Writer.
// Message follows the EitherError structure for Either.
// Writer holds a one arg functoid and a message.
//
// Writer<FType> wtype(functoid,message)
//
// There are three ways to use this.
//
// write_(functoid,argument,message)
// writep(functoid,make_pair(argument,message))
// writew(writer,make_pair(argument,message))
//
// All these return std::pair(output,message)
//
// The third sort accumulates the messages.
// It can be composed e.g.
//
// compose(writew(wtoWords),writew(wtoUpper))(make_pair(argument,message))
//
// where the first writew operation follows the second.
//
// Also available as 'dot'
// (writew(wtoWords) ^dot^ writew(wtoUpper))
//
// The opposite order is also available:
//
// invcompose(writew(wtoUpper),writew(wtoWords)) is also available.
//
// also available as 'dash' (my name for it).
// (writew(wtoUpper) ^dash^ writew(wtoWords))
//
// Things to do: test using the DEBUG name as the message.
/////////////////////////////////////////////////////////
// I have now found a link to the work in unify.cpp.
// The functoids bimap and parallel can be used to implement
// the functionality of writer without new code.
// e.g.
// Message mdec("dec ");
// bimap(dec,plus(_,mdec),std::pair(1,message))
//
// This applies 'dec' to '1' and adds mdec at the end of 'message'.
//
// This involves implementing the operator+ for classtype Message.
//
// parallel(makePair(dec,trail(mdec)),std::pair(1,message))
//
// This does the same using a new functoid trail(a) == plus(_,a)
//
// This way also gains the flexibility to do other operations on the message.
// e.g konst(mdec) will reset the message to 'mdec'.
//
// See examples in the file.
//
//////////////////////////////////////////////////////////

struct Message {
private:
  std::string message_;
public:
  Message() { message_ = std::string("nothing"); }
  Message(const Message &mm) : message_(mm.message_) {}
  Message(const std::string &m) : message_(m) {}
  std::string operator()() const { return message_; }
};

  Message operator+(const Message& m1,const Message& m2)
  {
    return Message(m1() + m2());
  }

#ifndef FCPP_ARDUINO
  namespace traits {

       template<>
   class argument_traits<Message>
       {
       public:
   //static const bool has_name = true;
   static std::string value(const Message &m)
   { return m();}
       };

  }
#endif

template <class F>
class Writer {
private:
  F f_;
  Message m_;
public:
  typedef F FType;
  template <class FF>
  struct Sig : public FunType<Writer<FF>,FF> {};
  Writer(const F& f) : f_(f) {}
  Writer(const F& f, const Message &m) : f_(f), m_(m) {}
  typename Sig<F>::ResultType
  operator()() const { return f_;}
  Message message() const { return m_; }
};

namespace impl {
   struct XWrite {
     template <class F, class A, class M> struct Sig :
       public FunType<F,A,M,
     std::pair<typename RT<F,A>::ResultType,M > >{};
   
     template <class F, class A, class M>
     typename Sig<F,A,M>::ResultType
     operator()(const F & f, const A& x, const M &m) const {
       return std::make_pair(f(x), m );
      }
   };


   struct XWriteP {
     template <class F, class P, class M> struct Sig :
       public FunType<F,P,M,
       std::pair<typename RT<F,typename P::first_type>::ResultType,
     typename P::second_type > >{};
   
     template <class F, class P, class M>
     typename Sig<F,P,M>::ResultType
     operator()(const F & f, const P &p, const M& m) const {
       //return std::make_pair(f(p.first), p.second() + m() );
       // Now using operator+ defined for Message
       // which means this would work for any time supporing plus.
       return std::make_pair(f(p.first), plus(p.second,m) );
      }
   };

   struct XWriteW {
     // The key to this is to get the correct function type
     // from W, which is now defined as typename W::FType.
     // This now works when the result type is not P::first_type.
     template <class W, class P> struct Sig :
       public FunType<W,P,
          std::pair<typename RT<
                       typename W::FType,
                       typename P::first_type>::ResultType,
           typename P::second_type > >{};
   
     template <class W, class P>
     typename Sig<W,P>::ResultType
     operator()(const W& w, const P &p) const {
       //return std::make_pair(w()(p.first), p.second()+w.message()() );
       // Now using operator+ defined for Message
       // which means this would work for any time supporing plus.
       return std::make_pair(w()(p.first), plus(p.second,w.message()) );
      }
   };

}

typedef Full3<impl::XWrite>  Write;
typedef Full3<impl::XWriteP> WriteP;
typedef Full2<impl::XWriteW> WriteW;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Write write_;
FCPP_MAYBE_EXTERN WriteP writep;
FCPP_MAYBE_EXTERN WriteW writew;
FCPP_MAYBE_NAMESPACE_CLOSE

// Not yet implemented....
struct WriterM {
   template <class A>  struct Rep   { typedef Writer<A> Type; };
   template <class EA> struct UnRep { typedef typename EA::ElementType Type; };

#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("WriterM");
   }
#endif
};

  //////////////////////////////////////////////
  // ParallelM monad moved to fcpp/pattern.h  //
  //////////////////////////////////////////////

}

void category_examples()
{
  Serial << "Without logging" << endl;
  Serial << "=======================" << endl;
  std::string s = ("What a load of rubbish");
  Serial << s << endl;
  std::string su = toUpper_a(s);
  Serial << su << endl;
  std::vector<std::string> vw1 = toWords_a(su);
  for (auto i = begin(vw1); i != end(vw1); ++i)
    {
        Serial << *i << endl;
    }
  Serial << "=======================" << endl;
  Serial << "Using FC++ functoids" << endl;
  Serial << "=======================" << endl;
  std::string su2 = toUpper(s);
  Serial << su2 << endl;
  std::vector<std::string> vw2 = toWords(su);
  for (auto i = begin(vw2); i != end(vw2); ++i)
    {
        Serial << *i << endl;
    }
  Serial << "==========================" << endl;
  Serial << "FC++ functoids with Writer" << endl;
  Serial << "==========================" << endl;
  Message mtoUpper ("toUpper ");
  Message mtoWords ("toWords ");
  typedef std::vector<std::string> vstring;
  typedef Fun1<std::string,vstring> Fun1sv;
  Fun1sv ftoWords(toWords);
  Writer<ToUpper> wtoUpper(toUpper,mtoUpper);
  Writer<ToWords> wtoWords(toWords,mtoWords);
  Writer<Fun1sv>  wftoWords(ftoWords,mtoWords);
  Message mnull(std::string(""));
  std::pair<std::string,Message> sm = std::make_pair(s,mnull);
  std::pair<std::string,Message> um = writew(wtoUpper)(sm);
  Serial << um.first << " : " << um.second() << endl;
  vstring vw3 = wftoWords()(su);
  Serial <<  "wftoWords : " << wftoWords.message()() << endl;
  for (auto i = begin(vw3); i != end(vw3); ++i)
    {
        Serial << *i << endl;
    }
  vstring vw4 = wtoWords()(su);
  Serial << "wtoWords  : " << wtoWords.message()() << endl;
  for (auto i = begin(vw4); i != end(vw4); ++i)
    {
        Serial << *i << endl;
    }
  std::pair<vstring,Message> wm = writew(wtoWords)(um);
  Serial << "writew(wtoWords)(um)  : " << wm.second() << endl;
  for (auto i = begin(wm.first); i != end(wm.first); ++i)
    {
        Serial << *i << endl;
    }
  std::pair<vstring,Message> wm2 =
       compose(writew(wtoWords),writew(wtoUpper))(sm);
  Serial << "compose(writew(wtoWords),writew(wtoUpper))(sm)  : "
            << wm2.second() << endl;
  for (auto i = begin(wm2.first); i != end(wm2.first); ++i)
    {
        Serial << *i << endl;
    }
  std::pair<vstring,Message> wm2a =
    (writew(wtoWords) ^dot^ writew(wtoUpper))(sm);
  std::pair<vstring,Message> wm3 =
       invcompose(writew(wtoUpper),writew(wtoWords))(sm);
  Serial << "invcompose(writew(wtoUpper),writew(wtoWords))(sm)  : "
            << wm3.second() << endl;
  for (auto i = begin(wm3.first); i != end(wm3.first); ++i)
    {
        Serial << *i << endl;
    }
  std::pair<vstring,Message> wm3a =
    (writew(wtoUpper) ^dash^ writew(wtoWords))(sm);
  Serial << "(writew(wtoUpper) ^dash^ writew(wtoWords))(sm  : "
            << wm3a.second() << endl;
  for (auto i = begin(wm3a.first); i != end(wm3a.first); ++i)
    {
        Serial << *i << endl;
    }
  Serial << "=========================" << endl;
  Serial << "Using the ParallelM monad" << endl;
  Serial << "=========================" << endl;
  std::pair<std::string,Message> monsm = unitM<ParallelM>()(makePair(s,mnull));
  Serial << "unitM<ParallelM>()(makePair(s,mnull)) : ";
  Serial << monsm.second() << " : " << monsm.first << endl;
  std::pair<std::string,Message> monsu =
    bindM<ParallelM>()(monsm,makePair(toUpper,trail(mtoUpper)));
  Serial << "bindM<ParallelM>()(monsm,makePair(toUpper,trail(mtoUpper))) : ";
  Serial << monsu.second() << " : " << monsu.first << endl;
  std::pair<vstring,Message> monsw = bindM<ParallelM>()(monsu,makePair(toWords,trail(mtoWords)));
  Serial << "bindM<ParallelM>()(monsu,makePair(toWords,trail(mtoWords))) : "
            << monsw.second() << endl;
  for (auto i = begin(monsw.first); i != end(monsw.first); ++i)
    {
        Serial << *i << endl;
    }
  Serial << "=======================" << endl;
  Serial << "Experiments with Writer" << endl;
  Serial << "=======================" << endl;
  
  Message mplus ("plus ");
  Message mplus2("plus 2 ");
  Message minc  ("inc ");
  typedef Fun1<int,int> Fun1ii;
  Fun1ii finc(inc), fdec(dec), fplus2(plus(2)); // Monomorphic versions
  // This does not work well with polymorphic functoids.
  Writer<Plus> wplus(plus,mplus);
  Writer<Inc>  winc (inc ,minc);
  Writer<Fun1ii> wfinc  (finc ,minc);
  Writer<Fun1ii> wfplus2(fplus2,mplus2); // monomorphic type.
  int wincres =  winc()(1);
  Serial << "Writer<Inc>  winc (inc ,minc);" << endl;
  Serial << "winc   : "
            <<wincres << " : " << winc.message()() << endl;
   // write_ takes (functoid,value,message)
  std::pair<int,Message> writeres = write_(inc,wincres,minc);
  Serial << "write_  : "
            << writeres.first << " : " << writeres.second() << endl;
  // writep takes (functoid,pair(value,messagea,messageb)
  // and concatenates the messages.
  std::pair<int,Message> plusres = writep(plus(2),writeres,mplus2);
  Serial << "writep  : "
            << plusres.first << " : " << plusres.second() << endl;
  std::pair<int,Message> plusres2 = writep(fplus2,plusres,mplus2);
  Serial << "writep  : "
            << plusres2.first << " : " << plusres2.second() << endl;
  std::pair<int,Message> plusres3 = writew(wfinc,plusres2);
  Serial << "writew  : "
            << plusres3.first << " : " << plusres3.second() << endl;
  std::pair<int,Message> plusres4 = compose(writew(wfinc),writew(wfinc))(plusres3);
  Serial << "compose : "
            << plusres4.first << " : " << plusres4.second() << endl;
  Serial << "===========================" << endl;
  Serial << "Some other ways to do this." << endl;
  Serial << "===========================" << endl;
  Message mone("one "), mtwo("two ");
  Message moneplustwo = mone + mtwo;
  Message mdec("dec ");
  std::pair<int,Message> decres = bimap(dec,plus(_,mdec),plusres4);
  Serial << "bimap(dec,plus(_,mdec),plusres4) : "
            << decres.first << " : " << decres.second() << endl;
  std::pair<int,Message> decres2 = parallel(makePair(dec,trail(mdec)),decres);
  Serial << "parallel(makePair(dec,trail(mdec)),decres) : "
            << decres2.first << " : " << decres2.second() << endl;
  std::pair<int,Message> decres3 = parallel(makePair(dec,konst(mdec)),decres);
  Serial << "parallel(makePair(dec,konst(mdec)),decres) : "
            << decres3.first << " : " << decres3.second() << endl;
  Serial << "===========================" << endl;
  Serial << "Exploration using monads." << endl;
  Serial << "===========================" << endl;
  std::pair<int,Message> monres = unitM<IdentityM>()(makePair(1,mone));
  Serial << "unitM<IdentityM>()(makePair(1,mone) : "
            << monres.first << " : " << monres.second() << endl;
  std::pair<int,Message> monres2 =
    bindM<IdentityM>()(monres,parallel(makePair(inc,trail(minc))));
  Serial << "bindM<IdentityM>()(monres,parallel(makePair(inc,trail(minc)))) : "
            << monres2.first << " : " << monres2.second() << endl;
  std::pair<int,Message> monres3 =
    bindM<ParallelM>()(monres,makePair(inc,trail(minc)));
  Serial << "bindM<ParallelM>()(monres,makePair(inc,trail(minc))) : "
            << monres3.first << " : " << monres3.second() << endl;
  std::pair<int,Message> monres3a = unitM<ParallelM>()(makePair(1,mone));
  // Not solved at present.
  //std::pair<int,Message> monres3b = unit (makePair(1,mone));
  // This works anyway...
  std::pair<int,Message> monres3b = unit (ident(makePair(1,mone)));
  std::pair<int,Message> monres4 =
    bind (monres3,makePair(dec,trail(mdec)));
  Serial << "ParallelM monad is now inferrable." << endl;
  Serial << "bind (monres3,makePair(dec,trail(mdec))) : "
            << monres4.first << " : " << monres4.second() << endl;
  Serial << "===========================" << endl;
  Serial << "Now with the StateM monad." << endl;
  Serial << "===========================" << endl;
  Serial << "I am now understanding this as more complicated." << endl;
  Serial << "There can be more than one way of doing things." << endl;
  Serial << "This sets up a pair with the types of the first argument"
            << endl;
  Serial << "and the second argument which is also the type" << endl;
  Serial << "given to the StateM monad." << endl;
  Serial << "==============================================" << endl;
  std::pair<int,Message> monstate = unitM<StateM<Message> >()(1)(mone);
  Serial << "monstate = unitM<StateM<Message> >()(1)(mone) : "
            << monstate.first << " : " << monstate.second() << endl;
  typedef std::pair<int,Message> StateType;
  Serial << "The state type now needs to have both int and Message."
            << endl;
  Serial << "typedef std::pair<int,Message> StateType;" << endl;
  Serial << "This is not very useful as it does not change the State variable." << endl;
  std::pair<int,StateType> is1 = unitM<StateM<StateType> >()(2)(monstate);
  Serial << "unitM<StateM<StateType> >()(2)(monstate) : ";
  Serial << is1.first << " : ( " << is1.second.first
            << " : "  << is1.second.second() << " )" << endl;
  //bindM<StateM<StateType> >()(parallel(id,id),id)(monstate);
  std::pair<int,Message> monstate2 =
       bindM<StateM<Message> >()(id,makePair)(monstate);
  Serial << "monstate2 = bindM<StateM<Message> >()(id,makePair)(monstate) : "
            << monstate2.first << " : " << monstate2.second() << endl;
  Serial << "This works to apply an update and append a message."
            << endl;
  std::pair<int,Message> monstate3 =
    bindM<StateM<Message> >()(parallel(makePair(inc,trail(minc))),makePair)(monstate);
  Serial << "monstate3 = bindM<StateM<Message> >()(parallel(makePair(inc,trail(minc))),makePair)(monstate) : "
            << monstate3.first << " : " << monstate3.second() << endl;
  //std::pair<int,Message> monstate2 = bindM<StateM<Message> >()(monstate,id);
  // This is not doing what I need.
  std::pair<Message,Message> pm = StateM<Message>::fetch()(monstate.second);
  Serial << pm.first() << " : " << pm.second() << endl;
  Message tm = trail(minc)(monstate.second);
  std::pair<Empty,Message> em = StateM<Message>::assign(minc)(monstate.second);
  Serial << em.second() << endl;
  Serial << "---------" << endl;
  LambdaVar<1> X;
  LambdaVar<2> Y;
  std::pair<int,int> p = bindM<StateM<int> >()( lambda(X)[ makePair[3,X] ],
          lambda(Y)[ unitM<StateM<int> >()[ Y ] ])(0);
  Serial << p.first << "," << p.second << endl;
  std::pair<int,int> p2 = bindM<StateM<int> >()(id,makePair)(p);
  Serial << "bindM<StateM<int> >()(id,makePair)(p) : ";
  Serial << p2.first << "," << p2.second << endl;
  std::pair<int,int> p2a = bindM<StateM<int> >()
           (parallel(makePair(dec,inc)),makePair)(p);
  Serial << "bindM<StateM<int> >()(parallel(makePair(dec,inc)),makePair)(p); : ";
  Serial << p2a.first << "," << p2a.second << endl;
  std::pair<int,int> p3 = parallel(makePair(dec,inc))(p);
#ifdef FCPP_DEBUG
  Serial << sprint(bindM<StateM<int> >()(id,makePair)) << endl;
  Serial << sprint(bindM<StateM<int> >()(id,makePair)(p)) << endl;
#endif
  Serial << p3.first << "," << p3.second << endl;
  ////////////////////////////////////////////////////////
  // Reader Monad
  ////////////////////////////////////////////////////////
  Serial << "============================" << endl;
  Serial << "Now reader monad experiments." << endl;
  Serial << "============================" << endl;
  // Addition of polymorphic functoids not supported.
  auto a = multiplies(2);
  auto b = plus(10);
  auto c = duplicate(multiplies);
  int y = 10;
  Serial << "auto a = multiplies(2);" << endl;
  Serial << "auto b = plus(10);" << endl;
  //auto c = plus(a,b);
  Serial << "auto c = duplicate(multiplies);" << endl;
  Serial << "c(" << y << ") = " << c(y) << endl;
  // Addition of monmorphic functoids not supported.
  Serial << "Attempts to add the two operators as (a + b) fail."
            << endl;
  Fun1<int,int> fa = a;
  Fun1<int,int> fb = b;
  Fun1<int,int> fc = c;
  Serial << "The monomorphic versions can be put into a List." << endl;
  Serial << "Then the list can be used with star." << endl;
  Serial << "This yields a result list which can be summed." << endl;
  Serial << "This is not limited to two functions." << endl;
  //auto fc = plus(fa,fb);
  // This does work.
  // Put the monomorphic functoids into a list
  auto lfc = makeList3(fa,fb,fc);
  // Put the argument in a list of 1 element.
  int x = 1;
  auto lx = makeList1(x);
  // star them together
  auto res = lfc ^star^ lx;
  // sum the result.
  auto sumres = sum(res);
  Serial << "(lfc ^star^ lx) : ( " << head(res) << " "
            << head(tail(res)) << " "
            << head(tail(tail(res))) << " ) : " << sumres << endl;
  // These do work. This merits more attention
  auto resa = a(x) + b(x);
  auto fres = fa(x) + fb(x);
  LambdaVar<3> F;
  LambdaVar<4> G;
  LambdaVar<5> H;
  LambdaVar<6> J;
  auto fgplus = lambda(F,G,X)[ F[X] %plus% G[X] ];
  Serial << "====================================================="
            << endl;
  Serial << "A more general solution is to use an FC++ lambda." << endl;
  Serial << "The lambda defers the plus until the results are known"
            << endl;
  Serial << "auto fgplus = lambda(F,G,X)[ F[X] %plus% G[X] ];" << endl;
  Serial << "fgplus(a,b)(x)   : " << fgplus(a,b)(x) << endl;
  Serial << "A more general lambda allows the choice of two parameter operators."
            << endl;
  auto fgh = lambda(F,G,H,X)[ F [G[X]][H[X]] ];
  Serial << "auto fgh = lambda(F,G,H,X)[ F [G[X]][H[X]] ];" << endl;
  Serial << "fgh(plus,a,b)(x)       : " << fgh(plus,a,b)(x) << endl;
  Serial << "fgh(multiplies,a,b)(x) : " << fgh(multiplies,a,b)(x)
            << endl;
  auto fghj = lambda(F,G,H,J,X)[ F [ F [G[X]][H[X]] ] [J[X]] ];
  Serial << "This approach can be extended to three functions." << endl;
  Serial << "It gets messy because plus takes two arguments." << endl;
  Serial << "F is applied to two results and again to add the third."
            << endl;
  Serial << "auto fghj = lambda(F,G,H,J,X)[ F [ F [G[X]][H[X]] ] [J[X]] ];"
            << endl;
  Serial << "fghj(plus,a,b,c)(x)       : "
            << fghj(plus,a,b,c)(x) << endl;
  Serial << "fghj(multiplies,a,b,c)(x) : "
            << fghj(multiplies,a,b,c)(x) << endl;
  Serial << "====================================================="
            << endl;
  auto lfg = lambda(F,G)[makeList[F,G] ];
  // This will work only with monomorphic functoids.
  auto test = lfg(fa,fb) ^star^ lx;
  auto comp2 = compose2(plus,a,b)(1);
  // dot2 is an alternative name.
  auto comp2a = dot2(plus,a,b)(1);
  // This does not look very natural.
  // It has to be like this because of the currying operation.
  // What is needed is (a !plus! b) where ! is some other symbol
  // which can be overloaded.
  auto comp2b = (plus ^dot2^ a)(b)(1);
  // This works, wrapping the dot2 and plus together first.
  auto dot2plus = dot2(plus);
  auto dot2multiplies = dot2(multiplies);
  Serial << "====================================================="
            << endl;
  Serial << "FC++ compose2 (dot2) does the same as fgh." << endl;
  Serial << "dot2plus defines an operator to do a+b for functoids!"
            << endl;
  Serial << "dot2multiplies does a*b for functoids!"
            << endl;
  Serial << "auto dot2plus = dot2(plus);" << endl;
  Serial << "auto dot2multiplies = dot2(multiplies);" << endl;
  Serial << "This can also be done using a lambda" << endl;
  Serial << "auto fdot2 = lambda(G,F,H,X)[ F [G[X]][H[X]] ];" << endl;
  Serial << "The FC++ lambda gives the means to meet specific needs."
            << endl;
  Serial << "Use of auto makes it easy to store and pass lambda functions."
            << endl;
  Serial << "======================================================"
            << endl;
  auto comp2b1 = (a ^dot2plus^ b)(1);
  Serial << "(a ^dot2plus^ b)(1) : " << comp2b1 << endl;
  auto fdot2 = lambda(G,F,H,X)[ F [G[X]][H[X]] ];
  auto comp2c = (a ^fdot2^ plus)(b)(1);
  Serial << "(a ^fdot2^ plus)(b)(1) : " << comp2c << endl;
  auto comp2d = (a ^fdot2^ multiplies)(b)(1);
  auto comp2e = (a ^dot2multiplies^b)(1);
  auto comp3 = compose2(multiplies,a,b)(1);
  Serial << "(a ^fdot2^ multiplies)(b)(1) : " << comp2d << endl;
  Serial << "(a ^dot2multiplies^ b)(1)    : " << comp2e << endl;
  Serial << "FC++ compose2 does the same as fgh." << endl;
  Serial << "compose2(plus,a,b)(1) : " << comp2 << endl;
  Serial << "compose2(multiplies,a,b)(1) : " << comp3 << endl;
  Serial << "======================================================"
            << endl;
  Serial << "Can I generalise this to form a reader monad?" << endl;
  Serial << "What would that consist of?" << endl;
  Serial << "I would need formulations for unit and bind." << endl;
  Serial << "That would enable access to other things as well."
            << endl;
  Serial << "======================================================"
            << endl;

}


void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  while (!Serial) { }
  //::delay(5000);
  Serial.printf("Pico RP2040 FC++ operations\n");
  fcpp_examples();
  Serial.println("after fcpp_examples");
  prove_a_point();
  Serial.println("--------------------------");
  category_examples();
  Serial.println("--------------------------");
  //Serial.flush();
}

int i = 0;
 
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(i);
  //i++;
}