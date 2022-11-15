// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_HELPERS6_DOT_H
#define FCPP_CURRY_HELPERS6_DOT_H

//////////////////////////////////////////////////////////////////////
// Needed for Full6
//////////////////////////////////////////////////////////////////////

template <class R, class F, class X, class Y, class Z, class U, class V, class W>
struct Curryable6Helper {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z, 
                                   const U& u, const V& v, const W& w ) {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper called with 6 args" << std::endl;
#endif
      return f(x,y,z,u,v,w); 
   }
};

template <class R, class F, class X>
struct Curryable6Helper<R,F,X,auto_curry_type,auto_curry_type,auto_curry_type,
                              auto_curry_type,auto_curry_type> 
{
   static inline R go( const F& f, const X& x, const auto_curry_type, const auto_curry_type,
                 const auto_curry_type, const auto_curry_type, const auto_curry_type) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper called with 1 arg for x" << std::endl;
#endif
      return makeFull5(binder1of6<F,X>(f,x));
   }
};


template <class R, class F, class Y>
  struct Curryable6Helper<R,F,auto_curry_type,Y,auto_curry_type,auto_curry_type,
                               auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const Y& y, const auto_curry_type&,
		 const auto_curry_type&,const auto_curry_type&,const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper called with 1 arg for y" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class Z>
  struct Curryable6Helper<R,F,auto_curry_type,auto_curry_type,Z,
                              auto_curry_type,auto_curry_type,auto_curry_type>
{
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,const Z&z,
                 const auto_curry_type&,const auto_curry_type&, const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper called with 1 arg for z" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind3,Z> >(f,std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class U>
  struct Curryable6Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,
                          U,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
         const auto_curry_type&, const U& u,const auto_curry_type&,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper called with 1 arg for u" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind4,U> >(f,std::make_pair(bind_fourth,u)));
   }
};
template <class R, class F, class V>
  struct Curryable6Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,V,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,const auto_curry_type&, const auto_curry_type&, const V& v,const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper called with 1 arg for v" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind5,V> >(f,std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class W>
  struct Curryable6Helper<R,F,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,W> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,const auto_curry_type&,const auto_curry_type&, const auto_curry_type&, const W& w) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper called with 1 arg for w" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind6,W> >(f,std::make_pair(bind_sixth,w)));
   }
};

template <class R, class F, class Y, class V,class W>
  struct Curryable6Helper<R,F,auto_curry_type,Y,auto_curry_type,auto_curry_type,V,W> {
  static inline R go( const F& f, const auto_curry_type&, const Y& y,const auto_curry_type&,const auto_curry_type&, const V& v, const W& w)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper called with 3 args for y, v & w" << std::endl;
#endif
      return makeFull3(binderXtoZof6<F,std::pair<Bind2,Y>,std::pair<Bind5,V>,std::pair<Bind6,W> >(f,std::make_pair(bind_second,y),std::make_pair(bind_fifth,v),std::make_pair(bind_sixth,w)));
   }
};

template <class R, class F, class Z, class U,class V>
  struct Curryable6Helper<R,F,auto_curry_type,auto_curry_type,Z,U,V,auto_curry_type> {
  static inline R go( const F& f, const auto_curry_type&,const auto_curry_type&, const Z& z,const U& u, const V& v, const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper called with 3 args for z, u & v" << std::endl;
#endif
      return makeFull3(binderXtoZof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U>,std::pair<Bind5,V> >(f,std::make_pair(bind_third,z),std::make_pair(bind_fourth,u),std::make_pair(bind_fifth,v)));
   }
};


 template <class R, class F, class X, class Y, class Z, class U,class V>
   struct Curryable6Helper<R,F,X,Y,Z,U,V,auto_curry_type> {
  static inline R go( const F& f, const X& x, const Y& y, const Z& z, const U& u,const V& v, const auto_curry_type) {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper called with 5 args for x,y,z,u,v" << std::endl;
#endif
     return makeFull1(impl::binder1to5of6<F,X,Y,Z,U,V>(f,x,y,z,u,v));
   }
};

template <class R, class F, class X, class Y, class Z, class U, class V>
  struct Curryable6Helper<R,F,auto_curry_type,X,Y,Z,U,V> {
  static inline R go( const F& f, const auto_curry_type&, const X& x,const Y& y,
                      const Z& z, const U& u, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper called with 5 args for x y z u and v" << std::endl;
#endif
     return makeFull1(impl::binderVtoZof6<F,std::pair<Bind2,X>,
           std::pair<Bind3,Y>,std::pair<Bind4,Z>,std::pair<Bind5,U>,
           std::pair<Bind6,V> >(f,
                   std::make_pair(bind_second,x),
		   std::make_pair(bind_third,y),
                   std::make_pair(bind_fourth,z),
                   std::make_pair(bind_fifth,u),
		   std::make_pair(bind_sixth,v)));
   }
};


 template <class R, class F, class X, class Y, class Z, class U,class V>
struct Curryable6Helper2 {
  static inline R go( const F& f, const X& x, const Y& y, const Z& z, const U& u,const V& v) {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 5 args" << std::endl;
#endif
     return makeFull1(impl::binder1to5of6<F,X,Y,Z,U,V>(f,x,y,z,u,v));
   }
};

template <class R, class F, class X>
struct Curryable6Helper2<R,F,X,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type> {
   static inline R go( const F& f, const X& x, const auto_curry_type, const auto_curry_type,
                                        const auto_curry_type, const auto_curry_type) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 1 arg for x" << std::endl;
#endif
      return makeFull5(binder1of6<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable6Helper2<R,F,auto_curry_type,Y,auto_curry_type,auto_curry_type,
                               auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const Y& y, const auto_curry_type&,
		 const auto_curry_type&,const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 1 arg for y" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class Z>
  struct Curryable6Helper2<R,F,auto_curry_type,auto_curry_type,Z,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,const Z&z,const auto_curry_type&, const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 1 arg for z" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind3,Z> >(f,std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class U>
  struct Curryable6Helper2<R,F,auto_curry_type,auto_curry_type,auto_curry_type,U,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,const auto_curry_type&, const U& u,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 1 arg for u" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind4,U> >(f,std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class V>
  struct Curryable6Helper2<R,F,auto_curry_type,auto_curry_type,auto_curry_type,auto_curry_type,V> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,const auto_curry_type&, const auto_curry_type&, const V& v) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 1 arg for v" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind5,V> >(f,std::make_pair(bind_fifth,v)));
   }
};


template <class R, class F, class X,class Y>
  struct Curryable6Helper2<R,F,X,Y,auto_curry_type,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const X& x, const Y& y,const auto_curry_type&,
                 const auto_curry_type&, const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 2 args for x and y" << std::endl;
#endif
     return makeFull4(impl::binder1and2of6<F,X,Y>(f,x,y));
     // return makeFull4(binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind2,Y> >
     //		      (f,std::make_pair(bind_first,x),std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class X,class Z>
  struct Curryable6Helper2<R,F,X,auto_curry_type,Z,auto_curry_type,auto_curry_type> {
  static inline R go( const F& f, const X& x, const auto_curry_type&, const Z& z, 
		 const auto_curry_type&, const auto_curry_type& ) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 2 args for x and z" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> >
		      (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class X,class U>
  struct Curryable6Helper2<R,F,X,auto_curry_type,auto_curry_type,U,auto_curry_type> {
    static inline R go( const F& f, const X& x, const auto_curry_type&,
		 const auto_curry_type&, const U& u, const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 2 args for x and u" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind4,U> >
		      (f,std::make_pair(bind_first,x),std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class X,class V>
  struct Curryable6Helper2<R,F,X,auto_curry_type,auto_curry_type,auto_curry_type,V> {
    static R go( const F& f, const X& x, const auto_curry_type&,
		 const auto_curry_type&, const auto_curry_type&, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 2 args for x and v" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind5,V> >
		      (f,std::make_pair(bind_first,x),std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class Y,class Z>
  struct Curryable6Helper2<R,F,auto_curry_type,Y,Z,auto_curry_type,auto_curry_type> {
  static inline R go( const F& f, const auto_curry_type&, const Y& y, const Z&z,
		 const auto_curry_type&, const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 2 args for y and z" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> >
		    (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class Y,class U>
  struct Curryable6Helper2<R,F,auto_curry_type,Y,auto_curry_type,U,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const Y& y,
		 const auto_curry_type&, const U& u,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 2 args for y and u" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind4,U> >
		    (f,std::make_pair(bind_second,y),std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class Y,class V>
  struct Curryable6Helper2<R,F,auto_curry_type,Y,auto_curry_type,auto_curry_type,V> {
    static inline R go( const F& f, const auto_curry_type&, const Y& y,
		 const auto_curry_type&, const auto_curry_type&, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 2 args for y and v" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind5,V> >
		    (f,std::make_pair(bind_second,y),std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class Z,class U>
  struct Curryable6Helper2<R,F,auto_curry_type,auto_curry_type,Z,U,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
			const Z& z, const U& u,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 2 args for z and u" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U> >
		    (f,std::make_pair(bind_third,z),std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class Z,class V>
  struct Curryable6Helper2<R,F,auto_curry_type,auto_curry_type,Z,auto_curry_type,V> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                 const Z& z, const auto_curry_type&, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 2 args for z and v" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind5,V> >
		    (f,std::make_pair(bind_third,z),std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class U,class V>
  struct Curryable6Helper2<R,F,auto_curry_type,auto_curry_type,auto_curry_type,U,V> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                  const auto_curry_type&, const U& u, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper2 called with 2 args for u and v" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind4,U>,std::pair<Bind5,V> >
		    (f,std::make_pair(bind_fourth,u),std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class X, class Y, class Z>
  struct Curryable6Helper2<R,F,X,Y,Z,auto_curry_type,auto_curry_type> {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z, const auto_curry_type&, const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 3 args for x y and z" << std::endl;
#endif
     return makeFull3(impl::binder1to3of6<F,X,Y,Z>(f,x,y,z));
   }
};

template <class R, class F, class X, class Y, class U>
  struct Curryable6Helper2<R,F,X,Y,auto_curry_type,U,auto_curry_type> {
   static inline R go( const F& f, const X& x, const Y& y, const auto_curry_type&,
                                   const U& u, const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 3 args for x y and u" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind4,U> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class X, class Y, class V>
  struct Curryable6Helper2<R,F,X,Y,auto_curry_type,auto_curry_type,V> {
   static inline R go( const F& f, const X& x, const Y& y, const auto_curry_type&,
		       const auto_curry_type&, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 3 args for x y and v" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind5,V> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class X, class Z, class U>
  struct Curryable6Helper2<R,F,X,auto_curry_type,Z,U,auto_curry_type> {
    static inline R go( const F& f, const X& x, const auto_curry_type&, const Z& z,const U& u, const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 3 args for x z and u" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind3,Z>,std::pair<Bind4,U> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_third,z),
                   std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class X, class Z, class V>
  struct Curryable6Helper2<R,F,X,auto_curry_type,Z,auto_curry_type,V> {
    static inline R go( const F& f, const X& x, const auto_curry_type&, const Z& z,
                        const auto_curry_type&, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 3 args for x z and v" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind3,Z>,std::pair<Bind5,V> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_third,z),
                   std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class X, class U, class V>
  struct Curryable6Helper2<R,F,X,auto_curry_type,auto_curry_type,U,V> {
    static inline R go( const F& f, const X& x, const auto_curry_type&,
                        const auto_curry_type&, const U& u, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 3 args for x u and v" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind4,U>,std::pair<Bind5,V> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_fourth,u),
                   std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class Y, class Z, class U>
  struct Curryable6Helper2<R,F,auto_curry_type,Y,Z,U,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const Y& y,const Z& z,
                        const U& u, const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 3 args for y z and u" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind2,Y>,
		      std::pair<Bind3,Z>,std::pair<Bind4,U> >(f,
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_third,z),
                   std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class Y, class Z, class V>
  struct Curryable6Helper2<R,F,auto_curry_type,Y,Z,auto_curry_type,V> {
    static inline R go( const F& f, const auto_curry_type&, const Y& y,const Z& z,
                        const auto_curry_type&, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 3 args for y z and v" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind2,Y>,
		      std::pair<Bind3,Z>,std::pair<Bind5,V> >(f,
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_third,z),
                   std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class Y, class U, class V>
  struct Curryable6Helper2<R,F,auto_curry_type,Y,auto_curry_type,U,V> {
    static inline R go( const F& f, const auto_curry_type&, const Y& y,
                        const auto_curry_type&, const U& u, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 3 args for y u and v" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind2,Y>,
		      std::pair<Bind4,U>,std::pair<Bind5,V> >(f,
                   std::make_pair(bind_second,y),
		   std::make_pair(bind_fourth,u),
                   std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class Z, class U, class V>
  struct Curryable6Helper2<R,F,auto_curry_type,auto_curry_type,Z,U,V> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                        const Z& z, const U& u, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 3 args for z u and v" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind3,Z>,
		      std::pair<Bind4,U>,std::pair<Bind5,V> >(f,
                   std::make_pair(bind_third,z),
		   std::make_pair(bind_fourth,u),
                   std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class X, class Y, class Z, class U>
  struct Curryable6Helper2<R,F,X,Y,Z,U,auto_curry_type> {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z, const U& u, const auto_curry_type) {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 4 args for x,y,z,u" << std::endl;
#endif
     return makeFull2(impl::binder1to4of6<F,X,Y,Z,U>(f,x,y,z,u)); 
   }
};

template <class R, class F, class X, class Y, class Z, class V>
  struct Curryable6Helper2<R,F,X,Y,Z,auto_curry_type,V> {
    static inline R go( const F& f, const X& x, const Y& y,const Z& z,
                        const auto_curry_type&, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 4 args for x y z and v" << std::endl;
#endif
     return makeFull2(impl::binderWtoZof6<F,std::pair<Bind1,X>,
           std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,V> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_third,z),
                   std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class X, class Y, class U, class V>
  struct Curryable6Helper2<R,F,X,Y,auto_curry_type,U,V> {
    static inline R go( const F& f, const X& x, const Y& y,
                        const auto_curry_type&, const U& u, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 4 args for x y u and v" << std::endl;
#endif
     return makeFull2(impl::binderWtoZof6<F,std::pair<Bind1,X>,
           std::pair<Bind2,Y>,std::pair<Bind4,U>,std::pair<Bind5,V> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_fourth,u),
                   std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class X, class Z, class U, class V>
  struct Curryable6Helper2<R,F,X,auto_curry_type,Z,U,V> {
    static inline R go( const F& f, const X& x, const auto_curry_type&,
                        const Z& z, const U& u, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 4 args for x z u and v" << std::endl;
#endif
     return makeFull2(impl::binderWtoZof6<F,std::pair<Bind1,X>,
           std::pair<Bind3,Z>,std::pair<Bind4,U>,std::pair<Bind5,V> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_third,z),
                   std::make_pair(bind_fourth,u),
                   std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class Y, class Z, class U, class V>
  struct Curryable6Helper2<R,F,auto_curry_type,Y,Z,U,V> {
   static inline R go( const F& f, const auto_curry_type&, const Y& y,
                       const Z& z, const U& u, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper2 called with 4 args for y z u and v" << std::endl;
#endif
     return makeFull2(impl::binderWtoZof6<F,std::pair<Bind2,Y>,
           std::pair<Bind3,Z>,std::pair<Bind4,U>,std::pair<Bind5,V> >(f,
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_third,z),
                   std::make_pair(bind_fourth,u),
                   std::make_pair(bind_fifth,v)));
   }
};

template <class R, class F, class X, class Y, class Z, class U>
struct Curryable6Helper3 {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z, const U& u) {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper3 called with 4 args" << std::endl;
#endif
     return makeFull2(impl::binder1to4of6<F,X,Y,Z,U>(f,x,y,z,u)); 
   }
};

template <class R, class F, class X>
struct Curryable6Helper3<R,F,X,auto_curry_type,auto_curry_type,auto_curry_type> {
   static R go( const F& f, const X& x, const auto_curry_type, const auto_curry_type, const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper3 called with 1 arg for x" << std::endl;
#endif
      return makeFull5(binder1of6<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable6Helper3<R,F,auto_curry_type,Y,auto_curry_type,auto_curry_type> {
    static R go( const F& f, const auto_curry_type&, const Y& y, const auto_curry_type&,
                             const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper3 called with 1 arg for y" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class Z>
  struct Curryable6Helper3<R,F,auto_curry_type,auto_curry_type,Z,auto_curry_type> {
    static R go( const F& f, const auto_curry_type&, const auto_curry_type&, const Z& z,
                             const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper3 called with 1 arg for z" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind3,Z> >(f,std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class U>
  struct Curryable6Helper3<R,F,auto_curry_type,auto_curry_type,auto_curry_type,U> {
    static R go( const F& f, const auto_curry_type&, const auto_curry_type&,
                 const auto_curry_type&, const U& u) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper3 called with 1 arg for u" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind4,U> >(f,std::make_pair(bind_fourth,u)));
   }
};


template <class R, class F, class X,class Y>
  struct Curryable6Helper3<R,F,X,Y,auto_curry_type,auto_curry_type> {
    static R go( const F& f, const X& x, const Y& y,const auto_curry_type&,
                 const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper3 called with 2 args for x and y" << std::endl;
#endif
     return makeFull4(impl::binder1and2of6<F,X,Y>(f,x,y));
     // return makeFull4(binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind2,Y> >
     //		      (f,std::make_pair(bind_first,x),std::make_pair(bind_second,y)));
   }
};


template <class R, class F, class X,class Z>
  struct Curryable6Helper3<R,F,X,auto_curry_type,Z,auto_curry_type> {
    static R go( const F& f, const X& x, const auto_curry_type&, const Z& z, 
                             const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper3 called with 2 args for x and z" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> >
		      (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z)));
   }
};


template <class R, class F, class X,class U>
  struct Curryable6Helper3<R,F,X,auto_curry_type,auto_curry_type,U> {
    static R go( const F& f, const X& x, const auto_curry_type&, 
		 const auto_curry_type&, const U& u) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper3 called with 2 args for x and u" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind4,U> >
		      (f,std::make_pair(bind_first,x),std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class Y,class Z>
  struct Curryable6Helper3<R,F,auto_curry_type,Y,Z,auto_curry_type> {
    static R go( const F& f, const auto_curry_type&, const Y& y, const Z& z, 
                             const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper3 called with 2 args for y and z" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> >
		      (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class Y,class U>
  struct Curryable6Helper3<R,F,auto_curry_type,Y,auto_curry_type,U> {
    static R go( const F& f, const auto_curry_type&, const Y& y, 
		 const auto_curry_type&,const U& u) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper3 called with 2 args for y and u" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind4,U> >
		      (f,std::make_pair(bind_second,y),std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class Z,class U>
  struct Curryable6Helper3<R,F,auto_curry_type,auto_curry_type,Z,U> {
    static R go( const F& f, const auto_curry_type&,const auto_curry_type&, const Z& z, 
		 const U& u) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper3 called with 2 args for z and u" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind3,Z>,std::pair<Bind4,U> >
		      (f,std::make_pair(bind_third,z),std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class X, class Y, class Z>
  struct Curryable6Helper3<R,F,X,Y,Z,auto_curry_type> {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z, const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper3 called with 3 args for x y and z" << std::endl;
#endif
     return makeFull3(impl::binder1to3of6<F,X,Y,Z>(f,x,y,z));
   }
};

template <class R, class F, class X, class Y, class U>
  struct Curryable6Helper3<R,F,X,Y,auto_curry_type,U> {
   static inline R go( const F& f, const X& x, const Y& y, const auto_curry_type&, const U& u)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper3 called with 3 args for x y and u" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind2,Y>,std::pair<Bind4,U> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class X, class Z, class U>
  struct Curryable6Helper3<R,F,X,auto_curry_type,Z,U> {
    static inline R go( const F& f, const X& x, const auto_curry_type&, const Z& z,const U& u)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper3 called with 3 args for x z and u" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind1,X>,
		      std::pair<Bind3,Z>,std::pair<Bind4,U> >(f,
                   std::make_pair(bind_first,x),
                   std::make_pair(bind_third,z),
                   std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class Y, class Z, class U>
  struct Curryable6Helper3<R,F,auto_curry_type,Y,Z,U> {
    static inline R go( const F& f, const auto_curry_type&, const Y& y,const Z& z,const U& u)
   {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper3 called with 3 args for y z and u" << std::endl;
#endif
     return makeFull3(impl::binderXtoZof6<F,std::pair<Bind2,Y>,
		      std::pair<Bind3,Z>,std::pair<Bind4,U> >(f,
                   std::make_pair(bind_second,y),
                   std::make_pair(bind_third,z),
                   std::make_pair(bind_fourth,u)));
   }
};

template <class R, class F, class X, class Y, class Z>
struct Curryable6Helper4 {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z) {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper4 called with 3 args" << std::endl;
#endif
     return makeFull3(impl::binder1to3of6<F,X,Y,Z>(f,x,y,z));
   }
};

template <class R, class F, class X>
struct Curryable6Helper4<R,F,X,auto_curry_type,auto_curry_type> {
   static R go( const F& f, const X& x, const auto_curry_type, const auto_curry_type) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper4 called with 1 arg for x" << std::endl;
#endif
      return makeFull5(binder1of6<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable6Helper4<R,F,auto_curry_type,Y,auto_curry_type> {
    static R go( const F& f, const auto_curry_type&, const Y& y, const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper4 called with 1 arg for y" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class Z>
  struct Curryable6Helper4<R,F,auto_curry_type,auto_curry_type,Z> {
    static R go( const F& f, const auto_curry_type&, const auto_curry_type&, const Z& z) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper4 called with 1 arg for z" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind3,Z> >(f,std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class X,class Y>
  struct Curryable6Helper4<R,F,X,Y,auto_curry_type> {
    static R go( const F& f, const X& x, const Y& y,const auto_curry_type&) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper4 called with 2 args for x and y" << std::endl;
#endif
     return makeFull4(impl::binder1and2of6<F,X,Y>(f,x,y));
     //return makeFull4(binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind2,Y> >
     //		      (f,std::make_pair(bind_first,x),std::make_pair(bind_second,y)));
   }
};

template <class R, class F, class X,class Z>
  struct Curryable6Helper4<R,F,X,auto_curry_type,Z> {
    static R go( const F& f, const X& x, const auto_curry_type&, const Z& z) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper4 called with 2 args for x and z" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind1,X>,std::pair<Bind3,Z> >
		      (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z)));
   }
};

template <class R, class F, class Y,class Z>
  struct Curryable6Helper4<R,F,auto_curry_type,Y,Z> {
    static R go( const F& f, const auto_curry_type&, const Y& y, const Z& z) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper4 called with 2 args for y and z" << std::endl;
#endif
      return makeFull4(binderXandYof6<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> >
		      (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z)));
   }
};


template <class R, class F, class X, class Y>
struct Curryable6Helper5 {
   static inline R go( const F& f, const X& x, const Y& y) {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "Curryable6Helper5 called with 2 args" << std::endl;
#endif
     return makeFull4(impl::binder1and2of6<F,X,Y>(f,x,y));
   }
};

template <class R, class F, class X>
struct Curryable6Helper5<R,F,X,auto_curry_type> {
   static R go( const F& f, const X& x, const auto_curry_type) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper5 called with 1 arg for x" << std::endl;
#endif
      return makeFull5(binder1of6<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable6Helper5<R,F,auto_curry_type,Y> {
    static R go( const F& f, const auto_curry_type&, const Y& y) 
   {
#ifdef FCPP_PRINT_HELPERS6
      std::cout << "Curryable6Helper5 called with 1 arg for y" << std::endl;
#endif
      return makeFull5(binderXof6<F,std::pair<Bind2,Y> >(f,std::make_pair(bind_second,y)));
   }
};


#endif
