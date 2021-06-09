//===========================================================
// variadic_monad.h
//===========================================================
//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
// Copyright (c) 2007-2016 John Fletcher
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

// Monad only applications for variadic FC++

#ifndef FCPP_VARIADIC_MONAD_H
#define FCPP_VARIADIC_MONAD_H

namespace variadic_fcpp {

#ifdef FCPP_ENABLE_LAMBDA
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// This part only for monads.  IT IS NOW WORKING AFTER SOME DIFFICULTIES.
// bLiftN is now implemented for up to 6 arguments for VFC++, 4 arguments for FC++.
// Note that there are some problems associated with the operator, in lambda
// which results in some arguments being discarded.
// In user code use the following for a VFC++ functoid such as plusN
// where a,b,c,d,e and f are arguments to be used (or can be lambdavars)
// 1 arg is O.K. : [a]
// 2 args use [_b_,b][a]
//        and NOT [a,b] (throws away a)
//             or [a][b] (invalid)
// 3 args use [_b_,b,c][a]
//        and NOT [a,b,c] (throws away a and b)
//             or [a][b][c] (invalid)
// 4 args use [_b_,b,c,d][a]
//        and NOT [a,b,c,d] (throws away a b and c)
//             or [a][b][c][d] (invalid)
// 5 args use [_b_,b,c,d,e][a]
//        and NOT [a,b,c,d,e] (throws away a b c and d)
//             or [a][b][c][d][e] (invalid)
// 6 args use [_b_,b,c,d,e][_b_,f][a]
//        and NOT [a,b,c,d,e,f] (throws away a b c d and e)
//             or [a][b][c][d][e][f] (invalid)
//        Note: [_b_,b,c,d,e,f][a] requires a binder not yet implemented.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template <class F>
class XBLifterN  : public SmartFunctoidN {
   F f;
public:
   XBLifterN( const F& ff ) : f(ff) {}

    // Now use ArgN for the argument types.
  template <int i,typename... Args> struct SigN;

  template <typename X> struct SigN<1,X> :
  public FunType<X,fcpp::ByNeed<typename RT<F,X>::ResultType> > {};

  template <typename X, typename Y> struct SigN<2,X,Y> :
  public FunType<X,Y,fcpp::ByNeed<typename RT<F,X,Y>::ResultType> > {};

  template <typename X, typename Y, typename Z> struct SigN<3,X,Y,Z> :
  public FunType<X,Y,Z,fcpp::ByNeed<typename RT<F,X,Y,Z>::ResultType> > {};

  template <typename X, typename Y, typename Z, typename U>
  struct SigN<4,X,Y,Z,U> :
  public FunType<X,Y,Z,U,fcpp::ByNeed<typename RT<F,X,Y,Z,U>::ResultType> > {};

  template <typename X, typename Y, typename Z, typename U, typename V>
  struct SigN<5,X,Y,Z,U,V> :
  public FunType<X,Y,Z,U,V,fcpp::ByNeed<typename RT<F,X,Y,Z,U,V>::ResultType> >
  {};

  template <typename X, typename Y, typename Z, typename U, typename V,
            typename W> struct SigN<6,X,Y,Z,U,V,W> :
  public FunType<X,Y,Z,U,V,W,fcpp::ByNeed<typename RT<F,X,Y,Z,U,V,W>::ResultType> >
  {};

  template <typename... Args> struct Sig : public SigN<sizeof...(Args),Args...>,
                        ArgN<sizeof... (Args),Args...> {};

  //template <typename... Args> struct Sig
  //   : public FunType<Args...,fcpp::ByNeed<typename RT<F,Args...>::ResultType> > {};

   template <int i,typename... Args> struct HelperN;

   template <typename A> struct HelperN<1,A>
     {
      typedef typename Sig<A>::ResultType Result;
      static Result go(const F& f,const A& a)
      {
	//std::cout << "XBLifterN::HelperN<1,A> called with " << a << std::endl;
         return Result( fcpp::lambda()[ f[a] ] ); }
     };

   template <typename A, typename B> struct HelperN<2,A,B>
     {
       typedef typename Sig<A,B>::ResultType Result;
       static Result go(const F& f,const A& a, const B& b)
       {
	 // std::cout << "XBLifterN::HelperN<2,A,B> called with " << a << " and " << b << std::endl;
#ifdef FCPP_LAMBDA_AUTOCURRY
	 // std::cout << "using FCPP_LAMBDA_AUTOCURRY" << std::endl;
	 return Result( fcpp::lambda()[ f[fcpp::_b_,b][a]] );
#else
	 // This is known not to work but I have no usable alternative
	 // without using lambda autocurrying.
	 // This is because of issues with the lambda operator,
	 return Result( fcpp::lambda()[ f[a,b] ] );
#endif
       }
     };

   template <typename A, typename B, typename C> struct HelperN<3,A,B,C>
     {
       typedef typename Sig<A,B,C>::ResultType Result;
       static Result go(const F& f,const A& a, const B& b, const C&c)
       {
	 //std::cout << "XBLifterN::HelperN<3,A,B,C> called with " << a << ", " << b
         //          << " and " << c << std::endl;
#ifdef FCPP_LAMBDA_AUTOCURRY
	 //std::cout << "using FCPP_LAMBDA_AUTOCURRY" << std::endl;
	 return Result( fcpp::lambda()[ f[fcpp::_b_,b,c][a]] );
#else
	 // This is known not to work but I have no usable alternative
	 // without using lambda autocurrying.
	 // This is because of issues with the lambda operator,
	 return Result( fcpp::lambda()[ f[a,b,c] ] );
#endif
       }
     };

   template <typename A, typename B, typename C,typename D> struct HelperN<4,A,B,C,D>
     {
       typedef typename Sig<A,B,C,D>::ResultType Result;
       static Result go(const F& f,const A& a, const B& b, const C&c, const D&d)
       {
	 //std::cout << "XBLifterN::HelperN<4,A,B,C,D> called with " << a << ", " << b << ", "<< c
         //          << " and " << d << std::endl;
#ifdef FCPP_LAMBDA_AUTOCURRY
	 //std::cout << "using FCPP_LAMBDA_AUTOCURRY" << std::endl;
	 return Result( fcpp::lambda()[ f[fcpp::_b_,b,c,d][a]] );
#else
	 // This is known not to work but I have no usable alternative
	 // without using lambda autocurrying.
	 // This is because of issues with the lambda operator,
	 return Result( fcpp::lambda()[ f[a,b,c,d] ] );
#endif
       }
     };

   template <typename A, typename B, typename C,typename D, typename E> struct HelperN<5,A,B,C,D,E>
     {
       typedef typename Sig<A,B,C,D,E>::ResultType Result;
       static Result go(const F& f,const A& a, const B& b, const C&c, const D&d, const E&e)
       {
	 //std::cout << "XBLifterN::HelperN<5,A,B,C,D,E> called with " << a << ", " << b << ", "<< c
         //          << ", " << d << " and " << e << std::endl;
#ifdef FCPP_LAMBDA_AUTOCURRY
	 //std::cout << "using FCPP_LAMBDA_AUTOCURRY" << std::endl;
	 return Result( fcpp::lambda()[ f[fcpp::_b_,b,c,d,e][a]] );
#else
	 // This is known not to work but I have no usable alternative
	 // without using lambda autocurrying.
	 // This is because of issues with the lambda operator,
	 return Result( fcpp::lambda()[ f[a,b,c,d,e] ] );
#endif
       }
     };


   // Cannot use F and f as we have them already - use FF and ff instead.
   template <typename A, typename B, typename C,typename D, typename E, typename FF>
     struct HelperN<6,A,B,C,D,E,FF>
     {
       typedef typename Sig<A,B,C,D,E,FF>::ResultType Result;
       static Result go(const F& f,const A& a, const B& b, const C&c, const D&d, const E&e, const FF&ff)
       {
	 //std::cout << "XBLifterN::HelperN<6,A,B,C,D,E,F> called with " << a << ", " << b << ", "<< c
         //          << ", " << d << ", " << e <<  " and " << ff << std::endl;
#ifdef FCPP_LAMBDA_AUTOCURRY
	 //std::cout << "using FCPP_LAMBDA_AUTOCURRY" << std::endl;
	 return Result( fcpp::lambda()[ f[fcpp::_b_,b,c,d,e,ff][a]] );
#else
	 // This is known not to work but I have no usable alternative
	 // without using lambda autocurrying.
	 // This is because of issues with the lambda operator,
	 return Result( fcpp::lambda()[ f[a,b,c,d,e] ] );
#endif
       }
     };

   template <typename... Args>
     typename Sig<Args...>::ResultType
     operator()( const Args&... args) const {
     // lambda will not take f[args...]
     // So we are going to need helpers which will do this.
     return HelperN<sizeof...(Args),Args...>::go(f,args...);
   }
};

class BLiftN  : public SmartFunctoidN {
   template <int i, class F> struct HelperN;
   template <class F>
   struct HelperN<-1,F> {
      typedef FullN<XBLifterN<F> > Result;
      static Result go( const F& f )
      { return makeFullN( XBLifterN<F>(f) ); }
   };

   // These enable bLiftN to process FC++ functoids by passing through to FC++ implementation.
   template <class F>
     struct HelperN<0,F> : public fcpp::impl::XBLift::Helper<0,F> { };
   template <class F>
     struct HelperN<1,F> : public fcpp::impl::XBLift::Helper<1,F> { };
   template <class F>
     struct HelperN<2,F> : public fcpp::impl::XBLift::Helper<2,F> { };
   template <class F>
     struct HelperN<3,F> : public fcpp::impl::XBLift::Helper<3,F> { };
   template <class F>
     struct HelperN<4,F> : public fcpp::impl::XBLift::Helper<4,F> { };

public:
   template <class F> struct Sig : public FunType<F,
   typename HelperN<fcpp::FunctoidTraits<F>::max_args,F>::Result> {};
   template <class F>
   typename Sig<F>::ResultType operator()( const F& f ) const {
     return HelperN<fcpp::FunctoidTraits<F>::max_args,F>::go( f );
   }

     static FullN<BLiftN>& full() {static FullN<BLiftN> f; return f;}

};

static FullN<BLiftN>& bLiftN=BLiftN::full();


#endif

}

#endif
