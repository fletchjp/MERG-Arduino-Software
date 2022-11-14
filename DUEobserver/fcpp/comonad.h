//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_COMONAD_DOT_H
#define FCPP_COMONAD_DOT_H

/* Starting point from http://c2.com/cgi/wiki?CoMonads

Here is an extract from it. It is WRONG and is not going to be
implemented in this form - see later.
---------------------------------
As far as I can tell, a CoMonad is an object which defines its 
method =>> such that

    a =>> b :: (w a -> (w a -> b) -> b) -- w is the comonad type.

Now corrected as

    a =>> b :: (w a -> (w a -> b) -> w b) -- w is the comonad type.

==========================================================================
    *** NOTE ***
    This wikipedia page gives a different formulation:

http://en.wikipedia.org/wiki/Monad_%28functional_programming%29#Comonads

Comonads are the categorical dual of monads.
They are defined by a type constructor W T and two operations:
extract with type W T → T for any T,
and extend with type (W T → T') → W T → W T' .

This corresponds in the above notation to this, which is different.

This is the result of lifting (or extending) a comonad.

    a =>> b :: (w a -> (w a -> b) -> w b) -- w is the comonad type.
    *** NOTE ENDS ***
============================================================================

This translates (I think, CorrectMe if I'm wrong)
to the more C++ like syntax of

    template<typename A> abstract class CoMonad {
        template<typename B>
        Functor<CoMonad<A>, B> operator =>>(CoMonad<A> a, B b) {
        }
    }

This is in a similar vein as a monad (OnMonads) define their method >>= as

    a >>= b :: (a -> (a -> m b) -> m b) --m is the monad.
----------------------------------------

According to information here:

http://en.wikipedia.org/wiki/Monad_%28functional_programming%29#Comonads

the corresponding action names are:  >>= Bind   for a Monad
                                     =>> Extend for a CoMonad.

A Monad has Unit while a CoMonad has Extract.

So I will build shells for ExtractW and ExtendW and figure out what should
be inside them.

Following monad.h gives the following.
*/

namespace fcpp {

namespace impl {
   template <class CoMonad>
   struct XExtractW
   {
      template <class A> struct Sig 
      : public FunType<A,typename CoMonad::Extract::template Sig<A>::ResultType> {};
      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a ) const {
         return CoMonad::extract()(a);
      }

   };

   template <class CoMonad>
   struct XRemainderW
   {
      template <class A> struct Sig
      : public FunType<A,typename CoMonad::Remainder::template Sig<A>::ResultType> {};
      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a ) const {
         return CoMonad::remainder()(a);
      }

   };
}

template <class CoMonad> Full1<impl::XExtractW<CoMonad> > extractW()
{ return makeFull1( impl::XExtractW<CoMonad>() ); }
template <class CoMonad> struct ExtractW
{ typedef Full1<impl::XExtractW<CoMonad> > Type; };
template <class CoMonad> Full1<impl::XRemainderW<CoMonad> > remainderW()
{ return makeFull1( impl::XRemainderW<CoMonad>() ); }
template <class CoMonad> struct RemainderW
{ typedef Full1<impl::XRemainderW<CoMonad> > Type; };


namespace impl {
   template <class CoMonad>
   struct XExtendW
   {
      template <class W, class K> struct Sig : public FunType<W,K,
         typename CoMonad::Extend::template Sig<W,K>::ResultType> {};
   
      template <class W, class K>
      typename Sig<W,K>::ResultType
      operator()( const W& w, const K& k ) const {
         return CoMonad::extend()(w,k);
      }

   };

   template <class CoMonad>
   struct XCoBindW
   {
      template <class W, class K> struct Sig : public FunType<W,K,
         typename CoMonad::CoBind::template Sig<W,K>::ResultType> {};
   
      template <class W, class K>
      typename Sig<W,K>::ResultType
      operator()( const W& w, const K& k ) const {
         return CoMonad::coBind()(w,k);
      }

   };

}

template <class CoMonad> Full2<impl::XExtendW<CoMonad> > extendW()
{ return makeFull2( impl::XExtendW<CoMonad>() ); }
template <class CoMonad> struct ExtendW
{ typedef Full2<impl::XExtendW<CoMonad> > Type; };
template <class CoMonad> Full2<impl::XCoBindW<CoMonad> > coBindW()
{ return makeFull2( impl::XCoBindW<CoMonad>() ); }
template <class CoMonad> struct CoBindW
{ typedef Full2<impl::XCoBindW<CoMonad> > Type; };


// Following the example in monad.h
//////////////////////////////////////////////////////////////////////
// CoMonad concepts
//////////////////////////////////////////////////////////////////////
// Here we outline the C++ concepts necessary for monads to work in the
// framework.
/*
   concept CoMonad {
      // extract :: w a -> a
      // extend  :: w a -> ( w a -> b ) ->  b
      typedef Extract;           // a full functoid type
      static  Extract extract;
      typedef Extend;           // a full functoid type
      static  Extend extend;
   }
*/

//////////////////////////////////////////////////////////////////////
// liftW
//haskell lift_w :: Comonad w => (a -> b) -> (w a -> b)
//haskell lift_w f a = f (extract a)
//////////////////////////////////////////////////////////////////////

// First attempt to build this, the internals are still liftM
// which is not correct for this.
// Review - I am not sure this makes any sense.
namespace impl {
   template <class CoMonad>
   struct XLiftW {
       template <class F> struct Sig : public FunType<F,typename
       LEType<LAM<LV<1>,COMP<CoMonad,CALL<F,LV<2> >,GETS<2,LV<1> > > > >::Type> {};
       template <class F>
       typename Sig<F>::ResultType operator()( const F& f ) const {
          LambdaVar<1> X;
          LambdaVar<2> X_;
       // Brackets put in at suggestion of clang
       // and taken out again because it could lead to immediate
       // calculation - not needed.
	  // put back in to get rid of the warning (again).
          return lambda(X)[ f[X_] | (X_ <= CoMonad::extract[X]) ] ;
       }
   };
}

template <class CoMonad> Full1<impl::XLiftW<CoMonad> > liftW()
{ return makeFull1( impl::XLiftW<CoMonad>() ); }
template <class CoMonad> struct LiftW 
{ typedef Full1<impl::XLiftW<CoMonad> > Type; };


//////////////////////////////////////////////////////////////////////
// Attempt at IdentityW CoMonad
//////////////////////////////////////////////////////////////////////
struct IdentityW {
   // W a = a

   // This change follows a similar change in IdentityM in monad.h
   // I am not sure if this changes anything as Id will have a Sig.
   // It does cause one more level of indirection.
   struct XExtract {
      template <class A> struct Sig : public FunType<A,A> {};
      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a ) const { return id(a); }
   };
   typedef Full1<XExtract> Extract;
  //typedef Id Extract;
   static Extract& extract() {static Extract f; return f;}

   struct XExtend {
       template <class W, class K> struct Sig : public FunType<W,K,
	 typename RT<K,W>::ResultType> {};
       template <class W, class K>
       typename Sig<W,K>::ResultType
       operator()( const W& w, const K& k ) const {
          return k(w);
       }
    };
    typedef Full2<XExtend> Extend;
    static Extend& extend() {static Extend f; return f;}
 };

//////////////////////////////////////////////////////////////////////
// First attempt at ListW CoMonad.
//////////////////////////////////////////////////////////////////////
struct ListW {

  // For extract, return the head of the list.
  // This action is something I have decided on.
  // I am not sure if it is correct.
   struct XExtract {
      template <class W> struct Sig : public FunType<W,
	typename W::ElementType> {};
      template <class W>
      typename Sig<W>::ResultType
      operator()( const W& w ) const { return head(w); }
   };
   typedef Full1<XExtract> Extract;
   static Extract& extract() {static Extract f; return f;}

   // I have made up a name for this.
   struct XRemainder {
      template <class W> struct Sig : public FunType<W,W> {};
      template <class W>
      typename Sig<W>::ResultType
      operator()( const W& w ) const { return tail(w); }
    };
   typedef Full1<XRemainder> Remainder;
   static Remainder& remainder() {static Remainder f; return f;}

   // For extend, I am applying the function k to the list w
   // and returning the single result.
   struct XExtend {
       template <class W, class K> struct Sig : public FunType<W,K,
	 typename RT<K,W>::ResultType> {};
       template <class W, class K>
       typename Sig<W,K>::ResultType
       operator()( const W& w, const K& k ) const {
	 return k(w);
       }
    };
    typedef Full2<XExtend> Extend;
    static Extend& extend() {static Extend f; return f;}
    
// from
// http://mrlauer.wordpress.com/2010/12/18/rambling-thoughts-about-comonads/

// 'Cobinding a function to a stream simply applies the function
//  to each successive “tail” in turn.'

// (1, 2, 3, 4, ... ) =>> f =
//        ((f (1, 2, 3, 4, ... ),
//            (f (2, 3, 4, 5, ...),
//                (f (3, 4, 5, 6, ...), ...)

// That is interesting. Take it up in the ListW comonad.
    template <class F, class X>
    static List<List<X> > cobindHelper (const F& f,const List<X>& l)
    {
        List<List<X> > result;
        List<X> remainder = l;
        List<X> temp = f(remainder);
        remainder = tail(remainder);
        if (!null(remainder) ) result = cons(temp,cobindHelper(f,remainder));
        else result = cons(temp,NIL);
        return result;
    }

    struct XCoBind {
       template <class W, class K> struct Sig : public FunType<W,K,
	 List<W> > {};
       template <class W, class K>
       typename Sig<W,K>::ResultType
       operator()( const W& w, const K& k ) const {
	   return cobindHelper(k,w);
       }

    };
    typedef Full2<XCoBind> CoBind;
    static CoBind& coBind() {static CoBind f; return f;}

};

} // end namespace fcpp

#endif
