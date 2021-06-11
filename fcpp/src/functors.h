//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FUNCTORS_DOT_H
#define FCPP_FUNCTORS_DOT_H

#ifndef FCPP_ARDUINO
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_void.hpp>
#endif

  /////////////////////////////////////////////////////////////////////////
  // Inferrable Functors
  //
  // I want to generalize the inferrable property first developed
  // for the monads of FC++.
  // I have already built an Applicative property which I used on the
  // applicative functors MaybeA EitherA and ListA.
  //
  // I have now introduced a new Inferrable property which could apply to
  // any of a range of functors based on the same data type.
  // I have structured this to be inherited by any functor type and
  // it is already in use for MaybeA, EitherA and ListA.
  //
  // For example, there are already functors MaybeM and MaybeA which have
  // the same data type Maybe<T>. So Maybe<T> is not the exclusive property
  // of either MaybeM or MaybeA. It is shared property.
  //
  // I want to use this as a basis for a family of functors working on the
  // same property e.g. Maybe<A>
  // The relations (a -> m a) and (m a -> a) are known and can
  // be deployed in different combinations for the different cases.
  // See functor_definitions.cpp for some starting ideas on this.
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  // Applicative Functors - see p.228
  // Page references are to "Learn You a Haskell for Great Good!"
  // unless otherwise noted.
  // There is a longer discussion with the examples in functors.cpp
  //
  // class (Functor f) => Applicative f where
  //   pure :: a -> f a
  //  (<*>) :: f (a -> b) -> f a -> f b
  //
  // Note: fmap :: (a -> b) -> f a -> f b
  //
  // I need a name for <*> which I will call 'star'
  // This is difficult to construct in general as what is needed
  // is a way to get to (a -> b) from within f (a -> b)
  // (I think that this is a comonad!!)
  //
  /////////////////////////////////////////////////////////////////////////
  // NOTE: My present implementation of 'pure' is in fact 'id'.
  //       This is now understood as follows.
  //
  // My idea has been to make the implementation look as close as possible
  // to the Haskell code. I therefore made star able to infer the types,
  // and left 'pure' as 'id'. This works when there is a 'star' in the
  // expression. If not e.g. pure f x  it does not work as 'pure'
  // does not deduce the type. The solution is to use either
  // Functor::pure()(x) or pureA<Functor>()(x) instead which do deduce
  // because these access pure as defined in the Functor.
  // Another way of doing this is to replace 'pure' with the correct
  // expression needed e.g. just(x) for Maybe<T>.
  // This will not work except where the context is known.
  //
  // I have now also developed a version called pureL which works with ListA.
  // The next section is copied from functors.cpp.
  // Examples of use can be found in that file.
  ////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Inferrable pure is a problem.
//////////////////////////////////////////////////////////////////////////
// The problem is that in many cases it is not known what it should infer.
// This can be remedied in most cases when it is used around an argument
// to star, when what it is can be inferred from the other argument.
// To get to that I have put in a placeholder of id.
// This works in many cases but not with ListA.
//
// New idea. Make id a default case with a hidden argument.
// Experiment with pure2 carried out.
//
// There is now a version called pureL which works with ListA.
// Unfortunately I do not yet have a way in which it can be called
// silently. I will think about that.
//
//     example    :    pureL(inc) ^star^ (l12)
//     equivalent :    map(inc,l12)
//
// I have now done pureM and pureE for MaybeA and EitherA.
//
// So I now have a situation where 'pure' will work in some cases where
// the correct action can be deduced. Where the action needed is known the
// user can give a steer by using the correct one, as follows:
//
// pureE  for EitherA and Either<T>
// pureL  for ListA   and List<T>
// pureM  for MaybeA  and Maybe<T>
//
/////////////////////////////////////////////////////////////////////////

 
  /////////////////////////////////////////////////////////////////////////
  //
  // contrafmap :: (b -> a) -> f a -> f b
  // xmap :: (a -> b) -> (b -> a) -> f a -> f b
  //
  /////////////////////////////////////////////////////////////////////////

namespace fcpp {

  ////////////////////////////////////////////////////////////////////////
  // Helper functions for the use of generalised Functors.
  // This is an adaption of the code for inferrable monads.
  ////////////////////////////////////////////////////////////////////////
  // Note that this adaption of the ApplicativeTraits does a lot of things
  // which are not in the original FC++ monad traits.
  ////////////////////////////////////////////////////////////////////////
template <class Rep> struct ThisTypeIsNotAnInstanceOfAnInferrableFunctor {};
template <class T> struct InferrableError { inline static void error() {} };
template <class T>
struct  InferrableError<ThisTypeIsNotAnInstanceOfAnInferrableFunctor<T> > {};

 template <class Rep> struct InferrableTraitsSpecializer {
  static const bool isInferrable = false;
  typedef ThisTypeIsNotAnInstanceOfAnInferrableFunctor<Rep> InferrableFunctor;
  typedef Rep Type;  // To assist in getting one behaviour for star
  typedef Rep ReturnType;
   // These are retained from the Applicative definition.
  static ReturnType pure(const Rep& a) { return a; }
  typedef void Test;
};

template <class Rep> struct InferrableTraits {
  // This is the problem. I want to be able to have different
  // functors for the same Rep.
   typedef typename InferrableTraitsSpecializer<Rep>::InferrableFunctor InferrableFunctor;
   //Type is the underlying type (always)
   typedef typename InferrableTraitsSpecializer<Rep>::Type Type;
   inline static void ensure_is_instance_of_inferrable_functor() {
      InferrableError<InferrableFunctor>::error();
   }
   static const bool isInferrable =
      InferrableTraitsSpecializer<Rep>::isInferrable;
};

 template <class Name> struct InferrableNameTraits {
   static const bool isInferrableName = false;
   typedef Name InferrableName;
   template <typename T> struct Rep
   { typedef T Type; };
 };

  namespace impl {
    //template <class A, class K> struct StarHelper;
   template <bool b> struct InferrableError {};
   template <> struct InferrableError<true> { static inline void error() {} };
   template <class A, class K>
   struct InferrableHelper {
     typedef typename InferrableTraits<A>::InferrableFunctor InferrableA;
      typedef typename InferrableA::template UnRep<A>::Type AA;
      typedef typename InferrableA::Star::template Sig<A,K>::ResultType ResultType;
      typedef typename InferrableTraits<K>::InferrableFunctor InferrableK;
      typedef typename InferrableTraits<ResultType>::InferrableFunctor InferrableB;
      inline static void ensure_a_and_k_are_in_the_same_applicative_instance() {
      InferrableError<Conversion<InferrableA,InferrableB>::sameType>::error();
      }
   };
  }

  /////////////////////////////////////////////////////////////////////////
  // This is a generalisation of the internal contents for inferrable
  // which is now inherited by MaybeA, EitherA and ListA (see below).
  // There is a problem which is that there are two things being
  // looked after here.
  // There is the object type e.g. Maybe<T>
  // There is also the functor object e.g. MaybeA or MaybeM.
  // These have been combined into one trait.
  // This makes it hard to generalise as the past usage
  // contained the information relating e.g. MaybeA <-> Maybe<T>.
  // I think what is needed is separate traits.
  // I have now sorted out a way to separate this.
  // Inferrable is now inherited by anything I want to be inferrable.
  /////////////////////////////////////////////////////////////////////////
  // FCPP_UNIFY is a new define.
  // When this is defined code comes into play which means that
  // MaybeA has also the internals of MaybeM. This is under test.
  // It is intended that MaybeA can be used wherever MaybeM can be used.
  // Now tested:
  // liftM<MaybeA>() in unify.cpp
  // sprint of it in functors.cpp shows the use of
  // BindM<MaybeA> and UnitM<MaybeA> as well.
  // forM<MaybeA> and mapM<MaybeA> now also tested.
  // I have now also done this for EitherA.
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  // Here Name will be the classname of the Functor operator itself (CRTP).
  // Functor is the template name of the type of the object e.g. Maybe.
  // It is important that the name of the functor is declared this way
  // and not in the InferrableTraitsSpecializer which deals with the
  // object type. There could now be more than one functor operator
  // for a given object type.
  // e.g Maybe<T> has MaybeM and MaybeA.
  // I still cannot sort out the one to many problem.
  template <class Name, template <class> class Functor >
  struct Inferrable {
     static bool const isInferrableFunctor = true;
     typedef Name InferrableFunctor;
     template <class A>  struct Rep
     { typedef Functor<A> Type; };
     template <class FA> struct UnRep
     { typedef typename FA::ElementType Type; };
     
  };

  

  /////////////////////////////////////////////////////////////////////////
  // So what is there?
  //
  // The example of Maybe types with defining file listed.
  //
  // These are the types from FC++
  //
  // Maybe<T>        The type             prelude.h
  // MaybeM          Inferrable monad.    monad.h
  //
  // These are the new types added to FC++
  //
  // MaybeT<Monad>   Monad transformer    pattern.h
  // MaybeA          Applicative functor  functors.h
  // MaybeF<Functor> Functor transformer  functors.cpp
  //
  // Currently only applicative functors are inheriting Inferrable.
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  //
  // fmap :: (a -> b) -> f a -> f b
  //
  // This works only for monads with the Rep/Unrep and LiftM<Monad>
  // structures.  I want to generalise it to other Functors.
  //
  // I have build cofmap to work with that.
  // I think my fmap may be wrong. (This is I think out of date).
  /////////////////////////////////////////////////////////////////////////

  namespace impl {
    template <class F, class M>
    struct FmapHelper {
      typedef typename MonadTraits<M>::Monad Monad;
      typedef typename Monad::template UnRep<M>::Type A;
      typedef typename RT<F,A>::ResultType B;
      typedef typename Monad::template Rep<B>::Type ResultType;
   };
    struct XFmap
    {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("Fmap");
   }
#endif

       template <class F, class M> struct Sig :
	public FunType<F,M,typename FmapHelper<F,M>::ResultType> {};

       template <class F, class M>
       typename Sig<F,M>::ResultType
       operator()(const F &f,const M &m) const
       {
	 // I want to retain this for the things which are
	 // Monads. At the same time I want an alternative
	 // way of handling Functors which are not Monads.
	 // I think this is best done through a trait system.
	 // This can now be done with e.g. MaybeA as it now
	 // supports liftM.
	 typedef typename FmapHelper<F,M>::Monad Monad;
         return liftM<Monad>()(f)(m);
       }
    };
  }
typedef Full2<impl::XFmap> Fmap;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Fmap fmap;
FCPP_MAYBE_NAMESPACE_CLOSE

  /////////////////////////////////////////////////////////////////////////
  //
  // bimap :: (a -> b) -> (c -> d) -> p a c -> p b d
  //
  // Here p is a pair and there are two functions.
  // The first changes the first of the pair and
  // the second changes the second of the pair.
  //
  // 'first' and 'second' which are in pattern.h are related.
  //
  // This is similar but not the same as 'parallel' which is in pattern.h
  //
  /////////////////////////////////////////////////////////////////////////

  namespace impl {

    struct XBimap {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Bimap");
       }
#endif
      template <class F, class G,class Q>
      struct Sig : public FunType<F,G,Q,
	     std::pair<typename RT<F,typename Q::first_type>::ResultType,
		       typename RT<G,typename Q::second_type>::ResultType > > { };

      template <class F, class G,class Q>
      typename Sig<F,G,Q>::ResultType operator()
      (const F& f,const G& g,const Q &q) const
      {
	return std::make_pair(f(q.first),g(q.second));
      }
    };

  }

 typedef Full3<impl::XBimap> Bimap;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Bimap bimap;
 FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP152

 namespace impl {

  /////////////////////////////////////////////////////////////////////////
  //
  // cofmap     :: (a -> b) -> f a -> f b
  // contrafmap :: (b -> a) -> f a -> f b
  // xmap       :: (a -> b) -> (b -> a) -> f a -> f b
  //
  // cofmap(f,g)      -> compose(f,g)             CompF : runCompG :: f (g a)
  // contrafmap2(f,g) -> apply2fg(g,f)
  // xmap(f,g,h)      -> compose(f,compose(h,g))
  // xmap2(f,g,h)     -> compose(f,apply2fg(h,g))
  //
  // f,g and h are functoids but monads.
  //
  // apply2fg(f,g,a,b)   -> f(g(a),g(b)) is in prelude.h
  /////////////////////////////////////////////////////////////////////////
    template <class F, class G>
    struct CoFmapHelper {
      typedef typename RT<Compose,F,G>::ResultType ResultType;
    };
    struct XCoFmap
    {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("CoFmap");
      }
#endif

        template <class F, class G> struct Sig :
	public FunType<F,G,typename CoFmapHelper<F,G>::ResultType> {};

       template <class F, class G>
       typename Sig<F,G>::ResultType
       operator()(const F &f,const G &g) const
       {
	 return compose(f,g);
       }

    };

    template <class F, class G>
    struct CFmapHelper {
      typedef typename RT<Compose,G,F>::ResultType ResultType;
    };
    struct XContraFmap
    {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("ContraFmap");
      }
#endif

        template <class F, class G> struct Sig :
	public FunType<F,G,typename CFmapHelper<F,G>::ResultType> {};

       template <class F, class G>
       typename Sig<F,G>::ResultType
       operator()(const F &f,const G &g) const
       {
	 return compose(g,f);
       }

    };

    template <class F, class G>
    struct CFmapHelper2 {
      typedef typename RT<Apply2FG,G,F>::ResultType ResultType;
    };
    struct XContraFmap2
    {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("ContraFmap2");
      }
#endif

        template <class F, class G> struct Sig :
	public FunType<F,G,typename CFmapHelper2<F,G>::ResultType> {};

       template <class F, class G>
       typename Sig<F,G>::ResultType
       operator()(const F &f,const G &g) const
       {
	 return apply2fg(g,f);
       }

    };

    template <class F, class G, class H>
    struct XmapHelper {
      typedef typename RT<Compose,F,typename RT<Compose,H,G>::ResultType>::ResultType
              ResultType;
    };
    struct XXmap
    {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("Xmap");
      }
#endif

      template <class F, class G, class H> struct Sig :
	public FunType<F,G,H,typename XmapHelper<F,G,H>::ResultType> {};

      template <class F, class G, class H>
      typename Sig<F,G,H>::ResultType
      operator()(const F &f,const G &g, const H& h) const
       {
	 return compose(f,compose(h,g));
       }

    };

    template <class F, class G, class H>
    struct XmapHelper2 {
      typedef typename RT<Compose,F,typename RT<Apply2FG,H,G>::ResultType>::ResultType
              ResultType;
    };
    struct XXmap2
    {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("Xmap2");
      }
#endif

      template <class F, class G, class H> struct Sig :
	public FunType<F,G,H,typename XmapHelper2<F,G,H>::ResultType> {};

      template <class F, class G, class H>
      typename Sig<F,G,H>::ResultType
      operator()(const F &f,const G &g, const H& h) const
       {
	 return compose(f,apply2fg(h,g));
       }

    };
  
}
  typedef Full2<impl::XCoFmap>      CoFmap;
  typedef Full2<impl::XContraFmap>  ContraFmap;
  typedef Full2<impl::XContraFmap2> ContraFmap2;
  typedef Full3<impl::XXmap>        Xmap;
  typedef Full3<impl::XXmap2>       Xmap2;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN CoFmap      cofmap;
  FCPP_MAYBE_EXTERN ContraFmap  contrafmap;
  FCPP_MAYBE_EXTERN ContraFmap2 contrafmap2;
  FCPP_MAYBE_EXTERN Xmap        xmap;
  FCPP_MAYBE_EXTERN Xmap2       xmap2;
  FCPP_MAYBE_NAMESPACE_CLOSE

#endif //FCPP152

  ////////////////////////////////////////////////////////////////////////
  // Helper functions for the use of Applicative Functors.
  // This is an adaption of the code for inferrable monads.
  ////////////////////////////////////////////////////////////////////////

template <class Rep> struct ThisTypeIsNotAnInstanceOfAnApplicativeFunctor {};
template <class T> struct ApplicativeError { inline static void error() {} };
template <class T>
struct ApplicativeError<ThisTypeIsNotAnInstanceOfAnApplicativeFunctor<T> > {};

template <class Rep> struct ApplicativeTraitsSpecializer {
  static const bool isApplicative = false;
  typedef ThisTypeIsNotAnInstanceOfAnApplicativeFunctor<Rep> Applicative;
  typedef Rep Type;  // To assist in getting one behaviour for star
  typedef Rep ReturnType;
  static ReturnType pure(const Rep& a) { return a; }
  typedef void Test;
};

template <class Rep> struct ApplicativeTraits {
   typedef typename ApplicativeTraitsSpecializer<Rep>::Applicative Applicative;
   //Type is the underlying type (always)
   typedef typename ApplicativeTraitsSpecializer<Rep>::Type Type;
   inline static void ensure_is_instance_of_applicative_functor() {
      ApplicativeError<Applicative>::error();
   }
   static const bool isApplicative = ApplicativeTraitsSpecializer<Rep>::isApplicative;
};

  namespace impl {
   template <class A, class K> struct StarHelper;
   template <bool b> struct ApplicativeError {};
   template <> struct ApplicativeError<true> { static inline void error() {} };
   template <class A, class K>
   struct ApplicativeHelper {
      typedef typename ApplicativeTraits<A>::Applicative ApplicativeA;
      typedef typename ApplicativeA::template UnRep<A>::Type AA;
      typedef typename ApplicativeA::Star::template Sig<A,K>::ResultType ResultType;
      typedef typename ApplicativeTraits<K>::Applicative ApplicativeK;
      typedef typename ApplicativeTraits<ResultType>::Applicative ApplicativeB;
      inline static void ensure_a_and_k_are_in_the_same_applicative_instance() {
         ApplicativeError<Conversion<ApplicativeA,ApplicativeB>::sameType>::error();
      }
   };

  ////////////////////////////////////////////////////////////////////////
  // This allows star to be inferred for the MaybeA applicative functor.
  //
  // Idea: Use either argument to infer pure for the other one!
  //       This would allow such things as:
  //
  //       star( pure(plus(2)), just(4))
  //
  //       rather than this which already works.
  //
  //       star ((MaybeA::pure()(plus(2))), just(4))
  //
  // These have now been implemented using a series of helper structs.
  // These are in order with the deepest level first.
  // I have attempted to keep the top level code as clean as possible.
  ////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////
    // I want this to detect whether one of the arguments to star
    // is an applicative functor or a suitable type.
    // If the other argument is pure then convert it.
    // e.g.
    // star (pure inc, just(2) )         -> star(MaybeA::pure()(inc),just(2))
    // star (MaybeA::pure()(inc),pure 2) -> star(MaybeA::pure()(inc),just(2))
    //
    // If this works it could also be inferred within
    // an applicative functor's star as well.
    //
    // Note that Applicative::pure applies a functor and could be useful.
    //
    ///////////////////////////////////////////////////////////////////////
    template <class A, class K>
    struct ChooseApplicative
    {
      // What I need here is to choose the one which is applicative.
      // This is a compile time calculation.
      // It is assumed that A is more likely to be applicative.
      // If K is not applicative, A is assumed to be applicative.
      // If neither are applicative this will fail.
      // A good result is passed to StarTypeHelper (next) to sort out
      // the inferred types.
         typedef typename ApplicativeTraits<A>::Applicative AA;
         typedef typename ApplicativeTraits<K>::Applicative AK;
#ifndef FCPP_ARDUINO
      // See C++ Template Metaprogramming p.65 for the source of this.
      // Test is given the type void in the default case, to force the fail.
         typedef typename boost::mpl::eval_if<
           boost::is_void<typename ApplicativeTraitsSpecializer<K>::Test>,
           AA, AK >::type Applicative;
#else
	   // On the Arduino I don't have boost available.
       // I am going to follow the suggestion above and make the answer AA for now.
	   // This is going to fail sometimes.
	      typedef AA Applicative;
#endif
    };

    ////////////////////////////////////////////////////////////////
    // StarTypeHelper sorts out applicative types.
    ////////////////////////////////////////////////////////////////

    template <class A, class K>
    struct StarTypeHelper {
         typedef typename ChooseApplicative<A,K>::Applicative Applicative;
         typedef typename ApplicativeTraits<A>::Type AType;
         typedef typename ApplicativeTraits<K>::Type KType;
         typedef typename Applicative::template Rep<AType>::Type AA;
         typedef typename Applicative::template Rep<KType>::Type KK;
      // Note the use of the Applicative::Star's Sig to deduce the ResultType.
         typedef typename Applicative::Star::template Sig<AA,KK>::ResultType ResultType;
    };

    ////////////////////////////////////////////////////////////////////
    //  struct StarHelper
    // This replaces ApplicativeHelper and give the same result
    // for two arguments neither of which is pure(x).
    // For (a,pure(x)) it will use the type of a.
    // for (pure(f),b) it will use the type of b.
    // for (a,b) it will us Ids on both (not yet available).
    //
    // Note the availability of Applicative::pure()(f,x)
    //
    // The Type calculation needs to be like this:
    // Given A and K which can be applicative or not.
    // if both A and K are applicative -> use K
    // if only A       is  applicative -> use A
    // if only K       is  applicative -> use K
    //
    // The return type is the same in each case.
    //
    // StarHelper works with Applicative types sorted out already.
    //
    ////////////////////////////////////////////////////////////////////

   template <class A, class K> struct StarHelper
   {
     //typedef typename ApplicativeTraits<A>::Type AType;
     //typedef typename ApplicativeTraits<K>::Type KType;
         typedef typename StarTypeHelper<A,K>::AA AA;
         typedef typename StarTypeHelper<A,K>::KK KK;
         typedef typename ApplicativeHelper<AA,KK>::ApplicativeA ApplicativeA;
       //Return type adjusted
       //typedef typename StarTypeHelper<A,K>::ResultType ResultType;
         typedef typename StarTypeHelper<AA,KK>::ResultType ResultType;

         ResultType operator()(const A& a, const K& k ) const {
	   // This test disabled for ListA.
	   // ApplicativeHelper<A,K>::ensure_a_and_k_are_in_the_same_applicative_instance();
	   return ApplicativeA::star()(a,k);
	   // Attempt to apply pure now we know what it is. Fails.
	   //return ApplicativeA::star()(ApplicativeA::pure()(a),k);
         }
   };

  struct XStar {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("star");
   }
#endif
    // This is not consistent.
    // The ResultType is calculated from StarHelper<A,K> with A and K.
    // The actual return comes from StarHelper<AA,KK>().
    // What is going on?
    // Result type now ammended above to match what is done later.
    // There is still a problem with ListA.
     template <class A, class K>
     struct Helper {
         typedef typename StarHelper<A,K>::ResultType ResultType;
         ResultType operator()(const A& a, const K& k ) const {
            // StarTypeHelper sorts out the applicative types.
            typedef typename StarTypeHelper<A,K>::KK KK;
            typedef typename StarTypeHelper<A,K>::AA AA;
            // These are not needed as it gets sorted out lower down.
            //AA aa = ApplicativeTraitsSpecializer<A>::pure(a);
            //KK kk = ApplicativeTraitsSpecializer<K>::pure(k);
            // StarHelper works with Applicative types.
            return StarHelper<AA,KK>()(a,k);
         }
     };

      template <class A, class K> struct Sig
         : public FunType<A,K,typename Helper<A,K>::ResultType> {};
   
      template <class A, class K>
      typename Sig<A,K>::ResultType
      operator()( const A& a, const K& k ) const {
          return Helper<A,K>()(a,k);
      }
   };

  ////////////////////////////////////////////////////////////////////////
  // I have made pure do the same thing as Id.
  // I can then work on the context inside XStar to change the
  // result. This works when it is used with star but not otherwise.
  // This provides in those cases the wrong behaviour.
  // The solution is to use pureA<Functor>() or Functor::pure() instead.
  // e.g. MaybeA::pure()(inc (3));
  // See examples in unify.cpp for Applicative Functor Law 4
  //         pure f <*> pure x = pure ( f x )
  // where the RHS has pure with no star, so the type cannot be deduced.
  ////////////////////////////////////////////////////////////////////////

  struct XPure {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("Pure ");
   }
#endif
      template <class A> struct Sig
      : public FunType<A,A> {};
   
      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a) const {
          return id(a);
      }
   };

  struct XPureM {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("PureM ");
   }
#endif
    // Copied from ListA Pure.
      template <class A> struct Sig : public FunType<A,Maybe<A> > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
	// OddList is turned to List as the result is returned.
	return just(x);
      }
   };

  struct XPureE {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("PureE ");
   }
#endif
    // Copied from ListA Pure.
      template <class A> struct Sig : public FunType<A,Either<A> > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
	// OddList is turned to List as the result is returned.
	return right(x);
      }
   };

  struct XPureL {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("PureL ");
   }
#endif
    // Copied from ListA Pure.
      template <class A> struct Sig : public FunType<A,List<A> > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
	// OddList is turned to List as the result is returned.
	return cons(x,NIL);
      }
   };


  struct XPure2 {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("Pure2 ");
   }
#endif
    template <class F, class A> struct Sig
	: public FunType<F,A,typename RT<F,A>::ResultType> {};
   
    template <class F, class A>
      typename Sig<F,A>::ResultType
      operator()(const F& f, const A& a) const {
          return f(a);
      }
   };

  }

typedef Full2<impl::XStar> Star;
typedef Full1<impl::XPure> Pure;
typedef Full1<impl::XPureM> PureM;
typedef Full1<impl::XPureE> PureE;
typedef Full1<impl::XPureL> PureL;
typedef Full2<impl::XPure2> Pure2;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Pure pure;
FCPP_MAYBE_EXTERN PureM pureM;
FCPP_MAYBE_EXTERN PureE pureE;
FCPP_MAYBE_EXTERN PureL pureL;
FCPP_MAYBE_EXTERN Pure2 pure2;
FCPP_MAYBE_EXTERN Star star;
FCPP_MAYBE_NAMESPACE_CLOSE

/////////////////////////////////////////////////////////////////////////////
// star2( a, k1, k2) is the case where a is a two argument functoid.
/////////////////////////////////////////////////////////////////////////////
 namespace impl {
  struct XStar2 {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("star2 ");
   }
#endif
    template <class A, class K1,class K2>
     struct Helper {
         typedef typename StarHelper<A,K1>::ResultType AK1;
         typedef typename StarHelper<AK1,K2>::ResultType ResultType;
         ResultType operator()(const A& a, const K1& k1,const K2& k2 ) const {
            // StarTypeHelper sorts out the applicative types.
            typedef typename StarTypeHelper<A,K1>::AA AA1;
            typedef typename StarTypeHelper<A,K1>::KK KK1;
            typedef typename StarTypeHelper<AK1,K2>::AA AA2;
            typedef typename StarTypeHelper<AK1,K2>::KK KK2;
            return StarHelper<AA2,KK2>()(StarHelper<AA1,KK1>()(a,k1),k2);
         }
     };

    template <class A, class K1, class K2> struct Sig
      : public FunType<A,K1,K2,typename Helper<A,K1,K2>::ResultType> {};
   
    template <class A, class K1, class K2>
      typename Sig<A,K1,K2>::ResultType
      operator()( const A& a, const K1& k1, const K2& k2 ) const {
      return Helper<A,K1,K2>()(a,k1,k2);
      }
   };
 }
typedef Full3<impl::XStar2> Star2;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Star2 star2;
FCPP_MAYBE_NAMESPACE_CLOSE

/////////////////////////////////////////////////////////////////////////////
// star2a( a1, a2, k) is the case where a1 and a2 are one argument functoids.
/////////////////////////////////////////////////////////////////////////////
 namespace impl {
  struct XStar2a {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("star3 ");
   }
#endif
    template <class A1, class A2,class K>
     struct Helper {
         typedef typename StarHelper<A2,K>::ResultType A2K;
         typedef typename StarHelper<A1,A2K>::ResultType ResultType;
         ResultType operator()(const A1& a1, const A2& a2,const K& k ) const {
            // StarTypeHelper sorts out the applicative types.
            typedef typename StarTypeHelper<A2,K>::KK KK;
            typedef typename StarTypeHelper<A2,K>::AA AA2;
            typedef typename StarTypeHelper<A1,A2K>::AA AA1;
            return StarHelper<AA1,A2K>()(a1,StarHelper<AA2,KK>()(a2,k));
         }
     };

    template <class A1, class A2, class K> struct Sig
      : public FunType<A1,A2,K,typename Helper<A1,A2,K>::ResultType> {};
   
    template <class A1, class A2, class K>
	typename Sig<A1,A2,K>::ResultType
      operator()( const A1& a1, const A2& a2, const K& k ) const {
      return Helper<A1,A2,K>()(a1,a2,k);
      }
   };
 }
typedef Full3<impl::XStar2a> Star2a;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Star2a star2a;
FCPP_MAYBE_NAMESPACE_CLOSE

  ////////////////////////////////////////////////////////////////////
  // pureA<Functor>() and starA<Functor>() implemented as in Monads.
  ////////////////////////////////////////////////////////////////////
namespace impl {
   template <class Functor>
   struct XPureA {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Functor>::has_name) {
       Functor f; // Need to create an object for the call.
       temp = std::string("PureA<");
       temp += fcpp::traits::debug_traits<Functor>::name(f);
       temp += std::string(">()");
     } else {
       temp = std::string("PureA");
     }
     return temp;
   }
#endif
      template <class A> struct Sig
      : public FunType<A,typename Functor::Pure::template Sig<A>::ResultType> {};

      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a ) const {
         return Functor::pure()(a);
      }
   };
}
template <class Functor> Full1<impl::XPureA<Functor> > pureA()
{ return makeFull1( impl::XPureA<Functor>() ); }
template <class Functor> struct PureA
{ typedef Full1<impl::XPureA<Functor> > Type; };

namespace impl {
   template <class Functor>
   struct XStarA {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Functor>::has_name) {
       Functor f; // Need to create an object for the call.
       temp = std::string("StarA<");
       temp += fcpp::traits::debug_traits<Functor >::name(f);
       temp += std::string(">()");
     } else {
       temp = std::string("StarA");
     }
     return temp;
   }
#endif
      template <class A, class K> struct Sig : public FunType<A,K,
         typename Functor::Star::template Sig<A,K>::ResultType> {};
   
      template <class A, class K>
      typename Sig<A,K>::ResultType
      operator()( const A& a, const K& k ) const {
         return Functor::star()(a,k);
      }
   };
}
template <class Functor> Full2<impl::XStarA<Functor> > starA()
{ return makeFull2( impl::XStarA<Functor>() ); }
template <class Functor> struct StarA
{ typedef Full2<impl::XStarA<Functor> > Type; };

////////////////////////////////////////////////////////////////
// This returns a NOTHING with the correct type for the context.
////////////////////////////////////////////////////////////////
  namespace impl {
    struct XNothing {
 #ifdef FCPP_DEBUG
     std::string name() const
     {
       return std::string("Nothing");
     }
#endif
      template <class T> struct Sig
        : public FunType<T,Maybe<T> > {};
      template <class T>
      typename Sig<T>::ResultType operator()( const T& x ) const {
          return Maybe<T>();
      }
   };
  }
typedef Full1<impl::XNothing> Nothing;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Nothing nothing;
FCPP_MAYBE_NAMESPACE_CLOSE

  /////////////////////////////////////////////////////////////////////////
  // Applicative - see p.228
  //
  // class (Functor f) => Applicative f where
  //   pure :: a -> f a
  //  (<*>) :: f (a -> b) -> f a -> f b
  //
  // Note: fmap :: (a -> b) -> f a -> f b
  //
  // I need a name for <*> which I will call 'star'
  // This is difficult to construct in general as what is needed
  // is a way to get to (a -> b) from within f (a -> b)
  // (I think that this is a comonad!!)
  //
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  // MaybeA is an applicable version of Maybe.
  //
  // The most significant aspect of this is the idea to store
  // functoids (functors) in Maybe types and then apply them to other
  // Maybe objects.
  //
  // This works (see examples) but the grammar is clunky.
  // I have now made this inferrable for 'star'.
  // I want to do the same for 'pure' if a context can be established.
  // It turns out that this can be done for the context established by
  // 'star' but not otherwise. This means that 'pure f x'
  // has to be done using pureA<Functor>() or Functor::pure() instead.
  // In those contexts 'pure' defaults to 'id' which is not what is needed.
  //
  // There is also the alternative of starA<MaybeA>() following the example
  // of FC++ bindM<MaybeM>() for the MaybeM monad.
  //
  /////////////////////////////////////////////////////////////////////////

 struct MaybeA : public Inferrable<MaybeA,Maybe>
 {
 // added to make this inferrable. Now done through inheriting Inferrable
 //template <class A>  struct Rep   { typedef Maybe<A> Type; };
 //template <class MA> struct UnRep { typedef typename MA::ElementType Type; };
#ifdef FCPP_DEBUG
     std::string name() const
     {
       return std::string("MaybeA");
     }
#endif
     // pure is the same as unit.
      typedef Just Pure;
      static Pure& pure() {static Pure f; return f;}
#ifdef FCPP_UNIFY
      // This imports much of the innards of MaybeM.
   typedef Just Unit;
   static Unit& unit() {static Unit f; return f;}

   struct XBind {
#ifdef FCPP_DEBUG
         std::string name() const
         {
            return std::string("MaybeA::Bind");
         }
#endif
      template <class M, class K> struct Sig : public FunType<M,K,
         typename RT<K,typename M::ElementType>::ResultType> {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         if( m.is_nothing() )
            return NOTHING;
         else
            return k( m.value() );
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}

  // This is not needed in every functoid as I have now
  // defined the join for inferrable monads in terms of
  // the monad internal bind rather than using join.
   struct XJoin {
#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("MaybeA::Join");
   }
#endif
      template <class Z> struct Sig : public FunType<Z,
         typename RT<typename ::fcpp::BindM<MaybeM>::Type,Z,Id>::ResultType> {};
      template <class Z>
      typename Sig<Z>::ResultType
      operator()( const Z& z ) const {
         return bindM<MaybeM>()( z, id );
      }
   };
   typedef Full1<XJoin> Join;
   static Join& join() {static Join f; return f;}

   typedef AUniqueTypeForNothing Zero;
   static Zero& zero() {static Zero f; return f;}
  //   static Zero zero;

   struct XPlus {
#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("MaybeA::Plus");
   }
#endif
      template <class MA, class MA2> struct Sig : public FunType<MA,MA2,MA> {};
      template <class MA, class MA2>
      typename Sig<MA,MA2>::ResultType
      operator()( const MA& x, const MA2& y ) const {
	// It turns out there is reason in this.
	// It is not arithmetic at all.
        // See Real World Haskell p.362
            if( x.is_nothing() )
               return y;
	    else
               return x;
      }
   };
   typedef Full2<XPlus> Plus;
  //   static Plus plus;
   static Plus& plus() {static Plus f; return f;}

#endif

  // I would like this internal star to be able to infer argument types.
  // I have not figured out yet how to do this.
  // I have to be careful about this as the code here for
  // Sig is used in the 'star' operation above.
  // I need to avoid circular dependencies.
      struct XStar {
#ifdef FCPP_DEBUG
         std::string name() const
         {
            return std::string("MaybeA::Star");
         }
#endif
        // Note: e.g typename MB::ElementType B;
        // used to extract type of what is in Maybe<T>
        template <typename MF,typename  MA>
        struct Sig : public FunType<MF,MA,
          Maybe<typename RT<typename MF::ElementType,
            typename MA::ElementType>::ResultType> > {};

        template <typename MF, typename MA>
        typename Sig<MF,MA >::ResultType
        operator() (const MF& mf,const MA& ma) const
        {
          //typedef Maybe<A> MA;
          typedef typename Sig<MF,MA>::ResultType MB;
          typedef typename MB::ElementType B;
          if (!mf.is_nothing() && ! ma.is_nothing() ) {
            return Maybe<B>(mf.value()(ma.value()));
          } else return NOTHING;
        }
      };
      typedef Full2<XStar> Star;
      static Star& star() {static Star f; return f;}
      typedef MaybeA type; // for boost::is_void<T>
    };

template <class T> struct ApplicativeTraitsSpecializer<Maybe<T> > {
    static const bool isApplicative = true;
    typedef MaybeA Applicative;
    typedef T Type;
    typedef Maybe<T> ReturnType;
  // This overload solves a lot of problems.
    static ReturnType pure(const Maybe<T>& a) { return a; }
    static ReturnType pure(const T& a) { return MaybeA::pure()(a); }
    typedef T Test; // in contrast to void defined in the default.
};

template <class T> struct InferrableTraitsSpecializer<Maybe<T> > {
    static const bool isInferrable = true;
    typedef MaybeA InferrableFunctor;
    typedef T Type;
    typedef Maybe<T> ReturnType;
};

template <> struct InferrableNameTraits<MaybeA> {
   static const bool isInferrableName = true;
   typedef MaybeA InferrableName;
   template <typename T> struct Rep
   { typedef typename MaybeA::Rep<T>::Type Type; };
};

// Attempt to do this on MaybeA itself.
// This is a trick as the type T is not known.
template <> struct InferrableTraitsSpecializer<MaybeA > {
    static const bool isInferrable = true;
    typedef MaybeA InferrableFunctor;
    // Not used at present.
    template <typename T> struct Rep
    { typedef typename MaybeA::Rep<T>::Type Type;
      typedef T ElementType;
    };
    typedef MaybeA Type; // I want to make this T but it is not known.
};

// This is in here as a demo.
// MaybeM does not yet inherit Inferrable so the other traits
// are not yet active.
// If I was to do this I would have to move the definitions
// to somewhere where MaybeM can see them.
// For the moment I will leave the old code as it is and
// change MaybeA to do the work of MaybeM as well.
template <> struct InferrableNameTraits<MaybeM> {
   static const bool isInferrableName = true;
   typedef MaybeM InferrableName;
   template <typename T> struct Rep
   { typedef typename MaybeM::Rep<T>::Type Type; };
};


  ///////////////////////////////////////////////////////////////////
  // Functor Transformer MaybeF puts Maybe on the inside.
  // Usage is MaybeF<Functor>
  // which is a version of MaybeT transformer for monads.
  // I want to explore using FCPP_UNIFY to make this do the same
  // as MaybeF. I am going to leave this for now.
  ///////////////////////////////////////////////////////////////////

template <typename Functor>
struct MaybeF
{
  // Add these by hand from Inferrable.
   static bool const isInferrableFunctor = true;
   typedef MaybeF<Functor> InferrableFunctor;

  // This is the type of Functor::Rep<A> for some type A.
  // Note that this is used in both Rep and UnRep.
   template <class A> struct FRep {
     typedef typename Functor::template Rep<A>::Type Type;
   };

   // This is the ultimate type of the transformed functor Functor<Maybe<A> >.
   template <class A>  struct Rep   {
     //typedef typename FRep<Maybe<typename A> >::Type Type;
     typedef typename FRep<Maybe<typename A::ElementType > >::Type Type;
   };

  // This gets back Maybe<A> from the transformed functor.
  // and then gets back the type from that.
  // ElementType defines the lowest type.
  // Type defines the original functor type.
   template <class FMA> struct UnRep {
     typedef typename Functor::template UnRep<FMA>::Type MType;
     typedef typename MType::ElementType ElementType;
     typedef typename FRep<ElementType>::Type Type;
   };

#ifdef FCPP_DEBUG
   std::string name() const
   {
     if (fcpp::traits::debug_traits<Functor>::has_name)
       {
         Functor f; // Need to create an object for the call.
	 std::string temp = std::string("MaybeF<")
            + fcpp::traits::debug_traits<Functor>::name(f)
	    + std::string("> ");
         return temp;
       } else
     return std::string("MaybeF<Functor>");
   }
#endif

  // Note that the name of the functor is returned in a call to name().
  // This is because it is known in the trait but not here.
  struct XPure {
#ifdef FCPP_DEBUG
      std::string name(const std::string& functor) const
      {
         return std::string("MaybeF<") + functor + std::string(">::Pure");
      }
#endif
   template <class A> struct Sig : public FunType<A,typename Rep<A>::Type > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
	return pureA<Functor>()(x);
      }
   };
   typedef Full1<XPure> Pure;
   static Pure& pure() {static Pure f; return f;}

   struct XStar {
#ifdef FCPP_DEBUG
     std::string name(const std::string& functor) const
      {
         return std::string("MaybeF<") + functor + std::string(">::Star");
      }
#endif
     template <class A, class K> struct Sig : public FunType<A,K,
     typename RT<Map,typename A::ElementType,K>::ResultType> {};
     template <class A,class K>
     typename Sig<A,K>::ResultType
     //operator()( const typename Rep<A>::Type& m, const K& k ) const {
     operator()( const A& m, const K& k ) const {
       // This needs some elaboration to deal with new
       // failing cases generated by the operation of k.
       //return liftM<Monad>()(liftM<MaybeM>()(k))(m);
       typedef typename RT<Map,typename A::ElementType,K>::ResultType R;
       typename A::ElementType ma = m.value();
       typename K::ElementType mk = k.value();
       R r = Functor::star()(ma,mk);
       return just(r);// Maybe<R>(); // This was incomplete.
      }
   };
   typedef Full2<XStar> Star;
   static Star& star() {static Star f; return f;}

   struct XStarF {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("MaybeF<Functor>::StarF");
      }
#endif
     template <class A, class K> struct Sig : public FunType<A,K,
     typename RT<Map,typename A::ElementType,K>::ResultType> {};
     template <class A,class K>
     typename Sig<A,K>::ResultType
     //operator()( const typename Rep<A>::Type& m, const K& k ) const {
     operator()( const A& m, const K& k ) const {
       // This needs some elaboration to deal with new
       // failing cases generated by the operation of k.
       //return liftM<Monad>()(liftM<MaybeM>()(k))(m);
       typedef typename RT<Map,typename A::ElementType,K>::ResultType R;
       typename A::ElementType ma = m.value();
       typename K::ElementType mk = k.value();
       R r = Functor::star()(ma,mk);
       return Maybe<R>();
      }
   };
   typedef Full2<XStarF> StarF;
   static StarF& starf() {static StarF f; return f;}

};

// template template parameter needed, see C++ Templates p.50
template <template < class ELEM> class M, class T>
 struct ApplicativeTraitsSpecializer<MaybeF<M<T> > > {
  typedef MaybeF<M<T> > Applicative;
  typedef Maybe<T> Type;
  typedef M<Maybe<T> > ReturnType;
  typedef Maybe<T> Test;
};

// Attempt to do this on MaybeF<Functor>.
// This is a trick as the type T is not known.
  template <class Functor> struct InferrableTraitsSpecializer<MaybeF<Functor> >
  {
    static const bool isInferrable = true;
    typedef MaybeF<Functor> InferrableFunctor;
    // Not used at present.
    template <typename T> struct Rep
    { typedef typename MaybeF<Functor>::template Rep<T>::Type Type;
      typedef T ElementType;
    };
    typedef MaybeF<Functor> Type; // I want to make this T but it is not known.
};

  /////////////////////////////////////////////////////////////////////////
  // EitherA is an applicable version of Either.
  /////////////////////////////////////////////////////////////////////////

struct EitherA : public Inferrable<EitherA,Either> {
  // added to make this inferrable. Now done through inheriting Inferrable
  //template <class A>  struct Rep   { typedef Either<A> Type; };
  //template <class EA> struct UnRep { typedef typename EA::ElementType Type; };
#ifdef FCPP_DEBUG
     std::string name() const
     {
       return std::string("EitherA");
     }
#endif

      typedef Right Pure;
      static Pure& pure() {static Pure f; return f;}

#ifdef FCPP_UNIFY
   typedef Right Unit;
   static Unit& unit() {static Unit f; return f;}
  //   static Unit unit;

   struct XBind {
#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("EitherA::Bind");
   }
#endif
      template <class M, class K> struct Sig : public FunType<M,K,
         typename RT<K,typename M::ElementType>::ResultType> {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         if( m.is_error() )
	   return ( EitherError(m.left()) );
         else
            return k( m.right() );
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}
  //   static Bind bind;

   typedef AUniqueTypeForNothing Zero;
   static Zero& zero() {static Zero f; return f;}
  //   static Zero zero;

   struct XPlus {
#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("EitherA::Plus");
   }
#endif
      template <class EA, class EA2> struct Sig : public FunType<EA,EA2,EA> {};
      template <class EA, class EA2>
      typename Sig<EA,EA2>::ResultType
      operator()( const EA& x, const EA2& y ) const {
	// It turns out there is reason in this.
	// It is not arithmetic at all.
        // See Real World Haskell p.362
        if( x.is_error() )
            return y;
        else
	    return x;
      }
   };
   typedef Full2<XPlus> Plus;
  //static Plus plus;
   static Plus& plus() {static Plus f; return f;}
#endif

      struct XStar {
#ifdef FCPP_DEBUG
         std::string name() const
         {
            return std::string("EitherA::Star");
         }
#endif
        // Note: e.g typename MB::ElementType B;
        // used to extract type of what is in Maybe<T>
        template <typename MF,typename  MA>
        struct Sig : public FunType<MF,MA,
          Either<typename RT<typename MF::ElementType,
            typename MA::ElementType>::ResultType> > {};

        //template <typename MF,typename A>
        //struct Sig<MF,Maybe<A> > : public FunType<MF,Maybe<A>,
        //   Maybe<typename RT<typename MF::ElementType,
        //    A>::ResultType> > {};

        template <typename MF, typename MA>
        typename Sig<MF, MA >::ResultType
        operator() (const MF& mf,const MA& ma) const
        {
          //typedef M MA;
          typedef typename Sig<MF,MA>::ResultType MB;
          typedef typename MB::ElementType B;
          if (!mf.is_error() && ! ma.is_error() ) {
            return Either<B>(mf.right()(ma.right()));
          } else { /* one or the other */
            if (mf.is_error()) return EitherError(mf.left());
            else return EitherError(ma.left());
          }
        }
      };
      typedef Full2<XStar> Star;
      static Star& star() {static Star f; return f;}
      typedef EitherA type; // for boost::is_void<T>
    };
    //  }

  //typedef Full1<impl::XEitherA> EitherA;
  //FCPP_MAYBE_NAMESPACE_OPEN
  //FCPP_MAYBE_EXTERN EitherA eitherA;
  //FCPP_MAYBE_NAMESPACE_CLOSE

template <class T> struct ApplicativeTraitsSpecializer<Either<T> > {
    static const bool isApplicative = true;
    typedef EitherA Applicative;
    typedef T Type;
    typedef Either<T> ReturnType;
  // This overload solves a lot of problems.
    static ReturnType pure(const Either<T>& a) { return a; }
    static ReturnType pure(const T& a) { return EitherA::pure()(a); }
    typedef T Test; // in contrast to void defined in the default.
};

template <class T> struct InferrableTraitsSpecializer<Either<T> > {
    static const bool isInferrable = true;
    typedef EitherA InferrableFunctor;
    typedef T Type;
    typedef Either<T> ReturnType;
};

template <> struct InferrableNameTraits<EitherA> {
   static const bool isInferrableName = true;
   typedef EitherA InferrableName;
   template <typename T> struct Rep
   { typedef typename EitherA::Rep<T>::Type Type; };
};

// Attempt to do this on EitherA itself.
// This is a trick as the type T is not known.
template <> struct InferrableTraitsSpecializer<EitherA > {
    static const bool isInferrable = true;
    typedef EitherA InferrableFunctor;
    // Not used at present.
    template <typename T> struct Rep
    { typedef typename EitherA::Rep<T>::Type Type;
      typedef T ElementType;
    };
    typedef EitherA Type; // I want to make this T but it is not known.
};

//////////////////////////////////////////////////////////////////////
// EitherF functor transformer following MaybeF
//////////////////////////////////////////////////////////////////////

template <typename Functor>
struct EitherF
{
  // Add these by hand from Inferrable.
   static bool const isInferrableFunctor = true;
   typedef EitherF<Functor> InferrableFunctor;

  // This is the type of Functor::Rep<A> for some type A.
  // Note that this is used in both Rep and UnRep.
   template <class A> struct FRep {
     typedef typename Functor::template Rep<A>::Type Type;
   };

   // This is the ultimate type of the transformed functor Functor<Maybe<A> >.
   template <class A>  struct Rep   {
     typedef typename FRep<Either<typename A::ElementType > >::Type Type;
   };

  // This gets back Either<A> from the transformed functor.
  // and then gets back the type from that.
  // ElementType defines the lowest type.
  // Type defines the original functor type.
   template <class FMA> struct UnRep {
     typedef typename Functor::template UnRep<FMA>::Type MType;
     typedef typename MType::ElementType ElementType;
     typedef typename FRep<ElementType>::Type Type;
   };

  // Attempt to do what I have done for monads.
  // It does not work.
  //typedef EitherF   OuterType; // Experiments for LiftT
  //typedef Functor   InnerType;


#ifdef FCPP_DEBUG
   std::string name() const
   {
     if (fcpp::traits::debug_traits<Functor>::has_name)
       {
         Functor f; // Need to create an object for the call.
	 std::string temp = std::string("EitherF<")
            + fcpp::traits::debug_traits<Functor>::name(f)
	    + std::string("> ");
         return temp;
       } else
     return std::string("EitherF<Functor>");
   }
#endif

  struct XPure {
#ifdef FCPP_DEBUG
      std::string name(const std::string& functor) const
      {
         return std::string("EitherF<") + functor + std::string(">::Pure");
      }
#endif
   template <class A> struct Sig : public FunType<A,typename Rep<A>::Type > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
	return pureA<Functor>()(x);
      }
   };
   typedef Full1<XPure> Pure;
   static Pure& pure() {static Pure f; return f;}

   struct XStar {
#ifdef FCPP_DEBUG
      std::string name(const std::string& functor) const
      {
         return std::string("EitherF<") + functor + std::string(">::Star");
      }
#endif
     template <class A, class K> struct Sig : public FunType<A,K,
     typename RT<Map,typename A::ElementType,K>::ResultType> {};
     template <class A,class K>
     typename Sig<A,K>::ResultType
     //operator()( const typename Rep<A>::Type& m, const K& k ) const {
     operator()( const A& m, const K& k ) const {
       // This needs some elaboration to deal with new
       // failing cases generated by the operation of k.
       //return liftM<Monad>()(liftM<MaybeM>()(k))(m);
       typedef typename RT<Map,typename A::ElementType,K>::ResultType R;
       typename A::ElementType ma = m.value();
       typename K::ElementType mk = k.value();
       R r = Functor::star()(ma,mk);
       return Either<R>();
      }
   };
   typedef Full2<XStar> Star;
   static Star& star() {static Star f; return f;}

};

// template template parameter needed, see C++ Templates p.50
template <template < class ELEM> class M, class T>
 struct ApplicativeTraitsSpecializer<EitherF<M<T> > > {
  typedef EitherF<M<T> > Applicative;
  typedef Either<T> Type;
  typedef M<Either<T> > ReturnType;
  typedef Either<T> Test;
};

// Attempt to do this on EitherF<Functor>.
// This is a trick as the type T is not known.
  template <class Functor> struct InferrableTraitsSpecializer<EitherF<Functor> >
  {
    static const bool isInferrable = true;
    typedef EitherF<Functor> InferrableFunctor;
    // Not used at present.
    template <typename T> struct Rep
    { typedef typename EitherF<Functor>::template Rep<T>::Type Type;
      typedef T ElementType;
    };
    typedef EitherF<Functor> Type; // I want to make this T but it is not known.
};

  ///////////////////////////////////////////////////////////////////
  // ListA Applicative operations on List<T>.
  // The functors in lists will have to be monomorphic.
  // The type will be Fun1<T,T> where T is the element type.
  // Otherwise the types are different and cannot be in a List type.
  ///////////////////////////////////////////////////////////////////
struct ListA : public Inferrable<ListA,List> {
 // template <class A> struct Rep { typedef List<A> Type; };
 // template <class MA> struct UnRep { typedef typename MA::ElementType Type; };

#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("ListA");
   }
#endif

#ifdef FCPP_UNIFY
   struct XUnit {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("ListA::Unit");
      }
#endif
      template <class A> struct Sig : public FunType<A,OddList<A> > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
         return cons(x,NIL);
      }
   };
   typedef Full1<XUnit> Unit;
   static Unit& unit() {static Unit f; return f;}

   struct XBind {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("ListA::Bind");
      }
#endif
      template <class M, class K> struct Sig : public FunType<M,K,
         typename RT<Concat,typename RT<Map,K,M>::ResultType>::ResultType> {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
	return concat( map(k,m) );
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}


   struct XBind_ {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("ListA::Bind_");
      }
#endif
      template <class M, class K> struct Sig : public FunType<M,K,
	typename RT<Concat,
        typename RT<Map,
        typename RT<Konst,K>::ResultType,M>::ResultType>::ResultType> {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
	return bind(m,konst(k));
	//return concat( map(konst(k),m) );
      }
   };
   typedef Full2<XBind_> Bind_;
   static Bind_& bind_() {static Bind_ f; return f;}

   typedef AUniqueTypeForNil Zero;
   static Zero& zero() {static Zero f; return f;}
  //   static Zero zero;

   typedef Cat Plus;   // cat is monad plus for lists
  //static Plus plus;
   static Plus& plus() {static Plus f; return f;}
 
#endif

   struct XPure {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("ListA::Pure");
      }
#endif
      template <class A> struct Sig : public FunType<A,List<A> > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
         return cons(x,NIL);
      }
   };
   typedef Full1<XPure> Pure;
   static Pure& pure() {static Pure f; return f;}

  // At first this only processed a single functor.
  // This has now been extended to a list of monomorphic functors.
  // These must be of the correct type structure for the input.
   struct XStar {
 #ifdef FCPP_DEBUG
         std::string name() const
         {
            return std::string("ListA::Star");
         }
#endif
     template <class A, class K>
      struct Sig : public FunType<A,K, typename RT<Map,typename A::ElementType,K>::ResultType> {};
     // typename RT<Concat,typename RT<Map,A,K>::ResultType>::ResultType> {};
      template <class A, class K>
      typename Sig<A,K>::ResultType
      operator()( const A& a, const K& k ) const {
        typename A::iterator ai;
        typename Sig<A,K>::ResultType result;
        for (ai= a.begin(); ai!=a.end(); ++ai)
        {
          result = cat(result,map(*ai)(k));
        }
        return result;
        //return map(head(a),k);
      }
   };
   typedef Full2<XStar> Star;
   static Star& star() {static Star f; return f;}

   typedef ListA type; // for boost::is_void<T>
};

template <class T> struct ApplicativeTraitsSpecializer<List<T> > {
    static const bool isApplicative = true;
    typedef ListA Applicative;
    typedef T Type;
    typedef List<T> ReturnType;
  // This overload solves a lot of problems.
    static ReturnType pure(const List<T>& a) { return a; }
    static ReturnType pure(const T& a) { return ListA::pure()(a); }
    typedef T Test; // in contrast to void defined in the default.
};

template <class T> struct InferrableTraitsSpecializer<List<T> > {
    static const bool isInferrable = true;
    typedef ListA InferrableFunctor;
    typedef T Type;
    typedef List<T> ReturnType;
};

template <> struct InferrableNameTraits<ListA> {
   static const bool isInferrableName = true;
   typedef ListA InferrableName;
   template <typename T> struct Rep
   { typedef typename ListA::Rep<T>::Type Type; };
};

// Attempt to do this on ListA itself.
// This is a trick as the type T is not known.
template <> struct InferrableTraitsSpecializer<ListA > {
    static const bool isInferrable = true;
    typedef ListA InferrableFunctor;
    // Not used at present.
    template <typename T> struct Rep
    { typedef typename ListA::Rep<T>::Type Type;
      typedef T ElementType;
    };
    typedef ListA Type; // I want to make this T but it is not known.
};

}

// Includes argument traits needed without FCPP_DEBUG
#include "functors_debug.h"

#endif
