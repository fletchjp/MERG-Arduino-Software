// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_HELPERS7_DOT_H
#define FCPP_CURRY_HELPERS7_DOT_H

//////////////////////////////////////////////////////////////////////
// Needed for Full7
// The CurryableNHelperM files have become very complicated.
// The naming is inherited from the original design of FC++.
// Here there are many more arguments and options.
// These have to cover autocurrying.
// The choice of helper depends on the total number of arguments
// including autocurry items.
// The value of N is always the total number of arguments (here 7).
// Other values of N are found in other files.
// M increases as the number of arguments decreases.
// M = 1 is implicit so the series goes like this:
// Curryable7Helper  with 7 arguments
// Curryable7Helper2 with 6 arguments
// Curryable7Helper3 with 5 arguments
// Curryable7Helper4 with 4 arguments
// Curryable7Helper5 with 3 arguments
// Curryable7Helper6 with 2 arguments
// In each case the first one has the full number of real arguments.
// Further items are template specialisations corresponding to
// autocurry cases.
// There are a lot of cases and not all are implemented.
// All the helpers pass on a call as needed mostly to binders.
// These are located in curry_binders7.h for this file.
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Curryable7Helper works on cases with the full 7 arguments.
//////////////////////////////////////////////////////////////////////

 template <class R, class F, class X, class Y, class Z, class T, class U, class V, class W>
struct Curryable7Helper {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z,
                       const T& t, const U&u, const V& v, const W& w ) {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 7 args" << std::endl;
#endif
      return f(x,y,z,t,u,v,w);
   }
};

template <class R, class F, class X>
  struct Curryable7Helper<R,F,X,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const X& x,const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 1 arg for x" << std::endl;
#endif
     return makeFull6(impl::binder1of7<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable7Helper<R,F,auto_curry_type,Y,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const Y& y,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 1 arg for y" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class Z>
  struct Curryable7Helper<R,F,auto_curry_type,auto_curry_type,Z,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,const Z& z,
                 const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 1 arg for z" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind3,Z> >(f,std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class T>
  struct Curryable7Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,T,auto_curry_type,auto_curry_type,auto_curry_type> {
    static R go( const F& f, const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&,const T& t,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 1 arg for t" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind4,T> >(f,std::make_pair(bind_fourth,t)));
   }
};

template <class R, class F, class U>
  struct Curryable7Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,U,auto_curry_type,auto_curry_type> {
    static R go( const F& f, const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&,const U& u,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 1 arg for u" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind5,U> >(f,std::make_pair(bind_fifth,u)));
   }
};

template <class R, class F, class V>
  struct Curryable7Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,V,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,const V& v,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 1 arg for v" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind6,V> >(f,std::make_pair(bind_sixth,v)));
   }
};

template <class R, class F, class W>
  struct Curryable7Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,W> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&,const W& w)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 1 arg for w" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind7,W> >(f,std::make_pair(bind_seventh,w)));
   }
};

template <class R, class F, class X,class Y>
  struct Curryable7Helper<R,F,X,Y,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const X& x, const Y& y,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 2 args for x and y" << std::endl;
#endif
     return makeFull5(impl::binder1and2of7<F,X,Y>(f,x,y));
   }
};

template <class R, class F, class V,class W>
  struct Curryable7Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,V,W> {
    static inline R go( const F& f,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&, const V& v, const W& w) 
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 2 args for v and w" << std::endl;
#endif
      return makeFull5(impl::binderXandYof7<F,std::pair<Bind6,V>,std::pair<Bind7,W> >(f,std::make_pair(bind_sixth,v),std::make_pair(bind_seventh,w)));
   }
};

template <class R, class F, class X, class Z, class T>
  struct Curryable7Helper<R,F,auto_curry_type,auto_curry_type,X,auto_curry_type,
                          Z,auto_curry_type,T> {
  static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                      const X& x, const auto_curry_type&,
                      const Z& z, const auto_curry_type&, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper called with 3 args for x z and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind3,X>,
		      std::pair<Bind5,Z>,std::pair<Bind7,T> >(f,
                   std::make_pair(bind_third,x),
                   std::make_pair(bind_fifth,z),
                   std::make_pair(bind_seventh,t)));
   }
};

template <class R, class F, class X, class Z, class T>
  struct Curryable7Helper<R,F,auto_curry_type,auto_curry_type,X,auto_curry_type,
                          auto_curry_type,Z,T> {
  static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                      const X& x, const auto_curry_type&,
                      const auto_curry_type&, const Z& z, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper called with 3 args for x z and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind3,X>,
		      std::pair<Bind6,Z>,std::pair<Bind7,T> >(f,
                   std::make_pair(bind_third,x),
                   std::make_pair(bind_sixth,z),
                   std::make_pair(bind_seventh,t)));
   }
};

template <class R, class F, class X, class Z, class T>
  struct Curryable7Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,
                          X,Z,auto_curry_type,T> {
  static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                      const auto_curry_type&,
                      const X& x, const Z& z, const auto_curry_type&, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper called with 3 args for x z and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind4,X>,
		      std::pair<Bind5,Z>,std::pair<Bind7,T> >(f,
                   std::make_pair(bind_fourth,x),
                   std::make_pair(bind_fifth,z),
                   std::make_pair(bind_seventh,t)));
   }
};

template <class R, class F, class X, class Z, class T>
  struct Curryable7Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,
                             X,auto_curry_type,Z,T> {
  static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                      const auto_curry_type&,
                      const X& x, const auto_curry_type&, const Z& z, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper called with 3 args for x z and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind4,X>,
		      std::pair<Bind6,Z>,std::pair<Bind7,T> >(f,
                   std::make_pair(bind_fourth,x),
                   std::make_pair(bind_sixth,z),
                   std::make_pair(bind_seventh,t)));
   }
};

template <class R, class F, class X, class Z, class T>
  struct Curryable7Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,
                             auto_curry_type,X,Z,T> {
  static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                      const auto_curry_type&, const auto_curry_type&,
                      const X& x, const Z& z, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper called with 3 args for x z and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind5,X>,
		      std::pair<Bind6,Z>,std::pair<Bind7,T> >(f,
                   std::make_pair(bind_fifth,x),
                   std::make_pair(bind_sixth,z),
                   std::make_pair(bind_seventh,t)));
   }
};


template <class R, class F, class V, class W, class X, class Y, class Z>
  struct Curryable7Helper<R,F,V,AutoCurryType,AutoCurryType,W,X,Y,Z> {
  static inline R go( const F& f, const V& v, const auto_curry_type&,
                                  const auto_curry_type&, const W& w,
                                  const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 5 args" << std::endl;
#endif
      return makeFull2(impl::binderVtoZof7<F,
                       std::pair<Bind1,V>,std::pair<Bind4,W>,std::pair<Bind5,X>,
                       std::pair<Bind6,Y>,std::pair<Bind7,Z> >
                       (f,std::make_pair(bind_first,v),
                        std::make_pair(bind_fourth,w),std::make_pair(bind_fifth,x),
                        std::make_pair(bind_sixth,y),std::make_pair(bind_seventh,z)));
   }
};

template <class R, class F, class U, class V, class W, class X, class Y, class Z>
  struct Curryable7Helper<R,F,AutoCurryType,U,V,W,X,Y,Z> {
  static inline R go( const F& f, const auto_curry_type&,
                                  const U& u, const V& v, const W& w,
                                  const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 6 args" << std::endl;
#endif
      return makeFull1(impl::binderUtoZof7<F,std::pair<Bind2,U>,
                       std::pair<Bind3,V>,std::pair<Bind4,W>,std::pair<Bind5,X>,
                       std::pair<Bind6,Y>,std::pair<Bind7,Z> >
                       (f,std::make_pair(bind_second,u),std::make_pair(bind_third,v),
                        std::make_pair(bind_fourth,w),std::make_pair(bind_fifth,x),
                        std::make_pair(bind_sixth,y),std::make_pair(bind_seventh,z)));
   }
};

template <class R, class F, class U, class V, class W, class X, class Y, class Z>
  struct Curryable7Helper<R,F,U,AutoCurryType,V,W,X,Y,Z> {
  static inline R go( const F& f, const U& u, const auto_curry_type&,
                                  const V& v, const W& w,
                                  const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper called with 6 args" << std::endl;
#endif
      return makeFull1(impl::binderUtoZof7<F,std::pair<Bind1,U>,
                       std::pair<Bind3,V>,std::pair<Bind4,W>,std::pair<Bind5,X>,
                       std::pair<Bind6,Y>,std::pair<Bind7,Z> >
                       (f,std::make_pair(bind_first,u),std::make_pair(bind_third,v),
                        std::make_pair(bind_fourth,w),std::make_pair(bind_fifth,x),
                        std::make_pair(bind_sixth,y),std::make_pair(bind_seventh,z)));
   }
};

//////////////////////////////////////////////////////////////////////
// Curryable7Helper2 works on cases with 6  arguments.
//////////////////////////////////////////////////////////////////////

template <class R, class F, class U,class V, class W,class X, class Y, class Z>
  struct Curryable7Helper2 {
    static inline R go( const F& f, const U& u,const V& v,const W& w,const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper2 called with 6 args for u v w x y and z" << std::endl;
#endif
     return makeFull1(impl::binder1to6of7<F,U,V,W,X,Y,Z >(f,u,v,w,x,y,z));
   }
};

template <class R, class F, class X>
  struct Curryable7Helper2<R,F,X,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const X& x,const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper2 called with 1 arg for x" << std::endl;
#endif
     return makeFull6(impl::binder1of7<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable7Helper2<R,F,auto_curry_type,Y,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const Y& y,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper2 called with 1 arg for y" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class Z>
  struct Curryable7Helper2<R,F,auto_curry_type,auto_curry_type,Z,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,const Z& z,
                 const auto_curry_type&,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper2 called with 1 arg for z" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind3,Z> >(f,std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class T>
  struct Curryable7Helper2<R,F,auto_curry_type,auto_curry_type,auto_curry_type,T,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,
                 const T& t,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper2 called with 1 arg for t" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind4,T> >(f,std::make_pair(bind_fourth,t)));
   }
};

template <class R, class F, class U>
  struct Curryable7Helper2<R,F,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,U,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&,const U& u,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper2 called with 1 arg for u" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind5,U> >(f,std::make_pair(bind_fifth,u)));
   }
};

template <class R, class F, class V>
  struct Curryable7Helper2<R,F,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,V> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&,const V& v)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper2 called with 1 arg for v" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind6,V> >(f,std::make_pair(bind_sixth,v)));
   }
};

template <class R, class F, class X,class Y>
  struct Curryable7Helper2<R,F,X,Y,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const X& x, const Y& y,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper2 called with 2 args for x and y" << std::endl;
#endif
     return makeFull5(impl::binder1and2of7<F,X,Y>(f,x,y));
   }
};

template <class R, class F, class X, class Z, class T>
  struct Curryable7Helper2<R,F,auto_curry_type,auto_curry_type,X,Z,auto_curry_type,
                             T> {
  static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                      const X& x, const Z& z, const auto_curry_type&,
                      const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper2 called with 3 args for x z and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind3,X>,
		      std::pair<Bind4,Z>,std::pair<Bind6,T> >(f,
                   std::make_pair(bind_third,x),
                   std::make_pair(bind_fourth,z),
                   std::make_pair(bind_sixth,t)));
   }
};

template <class R, class F, class X, class Z, class T>
  struct Curryable7Helper2<R,F,auto_curry_type,auto_curry_type,X,auto_curry_type,
                             Z,T> {
  static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                      const X& x, const auto_curry_type&,
                      const Z& z, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper2 called with 3 args for x z and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind3,X>,
		      std::pair<Bind5,Z>,std::pair<Bind6,T> >(f,
                   std::make_pair(bind_third,x),
                   std::make_pair(bind_fifth,z),
                   std::make_pair(bind_sixth,t)));
   }
};

template <class R, class F, class X, class Z, class T>
  struct Curryable7Helper2<R,F,auto_curry_type,auto_curry_type,auto_curry_type,
                             X,Z,T> {
  static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                      const auto_curry_type&,
                      const X& x, const Z& z, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper2 called with 3 args for x z and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind4,X>,
		      std::pair<Bind5,Z>,std::pair<Bind6,T> >(f,
                   std::make_pair(bind_fourth,x),
                   std::make_pair(bind_fifth,z),
                   std::make_pair(bind_sixth,t)));
   }
};

template <class R, class F, class V,class X, class Y, class Z>
  struct Curryable7Helper2<R,F,V,auto_curry_type,auto_curry_type,X,Y,Z> {
    static inline R go( const F& f,const V& v,auto_curry_type,auto_curry_type,const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper2 called with 4 args for v x y and z" << std::endl;
#endif
     return makeFull3(impl::binderWtoZof7<F,std::pair<Bind1,V>,
      std::pair<Bind4,X>,std::pair<Bind5,Y>,std::pair<Bind6,Z> >(f,
                   std::make_pair(bind_first,v),
                   std::make_pair(bind_fourth,x),
                   std::make_pair(bind_fifth,y),
                   std::make_pair(bind_sixth,z)));
   }
};

template <class R, class F, class V,class W, class X, class Y, class Z>
  struct Curryable7Helper2<R,F,auto_curry_type,V,W,X,Y,Z> {
  static inline R go( const F& f,auto_curry_type,const V& v,const W& w,
                      const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper2 called with 5 args for v w x y and z" << std::endl;
#endif
     return makeFull2(impl::binderVtoZof7<F,std::pair<Bind2,V>,
		      std::pair<Bind3,W>,std::pair<Bind4,X>,std::pair<Bind5,Y>,
                      std::pair<Bind6,Z> >(f,
                   std::make_pair(bind_second,v),
                   std::make_pair(bind_third,w),
                   std::make_pair(bind_fourth,x),
                   std::make_pair(bind_fifth,y),
                   std::make_pair(bind_sixth,z)));
   }
};

template <class R, class F, class V,class W, class X, class Y, class Z>
  struct Curryable7Helper2<R,F,V,auto_curry_type,W,X,Y,Z> {
  static inline R go( const F& f,const V& v,auto_curry_type,const W& w,
                      const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper2 called with 5 args for u w x y and z" << std::endl;
#endif
     return makeFull2(impl::binderVtoZof7<F,std::pair<Bind1,V>,
		      std::pair<Bind3,W>,std::pair<Bind4,X>,std::pair<Bind5,Y>,
                      std::pair<Bind6,Z> >(f,
                   std::make_pair(bind_first,v),
                   std::make_pair(bind_third,w),
                   std::make_pair(bind_fourth,x),
                   std::make_pair(bind_fifth,y),
                   std::make_pair(bind_sixth,z)));
   }
};



//////////////////////////////////////////////////////////////////////
// Curryable7Helper3 works on cases with 5 arguments.
//////////////////////////////////////////////////////////////////////

template <class R, class F, class V, class W,class X, class Y, class Z>
  struct Curryable7Helper3 {
    static inline R go( const F& f, const V& v,const W& w,const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper3 called with 5 args for v w x y and z" << std::endl;
#endif
     return makeFull2(impl::binder1to5of7<F,V,W,X,Y,Z >(f,v,w,x,y,z));
     
     /*     return makeFull2(impl::binderVtoZof7<F,std::pair<Bind1,V>,std::pair<Bind2,W>,
      std::pair<Bind3,X>,std::pair<Bind4,Y>,std::pair<Bind5,Z> >(f,
                   std::make_pair(bind_first,v),
                   std::make_pair(bind_second,w),
                   std::make_pair(bind_third,x),
                   std::make_pair(bind_fourth,y),
                   std::make_pair(bind_fifth,z))); */
   }
};

template <class R, class F, class V,class W, class X, class Z>
  struct Curryable7Helper3<R,F,V,W,X,auto_curry_type,Z> {
  static inline R go( const F& f,const V& v,const W& w,const X& x,auto_curry_type,const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper3 called with 4 args for v w x and z" << std::endl;
#endif
     return makeFull3(impl::binderWtoZof7<F,std::pair<Bind1,V>,
      std::pair<Bind2,W>,std::pair<Bind3,X>,std::pair<Bind5,Z> >(f,
                   std::make_pair(bind_first,v),
                   std::make_pair(bind_second,w),
                   std::make_pair(bind_third,x),
                   std::make_pair(bind_fifth,z)));
   }
};

template <class R, class F, class V,class X, class Y, class Z>
  struct Curryable7Helper3<R,F,V,auto_curry_type,X,Y,Z> {
    static inline R go( const F& f,const V& v,auto_curry_type,const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper3 called with 4 args for v x y and z" << std::endl;
#endif
     return makeFull3(impl::binderWtoZof7<F,std::pair<Bind1,V>,
      std::pair<Bind3,X>,std::pair<Bind4,Y>,std::pair<Bind5,Z> >(f,
                   std::make_pair(bind_first,v),
                   std::make_pair(bind_third,x),
                   std::make_pair(bind_fourth,y),
                   std::make_pair(bind_fifth,z)));
   }
};

template <class R, class F, class V,class W, class Y, class Z>
  struct Curryable7Helper3<R,F,V,W,auto_curry_type,Y,Z> {
  static inline R go( const F& f,const V& v,const W& w,auto_curry_type, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper3 called with 4 args for v w  y and z" << std::endl;
#endif
     return makeFull3(impl::binderWtoZof7<F,std::pair<Bind1,V>,
      std::pair<Bind2,W>,std::pair<Bind4,Y>,std::pair<Bind5,Z> >(f,
                   std::make_pair(bind_first,v),
                   std::make_pair(bind_second,w),
                   std::make_pair(bind_fourth,y),
                   std::make_pair(bind_fifth,z)));
   }
};

template <class R, class F, class V, class Y, class Z>
  struct Curryable7Helper3<R,F,V,auto_curry_type,auto_curry_type,Y,Z> {
  static inline R go( const F& f,const V& v,auto_curry_type,auto_curry_type,
		      const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper3 called with 3 args for v y and z" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind1,V>,
            std::pair<Bind4,Y>,std::pair<Bind5,Z> >(f,
                   std::make_pair(bind_first,v),
                   std::make_pair(bind_fourth,y),
                   std::make_pair(bind_fifth,z)));
   }
};

template <class R, class F, class X>
  struct Curryable7Helper3<R,F,X,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const X& x,const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper3 called with 1 arg for x" << std::endl;
#endif
     return makeFull6(impl::binder1of7<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable7Helper3<R,F,auto_curry_type,Y,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const Y& y,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper3 called with 1 arg for y" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class Z>
  struct Curryable7Helper3<R,F,auto_curry_type,auto_curry_type,Z,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,const Z& z,
                 const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper3 called with 1 arg for z" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind3,Z> >(f,std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class T>
  struct Curryable7Helper3<R,F,auto_curry_type,auto_curry_type,auto_curry_type,T,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&, const T& t,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper3 called with 1 arg for t" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind4,T> >(f,std::make_pair(bind_fourth,t)));
   }
};

template <class R, class F, class U>
  struct Curryable7Helper3<R,F,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,U> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&,const auto_curry_type&,const U& u)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper3 called with 1 arg for u" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind5,U> >(f,std::make_pair(bind_fifth,u)));
   }
};

template <class R, class F, class X,class Y>
  struct Curryable7Helper3<R,F,X,Y,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const X& x, const Y& y,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper3 called with 2 args for x and y" << std::endl;
#endif
     return makeFull5(impl::binder1and2of7<F,X,Y>(f,x,y));
   }
};

template <class R, class F, class X, class Y, class Z>
  struct Curryable7Helper3<R,F,X,Y,Z,auto_curry_type,auto_curry_type> {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper3 called with 3 args for x y and z" << std::endl;
#endif
     return makeFull4(impl::binder1to3of7<F,X,Y,Z >(f,x,y,z));
   }
};

template <class R, class F, class X, class Y, class T>
  struct Curryable7Helper3<R,F,X,Y,auto_curry_type,T,auto_curry_type> {
  static inline R go( const F& f, const X& x, const Y& y,const auto_curry_type&, const T& t,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper3 called with 3 args for x y and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind4,T> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_fourth,t)));
   }
};

template <class R, class F, class X, class Y, class U>
  struct Curryable7Helper3<R,F,X,Y,auto_curry_type,auto_curry_type,U> {
  static inline R go( const F& f, const X& x, const Y& y,const auto_curry_type&, const auto_curry_type&, const U& u)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper3 called with 3 args for x y and u" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind5,U> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_fifth,u)));
   }
};


template <class R, class F, class W,class X, class Y, class Z>
  struct Curryable7Helper3<R,F,W,X,Y,Z,auto_curry_type> {
  static inline R go( const F& f,const W& w,const X& x, const Y& y, const Z& z,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper3 called with 4 args for w x y and z" << std::endl;
#endif
     return makeFull3(impl::binder1to4of7<F,W,X,Y,Z >(f,w,x,y,z));
     //return makeFull3(impl::binderWtoZof7<F,std::pair<Bind1,W>,
     //		      std::pair<Bind2,X>,std::pair<Bind3,Y>,std::pair<Bind4,Z> >(f,
     //            std::make_pair(bind_first,w),
     //            std::make_pair(bind_second,x),
     //            std::make_pair(bind_third,y),
     //            std::make_pair(bind_fourth,z)));
   }
};

template <class R, class F, class X, class Z, class T>
  struct Curryable7Helper3<R,F,auto_curry_type,auto_curry_type,X,Z,T> {
  static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                      const X& x, const Z& z, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper3 called with 3 args for x z and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind3,X>,
		      std::pair<Bind4,Z>,std::pair<Bind5,T> >(f,
                   std::make_pair(bind_third,x),
                   std::make_pair(bind_fourth,z),
                   std::make_pair(bind_fifth,t)));
   }
};

template <class R, class F, class W,class X, class Y, class Z>
  struct Curryable7Helper3<R,F,auto_curry_type,W,X,Y,Z> {
  static inline R go( const F& f,const auto_curry_type&, const W& w, const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper3 called with 4 args for w x y and z" << std::endl;
#endif
     return makeFull3(impl::binderWtoZof7<F,std::pair<Bind2,W>,
		      std::pair<Bind3,X>,std::pair<Bind4,Y>,std::pair<Bind5,Z> >(f,
                   std::make_pair(bind_second,w),
                   std::make_pair(bind_third,x),
                   std::make_pair(bind_fourth,y),
                   std::make_pair(bind_fifth,z)));
   }
};

//////////////////////////////////////////////////////////////////////
// Curryable7Helper4 works on cases with 4 arguments.
//////////////////////////////////////////////////////////////////////

template <class R, class F, class W,class X, class Y, class Z>
  struct Curryable7Helper4 {
    static inline R go( const F& f, const W& w,const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper4 called with 4 args for w x y and z" << std::endl;
#endif
     return makeFull3(impl::binder1to4of7<F,W,X,Y,Z >(f,w,x,y,z));
   }
};

template <class R, class F, class X>
  struct Curryable7Helper4<R,F,X,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const X& x,const auto_curry_type&,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper4 called with 1 arg for x" << std::endl;
#endif
     return makeFull6(impl::binder1of7<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable7Helper4<R,F,auto_curry_type,Y,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const Y& y,const auto_curry_type&,
                 const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper4 called with 1 arg for y" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class Z>
  struct Curryable7Helper4<R,F,auto_curry_type,auto_curry_type,Z,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,const Z& z,
                 const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper4 called with 1 arg for z" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind3,Z> >(f,std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class T>
  struct Curryable7Helper4<R,F,auto_curry_type,auto_curry_type,auto_curry_type,T> {
    static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&,
                 const auto_curry_type&, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper4 called with 1 arg for t" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind4,T> >(f,std::make_pair(bind_fourth,t)));
   }
};

template <class R, class F, class X,class Y>
  struct Curryable7Helper4<R,F,X,Y,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const X& x, const Y& y,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper4 called with 2 args for x and y" << std::endl;
#endif
     return makeFull5(impl::binder1and2of7<F,X,Y>(f,x,y));
   }
};

template <class R, class F, class X,class Z>
  struct Curryable7Helper4<R,F,X,auto_curry_type,Z,auto_curry_type> {
    static inline R go( const F& f, const X& x,const auto_curry_type&,const Z& z,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper4 called with 2 args for x and z" << std::endl;
#endif
      return makeFull5(binderXandYof7<F,std::pair<Bind1,X>,std::pair<Bind3,Z> >
      (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class X,class Z>
  struct Curryable7Helper4<R,F,X,auto_curry_type,auto_curry_type,Z> {
  static inline R go( const F& f, const X& x,const auto_curry_type&,const auto_curry_type&,const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper4 called with 2 args for x and z" << std::endl;
#endif
      return makeFull5(binderXandYof7<F,std::pair<Bind1,X>,std::pair<Bind4,Z> >
      (f,std::make_pair(bind_first,x),std::make_pair(bind_fourth,z)));
   }
};

template <class R, class F, class X, class Y, class Z>
  struct Curryable7Helper4<R,F,X,Y,Z,auto_curry_type> {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper4 called with 3 args for x y and z" << std::endl;
#endif
     return makeFull4(impl::binder1to3of7<F,X,Y,Z >(f,x,y,z));
   }
};

template <class R, class F, class X, class Z, class T>
  struct Curryable7Helper4<R,F,X,auto_curry_type,Z,T> {
  static inline R go( const F& f, const X& x, const auto_curry_type&, const Z& z, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper4 called with 3 args for x z and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind3,Z>,std::pair<Bind4,T> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_third,z),
                   std::make_pair(bind_fourth,t)));
   }
};

template <class R, class F, class X, class Z, class T>
  struct Curryable7Helper4<R,F,auto_curry_type,X,Z,T> {
  static inline R go( const F& f, const auto_curry_type&, const X& x, const Z& z, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper4 called with 3 args for x z and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind2,X>,
		      std::pair<Bind3,Z>,std::pair<Bind4,T> >(f,
                   std::make_pair(bind_second,x),
                   std::make_pair(bind_third,z),
                   std::make_pair(bind_fourth,t)));
   }
};

template <class R, class F, class X, class Y, class T>
  struct Curryable7Helper4<R,F,X,Y,auto_curry_type,T> {
  static inline R go( const F& f, const X& x, const Y& y,const auto_curry_type&, const T& t)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper4 called with 3 args for x y and t" << std::endl;
#endif
     return makeFull4(impl::binderXtoZof7<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind4,T> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_fourth,t)));
   }
};

//////////////////////////////////////////////////////////////////////
// Curryable7Helper5 works on cases with 3 arguments.
//////////////////////////////////////////////////////////////////////


template <class R, class F, class X, class Y, class Z>
  struct Curryable7Helper5 {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper5 called with 3 args for x y and z" << std::endl;
#endif
     return makeFull4(impl::binder1to3of7<F,X,Y,Z >(f,x,y,z));
   }
};

template <class R, class F, class X>
  struct Curryable7Helper5<R,F,X,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const X& x,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper5 called with 1 arg for x" << std::endl;
#endif
     return makeFull6(impl::binder1of7<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable7Helper5<R,F,auto_curry_type,Y,auto_curry_type> {
    static inline R go( const F& f,const auto_curry_type&,const Y& y,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper5 called with 1 arg for y" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class Z>
  struct Curryable7Helper5<R,F,auto_curry_type,auto_curry_type,Z> {
    static inline R go( const F& f,const auto_curry_type&,const auto_curry_type&,const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper5 called with 1 arg for z" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind3,Z> >(f,std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class X,class Y>
  struct Curryable7Helper5<R,F,X,Y,auto_curry_type> {
    static inline R go( const F& f, const X& x, const Y& y,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper5 called with 2 args for x and y" << std::endl;
#endif
     return makeFull5(impl::binder1and2of7<F,X,Y>(f,x,y));
   }
};

template <class R, class F, class X,class Z>
  struct Curryable7Helper5<R,F,X,auto_curry_type,Z> {
    static inline R go( const F& f, const X& x,const auto_curry_type&,const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper5 called with 2 args for x and z" << std::endl;
#endif
      return makeFull5(binderXandYof7<F,std::pair<Bind1,X>,std::pair<Bind3,Z> >
      		      (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class Y,class Z>
  struct Curryable7Helper5<R,F,auto_curry_type,Y,Z> {
    static inline R go( const F& f,const auto_curry_type&,const Y& y,const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper5 called with 2 args for y and z" << std::endl;
#endif
      return makeFull5(binderXandYof7<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> >
      		      (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z)));
   }
};

//////////////////////////////////////////////////////////////////////
// Curryable7Helper6 works on cases with 2 arguments.
//////////////////////////////////////////////////////////////////////

template <class R, class F, class X, class Y>
struct Curryable7Helper6 {
   static inline R go( const F& f, const X& x, const Y& y) {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "Curryable7Helper6 called with 2 args" << std::endl;
#endif
     return makeFull5(impl::binder1and2of7<F,X,Y>(f,x,y));
   }
};

template <class R, class F, class X>
struct Curryable7Helper6<R,F,X,auto_curry_type> {
   static inline R go( const F& f, const X& x, const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper6 called with 1 arg for x" << std::endl;
#endif
      return makeFull6(binder1of7<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable7Helper6<R,F,auto_curry_type,Y> {
    static inline R go( const F& f, const auto_curry_type, const Y& y)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable7Helper6 called with 1 arg for y" << std::endl;
#endif
      return makeFull6(binderXof7<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};
#endif
