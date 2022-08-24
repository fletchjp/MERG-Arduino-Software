//////////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Monoids
// This file has the monoid examples from PicoRP2040FCPPCategory
// This example is also being used for the development of the monoid code. 
// I now have an example working applying two different operators to paired data.
// I have built bimap2 and parallel2 to operate on pairs of two argument functoids.
// As I invent new FC++ functoids I am moving them to be in the most relevant location.
// I have managed to make e.g. mmappend(inc,inc) by making a specialization for it.
// I have now also extended that to monoporphic versions.
// I think that is a clue to me that there are things which are Monoid implementations
// in FC++ which I have not recognised as such as I did not understand Monoid. 
//////////////////////////////////////////////////////////////////////
// First experiments with the Monoid structure is looking interesting.
// Idea: use inheritance from List<T> to implement a Monoid for List<T>.
// This is now working.
// The monoid codes have now been moved to a new header file fcpp/monoids.h
// The Arduino specific output operators are in this file.
// I have now built a more general way of doing Monoid types.
// These are defines by MonoidT<MonoidType<T,Op>> where T is the return type and 
// Op the operation defined as an FC++ binary operator.
// e.g. MonoidType<int,fcpp::Plus> and MonoidType<int,fcpp::Multiplies>
// I have started to implement ways in which monoids of the same type T and different 
// operators can be combined using mappend operations to get a result.
// I want to have types which do bool with and and or.
// I have done a lot on the use of mixed monoids where the result type is the same.
// I do not think it is possible to infer the type of the mixed operations.
// I have now extended this to exclusive or (XOR) which I have not found in FC++
// so I have had to add a functoid operator for this.
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

/// Monoid operations based on ideas from https://bartoszmilewski.com/2014/12/05/categories-great-and-small/
/// and also from Learn You a Haskell for Great Good! p.252.
///
/// Haskell code:
///
/// class Monoid m where
///    mempty  :: m
///    mappend :: m -> m -> m
///    mconcat :: [m] -> m
///    mconcat = foldr mappend mempty
///    mnot    :: m -> !m  - implemented to negate bool values.
///
/// Note: mconcat is only in Learn You a Haskell for Great Good! p.252.
///
/// Following Monads in monad.h what I need first are the free functions.

/// Infer structure from the equivalent for Monad in monad.h

/// Code now moved to fcpp/monoids.h except for Arduino specific output operators which are retained here.
/// It is now renamed from fcpp/functoids.h.
///
/// More code (below) can now move as well.
///
/// Note: the inferred versions start with mm.
/// They infer the type of the monoid from the arguments.
/// In the case of Mendo the type is Full1<T> and there is a specific implementation.
///
//////////////////////////////////////////////////////////////////////
/// Free functions for Monoids
///    mempty  :: m
///    mappend :: m -> m -> m
///    mconcat :: [m] -> m
//////////////////////////////////////////////////////////////////////
/// mempty<Monoid>()();
///
/// mappend<Monoid>()(m1,m2);
/// mmappend(m1,m2) - now inferrable for Mstring, Mlist<T> and MonoidT<T> types
///.
///
/// mconcat<Monoid>()(lm) where lm has the type List<Monoid>
/// mmconcat(lm)       - now inferrable for Mstring, Mlist<T> and MonoidT<T> types
///
/// mnot<Monoid>()(m)  - m -> !m  - I have invented this to make things easier to use.
/// mmnot(m)           - now inferrable for MonoidT<MonoidType> types.
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// The next thing is to infer mmappend(m1,m2)
// based on bind in monad.h from line 206
// I have now done mmconcat(lm) as well.
////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
/// A first construct for an instance of a Monoid.
/// I also want to make other things e.g. List become Monoids as well.
/// This has been extended.
/// (1) to provide for it to hold the string.
/// (2) to have a constructor from a string.
/// (3) to be able to output the string.
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// A construct for an instance of a Monoid based on List<T>.
/////////////////////////////////////////////////////////////////////
/// The above have been moved to fcpp/monoids.h
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Idea for a structure which can be a Monoid for different types.
// The type needs to have a zero and an op for append.
// This now works such that the operations specific to the 
// type and operator are in MonoidType and MonoidT handles the interface.
// This makes it very easy to generate new instances.
// It now has mconcat as well.
// I want to explore interoperability of types with the same underlying T.
// MonoidType and MonoidT code now moved to fcpp/monoid.h
/////////////////////////////////////////////////////////////////////
// Insights from Typeclassopedia.
// 1.
// If a and b are instances of Monoid, then so is (a,b),
// using the monoid operations for a and b in the obvious pairwise manner.
// Note: (a,b) is Haskell notation for a pair.
// 2.
// If a is a Monoid, then so is the function type e -> a for any e;
// in particular, g `mappend` h is the function which applies both g and h
// to its argument and then combines the results
// using the underlying Monoid instance for a.
// This can be quite useful and elegant 
// 3.
// Endo a is a newtype wrapper for functions a -> a, which form a monoid under composition.
// I have been exploring this and have found these which I want to explore:
// https://www.reddit.com/r/haskelltil/comments/337g7t/endo_the_monoid_of_endomorphisms_under/
// https://hackage.haskell.org/package/endo-0.1.0.0/candidate/docs/Data-Monoid-Endo.html
//////////////////////////////////////////////////////////////////////////
// Examples of 1. Monoids using new FC++ operators bimap2 or parallel2 
//////////////////////////////////////////////////////////////////////////
// I have adapted bimap as bimap2.
// I need to store the type of this curried function.
using OpType = typeof(bimap2(fcpp::plus,fcpp::multiplies));
typedef MonoidType<std::pair<int,int>,OpType> MonoidPair;
template <> std::pair<int,int> MonoidPair::zero = std::make_pair(0,1);
// This stores an operator which will work on two arguments which are both pairs.
// I have restricted it to the case where the two pairs have the same type std::pair<A,B>
template <> OpType MonoidPair::op = bimap2(fcpp::plus,fcpp::multiplies);
//////////////////////////////////////////////////////////////////////////
// This example combines Any and All into one monoid working on pairs.
// This version uses bimap2
using OpTypeAnyAll = typeof(bimap2(fcpp::or2,fcpp::and2));
typedef MonoidType<std::pair<bool,bool>,OpTypeAnyAll> MonoidAnyAll;
template <> std::pair<bool,bool> MonoidAnyAll::zero = std::make_pair(false,true);
template <> OpTypeAnyAll MonoidAnyAll::op = bimap2(fcpp::or2,fcpp::and2);
//////////////////////////////////////////////////////////////////////////
// This example combines Any and All into one monoid working on pairs.
// This is an alternative using parallel2.
using OpTypeAnyAll2 = typeof(parallel2(makePair(fcpp::or2,fcpp::and2)));
typedef MonoidType<std::pair<bool,bool>,OpTypeAnyAll2> MonoidAnyAll2;
template <> std::pair<bool,bool> MonoidAnyAll2::zero = std::make_pair(false,true);
template <> OpTypeAnyAll2 MonoidAnyAll2::op = parallel2(makePair(fcpp::or2,fcpp::and2));
//////////////////////////////////////////////////////////////////////////
// The difference is that bimap2 takes the two functors as separate arguments
// and parallel2 takes them as a pair.
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Examples of 3. Endomorphic operations with Fun1<T,T> and Full1<F> operations.
// These overcome an important issue that Full1<F> objects all have different types.
///////////////////////////////////////////////////////////////////////////////
// Monoid Type for Endo.
// I think I need a functoid first to handle the polymorphism.
// I have a problem with this as each Full1<F> has a different type.
// I can have monomorphic versions Fun1<T,T> which I have used in the past.
// I would like Endo to be a non template type which can hold ANY Full1<T> type.
// I have not figured out how to do this.
// I think the answer is to template it for the type and hold the object
// as monomorphic.
// Endo<int> endoid now holds id such that endoid() is equivalent.
// I now have MonoidEndo which uses Endo and MonadType.
// It supports mconcat operations, which Mendo does not.
// I think this has other applications.
// This is a separate thing from Mendo which is polymorphic.
//////////////////////////////////////////////////////////////////////////

  template <class T>
  struct Endo {
  typedef Fun1<T,T> Type;
  typedef T ElementType;
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Endo");
       }
#endif
private:
       Type f_;
public:
       Endo() : f_(id) { }
       template <class F>
       Endo(const Full1<F> &f) : f_(f) { }
       Endo(const Fun1<T,T> &f) : f_(f) { }
       Endo(const Endo &x) : f_(x.f_) { }
       Type operator()() const
       {
          return f_;
       }
       operator Type() const
       {
          return f_;
       }
       T operator()(const T& t) const
       {
          return f_(t);
       }

  };

// This is using MonoidType and MonoidT from monoids.h
typedef MonoidType<Endo<int>::Type,Compose> MonoidEndo;
template <> Endo<int>::Type MonoidEndo::zero = id;
template <> Compose MonoidEndo::op = compose;


//////////////////////////////////////////////////////////////////////////
// I am caught by the polymorphism of FC++ - I want a "type" for any Full1<T>.
// So I made Mendo polymorphic.
// This now works but I am not sure when it is worthwhile to use it.
// Mendo::mempty()()(1) == id(1)
// Mendo::mappend()(inc,inc)(1) ==  compose(inc,inc)(1)
// These cases use compose which can cope with e.g compose(inc,plus)
// as long as it is the second arg which has more than one argument.
// with more than one argument.
// I have now made mmappend(inc,inc) work using a specialisation
// in fcpp/monoids.h to use compose directly.
//////////////////////////////////////////////////////////////////////////

struct Mendo {
   struct Rep { typedef Mendo Type; };
   typedef Mendo MonoidType;
private:
// ?? How do I hold the contents? Do I need to?
// No - this Monoid has no state.
// mempty returns id
// mappend on (f,g) returns compose(f,g).
// mconcat is not implemented as I have no way
// to have a list of polymorphic functoids.
public:
      struct XMempty : public CFunType<Id> {      
      Id operator()() const {
         return id;
      }
    };
    typedef Full0<XMempty> Mempty;
    static Mempty& mempty() {static Mempty f; return f;}

    struct XMappend {
     template <class F,class G>
     struct Sig : public FunType<F,G,typename impl::XCompose::Helper<FunctoidTraits<G>::max_args,F,G>::Result> {};
       //typename F::template Sig<typename RT<G>::ResultType>::ResultType> {};

       template <class F,class G>
       typename Sig<F,G>::ResultType
       operator()(const F &f,const G &g) const {
         return f ^dot^ g;
      }
    };
    typedef Full2<XMappend> Mappend;
    static Mappend& mappend() {static Mappend f; return f;}

};

template <> struct MonoidTraitsSpecializer<Mendo > {
   typedef Mendo Monoid;
};

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

void monoid_examples()
{
  Serial << "Monoid examples"
            << endl;
  Serial << "======================================================"
            << endl;
  Mstring ms0;
  Mstring ms1(std::string(" one"));
  Mstring ms2(std::string(" two"));
  Mstring ms3(std::string(" three"));
  Mstring ms1s2 = Mstring::mappend()(ms1,ms2);
  Serial << "mappend  : " << ms1s2 << endl;
  // This one now works.
  Mstring ms2s1 = mappend<Mstring>()(ms2,ms1);
  Serial << "mappend  : "<< ms2s1 << endl;
  Mstring ms2s3 = mmappend(ms2,ms3);
  Serial << "mmappend : "<< ms2s3 << endl;
  Mstring mtest = Mstring::mempty()();
  Mstring mtest2 = mempty<Mstring>()(); // This does now work
  //Mstring mtest3  = impl::XMempty<Mstring>()(); // This does work.
  List<Mstring> lms = list_with(ms1,ms2,ms3);
/*    Serial << "[ ";
    for (auto i = lms.begin(); i != lms.end(); ++i)
    {
        Serial << (*i).get_string() << " ";
    }
    Serial << "]" << endl; */
  Serial << lms << endl;
  //Mstring ms2lms = mmappend(ms2,lms);
  Mstring mres = Mstring::mconcat()(lms);
  Serial << "mconcat  : " << mres << endl;
  List<Mstring> lms2 = list_with(ms3,ms2,ms1);
  Mstring mres2 = mconcat<Mstring>()(lms2);
  Serial << "mconcat  : " << mres2 << endl;
  List<Mstring> lms3 = list_with(ms3,ms1,ms2);
  Mstring mres3 = mmconcat(lms3);
  Serial << "mmconcat : " << mres3 << endl;
  Serial << "======================================================"
            << endl;
  Mlist<int> ml0;
  Mlist<int> ml12 = list_with(1,2);
/*    Serial << "[ ";
    for (auto i = ml1.begin(); i != ml1.end(); ++i)
    {
        Serial << *i << " ";
    }
    Serial << "]" << endl;
*/
  Serial << ml12 << endl;
  Mlist<int> ml34 = list_with(3,4);
  Serial << ml34 << endl;
  Mlist<int> ml34a = ml34;  // Copy from Mlist<T>
  Serial << ml34a << endl;
  List<Mlist<int>> lm;
  lm = cons(ml12,lm);
  lm = snoc(ml34,lm);
  Serial << lm << endl;
  List<int>  l1234 = concat(lm);
  Serial << l1234 << endl;
  List<int>  ml1234 = l1234; // Copy from List<T>
  Serial << ml1234 << endl;
  List<int> l56 = list_with(5,6);
  Serial << l56 << endl;
  Mlist<int> ml56 = l56; // Copy from List<T>
  Serial << ml56 << endl;
  List<int> l78 = list_with(7,8);
  Mlist<int> ml78 = l78; // Copy from List<T>
  List<List<int>> ll;
  ll = cons(l56,ll);
  ll = snoc(l78,ll);
  List<int> l5678 = concat(ll);
  Serial << l5678 << endl;
  Serial << "======================================================"
            << endl;
  Serial << "Monoid operations mmappend and mmconcat on Mlist<T>" << endl;
  Serial << "======================================================"
            << endl;  
  Mlist<int> ml1234a = Mlist<int>::mappend()(ml12,ml34);
  Serial << "Mlist<int>::mappend()(ml12,ml34) = " << ml1234a << endl;
  Mlist<int> ml1234b = mappend<Mlist<int>>()(ml12,ml34);
  Serial << "mappend<Mlist<int>>()(ml12,ml34) = " << ml1234b << endl;
  Mlist<int> ml1234c = mmappend(ml12,ml34);
  Serial << "mmappend(ml12,ml34)              = " << ml1234c << endl;
  Mlist<int> ml1234d = Mlist<int>::mconcat()(lm);
  Serial << "Mlist<int>::mconcat()(lm)        = " << ml1234d << endl;
  lm = snoc(ml56,lm);
  Mlist<int> ml1234e = mconcat<Mlist<int>>()(lm);
  Serial << "mconcat<Mlist<int>>()(lm)        = " << ml1234e << endl;
  lm = snoc(ml78,lm);
  Mlist<int> ml1234f = mmconcat(lm);
  Serial << "mmconcat(lm)                     = " << ml1234f << endl;
  Serial << "======================================================"
            << endl;
  Serial << "MonoidType examples for plus and multiplies" << endl;
  Serial << "======================================================"
            << endl;
  MonoidPlus p00;
  MonoidPlus p01 = MonoidT<MonoidPlus>::mempty()();
  Serial << "MonoidT<MonoidPlus>::mempty()() = " << p01 << endl;
  MonoidPlus p1(1);
  MonoidPlus p2(2);
  MonoidPlus p3(3);
  MonoidPlus p30 = MonoidT<MonoidPlus>::mappend()(p1,p2);
  Serial << "MonoidT<MonoidPlus>::mappend()(p1,p2) = " << p30 << endl;
  MonoidPlus p3a = mappend<MonoidT<MonoidPlus>>()(p1,p2);
  Serial << "mappend<MonoidT<MonoidPlus>>()(p1,p2) = " << p3a << endl;
  MonoidPlus p3b = mmappend(p1,p2);
  Serial << "mmappend(p1,p2) = " << p3b << endl;
  List<MonoidPlus> lp3 = list_with(p1,p2,p3);
  MonoidPlus p6 = mconcat<MonoidT<MonoidPlus>>()(lp3);
  Serial << "mconcat<MonoidT<MonoidPlus>>()(lp3) = " << p6 << endl;
  MonoidMultiplies m1(1);
  MonoidMultiplies m2(2);
  MonoidMultiplies m3(3);
  List<MonoidMultiplies> lm3 = list_with(m1,m2,m3);
  MonoidMultiplies m01 = MonoidT<MonoidMultiplies>::mempty()();
  Serial << "MonoidT<MonoidMultiplies>::mempty()() = " << m01 << endl;
  MonoidMultiplies m30 = MonoidT<MonoidMultiplies>::mappend()(m1,m2);
  Serial << "MonoidT<MonoidMultiplies>::mappend()(m1,m2) = " << m30 << endl;
  MonoidMultiplies m6 = mconcat<MonoidT<MonoidMultiplies>>()(lm3);
  Serial << "mconcat<MonoidT<MonoidMultiplies>>()(lm3) = " << m6 << endl;
  //MonoidMultiplies m330 = mmappend(m1,m2);
  //Serial << "mmappend(m1,m2) = " << m330 << endl;
  Serial << "======================================================"
            << endl;
  Serial << "mixed type operations" << endl;
  Serial << "======================================================"
            << endl;
  MonoidMultiplies m31 = MonoidT<MonoidMultiplies>::mappend()(m1,p2);
  MonoidMultiplies m32 = MonoidT<MonoidMultiplies>::mappend()(p1,m2);
  MonoidMultiplies m33 = MonoidT<MonoidMultiplies>::mappend()(p1,p2);
  Serial << "MonoidT<MonoidMultiplies>::mappend()(m1,p2) = " << m31 << endl;
  Serial << "MonoidT<MonoidMultiplies>::mappend()(p1,m2) = " << m32 << endl;
  Serial << "MonoidT<MonoidMultiplies>::mappend()(p1,p2) = " << m33 << endl;
  MonoidPlus p33 = MonoidT<MonoidPlus>::mappend()(m1,m2);
  Serial << "MonoidT<MonoidPlus>::mappend()(m1,m2) = " << p33 << endl;
  // This is now working.
  MonoidPlus p33a = mappend<MonoidT<MonoidPlus>>()(p1,m2);
  Serial << "mappend<MonoidT<MonoidPlus>>()(p1,m2) = " << p33a << endl;
  MonoidPlus p33b = mappend<MonoidT<MonoidPlus>>()(m1,p2);
  Serial << "mappend<MonoidT<MonoidPlus>>()(m1,p2) = " << p33b << endl;
  // This also works.
  MonoidPlus p33c = mappend<MonoidT<MonoidPlus>>()(m1,m2);
  Serial << "mappend<MonoidT<MonoidPlus>>()(m1,m2) = " << p33c << endl;
  // I can see no way to get mmappend(p1,m2) working.
  Serial << "======================================================"
            << endl;
  Serial << "Operations on monoid types returning bool" << endl;
  Serial << "======================================================"
            << endl;
  MonoidAny any0;
  MonoidAny anyt = true;
  MonoidAny anyf = false;
  MonoidAny any1 = mmappend(any0,anyt);
  if (any1()) Serial << "mmappend(any0,anyt) is true"  << endl;
  MonoidAny any2 = mmappend(any0,anyf);
  if (any2()) Serial << "mmappend(any0,anyf) is true"  << endl;
  else Serial << "mmappend(any0,anyf) is false"  << endl;
  List<MonoidAny> lany = list_with(any0,anyt,anyf);
  Serial << "List<MonoidAny> lany = list_with(any0,anyt,anyf);" << endl;
  MonoidAny any3 = mmconcat(lany);
  if (any3()) Serial << "mmconcat(lany) is true"  << endl;
  else Serial << "mmconcat(lany) is false"  << endl;
  MonoidAny any4 = MonoidT<MonoidAny>::mnot()(mmconcat(lany));
  if (any4()) Serial << "MonoidT<MonoidAny>::mnot()(mmconcat(lany)) is true"  << endl;
  else Serial << "MonoidT<MonoidAny>::mnot()(mmconcat(lany)) is false"  << endl;
  MonoidAny any5 = mnot<MonoidT<MonoidAny>>()(mmconcat(lany));
  if (any4()) Serial << "mnot<MonoidT<MonoidAny>>()(mmconcat(lany)) is true"  << endl;
  else Serial << "mnot<MonoidT<MonoidAny>>()(mmconcat(lany)) is false"  << endl;
  MonoidAny any6 = mmnot(mmconcat(lany));
  if (any4()) Serial << "mmnot(mmconcat(lany)) is true"  << endl;
  else Serial << "mmnot(mmconcat(lany)) is false"  << endl;
  MonoidAll all0;
  MonoidAll allt = true;
  MonoidAll allf = false;
  MonoidAll all1 = mmappend(all0,allt);
  if (all1()) Serial << "mmappend(all0,allt) is true"  << endl;
  else Serial << "mmappend(all0,allt) is false"  << endl;
  MonoidAll all2 = mmappend(all0,allf);
  if (all2()) Serial << "mmappend(all0,allf) is true"  << endl;
  else Serial << "mmappend(all0,allf) is false"  << endl;
  List<MonoidAll> lall = list_with(all0,allt,allf);
  Serial << "List<MonoidAll> lall = list_with(all0,allt,allf);" << endl;
  MonoidAll all3 = mmconcat(lall);
  if (all3()) Serial << "mmconcat(lall) is true"  << endl;
  else Serial << "mmconcat(lall) is false"  << endl;
  MonoidXor xor0;
  MonoidXor xort = true;
  MonoidXor xorf = false;
  MonoidXor xor1 = mmappend(xor0,xort);
  if (xor1()) Serial << "mmappend(xor0,xort) is true"  << endl;
  MonoidXor xor2 = mmappend(xor0,xorf);
  if (xor2()) Serial << "mmappend(xor0,xorf) is true"  << endl;
  else Serial << "mmappend(xor0,xorf) is false"  << endl;
  MonoidXor xor3 = mmappend(xort,xort);
  if (xor3()) Serial << "mmappend(xort,xort) is true"  << endl;
  else Serial << "mmappend(xort,xort) is false"  << endl;
  MonoidXor xor4 = mmappend(xort,mmnot(xort));
  if (xor4()) Serial << "mmappend(xort,mmnot(xort)) is true"  << endl;
  else Serial << "mmappend(xort,mmnot(xort)) is false"  << endl;
  MonoidXor xor1a = xor0 ^mmappend^ xort; // Infix works so I could invent a shorter name.
  if (xor1a()) Serial << "xor0 ^mmappend^ xort is true"  << endl;
  Serial << "======================================================"
            << endl;
  Serial << "New ideas for monoid types " << endl;
  Serial << "(1) - monoids which work on a pair of values" << endl;
  Serial << "======================================================"
            << endl;
  MonoidPair mp0;
  MonoidPair mp1 = std::make_pair(1,2);
  Serial << "MonoidPair mp0 = ( " << mp0.value.first << ", " << mp0.value.second << " )" << endl;
  Serial << "MonoidPair mp1 = std::make_pair(1,2) = ( " << mp1.value.first << ", " << mp1.value.second << " )" << endl;
  // This does now work with an operator which can work on the two parts.
  MonoidPair mp2 = mmappend(mp0,mp1);
  Serial << "mp2 = mmappend(mp0,mp1) = ( " << mp2.value.first << ", " << mp2.value.second << " )" << endl;
  MonoidPair mp3 = mmappend(mp2,mp1);
  Serial << "mp3 = mmappend(mp2,mp1) = ( " << mp3.value.first << ", " << mp3.value.second << " )" << endl;
  MonoidPlus p4(4);
  MonoidPlus p4a(p4); // Make a monoid instance from another of the same type.
  MonoidMultiplies m4(p4a.value); // If the data types match this works.
  MonoidPair mp4 = std::make_pair(p4(),m4());
  List<MonoidPair> lmp = list_with(mp0,mp1,mp2,mp3,mp4);
  MonoidPair mp5 = mmconcat(lmp);
  Serial << "mp5 = mmconcat(lmp) = ( " << mp5.value.first << ", " << mp5.value.second << " )" << endl;
  MonoidPair mp6 = parallel2(makePair(fcpp::plus,fcpp::multiplies),mp2.value,mp3.value);
  Serial << "MonoidPair mp6 = parallel2(makePair(fcpp::plus,fcpp::multiplies),mp2.value,mp3.value);" << endl;
  Serial << "mp6 = ( " << mp6.value.first << ", " << mp6.value.second << " )" << endl;  
  Serial << "======================================================"
            << endl;
  Serial << "MonoidAnyAll combines any and all into a pair." << endl;
  Serial << "======================================================"
            << endl;
  MonoidAnyAll maa0;
  MonoidAnyAll maa1 = std::make_pair(any0.value,all0.value);
  MonoidAnyAll maa2 = std::make_pair(anyt(),allt());
  Serial << "MonoidAnyAll maa2 = std::make_pair(anyt(),allt());" << endl;
  if (maa2.value.first && maa2().second) Serial << "maa2 values are both true" << endl;
  MonoidAnyAll2 maa20;
  MonoidAnyAll2 maa22 = std::make_pair(anyt(),allt());
  Serial << "MonoidAnyAll2 maa22 = std::make_pair(anyt(),allt());" << endl;
  if (maa22.value.first && maa22().second) Serial << "maa22 values are both true" << endl;
  Serial << "======================================================"
            << endl;
  Serial << "Endo<T> is an endomorphic wrapper for Full1 operators once a type is given." << endl;
  Serial << "MonoidEndo provides mempty mappend and mconcat for Endo<T>." << endl;          
  Serial << "typedef MonoidType<Endo<int>::Type,Compose> MonoidEndo;" << endl;
  Serial << "======================================================"
            << endl;
  Endo<int> endoid;
  Serial << "Endo<int> endoid;"  << endl;
  Endo<int> endoinc(inc);
  Serial << "Endo<int> endoinc(inc);" << endl;
  MonoidEndo monendo;
  Serial << "MonoidEndo monendo;" << endl;
  MonoidEndo monendoinc(endoinc);
  Serial << "MonoidEndo monendoinc(endoinc);" << endl;
  MonoidEndo monendoP2(fcpp::plus(2));
  MonoidEndo monendo2 = MonoidT<MonoidEndo>::mappend()(monendo,monendoinc);
  Serial << "MonoidEndo monendo2 = MonoidT<MonoidEndo>::mappend()(monendo,monendoinc);" << endl;
  Serial << "monendo2()(3) = " << monendo2()(3) << endl; ;
  MonoidEndo monendo3 = mappend<MonoidT<MonoidEndo>>()(monendo,monendoinc);
  Serial << "MonoidEndo monendo3 = mappend<MonoidT<MonoidEndo>>()(monendo,monendoinc);" << endl;
  Serial << "monendo3()(4) = " << monendo3()(4) << endl;
  MonoidEndo monendo4 = mmappend(monendo,monendoinc);
  Serial << "MonoidEndo monendo4 = mmappend(monendo,monendoinc);" << endl;
  Serial << "monendo4()(5) = " << monendo4()(5) << endl;
  List<MonoidEndo> lmonendo = list_with(monendo,monendoinc,monendoP2);
  MonoidEndo monendo5 = MonoidT<MonoidEndo>::mconcat()(lmonendo);
  Serial << "List<MonoidEndo> lmonendo = list_with(monendo,monendoinc,monendoP2);" << endl;
  Serial << "MonoidEndo monendo5 = MonoidT<MonoidEndo>::mconcat()(lmonendo);" << endl;
  Serial << "monendo5()(4) = " << monendo5()(4) << endl;
  Serial << "======================================================"
            << endl;
  int w = Mendo::mempty()()(1);
  Serial << "Mendo::mempty()()(1) = " << w 
         << ", id(1) = " << id(1) << endl;
  int x = Mendo::mappend()(inc,inc)(1);
  Serial << "Mendo::mappend()(inc,inc)(1) = " << x 
         << ", compose(inc,inc)(1) = " << compose(inc,inc)(1)<< endl;
  int y = mappend<Mendo>()(inc,inc)(2);
  Serial << "mappend<Mendo>()(inc,inc)(2) = " << y 
         << ", compose(inc,inc)(2) = " << compose(inc,inc)(2)<< endl;
  int z = mappend<Mendo>()(endoinc(),endoinc())(3);
  Serial << "mappend<Mendo>()(endoinc(),endoinc())(3) = " << z << endl;
  int zz = mmappend(inc,inc)(2);
  Serial << "mmappend(inc,inc)(2) = " << zz << endl;
  int zzz = mmappend(endoinc(),endoinc())(4);
  Serial << "mmappend(endoinc(),endoinc())(4) = " << zzz << endl;
  Serial << "======================================================"
            << endl;
  Serial << "The way Mendo mappend is defined allows the second argument" << endl;
  Serial << "to have more than one argument."  << endl;
  Serial << "This is because of the way compose is implemented." << endl;
  Serial << "This does not apply to the mmappend implementation," << endl;
  Serial << "which must use the curried version." << endl;
  Serial << "======================================================"
            << endl;
  int p = Mendo::mappend()(inc,fcpp::plus(2))(3);
  Serial << "Mendo::mappend()(inc,fcpp::plus(2))(3) = " << p << endl;
  int q = mappend<Mendo>()(inc,fcpp::plus(2))(4);
  Serial << "mappend<Mendo>()(inc,fcpp::plus(2))(4) = " << q << endl;
  int r = Mendo::mappend()(inc,fcpp::plus)(2,5);
  Serial << "Mendo::mappend()(inc,fcpp::plus)(2,5) = " << r << endl;
  int s = mappend<Mendo>()(inc,fcpp::plus)(3,5);
  Serial << "mappend<Mendo>()(inc,fcpp::plus)(3,5) = " << s << endl;
  int t= mmappend(inc,fcpp::plus(3))(6);
  Serial << "mmappend(inc,fcpp::plus(3))(6) = " << t << endl;
  Serial << "======================================================"
            << endl;
  Serial << "Monomorphic operations are now working." << endl;
  Serial << "======================================================"
            << endl;
  Fun1<int,int> fid = id; // This was fid which was causing a big problem!!
  Fun1<int,int> finc = inc;
  //Fun1<int,int> eid = endo(fid);
  int f2 = mmappend(fid,finc)(1);
  Serial << "f2 = mmappend(fid,finc)(1) = " << f2 << endl;
  int f3 = compose(fid,finc)(2);
  Serial << "f3 = compose(fid,finc)(2) = " << f3 << endl;
  int f4 = fid(finc(3));
  Serial << "f4 = fid(finc(3)) = " << f4 << endl;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  while (!Serial) { }
  //::delay(5000);
  Serial.printf("Pico RP2040 FC++ Monoid operations\n");
  Serial.println("------------------------------------);
  monoid_examples();
  Serial.println("------------------------------------");
  Serial.println("End of FC++ Monoid examples.");
  Serial.println("------------------------------------");
}

int i = 0;
 
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(i);
  //i++;
}
