// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_HELPERS5_DOT_H
#define FCPP_CURRY_HELPERS5_DOT_H

//////////////////////////////////////////////////////////////////////
// Needed for Full5
//////////////////////////////////////////////////////////////////////

template <class R, class F, class X, class Y, class Z, class V, class W>
struct Curryable5Helper {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z, const V& v, const W& w ) {
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "Curryable5Helper called with 5 args" << std::endl;
#endif
      return f(x,y,z,v,w); 
   }
};

template <class R, class F, class X>
struct Curryable5Helper<R,F,X,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType> {
   static inline R go( const F& f, const X& x, const AutoCurryType&, const AutoCurryType&, const AutoCurryType&, const AutoCurryType& ) {
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "Curryable5Helper called with 1 arg for x" << std::endl;
#endif
      return makeFull4( binder1of5<F,X>(f,x) );
   }
};

template <class R, class F, class Y>
  struct Curryable5Helper<R,F,AutoCurryType,Y,AutoCurryType,AutoCurryType,AutoCurryType> {
    static inline R go( const F& f, const AutoCurryType&, const Y& y,const AutoCurryType&, const AutoCurryType&, const AutoCurryType& ) {
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "Curryable5Helper called with 1 arg for y" << std::endl;
#endif
      return makeFull4( binder2of5<F,Y>(f,y) );
   }
};

template <class R, class F, class Z>
  struct Curryable5Helper<R,F,AutoCurryType,AutoCurryType,Z,AutoCurryType,AutoCurryType> {
    static R go( const F& f, const AutoCurryType&, const AutoCurryType&, const Z& z,const AutoCurryType&, const AutoCurryType& ) {
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "Curryable5Helper called with 1 arg for z" << std::endl;
#endif
      return makeFull4(binderAof5<F,Bind3,Z>(f,bind_third,z));
   }
};

template <class R, class F, class V>
  struct Curryable5Helper<R,F,AutoCurryType,AutoCurryType,AutoCurryType,V,AutoCurryType> {
    static inline R go( const F& f, const AutoCurryType&, const AutoCurryType&, const AutoCurryType&, const V& v,const AutoCurryType& ) {
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "Curryable5Helper called with 1 arg for v" << std::endl;
#endif
      return makeFull4(binderAof5<F,Bind4,V>(f,bind_fourth,v));
   }
};

template <class R, class F, class W>
  struct Curryable5Helper<R,F,AutoCurryType,AutoCurryType,AutoCurryType,AutoCurryType,W> {
    static inline R go( const F& f, const AutoCurryType&, const AutoCurryType&, const AutoCurryType&, const AutoCurryType&, const W& w ) {
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "Curryable5Helper called with 1 arg for w" << std::endl;
#endif
      return makeFull4(binderAof5<F,Bind5,W>(f,bind_fifth,w));
   }
};

 template <class R, class F, class X, class Y>
struct Curryable5Helper<R,F,X,Y,auto_curry_type,auto_curry_type,auto_curry_type> {
   static inline R go( const F& f, const X& x, const Y& y, const auto_curry_type, const auto_curry_type, const auto_curry_type) 
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 2 args for x and y"
               << std::endl;
#endif
     return makeFull3(impl::binder1and2of5<F,X,Y>(f,x,y));
   }
};

 template <class R, class F, class X, class Z>
   struct Curryable5Helper<R,F,X,auto_curry_type,Z,auto_curry_type,auto_curry_type> {
     static inline R go( const F& f, const X& x, const auto_curry_type, const Z& z,const auto_curry_type, const auto_curry_type) 
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 2 args for x and z"
               << std::endl;
#endif
     return makeFull3(binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z> >
		     (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z)));
   }
};

 template <class R, class F, class X, class V>
   struct Curryable5Helper<R,F,X,auto_curry_type,auto_curry_type,V,auto_curry_type> {
     static inline R go( const F& f, const X& x, const auto_curry_type, const auto_curry_type, const V& v,const auto_curry_type) 
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 2 args for x and v"
               << std::endl;
#endif
      return makeFull3(binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind4,V> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_fourth,v)));
   }
};

 template <class R, class F, class X, class W>
   struct Curryable5Helper<R,F,X,auto_curry_type,auto_curry_type,auto_curry_type,W> {
     static inline R go( const F& f, const X& x, const auto_curry_type, const auto_curry_type, const auto_curry_type, const W& w) 
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 2 args for x and w"
               << std::endl;
#endif
      return makeFull3(binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind5,W> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_fifth,w)));
   }
};


 template <class R, class F, class Y, class Z>
   struct Curryable5Helper<R,F,auto_curry_type,Y,Z,auto_curry_type,auto_curry_type> {
     static inline R go( const F& f,const auto_curry_type, const Y& y,const Z& z,const auto_curry_type,const auto_curry_type) 
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 2 args for y and z"
               << std::endl;
#endif
     return makeFull3(binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> >
     		     (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z)));
     //return makeFull3(binder2of4<binder2of5<F,Y>,Z>(binder2of5<F,Y>(f,y),z));
   }
};

 template <class R, class F, class Y, class V>
   struct Curryable5Helper<R,F,auto_curry_type,Y,auto_curry_type,V,auto_curry_type> {
     static inline R go( const F& f,const auto_curry_type, const Y& y,const auto_curry_type, const V& v,const auto_curry_type) 
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 2 args for y and v"
               << std::endl;
#endif
     return makeFull3(binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind4,V> >
     		       (f,std::make_pair(bind_second,y),std::make_pair(bind_fourth,v)));
   }
};

 template <class R, class F, class Y, class W>
   struct Curryable5Helper<R,F,auto_curry_type,Y,auto_curry_type,auto_curry_type,W> {
     static inline R go( const F& f,const auto_curry_type, const Y& y,const auto_curry_type, const auto_curry_type, const W& w) 
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 2 args for y and w"
               << std::endl;
#endif
     return makeFull3(binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind5,W> >
     		     (f,std::make_pair(bind_second,y),std::make_pair(bind_fifth,w)));
   }
};

 template <class R, class F, class Z, class V>
   struct Curryable5Helper<R,F,AutoCurryType,AutoCurryType,Z,V,AutoCurryType> {
     static inline R go( const F& f, const AutoCurryType&, const AutoCurryType&, const Z& z,const V& v,const AutoCurryType&) {
#ifdef FCPP_PRINT_HELPERS5
       std::cout << "Curryable5Helper called with 2 args for z and v"
                 << std::endl;
#endif
     return makeFull3(binderXandYof5<F,std::pair<Bind3,Z>,std::pair<Bind4,V> >
     		     (f,std::make_pair(bind_third,z),std::make_pair(bind_fourth,v)));
   }
};

// this does (_,_,z,_,w) using a combination of binders converted to use binderXandYof5.
 template <class R, class F, class Z, class W>
  struct Curryable5Helper<R,F,AutoCurryType,AutoCurryType,Z,AutoCurryType,W> {
    static inline R go( const F& f, const AutoCurryType&, const AutoCurryType&, const Z& z,const AutoCurryType&, const W& w) {
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "Curryable5Helper called with 2 args for z and w"
                << std::endl;
#endif
      return makeFull3(binderXandYof5<F,std::pair<Bind3,Z>,std::pair<Bind5,W> >
		    (f,std::make_pair(bind_third,z),std::make_pair(bind_fifth,w)));
   }
};

// this does (_,_,_,v,w) using a combination of binders converted to use binderXandYof5.
 template <class R, class F, class V, class W>
   struct Curryable5Helper<R,F,AutoCurryType,AutoCurryType,AutoCurryType,V,W> {
     static inline R go( const F& f, const AutoCurryType&, const AutoCurryType&, const AutoCurryType&, const V& v,const W& w) {
#ifdef FCPP_PRINT_HELPERS5
       std::cout << "Curryable5Helper called with 2 args for v and w"
                 << std::endl;
#endif
      return makeFull3(binderXandYof5<F,std::pair<Bind4,V>,std::pair<Bind5,W> >
		    (f,std::make_pair(bind_fourth,v),std::make_pair(bind_fifth,w)));
   }
};

 template <class R, class F, class X, class Y, class Z>
struct Curryable5Helper<R,F,X,Y,Z,auto_curry_type,auto_curry_type> {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z, const auto_curry_type, const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 3 args for x y and z"
               << std::endl;
#endif
     return makeFull2(impl::binder1to3of5<F,X,Y,Z>(f,x,y,z));
   }
};

template <class R, class F, class X, class Y, class V>
  struct Curryable5Helper<R,F,X,Y,auto_curry_type,V,auto_curry_type> {
    static inline R go( const F& f, const X& x, const Y& y, const auto_curry_type, const V& v,const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 3 args for x y and v"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_second,y),
                          std::make_pair(bind_fourth,v)));
   }
};

template <class R, class F, class X, class Y, class W>
  struct Curryable5Helper<R,F,X,Y,auto_curry_type,auto_curry_type,W> {
    static inline R go( const F& f, const X& x, const Y& y, const auto_curry_type, const auto_curry_type, const W& w)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 3 args for x y and w"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,
                                            std::pair<Bind5,W> >
		      (f,std::make_pair(bind_first,x),std::make_pair(bind_second,y),
                          std::make_pair(bind_fifth,w)));
   }
};

template <class R, class F, class X, class Z, class V>
  struct Curryable5Helper<R,F,X,auto_curry_type,Z,V,auto_curry_type> {
    static inline R go( const F& f, const X& x, const auto_curry_type, const Z& z, const V& v,const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 3 args for x z and v"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z),
                          std::make_pair(bind_fourth,v)));
   }
};

template <class R, class F, class X, class Z, class W>
   struct Curryable5Helper<R,F,X,auto_curry_type,Z,auto_curry_type,W> {
     static inline R go( const F& f, const X& x, const auto_curry_type, const Z& z, const auto_curry_type, const W& w)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 3 args for x z and w"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind5,W> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z),
                          std::make_pair(bind_fifth,w)));
   }
};

template <class R, class F, class X, class V, class W>
struct Curryable5Helper<R,F,X,auto_curry_type,auto_curry_type,V,W> {
  static inline R go( const F& f, const X& x, const auto_curry_type, const auto_curry_type, const V& v, const W& w)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 3 args for x v and w"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind4,V>,std::pair<Bind5,W> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_fourth,v),
                          std::make_pair(bind_fifth,w)));
   }
};

template <class R, class F, class Y, class Z, class V>
  struct Curryable5Helper<R,F,auto_curry_type,Y,Z,V,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type, const Y& y,const Z& z, const V& v,const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 3 args for y z and v"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V> >
		       (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z),
                          std::make_pair(bind_fourth,v)));
   }
};

template <class R, class F, class Y, class Z, class W>
  struct Curryable5Helper<R,F,auto_curry_type,Y,Z,auto_curry_type,W> {
    static inline R go( const F& f, const auto_curry_type, const Y& y,const Z& z,const auto_curry_type, const W& w)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 3 args for y z and w"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,W> >
		       (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z),
                          std::make_pair(bind_fifth,w)));
   }
};

template <class R, class F, class Y, class V, class W>
  struct Curryable5Helper<R,F,auto_curry_type,Y,auto_curry_type,V,W> {
    static inline R go( const F& f, const auto_curry_type, const Y& y,const auto_curry_type,const V& v, const W& w)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 3 args for y v and w"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind4,V>,std::pair<Bind5,W> >
		       (f,std::make_pair(bind_second,y),std::make_pair(bind_fourth,v),
                          std::make_pair(bind_fifth,w)));
   }
};

// this does (_,_,z,v,w) using a combination of binders.
 template <class R, class F, class Z, class V, class W>
   struct Curryable5Helper<R,F,AutoCurryType,AutoCurryType,Z,V,W> {
     static inline R go( const F& f, const AutoCurryType&, const AutoCurryType&, const Z& z,const V& v,const W& w)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 3 args for z v and w"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind3,Z>,std::pair<Bind4,V>,std::pair<Bind5,W> >
		       (f,std::make_pair(bind_third,z),std::make_pair(bind_fourth,v),
                          std::make_pair(bind_fifth,w)));
   }
};

template <class R, class F, class X,class Y, class Z, class V>
   struct Curryable5Helper<R,F,X,Y,Z,V,auto_curry_type>
{
    static inline R go( const F& f, const X& x, const Y& y,const Z& z, const V& v,const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 4 args for x y z and v"
               << std::endl;
#endif
    return makeFull1( binder1to4of5<F,X,Y,Z,V>(f,x,y,z,v) );
   }
};

template <class R, class F, class X,class Y, class Z, class W>
   struct Curryable5Helper<R,F,X,Y,Z,auto_curry_type,W>
{
     static inline R go( const F& f, const X& x, const Y& y,const Z& z, const auto_curry_type, const W& w)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 4 args for x y z and w"
               << std::endl;
#endif
      return makeFull1(binderWtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind5,W> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_second,y),std::make_pair(bind_third,z),
                          std::make_pair(bind_fifth,w)));
   }
};

template <class R, class F, class X, class Y, class V, class W>
   struct Curryable5Helper<R,F,X,Y,auto_curry_type,V,W>
{
     static inline R go( const F& f, const X& x, const Y& y,const auto_curry_type, const V& v, const W& w)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 4 args for x y v and w"
               << std::endl;
#endif
      return makeFull1(binderWtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V>,std::pair<Bind5,W> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_second,y),std::make_pair(bind_fourth,v),
                          std::make_pair(bind_fifth,w)));
   }
};

template <class R, class F, class X, class Z, class V, class W>
  struct Curryable5Helper<R,F,X,auto_curry_type,Z,V,W>
{
  static inline R go( const F& f, const X& x,const auto_curry_type, const Z& z,const V& v, const W& w)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 4 args for x z v and w"
               << std::endl;
#endif
      return makeFull1(binderWtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V>,std::pair<Bind5,W> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z),std::make_pair(bind_fourth,v),
                          std::make_pair(bind_fifth,w)));
   }
};

template <class R, class F, class Y, class Z, class V, class W>
  struct Curryable5Helper<R,F,auto_curry_type,Y,Z,V,W>
{
  static inline R go( const F& f, const auto_curry_type, const Y& y,const Z& z,const V& v, const W& w)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper called with 4 args for y z v and w"
               << std::endl;
#endif
      return makeFull1(binderWtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V>,std::pair<Bind5,W> >
		       (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z),std::make_pair(bind_fourth,v),
                          std::make_pair(bind_fifth,w)));
   }
};

 template <class R, class F, class X, class Y, class Z,class V>
struct Curryable5Helper2 {
  static inline R go( const F& f, const X& x, const Y& y, const Z& z,const V& v ) {
#ifdef FCPP_PRINT_HELPERS5
       std::cout << "Curryable5Helper2 called with 4 args" << std::endl;
#endif
    return makeFull1( binder1to4of5<F,X,Y,Z,V>(f,x,y,z,v) );
   }
};


template <class R, class F, class X>
struct Curryable5Helper2<R,F,X,auto_curry_type,auto_curry_type,auto_curry_type> {
   static inline R go( const F& f, const X& x, const auto_curry_type&, const auto_curry_type, const auto_curry_type& )
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 1 arg for x" << std::endl;
#endif
      return makeFull4(binder1of5<F,X>(f,x));
   }
   };

template <class R, class F, class Y>
  struct Curryable5Helper2<R,F,auto_curry_type,Y,auto_curry_type,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const Y& y,const auto_curry_type, const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 1 arg for y" << std::endl;
#endif
      return makeFull4(binder2of5<F,Y>(f,y));
   }
   };

template <class R, class F, class Z>
  struct Curryable5Helper2<R,F,auto_curry_type,auto_curry_type,Z,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type, const Z& z, const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "Curryable5Helper2 called with 1 arg for z" << std::endl;
#endif
      return makeFull4(binderAof5<F,Bind3,Z>(f,bind_third,z));
   }
   };


template <class R, class F, class V>
  struct Curryable5Helper2<R,F,auto_curry_type,auto_curry_type,auto_curry_type,V> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type, const auto_curry_type&, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 1 arg for v" << std::endl;
#endif
      return makeFull4(binderAof5<F,Bind4,V>(f,bind_fourth,v));
   }
   };

 template <class R, class F, class X, class Y>
struct Curryable5Helper2<R,F,X,Y,auto_curry_type,auto_curry_type> {
   static inline R go( const F& f, const X& x, const Y& y, const auto_curry_type, const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 2 args for x and y"
               << std::endl;
#endif
     return makeFull3(impl::binder1and2of5<F,X,Y>(f,x,y));
   }
};

 template <class R, class F, class X, class Z>
   struct Curryable5Helper2<R,F,X,auto_curry_type,Z,auto_curry_type> {
     static inline R go( const F& f, const X& x, const auto_curry_type, const Z& z,const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 2 args for x and z"
               << std::endl;
#endif
      return makeFull3(binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z)));
   }
};

 template <class R, class F, class X, class V>
   struct Curryable5Helper2<R,F,X,auto_curry_type,auto_curry_type,V> {
     static inline R go( const F& f, const X& x, const auto_curry_type, const auto_curry_type, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 2 args for x and v"
               << std::endl;
#endif
      return makeFull3(binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind4,V> >
		    (f,std::make_pair(bind_first,x),std::make_pair(bind_fourth,v)));
   }
};

 template <class R, class F, class Y, class Z>
   struct Curryable5Helper2<R,F,auto_curry_type,Y,Z,auto_curry_type> {
     static inline R go( const F& f,const auto_curry_type, const Y& y,const Z& z,const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 2 args for y and z"
               << std::endl;
#endif
     return makeFull3(binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> >
     		     (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z)));
   }
};

 template <class R, class F, class Y, class V>
   struct Curryable5Helper2<R,F,auto_curry_type,Y,auto_curry_type,V> {
     static inline R go( const F& f,const auto_curry_type, const Y& y,const auto_curry_type, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 2 args for y and v"
               << std::endl;
#endif
     return makeFull3(binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind4,V> >
     		       (f,std::make_pair(bind_second,y),std::make_pair(bind_fourth,v)));
   }
};


// this does (_,_,z,v) using a combination of binders.
 template <class R, class F, class Z, class V>
   struct Curryable5Helper2<R,F,AutoCurryType,AutoCurryType,Z,V> {
     static R go( const F& f, const AutoCurryType&, const AutoCurryType&, const Z& z,const V& v) {
#ifdef FCPP_PRINT_HELPERS5
       std::cout << "Curryable5Helper2 called with 2 args for z and v" << std::endl;
#endif
     return makeFull3(binderXandYof5<F,std::pair<Bind3,Z>,std::pair<Bind4,V> >
     		     (f,std::make_pair(bind_third,z),std::make_pair(bind_fourth,v)));
   }
};

 template <class R, class F, class X, class Y, class Z>
struct Curryable5Helper2<R,F,X,Y,Z,auto_curry_type> {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z, const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 3 args for x y and z"
               << std::endl;
#endif
     return makeFull2(impl::binder1to3of5<F,X,Y,Z>(f,x,y,z));
   }
};

template <class R, class F, class X, class Y, class V>
  struct Curryable5Helper2<R,F,X,Y,auto_curry_type,V> {
    static inline R go( const F& f, const X& x, const Y& y, const auto_curry_type, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 3 args for x y and v"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind2,Y>,std::pair<Bind4,V> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_second,y),
                          std::make_pair(bind_fourth,v)));
   }
};

template <class R, class F, class X, class Z, class V>
  struct Curryable5Helper2<R,F,X,auto_curry_type,Z,V> {
    static inline R go( const F& f, const X& x, const auto_curry_type, const Z& z, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 3 args for x z and v"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z>,std::pair<Bind4,V> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z),
                          std::make_pair(bind_fourth,v)));
   }
};

template <class R, class F, class Y, class Z, class V>
  struct Curryable5Helper2<R,F,auto_curry_type,Y,Z,V> {
    static inline R go( const F& f, const auto_curry_type, const Y& y,const Z& z, const V& v)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper2 called with 3 args for y z and v"
               << std::endl;
#endif
      return makeFull2(binderXtoZof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z>,std::pair<Bind4,V> >
		       (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z),
                          std::make_pair(bind_fourth,v)));
   }
};

template <class R, class F, class X, class Y, class Z>
struct Curryable5Helper3 {
   static inline R go( const F& f, const X& x, const Y& y, const Z& z) {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper3 called with 3 args" << std::endl;
#endif
     return makeFull2(impl::binder1to3of5<F,X,Y,Z>(f,x,y,z));
   }
};

template <class R, class F, class X>
struct Curryable5Helper3<R,F,X,auto_curry_type,auto_curry_type> {
   static inline R go( const F& f, const X& x, const auto_curry_type&, const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "Curryable5Helper3 called with 1 arg for x" << std::endl;
#endif
      return makeFull4(binder1of5<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable5Helper3<R,F,auto_curry_type,Y,auto_curry_type> {
    static inline R go( const F& f, const auto_curry_type&, const Y& y,const auto_curry_type&)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper3 called with 1 arg for y" << std::endl;
#endif
      return makeFull4(binder2of5<F,Y>(f,y));
   }
   };

template <class R, class F, class Z>
  struct Curryable5Helper3<R,F,auto_curry_type,auto_curry_type,Z> {
    static inline R go( const F& f, const auto_curry_type&, const auto_curry_type, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper3 called with 1 arg for z" << std::endl;
#endif
      return makeFull4(binderAof5<F,Bind3,Z>(f,bind_third,z));
   }
};

 template <class R, class F, class X, class Y>
struct Curryable5Helper3<R,F,X,Y,auto_curry_type> {
   static inline R go( const F& f, const X& x, const Y& y, const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper3 called with 2 args for x and y"
               << std::endl;
#endif
     return makeFull3(impl::binder1and2of5<F,X,Y>(f,x,y));
   }
};

 template <class R, class F, class X, class Z>
   struct Curryable5Helper3<R,F,X,auto_curry_type,Z> {
     static inline R go( const F& f, const X& x, const auto_curry_type, const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper3 called with 2 args for x and z" 
               << std::endl;
#endif
      return makeFull3(binderXandYof5<F,std::pair<Bind1,X>,std::pair<Bind3,Z> >
		       (f,std::make_pair(bind_first,x),std::make_pair(bind_third,z)));
   }
};

 template <class R, class F, class Y, class Z>
   struct Curryable5Helper3<R,F,auto_curry_type,Y,Z> {
     static inline R go( const F& f,const auto_curry_type, const Y& y,const Z& z)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper3 called with 2 args for y and z"
               << std::endl;
#endif
     return makeFull3(binderXandYof5<F,std::pair<Bind2,Y>,std::pair<Bind3,Z> >
     		     (f,std::make_pair(bind_second,y),std::make_pair(bind_third,z)));
     // return makeFull3(binder2of4<binder2of5<F,Y>,Z>(binder2of5<F,Y>(f,y),z));
   }
};

template <class R, class F, class X, class Y>
struct Curryable5Helper4 {
   static inline R go( const F& f, const X& x, const Y& y) {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper4 called with 2 args" << std::endl;
#endif
     return makeFull3(impl::binder1and2of5<F,X,Y>(f,x,y));
   }
};

template <class R, class F, class X>
struct Curryable5Helper4<R,F,X,auto_curry_type> {
   static inline R go( const F& f, const X& x, const auto_curry_type)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper4 called with 1 arg for x" << std::endl;
#endif
      return makeFull4(binder1of5<F,X>(f,x));
   }
};

template <class R, class F, class Y>
  struct Curryable5Helper4<R,F,auto_curry_type,Y> {
    static inline R go( const F& f, const auto_curry_type&, const Y& y)
   {
#ifdef FCPP_PRINT_HELPERS5
     std::cout << "Curryable5Helper4 called with 1 arg for y" << std::endl;
#endif
      return makeFull4(binder2of5<F,Y>(f,y));
   }
};

// Start to build this which has not been built before.
// This is incomplete - compare with Curryable3
// Use makeFull5.
template <class F>
class Curryable5 {
   const F f;
public:
   Curryable5( const F& ff ) : f(ff) {}

     template <class X, class Y=void, class Z=void, class V=void, class W=void>
   struct Sig
       : public FunType<typename F::template Sig<X,Y,Z,V,W>::Arg1Type,
     typename F::template Sig<X,Y,Z,V,W>::Arg2Type,
     typename F::template Sig<X,Y,Z,V,W>::Arg3Type,
     typename F::template Sig<X,Y,Z,V,W>::Arg4Type,
     typename F::template Sig<X,Y,Z,V,W>::Arg5Type,
     typename RT<F,X,Y,Z,V,W>::ResultType> {};

   template <class X, class Y, class Z, class V, class W>
     inline typename Sig<X,Y,Z,V,W>::ResultType 
     operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w ) const {
      // need partial specialization, so defer to a class helper
#ifdef FCPP_PRINT_HELPERS5
      std::cout << "Curryable5 operator() called with 5 args" << std::endl;
#endif
     return Curryable5Helper<typename Sig<X,Y,Z,V,W>::ResultType,F,X,Y,Z,V,W>::go(f,x,y,z,v,w);
   }
 
};


#endif
