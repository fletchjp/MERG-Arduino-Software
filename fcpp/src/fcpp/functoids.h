//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FUNCTOIDS_DOT_H
#define FCPP_FUNCTOIDS_DOT_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///
/// Note: mconcat is only in Learn You a Haskell for Great Good! p.252.
///
/// Following Monads in monad.h what I need first are the free functions.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace fcpp {
	
/// Infer structure from the equivalent for Monad in monad.h
template <class Rep> struct ThisTypeIsNotAnInstanceOfAnInferrableMonoid {};
template <class T> struct MonoidError { inline static void error() {} };
template <class T>
struct MonoidError<ThisTypeIsNotAnInstanceOfAnInferrableMonoid<T> > {};
template <class Rep> struct MonoidTraitsSpecializer {
   typedef ThisTypeIsNotAnInstanceOfAnInferrableMonoid<Rep> Monoid;
};
template <class Rep> struct MonoidTraits {
   typedef typename MonoidTraitsSpecializer<Rep>::Monoid Monoid;
   inline static void ensure_is_instance_of_monoid() {
      MonoidError<Monoid>::error();
   }
};

//////////////////////////////////////////////////////////////////////
/// Free functions for Monoids
///    mempty  :: m
///    mappend :: m -> m -> m
///    mconcat :: [m] -> m
//////////////////////////////////////////////////////////////////////
/// mempty<Monoid>()();
///
/// mappend<Monoid>()(m1,m2);
/// mmappend(m1,m2) - now inferrable for Mstring.
///
///
/// mconcat<Monoid>()(lm) where lm has the type List<Monoid>
/// mmconcat(lm)        - now inferrable for Mstring.
//////////////////////////////////////////////////////////////////////

namespace impl {
  // I have used the example of IntZero in prelude.h line 3947 as a basis.
   template <class Monoid>
   struct XMempty : public CFunType<typename Monoid::Rep::Type> {
    using MonoidResultType = typename Monoid::Rep::Type;
       MonoidResultType
      operator()() const {
         return Monoid::mempty()();
      }

   };

   template <class Monoid>
   struct XMappend {

      template <class A, class B> struct Sig;
      // Force the monoids to have the same type
      template <class A> struct Sig<A,A>
      : public FunType<A,A,typename Monoid::Mappend::template Sig<A,A>::ResultType> {};

      template <class A>
      typename Sig<A,A>::ResultType
      operator()( const A& a1 , const A& a2) const {
         return Monoid::mappend()(a1,a2);
      }

   };

   template <class Monoid>
   struct XMconcat {
      template <class L> struct Sig
      : public FunType<L,typename Monoid::Mconcat::template Sig<L>::ResultType> {};

      template <class L>
      typename Sig<L>::ResultType
      operator()( const L& l ) const {
         return Monoid::mconcat()(l);
      }

   };



  }

template <class Monoid> Full0<impl::XMempty<Monoid> > mempty()
{ return makeFull0( impl::XMempty<Monoid>() ); }
template <class Monoid> struct Mempty
{ typedef Full0<impl::XMempty<Monoid> > Type; };

template <class Monoid> Full2<impl::XMappend<Monoid> > mappend()
{ return makeFull2( impl::XMappend<Monoid>() ); }
template <class Monoid> struct Mappend
{ typedef Full2<impl::XMappend<Monoid> > Type; };


template <class Monoid> Full1<impl::XMconcat<Monoid> > mconcat()
{ return makeFull1( impl::XMconcat<Monoid>() ); }
template <class Monoid> struct Mconcat
{ typedef Full1<impl::XMconcat<Monoid> > Type; };

////////////////////////////////////////////////////////////////////
// The next thing is to infer mmappend(m1,m2)
// based on bind in monad.h from line 206
// I have now done mmconcat(lm) as well.
////////////////////////////////////////////////////////////////////

namespace impl {
   template <bool b> struct AppendError {};
   template <> struct AppendError<true> { static inline void error() {} };
   template <class M, class K>
   struct AppendHelper {
      typedef typename MonoidTraits<M>::Monoid MonoidA;
      //typedef typename MonadA::template UnRep<M>::Type A;
      typedef typename MonoidA::Mappend::template Sig<M,K>::ResultType ResultType;
      typedef typename MonoidTraits<ResultType>::Monoid MonoidB;
      inline static void ensure_m_and_k_are_in_the_same_monoid_instance() {
         AppendError<Conversion<MonoidA,MonoidB>::sameType>::error();
      }
   };
   struct XMMappend {
      template <class M, class K> struct Sig
      : public FunType<M,K,typename AppendHelper<M,K>::ResultType> {};

      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         AppendHelper<M,K>::ensure_m_and_k_are_in_the_same_monoid_instance();
         typedef typename AppendHelper<M,K>::MonoidA MM;
         return MM::mappend()(m,k);
      }

   };
   template <class L>
   struct ConcatHelper {
      //EnsureListLike<L>();
      typedef typename L::ElementType M;
      typedef typename MonoidTraits<M>::Monoid MonoidA;
      //typedef typename MonadA::template UnRep<M>::Type A;
      typedef typename MonoidA::Mconcat::template Sig<M>::ResultType ResultType;
      typedef typename MonoidTraits<ResultType>::Monoid MonoidB;
      inline static void ensure_m_and_k_are_in_the_same_monoid_instance() {
         AppendError<Conversion<MonoidA,MonoidB>::sameType>::error();
      }
   };
   struct XMMconcat {
      template <class L> struct Sig
      : public FunType<L,typename ConcatHelper<L>::ResultType> {};

      template <class L>
      typename Sig<L>::ResultType
      operator()( const L& l ) const {
         //AppendHelper<M,K>::ensure_m_and_k_are_in_the_same_monoid_instance();
         typedef typename ConcatHelper<L>::MonoidA MM;
         return MM::mconcat()(l);
      }

   };
}

typedef Full2<impl::XMMappend> MMappend;
typedef Full1<impl::XMMconcat> MMconcat;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN MMappend mmappend;
FCPP_MAYBE_EXTERN MMconcat mmconcat;
FCPP_MAYBE_NAMESPACE_CLOSE

}

#endif
