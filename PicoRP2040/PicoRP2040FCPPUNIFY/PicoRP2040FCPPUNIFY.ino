////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Unify from Pico RP2040 FC++ Monad
// I am working to bring in things from the unify.cpp example file.
// I have modified more of the FC++ headers to work in the Arduino/Pico environment.
// This involves disabling exceptions.
// I have improved the output operators for Maybe<T> Either<T> and List<T>
// I have also done a lot of work on contrafunctors.
// I have been using head() to extract a value from a List type.
// I have invented unjust() to extract a value from a Maybe type.
// I have also added a new test MaybeLike to check the operation of unjust.
// This required a change to prelude.h. The code for unjust could now be moved there.
// I could now do the same thing for the Either type.
// I have started to move some of the new code to header files.
// MaybeLike and EitherLike and associated things are now in prelude.h
////////////////////////////////////////////////////////////////
// See also PicoRP2040FCPPCategory
////////////////////////////////////////////////////////////////
// Some of the notes from unify.cpp
////////////////////////////////////////////////////////////////
//
// There are now examples of 2 functor laws and 5 applicative functor laws.
// These come from "Learn You a Haskell for Great Good" on the pages noted.
// There is one more applicative functor law still to be done.

// I think the most important one for my work is this:

// pure f <*> x = fmap f x

// Here 'x' is an item already in one or other monad e.g. just(3).

// The reason is that it provides a unification between functors (fmap) and
// applicative functors (<*>). That works for me for cases where there is
// both a monad and an applicative functor implementation.

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
// Some exceptions guarding in pattern.h have had to be removed.
#define FCPP_UNIFY // Unification of Monads and Functors in functors.h
#include "fcpp_prelude.h"
#include "fcpp/functors.h"
#include "fcpp/comonad.h"
#include "fcpp/arrow.h"


#include <Streaming.h>


using namespace fcpp;

/// I have renamed String to StringL to avoid a name clash with the Arduino use of String.
typedef List<char> StringL;

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
//   LambdaVar<2> Y;
//   LambdaVar<3> Z;
//   LambdaVar<4> W;
//   LambdaVar<5> A;
//   LambdaVar<6> P;
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

// By using Serial << arg.value(); to output the arg, it picks up the correct output operator.
template <typename T>
Print &operator <<( Print &obj, const Maybe<T> &arg)
{
    if (arg.is_nothing()) { 
       obj.print("nothing");
    } else {
       obj.print("Just ");
       Serial << arg.value();
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
       Serial << arg.right();
    }
    return obj; 
}

template <typename T>
Print &operator <<( Print &obj, const List<T> &arg)
{
    typename List<T>::iterator it;
    if (arg.is_empty()) {
       obj.print("[ ]");
    } else {
       obj.print("[ ");
       for (it = arg.begin(); it != arg.end(); ++it) {
          Serial << *it << " ";
       }
       Serial << "]" ;
    }
    return obj; 
}

//////////////////////////////////////////////////////////
// Space here for new functoids!!
namespace fcpp {

// I think these are related to work on Arrows and also Categories.

  namespace impl {

    struct XArrA {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("ArrA");
       }
#endif
      template <class F, class A>
      struct Sig : public FunType<F,A,
       std::pair<A,typename RT<F,A>::ResultType > > { };

      template <class F, class A>
      typename Sig<F, A>::ResultType operator()
      (const F& f, const A& a) const
      {
  return std::make_pair(a,f(a));
      }
    };
    
   struct XArrB {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("ArrB");
       }
#endif
      template <class F, class A>
      struct Sig : public FunType<F,A,
       std::pair<typename RT<F,A>::ResultType, A > > { };

      template <class F, class A>
      typename Sig<F, A>::ResultType operator()
      (const F& f, const A& a) const
      {
  return std::make_pair(f(a),a);
      }
    };


  }

 typedef Full2<impl::XArrA> ArrA;
 typedef Full2<impl::XArrB> ArrB;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN ArrA arrA;
 FCPP_MAYBE_EXTERN ArrB arrB;
 FCPP_MAYBE_NAMESPACE_CLOSE


  /////////////////////////////////////////////////////////////////////////
  //
  // xmap :: (a -> b) -> (b -> a) -> f a -> f b
  //
  // See later for a better implementation.
  //
  // This is set up to work only for monads with the Rep/Unrep
  // and liftM<Monad> structures.
  // I will want to generalise it to other Functors.
  // I am not convinced that this is correct as it currently
  // ignores the second argument.
  // I have been exploring the literature to find an example implemented.
  //
  // At the moment the implementation is xmapm(f,g,m).
  // It ignores g entirely and does liftM<Monad>()(f)(m)
  // deducing the correct return type.
  //
  // Now that I have made some progress with contraFmap I can do a rethink
  // on this one.
  /////////////////////////////////////////////////////////////////////////

  namespace impl {
    template <class F, class M>
    struct XmapMHelper {
      typedef typename MonadTraits<M>::Monad Monad;
      typedef typename Monad::template UnRep<M>::Type A;
      typedef typename RT<F,A>::ResultType B;
      typedef typename Monad::template Rep<B>::Type ResultType;
   };
    struct XXmapM
    {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("XmapM");
   }
#endif

// I have modified this to remove the parameter G g which is not being used.
// I don't remember what was going on.
// I have taken all the examples out of use.
// When I work out what G is doing I can do another example with it in.
//      template <class F, class G, class M> struct Sig :
//  public FunType<F,G,M,typename XmapMHelper<F,M>::ResultType> {};
      template <class F, class M> struct Sig :
  public FunType<F,M,typename XmapMHelper<F,M>::ResultType> {};

//      template <class F, class G, class M>
//      typename Sig<F,G,M>::ResultType
//      operator()(const F &f,const G &g, const M &m) const
       template <class F, class M>
      typename Sig<F,M>::ResultType
      operator()(const F &f, const M &m) const
      {
   // I want to retain this for the things which are
   // Monads. At the same time I want an alternative
   // way of handling Functors which are not Monads.
   // I think this is best done through a trait system.
   // This does not use parameter g - is that correct?
   typedef typename XmapMHelper<F,M>::Monad Monad;
         return liftM<Monad>()(f)(m);
       }
    };
  }
typedef Full3<impl::XXmapM> XmapM;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN XmapM xmapm;
FCPP_MAYBE_NAMESPACE_CLOSE


//////////////////////////////////////////////////////////////////////
// New functors xcomap and xcontramap
// These are a significant breakthrough in understanding.
// See examples of usage in contrafunctor_examples further down this file.
//////////////////////////////////////////////////////////////////////
namespace impl {
    template <class F, class G, class H>
    struct XcomapHelper {
      typedef typename RT<Compose,F,typename RT<Compose,G,H>::ResultType>::ResultType
              ResultType;
    };
    struct XXcomap
    {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("Xcomap");
      }
#endif

      template <class F, class G, class H> struct Sig :
  public FunType<F,G,H,typename XcomapHelper<F,G,H>::ResultType> {};

      template <class F, class G, class H>
      typename Sig<F,G,H>::ResultType
      operator()(const F &f,const G &g, const H& h) const
       {
   return compose(f,compose(g,h));
       }

    };

    template <class F, class G, class H>
    struct XcontramapHelper {
      typedef typename RT<Compose,H,typename RT<Compose,G,F>::ResultType>::ResultType
              ResultType;
    };
    struct XXcontramap
    {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("Xcontramap");
      }
#endif

      template <class F, class G, class H> struct Sig :
  public FunType<F,G,H,typename XcontramapHelper<F,G,H>::ResultType> {};

      template <class F, class G, class H>
      typename Sig<F,G,H>::ResultType
      operator()(const F &f,const G &g, const H& h) const
       {
   return compose(h,compose(g,f));
       }

    };

}
  typedef Full3<impl::XXcomap>  Xcomap;
  typedef Full3<impl::XXcontramap> Xcontramap;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN Xcomap      xcomap;
  FCPP_MAYBE_EXTERN Xcontramap  xcontramap;
  FCPP_MAYBE_NAMESPACE_CLOSE

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
  Serial << "I was having problems with Law 4" << endl;
  Serial << "It is star which does all the work currently." << endl;
  Serial << "Some experiments following p.239" << endl;
  Serial << "There are contexts where 'pure' is not sufficient."
         << endl;
  Serial << "====================================" << endl;
  List<int> l4_0;
  Serial << "l4_0 = " << l4_0 << endl;
  List<int> l4_1 = makeList1(4);
  List<int> l4_2 = cons(3,l4_1);
  Serial << "l4_2 = " << l4_2 << endl;
  Maybe<List<int> > ml4_1 = fmap (makeList1) (just(4));
  Either<List<int> > el4_1 = fmap (makeList1) (right(4));
  // Note: This returns an oddlist and needs somehow to
  // be forced.
  // cons returns an OddList<T> which will be changed to List<T>
  // by assignment.
  // However Maybe<OddList<int> >  is not changed to Maybe<List<int> >
  // by assignment and the compilation fails.
  // I am looking for a solution.
  // I would prefer one which is more general that just with Maybe<T>.
  // The answer is to use delay ^dot^ cons as the operator.
  Maybe<OddList<int> > mol4_2 = fmap (cons) ( just(3) ) ^star^ (ml4_1);
  // One solution is to use delay ^dot^ cons.
  Maybe<List<int> > ml4_2 = fmap (fcpp::delay ^dot^ cons) ( just(3) ) ^star^ (ml4_1);
  Either<List<int> > el4_2 = fmap (fcpp::delay ^dot^ cons) (right(3)) ^star^ (el4_1);
  Serial << "Maybe<List<int> > ml4_2 = fmap (delay ^dot^ cons) ( just(3) ) ^star^ (ml4_1); = " << endl;
  Serial << ml4_2 << endl;
  Serial << "Either<List<int> > el4_2 = fmap (fcpp::delay ^dot^ cons) (right(3)) ^star^ (el4_1); = " << endl;
  Serial << el4_2 << endl;
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
  Serial << "-----------------------------------------------"
            << endl;

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

//////////////////////////////////////////////////////////
void parallel_examples() {
  Serial << "=================================================="
            << endl;
  Serial << "parallel functoid (aka prod)" << endl;
  Serial << "parallel((f,g),(a,b) ) -> (f(a),g(b))" << endl;
  Serial << "bimap is related but different." << endl;
  Serial << "bimap(f,g,(a,b) ) -> (f(a),g(b))" << endl;
  Serial << "=================================================="
         << endl;
  Serial << "parallel(makePair(inc,dec),std::make_pair(4,6)) = ";
  std::pair<int,int> pii = parallel(makePair(inc,dec),std::make_pair(4,6));
  Serial << "(" << pii.first << "," << pii.second << ")" << endl;
  Serial << "parallel(makePair(id,id),std::make_pair(4,6))   = ";
  std::pair<int,int> pii2 = parallel(makePair(id,id),std::make_pair(4,6));
  Serial << "(" << pii2.first << "," << pii2.second << ")" << endl;
  Serial << "bimap(inc,dec,std::make_pair(4,6))              = ";
  std::pair<int,int> pii3 = bimap(inc,dec,std::make_pair(4,6));
  Serial << "(" << pii3.first << "," << pii3.second << ")" << endl;
  Serial << "bimap(id,id,std::make_pair(4,6))                = ";
  std::pair<int,int> pii4 = bimap(id,id,std::make_pair(4,6));
  Serial << "(" << pii4.first << "," << pii4.second << ")" << endl;
  Serial << "(first(inc)^dot^second(dec))(std::make_pair(4,6)) = ";
  std::pair<int,int> pii5 = (first(inc)^dot^second(dec))(std::make_pair(4,6));
  Serial << "(" << pii5.first << "," << pii5.second << ")" << endl;
  Serial << "=================================================="
            << endl;
  Serial << "contravariant functors (contrafunctors)" << endl;
  Serial << "   (b -> a) -> f a -> f b" << endl;
  Serial << "arrA : (a -> b) -> a -> p a b" << endl;
  Serial << "arrA returns a pair of the input and the output" << endl;
  Serial << "arrB : (a -> b) -> a -> p b a" << endl;
  Serial << "arrB returns a pair of the output and the input" << endl;
  Serial << "=================================================="
            << endl;
  Serial << "I am seeking a good example...." << endl;
  Serial << "=================================================="
            << endl;
  std::pair<int,int> pii6 = arrA(inc,2);
  Serial << "arrA(inc,2)    = ";
  Serial << "(" << pii6.first << "," << pii6.second << ")" << endl;
  std::pair<int,int> pii7 = arrB(inc,2);
  Serial << "arrB(inc,2)    = ";
  Serial << "(" << pii7.first << "," << pii7.second << ")" << endl;
  Serial << "Still digging" << endl;
  Serial << "fmap (inc)( just(3) )          : "
            <<  fmap (inc)( just(3) ) << endl;
//  Serial << "xmapm (inc,dec)(just(3))        : "
//            <<  xmapm(inc,dec)(just(3)) << endl;
//  Serial << "xmapm (dec,inc)(just(5))        : "
//            <<  xmapm(dec,inc)(just(5)) << endl;
  Serial << "==================================================="
            << endl;
  Serial << "Light dawns (a -> b) is about types as well as data"
            << endl;
  Serial << "Here are some examples moving in and out of lists"
            << endl;
  Serial << "==================================================="
            << endl;
  Maybe<List<int> > ml1 = just(list_with(1));
  Serial << "Maybe<List<int> > ml1 = just(list_with(1));" << endl;
  Serial << "fmap (head)( ml1 )            : "
      <<  fmap (head)( ml1 ) << endl;
//  Serial << "xmapm (head, enumFrom)( ml1 )  : "
//      <<  xmapm (head, enumFrom)( ml1 ) << endl;
  Serial << "Maybe<List<int> > ml2 = fmap( enumFrom )(just(1));"
            << endl;
  Maybe<List<int> > ml2 = fmap( enumFrom )(just(1));
  Serial << "fmap (head)( ml2 )            : "
      <<  fmap (head)( ml2 ) << endl;
//  Serial << "Maybe<List<int> > ml3 = xmapm( enumFrom, head )(just(1));"
//            << endl;
//  Maybe<List<int> > ml3 = xmapm( enumFrom, head )(just(1));
//  Serial << "fmap (head)( ml3 )  : "
//      <<  fmap (head)( ml3 ) << endl;
  //Serial << "fmap (enumFrom)( ml1 ) : "
  //      <<  fmap (enumFrom)( ml1 ) << endl;
//  Maybe<List<int> > ml4 = fmap (tail)( ml3 );
//  Serial << "Maybe<List<int> > ml4 = fmap (tail)( ml3 );"
//         << endl;
//  Serial << "fmap (head)( ml4 )  : "
//      <<  fmap (head)( ml4 ) << endl;
//  Serial << "Maybe<List<int> > ml5 = xmapm (tail,head)( ml4 );"
//            << endl;
//  Maybe<List<int> > ml5 = xmapm (tail,head)( ml4 );
//  Serial << "fmap (head)( ml5 )  : "
//      <<  fmap (head)( ml5 ) << endl;
}

void contrafunctor_examples()
{
  Serial << "==================================================="
            << endl;
  Serial << "Some examples of fmap, cofmap and contrafmap" << endl;
  Serial << "A lot of this is a question of type deduction." << endl;
  Serial << "fmap(f)(mx) - applies f to the contents of a monadic type mx" << endl;
  Serial << "The monad is deduced. This does not work with nonmonadic types." << endl;
  Serial << "I have made an Identity<T> object which can be made using ident(t)." << endl; 
  Serial << "The restriction is removed by using cofmap or contrafmap which do the conversions." << endl;
  Serial << "cofmap(f,g)(x) - applies g and then f to the object x" << endl;
  Serial << "contrafmap(f,g)(x) - applies f and then g to the object x" << endl;
  Serial << "Also xmap, xcomap and xcontramap" << endl;
  Serial << "for three functors F,G,H they are used as follows:" << endl;
  Serial << "xmap(f,g,h)(x) -> compose(f,compose(h,g))(x) - apply g then h then f." << endl;
  Serial << "This means that g(x) extracts a value which is processed using h and wrapped using f." << endl;
  Serial << "xcomap(f,g,h)(x) -> compose(f,compose(g,h))(x) - apply h then g then f." << endl;
  Serial << "xcontramap(f,g,h)(x) -> compose(h,compose(g,f))(x) - apply f then g then h." << endl;
  Serial << "==================================================="
            << endl;
   //fmap(id)(inc(1));
   int y00  = fmap(inc)(ident(1));
   // These work both ways around.
   int y01  = cofmap(inc,id)(1); 
   int y02  = cofmap(id,inc)(1); 
   int y03  = contrafmap(inc,id)(1); 
   int y04  = contrafmap(id,inc)(1); 
   Serial << "y00 = fmap(inc)(ident(1))   = " << y00 << " needs ident(1) " << endl;
   Serial << "y01 = cofmap(inc,id)(1)     = " << y01 << endl;
   Serial << "y02 = cofmap(id,inc)(1)     = " << y02 << endl;
   Serial << "y03 = contrafmap(inc,id)(1) = " << y03 << endl;
   Serial << "y04 = contrafmap(id,inc)(1) = " << y04 << endl;
   Serial << "==================================================="
          << endl;
   List<int> l1 = makeList1(1);
   Serial << "l1                            = " << l1 << endl;
   //List<int> l2 = contrafmap(head,inc)(l1);
   List<int> l2 = fcpp::map(inc)(l1);
   Serial << "l2 = fcpp::map(inc)(l1)       = " << l2 << endl;
   List<int> l3 = fmap(inc)(l2);
   Serial << "l3 = fmap(inc)(l2)            = " << l3 << endl;
   List<int> l4 = cofmap(makeList1,inc)(3);
   Serial << "l4 = cofmap(makeList1,inc)(3) = " << l4 << endl;
   List<int> l5 =  xmap(makeList1,head)(inc)(l4);
   Serial << "l5 = xmap(makeList1,head)(inc)(l4) = " << l5 << endl;
   // Note that head() tests that its argument is a list.
   // int y0 = contrafmap(head,inc)(just(4)); this fails at compile time with a clear message.
   int y1 = contrafmap(head,inc)(l4);
   Serial << "y1 = contrafmap(head,inc)(l4) =   " << y1 << endl;
   int y2 = xmap(head,inc,makeList1)(y1);
   Serial << "y2 = xmap(head,inc,makeList1)(y) = " << y2 << endl;
   int y3 = xcomap(head,makeList1,inc)(y2);
   Serial << "y3 = xcomap(head,makeList1,inc)(y2) = " << y3 << endl;
   int y4 = xcontramap(inc,makeList1,head)(y3);
   Serial << "y4 = xcontramap(inc,makeList1,head)(y3) = " << y4 << endl;
   Serial << "==================================================="
            << endl;
   Serial << "There is now a test to make sure that unjust() is called with a Maybe type." << endl;
   Serial << "This is copied from the similar test for ListLike types." << endl;
   Serial << "It was not needed before as there was nothing like unjust to extract the value." << endl;
   Serial << "==================================================="
          << endl;
   Maybe<int> m1 = just(1);
   Serial << m1 << endl;
   Maybe<int> m2 = fmap(inc)(m1);
   Serial << m2 << endl;
   int x = unjust(m2);
   Serial << x << endl;
   //int y = contrafmap(unjust,inc)(l2); fails as expected.
   int y = contrafmap(unjust,inc)(m2);
   Serial << "y = contrafmap(unjust,inc)(m2) =   " << y << endl;
   int z = cofmap(inc,unjust)(m2);
   Serial << "z = cofmap(inc,unjust)(m2) =   " << z << endl;
   Serial << "==================================================="
          << endl;
   Serial << "A test has now been inserted into Either<T> to test unright() in the same way." << endl;
   Serial << "==================================================="
          << endl;
   Either<int> e1 = right(1);
   Serial << "e1 = right(1) = " << e1 << endl;
   Either<int> e2 = fmap(inc)(e1);
   Serial << "e2 = fmap(inc)(e1) = " << e2 << endl;
   //int ex = unright(m2); //fails as expected.
   int ex = unright(e2);
   Serial << "ex = unright(e2) = " << ex << endl;
   int ey = contrafmap(unright,inc)(e2);
   Serial << "ey = contrafmap(unright,inc)(e2) =   " << ey << endl;
   int ez = cofmap(inc,unright)(e2);
   Serial << "ez = cofmap(inc,unright)(e2) =   " << ez << endl;
   Serial << "==================================================="
          << endl;
  
}
//////////////////////////////////////////////////////////

void monad_examples() {
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
  // put your setup code here, to run once:
  Serial.begin (115200);
  while (!Serial) { }
  
  monad_examples();
  unify_examples();
  lambda_examples();
  parallel_examples();
  contrafunctor_examples();
}
void loop() {
  // put your main code here, to run repeatedly:

}
