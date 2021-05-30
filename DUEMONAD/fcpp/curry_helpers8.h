// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_HELPERS8_DOT_H
#define FCPP_CURRY_HELPERS8_DOT_H

//////////////////////////////////////////////////////////////////////
// Needed for Full8
//////////////////////////////////////////////////////////////////////

template <class R, class F, class X, class Y, class Z, class S, class T, class U, class V, class W>
struct Curryable8Helper {
  static inline R go( const F& f, const X& x, const Y& y, const Z& z, const S& s,
                      const T& t, const U&u, const V& v, const W& w ) {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "Curryable8Helper called with 8 args" << std::endl;
#endif
      return f(x,y,z,s,t,u,v,w);
   }
};


template <class R, class F, class W,class X, class Y, class Z>
  struct Curryable8Helper5 {
    static inline R go( const F& f, const W& w,const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable8Helper5 called with 4 args for w x y and z" << std::endl;
#endif
     return makeFull4(impl::binderWtoZof8<F,std::pair<Bind1,W>,std::pair<Bind2,X>,
		      std::pair<Bind3,Y>,std::pair<Bind4,Z> >(f,
                   std::make_pair(bind_first,w),
                   std::make_pair(bind_second,x),
                   std::make_pair(bind_third,y),
                   std::make_pair(bind_fourth,z)));
   }
};


// I am not sure if this is the correct name for this one.
template <class R, class F, class X, class Y, class Z>
  struct Curryable8Helper6 {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS8
     std::cout << "Curryable8Helper6 called with 3 args for x y and z" << std::endl;
#endif
     return makeFull5(impl::binderXtoZof8<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind3,Z> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_third,z)));
   }
};


template <class R, class F, class X>
  struct Curryable8Helper6<R,F,X,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const X& x,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "Curryable8Helper6 called with 1 arg for x" << std::endl;
#endif
     return makeFull7(impl::binder1of8<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable8Helper6<R,F,auto_curry_type,Y,auto_curry_type> {
    static inline R go( const F& f,const auto_curry_type&,const Y& y,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "Curryable8Helper6 called with 1 arg for y" << std::endl;
#endif
      return makeFull7(binderXof8<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class Z>
  struct Curryable8Helper6<R,F,auto_curry_type,auto_curry_type,Z> {
    static inline R go( const F& f,const auto_curry_type&,const auto_curry_type&,const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "Curryable8Helper6 called with 1 arg for z" << std::endl;
#endif
      return makeFull7(binderXof8<F,std::pair<Bind3,Z> >(f,std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class X,class Y>
  struct Curryable8Helper6<R,F,X,Y,auto_curry_type> {
    static inline R go( const F& f, const X& x, const Y& y,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "Curryable8Helper6 called with 2 args for x and y" << std::endl;
#endif
     return makeFull6(impl::binder1and2of8<F,X,Y>(f,x,y));
   }
};

template <class R, class F, class X,class Z>
  struct Curryable8Helper6<R,F,X,auto_curry_type,Z> {
    static inline R go( const F& f, const X& x,const auto_curry_type&,const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "Curryable8Helper6 called with 2 args for x and z" << std::endl;
#endif
      return makeFull6(binderXandYof8<F,std::pair<Bind1,X>,std::pair<Bind3,Z> >
      		      (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class Y,class Z>
  struct Curryable8Helper6<R,F,auto_curry_type,Y,Z> {
    static inline R go( const F& f,const auto_curry_type&,const Y& y,const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "Curryable8Helper6 called with 2 args for y and z" << std::endl;
#endif
      return makeFull6(binderXandYof8<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> >
      		      (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class X, class Y>
struct Curryable8Helper7 {
   static inline R go( const F& f, const X& x, const Y& y) {
#ifdef FCPP_PRINT_HELPERS8
     std::cout << "Curryable8Helper7 called with 2 args" << std::endl;
#endif
     return makeFull6(impl::binder1and2of8<F,X,Y>(f,x,y));
   }
};

template <class R, class F, class X>
struct Curryable8Helper7<R,F,X,auto_curry_type> {
   static inline R go( const F& f, const X& x, const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "Curryable8Helper7 called with 1 arg for x" << std::endl;
#endif
      return makeFull7(binder1of8<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable8Helper7<R,F,auto_curry_type,Y> {
    static inline R go( const F& f, const auto_curry_type, const Y& y)
   {
#ifdef FCPP_PRINT_HELPERS7
      std::cout << "Curryable8Helper7 called with 1 arg for y" << std::endl;
#endif
      return makeFull7(binderXof8<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};

#endif
