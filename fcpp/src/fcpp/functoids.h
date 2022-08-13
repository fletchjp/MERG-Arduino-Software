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





}


#endif
