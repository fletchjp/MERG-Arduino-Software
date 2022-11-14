//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

/*
List of things I know I can do better...
 - being more careful about LEs and 'wrapped' LEs (LambdaExp<LE>), so
   that I don't end up unnecessarily wrapping getses, etc.  Indeed,
   design so that comprehension commas make sense...
 - a way to ask for a fresh variable
 - have any LE which is sans-free-vars be a lambda--that is, callable
   via no-arg op().  this will make "compM<M>[ yadda ]()" work, yay!
    - this looks like a bit of work, and will need new LETypes like 
      COMPX or something, so I've decided not to do it

syntax summary:
   lambda(X,Y)[ LE ] //(a,b)
   let[ X==f[a], Y==g[X,b] ].in[ LExy ] //()     
   letrec[ x==f[x,y], y==g[x,y] ].in[ LExy ] //()
   compM<M>()[ LE | LE, x<=LE, guard[LE] ] //()
   doM[ LE, x<=LE, LE ] //()
   if0,if1,if2

Everything centers around LEs; whether LEs have free vars can be key to
whether they're callable; explicit lambdas are a special case of
callable LEs where certain bindings on params happen.
*/

#ifndef FCPP_LAMBDA_DOT_H
#define FCPP_LAMBDA_DOT_H

#include "full.h"

#ifdef FCPP_ENABLE_LAMBDA
namespace fcpp {

  //class Inc;

namespace fcpp_lambda {

//////////////////////////////////////////////////////////////////////
// Literate error messages look nicer when emitted as compiler
// diagnostics when they're not deeply nested inside classes, so we move
// them all out to the 'top' level of this namespace here.
//////////////////////////////////////////////////////////////////////
   template <class Actual, class Given, bool b>
   struct TheActualTypeOfTheLambdaExpressionIsNotConvertibleToItsGivenType;
   template <class Actual, class Given>
   struct TheActualTypeOfTheLambdaExpressionIsNotConvertibleToItsGivenType
   <Actual,Given,true> { typedef int Error; };

   template <class FreeVars>
   struct YouCannotInvokeALambdaContainingFreeVars {};

   template <class Dummy, bool b>
   struct TheTypeSpecYouHaveGivenIsIncompatibleWithItsLE;
   template <class Dummy>
   struct TheTypeSpecYouHaveGivenIsIncompatibleWithItsLE<Dummy,true>
   { typedef int Error; };

   template <class EE, bool b> struct IfExpressionMustHaveTypeConvertibleToBool;
   template <class EE> 
   struct IfExpressionMustHaveTypeConvertibleToBool<EE,true> 
   { typedef EE Type; };

   template <class TT, class FF, bool b> struct
   ActualTypeOfFalseBranchMustBeImplicitlyConvertibleToTypeOfTrueBranch;
   template <class TT, class FF> struct
   ActualTypeOfFalseBranchMustBeImplicitlyConvertibleToTypeOfTrueBranch
   <TT,FF,true> { typedef TT Error; };

   template <class FF, class TT, bool b> struct
   ActualTypeOfTrueBranchMustBeImplicitlyConvertibleToTypeOfFalseBranch;
   template <class FF, class TT> struct
   ActualTypeOfTrueBranchMustBeImplicitlyConvertibleToTypeOfFalseBranch
   <FF,TT,true> { typedef FF Error; };

   template <class TT, class FF> struct TrueAndFalseBranchOfIfMustHaveSameType;
   template <class TT> struct TrueAndFalseBranchOfIfMustHaveSameType<TT,TT>
   { typedef TT Type; };

   template <bool b> 
   struct YouCannotPassTheSameLambdaVarTo_lambda_MoreThanOnce;
   template <> 
   struct YouCannotPassTheSameLambdaVarTo_lambda_MoreThanOnce<false>
   { static inline void go() {} };

//////////////////////////////////////////////////////////////////////
// Useful helpers
//////////////////////////////////////////////////////////////////////

// some quick forward decls
namespace exp { 
   template <int i> class LambdaVar; 
   template <class T> class Value; 
}
using exp::LambdaVar;

template <class T>
struct ThunkifyType {
   struct Result {
      template <class Dummy> struct Go {
         typedef T Type;
      };
   };
};

//////////////////////////////////////////////////////////////////////
// Environment stuff
//////////////////////////////////////////////////////////////////////

// I seem to recall that
//   ET - Environment Thunk
//   BE - Binding Environment
//   TE - Type Environment
// Yes.  See pre_lambda.h for a little more explanation.

struct NIL_ET {
   template <class Dummy> struct Go {
      typedef NIL TE;
      typedef NIL BE;
   };
};

template <class OldET, class LVList>
struct EraseLVsFromET {
   struct Result {
      template <class Dummy> struct Go {
         typedef typename OldET::template Go<Dummy>::TE TE1;
         typedef typename OldET::template Go<Dummy>::BE BE1;

         struct BPred {
            template <class DumDum, class BEP> struct Go;
            template <class D, int i, class LE> struct Go<D,BEPair<i,LE> > { 
               static const bool value = 
                  !(Contains<LVList,LambdaVar<i> >::value);
            };
         };
         typedef Filter<BPred,BE1> BF;
         typedef typename BF::Result BE;
         static inline BE go( const BE1& x ) { return BF::go(x); }
   
         struct TPred {
            template <class DumDum, class TEP> struct Go;
            template <class D, int i, class T> struct Go<D,TEPair<i,T> > { 
               static const bool value = 
                  !(Contains<LVList,LambdaVar<i> >::value);
            };
         };
         typedef Filter<TPred,TE1> TF;
         typedef typename TF::Result TE;
      };
   };
};

template <class ET1, int i, class X>
struct ETUpdateX {
   struct Result {
      template <class Dummy> struct Go {
         typedef CONS<LambdaVar<i>,NIL> BoundVars;
         typedef typename ET1::template Go<Dummy>::BE BE1;
         typedef typename EraseLVsFromET<ET1,BoundVars>::Result ET2;
         typedef typename ET2::template Go<Dummy>::TE TE2;
         typedef typename ET2::template Go<Dummy>::BE BE2;
         typedef typename exp::Value<X> LEX; typedef BEPair<i,LEX> BEX;
         typedef CONS<BEX,BE2> BE;
         typedef typename ThunkifyType<X>::Result XTT;
         typedef TEPair<i,XTT> TEX;
         typedef CONS<TEX,TE2> TE;
         static inline BE go( const BE1& be, const X& x ) {
            return BE( BEX(LEX(x)), ET2::template Go<Dummy>::go(be) );
         }
      };
   };
};

template <class ET1, int i, class X, int j, class Y>
struct ETUpdateXY {
   struct Result {
      template <class Dummy> struct Go {
         typedef CONS<LambdaVar<i>,CONS<LambdaVar<j>,NIL> > BoundVars;
         typedef typename ET1::template Go<Dummy>::BE BE1;
         typedef typename EraseLVsFromET<ET1,BoundVars>::Result ET2;
         typedef typename ET2::template Go<Dummy>::TE TE2;
         typedef typename ET2::template Go<Dummy>::BE BE2;
         typedef typename exp::Value<X> LEX; typedef BEPair<i,LEX> BEX;
         typedef typename exp::Value<Y> LEY; typedef BEPair<j,LEY> BEY;
         typedef CONS<BEY,BE2> YBE;
         typedef CONS<BEX,YBE> BE;
         typedef typename ThunkifyType<X>::Result XTT;
         typedef TEPair<i,XTT> TEX;
         typedef typename ThunkifyType<Y>::Result YTT;
         typedef TEPair<j,YTT> TEY;
         typedef CONS<TEX,CONS<TEY,TE2> > TE;
         static inline BE go( const BE1& be, const X& x, const Y& y ) {
            return BE( BEX(LEX(x)), YBE( BEY(LEY(y)), 
                       ET2::template Go<Dummy>::go(be) ) );
         }
      };
   };
};

template <class ET1, int i, class X, int j, class Y, int k, class Z>
struct ETUpdateXYZ {
   struct Result {
      template <class Dummy> struct Go {
         typedef CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                    CONS<LambdaVar<k>,NIL> > > BoundVars;
         typedef typename ET1::template Go<Dummy>::BE BE1;
         typedef typename EraseLVsFromET<ET1,BoundVars>::Result ET2;
         typedef typename ET2::template Go<Dummy>::TE TE2;
         typedef typename ET2::template Go<Dummy>::BE BE2;
         typedef typename exp::Value<X> LEX; typedef BEPair<i,LEX> BEX;
         typedef typename exp::Value<Y> LEY; typedef BEPair<j,LEY> BEY;
         typedef typename exp::Value<Z> LEZ; typedef BEPair<k,LEZ> BEZ;
         typedef CONS<BEZ,BE2> ZBE;
         typedef CONS<BEY,ZBE> YBE;
         typedef CONS<BEX,YBE> BE;
         typedef typename ThunkifyType<X>::Result XTT;
         typedef TEPair<i,XTT> TEX;
         typedef typename ThunkifyType<Y>::Result YTT;
         typedef TEPair<j,YTT> TEY;
         typedef typename ThunkifyType<Z>::Result ZTT;
         typedef TEPair<k,ZTT> TEZ;
         typedef CONS<TEX,CONS<TEY,CONS<TEZ,TE2> > > TE;
         static inline BE go( const BE1& be, const X& x, const Y& y,
                              const Z& z ) {
            return BE( BEX(LEX(x)), YBE( BEY(LEY(y)), ZBE( BEZ(LEZ(z)),
                       ET2::template Go<Dummy>::go(be) ) ) );
         }
      };
   };
};

#ifdef FCPP152
// Various extensions to cover 4 5 and 6 parameter cases.

template <class ET1, int i, class X, int j, class Y, int k, class Z, int l, class V>
struct ETUpdateXYZV {
   struct Result {
      template <class Dummy> struct Go {
         typedef CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                    CONS<LambdaVar<k>,CONS<LambdaVar<l>,NIL> > > > BoundVars;
         typedef typename ET1::template Go<Dummy>::BE BE1;
         typedef typename EraseLVsFromET<ET1,BoundVars>::Result ET2;
         typedef typename ET2::template Go<Dummy>::TE TE2;
         typedef typename ET2::template Go<Dummy>::BE BE2;
         typedef typename exp::Value<X> LEX; typedef BEPair<i,LEX> BEX;
         typedef typename exp::Value<Y> LEY; typedef BEPair<j,LEY> BEY;
         typedef typename exp::Value<Z> LEZ; typedef BEPair<k,LEZ> BEZ;
         typedef typename exp::Value<V> LEV; typedef BEPair<l,LEV> BEV;
         typedef CONS<BEV,BE2> VBE;
         typedef CONS<BEZ,VBE> ZBE;
         typedef CONS<BEY,ZBE> YBE;
         typedef CONS<BEX,YBE> BE;
         typedef typename ThunkifyType<X>::Result XTT;
         typedef TEPair<i,XTT> TEX;
         typedef typename ThunkifyType<Y>::Result YTT;
         typedef TEPair<j,YTT> TEY;
         typedef typename ThunkifyType<Z>::Result ZTT;
         typedef TEPair<k,ZTT> TEZ;
         typedef typename ThunkifyType<V>::Result VTT;
         typedef TEPair<l,VTT> TEV;
         typedef CONS<TEX,CONS<TEY,CONS<TEZ,CONS<TEV,TE2> > > > TE;
         static inline BE go( const BE1& be, const X& x, const Y& y,
                              const Z& z , const V& v) {
           return BE( BEX(LEX(x)), YBE( BEY(LEY(y)), ZBE( BEZ(LEZ(z)), VBE( BEV(LEV(v)),
                       ET2::template Go<Dummy>::go(be) ) ) ) );
         }
      };
   };
};


template <class ET1, int i, class X, int j, class Y, int k, class Z, int l, class V, 
                      int m, class W>
struct ETUpdateXYZVW {
   struct Result {
      template <class Dummy> struct Go {
         typedef CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                    CONS<LambdaVar<k>,CONS<LambdaVar<l>,CONS<LambdaVar<m>,NIL> > > > > BoundVars;
         typedef typename ET1::template Go<Dummy>::BE BE1;
         typedef typename EraseLVsFromET<ET1,BoundVars>::Result ET2;
         typedef typename ET2::template Go<Dummy>::TE TE2;
         typedef typename ET2::template Go<Dummy>::BE BE2;
         typedef typename exp::Value<X> LEX; typedef BEPair<i,LEX> BEX;
         typedef typename exp::Value<Y> LEY; typedef BEPair<j,LEY> BEY;
         typedef typename exp::Value<Z> LEZ; typedef BEPair<k,LEZ> BEZ;
         typedef typename exp::Value<V> LEV; typedef BEPair<l,LEV> BEV;
         typedef typename exp::Value<W> LEW; typedef BEPair<m,LEW> BEW;
         typedef CONS<BEW,BE2> WBE;
         typedef CONS<BEV,WBE> VBE;
         typedef CONS<BEZ,VBE> ZBE;
         typedef CONS<BEY,ZBE> YBE;
         typedef CONS<BEX,YBE> BE;
         typedef typename ThunkifyType<X>::Result XTT;
         typedef TEPair<i,XTT> TEX;
         typedef typename ThunkifyType<Y>::Result YTT;
         typedef TEPair<j,YTT> TEY;
         typedef typename ThunkifyType<Z>::Result ZTT;
         typedef TEPair<k,ZTT> TEZ;
         typedef typename ThunkifyType<V>::Result VTT;
         typedef TEPair<l,VTT> TEV;
         typedef typename ThunkifyType<W>::Result WTT;
         typedef TEPair<m,WTT> TEW;
         typedef CONS<TEX,CONS<TEY,CONS<TEZ,CONS<TEV,CONS<TEW,TE2> > > > > TE;
         static inline BE go( const BE1& be, const X& x, const Y& y,
                              const Z& z , const V& v , const W& w) {
            return BE( BEX(LEX(x)), YBE( BEY(LEY(y)), ZBE( BEZ(LEZ(z)), VBE( BEV(LEV(v)),
                       WBE( BEW(LEW(w)),ET2::template Go<Dummy>::go(be) ) ) ) ) );
         }
      };
   };
};

template <class ET1, int i, class X, int j, class Y, int k, class Z, int l, class V,
                      int m, class W, int n, class U>
struct ETUpdateXYZVWU {
   struct Result {
      template <class Dummy> struct Go {
         typedef CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                    CONS<LambdaVar<k>,CONS<LambdaVar<l>,
                       CONS<LambdaVar<m>,CONS<LambdaVar<n>,NIL> > > > > > BoundVars;
         typedef typename ET1::template Go<Dummy>::BE BE1;
         typedef typename EraseLVsFromET<ET1,BoundVars>::Result ET2;
         typedef typename ET2::template Go<Dummy>::TE TE2;
         typedef typename ET2::template Go<Dummy>::BE BE2;
         typedef typename exp::Value<X> LEX; typedef BEPair<i,LEX> BEX;
         typedef typename exp::Value<Y> LEY; typedef BEPair<j,LEY> BEY;
         typedef typename exp::Value<Z> LEZ; typedef BEPair<k,LEZ> BEZ;
         typedef typename exp::Value<V> LEV; typedef BEPair<l,LEV> BEV;
         typedef typename exp::Value<W> LEW; typedef BEPair<m,LEW> BEW;
         typedef typename exp::Value<U> LEU; typedef BEPair<n,LEU> BEU;
         typedef CONS<BEU,BE2> UBE;
         typedef CONS<BEW,UBE> WBE;
         typedef CONS<BEV,WBE> VBE;
         typedef CONS<BEZ,VBE> ZBE;
         typedef CONS<BEY,ZBE> YBE;
         typedef CONS<BEX,YBE> BE;
         typedef typename ThunkifyType<X>::Result XTT;
         typedef TEPair<i,XTT> TEX;
         typedef typename ThunkifyType<Y>::Result YTT;
         typedef TEPair<j,YTT> TEY;
         typedef typename ThunkifyType<Z>::Result ZTT;
         typedef TEPair<k,ZTT> TEZ;
         typedef typename ThunkifyType<V>::Result VTT;
         typedef TEPair<l,VTT> TEV;
         typedef typename ThunkifyType<W>::Result WTT;
         typedef TEPair<m,WTT> TEW;
         typedef typename ThunkifyType<U>::Result UTT;
         typedef TEPair<n,UTT> TEU;
         typedef CONS<TEX,CONS<TEY,CONS<TEZ,CONS<TEV,CONS<TEW,CONS<TEU,TE2> > > > > > TE;
         static inline BE go( const BE1& be, const X& x, const Y& y,
                              const Z& z , const V& v , const W& w, const U& u) {
            return BE( BEX(LEX(x)), YBE( BEY(LEY(y)), ZBE( BEZ(LEZ(z)), VBE( BEV(LEV(v)),
                       WBE( BEW(LEW(w)), UBE( BEU(LEU(u)),
                       ET2::template Go<Dummy>::go(be) ) ) ) ) ) );
         }
      };
   };
};

#ifdef FCPP_LAMBDA7

template <class ET1, int i, class X, int j, class Y, int k, class Z,
  int l, class V, int m, class W, int n, class U, int o, class Q>
struct ETUpdateXYZVWUQ {
   struct Result {
      template <class Dummy> struct Go {
         typedef CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                    CONS<LambdaVar<k>,CONS<LambdaVar<l>,
                       CONS<LambdaVar<m>,CONS<LambdaVar<n>,
                           CONS<LambdaVar<o>,NIL> > > > > > > BoundVars;
         typedef typename ET1::template Go<Dummy>::BE BE1;
         typedef typename EraseLVsFromET<ET1,BoundVars>::Result ET2;
         typedef typename ET2::template Go<Dummy>::TE TE2;
         typedef typename ET2::template Go<Dummy>::BE BE2;
         typedef typename exp::Value<X> LEX; typedef BEPair<i,LEX> BEX;
         typedef typename exp::Value<Y> LEY; typedef BEPair<j,LEY> BEY;
         typedef typename exp::Value<Z> LEZ; typedef BEPair<k,LEZ> BEZ;
         typedef typename exp::Value<V> LEV; typedef BEPair<l,LEV> BEV;
         typedef typename exp::Value<W> LEW; typedef BEPair<m,LEW> BEW;
         typedef typename exp::Value<U> LEU; typedef BEPair<n,LEU> BEU;
         typedef typename exp::Value<Q> LEQ; typedef BEPair<o,LEQ> BEQ;
         typedef CONS<BEQ,BE2> QBE;
         typedef CONS<BEU,QBE> UBE;
         typedef CONS<BEW,UBE> WBE;
         typedef CONS<BEV,WBE> VBE;
         typedef CONS<BEZ,VBE> ZBE;
         typedef CONS<BEY,ZBE> YBE;
         typedef CONS<BEX,YBE> BE;
         typedef typename ThunkifyType<X>::Result XTT;
         typedef TEPair<i,XTT> TEX;
         typedef typename ThunkifyType<Y>::Result YTT;
         typedef TEPair<j,YTT> TEY;
         typedef typename ThunkifyType<Z>::Result ZTT;
         typedef TEPair<k,ZTT> TEZ;
         typedef typename ThunkifyType<V>::Result VTT;
         typedef TEPair<l,VTT> TEV;
         typedef typename ThunkifyType<W>::Result WTT;
         typedef TEPair<m,WTT> TEW;
         typedef typename ThunkifyType<U>::Result UTT;
         typedef TEPair<n,UTT> TEU;
         typedef typename ThunkifyType<Q>::Result QTT;
         typedef TEPair<o,QTT> TEQ;
         typedef CONS<TEX,CONS<TEY,CONS<TEZ,CONS<TEV,CONS<TEW,CONS<TEU,CONS<TEQ,TE2> > > > > > > TE;
         static inline BE go( const BE1& be, const X& x, const Y& y,
                              const Z& z , const V& v , const W& w, const U& u, const Q& q) {
           return BE( BEX(LEX(x)), YBE( BEY(LEY(y)), ZBE( BEZ(LEZ(z)), VBE( BEV(LEV(v)),
                       WBE( BEW(LEW(w)), UBE( BEU(LEU(u)), QBE( BEQ(LEQ(q)),
                       ET2::template Go<Dummy>::go(be) ) ) ) ) ) ) );
         }
      };
   };
};

template <class ET1, int i, class X, int j, class Y, int k, class Z,
  int l, class V, int m, class W, int n, class U, int o, class Q, int p, class P>
struct ETUpdateXYZVWUQP {
   struct Result {
      template <class Dummy> struct Go {
         typedef CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                    CONS<LambdaVar<k>,CONS<LambdaVar<l>,
                       CONS<LambdaVar<m>,CONS<LambdaVar<n>,
                           CONS<LambdaVar<o>,
                               CONS<LambdaVar<p>,NIL> > > > > > > > BoundVars;
         typedef typename ET1::template Go<Dummy>::BE BE1;
         typedef typename EraseLVsFromET<ET1,BoundVars>::Result ET2;
         typedef typename ET2::template Go<Dummy>::TE TE2;
         typedef typename ET2::template Go<Dummy>::BE BE2;
         typedef typename exp::Value<X> LEX; typedef BEPair<i,LEX> BEX;
         typedef typename exp::Value<Y> LEY; typedef BEPair<j,LEY> BEY;
         typedef typename exp::Value<Z> LEZ; typedef BEPair<k,LEZ> BEZ;
         typedef typename exp::Value<V> LEV; typedef BEPair<l,LEV> BEV;
         typedef typename exp::Value<W> LEW; typedef BEPair<m,LEW> BEW;
         typedef typename exp::Value<U> LEU; typedef BEPair<n,LEU> BEU;
         typedef typename exp::Value<Q> LEQ; typedef BEPair<o,LEQ> BEQ;
         typedef typename exp::Value<P> LEP; typedef BEPair<p,LEP> BEP;
         typedef CONS<BEP,BE2> PBE;
         typedef CONS<BEQ,PBE> QBE;
         typedef CONS<BEU,QBE> UBE;
         typedef CONS<BEW,UBE> WBE;
         typedef CONS<BEV,WBE> VBE;
         typedef CONS<BEZ,VBE> ZBE;
         typedef CONS<BEY,ZBE> YBE;
         typedef CONS<BEX,YBE> BE;
         typedef typename ThunkifyType<X>::Result XTT;
         typedef TEPair<i,XTT> TEX;
         typedef typename ThunkifyType<Y>::Result YTT;
         typedef TEPair<j,YTT> TEY;
         typedef typename ThunkifyType<Z>::Result ZTT;
         typedef TEPair<k,ZTT> TEZ;
         typedef typename ThunkifyType<V>::Result VTT;
         typedef TEPair<l,VTT> TEV;
         typedef typename ThunkifyType<W>::Result WTT;
         typedef TEPair<m,WTT> TEW;
         typedef typename ThunkifyType<U>::Result UTT;
         typedef TEPair<n,UTT> TEU;
         typedef typename ThunkifyType<Q>::Result QTT;
         typedef TEPair<o,QTT> TEQ;
         typedef typename ThunkifyType<P>::Result PTT;
         typedef TEPair<p,PTT> TEP;
         typedef CONS<TEX,CONS<TEY,CONS<TEZ,CONS<TEV,CONS<TEW,CONS<TEU,CONS<TEQ,CONS<TEP,TE2> > > > > > > > TE;
         static inline BE go( const BE1& be, const X& x, const Y& y,
                              const Z& z , const V& v , const W& w, const U& u,
                              const Q& q, const P& pp) {
            return BE( BEX(LEX(x)), YBE( BEY(LEY(y)), ZBE( BEZ(LEZ(z)), VBE( BEV(LEV(v)),
                 WBE( BEW(LEW(w)), UBE( BEU(LEU(u)), QBE( BEQ(LEQ(q)), PBE( BEP(LEP(pp)),
		 ET2::template Go<Dummy>::go(be) ) ) ) ) ) ) ) );
         }
      };
   };
};

#endif

#endif

//////////////////////////////////////////////////////////////////////
// The lambda expression types, all defined right together here
// in one big batch
//////////////////////////////////////////////////////////////////////

namespace exp {

template <class X>
class LambdaExp : public LEBase {
   X x;
public:
   LambdaExp( const X& xx ) : x(xx) { EnsureLE<X>::go(); }

   typedef typename X::FreeVars FreeVars;
   template <class EnvThunk> struct MyType {
      typedef typename X::template MyType<EnvThunk>::TypeThunk TypeThunk;
   };
   template <class EnvThunk> struct RecheckType {
      typedef typename X::template RecheckType<EnvThunk>::Ok Ok;
   };

   template <class EnvThunk>
   typename MyType<EnvThunk>::TypeThunk::template Go<int>::Type 
   eval( const typename EnvThunk::template Go<int>::BE& be ) const {
      return x.template eval<EnvThunk>( be );
   }

   typedef LambdaExp This;
   template <class A> typename BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return BracketCallable<This,A>::go( *this, a ); }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("LambdaExp");
    }
#endif
};

#ifdef FCPP_LAMBDA_AUTOCURRY
  // **************************************************************
  // Idea for overcoming the problem of autocurry failing.
  // This works, using the object _A_ as the autocurry inside
  // a lambda instead of _. Now replaced by _b_.
  // I wanted to use _l_ but it would be difficult to read.
  // _a_ already being used in BoostFC++ and maybe here too.
  // I have put this together because of the remark in a comment 
  // at operator, below about the way in which Koenig lookup works.
  // I came to the conclusion that what was needed was an autocurry 
  // object which inherits from LEBase and this is it.
  // I then added things to it which the compiler insisted upon,
  // making the following decisions.
  // FreeVars: follow Value
  // MyType  : set Type as AutoCurryType
  // eval    : return a copy of _
  // operator[] : not yet implemented, would be needed
  // for e.g.  lambda() [ _b_ [ ... ] ]
  // where the _b_ stood for a function name.
  // This then works and makes example 102 in testlambda.cpp
  // work correctly in a similar manner to example 100.
  // 
  // I like this solution as it uses the structure of the lambda code
  // and does not have to cut into the way in which lists are set up.
  //
  // **************************************************************
class AutoCurry : public LEBase {

  public:
   typedef NIL FreeVars;

   template <class EnvThunk> struct MyType { 
      struct TypeThunk {
         template <class Dummy> struct Go {
            typedef AutoCurryType Type; 
         };
      };
   };

   template <class EnvThunk> struct RecheckType { typedef int Ok; };
   template <class EnvThunk>
   typename MyType<EnvThunk>::TypeThunk::template Go<int>::Type 
   eval( const typename EnvThunk::template Go<int>::BE& ) const
   { 
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
      std::cout << "AutoCurry.eval called" << std::endl;
#endif
      return _; // should this be _?
   }
   /* First attempt to make _b_ [ ] work fails.
      Code complains of no Sig in AutoCurryType.
      There is a need to decode the call to find it.
      It is probably misconceived to attempt to do this at all.
      FC++ autocurrying does not work for function names in a call,
      only for arguments.
    */
   /* This does compile with concepts, but does not solve the problem. 
#ifdef FCPP_CONCEPTS
   typedef AutoCurry This;
   template <class A> typename BracketCallable<This,A>::Result
   operator[]( const A& a ) const
 { return BracketCallable<This,A>::go( *this, a ); }
#endif
   */
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("AutoCurry");
    }
#endif
};

  // Unfortunately _A_ is not consistent with the C++ standard
  // as it is _ followed by a capital letter.
  // I need to do something else.
AutoCurry _b_;

/*
#ifdef FCPP_CONCEPTS
} } // End two namespaces

// Idea
 
using fcpp_lambda::exp::AutoCurry;

template <class F,class Y>
  concept_map ConstCallable2<F,AutoCurry,Y>
{
  typedef Full1<impl::binder2of2<F,Y> > result_type;
};

// Restart the namespaces
namespace fcpp_lambda {
  namespace exp {
#endif
*/
#endif


// ****************************************************************
//
// LambdaType<T> and lambdify are the workaround for the problem
// that operator, fails to detect some cases e.g. F[ inc, inc ].
// See comments after operator, for explanation of this.
// These are unconditionally part of the lambda code and
// exported at the end of this file.
// The purpose of this class is to wrap an object (normally an fcpp functoid).
// This makes a new object in the namespace of the lambda operations, 
// which is picked up by operator, and interpreted correctly.
//
// John Fletcher May 2008
//
// ****************************************************************
template <typename T>
class LambdaType : public LEBase {
   T object;
public:
  LambdaType( const T& x) : object (x) {}
   typedef NIL FreeVars;

   template <class EnvThunk> struct MyType { 
      struct TypeThunk {
         template <class Dummy> struct Go {
            typedef T Type; 
         };
      };
   };
   template <class EnvThunk> struct RecheckType { typedef int Ok; };
   template <class EnvThunk>
   typename MyType<EnvThunk>::TypeThunk::template Go<int>::Type 
   eval( const typename EnvThunk::template Go<int>::BE& ) const
     { 
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
        std::cout << "LambdaType.eval called" << std::endl;
#endif
        return object;
     }
   // Enable ordinary use. e.g. lambdainc()(arg)
   T operator()( void ) const { return object; }
   /*
   typedef T This;
   template <class A> typename BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return BracketCallable<This,A>::go( *this, a ); }
   */
#ifdef FCPP_DEBUG
    std::string name() const
    {
       std::string temp = std::string("LambdaType::");
       temp += debug_name_or_string(object,"Fun");
       return temp;
    }
#endif
};

template <typename T>
LambdaType<T> lambdify(const T &x) { return LambdaType<T>(x); }

template <class T>
class Value : public LEBase {
   T data;
public:
   Value( const T& x ) : data(x) {}

   typedef NIL FreeVars;
   template <class EnvThunk> struct MyType { 
      struct TypeThunk {
         template <class Dummy> struct Go {
            typedef T Type; 
         };
      };
   };
   template <class EnvThunk> struct RecheckType { typedef int Ok; };
   template <class EnvThunk>
   typename MyType<EnvThunk>::TypeThunk::template Go<int>::Type 
   eval( const typename EnvThunk::template Go<int>::BE& ) const
   { return data; }

   typedef Value This;
   template <class A> typename BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return BracketCallable<This,A>::go( *this, a ); }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      std::string temp = std::string("Value<T> ( ");
      if ( debug_traits_has_name<T>() ) {
        temp += debug_traits_name(data);
      } else {
        temp += argument_traits_value(data);
      }
      temp +=  std::string(" )");
      return temp;
    }
#endif

};

template <int i>
class LambdaVar : public LEBase {
   // 'ii' is used to get around what appears to be a g++ bug...
public:
   template <class Dummy, int ii, class TE> struct Lookup;
   template <class D, int ii, class T, class Rest> 
   struct Lookup<D,ii,CONS<TEPair<ii,T>,Rest> >
   { typedef typename T::template Go<int>::Type Type; };
   template <class D, int ii, class TEP, class Rest> 
   struct Lookup<D,ii,CONS<TEP,Rest> >
   { typedef typename Lookup<D,ii,Rest>::Type Type; };

private:
   template <class Dummy, int ii, class Result, class EnvThunk, 
             class BE, class WBE> struct Find;
   template <class D, int ii, class R, class ET, class LE, class Rest, 
             class WBE>
   struct Find<D,ii,R,ET,CONS<BEPair<ii,LE>,Rest>,WBE> { 
      static inline R go( const CONS<BEPair<ii,LE>,Rest>& be,
                          const WBE& wbe ) 
      { return be.head.value.template eval<ET>(wbe); }
   };
   template <class D, int ii, class R, class ET, class BEP, 
             class Rest, class WBE> 
   struct Find<D,ii,R,ET,CONS<BEP,Rest>,WBE> { 
      static inline R go( const CONS<BEP,Rest>& be, const WBE& wbe ) {
         return Find<D,ii,R,ET,Rest,WBE>::go( be.tail, wbe );
      }
   };
public:
   typedef CONS<LambdaVar,NIL> FreeVars;
   template <class EnvThunk> struct MyType { 
      struct TypeThunk {
         template <class Dummy> struct Go {
            typedef typename EnvThunk::template Go<Dummy>::TE TE;
            typedef typename Lookup<int,i,TE>::Type Type;
         };
      };
   };
   template <class EnvThunk> struct RecheckType { typedef int Ok; };
   template <class EnvThunk>
   typename MyType<EnvThunk>::TypeThunk::template Go<int>::Type 
   eval( const typename EnvThunk::template Go<int>::BE& be ) const {
      typedef 
         typename MyType<EnvThunk>::TypeThunk::template Go<int>::Type Result;
      typedef typename EnvThunk::template Go<int>::BE BE;
      return Find<int,i,Result,EnvThunk,BE,BE>::go(be,be);
   }

   typedef LambdaVar This;
   template <class A> typename BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return BracketCallable<This,A>::go( *this, a ); }
#ifdef FCPP_DEBUG
    std::string name() const
    {
        std::ostringstream o;
        o << "LambdaVar<" << i << ">";
        return o.str();
    }
#endif

};

template <class Fun, class Args>
class Call : public LEBase {
   Fun fun;  Args args;
public:
   template <class ET, class F, class A> struct TypeHelper;
   template <class ET, class F>
   struct TypeHelper<ET,F,NIL> {
#ifdef FCPP_CONCEPTS
      typedef typename ConstCallable0<F>::result_type Type;
#else
      typedef typename RT<F>::ResultType Type;
#endif
   };
   template <class ET, class F, class A1>
   struct TypeHelper<ET,F,CONS<A1,NIL> > {
      typedef typename A1::template MyType<ET>::TypeThunk XTT;
      typedef typename XTT::template Go<int>::Type X;
#ifdef FCPP_CONCEPTS
      typedef typename ConstCallable1<F,X>::result_type Type;
#else
      typedef typename RT<F,X>::ResultType Type;
#endif
   };
   template <class ET, class F, class A1, class A2>
   struct TypeHelper<ET,F,CONS<A2,CONS<A1,NIL> > > {
      typedef typename A1::template MyType<ET>::TypeThunk XTT;
      typedef typename XTT::template Go<int>::Type X;
      typedef typename A2::template MyType<ET>::TypeThunk YTT;
      typedef typename YTT::template Go<int>::Type Y;
#ifdef FCPP_CONCEPTS
      typedef typename ConstCallable2<F,X,Y>::result_type Type;
#else
      // Test for Sig in F
      // This test is failing.
      //typedef typename SmartHasSig<FunctoidTraits<F >::has_sig>::Result Has_Sig;
      typedef typename RT<F,X,Y>::ResultType Type;
#endif
   };
   template <class ET, class F, class A1, class A2, class A3>
   struct TypeHelper<ET,F,CONS<A3,CONS<A2,CONS<A1,NIL> > > > {
      typedef typename A1::template MyType<ET>::TypeThunk XTT;
      typedef typename XTT::template Go<int>::Type X;
      typedef typename A2::template MyType<ET>::TypeThunk YTT;
      typedef typename YTT::template Go<int>::Type Y;
      typedef typename A3::template MyType<ET>::TypeThunk ZTT;
      typedef typename ZTT::template Go<int>::Type Z;
#ifdef FCPP_CONCEPTS
      typedef typename ConstCallable3<F,X,Y,Z>::result_type Type;
#else
      typedef typename RT<F,X,Y,Z>::ResultType Type;
#endif
   };
#ifdef FCPP152
   template <class ET, class F, class A1, class A2, class A3, class A4>
   struct TypeHelper<ET,F,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > {
      typedef typename A1::template MyType<ET>::TypeThunk XTT;
      typedef typename XTT::template Go<int>::Type X;
      typedef typename A2::template MyType<ET>::TypeThunk YTT;
      typedef typename YTT::template Go<int>::Type Y;
      typedef typename A3::template MyType<ET>::TypeThunk ZTT;
      typedef typename ZTT::template Go<int>::Type Z;
      typedef typename A4::template MyType<ET>::TypeThunk VTT;
      typedef typename VTT::template Go<int>::Type V;
#ifdef FCPP_CONCEPTS
      typedef typename ConstCallable4<F,X,Y,Z,V>::result_type Type;
#else
      typedef typename RT<F,X,Y,Z,V>::ResultType Type;
#endif
   };

   template <class ET, class F, class A1, class A2, class A3, class A4, class A5>
     struct TypeHelper<ET,F,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > >{
      typedef typename A1::template MyType<ET>::TypeThunk XTT;
      typedef typename XTT::template Go<int>::Type X;
      typedef typename A2::template MyType<ET>::TypeThunk YTT;
      typedef typename YTT::template Go<int>::Type Y;
      typedef typename A3::template MyType<ET>::TypeThunk ZTT;
      typedef typename ZTT::template Go<int>::Type Z;
      typedef typename A4::template MyType<ET>::TypeThunk VTT;
      typedef typename VTT::template Go<int>::Type V;
      typedef typename A5::template MyType<ET>::TypeThunk WTT;
      typedef typename WTT::template Go<int>::Type W;
#ifdef FCPP_CONCEPTS
      typedef typename ConstCallable5<F,X,Y,Z,V,W>::result_type Type;
#else
      typedef typename RT<F,X,Y,Z,V,W>::ResultType Type;
#endif
   };


   template <class ET, class F, class A1, class A2, class A3, class A4, class A5, class A6>
     struct TypeHelper<ET,F,CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > >{
      typedef typename A1::template MyType<ET>::TypeThunk XTT;
      typedef typename XTT::template Go<int>::Type X;
      typedef typename A2::template MyType<ET>::TypeThunk YTT;
      typedef typename YTT::template Go<int>::Type Y;
      typedef typename A3::template MyType<ET>::TypeThunk ZTT;
      typedef typename ZTT::template Go<int>::Type Z;
      typedef typename A4::template MyType<ET>::TypeThunk VTT;
      typedef typename VTT::template Go<int>::Type V;
      typedef typename A5::template MyType<ET>::TypeThunk WTT;
      typedef typename WTT::template Go<int>::Type W;
      typedef typename A6::template MyType<ET>::TypeThunk UTT;
      typedef typename UTT::template Go<int>::Type U;
#ifdef FCPP_CONCEPTS
      typedef typename ConstCallable6<F,X,Y,Z,V,W,U>::result_type Type;
#else
      typedef typename RT<F,X,Y,Z,V,W,U>::ResultType Type;
#endif
   };

#ifdef FCPP_LAMBDA7

   template <class ET, class F, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
     struct TypeHelper<ET,F,CONS<A7,CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > > >{
      typedef typename A1::template MyType<ET>::TypeThunk XTT;
      typedef typename XTT::template Go<int>::Type X;
      typedef typename A2::template MyType<ET>::TypeThunk YTT;
      typedef typename YTT::template Go<int>::Type Y;
      typedef typename A3::template MyType<ET>::TypeThunk ZTT;
      typedef typename ZTT::template Go<int>::Type Z;
      typedef typename A4::template MyType<ET>::TypeThunk VTT;
      typedef typename VTT::template Go<int>::Type V;
      typedef typename A5::template MyType<ET>::TypeThunk WTT;
      typedef typename WTT::template Go<int>::Type W;
      typedef typename A6::template MyType<ET>::TypeThunk UTT;
      typedef typename UTT::template Go<int>::Type U;
      typedef typename A7::template MyType<ET>::TypeThunk QTT;
      typedef typename QTT::template Go<int>::Type Q;
#ifdef FCPP_CONCEPTS
      typedef typename ConstCallable7<F,X,Y,Z,V,W,U,Q>::result_type Type;
#else
      typedef typename RT<F,X,Y,Z,V,W,U,Q>::ResultType Type;
#endif
   };

   // Lambda 8
   template <class ET, class F, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
     struct TypeHelper<ET,F,CONS<A8,CONS<A7,CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > > > >{
      typedef typename A1::template MyType<ET>::TypeThunk XTT;
      typedef typename XTT::template Go<int>::Type X;
      typedef typename A2::template MyType<ET>::TypeThunk YTT;
      typedef typename YTT::template Go<int>::Type Y;
      typedef typename A3::template MyType<ET>::TypeThunk ZTT;
      typedef typename ZTT::template Go<int>::Type Z;
      typedef typename A4::template MyType<ET>::TypeThunk VTT;
      typedef typename VTT::template Go<int>::Type V;
      typedef typename A5::template MyType<ET>::TypeThunk WTT;
      typedef typename WTT::template Go<int>::Type W;
      typedef typename A6::template MyType<ET>::TypeThunk UTT;
      typedef typename UTT::template Go<int>::Type U;
      typedef typename A7::template MyType<ET>::TypeThunk QTT;
      typedef typename QTT::template Go<int>::Type Q;
      typedef typename A8::template MyType<ET>::TypeThunk PTT;
      typedef typename PTT::template Go<int>::Type P;
#ifdef FCPP_CONCEPTS
      typedef typename ConstCallable8<F,X,Y,Z,V,W,U,Q,P>::result_type Type;
#else
      typedef typename RT<F,X,Y,Z,V,W,U,Q,P>::ResultType Type;
#endif
   };

#endif

#endif
   template <class ET, class LEL> struct CheckHelper;
   template <class ET>
     struct CheckHelper<ET,NIL> { typedef int Result; };
   template <class ET, class H, class T>
   struct CheckHelper<ET,CONS<H,T> > {
      typedef typename AlwaysFirst<typename H::template RecheckType<ET>::Ok,
         typename CheckHelper<ET,T>::Result>::Type Result;
   };

   template <class Result, class ET, class BE, class Args_> struct EvalHelp;
   template <class R, class ET, class BE>
   struct EvalHelp<R,ET,BE,NIL> {
      static inline R go( const Fun& f, const NIL&, const BE& be ) {
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
         std::cout << "EvalHelp called with 0 args" << std::endl;
#endif
         return (f.template eval<ET>(be))();
      }
   };
   template <class R, class ET, class BE, class A1>
   struct EvalHelp<R,ET,BE,CONS<A1,NIL> > {
     //#ifdef FCPP_CONCEPTS
     // Idea for autocurry problem but breaks other code.
     //static inline ConstCallable1<R,CONS<A1,NIL> >::result_type
     //#else
      static inline R
     //#endif
      go( const Fun& f, const CONS<A1,NIL>& a, const BE& be ) {
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
         std::cout << "EvalHelp called with 1 args" << std::endl;
#endif
         return (f.template eval<ET>(be))(a.head.template eval<ET>(be));
      }
   };
   template <class R, class ET, class BE, class A1, class A2>
   struct EvalHelp<R,ET,BE,CONS<A2,CONS<A1,NIL> > > {
      static inline R go( const Fun& f, const CONS<A2,CONS<A1,NIL> >& a, 
                          const BE& be ) {
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
         std::cout << "EvalHelp called with 2 args" << std::endl;
         //a.tail.head.template eval<ET>(be) << " and " <<
         //a.head.template eval<ET>(be) << std::endl;
#endif
         return (f.template eval<ET>(be))(
            a.tail.head.template eval<ET>(be),
            a.head.template eval<ET>(be));
      }
   };
   template <class R, class ET, class BE, class A1, class A2, class A3> 
   struct EvalHelp<R,ET,BE,CONS<A3,CONS<A2,CONS<A1,NIL> > > > {
      static inline R go( const Fun& f, 
                          const CONS<A3,CONS<A2,CONS<A1,NIL> > >& a, 
                          const BE& be ) {
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
         std::cout << "EvalHelp called with 3 args" << std::endl;
#endif
         return (f.template eval<ET>(be))(
            a.tail.tail.head.template eval<ET>(be),
            a.tail.head.template eval<ET>(be),
            a.head.template eval<ET>(be));
      }
   };
#ifdef FCPP152
   template <class R, class ET, class BE, class A1, class A2, class A3, class A4> 
   struct EvalHelp<R,ET,BE,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > {
      static inline R go( const Fun& f, 
                          const CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > >& a, 
                          const BE& be ) {
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
         std::cout << "EvalHelp called with 4 args" << std::endl;
#endif
         return (f.template eval<ET>(be))(
            a.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.head.template eval<ET>(be),
            a.tail.head.template eval<ET>(be),
            a.head.template eval<ET>(be));
      }
   };

   template <class R, class ET, class BE, class A1, class A2, class A3, class A4, class A5> 
     struct EvalHelp<R,ET,BE,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > {
      static inline R go( const Fun& f, 
                          const CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > >& a, 
                          const BE& be ) {
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
         std::cout << "EvalHelp called with 5 args" << std::endl;
#endif
         return (f.template eval<ET>(be))(
            a.tail.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.head.template eval<ET>(be),
            a.tail.head.template eval<ET>(be),
            a.head.template eval<ET>(be));
      }
   };

   template <class R, class ET, class BE, class A1, class A2, class A3, class A4, class A5, class A6> 
     struct EvalHelp<R,ET,BE,CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > > {
      static inline R go( const Fun& f, 
                          const CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > >& a, 
                          const BE& be ) {
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
         std::cout << "EvalHelp called with 6 args" << std::endl;
#endif
         return (f.template eval<ET>(be))(
            a.tail.tail.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.head.template eval<ET>(be),
            a.tail.head.template eval<ET>(be),
            a.head.template eval<ET>(be));
      }
   };

#ifdef FCPP_LAMBDA7
   template <class R, class ET, class BE, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
     struct EvalHelp<R,ET,BE,CONS<A7,CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > > > {
      static inline R go( const Fun& f,
                          const CONS<A7,CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > >& a,
                          const BE& be ) {
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
         std::cout << "EvalHelp called with 7 args" << std::endl;
#endif
         return (f.template eval<ET>(be))(
            a.tail.tail.tail.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.head.template eval<ET>(be),
            a.tail.head.template eval<ET>(be),
            a.head.template eval<ET>(be));
      }
   };

   template <class R, class ET, class BE, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
     struct EvalHelp<R,ET,BE,CONS<A8,CONS<A7,CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > > > > {
      static inline R go( const Fun& f,
                          const CONS<A8,CONS<A7,CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > > >& a,
                          const BE& be ) {
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
         std::cout << "EvalHelp called with 8 args" << std::endl;
#endif
         return (f.template eval<ET>(be))(
            a.tail.tail.tail.tail.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.tail.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.tail.head.template eval<ET>(be),
            a.tail.tail.head.template eval<ET>(be),
            a.tail.head.template eval<ET>(be),
            a.head.template eval<ET>(be));
      }
   };

#endif

#endif
   struct FoldrOp {
      template <class LE, class FVList> struct Go {
         typedef typename AppendList<typename LE::FreeVars,FVList>::Result
            Result;
      };
   };
   typedef typename Foldr<FoldrOp,NIL,CONS<Fun,Args> >::Result
      AccumulatedFreeVars;

   Call( const Fun& f, const Args& a ) : fun(f), args(a) 
   { 
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
     std::cout << "Call called" << std::endl;
#endif
      EnsureLE<Fun>::go(); EnsureLEList<Args>::go(); 
   }

   typedef typename RemoveDuplicates<AccumulatedFreeVars>::Result FreeVars;
   template <class EnvThunk> struct MyType { 
      struct TypeThunk {
         template <class Dummy> struct Go {
            typedef typename Fun::template MyType<EnvThunk>::TypeThunk FTT;
            typedef typename FTT::template Go<Dummy>::Type F;
            typedef typename TypeHelper<EnvThunk,F,Args>::Type Type; 
         };
      };
   };
   template <class EnvThunk> struct RecheckType 
   { typedef typename CheckHelper<EnvThunk,CONS<Fun,Args> >::Result Ok; };
   template <class ET>
   typename MyType<ET>::TypeThunk::template Go<int>::Type 
   eval( const typename ET::template Go<int>::BE& be ) const {
      typedef typename ET::template Go<int>::BE BE;
      typedef typename MyType<ET>::TypeThunk::template Go<int>::Type Result;
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
     std::cout << "eval called" << std::endl;
#endif
      return EvalHelp<Result,ET,BE,Args>::go(fun,args,be);
   }

   typedef Call This;
   template <class A> typename BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return BracketCallable<This,A>::go( *this, a ); }
#ifdef FCPP_DEBUG
   // ArgHelp is here to decode the types and arguments of the call.
   // I have followed EvalHelp to put this together.
   // At the moment it works for 1 2 and 3 arguments.
 
   // ArgHelp decodes the types and N and
   // provides the functions for the arguments.
   // ArgHelper holds it all together and hides the details
   // from the top level name() itself.

   // I have put in Fun_ as an extra argument to ArgHelp for the N=0 case,
   // as it is within a struct and template<> is not allowed.

   // It is necessary to have go, go2 and go3 at the bottom level.
   // This is because the types of each argument can differ and
   // the correct type has to be returned.
   // It is not possible to switch the return types.

   // Also the first argument is in a different location in the cons list
   // of arguments which means that go() is different for different N.

   // It is ugly to have to give go2, go3 for cases where they are not needed.
   // This has now been rethought.
   // The new version uses the number of arguments to control the choice of a set
   // of structs called ArgsHelp which have the correct output choices for each number.
   // These pull each argument from the list which is in reverse order.

   // ArgHelp in now used to get the value of N the no of arguments.
   // It also supplies a Type.
   // I have given NIL the name "NIL" so that case N = 0 is output.
    static const  NIL nil;
    template <class Fun_,class Args_> struct ArgHelp {
      typedef NIL Type;
      static const int N = -1;  // Fail value.
    };

    template <class F> struct ArgHelp<F,NIL> {
      typedef NIL Type;
      static const int N = 0;
    };

    template <class F,class A1>
    struct ArgHelp<F,CONS<A1,NIL> > {
      typedef A1 Type;
      static const int N = 1;
    };

    template <class F,class A1, class A2>
      struct ArgHelp<F,CONS<A2,CONS<A1,NIL> > >  {
      typedef A1 Type;
      static const int N = 2;
    };

    template <class F,class A1, class A2, class A3>
    struct ArgHelp<F,CONS<A3,CONS<A2,CONS<A1,NIL> > > >  {
      typedef A1 Type;
      static const int N = 3;
    };

#ifdef FCPP152

    // This must be extended for each new size.
    template <class F,class A1, class A2, class A3, class A4>
      struct ArgHelp<F,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > {
      typedef A1 Type;
     static const int N = 4;
    };

    // This must be extended for each new size.
    template <class F,class A1, class A2, class A3, class A4, class A5>
      struct ArgHelp<F,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > {
      typedef A1 Type;
     static const int N = 5;
    };

    // This must be extended for each new size.
    template <class F,class A1, class A2, class A3, class A4, class A5, class A6>
      struct ArgHelp<F,CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > > {
      typedef A1 Type;
     static const int N = 6;
    };

#endif

    template <int N,class Fun_, class Args_> struct ArgsHelp;

    template <class Fun_,class Args_> struct ArgsHelp<0,Fun_,Args_> {
       static std::string output(const Args_ &args_) {
           return std::string("[  ]");
       }
    };

    template <class F,class A1>
    struct ArgsHelp<1,F,CONS<A1,NIL> > {
       static A1 arg1(const CONS<A1,NIL> &a) {
          return a.head;
       }
       static std::string output(const CONS<A1,NIL> &args_) {
           std::string temp;
            temp += std::string("[ ");
            temp += debug_traits_name_and_string(arg1(args_)," ]");  // <A1>
            return temp;
       }
    };

    template <class F,class A1,class A2>
    struct ArgsHelp<2,F,CONS<A2,CONS<A1,NIL> > > {
       static A1 arg1(const CONS<A2,CONS<A1,NIL> > &a) {
          return a.tail.head;
       }
       static A2 arg2(const CONS<A2,CONS<A1,NIL> > &a) {
          return a.head;
       }
       static std::string output(const CONS<A2,CONS<A1,NIL> > &args_) {
            std::string temp;
            temp += std::string("[ ");
            temp += debug_traits_name_and_string(arg1(args_)," , "); // <A1>
            temp += debug_traits_name_and_string(arg2(args_)," ]");  // <A2>
            return temp;
       }
    };
    
    template <class F,class A1,class A2,class A3>
    struct ArgsHelp<3,F,CONS<A3,CONS<A2,CONS<A1,NIL> > > > {
       static A1 arg1(const CONS<A3,CONS<A2,CONS<A1,NIL> > > &a) {
          return a.tail.tail.head;
       }
       static A2 arg2(const CONS<A3,CONS<A2,CONS<A1,NIL> > > &a) {
          return a.tail.head;
       }
       static A3 arg3(const CONS<A3,CONS<A2,CONS<A1,NIL> > > &a) {
          return a.head;
       }
        static std::string output(const CONS<A3,CONS<A2,CONS<A1,NIL> > > &args_) {
            std::string temp;
            temp += std::string("[ ");
            temp += debug_traits_name_and_string(arg1(args_)," , "); // <A1>
            temp += debug_traits_name_and_string(arg2(args_)," , "); // <A2>
            temp += debug_traits_name_and_string(arg3(args_)," ]");  // <A3>
            return temp;
       }
    };

#ifdef FCPP152

    template <class F,class A1,class A2,class A3, class A4>
    struct ArgsHelp<4,F,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > >{
       static A1 arg1(const CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > &a) {
          return a.tail.tail.tail.head;
       }
       static A2 arg2(const CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > &a) {
          return a.tail.tail.head;
       }
       static A3 arg3(const CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > &a) {
          return a.tail.head;
       }
       static A4 arg4(const CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > &a) {
          return a.head;
       }
        static std::string output(const CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > &args_) {
            std::string temp;
            temp += std::string("[ ");
	    temp += debug_traits_name_and_string(arg1(args_)," , "); // <A1>
	    temp += debug_traits_name_and_string(arg2(args_)," , "); // <A2>
	    temp += debug_traits_name_and_string(arg3(args_)," , "); // <A3>
	    temp += debug_traits_name_and_string(arg4(args_)," ]");  // <A4>
	    return temp;
       }
    };

    template <class F,class A1,class A2,class A3, class A4, class A5>
    struct ArgsHelp<5,F,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > {
       static A1 arg1(const CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > &a) {
          return a.tail.tail.tail.tail.head;
       }
       static A2 arg2(const CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > &a) {
          return a.tail.tail.tail.head;
       }
       static A3 arg3(const CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > &a) {
          return a.tail.tail.head;
       }
       static A4 arg4(const CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > &a) {
          return a.tail.head;
       }
       static A5 arg5(const CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > &a) {
          return a.head;
       }
        static std::string output(const CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > &args_) {
            std::string temp;
            temp += std::string("[ ");
	    temp += debug_traits_name_and_string(arg1(args_)," , "); // <A1>
	    temp += debug_traits_name_and_string(arg2(args_)," , "); // <A2>
	    temp += debug_traits_name_and_string(arg3(args_)," , "); // <A3>
	    temp += debug_traits_name_and_string(arg4(args_)," , "); // <A4>
	    temp += debug_traits_name_and_string(arg5(args_)," ]");  // <A5>
	    return temp;
       }
    };

    template <class F,class A1,class A2,class A3, class A4, class A5, class A6>
    struct ArgsHelp<6,F,CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > > {
       static A1 arg1(
       const CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > &a) {
          return a.tail.tail.tail.tail.tail.head;
       }
       static A2 arg2(
       const CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > &a) {
          return a.tail.tail.tail.tail.head;
       }
       static A3 arg3(
       const CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > &a) {
          return a.tail.tail.tail.head;
       }
       static A4 arg4(
       const CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > &a) {
          return a.tail.tail.head;
       }
       static A5 arg5(
       const CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > &a) {
          return a.tail.head;
       }
       static A6 arg6(
       const CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > &a) {
          return a.head;
       }
        static std::string output(const CONS<A6,CONS<A5,CONS<A4,CONS<A3,CONS<A2,CONS<A1,NIL> > > > > > &args_) {
            std::string temp;
            temp += std::string("[ ");
	    temp += debug_traits_name_and_string(arg1(args_)," , "); // <A1>
	    temp += debug_traits_name_and_string(arg2(args_)," , "); // <A2>
	    temp += debug_traits_name_and_string(arg3(args_)," , "); // <A3>
	    temp += debug_traits_name_and_string(arg4(args_)," , "); // <A4>
	    temp += debug_traits_name_and_string(arg5(args_)," , "); // <A5>
	    temp += debug_traits_name_and_string(arg6(args_)," ]");  // <A6>
	    return temp;
       }
    };

#endif

    template <class Fun_, class Args_> struct ArgsHelper {
         static std::string output(const Args_ &args_) {
           return ArgsHelp<ArgHelp<Fun,Args>::N,Fun_,Args_>::output(args_);
	 }
    };

    std::string name() const
    {
      std::string temp;
      const int N = ArgHelp<Fun,Args>::N;
      typedef typename ArgHelp<Fun,Args>::Type ArgType;
      //temp += std::string("Call< ");
      if ( debug_traits_has_name<Fun>() ) {
	 temp += fcpp::traits::debug_traits<Fun>::name(fun);
         // This test is a fudge as ArgType corresponds to A1.
         // For the case with 0 arguments it corresponds to NIL which has name "NIL".
         if ( debug_traits_has_name<ArgType>() ) {
	   temp += ArgsHelper<Fun,Args>::output(args);
         } else {
            temp += std::string("[ Args ]");
         }
      } else {
         temp += std::string("Fun ");
         if ( debug_traits_has_name<ArgType>() ) {
	   temp += ArgsHelper<Fun,Args>::output(args);
         } else {
            temp += std::string("[ Args ]");
         }
      }
      //temp += std::string(" >");
      return temp;
    }
#endif
};

// This is a weird special class only BindingEnvExp below uses...
template <class RealET, class LE, class FV, class BE1, class B>
class AddEnvLE : public LEBase {
   LE exp;
   BE1 be1;
   B b;
public:
   AddEnvLE( const LE& e, const BE1& x, const B& y ) : exp(e), be1(x), b(y) {}

   typedef FV FreeVars;

   template <class EnvThunk> struct MyType { 
      struct TypeThunk {
         template <class Dummy> struct Go {
            typedef typename LE::template MyType<RealET>::TypeThunk LETT;
            typedef typename LETT::template Go<Dummy>::Type Type; 
         };
      };
   };
   template <class EnvThunk> struct RecheckType { 
      typedef typename LE::template RecheckType<RealET>::Ok Ok; 
   };
   template <class EnvThunk>
   typename MyType<RealET>::TypeThunk::template Go<int>::Type 
   eval( const typename EnvThunk::template Go<int>::BE& ) const { 
      return exp.template eval<RealET>( RealET::template Go<int>::go(be1,b) );
   }

   typedef AddEnvLE This;
   template <class A> typename BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return BracketCallable<This,A>::go( *this, a ); }
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::ostringstream o;
        o << "AddEnvLE<RealET,LE,FV,BE1,B>::";
        return o.str();
	//return std::string("LambdaVar<" << i << ">::");
    }
#endif

};

template <class B, class LE>
class BindingEnvExp : public LEBase {    // letrec semantics
   B binders;
   LE exp;

   template <class Dummy, class BinderList> struct AccumFree;
   template <class D> struct AccumFree<D,NIL> { typedef NIL Result; };
   template <class D, int i, class LEa, class Rest> 
   struct AccumFree<D,CONS<Binder<i,LEa>,Rest> >
   { typedef typename AppendList<typename LEa::FreeVars,
        typename AccumFree<D,Rest>::Result>::Result Result; };
   
   template <class Dummy, class BinderList> struct AccumBound;
   template <class D> struct AccumBound<D,NIL> { typedef NIL Result; };
   template <class D, int i, class LEa, class Rest> 
   struct AccumBound<D,CONS<Binder<i,LEa>,Rest> >
   { typedef CONS<LambdaVar<i>,typename AccumBound<D,Rest>::Result> Result; };
   
public:
   typedef typename AccumBound<int,B>::Result NewlyBoundVars;
   typedef typename AccumFree<int,B>::Result FreeVarsInBinders;

   // In order to compute the environment for "exp", we must follow
   // these steps:
   //  - Call the outer env E1
   //  - foreach lv in lhs of binders, erase lv-entries from E1 => E2
   //  - foreach binder in binders, add <binder.lhs,AddEnvLE<E3>(binder.rhs)> 
   //    BE/TE pair to E2 => E3
   // Now exp should be evaled in E3.

   template <class Inner, class ET3>
   struct UnusualTTHelper {
      template <class DumDum> struct Go {
         typedef typename Inner::template MyType<ET3>::TypeThunk ITT;
         typedef typename ITT::template Go<DumDum>::Type Type;
      };
   };

   // Note the trick; we take the very result we are computing (ET3) as
   // a template parameter!  Ha ha!  Darn I'm clever.  :)
   template <class ET2, class ET3, class BinderList, class NBV, 
             class BE1, class BB> struct Env3FromEnv2;
   template <class ET2, class ET3, class NBV, class BE1, class BB> 
   struct Env3FromEnv2<ET2,ET3,NIL,NBV,BE1,BB> {
      struct Result {
         template <class Dummy> struct Go {
            typedef typename ET2::template Go<Dummy>::TE TE;
            typedef typename ET2::template Go<Dummy>::BE BE;
            static inline BE go( const BE& x, const NIL&, const BE1&,
                                 const BB& ) { return x; }
         };
      };
   };
   template <class ET2, class ET3, int i, class LEa, class Rest, class NBV, 
             class BE1, class BB> 
   struct Env3FromEnv2<ET2,ET3,CONS<Binder<i,LEa>,Rest>,NBV,BE1,BB> {
      struct Result {
         template <class Dummy> struct Go {
            typedef typename ET2::template Go<Dummy>::BE BE2;
            typedef typename 
               Env3FromEnv2<ET2,ET3,Rest,NBV,BE1,BB>::Result Recurse;
            typedef typename 
               ListDifference<typename LEa::FreeVars,NBV>::Result LEFV;
            // Inner will capture the letrec environment (by value)
            typedef AddEnvLE<ET3,LEa,LEFV,BE1,BB> Inner;
            typedef BEPair<i,Inner> BEP;
            typedef CONS<BEP,typename Recurse::template Go<Dummy>::BE> BE;
            static inline BE go( const BE2& be, 
                                 const CONS<Binder<i,LEa>,Rest>& binders,
                                 const BE1& be1, const BB& b ) { 
               return BE( BEP(Inner(binders.head.exp,be1,b)),
                  Recurse::template Go<Dummy>::go(be,binders.tail,be1,b) );
            }
            typedef UnusualTTHelper<Inner,ET3> TT;
            typedef TEPair<i,TT> TEP;
            typedef CONS<TEP,typename Recurse::template Go<Dummy>::TE> TE;
         };
      };
   };

   template <class OrigET>
   struct MakeNewET {
      struct Result {
         template <class Dummy> struct Go {
            typedef typename OrigET::template Go<Dummy>::BE BE1;
            typedef typename EraseLVsFromET<OrigET,NewlyBoundVars>::Result E2;
            // Here is the trick to tie the recursive knot:
            typedef typename 
               Env3FromEnv2<E2,Result,B,NewlyBoundVars,BE1,B>::Result E3;
//            typedef typename OrigET::template Go<Dummy>::BE BE1;
            typedef typename E3::template Go<Dummy>::BE BE;
            typedef typename E3::template Go<Dummy>::TE TE;
            static inline BE go( const BE1& be, const B& binders ) { 
               return E3::template Go<Dummy>::go( 
                  E2::template Go<Dummy>::go(be), binders, be, binders );
            }
         };
      };
   };
      
private:
   template <class NewET, class BinderList> struct CheckHelp;
   template <class ET> struct CheckHelp<ET,NIL> { typedef int Result; };
   template <class ET, int i, class LEa, class Rest> 
   struct CheckHelp<ET,CONS<Binder<i,LEa>,Rest> > { 
      typedef typename LEa::template RecheckType<ET>::Ok ThisOne;
      typedef typename AlwaysFirst<ThisOne,
         typename CheckHelp<ET,Rest>::Result>::Type Result;
   };
public:
   BindingEnvExp( const B& g, const LE& e ) : binders(g), exp(e) 
   { EnsureBinderList<B>::go(); EnsureLE<LE>::go(); }

   typedef typename RemoveDuplicates<typename ListDifference<
      typename AppendList<FreeVarsInBinders,typename LE::FreeVars>::Result,
      NewlyBoundVars>::Result>::Result FreeVars;

   template <class ET> struct MyType { 
      typedef typename LE::template MyType<typename
         MakeNewET<ET>::Result>::TypeThunk TypeThunk;
   };

   template <class ET> struct RecheckType { 
      typedef typename MakeNewET<ET>::Result NET;
      typedef typename AlwaysFirst<
         typename LE::template RecheckType<NET>::Ok,
         typename CheckHelp<NET,B>::Result>::Type Ok; 
   };

   template <class EnvThunk>
   typename MyType<EnvThunk>::TypeThunk::template Go<int>::Type 
   eval( const typename EnvThunk::template Go<int>::BE& be ) const {
      typedef typename MakeNewET<EnvThunk>::Result NET;
      return exp.template eval<NET>( NET::template Go<int>::go(be,binders) );
   }

   typedef BindingEnvExp This;
   template <class A> typename BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return BracketCallable<This,A>::go( *this, a ); }
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::ostringstream o;
        o << "BindingEnvExp<B,LE>::";
        return o.str();
	//return std::string("LambdaVar<" << i << ">::");
    }
#endif
};

template <class LE, class TBBV>  // To-Be-Bound Vars, in correct order
class LambdaExpWithFreeVars : public LEBase {
   // This class's name isn't great; nearly every kind of lambda
   // expression has free variables. Specifically, this class is about
   // such LEs which are created like so:
   //    lambda(X)[ f[X,Y] ]
   // LEs which, when evaluated, will result in a functoid which
   // captures a particular outer binding environment.

   LE exp;
public:
   template <class LEa, class ET>
   class Lambda0WithFreeVars : public CFunType<typename LEa::template 
            MyType<ET>::TypeThunk::template Go<int>::Type> {
      typedef typename ET::template Go<int>::BE BE;
      LEa exp;
      BE env;
   public:
      Lambda0WithFreeVars( const LEa& e, const BE& be ) : exp(e), env(be) {}
       
      typename AlwaysFirst<typename LEa::template MyType<ET>::TypeThunk
         ::template Go<int>::Type, typename LEa::template 
         RecheckType<ET>::Ok>::Type
      operator()() const {
         return exp.template eval<ET>( env );
      }
   };

   template <class LEa, class ET, int i>
   class Lambda1WithFreeVars {
   
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::ostringstream o;
        o << "Lambda1WithFreeVars<LEa, ET, " << i << ">::[ ";
	std::string temp = o.str();
        temp += debug_name_or_nothing(exp);
        temp += std::string(" ]");
	return temp;
    }
    #endif
      typedef typename ET::template Go<int>::BE BE;
      LEa exp;
      BE env;
      template <class X> struct NewET {
         typedef typename ETUpdateX<ET,i,X>::Result Result;
      };
   public:
      Lambda1WithFreeVars( const LEa& e, const BE& be ) : exp(e), env(be) {}
      template <class X> struct Sig : public FunType<X,
         typename LEa::template MyType<typename NewET<X>::Result>
         ::TypeThunk::template Go<int>::Type> {};
      template <class X>
      typename Sig<X>::ResultType
      operator()( const X& x ) const {
         return exp.template eval<typename NewET<X>::Result>
            ( NewET<X>::Result::template Go<int>::go( env, x ) );
      }
   };

   template <class LEa, class ET, int i, int j>
   class Lambda2WithFreeVars {
      typedef typename ET::template Go<int>::BE BE;
      LEa exp;
      BE env;
      template <class X, class Y> struct NewET {
         typedef typename ETUpdateXY<ET,i,X,j,Y>::Result Result;
      };
   public:
      Lambda2WithFreeVars( const LEa& e, const BE& be ) : exp(e), env(be) {}
      template <class X, class Y> struct Sig : public FunType<X,Y,
         typename LEa::template MyType<typename NewET<X,Y>::Result>
         ::TypeThunk::template Go<int>::Type> {};
      template <class X, class Y>
      typename Sig<X,Y>::ResultType
      operator()( const X& x, const Y& y ) const {
         return exp.template eval<typename NewET<X,Y>::Result>
            ( NewET<X,Y>::Result::template Go<int>::go( env, x, y ) );
      }
   };

   template <class LEa, class ET, int i, int j, int k>
   class Lambda3WithFreeVars {
      typedef typename ET::template Go<int>::BE BE;
      LEa exp;
      BE env;
      template <class X, class Y, class Z> struct NewET {
         typedef typename ETUpdateXYZ<ET,i,X,j,Y,k,Z>::Result Result;
      };
   public:
      Lambda3WithFreeVars( const LEa& e, const BE& be ) : exp(e), env(be) {}
      template <class X, class Y, class Z> struct Sig : public FunType<X,Y,Z,
         typename LEa::template MyType<typename NewET<X,Y,Z>::Result>
         ::TypeThunk::template Go<int>::Type> {};
      template <class X, class Y, class Z>
	//#ifdef FCPP_CONCEPTS
	//inline typename LEa::template MyType<typename NewET<X,Y,Z>::Result>
        // ::TypeThunk::template Go<int>::Type
	//#else
      typename Sig<X,Y,Z>::ResultType
	//#endif
      operator()( const X& x, const Y& y, const Z& z ) const {
#ifdef FCPP_PRINT_HELPERS
      std::cout << "Lambda3WithFreeVars called " << std::endl;
#endif
         return exp.template eval<typename NewET<X,Y,Z>::Result>
            ( NewET<X,Y,Z>::Result::template Go<int>::go( env, x, y, z ) );
      }
   };
#ifdef FCPP152
   template <class LEa, class ET, int i, int j, int k, int l>
   class Lambda4WithFreeVars {
      typedef typename ET::template Go<int>::BE BE;
      LEa exp;
      BE env;
      template <class X, class Y, class Z, class V> struct NewET {
         typedef typename ETUpdateXYZV<ET,i,X,j,Y,k,Z,l,V>::Result Result;
      };
   public:
      Lambda4WithFreeVars( const LEa& e, const BE& be ) : exp(e), env(be) {}
      template <class X, class Y, class Z, class V> struct Sig : public FunType<X,Y,Z,V,
         typename LEa::template MyType<typename NewET<X,Y,Z,V>::Result>
         ::TypeThunk::template Go<int>::Type> {};
      template <class X, class Y, class Z, class V>
      typename Sig<X,Y,Z,V>::ResultType
      operator()( const X& x, const Y& y, const Z& z, const V& v ) const {
         return exp.template eval<typename NewET<X,Y,Z,V>::Result>
            ( NewET<X,Y,Z,V>::Result::template Go<int>::go( env, x, y, z, v ) );
      }
   };

   template <class LEa, class ET, int i, int j, int k, int l, int m>
   class Lambda5WithFreeVars {
      typedef typename ET::template Go<int>::BE BE;
      LEa exp;
      BE env;
      template <class X, class Y, class Z, class V, class W> struct NewET {
	typedef typename ETUpdateXYZVW<ET,i,X,j,Y,k,Z,l,V,m,W>::Result Result;
      };
   public:
      Lambda5WithFreeVars( const LEa& e, const BE& be ) : exp(e), env(be) {}
	template <class X, class Y, class Z, class V, class W> struct Sig : 
      public FunType<X,Y,Z,V,W,
	typename LEa::template MyType<typename NewET<X,Y,Z,V,W>::Result>
         ::TypeThunk::template Go<int>::Type> {};
      template <class X, class Y, class Z, class V, class W>
	typename Sig<X,Y,Z,V,W>::ResultType
	operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w ) const {
	  // std::cout << "Lambda5WithFreeVars operator() called" << std::endl;
	return exp.template eval<typename NewET<X,Y,Z,V,W>::Result>
	  ( NewET<X,Y,Z,V,W>::Result::template Go<int>::go( env, x, y, z, v, w ) );
      }
   };

   template <class LEa, class ET, int i, int j, int k, int l, int m, int n>
   class Lambda6WithFreeVars {
      typedef typename ET::template Go<int>::BE BE;
      LEa exp;
      BE env;
      template <class X, class Y, class Z, class V, class W, class U> struct NewET {
	typedef typename ETUpdateXYZVWU<ET,i,X,j,Y,k,Z,l,V,m,W,n,U>::Result Result;
      };
   public:
      Lambda6WithFreeVars( const LEa& e, const BE& be ) : exp(e), env(be) {}
	template <class X, class Y, class Z, class V, class W, class U> struct Sig : 
      public FunType<X,Y,Z,V,W,U,
	typename LEa::template MyType<typename NewET<X,Y,Z,V,W,U>::Result>
         ::TypeThunk::template Go<int>::Type> {};
      template <class X, class Y, class Z, class V, class W, class U>
	typename Sig<X,Y,Z,V,W,U>::ResultType
	operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w, const U& u ) const {
	  // std::cout << "Lambda6WithFreeVars operator() called" << std::endl;
	return exp.template eval<typename NewET<X,Y,Z,V,W,U>::Result>
	  ( NewET<X,Y,Z,V,W,U>::Result::template Go<int>::go( env, x, y, z, v, w, u ) );
      }
   };

#ifdef FCPP_LAMBDA7
   template <class LEa, class ET, int i, int j, int k, int l, int m, int n, int o>
   class Lambda7WithFreeVars {
      typedef typename ET::template Go<int>::BE BE;
      LEa exp;
      BE env;
      template <class X, class Y, class Z, class V, class W, class U, class Q> struct NewET {
	typedef typename ETUpdateXYZVWUQ<ET,i,X,j,Y,k,Z,l,V,m,W,n,U,o,Q>::Result Result;
      };
   public:
      Lambda7WithFreeVars( const LEa& e, const BE& be ) : exp(e), env(be) {}
      template <class X, class Y, class Z, class V, class W, class U, class Q> struct Sig :
	public FunType<X,Y,Z,V,W,U,Q,
	typename LEa::template MyType<typename NewET<X,Y,Z,V,W,U,Q>::Result>
         ::TypeThunk::template Go<int>::Type> {};
      template <class X, class Y, class Z, class V, class W, class U, class Q>
	typename Sig<X,Y,Z,V,W,U,Q>::ResultType
	operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w, const U& u, const Q& q ) const {
	  // std::cout << "Lambda7WithFreeVars operator() called" << std::endl;
	return exp.template eval<typename NewET<X,Y,Z,V,W,U,Q>::Result>
	  ( NewET<X,Y,Z,V,W,U,Q>::Result::template Go<int>::go( env, x, y, z, v, w, u, q ) );
      }
   };

   template <class LEa, class ET, int i, int j, int k, int l, int m, int n, int o, int p>
   class Lambda8WithFreeVars {
      typedef typename ET::template Go<int>::BE BE;
      LEa exp;
      BE env;
      template <class X, class Y, class Z, class V, class W, class U, class Q, class P> struct NewET {
	typedef typename ETUpdateXYZVWUQP<ET,i,X,j,Y,k,Z,l,V,m,W,n,U,o,Q,p,P>::Result Result;
      };
   public:
      Lambda8WithFreeVars( const LEa& e, const BE& be ) : exp(e), env(be) {}
      template <class X, class Y, class Z, class V, class W, class U, class Q, class P> struct Sig :
	public FunType<X,Y,Z,V,W,U,Q,P,
	typename LEa::template MyType<typename NewET<X,Y,Z,V,W,U,Q,P>::Result>
         ::TypeThunk::template Go<int>::Type> {};
      template <class X, class Y, class Z, class V, class W, class U, class Q, class P>
	typename Sig<X,Y,Z,V,W,U,Q,P>::ResultType
	operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w, const U& u, const Q& q, const P& pp ) const {
	  // std::cout << "Lambda8WithFreeVars operator() called" << std::endl;
	return exp.template eval<typename NewET<X,Y,Z,V,W,U,Q,P>::Result>
	  ( NewET<X,Y,Z,V,W,U,Q,P>::Result::template Go<int>::go( env, x, y, z, v, w, u, q, pp ) );
      }
   };

#endif

#endif

   // Typedefs for Lambdai, Lambdaj and Lambdak are only used in with
   // FCPP_DEBUG. I have defined them unconditionally here and there may
   // be other uses for this information.
   template <class LEa, class ET, class TBBVars>
   struct NumBoundVarsHelp;
   template <class LEa, class ET>
   struct NumBoundVarsHelp<LEa,ET,NIL> {
      typedef Lambda0WithFreeVars<LEa,ET> Lam;
      typedef Full0<Lam> Full;
      typedef NIL Lambdai;
      typedef NIL Lambdaj;
      typedef NIL Lambdak;
   };
   template <class LEa, class ET, int i>
   struct NumBoundVarsHelp<LEa,ET,CONS<LambdaVar<i>,NIL> > {
      typedef Lambda1WithFreeVars<LEa,ET,i> Lam;
      typedef Full1<Lam> Full;
      typedef LambdaVar<i> Lambdai;
      typedef NIL Lambdaj;
      typedef NIL Lambdak;
   };
   template <class LEa, class ET, int i, int j>
   struct NumBoundVarsHelp<LEa,ET,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            NIL> > > {
      typedef Lambda2WithFreeVars<LEa,ET,i,j> Lam;
      typedef Full2<Lam> Full;
      typedef LambdaVar<i> Lambdai;
      typedef LambdaVar<j> Lambdaj;
      typedef NIL Lambdak;
   };
   template <class LEa, class ET, int i, int j, int k>
   struct NumBoundVarsHelp<LEa,ET,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>,NIL> > > > {
      typedef Lambda3WithFreeVars<LEa,ET,i,j,k> Lam;
      typedef Full3<Lam> Full;
      typedef LambdaVar<i> Lambdai;
      typedef LambdaVar<j> Lambdaj;
      typedef LambdaVar<k> Lambdak;
   };
#ifdef FCPP152
   template <class LEa, class ET, int i, int j, int k, int l>
   struct NumBoundVarsHelp<LEa,ET,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>,CONS<LambdaVar<l>,NIL> > > > > {
      typedef Lambda4WithFreeVars<LEa,ET,i,j,k,l> Lam;
      typedef Full4<Lam> Full;
   };

   template <class LEa, class ET, int i, int j, int k, int l, int m>
   struct NumBoundVarsHelp<LEa,ET,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>,CONS<LambdaVar<l>,CONS<LambdaVar<m>,NIL> > > > > > {
      typedef Lambda5WithFreeVars<LEa,ET,i,j,k,l,m> Lam;
      typedef Full5<Lam> Full;
   };

   template <class LEa, class ET, int i, int j, int k, int l, int m, int n>
   struct NumBoundVarsHelp<LEa,ET,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>,CONS<LambdaVar<l>,CONS<LambdaVar<m>,CONS<LambdaVar<n>,NIL> > > > > > > {
      typedef Lambda6WithFreeVars<LEa,ET,i,j,k,l,m,n> Lam;
      typedef Full6<Lam> Full;
   };

#ifdef FCPP_LAMBDA7

   template <class LEa, class ET, int i, int j, int k, int l, int m, int n, int o>
   struct NumBoundVarsHelp<LEa,ET,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>,CONS<LambdaVar<l>,CONS<LambdaVar<m>,CONS<LambdaVar<n>,CONS<LambdaVar<o>,NIL> > > > > > > > {
     typedef Lambda7WithFreeVars<LEa,ET,i,j,k,l,m,n,o> Lam;
      typedef Full7<Lam> Full;
   };

   template <class LEa, class ET, int i, int j, int k, int l, int m, int n, int o, int p>
   struct NumBoundVarsHelp<LEa,ET,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>,CONS<LambdaVar<l>,CONS<LambdaVar<m>,CONS<LambdaVar<n>,CONS<LambdaVar<o>,CONS<LambdaVar<p>,NIL> > > > > > > > > {
     typedef Lambda8WithFreeVars<LEa,ET,i,j,k,l,m,n,o,p> Lam;
      typedef Full8<Lam> Full;
   };

#endif
#endif

   LambdaExpWithFreeVars( const LE& e ) : exp(e) { EnsureLE<LE>::go(); }

   typedef typename RemoveDuplicates<typename ListDifference<typename
      LE::FreeVars,TBBV>::Result>::Result FreeVars;

   // Definition of Lambdai etc at the MyType level so that they can
   // be accessed when needed.
   template <class ET> struct MyType {
      typedef typename NumBoundVarsHelp<LE,ET,TBBV>::Full Full;
      typedef typename ThunkifyType<Full>::Result TypeThunk;
      typedef typename NumBoundVarsHelp<LE,ET,TBBV>::Lambdai Lambdai;
      typedef typename NumBoundVarsHelp<LE,ET,TBBV>::Lambdaj Lambdaj;
      typedef typename NumBoundVarsHelp<LE,ET,TBBV>::Lambdak Lambdak;
   };

   template <class ET> struct RecheckType
   { typedef typename LE::template RecheckType<ET>::Ok Ok; };

   template <class EnvThunk>
   typename MyType<EnvThunk>::TypeThunk::template Go<int>::Type
   eval( const typename EnvThunk::template Go<int>::BE& be ) const {
      typedef NumBoundVarsHelp<LE,EnvThunk,TBBV> NBVH;
      typedef typename NBVH::Lam Lam;
      typedef typename NBVH::Full Full;
      return Full( Lam(exp,be) );
   }

   YouCannotInvokeALambdaContainingFreeVars<FreeVars>
   operator()() const { return 0; }
   template <class X> 
   YouCannotInvokeALambdaContainingFreeVars<FreeVars>
   operator()(const X&) const { return 0; }
   template <class X, class Y> 
   YouCannotInvokeALambdaContainingFreeVars<FreeVars>
   operator()(const X&, const Y&) const { return 0; }
   template <class X, class Y, class Z> 
   YouCannotInvokeALambdaContainingFreeVars<FreeVars>
   operator()(const X&, const Y&, const Z&) const { return 0; }
#ifdef FCPP152
   template <class X, class Y, class Z, class V>
   YouCannotInvokeALambdaContainingFreeVars<FreeVars>
   operator()(const X&, const Y&, const Z&, const V&) const { return 0; }

   template <class X, class Y, class Z, class V, class W>
   YouCannotInvokeALambdaContainingFreeVars<FreeVars>
   operator()(const X&, const Y&, const Z&, const V&, const W& w) const { return 0; }

   template <class X, class Y, class Z, class V, class W, class U>
   YouCannotInvokeALambdaContainingFreeVars<FreeVars>
   operator()(const X&, const Y&, const Z&, const V&, const W& w,const U& u) const { return 0; }
 
#ifdef FCPP_LAMBDA7
   template <class X, class Y, class Z, class V, class W, class U, class Q>
   YouCannotInvokeALambdaContainingFreeVars<FreeVars>
   operator()(const X&, const Y&, const Z&, const V&, const W& w,const U& u, const Q& q) const { return 0; }

   template <class X, class Y, class Z, class V, class W, class U, class Q, class P>
   YouCannotInvokeALambdaContainingFreeVars<FreeVars>
     operator()(const X&, const Y&, const Z&, const V&, const W& w,const U& u, const Q& q, const P& pp) const { return 0; }

   template <class AA=Void, class BB=Void, class CC=Void, class DD=Void, class EE=Void, class FF=Void, class GG=Void, class HH=void>
   struct Sig {
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg1Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg2Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg3Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg4Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg5Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg6Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither ResultType;
   };

 
#else

   template <class AA=Void, class BB=Void, class CC=Void, class DD=Void, class EE=Void>
   struct Sig {
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg1Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg2Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg3Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg4Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg5Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg6Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither ResultType;
   };
#endif
#else
   template <class AA=Void, class BB=Void, class CC=Void> struct Sig {
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg1Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg2Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither Arg3Type;
      typedef typename YouCannotInvokeALambdaContainingFreeVars<FreeVars>
         ::AndThusYouShouldNotBeTryingToUseItsSigEither ResultType;
   };
#endif

   typedef LambdaExpWithFreeVars This;
   template <class A> typename BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return BracketCallable<This,A>::go( *this, a ); }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      // typedefs for the free variables.
       typedef typename MyType<LE>::Lambdai Lambdai;
       typedef typename MyType<LE>::Lambdaj Lambdaj;
       typedef typename MyType<LE>::Lambdak Lambdak;
       Lambdai li;
       std::string temp = std::string("LambdaExpWithFreeVars<LE,");
       // I have now added the numbers of the free variables here.
       if ( debug_traits_has_name<Lambdai>() ) {
            Lambdai li;
            temp += debug_traits_name(li);
            if ( debug_traits_has_name<Lambdaj>() ) {
               Lambdaj lj;
               temp += std::string(",");
               temp += debug_traits_name(lj);
	    }
            if ( debug_traits_has_name<Lambdak>() ) {
               Lambdak lk;
               temp += std::string(",");
               temp += debug_traits_name(lk);
	    }
       } else {
          temp += std::string("VARS");
       }
       temp += std::string(" >::[ ");
       if ( debug_traits_has_name<LE>() ) {
          temp += debug_traits_name(exp);
      }
       temp += std::string(" ]");
       return temp;
    }
#endif
};

template <class E, class T, class F, IfKind deduction_method>
class IfLE : public LEBase {
   E e;
   T t;
   F f;
public:
   template <class ET, IfKind deduct_method> struct XType;
   template <class ET, IfKind deduct_method> struct XRecheckType;

   // Normal if type deduction
   template <class ET> struct XType<ET,IfNormal> { 
      typedef typename T::template MyType<ET>::TypeThunk TypeThunk;
   };
   template <class ET> struct XRecheckType<ET,IfNormal> { 
      typedef typename E::template MyType<ET>::TypeThunk ETT; 
      typedef typename T::template MyType<ET>::TypeThunk TTT; 
      typedef typename F::template MyType<ET>::TypeThunk FTT; 
      typedef typename ETT::template Go<int>::Type EType;
      typedef typename TTT::template Go<int>::Type TType;
      typedef typename FTT::template Go<int>::Type FType;
      static const bool b = ImplicitlyConvertible<EType,bool>::value;
      typedef typename AlwaysFirst<typename AlwaysFirst<typename AlwaysFirst<
         typename AlwaysFirst<typename
         TrueAndFalseBranchOfIfMustHaveSameType<TType,FType>::Type, typename 
         IfExpressionMustHaveTypeConvertibleToBool<EType,b>::Type>::Type,
         typename E::template RecheckType<ET>::Ok>::Type,
         typename T::template RecheckType<ET>::Ok>::Type,
         typename F::template RecheckType<ET>::Ok>::Type Ok; 
   };

   // Type deduction based on true-branch
   template <class ET> struct XType<ET,IfTrue> { 
      typedef typename T::template MyType<ET>::TypeThunk TypeThunk;
   };
   template <class ET> struct XRecheckType<ET,IfTrue> { 
      typedef typename E::template MyType<ET>::TypeThunk ETT; 
      typedef typename T::template MyType<ET>::TypeThunk TTT; 
      typedef typename F::template MyType<ET>::TypeThunk FTT; 
      typedef typename ETT::template Go<int>::Type EType;
      typedef typename TTT::template Go<int>::Type TType;
      typedef typename FTT::template Go<int>::Type FType;
      static const bool bx = ImplicitlyConvertible<EType,bool>::value;
      typedef typename 
         IfExpressionMustHaveTypeConvertibleToBool<EType,bx>::Type Foo;
      static const bool b = ImplicitlyConvertible<FType,TType>::value;
      typedef typename
         ActualTypeOfFalseBranchMustBeImplicitlyConvertibleToTypeOfTrueBranch
         <FType,TType,b>::Error Tmp;
      typedef typename AlwaysFirst<typename AlwaysFirst<
         typename AlwaysFirst<typename AlwaysFirst<Tmp,Foo>::Type,
         typename E::template RecheckType<ET>::Ok>::Type,
         typename T::template RecheckType<ET>::Ok>::Type,
         typename F::template RecheckType<ET>::Ok>::Type Ok; 
   };

   // Type deduction based on false-branch
   template <class ET> struct XType<ET,IfFalse> { 
      typedef typename F::template MyType<ET>::TypeThunk TypeThunk;
   };
   template <class ET> struct XRecheckType<ET,IfFalse> { 
      typedef typename E::template MyType<ET>::TypeThunk ETT; 
      typedef typename T::template MyType<ET>::TypeThunk TTT; 
      typedef typename F::template MyType<ET>::TypeThunk FTT; 
      typedef typename ETT::template Go<int>::Type EType;
      typedef typename TTT::template Go<int>::Type TType;
      typedef typename FTT::template Go<int>::Type FType;
      static const bool bx = ImplicitlyConvertible<EType,bool>::value;
      typedef typename 
         IfExpressionMustHaveTypeConvertibleToBool<EType,bx>::Type Foo;
      static const bool b = ImplicitlyConvertible<TType,FType>::value;
      typedef typename
         ActualTypeOfTrueBranchMustBeImplicitlyConvertibleToTypeOfFalseBranch
         <TType,FType,b>::Error Tmp;
      typedef typename AlwaysFirst<typename AlwaysFirst<
         typename AlwaysFirst<typename AlwaysFirst<Tmp,Foo>::Type,
         typename E::template RecheckType<ET>::Ok>::Type,
         typename T::template RecheckType<ET>::Ok>::Type,
         typename F::template RecheckType<ET>::Ok>::Type Ok; 
   };

   IfLE( const E& ee, const T& tt, const F& ff ) : e(ee), t(tt), f(ff) 
   { EnsureLE<E>::go(); EnsureLE<T>::go(); EnsureLE<F>::go(); }

   typedef typename RemoveDuplicates<typename AppendList<typename E::FreeVars, 
      typename AppendList<typename T::FreeVars, typename F::FreeVars>::Result
      >::Result>::Result FreeVars;

   template <class ET> struct MyType 
   { typedef typename XType<ET,deduction_method>::TypeThunk TypeThunk; };

   template <class ET> struct RecheckType
   { typedef typename XRecheckType<ET,deduction_method>::Ok Ok; };

   template <class ET>
   typename MyType<ET>::TypeThunk::template Go<int>::Type 
   eval( const typename ET::template Go<int>::BE& be ) const {
      if( e.template eval<ET>(be) )
         return t.template eval<ET>(be);
      else
         return f.template eval<ET>(be);
   }

   typedef IfLE This;
   template <class A> typename BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return BracketCallable<This,A>::go( *this, a ); }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       std::string temp = std::string("IfLE<E,T,F,IfKind>::[ ");
       if ( debug_traits_has_name<E>() ) {
          temp += debug_traits_name(e);
       }
       if ( debug_traits_has_name<T>() ) {
          temp += std::string(" , ");
          temp += debug_traits_name(t);
       }
       if ( debug_traits_has_name<F>() ) {
          temp += std::string(" , ");
          temp += debug_traits_name(f);
       }
      temp += std::string(" ]");
       return temp;
    }
#endif
};

// operator, overloads
//
// Koenig lookup will only find these overloads if one of the arguments
// to comma is an LE (a type defined in this namespace).

// Either the LHS is already a CONS...
template <class H, class T, class RHS>
CONS<typename LEify<RHS>::Type,CONS<H,T> >
operator,( const CONS<H,T>& lhs, const RHS& rhs ) {
#ifdef FCPP_PRINT_HELPERS
   std::cout << "operator, Number 1 called for LE" << std::endl;
#endif
   return CONS<typename LEify<RHS>::Type,CONS<H,T> >
      ( LEify<RHS>::go(rhs), lhs );
}

// ... or it's not
template <class LHS, class RHS>
CONS<typename LEify<RHS>::Type,typename LEListify<LHS>::Type>
operator,( const LHS& lhs, const RHS& rhs ) {
#ifdef FCPP_PRINT_HELPERS
   std::cout << "operator, Number 2 called for LE" << std::endl;
#endif
   return CONS<typename LEify<RHS>::Type,typename LEListify<LHS>::Type>
      ( LEify<RHS>::go(rhs), LEListify<LHS>::go(lhs) );
}

// Why did I make this concepts only?
template <class LHS, class RHS>
CONS<typename LEify<LambdaType<RHS> >::Type,typename LEListify<LHS>::Type>
operator,( const LHS& lhs, const LambdaType<RHS>& rhs ) {
#ifdef FCPP_PRINT_HELPERS
   std::cout << "operator, Number 2L called for LE" << std::endl;
#endif
   return CONS<typename LEify<LambdaType<RHS> >::Type,typename LEListify<LHS>::Type>
      ( LEify<LambdaType<RHS> >::go(rhs), LEListify<LHS>::go(lhs) );
}

#ifdef FCPP_LAMBDA_AUTOCURRY
template <class LHS>
CONS<typename LEify<AutoCurry >::Type,typename LEListify<LHS>::Type>
operator,( const LHS& lhs, const AutoCurry& rhs ) {
#ifdef FCPP_PRINT_HELPERS
   std::cout << "operator, Number 2A called for LE" << std::endl;
#endif
   return CONS<typename LEify<AutoCurry >::Type,typename LEListify<LHS>::Type>
      ( LEify<AutoCurry >::go(rhs), LEListify<LHS>::go(lhs) );
}
#endif

//#ifdef FCPP_CONCEPTS
// ... special for AutoCurry when concepts are defined....
/* This is called for the case f[_b_,_b_] with concepts.
   It makes no difference to the subsequent processing. */
/*template <class RHS>
CONS<typename LEify<RHS>::Type,typename LEListify<AutoCurry>::Type>
operator,( const AutoCurry& lhs, const RHS& rhs ) {
#ifdef FCPP_PRINT_HELPERS
   std::cout << "operator, Number 3 called for LE" << std::endl;
#endif
   return CONS<typename LEify<RHS>::Type,typename LEListify<AutoCurry>::Type>
      ( LEify<RHS>::go(rhs), LEListify<AutoCurry>::go(lhs) );
      }
*/
/* Introducing this fails as it does not recognise Inc.
   However, it also introduced ambiguity into the name resolution
   for operator, for the case of [_b_,_b_] and I do not know why.
 */
/*
template <class RHS>
  CONS<typename LEify<RHS>::Type,typename LEListify<Inc>::Type>
operator,( const AutoCurry& lhs, const RHS& rhs ) {
#ifdef FCPP_PRINT_HELPERS
   std::cout << "operator, Number 4 called for LE" << std::endl;
#endif
   return CONS<typename LEify<RHS>::Type,typename LEListify<Inc>::Type>
      ( LEify<RHS>::go(rhs), LEListify<Inc>::go(lhs) );
}
  */
/*
template <class RHS>
  CONS<typename LEify<LambdaType<RHS> >::Type,typename LEListify<AutoCurry>::Type>
operator,( const AutoCurry& lhs, const LambdaType<RHS>& rhs ) {
#ifdef FCPP_PRINT_HELPERS
   std::cout << "operator, Number 5 called for LE" << std::endl;
#endif
   return CONS<typename LEify<LambdaType<RHS> >::Type,typename LEListify<AutoCurry>::Type>
      ( LEify<LambdaType<RHS> >::go(rhs), LEListify<AutoCurry>::go(lhs) );
}
*/
/* Another idea for a catchall for the operator, does not compile.
template <>
  //  CONS<typename LEify<LEBase>::Type,typename LEListify<LEBase>::Type>
LEBase
operator,( const LEBase& lhs, const LEBase& rhs ) {
#ifdef FCPP_PRINT_HELPERS
   std::cout << "operator, Number 6 called for LE" << std::endl;
#endif
   return lhs;
}
*/
//#endif

// *********************************************************************
//
// There is a further case where both arguments are fcpp functoids e.g.
// F [ inc, inc ] where F will become compose. This case fails, while
// F [ inc ] [ inc ] does not, precisely because the overload operator
// is not detected and therefore the first argument is ignored.
// Another overload of the operator has not (yet) been defined.
// Instead one or other (or both) of the operands can be converted to a 
// lambdified form which is recognised by the overload operator.
// There are two ways to do this. One is to write the following code:
//
// F [ lambdify(inc), inc ] or F [ inc, lambdify(inc) ]
//
// These  both work.
//
// If the lambdified object will be used more than once it can be named as
//
// LambdaType<Inc> lambdainc(inc);  
// 
// and used directly
//
// F [ lambdainc, inc ] or F [ inc, lambdainc ]
// 
// Both of these work too.
//
// Both LambdaType<T> and lambdify are defined earlier in this file 
// and exported at the end.
//
// I would like to devise a way to implement this feature without the 
// user needing to know about it but I have not found one, or even a way
// to issue a sensible warning.
//
// John Fletcher, May 2008.
//
// Note: clang 3.2 does issue a warning as in this example from testcontm.cpp:
//testcontm.cpp:793:29: warning: expression result unused [-Wunused-value]
//  outputR( lambda(F)   [ F [inc,inc] ](compose)(inc)(76) );    // 78
//                            ^~~
// The example does compile and run with an extra (inc) as an argument.
// This compiler was not available when I wrote the code. September 2013
// *********************************************************************

} // end namespace exp

//////////////////////////////////////////////////////////////////////
// lambda() and the functoids that get made when all the vars are bound
// and we make it back out into "C++ space"
//////////////////////////////////////////////////////////////////////

template <class LE>
class Lambda0 : public CFunType<typename LE::template
                   MyType<NIL_ET>::TypeThunk::template Go<int>::Type> {
   LE exp;
public:
   Lambda0( const LE& e ) : exp(e) { EnsureLE<LE>::go(); }
   typename LE::template MyType<NIL_ET>::TypeThunk::template Go<int>::Type
   operator()() const {
      return exp.template eval<NIL_ET>( NIL() );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::string temp = std::string("Lambda0<LE>::[ ");
        temp += debug_name_or_nothing(exp);
        temp += std::string(" ]");
	return temp;
   }
#endif
};
template <class LE, int i>
class Lambda1 {
   LE exp;
   template <class X> struct NewET 
   { typedef typename ETUpdateX<NIL_ET,i,X>::Result Result; };
public:
   Lambda1( const LE& e ) : exp(e) { EnsureLE<LE>::go(); }
   template <class X> struct Sig : public FunType<X,typename LE::template 
       MyType<typename NewET<X>::Result>::TypeThunk::template Go<int>::Type> {};
   template <class X>
#ifdef FCPP_CONCEPTS
     // A new try. The problem is to identify what should replace F here.
    typename LE::template MyType<typename NewET<X>::Result>::TypeThunk::template Go<int>::Type
     //typename Sig<X>::ResultType
#else
     // I think the answer for now is to rely on this 
     // local Sig which decodes above, and accesses the modifications
     // made in the Call section above.
   typename Sig<X>::ResultType
#endif
   operator()( const X& x ) const {
      typedef typename NewET<X>::Result NET;
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
      //BOOST_MPL_ASSERT((is_same_type<AutoCurryType,NET>::type));
      std::cout << "Lambda1 operator() called" << std::endl;
      if (is_same_type<AutoCurryType,NET>::value) 
        std::cout << "NET is AutoCurryType in Lambda1.operator()" << std::endl;
#endif
      return exp.template eval<NET>( NET::template Go<int>::go(NIL(),x) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::ostringstream o;
        o << "Lambda1<LE," << i << ">::[ ";
	std::string temp = o.str();
        temp += debug_name_or_nothing(exp);
        temp += std::string(" ]");
	return temp;
    }
#endif
 };
template <class LE, int i, int j>
class Lambda2 {
   LE exp;
   template <class X, class Y> struct NewET 
   { typedef typename ETUpdateXY<NIL_ET,i,X,j,Y>::Result Result; };
public:
   Lambda2( const LE& e ) : exp(e) { EnsureLE<LE>::go(); }
   template <class X, class Y> struct Sig : public FunType<X,Y, 
      typename LE::template MyType<typename NewET<X,Y>::Result>
      ::TypeThunk::template Go<int>::Type> {};
   template <class X, class Y>
   typename Sig<X,Y>::ResultType
   operator()( const X& x, const Y& y ) const {
      typedef typename NewET<X,Y>::Result NET;
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
      std::cout << "Lambda2 operator() called" << std::endl;
#endif
      return exp.template eval<NET>( NET::template Go<int>::go(NIL(),x,y) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::ostringstream o;
        o << "Lambda2<LE," << i << "," << j << ">::[ ";
	std::string temp = o.str();
        temp += debug_name_or_nothing(exp);
        temp += std::string(" ]");
	return temp;
    }
#endif
};
template <class LE, int i, int j, int k>
class Lambda3 {
   LE exp;
   template <class X, class Y, class Z> struct NewET 
   { typedef typename ETUpdateXYZ<NIL_ET,i,X,j,Y,k,Z>::Result Result; };
public:
   Lambda3( const LE& e ) : exp(e) { EnsureLE<LE>::go(); }
   template <class X, class Y, class Z> struct Sig : public FunType<X,Y,Z,
      typename LE::template MyType<typename NewET<X,Y,Z>::Result>
      ::TypeThunk::template Go<int>::Type> {};
   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType
   operator()( const X& x, const Y& y, const Z& z ) const {
      typedef typename NewET<X,Y,Z>::Result NET;
#ifdef FCPP_PRINT_HELPERS
      std::cout << "Lambda3 called " << std::endl;
#endif
      return exp.template eval<NET>( NET::template Go<int>::go(NIL(),x,y,z) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::ostringstream o;
        o << "Lambda3<LE," << i << "," << j << "," << k << ">::";
 	std::string temp = o.str();
        temp += debug_name_or_nothing(exp);
        temp += std::string(" ]");
	return temp;
    }
#endif
};
#ifdef FCPP152
template <class LE, int i, int j, int k, int l>
class Lambda4 {
   LE exp;
   template <class X, class Y, class Z, class V> struct NewET 
   { typedef typename ETUpdateXYZV<NIL_ET,i,X,j,Y,k,Z,l,V>::Result Result; };
public:
   Lambda4( const LE& e ) : exp(e) { EnsureLE<LE>::go(); }
   template <class X, class Y, class Z, class V> struct Sig : public FunType<X,Y,Z,V,
      typename LE::template MyType<typename NewET<X,Y,Z,V>::Result>
      ::TypeThunk::template Go<int>::Type> {};
   template <class X, class Y, class Z,class V>
   typename Sig<X,Y,Z,V>::ResultType
   operator()( const X& x, const Y& y, const Z& z, const V& v ) const {
      typedef typename NewET<X,Y,Z,V>::Result NET;
      return exp.template eval<NET>( NET::template Go<int>::go(NIL(),x,y,z,v) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::ostringstream o;
        o << "Lambda4<LE," << i << "," << j << "," << k << "," << l << ">::";
 	std::string temp = o.str();
        temp += debug_name_or_nothing(exp);
        temp += std::string(" ]");
	return temp;
    }
#endif
};

// This needs a class Lambda5 and various other things as well.
 template <class LE, int i, int j, int k, int l, int m>
class Lambda5 {
   LE exp;
   template <class X, class Y, class Z, class V, class W> struct NewET
   { typedef typename ETUpdateXYZVW<NIL_ET,i,X,j,Y,k,Z,l,V,m,W>::Result Result; };
public:
   Lambda5( const LE& e ) : exp(e) { EnsureLE<LE>::go(); }
     template <class X, class Y, class Z, class V, class W> struct Sig :
   public FunType<X,Y,Z,V,W,
     typename LE::template MyType<typename NewET<X,Y,Z,V,W>::Result>
      ::TypeThunk::template Go<int>::Type> {};
   template <class X, class Y, class Z,class V,class W>
     typename Sig<X,Y,Z,V,W>::ResultType
     operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w ) const {
     typedef typename NewET<X,Y,Z,V,W>::Result NET;
     return exp.template eval<NET>( NET::template Go<int>::go(NIL(),x,y,z,v,w) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::ostringstream o;
        o << "Lambda5<LE," << i << "," << j << "," << k << "," << l << "," << m << ">::";
 	std::string temp = o.str();
        temp += debug_name_or_nothing(exp);
       temp += std::string(" ]");
	return temp;
    }
#endif
};

// This needs a class Lambda6 and various other things as well.
 template <class LE, int i, int j, int k, int l, int m, int n>
class Lambda6 {
   LE exp;
   template <class X, class Y, class Z, class V, class W, class U> struct NewET
   { typedef typename ETUpdateXYZVWU<NIL_ET,i,X,j,Y,k,Z,l,V,m,W,n,U>::Result Result; };
public:
   Lambda6( const LE& e ) : exp(e) { EnsureLE<LE>::go(); }
     template <class X, class Y, class Z, class V, class W, class U> struct Sig :
   public FunType<X,Y,Z,V,W,U,
     typename LE::template MyType<typename NewET<X,Y,Z,V,W,U>::Result>
      ::TypeThunk::template Go<int>::Type> {};
   template <class X, class Y, class Z,class V,class W, class U>
     typename Sig<X,Y,Z,V,W,U>::ResultType
     operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w, const U& u ) const {
     typedef typename NewET<X,Y,Z,V,W,U>::Result NET;
     return exp.template eval<NET>( NET::template Go<int>::go(NIL(),x,y,z,v,w,u) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::ostringstream o;
        o << "Lambda6<LE," << i << "," << j << "," << k << ","
                           << l << "," << m << "," << n << ">::";
 	std::string temp = o.str();
        temp += debug_name_or_nothing(exp);
       temp += std::string(" ]");
	return temp;
    }
#endif
};

#ifdef FCPP_LAMBDA7
// This needs a class Lambda6 and various other things as well.
 template <class LE, int i, int j, int k, int l, int m, int n, int o>
class Lambda7 {
   LE exp;
   template <class X, class Y, class Z, class V, class W, class U, class Q> struct NewET
   { typedef typename ETUpdateXYZVWUQ<NIL_ET,i,X,j,Y,k,Z,l,V,m,W,n,U,o,Q>::Result Result; };
public:
   Lambda7( const LE& e ) : exp(e) { EnsureLE<LE>::go(); }
   template <class X, class Y, class Z, class V, class W, class U, class Q> struct Sig :
     public FunType<X,Y,Z,V,W,U,Q,
     typename LE::template MyType<typename NewET<X,Y,Z,V,W,U,Q>::Result>
      ::TypeThunk::template Go<int>::Type> {};
   template <class X, class Y, class Z,class V,class W, class U, class Q>
     typename Sig<X,Y,Z,V,W,U,Q>::ResultType
     operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w, const U& u, const Q& q ) const {
     typedef typename NewET<X,Y,Z,V,W,U,Q>::Result NET;
     return exp.template eval<NET>( NET::template Go<int>::go(NIL(),x,y,z,v,w,u,q) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::ostringstream oo;
        oo << "Lambda7<LE," << i << "," << j << "," << k << ","
                           << l << "," << m << "," << n << "," << o << ">::";
 	std::string temp = oo.str();
        temp += debug_name_or_nothing(exp);
       temp += std::string(" ]");
	return temp;
    }
#endif
};

 template <class LE, int i, int j, int k, int l, int m, int n, int o, int p>
class Lambda8 {
   LE exp;
   template <class X, class Y, class Z, class V, class W, class U, class Q, class P> struct NewET
										     { typedef typename ETUpdateXYZVWUQP<NIL_ET,i,X,j,Y,k,Z,l,V,m,W,n,U,o,Q,p,P>::Result Result; };
public:
   Lambda8( const LE& e ) : exp(e) { EnsureLE<LE>::go(); }
   template <class X, class Y, class Z, class V, class W, class U, class Q, class P> struct Sig :
     public FunType<X,Y,Z,V,W,U,Q,P,
     typename LE::template MyType<typename NewET<X,Y,Z,V,W,U,Q,P>::Result>
      ::TypeThunk::template Go<int>::Type> {};
   template <class X, class Y, class Z,class V,class W, class U, class Q, class P>
     typename Sig<X,Y,Z,V,W,U,Q,P>::ResultType
     operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w, const U& u, const Q& q, const P& pp ) const {
     typedef typename NewET<X,Y,Z,V,W,U,Q,P>::Result NET;
     return exp.template eval<NET>( NET::template Go<int>::go(NIL(),x,y,z,v,w,u,q,pp) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
	std::ostringstream oo;
        oo << "Lambda8<LE," << i << "," << j << "," << k << "," << l
                 << "," << m << "," << n << "," << o << "," << p << ">::";
 	std::string temp = oo.str();
        temp += debug_name_or_nothing(exp);
       temp += std::string(" ]");
	return temp;
    }
#endif
};

#endif

#endif


// LambdaThingy is the temporary object that lambda() returns which
// understands operator[] calls.
template <class TBBV>
struct LambdaThingy {
   typedef TBBV VarsThisLambdaBinds;

   template <class LE, class FV, class TBBVars> struct Help2 {
      typedef exp::LambdaExpWithFreeVars<LE,VarsThisLambdaBinds> Result; 
      typedef Result Full;
   };
   template <class LE> struct Help2<LE,NIL,NIL> {
      typedef Lambda0<LE> Result; 
      typedef Full0<Result> Full;
   };
   template <class LE, int i>
   struct Help2<LE,NIL,CONS<LambdaVar<i>,NIL> > {
      typedef Lambda1<LE,i> Result;
      typedef Full1<Result> Full;
   };
   template <class LE, int i, int j>
   struct Help2<LE,NIL,CONS<LambdaVar<i>,CONS<LambdaVar<j>,NIL> > > {
      typedef Lambda2<LE,i,j> Result;
      typedef Full2<Result> Full;
   };
   template <class LE, int i, int j, int k>
   struct Help2<LE,NIL,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                       CONS<LambdaVar<k>,NIL> > > > {
      typedef Lambda3<LE,i,j,k> Result;
      typedef Full3<Result> Full;
   };
#ifdef FCPP152
   template <class LE, int i, int j, int k,int l>
   struct Help2<LE,NIL,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                       CONS<LambdaVar<k>,CONS<LambdaVar<l>,NIL> > > > > {
      typedef Lambda4<LE,i,j,k,l> Result;
      typedef Full4<Result> Full;
   };

  template <class LE, int i, int j, int k,int l, int m>
  struct Help2<LE,NIL,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                       CONS<LambdaVar<k>,CONS<LambdaVar<l>,
                        CONS<LambdaVar<m>,NIL> > > > > > {
      typedef Lambda5<LE,i,j,k,l,m> Result;
      typedef Full5<Result> Full;
   };

  template <class LE, int i, int j, int k,int l, int m, int n>
  struct Help2<LE,NIL,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                       CONS<LambdaVar<k>,CONS<LambdaVar<l>,
                        CONS<LambdaVar<m>,CONS<LambdaVar<n>,NIL> > > > > > > {
      typedef Lambda6<LE,i,j,k,l,m,n> Result;
      typedef Full6<Result> Full;
   };

#ifdef FCPP_LAMBDA7
  template <class LE, int i, int j, int k,int l, int m, int n, int o>
  struct Help2<LE,NIL,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                       CONS<LambdaVar<k>,CONS<LambdaVar<l>,
                        CONS<LambdaVar<m>,CONS<LambdaVar<n>,
                         CONS<LambdaVar<o>,NIL> > > > > > > > {
    typedef Lambda7<LE,i,j,k,l,m,n,o> Result;
      typedef Full7<Result> Full;
   };

  template <class LE, int i, int j, int k,int l, int m, int n, int o, int p>
  struct Help2<LE,NIL,CONS<LambdaVar<i>,CONS<LambdaVar<j>,
                       CONS<LambdaVar<k>,CONS<LambdaVar<l>,
                        CONS<LambdaVar<m>,CONS<LambdaVar<n>,
                         CONS<LambdaVar<o>,CONS<LambdaVar<p>,NIL> > > > > > > > > {
    typedef Lambda8<LE,i,j,k,l,m,n,o,p> Result;
      typedef Full8<Result> Full;
   };

#endif

#endif
   template <class LE> struct Helper {
      typedef typename ListDifference<typename LE::FreeVars,
         VarsThisLambdaBinds>::Result FreeVars;
      typedef typename Help2<LE,FreeVars,TBBV>::Result Result;
      typedef typename Help2<LE,FreeVars,TBBV>::Full Full;
   };
public:
   template <class E> struct RT
   { typedef typename Helper<typename LEify<E>::Type>::Full Type; };

   template <class E>
   typename RT<E>::Type
   operator[]( const E& e ) const {
      typedef typename Helper<typename LEify<E>::Type>::Result Result;
      typedef typename Helper<typename LEify<E>::Type>::Full Full;
#ifdef FCPP_PRINT_HELPERS
   std::cout << "LambdaThingy operator[] called" << std::endl;
   //#ifdef FCPP_DEBUG
   // Testing name() OK
   //std::cout << this->name() << std::endl;
   //#endif
#endif
   Full full( Result( LEify<E>::go(e) ) );
#ifdef FCPP_DEBUG_LAMBDATHINGY
   //Full full;
   // This now outputs information about the transitory
   // functoid which is used to evaluate a lambda.
   std::string temp = std::string("LambdaThingy : ");
   if ( ::fcpp::debug_traits_has_name<Full>() )
     temp += ::fcpp::debug_traits_name(full);
   //if (::fcpp::traits::local_debug_traits<Full>::has_local_name)
   //    temp += std::string(" has local name");
   std::cout << temp << std::endl;
   std::cout << "-------------------------------" << std::endl;
#endif
   return full;
   //return Full( Result( LEify<E>::go(e) ) );
   //return Full( funify (Result( LEify<E>::go(e) ) ) );
   }
#ifdef FCPP_DEBUG

    std::string name() const;

#endif
};

#ifdef FCPP_DEBUG

template <class TBBV> std::string LambdaThingy<TBBV>::name() const
  {
   return std::string("LambdaThingy<TBBV>"); 
  }

// This one works.
template <>
std::string LambdaThingy<NIL>::name() const
{
   return std::string("LambdaThingy<NIL>"); 
}

// This one generates a compilation error.
// I want to get to the point where it will output 
// the numbers of the variables which have been passed in
// and I have not got the grammar correct to do this.
/*
template <int i>
std::string LambdaThingy<CONS<LambdaVar<i>,NIL> >::name() const
{
       std::ostringstream out;
       out << "lambda(LV<" << i << ">)";
       return out.str(); 
}
*/
#endif

inline LambdaThingy<NIL> lambda() { 
     return LambdaThingy<NIL>(); 
}

template <int i> 
LambdaThingy<CONS<LambdaVar<i>,NIL> > 
lambda( const LambdaVar<i>& ) 
{ 
#ifdef FCPP_PRINT_HELPERS
   std::cout << "lambda called with one parameter " << i << std::endl;
#endif
  return LambdaThingy<CONS<LambdaVar<i>,NIL> >(); 
}

template <int i, int j> 
LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,NIL> > > 
lambda( const LambdaVar<i>&, const LambdaVar<j>& ) { 
   YouCannotPassTheSameLambdaVarTo_lambda_MoreThanOnce<(i==j)>::go();
#ifdef FCPP_PRINT_HELPERS
   std::cout << "lambda called with two parameters " 
             << i << "," << j << std::endl;
#endif
   return LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,NIL> > >(); 
}

template <int i, int j, int k> 
LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,CONS<LambdaVar<k>,NIL> > > > 
lambda( const LambdaVar<i>&, const LambdaVar<j>&, const LambdaVar<k>& ) { 
   YouCannotPassTheSameLambdaVarTo_lambda_MoreThanOnce<
      (i==j || i==k || j==k) >::go();
#ifdef FCPP_PRINT_HELPERS
   std::cout << "lambda called with three parameters " 
             << i << "," << j << "," << k << std::endl;
#endif
   return LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>,NIL> > > >(); 
}

#ifdef FCPP152
template <int i, int j, int k, int l> 
LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,CONS<LambdaVar<k>,CONS<LambdaVar<l>,NIL> > > > > 
lambda( const LambdaVar<i>&, const LambdaVar<j>&, const LambdaVar<k>&, const LambdaVar<l>& ) { 
   YouCannotPassTheSameLambdaVarTo_lambda_MoreThanOnce<
      (i==j || i==k || i==l || j==k || j==l || k ==l ) >::go();
#ifdef FCPP_PRINT_HELPERS
   std::cout << "lambda called with four parameters " 
             << i << "," << j << "," << k << "," << l << std::endl;
#endif
   return LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>, CONS<LambdaVar<l>,NIL> > > > >(); 
}

 template <int i, int j, int k, int l, int m> 
LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,CONS<LambdaVar<k>,CONS<LambdaVar<l>,CONS<LambdaVar<m>,NIL> > > > > > 
lambda( const LambdaVar<i>&, const LambdaVar<j>&, const LambdaVar<k>&, const LambdaVar<l>&, const LambdaVar<m>& ) { 
   YouCannotPassTheSameLambdaVarTo_lambda_MoreThanOnce<
      (i==j || i==k || i==l || j==k || j==l || k ==l || i==m || j==m || k==m || l==m ) >::go();
#ifdef FCPP_PRINT_HELPERS5
   std::cout << "lambda called with five parameters " 
             << i << "," << j << "," << k << "," << l << "," << m << std::endl;
#endif
   return LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>, CONS<LambdaVar<l>, CONS<LambdaVar<m>,NIL> > > > > >(); 
}

 template <int i, int j, int k, int l, int m, int n> 
LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,CONS<LambdaVar<k>,CONS<LambdaVar<l>,CONS<LambdaVar<m>,CONS<LambdaVar<n>,NIL> > > > > > >
lambda( const LambdaVar<i>&, const LambdaVar<j>&, const LambdaVar<k>&, const LambdaVar<l>&, const LambdaVar<m>&, const LambdaVar<n>& ) { 
   YouCannotPassTheSameLambdaVarTo_lambda_MoreThanOnce<
      (i==j || i==k || i==l || j==k || j==l || k ==l || i==m || j==m || k==m || l==m 
            || i==n || j==n || k==n || l==n || m==n ) >::go();
#ifdef FCPP_PRINT_HELPERS6
   std::cout << "lambda called with six parameters " 
             << i << "," << j << "," << k << "," 
             << l << "," << m << "," << n << std::endl;
#endif
   return LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>, CONS<LambdaVar<l>, CONS<LambdaVar<m>,CONS<LambdaVar<n>,NIL> > > > > > >();
}

#ifdef FCPP_LAMBDA7
template <int i, int j, int k, int l, int m, int n, int o>
LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,CONS<LambdaVar<k>,CONS<LambdaVar<l>,CONS<LambdaVar<m>,CONS<LambdaVar<n>,CONS<LambdaVar<o>,NIL> > > > > > > >
lambda( const LambdaVar<i>&, const LambdaVar<j>&, const LambdaVar<k>&, const LambdaVar<l>&, const LambdaVar<m>&, const LambdaVar<n>&, const LambdaVar<o>& ) {
   YouCannotPassTheSameLambdaVarTo_lambda_MoreThanOnce<
      (i==j || i==k || i==l || j==k || j==l || k ==l || i==m || j==m || k==m || l==m
            || i==n || j==n || k==n || l==n || m==n
            || i==o || j==o || k==o || l==o || m==o || n==o) >::go();
#ifdef FCPP_PRINT_HELPERS7
   std::cout << "lambda called with seven parameters "
             << i << "," << j << "," << k << ","
             << l << "," << m << "," << n << "," << o << std::endl;
#endif
   return LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>, CONS<LambdaVar<l>, CONS<LambdaVar<m>,CONS<LambdaVar<n>,CONS<LambdaVar<o>,NIL> > > > > > > >();
}

template <int i, int j, int k, int l, int m, int n, int o, int p>
LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,CONS<LambdaVar<k>,CONS<LambdaVar<l>,CONS<LambdaVar<m>,CONS<LambdaVar<n>,CONS<LambdaVar<o>,CONS<LambdaVar<p>,NIL> > > > > > > > >
lambda( const LambdaVar<i>&, const LambdaVar<j>&, const LambdaVar<k>&, const LambdaVar<l>&, const LambdaVar<m>&, const LambdaVar<n>&, const LambdaVar<o>&, const LambdaVar<p>& ) {
   YouCannotPassTheSameLambdaVarTo_lambda_MoreThanOnce<
      (i==j || i==k || i==l || j==k || j==l || k ==l || i==m || j==m || k==m || l==m
            || i==n || j==n || k==n || l==n || m==n
            || i==o || j==o || k==o || l==o || m==o || n==o
            || i==p || j==p || k==p || l==p || m==p || n==p || o==p   ) >::go();
#ifdef FCPP_PRINT_HELPERS7
   std::cout << "lambda called with eight parameters "
             << i << "," << j << "," << k << ","
             << l << "," << m << "," << n << "," << o << "," << p << std::endl;
#endif
   return LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
            CONS<LambdaVar<k>, CONS<LambdaVar<l>, CONS<LambdaVar<m>,CONS<LambdaVar<n>,CONS<LambdaVar<o>,CONS<LambdaVar<p>,NIL> > > > > > > > >();
}

#endif

#endif

//////////////////////////////////////////////////////////////////////
// lambda language constructs 
//////////////////////////////////////////////////////////////////////

template <IfKind k>
struct IfLambdaoid {
   template <class E, class T, class F>
   exp::IfLE<E,T,F,k>
   operator[]( const CONS<F,CONS<T,CONS<E,NIL> > >& x ) const {
      EnsureLE<E>::go(); EnsureLE<T>::go(); EnsureLE<F>::go();
      return exp::IfLE<E,T,F,k>( x.tail.tail.head, x.tail.head, x.head );
   }
};

template <int i, class E>
Binder<i,typename LEify<E>::Type>
operator==( exp::LambdaVar<i>, const E& e ) {
   return Binder<i,typename LEify<E>::Type>( LEify<E>::go(e) );
}

template <class LHS, int i, class LE>
CONS<Binder<i,LE>,typename BinderListify<LHS>::Type>
operator,( const LHS& lhs, const Binder<i,LE>& b ) {
   return CONS<Binder<i,LE>,typename BinderListify<LHS>::Type>
      ( b, BinderListify<LHS>::go(lhs) );
}

template <class H, class T, int i, class LE>
CONS<Binder<i,LE>,CONS<H,T> >
operator,( const CONS<H,T>& lhs, const Binder<i,LE>& b ) 
{
#ifdef FCPP_PRINT_HELPERS
   std::cout << "operator, called" << std::endl;
#endif
   return CONS<Binder<i,LE>,CONS<H,T> >( b, lhs );
}

//////////////////////////////////////////////////////////////////////
// LEType stuff
//////////////////////////////////////////////////////////////////////

#ifdef FCPP152
#ifdef FCPP_LAMBDA7
template <class F, class X=Void, class Y=Void, class Z=Void, class V=Void, class W=Void, class U=Void, class Q=Void, class P=Void> struct CALL;
#else
template <class F, class X=Void, class Y=Void, class Z=Void, class V=Void, class W=Void, class U=Void> struct CALL;
#endif
#else
template <class F, class X=Void, class Y=Void, class Z=Void> struct CALL;
#endif
template <int i> struct LV;
template <class E, class T, class F> struct IF0;
template <class E, class T, class F> struct IF1;
template <class E, class T, class F> struct IF2;
#ifdef FCPP152
#ifdef FCPP_LAMBDA7
template <class A, class B=Void, class C=Void, class D=Void, class E=Void, class F=Void, class G=Void, class H=Void, class I=void> struct LAM;
#else
template <class A, class B=Void, class C=Void, class D=Void, class E=Void, class F=Void, class G=Void> struct LAM;
#endif
#else
template <class A, class B=Void, class C=Void, class D=Void> struct LAM;
#endif
template <class T> struct LEType { typedef T Type; };
#ifdef FCPP152
#ifdef FCPP_LAMBDA7
template <class A, class B=Void, class C=Void, class D=Void, class E=Void, class F=Void, class G=Void, class H=Void, class I=Void>
struct LET_LEListify {
   typedef typename LEify<typename LEType<I>::Type>::Type LE_I;
  typedef CONS<LE_I,typename LET_LEListify<A,B,C,D,E,F,G,H>::Type> Type;
};

template <class A, class B, class C, class D, class E, class F, class G, class H>
   struct LET_LEListify<A,B,C,D,E,F,G,H> {
   typedef typename LEify<typename LEType<H>::Type>::Type LE_H;
   typedef CONS<LE_H,typename LET_LEListify<A,B,C,D,E,F,G>::Type> Type;
};

 template <class A, class B, class C, class D, class E, class F, class G>
   struct LET_LEListify<A,B,C,D,E,F,G> {
   typedef typename LEify<typename LEType<G>::Type>::Type LE_G;
  typedef CONS<LE_G,typename LET_LEListify<A,B,C,D,E,F>::Type> Type;
};

#else
 template <class A, class B=Void, class C=Void, class D=Void, class E=Void, class F=Void, class G=Void>
struct LET_LEListify {
   typedef typename LEify<typename LEType<G>::Type>::Type LE_G;
  typedef CONS<LE_G,typename LET_LEListify<A,B,C,D,E,F>::Type> Type;
};
#endif

// Trailing voids not needed as LET_LEListify has default of void.
 template <class A, class B, class C, class D, class E, class F>
struct LET_LEListify<A,B,C,D,E,F> {
   typedef typename LEify<typename LEType<F>::Type>::Type LE_F;
  typedef CONS<LE_F,typename LET_LEListify<A,B,C,D,E>::Type> Type;
};
 
template <class A, class B, class C, class D, class E>
  struct LET_LEListify<A,B,C,D,E> {
   typedef typename LEify<typename LEType<E>::Type>::Type LE_E;
    typedef CONS<LE_E,typename LET_LEListify<A,B,C,D>::Type> Type;
};

template <class A, class B, class C, class D>
struct LET_LEListify<A,B,C,D> {
   typedef typename LEify<typename LEType<D>::Type>::Type LE_D;
   typedef CONS<LE_D,typename LET_LEListify<A,B,C>::Type> Type;
};

template <class A, class B, class C>
  struct LET_LEListify<A,B,C> {
   typedef typename LEify<typename LEType<C>::Type>::Type LE_C;
   typedef CONS<LE_C,typename LET_LEListify<A,B>::Type> Type;
};
template <class A, class B>
struct LET_LEListify<A,B> {
   typedef typename LEify<typename LEType<B>::Type>::Type LE_B;
   typedef CONS<LE_B,typename LET_LEListify<A>::Type> Type;
};
template <class A>
struct LET_LEListify<A> {
   typedef typename LEListify<typename LEType<A>::Type>::Type Type;
};

#else

template <class A, class B=Void, class C=Void, class D=Void>
struct LET_LEListify {
   typedef typename LEify<typename LEType<D>::Type>::Type LE_D;
   typedef CONS<LE_D,typename LET_LEListify<A,B,C>::Type> Type;
};

template <class A, class B, class C>
struct LET_LEListify<A,B,C,Void> {
   typedef typename LEify<typename LEType<C>::Type>::Type LE_C;
   typedef CONS<LE_C,typename LET_LEListify<A,B>::Type> Type;
};
template <class A, class B>
struct LET_LEListify<A,B,Void,Void> {
   typedef typename LEify<typename LEType<B>::Type>::Type LE_B;
   typedef CONS<LE_B,typename LET_LEListify<A>::Type> Type;
};
template <class A>
struct LET_LEListify<A,Void,Void,Void> {
   typedef typename LEListify<typename LEType<A>::Type>::Type Type;
};

#endif

#ifdef FCPP152
#ifdef FCPP_LAMBDA7
template <class F, class X, class Y, class Z, class V, class W, class U, class Q, class P>
  struct LEType< CALL<F,X,Y,Z,V,W,U,Q,P> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::Call<FF,typename LET_LEListify<X,Y,Z,V,W,U,Q,P>::Type> Type;
};

template <class F, class X, class Y, class Z, class V, class W, class U, class Q>
  struct LEType< CALL<F,X,Y,Z,V,W,U,Q> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::Call<FF,typename LET_LEListify<X,Y,Z,V,W,U,Q>::Type> Type;
};
#endif
template <class F, class X, class Y, class Z, class V, class W, class U>
   struct LEType< CALL<F,X,Y,Z,V,W,U> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
     typedef exp::Call<FF,typename LET_LEListify<X,Y,Z,V,W,U>::Type> Type;
};
// Trailing voids not needed as CALL has default of void.
template <class F, class X, class Y, class Z, class V, class W>
  struct LEType< CALL<F,X,Y,Z,V,W> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
     typedef exp::Call<FF,typename LET_LEListify<X,Y,Z,V,W>::Type> Type;
};
template <class F, class X, class Y, class Z, class V>
struct LEType< CALL<F,X,Y,Z,V> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::Call<FF,typename LET_LEListify<X,Y,Z,V>::Type> Type;
};
template <class F, class X, class Y, class Z>
struct LEType< CALL<F,X,Y,Z> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::Call<FF,typename LET_LEListify<X,Y,Z>::Type> Type;
};
template <class F, class X, class Y>
struct LEType< CALL<F,X,Y> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::Call<FF,typename LET_LEListify<X,Y>::Type> Type;
};
template <class F, class X>
struct LEType< CALL<F,X> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::Call<FF,typename LET_LEListify<X>::Type> Type;
};
template <class F>
struct LEType< CALL<F> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::Call<FF,NIL> Type;
};

#else

template <class F, class X, class Y, class Z> 
struct LEType< CALL<F,X,Y,Z> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::Call<FF,typename LET_LEListify<X,Y,Z>::Type> Type;
}; 
template <class F, class X, class Y>
struct LEType< CALL<F,X,Y,Void> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::Call<FF,typename LET_LEListify<X,Y>::Type> Type;
}; 
template <class F, class X>
struct LEType< CALL<F,X,Void,Void> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::Call<FF,typename LET_LEListify<X>::Type> Type;
}; 
template <class F>
struct LEType< CALL<F,Void,Void,Void> > {
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::Call<FF,NIL> Type;
}; 

#endif

template <int i> struct LEType< LV<i> > { typedef LambdaVar<i> Type; };

template <class E, class T, class F>
struct LEType< IF0<E,T,F> > {
   typedef typename LEify<typename LEType<E>::Type>::Type EE;
   typedef typename LEify<typename LEType<T>::Type>::Type TT;
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::IfLE<EE,TT,FF,IfNormal> Type;
};
template <class E, class T, class F>
struct LEType< IF1<E,T,F> > {
   typedef typename LEify<typename LEType<E>::Type>::Type EE;
   typedef typename LEify<typename LEType<T>::Type>::Type TT;
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::IfLE<EE,TT,FF,IfTrue> Type;
};
template <class E, class T, class F>
struct LEType< IF2<E,T,F> > {
   typedef typename LEify<typename LEType<E>::Type>::Type EE;
   typedef typename LEify<typename LEType<T>::Type>::Type TT;
   typedef typename LEify<typename LEType<F>::Type>::Type FF;
   typedef exp::IfLE<EE,TT,FF,IfFalse> Type;
};

#ifdef FCPP152
#ifdef FCPP_LAMBDA7
template <int i, int j, int k, int l, int m,int n,int o,int p,class E>
  struct LEType< LAM<LV<i>,LV<j>,LV<k>,LV<l>,LV<m>,LV<n>,LV<o>,LV<p>,E> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
        CONS<LambdaVar<k>,CONS<LambdaVar<l>,
        CONS<LambdaVar<m>,CONS<LambdaVar<n>,CONS<LambdaVar<o>,CONS<LambdaVar<p>,NIL> > > > > > > > >::template RT<EE>::Type Type;
};

template <int i, int j, int k, int l, int m,int n,int o,class E>
  struct LEType< LAM<LV<i>,LV<j>,LV<k>,LV<l>,LV<m>,LV<n>,LV<o>,E> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
        CONS<LambdaVar<k>,CONS<LambdaVar<l>,
        CONS<LambdaVar<m>,CONS<LambdaVar<n>,CONS<LambdaVar<o>,NIL> > > > > > > >::template RT<EE>::Type Type;
};
#endif
 template <int i, int j, int k, int l, int m,int n,class E>
   struct LEType< LAM<LV<i>,LV<j>,LV<k>,LV<l>,LV<m>,LV<n>,E> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
        CONS<LambdaVar<k>,CONS<LambdaVar<l>,
        CONS<LambdaVar<m>,CONS<LambdaVar<n>,NIL> > > > > > >::template RT<EE>::Type Type;
};

 template <int i, int j, int k, int l, int m,class E>
   struct LEType< LAM<LV<i>,LV<j>,LV<k>,LV<l>,LV<m>,E> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
      CONS<LambdaVar<k>,CONS<LambdaVar<l>,CONS<LambdaVar<m>,NIL> > > > > >::template RT<EE>::Type Type;
};

template <int i, int j, int k, int l, class E>
struct LEType< LAM<LV<i>,LV<j>,LV<k>,LV<l>,E,Void> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
      CONS<LambdaVar<k>,CONS<LambdaVar<l>,NIL> > > > >::template RT<EE>::Type Type;
};

template <int i, int j, int k, class E>
struct LEType< LAM<LV<i>,LV<j>,LV<k>,E,Void,Void> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
      CONS<LambdaVar<k>,NIL> > > >::template RT<EE>::Type Type;
};
template <int i, int j, class E>
struct LEType< LAM<LV<i>,LV<j>,E,Void,Void,Void> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
      NIL> > >::template RT<EE>::Type Type;
};
template <int i, class E>
struct LEType< LAM<LV<i>,E,Void,Void,Void,Void> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<CONS<LambdaVar<i>,
      NIL> >::template RT<EE>::Type Type;
};
template <class E>
struct LEType< LAM<E,Void,Void,Void,Void,Void> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<NIL>::template RT<EE>::Type Type;
};

#else

template <int i, int j, int k, class E>
struct LEType< LAM<LV<i>,LV<j>,LV<k>,E> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
      CONS<LambdaVar<k>,NIL> > > >::template RT<EE>::Type Type;
};
template <int i, int j, class E>
struct LEType< LAM<LV<i>,LV<j>,E,Void> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<CONS<LambdaVar<i>,CONS<LambdaVar<j>,
      NIL> > >::template RT<EE>::Type Type;
};
template <int i, class E>
struct LEType< LAM<LV<i>,E,Void,Void> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<CONS<LambdaVar<i>,
      NIL> >::template RT<EE>::Type Type;
};
template <class E>
struct LEType< LAM<E,Void,Void,Void> > {
   typedef typename LEType<E>::Type EE;
   typedef typename LambdaThingy<NIL>::template RT<EE>::Type Type;
};
#endif

//////////////////////////////////////////////////////////////////////
// more lambda language constructs
//////////////////////////////////////////////////////////////////////

struct LetLambdaoid {
   template <class BL>
   struct InLambdaoid {
      class Help {
         BL bl;
      public:
         template <class B, class LE> struct Lambdify;
         template <int i, class LEa, class LE> 
         struct Lambdify<CONS<Binder<i,LEa>,NIL>,LE> {
            typedef typename LEType<CALL<LAM<LV<i>,LE>,LEa> >::Type R;
            static inline R go( const CONS<Binder<i,LEa>,NIL>& binders, 
                                const LE& le ) {
               LambdaVar<i> X;
               return lambda(X)[ le ][ binders.head.exp ];
            }
         };
         template <int i, class LEa, class Rest, class LE> 
         struct Lambdify<CONS<Binder<i,LEa>,Rest>,LE> {
            typedef typename LEType<CALL<LAM<LV<i>,LE>,LEa> >::Type Inner;
            typedef typename Lambdify<Rest,Inner>::R R;
            static inline R go( const CONS<Binder<i,LEa>,Rest>& binders, 
                                const LE& le ) {
               LambdaVar<i> X;
               return Lambdify<Rest,Inner>::go( binders.tail,
                  lambda(X)[ le ][ binders.head.exp ] );
            }
         };

         Help( const BL& l ) : bl(l) {}
         template <class E> struct RT
         { typedef typename Lambdify<BL,typename LEify<E>::Type>::R Type; };
         template <class E>
         typename RT<E>::Type
         operator[]( const E& e ) {
            return Lambdify<BL,typename LEify<E>::Type>::go
               ( bl, LEify<E>::go(e) );
         }
      };
      Help in;
      InLambdaoid( const BL& l ) : in(l) {}
   };

   template <class BL> struct RT
   { typedef InLambdaoid<typename BinderListify<BL>::Type> Type; };
   template <class BL>
   typename RT<BL>::Type
   operator[]( const BL& bl ) const {
      EnsureBinderList<typename BinderListify<BL>::Type>::go();
      return InLambdaoid<typename BinderListify<BL>::Type>
         ( BinderListify<BL>::go(bl) );
   }
};

struct LetRecLambdaoid {
   template <class BL>
   struct InLambdaoid {
      class Help {
         BL bl;
      public:
         Help( const BL& l ) : bl(l) {}
         template <class E> struct RT 
         { typedef exp::BindingEnvExp<BL,typename LEify<E>::Type> Type; };
         template <class E>
         typename RT<E>::Type
         operator[]( const E& e ) {
            return exp::BindingEnvExp<BL,typename LEify<E>::Type>
               ( bl, LEify<E>::go(e) );
         }
      };
      Help in;
      InLambdaoid( const BL& l ) : in(l) {}
   };

   template <class BL> struct RT;
   template <class BL> friend struct RT;
   template <class BL> struct RT
   { typedef InLambdaoid<typename BinderListify<BL>::Type> Type; };
   template <class BL>
   typename RT<BL>::Type
   operator[]( const BL& bl ) const {
      EnsureBinderList<typename BinderListify<BL>::Type>::go();
      return InLambdaoid<typename BinderListify<BL>::Type>
         ( BinderListify<BL>::go(bl) );
   }
};

//////////////////////////////////////////////////////////////////////
// more LEType stuff
//////////////////////////////////////////////////////////////////////

template <int i, class LE> struct BIND;

#ifdef FCPP152

template <class A, class B, class C=Void, class D=Void, class E=Void, class F=Void> struct LET;
template <class A, class B, class C=Void, class D=Void, class E=Void, class F=Void> struct LETREC;

template <class A, class B=Void, class C=Void, class D=Void, class E=Void, class F=Void> 
struct LET_BinderListify {
   typedef CONS<F,typename LET_BinderListify<A,B,C,D,E>::Type> Type;
};

template <class A, class B, class C, class D, class E>
struct LET_BinderListify<A,B,C,D,E> {
   typedef CONS<E,typename LET_BinderListify<A,B,C,D>::Type> Type;
};

template <class A, class B, class C, class D>
struct LET_BinderListify<A,B,C,D> {
   typedef CONS<D,typename LET_BinderListify<A,B,C>::Type> Type;
};

template <class A, class B, class C>
struct LET_BinderListify<A,B,C> {
   typedef CONS<C,typename LET_BinderListify<A,B>::Type> Type;
};
template <class A, class B>
struct LET_BinderListify<A,B> {
   typedef CONS<B,typename LET_BinderListify<A>::Type> Type;
};
template <class A>
struct LET_BinderListify<A> {
   typedef typename BinderListify<A>::Type Type;
};


#else 

template <class A, class B, class C=Void, class D=Void> struct LET;
template <class A, class B, class C=Void, class D=Void> struct LETREC;

template <class A, class B=Void, class C=Void, class D=Void> 
struct LET_BinderListify {
   typedef CONS<D,typename LET_BinderListify<A,B,C>::Type> Type;
};

template <class A, class B, class C>
  struct LET_BinderListify<A,B,C,Void> {
   typedef CONS<C,typename LET_BinderListify<A,B>::Type> Type;
};
template <class A, class B>
  struct LET_BinderListify<A,B,Void,Void> {
   typedef CONS<B,typename LET_BinderListify<A>::Type> Type;
};
template <class A>
struct LET_BinderListify<A,Void,Void,Void> {
   typedef typename BinderListify<A>::Type Type;
};

#endif

#ifdef FCPP152

template <int i, class A, class LE>
struct LEType< LET<BIND<i,A>,LE> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef typename LetLambdaoid::template RT<typename
      LET_BinderListify<AA>::Type>::Type::Help::template RT<LELE>::Type Type;
};

template <int i, class A, int j, class B, class LE>
struct LEType< LET<BIND<i,A>,BIND<j,B>,LE> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef Binder<j,typename LEify<typename LEType<B>::Type>::Type> BB;
   typedef typename LetLambdaoid::template RT<typename
      LET_BinderListify<AA,BB>::Type>::Type::Help::template RT<LELE>::Type Type;
};

template <int i, class A, int j, class B, int k, class C, class LE>
struct LEType< LET<BIND<i,A>,BIND<j,B>,BIND<k,C>,LE> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef Binder<j,typename LEify<typename LEType<B>::Type>::Type> BB;
   typedef Binder<k,typename LEify<typename LEType<C>::Type>::Type> CC;
   typedef typename LetLambdaoid::template RT<typename
      LET_BinderListify<AA,BB,CC>::Type>::Type::Help::template 
      RT<LELE>::Type Type;
};

template <int i, class A, int j, class B, int k, class C, int l, class D, class LE>
struct LEType< LET<BIND<i,A>,BIND<j,B>,BIND<k,C>,BIND<l,D>,LE> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef Binder<j,typename LEify<typename LEType<B>::Type>::Type> BB;
   typedef Binder<k,typename LEify<typename LEType<C>::Type>::Type> CC;
   typedef Binder<l,typename LEify<typename LEType<D>::Type>::Type> DD;
   typedef typename LetLambdaoid::template RT<typename
      LET_BinderListify<AA,BB,CC,DD>::Type>::Type::Help::template 
      RT<LELE>::Type Type;
};

#else
template <int i, class A, class LE>
struct LEType< LET<BIND<i,A>,LE,Void,Void> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef typename LetLambdaoid::template RT<typename
      LET_BinderListify<AA>::Type>::Type::Help::template RT<LELE>::Type Type;
};

template <int i, class A, int j, class B, class LE>
struct LEType< LET<BIND<i,A>,BIND<j,B>,LE,Void> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef Binder<j,typename LEify<typename LEType<B>::Type>::Type> BB;
   typedef typename LetLambdaoid::template RT<typename
      LET_BinderListify<AA,BB>::Type>::Type::Help::template RT<LELE>::Type Type;
};
template <int i, class A, int j, class B, int k, class C, class LE>
struct LEType< LET<BIND<i,A>,BIND<j,B>,BIND<k,C>,LE> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef Binder<j,typename LEify<typename LEType<B>::Type>::Type> BB;
   typedef Binder<k,typename LEify<typename LEType<C>::Type>::Type> CC;
   typedef typename LetLambdaoid::template RT<typename
      LET_BinderListify<AA,BB,CC>::Type>::Type::Help::template 
      RT<LELE>::Type Type;
};

#endif

#ifdef FCPP152

template <int i, class A, class LE>
struct LEType< LETREC<BIND<i,A>,LE> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef typename LetRecLambdaoid::template RT<typename
      LET_BinderListify<AA>::Type>::Type::Help::template RT<LELE>::Type Type;
};
template <int i, class A, int j, class B, class LE>
struct LEType< LETREC<BIND<i,A>,BIND<j,B>,LE> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef Binder<j,typename LEify<typename LEType<B>::Type>::Type> BB;
   typedef typename LetRecLambdaoid::template RT<typename
      LET_BinderListify<AA,BB>::Type>::Type::Help::template RT<LELE>::Type Type;
};
template <int i, class A, int j, class B, int k, class C, class LE>
struct LEType< LETREC<BIND<i,A>,BIND<j,B>,BIND<k,C>,LE> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef Binder<j,typename LEify<typename LEType<B>::Type>::Type> BB;
   typedef Binder<k,typename LEify<typename LEType<C>::Type>::Type> CC;
   typedef typename LetRecLambdaoid::template RT<typename
      LET_BinderListify<AA,BB,CC>::Type>::Type::Help::template 
      RT<LELE>::Type Type;
};

template <int i, class A, int j, class B, int k, class C, int l, class D, class LE>
struct LEType< LETREC<BIND<i,A>,BIND<j,B>,BIND<k,C>,BIND<l,D>,LE> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef Binder<j,typename LEify<typename LEType<B>::Type>::Type> BB;
   typedef Binder<k,typename LEify<typename LEType<C>::Type>::Type> CC;
   typedef Binder<l,typename LEify<typename LEType<D>::Type>::Type> DD;
   typedef typename LetRecLambdaoid::template RT<typename
      LET_BinderListify<AA,BB,CC,DD>::Type>::Type::Help::template 
      RT<LELE>::Type Type;
};
#else

template <int i, class A, class LE>
struct LEType< LETREC<BIND<i,A>,LE,Void,Void> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef typename LetRecLambdaoid::template RT<typename
      LET_BinderListify<AA>::Type>::Type::Help::template RT<LELE>::Type Type;
};
template <int i, class A, int j, class B, class LE>
struct LEType< LETREC<BIND<i,A>,BIND<j,B>,LE,Void> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef Binder<j,typename LEify<typename LEType<B>::Type>::Type> BB;
   typedef typename LetRecLambdaoid::template RT<typename
      LET_BinderListify<AA,BB>::Type>::Type::Help::template RT<LELE>::Type Type;
};
template <int i, class A, int j, class B, int k, class C, class LE>
struct LEType< LETREC<BIND<i,A>,BIND<j,B>,BIND<k,C>,LE> > {
   typedef typename LEType<LE>::Type LELE;
   typedef Binder<i,typename LEify<typename LEType<A>::Type>::Type> AA;
   typedef Binder<j,typename LEify<typename LEType<B>::Type>::Type> BB;
   typedef Binder<k,typename LEify<typename LEType<C>::Type>::Type> CC;
   typedef typename LetRecLambdaoid::template RT<typename
      LET_BinderListify<AA,BB,CC>::Type>::Type::Help::template 
      RT<LELE>::Type Type;
};

#endif

} // end namespace fcpp_lambda

FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN fcpp_lambda::IfLambdaoid<fcpp_lambda::IfNormal> if0;
FCPP_MAYBE_EXTERN fcpp_lambda::IfLambdaoid<fcpp_lambda::IfTrue>   if1;
FCPP_MAYBE_EXTERN fcpp_lambda::IfLambdaoid<fcpp_lambda::IfFalse>  if2;
FCPP_MAYBE_EXTERN fcpp_lambda::LetRecLambdaoid letrec;
FCPP_MAYBE_EXTERN fcpp_lambda::LetLambdaoid let;
FCPP_MAYBE_NAMESPACE_CLOSE

using fcpp_lambda::LambdaVar;
using fcpp_lambda::exp::LambdaType;
using fcpp_lambda::exp::lambdify;
using fcpp_lambda::lambda;  // all that work for _one_ exported function :)

#ifdef FCPP_LAMBDA_AUTOCURRY
using fcpp_lambda::exp::AutoCurry; // The type is needed for LEType work.
using fcpp_lambda::exp::_b_;
#endif

using fcpp_lambda::LEType;
using fcpp_lambda::CALL;
using fcpp_lambda::LV;
using fcpp_lambda::IF0;
using fcpp_lambda::IF1;
using fcpp_lambda::IF2;
using fcpp_lambda::LAM;
using fcpp_lambda::BIND;
using fcpp_lambda::LET;
using fcpp_lambda::LETREC;


} // end namespace fcpp

template <class LE,int i>
  std::ostream & operator << (std::ostream &o, const fcpp::fcpp_lambda::Lambda1<LE,i> &f)
{
#ifdef FCPP_DEBUG
  o << f.name();
#else
  o << "**** ERROR: Attempt to output a fcpp_lambda::Lambda1<LE,i> ****";
#endif
  return o;
}

template <class LE,int i,int j>
  std::ostream & operator << (std::ostream &o, const fcpp::fcpp_lambda::Lambda2<LE,i,j> &f)
{
#ifdef FCPP_DEBUG
  o << f.name();
#else
  o << "**** ERROR: Attempt to output a fcpp_lambda::Lambda2<LE,i,j> ****";
#endif
  return o;
}
template <class LE,int i,int j,int k>
  std::ostream & operator << (std::ostream &o, const fcpp::fcpp_lambda::Lambda3<LE,i,j,k> &f)
{
#ifdef FCPP_DEBUG
  o << f.name();
#else
  o << "**** ERROR: Attempt to output a fcpp_lambda::Lambda3<LE,i,j,k> ****";
#endif
  return o;
}
#ifdef FCPP152
template <class LE,int i,int j,int k,int l>
  std::ostream & operator << (std::ostream &o, const fcpp::fcpp_lambda::Lambda4<LE,i,j,k,l> &f)
{
#ifdef FCPP_DEBUG
  o << f.name();
#else
  o << "**** ERROR: Attempt to output a fcpp_lambda::Lambda4<LE,i,j,k,l> ****";
#endif
  return o;
}

template <class LE,int i,int j,int k,int l,int m>
  std::ostream & operator << (std::ostream &o, const fcpp::fcpp_lambda::Lambda5<LE,i,j,k,l,m> &f)
{
#ifdef FCPP_DEBUG
  o << f.name();
#else
  o << "**** ERROR: Attempt to output a fcpp_lambda::Lambda5<LE,i,j,k,l,m> ****";
#endif
  return o;
}

template <class LE,int i,int j,int k,int l,int m,int n>
  std::ostream & operator << (std::ostream &o, const fcpp::fcpp_lambda::Lambda6<LE,i,j,k,l,m,n> &f)
{
#ifdef FCPP_DEBUG
  o << f.name();
#else
  o << "**** ERROR: Attempt to output a fcpp_lambda::Lambda6<LE,i,j,k,l,m,n> ****";
#endif
  return o;
}

#endif

// Includes any argument traits needed without FCPP_DEBUG
#include "lambda_debug.h"
//#endif

#endif

#endif

