//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_MONAD_DOT_H
#define FCPP_MONAD_DOT_H

namespace fcpp {

template <class Rep> struct ThisTypeIsNotAnInstanceOfAnInferrableMonad {};
template <class T> struct MonadError { inline static void error() {} };
template <class T>
struct MonadError<ThisTypeIsNotAnInstanceOfAnInferrableMonad<T> > {};
template <class Rep> struct MonadTraitsSpecializer {
   typedef ThisTypeIsNotAnInstanceOfAnInferrableMonad<Rep> Monad;
};
template <class Rep> struct MonadTraits {
   typedef typename MonadTraitsSpecializer<Rep>::Monad Monad;
   inline static void ensure_is_instance_of_monad() {
      MonadError<Monad>::error();
   }
};

//////////////////////////////////////////////////////////////////////
// If a monad has a type constructor which is not a type synonym, then
// we call it an "inferrable monad".  
// For any monad M, you can conjure up one of its functoids via, e.g. 
//    unitM<M>()     bindM<M>()
// whereas for an inferrable monad, you can just refer to e.g.
//    bind
// which will use the MonadTraitsSpecializer to infer "M" from "M a".
//
// (Section 4.3.2 in the Haskell Report explains that an instance of a
// type class's type constructor cannot be a type synonym.)
//////////////////////////////////////////////////////////////////////

namespace impl {
   template <class Monad>
   struct XUnitM {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if ( debug_traits_has_name<Monad>() ) {
       Monad m; // Need to create an object for the call.
       temp = std::string("UnitM<");
       temp += debug_traits_name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("UnitM");
     }
     return temp;
   }
#endif
      template <class A> struct Sig
      : public FunType<A,typename Monad::Unit::template Sig<A>::ResultType> {};

      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a ) const {
         return Monad::unit()(a);
	 //         return Monad::unit(a);
      }
   };
}
template <class Monad> Full1<impl::XUnitM<Monad> > unitM()
{ return makeFull1( impl::XUnitM<Monad>() ); }
template <class Monad> struct UnitM
{ typedef Full1<impl::XUnitM<Monad> > Type; };

namespace impl {
   template <class Monad>
   struct XBindM {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("BindM<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("BindM");
     }
     return temp;
   }
#endif
      template <class M, class K> struct Sig : public FunType<M,K,
         typename Monad::Bind::template Sig<M,K>::ResultType> {};

      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         return Monad::bind()(m,k);
	 //         return Monad::bind(m,k);
      }
   };
}
template <class Monad> Full2<impl::XBindM<Monad> > bindM()
{ return makeFull2( impl::XBindM<Monad>() ); }
template <class Monad> struct BindM
{ typedef Full2<impl::XBindM<Monad> > Type; };

namespace impl {
   template <class Monad>
   struct XBindM_ {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("BindM_<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("BindM_");
     }
     return temp;
   }
#endif
      template <class M, class K> struct Sig : public FunType<M,K,
	typename Monad::Bind::template Sig<M,
            typename RT<Konst,K>
            ::ResultType>::ResultType> {};
      //        typename RT<Ignore,typename RT<Const,K>
      //        ::ResultType>::ResultType>::ResultType> {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
	return Monad::bind()( m, konst(k) );
        //return Monad::bind()( m, ignore(const_(k)) );
	//         return Monad::bind( m, ignore(const_(k)) );
      }
   };
}
template <class Monad> Full2<impl::XBindM_<Monad> > bindM_()
{ return makeFull2( impl::XBindM_<Monad>() ); }
template <class Monad> struct BindM_
{ typedef Full2<impl::XBindM_<Monad> > Type; };

template <class Monad> typename Monad::Zero zeroM()
{ return Monad::zero(); }
// { return Monad::zero; }
template <class Monad> struct ZeroM
{
  typedef typename Monad::Zero Type;
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("ZeroM<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("ZeroM");
     }
     return temp;
     //  return std::string("ZeroM");
   }
#endif
};

namespace impl {
   template <class Monad>
   struct XPlusM {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("PlusM<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("PlusM");
     }
     return temp;
   }
#endif
      template <class MA, class MA2> struct Sig : public FunType<MA,MA2,
         typename Monad::Plus::template Sig<MA,MA2>::ResultType> {};
   
      template <class MA, class MA2>
      typename Sig<MA,MA2>::ResultType
      operator()( const MA& x, const MA2& y ) const {
	return Monad::plus()(x,y);
      }
   };
}
template <class Monad> Full2<impl::XPlusM<Monad> > plusM()
{ return makeFull2( impl::XPlusM<Monad>() ); }
template <class Monad> struct PlusM
{ typedef Full2<impl::XPlusM<Monad> > Type; };

namespace impl {
   template <bool b> struct BindError {};
   template <> struct BindError<true> { static inline void error() {} };
   template <class M, class K>
   struct BindHelper {
      typedef typename MonadTraits<M>::Monad MonadA;
      typedef typename MonadA::template UnRep<M>::Type A;
      typedef typename MonadA::Bind::template Sig<M,K>::ResultType ResultType;
      typedef typename MonadTraits<ResultType>::Monad MonadB;
      inline static void ensure_m_and_k_are_in_the_same_monad_instance() {
         BindError<Conversion<MonadA,MonadB>::sameType>::error();
      }
   };
   struct XBind {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("bind ");
   }
#endif
      template <class M, class K> struct Sig
      : public FunType<M,K,typename BindHelper<M,K>::ResultType> {};
   
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         BindHelper<M,K>::ensure_m_and_k_are_in_the_same_monad_instance();
         typedef typename BindHelper<M,K>::MonadA MM;
         return MM::bind()(m,k);
	 //         return MM::bind(m,k);
      }
   };
}
typedef Full2<impl::XBind> Bind;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Bind bind;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
   struct XBind_ {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("bind_ ");
   }
#endif
      template <class M, class K> struct Sig : public FunType<M,K,
      // I have changed this to ensure that the Bind is chosen from the correct
      // monad.
      //typename RT<Bind,M,typename RT<Konst,K>
        typename RT<typename BindHelper<M,K>::MonadA::Bind,M,typename RT<Konst,K>
      	::ResultType>::ResultType> {};
     //typename RT<Bind,M,typename RT<Ignore,typename RT<Const,
     // ::ResultType>::ResultType>::ResultType> {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
	// This is changed to replicate the code in XBind
	// rather than assuming that the monad can be inferred.
         BindHelper<M,K>::ensure_m_and_k_are_in_the_same_monad_instance();
         typedef typename BindHelper<M,K>::MonadA MM;
         return MM::bind()(m,konst(k));
	 // This assumes bind can be inferred.
	 //return bind( m, konst(k) );
	//return bind( m, ignore(const_(k)) );
      }
   };
}
typedef Full2<impl::XBind_> Bind_;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Bind_ bind_;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {

   template <class Monad>
   struct XMapM {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("MapM<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("MapM");
     }
     return temp;
   }
#endif
      template <class F, class M> struct Sig : FunType<F,M,
         typename RT<typename ::fcpp::BindM<Monad>::Type,M,typename LEType<
            LAM<LV<1>,CALL<typename ::fcpp::UnitM<Monad>::Type,
            CALL<F,LV<1> > > > >::Type>::ResultType> {};
      template <class F, class M>
      typename Sig<F,M>::ResultType
      operator()( const F& f, const M& m ) const {
         LambdaVar<1> A;
         return bindM<Monad>()( m, lambda(A)[ unitM<Monad>()[f[A]] ] );
      }
   };
}
template <class Monad> Full2<impl::XMapM<Monad> > mapM()
{ return makeFull2( impl::XMapM<Monad>() ); }
template <class Monad> struct MapM
{ typedef Full2<impl::XMapM<Monad> > Type; };

// If you are not against them you are forM!!
// mapM with the arguments switched.
// Added by John Fletcher (J.P.Fletcher@aston.ac.uk) October 2013
namespace impl {
   template <class Monad>
   struct XForM {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("ForM<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("ForM");
     }
     return temp;
   }
#endif
      template <class M, class F> struct Sig : FunType<M,F,
	 typename RT<typename ::fcpp::BindM<Monad>::Type,M,typename LEType<
            LAM<LV<1>,CALL<typename ::fcpp::UnitM<Monad>::Type,
            CALL<F,LV<1> > > > >::Type>::ResultType> {};
      template <class M, class F>
      typename Sig<M,F>::ResultType
      operator()( const M& m, const F& f ) const {
         LambdaVar<1> A;
         return bindM<Monad>()( m, lambda(A)[ unitM<Monad>()[f[A]] ] );
      }
   };
}
template <class Monad> Full2<impl::XForM<Monad> > forM()
{ return makeFull2( impl::XForM<Monad>() ); }
template <class Monad> struct ForM
{ typedef Full2<impl::XForM<Monad> > Type; };

// If you can't beat 'em, JoinM! :) :) :)
namespace impl {
   template <class Monad>
   struct XJoinM {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("JoinM<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("JoinM");
     }
     return temp;
   }
#endif
      template <class Z> struct Sig : public FunType<Z,
         typename RT<typename ::fcpp::BindM<Monad>::Type,Z,Id>::ResultType> {};
      template <class Z>
      typename Sig<Z>::ResultType
      operator()( const Z& z ) const {
         return bindM<Monad>()( z, id );
      }
   };
}
template <class Monad> Full1<impl::XJoinM<Monad> > joinM()
{ return makeFull1( impl::XJoinM<Monad>() ); }
template <class Monad> struct JoinM
{ typedef Full1<impl::XJoinM<Monad> > Type; };

//////////////////////////////////////////////////////////////////////
// join for inferrable monads.
//////////////////////////////////////////////////////////////////////

namespace impl {
   template <class Z>
   struct JoinHelper {
      typedef typename MonadTraits<Z>::Monad Monad;
      typedef typename Monad::Join::template Sig<Z>::ResultType ResultType;
   };

   struct XJoin {
#ifdef FCPP_DEBUG
      std::string name() const {
        return std::string("join ");
   }
#endif
      template <class Z> struct Sig : public FunType<Z,
      typename JoinHelper<Z>::ResultType> {};
   
      template <class Z>
      typename Sig<Z>::ResultType
      operator()( const Z& z) const {
         typedef typename JoinHelper<Z>::Monad MM;
         return MM::bind()(z,id);
	 //return MM::join()(z); works only if the Monad has join defined.
      }
   };
}
typedef Full1<impl::XJoin> Join;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Join join;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////

namespace fcpp_lambda {

// Gets is not really an LE, but making it appear so makes op,() and
// LEify work properly automagically.  So we fake it to make the
// implementation easier.
namespace exp {
   template <int i, class LE> struct Gets : public LEBase {
      static const int my_lv = i;
      LE exp; Gets( const LE& e ) : exp(e) { EnsureLE<LE>::go(); };
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Gets");
    }
#endif
   };
}
template <int i, class E>
exp::Gets<i,typename LEify<E>::Type>
operator<=( const LambdaVar<i>&, const E& e ) {
   return exp::Gets<i,typename LEify<E>::Type>( LEify<E>::go(e) );
}

//////////////////////////////////////////////////////////////////////
// Next on the agenda...
//    doM[ LE ]                 ==>  LE
//    doM[ LE, stmts ]          ==>  bind_[ LE, doM[stmts] ]
//    doM[ X<=LE, stmts ]  ==>  bind[ LE, lambda(X)[ doM[stmts] ] ]
// "doM" is an unfortunate name, as everything else monad-ly I've
// written uses the 'M' suffix to say "I'm a template, pass me a monad
// as a parameter, please", whereas this isn't true here.  But I still
// like "doM" better than "do_", and just "do" is a C++ keyword.

struct DoM_Lambdaoid {
   template <class Dummy, class FinalLE, class Stuff> struct Helper2;
   template <class D, class FinalLE>
   struct Helper2<D,FinalLE,NIL> {
      typedef FinalLE Result;
      static inline Result go( const FinalLE& le, const NIL& )
      { return le; }
   };
   template <class D, class FinalLE, class LE, class Rest>
   struct Helper2<D,FinalLE,CONS<LE,Rest> > {
      typedef typename LEType<CALL<Bind_,LE,FinalLE> >::Type Inner;
      typedef typename Helper2<D,Inner,Rest>::Result Result;
      static inline Result go( const FinalLE& le, const CONS<LE,Rest>& rest ) 
      { return Helper2<D,Inner,Rest>::go( bind_[ rest.head, le ], rest.tail ); }
   };
   template <class D, class FinalLE, int i, class LE, class Rest>
   struct Helper2<D,FinalLE,CONS<exp::Gets<i,LE>,Rest> > {
      typedef typename LEType<CALL<Bind,LE,LAM<LV<i>,FinalLE> > >::Type Inner;
      typedef typename Helper2<D,Inner,Rest>::Result Result;
      static inline Result 
      go( const FinalLE& le, const CONS<exp::Gets<i,LE>,Rest>& rest ) { 
         LambdaVar<i> X;
         return Helper2<D,Inner,Rest>::go( 
            bind[ rest.head.exp, lambda(X)[ le ] ],    rest.tail ); 
      }
   };

   template <class Dummy, class X> struct Helper;
   template <class D, class LE, class Rest>
   struct Helper<D,CONS<LE,Rest> > {
      typedef typename Helper2<D,LE,Rest>::Result Result;
      static inline Result go( const CONS<LE,Rest>& l ) 
      { return Helper2<D,LE,Rest>::go( l.head, l.tail ); }
   };

   template <class X> struct RT 
   { typedef typename DoM_Lambdaoid::Helper<int,typename
      LEListify<X>::Type>::Result Type; };

   template <class X>
   typename RT<X>::Type
   operator[]( const X& x ) const {
      typedef typename LEListify<X>::Type XP;
      return Helper<int,XP>::go( LEListify<X>::go(x) );
   }
};

template <class A, class B=Void, class C=Void, class D=Void> struct DOM;
template <int i, class E> struct GETS;

template <int i, class E>
struct LEType< GETS<i,E> > {
   typedef exp::Gets<i,typename LEify<typename LEType<E>::Type>::Type> Type;
};

template <class A, class B, class C, class D> 
struct LEType< DOM<A,B,C,D> > {
   typedef typename LET_LEListify<A,B,C,D>::Type L;
   typedef typename DoM_Lambdaoid::template RT<L>::Type Type;
};
template <class A, class B, class C>
struct LEType< DOM<A,B,C,Void> > {
   typedef typename LET_LEListify<A,B,C>::Type L;
   typedef typename DoM_Lambdaoid::template RT<L>::Type Type;
};
template <class A, class B>
struct LEType< DOM<A,B,Void,Void> > {
   typedef typename LET_LEListify<A,B>::Type L;
   typedef typename DoM_Lambdaoid::template RT<L>::Type Type;
};
template <class A>
struct LEType< DOM<A,Void,Void,Void> > {
   typedef typename LET_LEListify<A>::Type L;
   typedef typename DoM_Lambdaoid::template RT<L>::Type Type;
};

//////////////////////////////////////////////////////////////////////
// Finally, the mother-of-all-syntactic-sugar: comprehensions!
//    compM<M>()[ LE ]
//       ==>  unit<M>()[ LE ]
//(*) compM<M>()[ LE | LEa ]
//       ==>  bindM_[ LEa, lambda(X)[ unit<M>()[ LE ] ] ]
//(*) compM<M>()[ LE | X<=LEa ]
//       ==>  bindM[ LEa, lambda(X)[ unit<M>()[ LE ] ] ]
//    compM<M>()[ LE | LEa, stmts ]
//       ==>  bindM_[ LEa, comp<M>()[ LE | stmts ] ]
//    compM<M>()[ LE | X<=LEa, stmts ]
//       ==>  bindM[ LEa, lambda(X)[ comp<M>()[ LE | stmts] ] ]
//    compM<M>()[ LE | guard[LEa], stmts ]
//       ==>  if1[ LEa, comp<M>()[ LE | stmts ], zero<M>() ]
// The vert (|) will be a bit of a pain, as it binds tighter than the
// comma (but thankfully less tightly that <=), and so I'm gonna have to
// make it return a VertPair or something as a new "exp" type which
// operator,() can LEListify, and then have compM unwrap it on the back
// end.  Hmm, and just to ensure that we don't accidentally call C++'s
// built-in operator|(), I think I'll outlaw the first form of compM
// above, and require that there always be a vert and a right-hand-side
// in user calls.  Yup.
//
// (*) These lines are added because the first form is not allowed
//     and cover the cases where 'stmts' is nothing.
//     There is an example of this in testeither.cpp
//     John Fletcher 2016.

namespace exp {
   template <class LHS, class RHS>
   struct VertPair : public LEBase {
      LHS lhs;
      RHS rhs;
      VertPair( const LHS& l, const RHS& r ) : lhs(l), rhs(r)
      { EnsureLE<LHS>::go(); EnsureLE<RHS>::go(); }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("VertPair");
    }
#endif
   };
   template <class LHS, class RHS>
   VertPair<typename LEify<LHS>::Type,typename LEify<RHS>::Type>
   operator|( const LHS& l, const RHS& r ) {
      return VertPair<typename LEify<LHS>::Type,typename LEify<RHS>::Type>
         ( LEify<LHS>::go(l), LEify<RHS>::go(r) );
   }
   template <class BFLE> struct Guard : public LEBase
   { BFLE fun; Guard(const BFLE&f) : fun(f) { EnsureLE<BFLE>::go(); }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Guard");
    }
#endif
   };
}

struct Guard_Lambdaoid {
   template <class BF>
   exp::Guard<typename LEify<BF>::Type> operator[]( const BF& f ) const {
      return exp::Guard<typename LEify<BF>::Type>( LEify<BF>::go(f) );
   }
};

template <class Monad>
struct compM {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if ( debug_traits_has_name<Monad>() ) {
       Monad m; // Need to create an object for the call.
       temp = std::string("compM<");
       temp += debug_traits_name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("compM");
     }
     return temp;
   }
#endif
   // Dig walks all the way down the list and gets to the LHS of the
   // VertPair at the end, and returns a _reference_ to it.  This is
   // especially happy because it should translate to a no-op.
   template <class Dummy, class L> struct Dig;
   template <class D, class LHS, class RHS> 
   struct Dig<D,CONS<exp::VertPair<LHS,RHS>,NIL> > {
      typedef const LHS& Result;
      typedef LHS Type;
      static inline Result go( const CONS<exp::VertPair<LHS,RHS>,NIL>& l )
      { return l.head.lhs; }
   };
   template <class D, class E, class Rest> 
   struct Dig<D,CONS<E,Rest> > {
      typedef typename Dig<D,Rest>::Result Result;
      typedef typename Dig<D,Rest>::Type Type;
      static inline Result go( const CONS<E,Rest>& l )
      { return Dig<D,Rest>::go( l.tail ); }
   };

   template <class FinalLE, class Stuff> struct Help2;
   template <class FinalLE>
   struct Help2<FinalLE,NIL> {
      typedef FinalLE Result;
      static inline Result go( const FinalLE& le, const NIL& ) 
      { return le; }
   };
   template <class FinalLE, class LHS, class RHS> 
   struct Help2<FinalLE,CONS<exp::VertPair<LHS,RHS>,NIL> > {
      typedef typename Help2<FinalLE,CONS<RHS,NIL> >::Result Result;
      static inline Result 
      go( const FinalLE& le, const CONS<exp::VertPair<LHS,RHS>,NIL>& x ) {
         return Help2<FinalLE,CONS<RHS,NIL> >::go
            ( le, CONS<RHS,NIL>(x.head.rhs) );
      }
   };
   template <class FinalLE, class LE, class Rest> 
   struct Help2<FinalLE,CONS<LE,Rest> > {
      typedef typename LEType<CALL<typename BindM_<Monad>::Type,LE,
         FinalLE> >::Type NextLE;
      typedef typename Help2<NextLE,Rest>::Result Result;
      static inline Result 
      go( const FinalLE& le, const CONS<LE,Rest>& x ) {
         return Help2<NextLE,Rest>::go(
            bindM_<Monad>()[ x.head, le ], x.tail );
      }
   };
   template <class FinalLE, int i, class LE, class Rest> 
   struct Help2<FinalLE,CONS<exp::Gets<i,LE>,Rest> > {
      typedef typename LEType<CALL<typename BindM<Monad>::Type,LE,
         LAM<LV<i>,FinalLE> > >::Type NextLE;
      typedef typename Help2<NextLE,Rest>::Result Result;
      static inline Result 
      go( const FinalLE& le, const CONS<exp::Gets<i,LE>,Rest>& x ) {
         LambdaVar<i> X;
         return Help2<NextLE,Rest>::go(
            bindM<Monad>()[ x.head.exp, lambda(X)[ le ] ], x.tail );
      }
   };
   template <class FinalLE, class BF, class Rest> 
   struct Help2<FinalLE,CONS<exp::Guard<BF>,Rest> > {
      typedef typename LEType<IF1<BF,FinalLE,
         typename ZeroM<Monad>::Type> >::Type NextLE;
      typedef typename Help2<NextLE,Rest>::Result Result;
      static inline Result 
      go( const FinalLE& le, const CONS<exp::Guard<BF>,Rest>& x ) {
         return Help2<NextLE,Rest>::go(
            if1[ x.head.fun, le, zeroM<Monad>() ], x.tail );
      }
   };

   template <class L> struct Helper {
      typedef typename Dig<int,L>::Type DigLE;
      typedef typename LEType<CALL<typename UnitM<Monad>::Type,
         DigLE> >::Type FinalLE;
      typedef typename Help2<FinalLE,L>::Result Result;
      static inline Result go( const L& l ) { 
         return Help2<FinalLE,L>::go( unitM<Monad>()[Dig<int,L>::go(l)], l ); 
      }
   };

   template <class X> struct RT {
      typedef typename LEListify<X>::Type XP;
      typedef typename Helper<XP>::Result Type;
   };
   template <class X>
   typename RT<X>::Type
   operator[]( const X& x ) const {
      return Helper<typename RT<X>::XP>::go( LEListify<X>::go(x) );
   }
};

template <class T> struct GUARD;

template <class T> struct LEType< GUARD<T> > {
   typedef exp::Guard< typename LEify<typename LEType<T>::Type>::Type > Type;
};


#ifdef FCPP152
template <class A, class B=Void, class C=Void, class D=Void, class E=Void,
  class F=Void, class G=Void, class H=Void> struct COMP {};

template <class M, class LHS, class A>
struct LEType< COMP<M,LHS,A,Void,Void,Void,Void> > {
   typedef typename LEify<typename LEType<LHS>::Type>::Type LHST;
   typedef typename LEify<typename LEType<A>::Type>::Type AT;
   typedef exp::VertPair<LHST,AT> T;
   typedef typename LET_LEListify<T>::Type TE;
   typedef typename compM<M>::template RT<TE>::Type Type;
};
template <class M, class LHS, class A, class B>
struct LEType< COMP<M,LHS,A,B,Void,Void,Void> > {
   typedef typename LEify<typename LEType<LHS>::Type>::Type LHST;
   typedef typename LEify<typename LEType<A>::Type>::Type AT;
   typedef exp::VertPair<LHST,AT> T;
   typedef typename LET_LEListify<T,B>::Type TE;
   typedef typename compM<M>::template RT<TE>::Type Type;
};
template <class M, class LHS, class A, class B, class C>
struct LEType< COMP<M,LHS,A,B,C,Void,Void> > {
   typedef typename LEify<typename LEType<LHS>::Type>::Type LHST;
   typedef typename LEify<typename LEType<A>::Type>::Type AT;
   typedef exp::VertPair<LHST,AT> T;
   typedef typename LET_LEListify<T,B,C>::Type TE;
   typedef typename compM<M>::template RT<TE>::Type Type;
};
template <class M, class LHS, class A, class B, class C, class D>
  struct LEType< COMP<M,LHS,A,B,C,D,Void> > {
   typedef typename LEify<typename LEType<LHS>::Type>::Type LHST;
   typedef typename LEify<typename LEType<A>::Type>::Type AT;
   typedef exp::VertPair<LHST,AT> T;
   typedef typename LET_LEListify<T,B,C,D>::Type TE;
   typedef typename compM<M>::template RT<TE>::Type Type;
};

template <class M, class LHS, class A, class B, class C, class D, class E>
struct LEType< COMP<M,LHS,A,B,C,D,E> > {
   typedef typename LEify<typename LEType<LHS>::Type>::Type LHST;
   typedef typename LEify<typename LEType<A>::Type>::Type AT;
   typedef exp::VertPair<LHST,AT> T;
   typedef typename LET_LEListify<T,B,C,D,E>::Type TE;
   typedef typename compM<M>::template RT<TE>::Type Type;
};

 template <class M, class LHS, class A, class B, class C, class D, class E, class F>
  struct LEType< COMP<M,LHS,A,B,C,D,E,F> > {
   typedef typename LEify<typename LEType<LHS>::Type>::Type LHST;
   typedef typename LEify<typename LEType<A>::Type>::Type AT;
   typedef exp::VertPair<LHST,AT> T;
   typedef typename LET_LEListify<T,B,C,D,E,F>::Type TE;
   typedef typename compM<M>::template RT<TE>::Type Type;
};

#else

template <class A, class B, class C, class D=Void, class E=Void,
          class F=Void> struct COMP {};

template <class M, class LHS, class A>
struct LEType< COMP<M,LHS,A,Void,Void,Void> > {
   typedef typename LEify<typename LEType<LHS>::Type>::Type LHST;
   typedef typename LEify<typename LEType<A>::Type>::Type AT;
   typedef exp::VertPair<LHST,AT> T;
   typedef typename LET_LEListify<T>::Type TE;
   typedef typename compM<M>::template RT<TE>::Type Type;
};
template <class M, class LHS, class A, class B>
struct LEType< COMP<M,LHS,A,B,Void,Void> > {
   typedef typename LEify<typename LEType<LHS>::Type>::Type LHST;
   typedef typename LEify<typename LEType<A>::Type>::Type AT;
   typedef exp::VertPair<LHST,AT> T;
   typedef typename LET_LEListify<T,B>::Type TE;
   typedef typename compM<M>::template RT<TE>::Type Type;
};
template <class M, class LHS, class A, class B, class C>
struct LEType< COMP<M,LHS,A,B,C,Void> > {
   typedef typename LEify<typename LEType<LHS>::Type>::Type LHST;
   typedef typename LEify<typename LEType<A>::Type>::Type AT;
   typedef exp::VertPair<LHST,AT> T;
   typedef typename LET_LEListify<T,B,C>::Type TE;
   typedef typename compM<M>::template RT<TE>::Type Type;
};
template <class M, class LHS, class A, class B, class C, class D>
struct LEType< COMP<M,LHS,A,B,C,D> > {
   typedef typename LEify<typename LEType<LHS>::Type>::Type LHST;
   typedef typename LEify<typename LEType<A>::Type>::Type AT;
   typedef exp::VertPair<LHST,AT> T;
   typedef typename LET_LEListify<T,B,C,D>::Type TE;
   typedef typename compM<M>::template RT<TE>::Type Type;
};

#endif

} // end namespace fcpp_lambda

using fcpp_lambda::DOM;
using fcpp_lambda::GETS;
using fcpp_lambda::GUARD;
using fcpp_lambda::COMP;
using fcpp_lambda::compM;   // a template type name

FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN fcpp_lambda::DoM_Lambdaoid doM;
FCPP_MAYBE_EXTERN fcpp_lambda::Guard_Lambdaoid guard;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// Monad concepts
//////////////////////////////////////////////////////////////////////
// Here we outline the C++ concepts necessary for monads to work in the
// framework.
/*
   concept Monad {
      // unit :: a -> m a
      // bind :: m a -> ( a -> m b ) -> m b
      typedef Unit;           // a full functoid type
      static Unit unit;
      typedef Bind;           // a full functoid type
      static Bind bind;
   }
   //////////////////////////////
   // Inferrable Monads let you use bind() instead of bindM<Monad>()(), and
   // are also the only Monads that work with do-notation (doM).
   concept InferrableMonad models Monad {
      // type constructor: pass an 'a', get back an 'M a'
      template <class A>  struct Rep   { typedef Type; };
      // type deconstructor: pass an 'M a', get back an 'a'
      template <class MA> struct UnRep { typedef Type; };
   }
   and also
   template <> struct MonadTraitsSpecializer< RepType > {
      typedef Monad;   // the monad concept class
   };
   //////////////////////////////
   // The example parser.cpp also has ReRep with the following desription:
   //
   // ReRep is a type-identity in Haskell; here it "indirect"ifies the
   // type, so direct functoids are turned into indirect ones so that
   // only the signature information appears in the type.
   template <class MA> struct ReRep
      { typedef typename Rep<typename UnRep<MA>::Type>::Type Type; };
   //
   // The only example I have of its use is in the ParserM monad.
   //////////////////////////////
   // Monad zeros enable comprehension guards to work
   concept MonadWithZero models Monad {
      // zero :: m a
      typedef Zero;           // a value type
      static Zero zero;
   }
   //////////////////////////////
   // Some monads have pluses
   concept MonadWithPlus models Monad {
      // plus :: m a -> ma -> ma
      typedef Plus;           // a full functoid type
      static Plus plus;
   }
*/
//////////////////////////////////////////////////////////////////////

struct IdentityM; // Advance declaration

struct ListM {
   template <class A> struct Rep { typedef List<A> Type; };
   template <class MA> struct UnRep { typedef typename MA::ElementType Type; };

  // Experiment for transformers so that LiftT behaves like LiftM
  // The outer lift is made on IdentityM which has to be declared in advance.
  typedef IdentityM OuterType; // Experiments for LiftT
  typedef ListM     InnerType;

#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("ListM");
   }
#endif

   struct XUnit {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("ListM::Unit");
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
  //   static Unit unit;

   struct XBind {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("ListM::Bind");
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
  //   static Bind bind;

   struct XBind_ {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("ListM::Bind_");
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
  //   static Bind bind;

   typedef AUniqueTypeForNil Zero;
   static Zero& zero() {static Zero f; return f;}
  //   static Zero zero;

   typedef Cat Plus;   // cat is monad plus for lists
  //static Plus plus;
   static Plus& plus() {static Plus f; return f;}
   // FIX THIS maybe: map/join/bind_ could be implemented here as
   // well.  default implementations could be inherited.  Not clear if
   // it gains anything, since you can just call the "free function"
   // versions of them, which have defaults based on unit/bind.
};
// FCPP_MAYBE_DEFINE(ListM::Unit ListM::unit;)
// FCPP_MAYBE_DEFINE(ListM::Bind ListM::bind;)
// FCPP_MAYBE_DEFINE(ListM::Zero ListM::zero;)

template <class T> struct MonadTraitsSpecializer<List<T> > {
   typedef ListM Monad;
};
template <class T> struct MonadTraitsSpecializer<OddList<T> > {
   typedef ListM Monad;
};

//////////////////////////////////////////////////////////////////////

struct MaybeM {
   template <class A>  struct Rep   { typedef Maybe<A> Type; };
   template <class MA> struct UnRep { typedef typename MA::ElementType Type; };
   // taken from parser.cpp (parser.cc originally)
   // ReRep is a type-identity in Haskell; here it "indirect"ifies the
   // type, so direct functoids are turned into indirect ones so that
   // only the signature information appears in the type.
   template <class MA> struct ReRep
      { typedef typename Rep<typename UnRep<MA>::Type>::Type Type; };

  // Experiment for transformers so that LiftT behaves like LiftM
  // The outer lift is made on IdentityM which has to be declared in advance.
  typedef IdentityM OuterType; // Experiments for LiftT
  typedef MaybeM    InnerType;

#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("MaybeM");
   }
#endif

   typedef Just Unit;
   static Unit& unit() {static Unit f; return f;}
  //   static Unit unit;

   struct XBind {
#ifdef FCPP_DEBUG
         std::string name() const
         {
            return std::string("MaybeM::Bind");
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
  //   static Bind bind;

  // This is not needed in every functoid as I have now
  // defined the join for inferrable monads in terms of
  // the monad internal bind rather than using join.
   struct XJoin {
#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("MaybeM::Join");
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
       return std::string("MaybeM::Plus");
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
};
// FCPP_MAYBE_DEFINE(MaybeM::Plus MaybeM::plus;) // needed
// FCPP_MAYBE_DEFINE(MaybeM::Unit MaybeM::unit;)
// FCPP_MAYBE_DEFINE(MaybeM::Bind MaybeM::bind;)
// FCPP_MAYBE_DEFINE(MaybeM::Zero MaybeM::zero;)

template <class T> struct MonadTraitsSpecializer<Maybe<T> > {
   typedef MaybeM Monad;
};

//////////////////////////////////////////////////////////////////////
// EitherM monad structure following MaybeM
// See Real World Haskell p.452
// Added by John Fletcher (J.P.Fletcher@aston.ac.uk) October 2013
//////////////////////////////////////////////////////////////////////
struct EitherM {
   template <class A>  struct Rep   { typedef Either<A> Type; };
   template <class EA> struct UnRep { typedef typename EA::ElementType Type; };
   // taken from parser.cpp (parser.cc originally)
   // ReRep is a type-identity in Haskell; here it "indirect"ifies the
   // type, so direct functoids are turned into indirect ones so that
   // only the signature information appears in the type.
   template <class MA> struct ReRep
      { typedef typename Rep<typename UnRep<MA>::Type>::Type Type; };

  // Experiment for transformers so that LiftT behaves like LiftM
  // The outer lift is made on IdentityM which has to be declared in advance.
  typedef IdentityM OuterType; // Experiments for LiftT
  typedef EitherM   InnerType;

#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("EitherM");
   }
#endif

   typedef Right Unit;
   static Unit& unit() {static Unit f; return f;}
  //   static Unit unit;

   struct XBind {
#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("EitherM::Bind");
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
       return std::string("EitherM::Plus");
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
};

template <class T> struct MonadTraitsSpecializer<Either<T> > {
   typedef EitherM Monad;
};


//////////////////////////////////////////////////////////////////////
// Original IdentityM monad.
struct IdentityM {
  // M a = a

  // AARGH.  Unit for IdentityM does not have a Sig!!!
  // I am going to have to do something about this.
  // How about this?
  // I am not sure if this changes anything as Id will have a Sig.
  // It does cause one more level of indirection.
#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("IdentityM");
   }
#endif
   struct XUnit {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("IdentityM::Unit");
      }
#endif
      template <class A> struct Sig : public FunType<A,A > {};
      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a ) const { return id(a); }
   };
   typedef Full1<XUnit> Unit;
  //typedef Id Unit;
   static Unit& unit() {static Unit f; return f;}
  //   static Unit unit;

   struct XBind {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("IdentityM::Bind");
      }
#endif
      template <class M, class K> struct Sig : public FunType<M,K,
         typename RT<K,M>::ResultType>  {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
	//	return Identity(k(m));
	return k(m);
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}

   struct XBind_ {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("IdentityM::Bind_");
      }
#endif
      template <class M, class K> struct Sig : public FunType<M,K,
      //      typename RT<typename RT<Konst,K>::ResultType,M>::ResultType>  {};
      typename RT<Bind,M,typename RT<Konst,K>::ResultType>::ResultType>  {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
	//	return Identity(k(m));
        return bind()(m,konst(k));
	// For the moment this does the same as Bind.
	//return k(m);
      }
   };
   typedef Full2<XBind_> Bind_;
   static Bind_& bind_() {static Bind_ f; return f;}

  //   static Bind bind;
};
// FCPP_MAYBE_DEFINE(IdentityM::Unit IdentityM::unit;)
// FCPP_MAYBE_DEFINE(IdentityM::Bind IdentityM::bind;)


//////////////////////////////////////////////////////////////////////
// Copy of IdentityM to make an inferrable version.
struct IdentityInfM {
  // M a = a
  // I have now made Identity<T> as a type
  // to have something to work with here.
#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("IdentityInfM");
   }
#endif
  template <class A> struct   Rep { typedef Identity<A> Type; };
  template <class EA> struct UnRep
  // I am going to revert this change as I am not sure of it here.
  //  { typedef typename IsIdentity<EA>::Type Type; };
      { typedef typename EA::ElementType Type; };
  
   typedef Ident Unit;
   static Unit& unit() {static Unit f; return f;}
  //   static Unit unit;

  // IsIdentity is used to distinguish between
  // Identity<T> and just a type T and returns the appropriate type T.
  // This trait code is in prelude.h after Identity<T> is defined.
  // This enables e.g. 3 %multiplies% X
  // and makes it possible to hide the Identity<T> objects.
   struct XBind {
      template <class M, class K> struct Sig : public FunType<M,K,
	       typename RT<K,typename IsIdentity<M>::Type>::ResultType>  {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
        // I should test for the error case which should use a trait as well
        // Now extracted explicitly using a trait.
        typedef typename IsIdentity<M>::Type A;
        A a = IsIdentity<M>::value(m);
        // Changed to use the result type does now work.
        typedef typename RT<K,A>::ResultType R;
        return Identity<R>(k(a));
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}
  //   static Bind bind;

   typedef AUniqueTypeForNothing Zero;
   static Zero& zero() {static Zero f; return f;}
  //   static Zero zero;

   struct XPlus {
      template <class MA, class MA2> struct Sig : public FunType<MA,MA2,MA> {};
      template <class MA, class MA2>
      typename Sig<MA,MA2>::ResultType
      operator()( const MA& x, const MA2& y ) const {
            if( x.is_error() )
            return y;
	    return x;
      }
   };
   typedef Full2<XPlus> Plus;
  //   static Plus plus;
   static Plus& plus() {static Plus f; return f;}

};
// FCPP_MAYBE_DEFINE(IdentityInfM::Unit IdentityM::unit;)
// FCPP_MAYBE_DEFINE(IdentityInfM::Bind IdentityM::bind;)

template <class T>
struct MonadTraitsSpecializer<Identity<T> > {
   typedef IdentityInfM Monad;
};

//////////////////////////////////////////////////////////////////////

template <class State>
struct StateM {
   // M a =  State -> (a,State)
#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("StateM");
   }
#endif

   struct XUnit {
      template <class A> struct Sig : public FunType<A,
         typename RT<MakePair,A>::ResultType> {};
      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a ) const { return makePair(a); }
   };
   typedef Full1<XUnit> Unit;
   static Unit& unit() {static Unit f; return f;}
  //static Unit unit;

   struct XBind {
      template <class M, class K> struct Sig : public FunType<M,K,
         typename LEType<LAM<LV<2>,LET<BIND<1,CALL<M,LV<2> > >,
                                       CALL<CALL<K,CALL<Fst,LV<1> > >,
                                            CALL<Snd,LV<1> > > 
                                      > > >::Type > {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         // \s0 -> let (a,s1) = m s0
         //            (b,s2) = k a s1
         //        in  (b,s2)
         LambdaVar<1> P;
         LambdaVar<2> s0;
         return lambda(s0)[ let[ P == m[s0] ].in[
                            k[fst[P]][snd[P]] ] ];
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}
  //   static Bind bind;

   static typename LEType< LAM<LV<1>,CALL<CALL<MakePair,Empty>,State> > >::Type
   assign( const State& sp ) {
      LambdaVar<1> s;
      return lambda(s)[ makePair[empty][sp] ];
   }

   static typename LEType< LAM<LV<1>,CALL<MakePair,LV<1>,LV<1> > > >::Type
   fetch() {
      LambdaVar<1> s;
      return lambda(s)[ makePair[s,s] ];
   }
};
// template <class State> typename StateM<State>::Unit StateM<State>::unit;
// template <class State> typename StateM<State>::Bind StateM<State>::bind;


//////////////////////////////////////////////////////////////////////
// liftM, liftM2, liftM3   (as in Haskell)

namespace impl {
template <class Monad>
struct XLiftM {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if ( debug_traits_has_name<Monad>() ) {
       Monad m; // Need to create an object for the call.
       temp = std::string("LiftM<");
       temp += debug_traits_name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("LiftM");
     }
     return temp;
   }
#endif
   template <class F> struct Sig : public FunType<F,typename
   LEType<LAM<LV<1>,COMP<Monad,CALL<F,LV<2> >,GETS<2,LV<1> > > > >::Type> {};
   template <class F>
   typename Sig<F>::ResultType operator()( const F& f ) const {
      LambdaVar<1> X;
      LambdaVar<2> X_;
      return lambda(X)[ compM<Monad>()[ f[X_] | X_ <= X ] ];
   }
};
}
template <class Monad> Full1<impl::XLiftM<Monad> > liftM()
{ return makeFull1( impl::XLiftM<Monad>() ); }
template <class Monad> struct LiftM
{ typedef Full1<impl::XLiftM<Monad> > Type; };

namespace impl {
template <class Monad>
struct XLiftM2 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if ( debug_traits_has_name<Monad>() ) {
       Monad m; // Need to create an object for the call.
       temp = std::string("LiftM2<");
       temp += debug_traits_name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("LiftM2");
     }
     return temp;
   }
#endif
   template <class F> struct Sig : public FunType<F,typename
      LEType<LAM<LV<1>,LV<3>,COMP<Monad,CALL<F,LV<2>,LV<4> >,GETS<2,LV<1>
      >,GETS<4,LV<3> > > > >::Type> {};
   template <class F>
   typename Sig<F>::ResultType operator()( const F& f ) const {
      LambdaVar<1> X;
      LambdaVar<2> X_;
      LambdaVar<3> Y;
      LambdaVar<4> Y_;
      return lambda(X,Y)[ compM<Monad>()[ f[X_,Y_] | X_ <= X, Y_ <= Y ] ];
   }
};
}
template <class Monad> Full1<impl::XLiftM2<Monad> > liftM2()
{ return makeFull1( impl::XLiftM2<Monad>() ); }
template <class Monad> struct LiftM2
{ typedef Full1<impl::XLiftM2<Monad> > Type; };

namespace impl {
template <class Monad>
struct XLiftM3 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if ( debug_traits_has_name<Monad>() ) {
       Monad m; // Need to create an object for the call.
       temp = std::string("LiftM3<");
       temp += debug_traits_name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("LiftM3");
     }
     return temp;
   }
#endif
   template <class F> struct Sig : public FunType<F,typename
      LEType<LAM<LV<1>,LV<3>,LV<5>,COMP<Monad,CALL<F,LV<2>,LV<4>,LV<6>
      >,GETS<2,LV<1> >,GETS<4,LV<3> >,GETS<6,LV<5> > > > >::Type> {};
   template <class F>
   typename Sig<F>::ResultType operator()( const F& f ) const {
      LambdaVar<1> X;
      LambdaVar<2> X_;
      LambdaVar<3> Y;
      LambdaVar<4> Y_;
      LambdaVar<5> Z;
      LambdaVar<6> Z_;
      return lambda(X,Y,Z)[ compM<Monad>()[ 
            f[X_,Y_,Z_] | X_ <= X, Y_ <= Y, Z_ <= Z ] ];
   }
};
}
template <class Monad> Full1<impl::XLiftM3<Monad> > liftM3()
{ return makeFull1( impl::XLiftM3<Monad>() ); }
template <class Monad> struct LiftM3
{ typedef Full1<impl::XLiftM3<Monad> > Type; };

#ifdef FCPP152

namespace impl {
template <class Monad>
struct XLiftM4 {
   template <class F> struct Sig : public FunType<F,typename 
       LEType<LAM<LV<1>,LV<3>,LV<5>,LV<7>,COMP<Monad,CALL<F,LV<2>,LV<4>,LV<6>,LV<8> 
      >,GETS<2,LV<1> >,GETS<4,LV<3> >,GETS<6,LV<5> >,GETS<8,LV<7> > > > >::Type> {};
   template <class F>
   typename Sig<F>::ResultType operator()( const F& f ) const {
      LambdaVar<1> X;
      LambdaVar<2> X_;
      LambdaVar<3> Y;
      LambdaVar<4> Y_;
      LambdaVar<5> Z;
      LambdaVar<6> Z_;
      LambdaVar<7> V;
      LambdaVar<8> V_;
      return lambda(X,Y,Z,V)[ compM<Monad>()[ 
            f[X_,Y_,Z_,V_] | X_ <= X, Y_ <= Y, Z_ <= Z, V_ <= V ] ];
   }
};

template <class Monad>
struct XLiftM5 {
   template <class F> struct Sig : public FunType<F,typename 
						  LEType<LAM<LV<1>,LV<3>,LV<5>,LV<7>,LV<9>,
                                                  COMP<Monad,CALL<F,LV<2>,LV<4>,LV<6>,LV<8>,LV<10> >,
            GETS<2,LV<1> >,GETS<4,LV<3> >,GETS<6,LV<5> >,GETS<8,LV<7> >,GETS<10,LV<9> > > > >::Type> {};
   template <class F>
   typename Sig<F>::ResultType operator()( const F& f ) const {
      LambdaVar<1> X;
      LambdaVar<2> X_;
      LambdaVar<3> Y;
      LambdaVar<4> Y_;
      LambdaVar<5> Z;
      LambdaVar<6> Z_;
      LambdaVar<7> V;
      LambdaVar<8> V_;
      LambdaVar<9> U;
      LambdaVar<10> U_;
      return lambda(X,Y,Z,V,U)[ compM<Monad>()[ 
            f[X_,Y_,Z_,V_,U_] | X_ <= X, Y_ <= Y, Z_ <= Z, V_ <= V, U_ <= U ] ];
   }
};
}
template <class Monad> Full1<impl::XLiftM4<Monad> > liftM4()
{ return makeFull1( impl::XLiftM4<Monad>() ); }
template <class Monad> struct LiftM4
{ typedef Full1<impl::XLiftM4<Monad> > Type; };
template <class Monad> Full1<impl::XLiftM5<Monad> > liftM5()
{ return makeFull1( impl::XLiftM5<Monad>() ); }
template <class Monad> struct LiftM5
{ typedef Full1<impl::XLiftM5<Monad> > Type; };

#endif

//////////////////////////////////////////////////////////////////////
// The ByNeed monad, for "lazifying" computations

template <class T>
struct ByNeedImpl {
   void incref() const { ++refC_; }
   void decref() const { if (!--refC_) delete this; }
private:
   mutable RefCountType refC_;
   typedef union {
      unsigned char val[ sizeof(T) ];   // The real variable
      // a bunch of dummies of every conceivable type
      long z1, *pz1; long double z2, *pz2; void *z3, **pz3; 
      impl::misc_types::PtrToFxn z4, *pz4;
      impl::misc_types::Argh *pz5;
      int z6, *pz6; char z7, *pz7; double z8, *pz8; 
      impl::misc_types::PtrToMember z9, *pz9;
      impl::misc_types::PtrToMemberFxn z10, *pz10;
   } U;
   mutable U u;
   mutable Fun0<T> fxn;
   mutable bool val_is_valid;  // FIX THIS: can optimize this away into fxn

   const T& value() const { 
      return *static_cast<const T*>(static_cast<const void*>(&u.val)); 
   }
   T& value() { 
      return *static_cast<T*>(static_cast<void*>(&u.val));
   }
   void init( const T& x ) const {
      new (static_cast<void*>(&u.val)) T(x);
   }

   static T dummyT() { throw fcpp_exception("Used invalid fxn in ByNeedImpl"); }
   static Fun0<T> dummy() { 
      static Fun0<T> f( ptr_to_fun(&ByNeedImpl::dummyT) ); 
      return f;
   }

   // No copy/assignment
   ByNeedImpl( const ByNeedImpl& );
   void operator=( const ByNeedImpl& );
public:
   typedef T ElementType;
   ByNeedImpl( const T& x ) 
     : refC_(0), fxn( dummy() ), val_is_valid(true)  { init(x); }
   ByNeedImpl( Fun0<T> f )  
     : refC_(0), fxn( f       ), val_is_valid(false) {}
   const T& force() const {
      if( val_is_valid )
         return value();
      else {
         init( fxn() );
         val_is_valid = true;
         return value();
      }
   }
   const T& get_value() { return value(); }
   bool is_valid() const { return val_is_valid; }
};

template <class T>
class ByNeed {
   IRef<ByNeedImpl<T> > ref;
public:
   typedef T ElementType;
   ByNeed( const T& x ) : ref( new ByNeedImpl<T>(x) ) {}
   ByNeed( Fun0<T> f ) : ref( new ByNeedImpl<T>(f) ) {}
   // This is new. Polymorphic functoid is converted to monomorphic.
   template<class F>
   ByNeed( Full0<F> f ) : ref( new ByNeedImpl<T>(f) ) {}
   // default copy constructor and assignment operator are fine
   const T& force() const {
      return ref->force();
   }
   bool is_valid() const { return ref->is_valid(); }
   const T& value() const { return ref->value(); }
   std::string seq_string() const {
     std::string temp;
     if(is_valid()) {
          temp +=  std::string ("( ")
	    +   fcpp::traits::argument_traits<T>::value(ref->get_value())
            +  std::string (" )");
     } else {
       temp +=  std::string ("( _ )");
     }
     return temp;
    }
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp = std::string("ByNeed");
     temp += seq_string();
     return temp;
   }
 #endif
};

namespace impl {
struct XBForce {
   template <class BT> struct Sig : FunType<BT,typename BT::ElementType> {};
   template <class T>
   const T& operator()( const ByNeed<T>& bt ) const {
      return bt.force();
   }
};
}
typedef Full1<impl::XBForce> BForce;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN BForce bForce;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XBDelay {
   template <class T> struct Sig : FunType<T,ByNeed<T> > {};
   template <class T>
   ByNeed<T> operator()( const T& x ) const {
      return ByNeed<T>( x );
   }
};
}
typedef Full1<impl::XBDelay> BDelay;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN BDelay bDelay;
FCPP_MAYBE_NAMESPACE_CLOSE

// bLift lazily lifts a function's results into the ByNeed monad:
//    bLift :: (a -> b) -> a -> ByNeed b
//    bLift(f)(args) == ByNeed<T>( lambda()[ f[args] ] )
namespace impl{
template <class F>
class XBLifter0 : public CFunType<ByNeed<typename RT<F>::ResultType> > {
   F f;
public:
 #ifdef FCPP_DEBUG
   std::string name() const
   {
      if (fcpp::traits::debug_traits<F>::has_name)
         return std::string("BLifter0::")
        +fcpp::traits::debug_traits<F>::name(f);
      else
         return std::string("BLifter0");
   }
 #endif
   XBLifter0( const F& ff ) : f(ff) {}
   ByNeed<typename RT<F>::ResultType>
   operator()() const {
      return ByNeed<typename RT<F>::ResultType>( lambda()[ f[_*_] ] );
   }
};
template <class F>
class XBLifter1 {
   F f;
public:
 #ifdef FCPP_DEBUG
   std::string name() const
   {
      if (fcpp::traits::debug_traits<F>::has_name)
         return std::string("BLifter1::")
        +fcpp::traits::debug_traits<F>::name(f);
      else
         return std::string("BLifter1");
   }
 #endif
   XBLifter1( const F& ff ) : f(ff) {}
   template <class X> struct Sig
      : public FunType<X,ByNeed<typename RT<F,X>::ResultType> > {};
   template <class X>
   typename Sig<X>::ResultType
   operator()( const X& x ) const {
      return typename Sig<X>::ResultType( lambda()[ f[x] ] );
   }
};
template <class F>
class XBLifter2 {
   F f;
public:
 #ifdef FCPP_DEBUG
   std::string name() const
   {
      if (fcpp::traits::debug_traits<F>::has_name)
         return std::string("BLifter2::")
        +fcpp::traits::debug_traits<F>::name(f);
      else
         return std::string("BLifter2");
   }
 #endif
   XBLifter2( const F& ff ) : f(ff) {}
   template <class X, class Y> struct Sig
      : public FunType<X,Y,ByNeed<typename RT<F,X,Y>::ResultType> > {};
   template <class X, class Y>
   typename Sig<X,Y>::ResultType
   operator()( const X& x, const Y& y ) const {
      return typename Sig<X,Y>::ResultType( lambda()[ f[x][y] ] );
   }
};
template <class F>
class XBLifter3 {
   F f;
public:
 #ifdef FCPP_DEBUG
   std::string name() const
   {
      if (fcpp::traits::debug_traits<F>::has_name)
         return std::string("BLifter3::")
        +fcpp::traits::debug_traits<F>::name(f);
      else
         return std::string("BLifter3");
   }
 #endif
   XBLifter3( const F& ff ) : f(ff) {}
   template <class X, class Y, class Z> struct Sig
      : public FunType<X,Y,Z,ByNeed<typename RT<F,X,Y,Z>::ResultType> > {};
   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType
   operator()( const X& x, const Y& y, const Z& z ) const {
      return typename Sig<X,Y,Z>::ResultType( lambda()[ f[x][y][z] ] );
   }
};
#ifdef FCPP152
template <class F>
class XBLifter4 {
   F f;
public:
   XBLifter4( const F& ff ) : f(ff) {}
   template <class X, class Y, class Z, class V> struct Sig
      : public FunType<X,Y,Z,V,ByNeed<typename RT<F,X,Y,Z,V>::ResultType> > {};
   template <class X, class Y, class Z, class V>
   typename Sig<X,Y,Z,V>::ResultType
   operator()( const X& x, const Y& y, const Z& z, const V& v ) const {
      return typename Sig<X,Y,Z,V>::ResultType( lambda()[ f[x][y][z][v] ] );
   }
};
template <class F>
class XBLifter5 {
   F f;
public:
   XBLifter5( const F& ff ) : f(ff) {}
     template <class X, class Y, class Z, class V, class U> struct Sig
       : public FunType<X,Y,Z,V,U,ByNeed<typename RT<F,X,Y,Z,V,U>::ResultType> > {};
   template <class X, class Y, class Z, class V, class U>
     typename Sig<X,Y,Z,V,U>::ResultType
     operator()( const X& x, const Y& y, const Z& z, const V& v, const U& u ) const {
     return typename Sig<X,Y,Z,V,U>::ResultType( lambda()[ f[x][y][z][v][u] ] );
   }
};
#endif
class XBLift {
 public: // Needed for variadic FC++
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BLift");
   }
 #endif
   template <int i, class F> struct Helper;
   template <class F>
   struct Helper<0,F> {
      typedef Full0<XBLifter0<F> > Result;
      static Result go( const F& f )
      { return makeFull0( XBLifter0<F>(f) ); }
   };
   template <class F>
   struct Helper<1,F> {
      typedef Full1<XBLifter1<F> > Result;
      static Result go( const F& f )
      { return makeFull1( XBLifter1<F>(f) ); }
   };
   template <class F>
   struct Helper<2,F> {
      typedef Full2<XBLifter2<F> > Result;
      static Result go( const F& f )
      { return makeFull2( XBLifter2<F>(f) ); }
   };
   template <class F>
   struct Helper<3,F> {
      typedef Full3<XBLifter3<F> > Result;
      static Result go( const F& f )
      { return makeFull3( XBLifter3<F>(f) ); }
   };
#ifdef FCPP152
   template <class F>
   struct Helper<4,F> {
      typedef Full4<XBLifter4<F> > Result;
      static Result go( const F& f )
      { return makeFull4( XBLifter4<F>(f) ); }
   };
   template <class F>
   struct Helper<5,F> {
      typedef Full5<XBLifter5<F> > Result;
      static Result go( const F& f )
      { return makeFull5( XBLifter5<F>(f) ); }
   };
#endif
public:
   template <class F> struct Sig : public FunType<F,
      typename Helper<FunctoidTraits<F>::max_args,F>::Result> {};
   template <class F>
   typename Sig<F>::ResultType operator()( const F& f ) const {
      return Helper<FunctoidTraits<F>::max_args,F>::go( f );
   }
};
}
typedef Full1<impl::XBLift> BLift;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN BLift bLift;
FCPP_MAYBE_NAMESPACE_CLOSE

 namespace traits {

     template <class Monad >
     class argument_traits<UnitM<Monad> > {
    public:
      static std::string value(const UnitM<Monad>& u)
      {
	return std::string("UnitM(arg)");
      }
    };

}

struct ByNeedM {
   template <class A> struct Rep { typedef ByNeed<A> Type; };
   template <class MA> struct UnRep { typedef typename MA::ElementType Type; };

#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("ByNeedM");
   }
#endif

   struct XUnit {
      template <class A> struct Sig : public FunType<A,ByNeed<A> > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
         return ByNeed<A>( x );
      }
   };
   typedef Full1<XUnit> Unit;
   static Unit& unit() {static Unit f; return f;}
  //   static Unit unit;

   struct XBind {
      template <class M, class K> struct Sig : public FunType<M,K,
         typename RT<K,typename M::ElementType>::ResultType > {};
      template <class M, class K>
      typename Sig<M,K>::ResultType operator()( const M& m, const K& k ) const {
         typedef typename Sig<M,K>::ResultType Res;
         return Res( lambda()[ bForce[ k[ bForce[ m ] ] ] ] );
         //return k( bForce( m ) );
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}
  //   static Bind bind;
};
// FCPP_MAYBE_DEFINE(ByNeedM::Unit ByNeedM::unit;)
// FCPP_MAYBE_DEFINE(ByNeedM::Bind ByNeedM::bind;)
   
template <class T> struct MonadTraitsSpecializer<ByNeed<T> > {
   typedef ByNeedM Monad;
};

} // end namespace fcpp

/* I am not sure about this as operator<< can be usefully
   defined for the ByNeed type. Something else is needed. */
/*
template <class T>
  std::ostream & operator << (std::ostream &o, const fcpp::ByNeed<T> &f)
{
  //#ifdef FCPP_DEBUG
  //o << f.name();
  //#else
  o << "**** ERROR: Attempt to output a ByNeed<T> ****";
  //#endif
  return o;
}
*/

// Includes argument traits needed without FCPP_DEBUG
#include "monad_debug.h"

#endif
