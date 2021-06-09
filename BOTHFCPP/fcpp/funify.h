 //
 // Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
 //
 // Permission to use, copy, modify, distribute and sell this software
 // and its documentation for any purpose is granted without fee,
 // provided that the above copyright notice and this permission notice
 // appear in all source code copies and supporting documentation. The
 // software is provided "as is" without any express or implied
 // warranty.

 #ifndef FCPP_FUNIFY_DOT_H
 #define FCPP_FUNIFY_DOT_H

// 22nd May 2011 New file funify.h so that the definition
// of funify can be moved to a location after function.h is defined.

//////////////////////////////////////////////////////////////////////
// funify is an identity for FullNs and FunNs, 
// but calls ptr_to_fun otherwise
//////////////////////////////////////////////////////////////////////

namespace fcpp {

namespace impl {

  // Failed attempt to be able to funify a Fun0 object.
  // It looks as though funify is being defined before the Fun
  // objects. This means it could be moved elsewhere...
  // It now works with funify defined after function.h is called.
  // This works for Full0 to Full7 amd Fun0 to Fun4.

struct XFunify {
   template <class P> struct Sig : public FunType<P,
      typename RT<PtrToFun,P>::ResultType> {};
   template <class F> struct Sig< Full0<F> >
   : public FunType< Full0<F>, Full0<F> > {};
   template <class F> struct Sig< Full1<F> >
   : public FunType< Full1<F>, Full1<F> > {};
   template <class F> struct Sig< Full2<F> >
   : public FunType< Full2<F>, Full2<F> > {};
   template <class F> struct Sig< Full3<F> >
   : public FunType< Full3<F>, Full3<F> > {};
// Extra Sigs for Fun0, Fun1, Fun2, Fun3
   template <class R> struct Sig< Fun0<R> >
   : public FunType< Fun0<R>, Fun0<R> > {};
   template <class A, class R> struct Sig< Fun1<A,R> >
    : public FunType< Fun1<A,R>, Fun1<A,R> > {};
   template <class A1, class A2, class R>
     struct Sig< Fun2<A1,A2,R> >
     : public FunType< Fun2<A1,A2,R>, Fun2<A1,A2,R> > {};
  template <class A1, class A2, class A3, class R>
    struct Sig< Fun3<A1,A2,A3,R> >
    : public FunType< Fun3<A1,A2,A3,R>, Fun3<A1,A2,A3,R> > {};
// FC++.1.5.2
#ifdef FCPP152
   template <class F> struct Sig< Full4<F> >
   : public FunType< Full4<F>, Full4<F> > {};
   template <class F> struct Sig< Full5<F> >
   : public FunType< Full5<F>, Full5<F> > {};
   template <class F> struct Sig< Full6<F> >
   : public FunType< Full6<F>, Full6<F> > {};
   template <class F> struct Sig< Full7<F> >
   : public FunType< Full7<F>, Full7<F> > {};
   template <class A1, class A2, class A3, class A4,class R>
    struct Sig< Fun4<A1,A2,A3,A4,R> >
    : public FunType< Fun4<A1,A2,A3,A4,R>, Fun4<A1,A2,A3,A4,R> > {};
#endif
   template <class P>
#ifdef FCPP_CONCEPTS
   ConstCallable1<PtrToFun,P>::result_type
#else
   typename Sig<P>::ResultType
#endif
   operator()( const P& p ) const { return ptr_to_fun(p); }
   template <class F>
#ifdef FCPP_CONCEPTS
   inline Full0<F>
#else
   typename Sig<Full0<F> >::ResultType
#endif
   operator()( const Full0<F>& f ) const { return f; }
  template <class F>
#ifdef FCPP_CONCEPTS
     inline Full1<F>
#else
     typename Sig<Full1<F> >::ResultType
#endif
     operator()( const Full1<F>& f ) const { return f; }
  template <class F>
#ifdef FCPP_CONCEPTS
    inline Full2<F>
#else
   typename Sig<Full2<F> >::ResultType
#endif
   operator()( const Full2<F>& f ) const { return f; }
   template <class F>
#ifdef FCPP_CONCEPTS
   inline Full3<F>
#else
   typename Sig<Full3<F> >::ResultType
#endif
   operator()( const Full3<F>& f ) const { return f; }
  // Extra operator() for Fun0, Fun1, Fun2, Fun3
   template <class R>
#ifdef FCPP_CONCEPTS
   inline Fun0<R>
#else
   typename Sig<Fun0<R> >::ResultType
#endif
   operator()( const Fun0<R>& f ) const { return f; }
  template <class A, class R>
#ifdef FCPP_CONCEPTS
     inline Fun1<A,R>
#else
   typename Sig<Fun1<A,R> >::ResultType
#endif
     operator()( const Fun1<A,R>& f ) const { return f; }
  template <class A1, class A2, class R>
#ifdef FCPP_CONCEPTS
    inline Fun2<A1,A2,R>
#else
    typename Sig<Fun2<A1,A2,R> >::ResultType
#endif
    operator()( const Fun2<A1,A2,R>& f ) const { return f; }
    template <class A1, class A2, class A3,class R>
#ifdef FCPP_CONCEPTS
    inline Fun3<A1,A2,A3,R>
#else
    typename Sig<Fun3<A1,A2,A3,R> >::ResultType
#endif
    operator()( const Fun3<A1,A2,A3,R>& f ) const { return f; }
// FC++.1.5.2
#ifdef FCPP152
   template <class F>
#ifdef FCPP_CONCEPTS
   inline Full4<F>
#else
   typename Sig<Full4<F> >::ResultType
#endif
   operator()( const Full4<F>& f ) const { return f; }
   template <class F>
#ifdef FCPP_CONCEPTS
   inline Full5<F>
#else
   typename Sig<Full5<F> >::ResultType
#endif
   operator()( const Full5<F>& f ) const { return f; }
   template <class F>
   typename Sig<Full6<F> >::ResultType
   operator()( const Full6<F>& f ) const { return f; }
   template <class F>
   typename Sig<Full7<F> >::ResultType
   operator()( const Full7<F>& f ) const { return f; }
  template <class A1, class A2, class A3,class A4,class R>
#ifdef FCPP_CONCEPTS
     inline Fun4<A1,A2,A3,A4,R>
#else
     typename Sig<Fun4<A1,A2,A3,A4,R> >::ResultType
#endif
     operator()( const Fun4<A1,A2,A3,A4,R>& f ) const { return f; }
#endif
};
}
typedef Full1<impl::XFunify> Funify;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Funify funify;
FCPP_MAYBE_NAMESPACE_CLOSE

}

#endif
