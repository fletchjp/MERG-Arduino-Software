//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_ARROW_DOT_H
#define FCPP_ARROW_DOT_H

namespace fcpp {
  // Arrow class (tentative).
  // First idea for the class which holds state.
  // Renamed as Arrow_ as I want the name for something else.
  template <class X>
  class Arrow_
  {
     X x_;
   public:
     Arrow_(const X& x) : x_(x) { }
     // This is the name of the operator.
     X arr() const { return x_; }
  };

  ///////////////////////////////////////////////////////////////////////////
  // Components for arrow operations in FC++
  // based on the ideas in these pages:
  //
  //     http://www.haskell.org/arrows/index.html
  //     http://www.haskell.org/arrows/syntax.html
  //
  // first(f,p) and second(f,p) with p a pair (x,y)
  // first  applies f to x and returns a pair f(x) and y
  // second applies f to y and returns a pair x and f(y)
  // I am doing this rather than swapping x and y
  // as that only works if they are the same type.
  //
  // Example of use:
  // hUncurry(plus,compose(first(inc),second(dec))(makePair(x,x)))
  // This takes an argument (x) and makes a pair of two copies.
  // It increments the first and decrements the second.
  // It then uncurries the pair and feeds it to plus,
  // adding them together.
  // All the other functoids are in prelude or operator.
  /////////////////////////////////////////////////////////////////////////
  // first and second have been moved to fcpp/pattern.h
  ///////////////////////////////////////////////////////////////////////////
 
 template <class F>
 class Arrow {
   F f_;
 public:
   Arrow(F &f) : f_(f) { }
   F operator()() const { return f_; }
   F arr() const { return f_; }
 };

 namespace impl {
     // based on my example arrowOp which has the same structure
     // Renamed from liftA2 as that name has another use for
     // applicative functors.
     // See "Learn You a Haskell..." p.238
     // That will leave me free to implement liftA2 again.
     struct XLiftArrow2 {
       template <class Op, class F, class G, class X> struct Sig :
         public FunType<Op,F,G,X,
           typename RT<Op,typename RT<F,X>::ResultType,
           typename RT<G,X>::ResultType>::ResultType>
         {};

         template <class Op, class F, class G, class X>
	 typename Sig<Op,F,G,X>::ResultType
	   operator()( const Op& op, const F& f,const G&g, const X& x ) const {
           //typedef typename Sig<Op,F,G,X>::ResultType Y;
	   return hUncurry(op,invcompose(first(f),second(g))(makePair(x,x)));
         }

     };
 }
 typedef Full4<impl::XLiftArrow2> LiftArrow2;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN LiftArrow2 liftArrow2;
 FCPP_MAYBE_NAMESPACE_CLOSE


} // end namespace fcpp

#endif
