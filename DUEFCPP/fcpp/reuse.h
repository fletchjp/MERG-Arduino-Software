//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_REUSE_DOT_H
#define FCPP_REUSE_DOT_H

#include "function.h"
// This was in operator.h. It is moved here
// so that it is defined after function.h is called.
#include "funify.h"

namespace fcpp {

// This is really a "list.h" thing, but Reusers borrow NIL to do some of
// their dirty work.
struct AUniqueTypeForNil {
   bool operator==( AUniqueTypeForNil ) const { return true; }
   bool operator< ( AUniqueTypeForNil ) const { return false; }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("NIL");
   }
#endif
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN AUniqueTypeForNil NIL;
FCPP_MAYBE_NAMESPACE_CLOSE

   namespace traits {
#ifndef FCPP_ARDUINO
    template <>
    class argument_traits <AUniqueTypeForNil> {
    public:
      static std::string value(const AUniqueTypeForNil &nil)
      {
	return std::string("NIL");
      }
    };
#endif

#ifdef FCPP_DEBUG

template <>
  class debug_traits<AUniqueTypeForNil>
      {
      public:
       static const bool has_name = true;
       static std::string name(const AUniqueTypeForNil &f) { return f.name(); }
      };

#endif
}

//////////////////////////////////////////////////////////////////////
// "Reuser"s are effectively special-purpose versions of curry() that
// enable recursive list functoids to reuse the thunk of the curried
// recursive call.  See 
//    http://www.cc.gatech.edu/~yannis/fc++/New/reusers.html
// for a more detailed description.
//////////////////////////////////////////////////////////////////////

// For efficiency, we mark parameters as either "Var"iant or "Inv"ariant.
struct Inv {};
struct Var {};

template <class V, class X> struct Maybe_Var_Inv;
template <class X>
struct Maybe_Var_Inv<Var,X> {
   static void remake( X& x, const X& val ) {
      x.~X();
      new (&x) X(val);
   }
   static X clone( const X& x ) { return X(x); }
};
template <class X>
struct Maybe_Var_Inv<Inv,X> {
   static void remake( X&, const X& ) {}
   static const X& clone( const X& x ) { return x; }
};

//////////////////////////////////////////////////////////////////////

template <class V1, class F> 
struct Reuser0;

template <class V1, class F, class R>
struct Thunk0 : public Fun0Impl<R> {
   mutable F f;
   Thunk0( const F& ff ) : f(ff) {}
   void init( const F& ff ) const {
      Maybe_Var_Inv<V1,F>::remake( f, ff );
   }
   R operator()() const {
      return Maybe_Var_Inv<V1,F>::clone(f)( Reuser0<V1,F>(this) );
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Thunk0::",f,"Thunk0");
   }
#endif
};

#ifdef FCPP_DEBUG
   namespace traits {

     template <class V1, class F, class R>
       class debug_traits<Thunk0<V1,F,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Thunk0<V1,F,R> &f)
           { return f.name(); }
      };
   }
#endif

template <class V1, class F>
struct Reuser0 {
   typedef typename RT<F>::ResultType R;
   typedef Thunk0<V1,F,R> M;
   IRef<const M> ref;
   Reuser0(AUniqueTypeForNil) {}
   Reuser0(const M* m) : ref(m) {}
   Fun0<R> operator()( const F& f ) {
      if( !ref )   ref = IRef<const M>( new M(f) );
      else         ref->init(f);
      return Fun0<R>( 1, ref );
   }
   void iter( const F& f ) {
      if( ref )    ref->init(f);
   }
};

//////////////////////////////////////////////////////////////////////

template <class V1, class V2, class F, class X> 
struct Reuser1;

template <class V1, class V2, class F, class X, class R>
struct Thunk1 : public Fun0Impl<R> {
   mutable F f;
   mutable X x;
   Thunk1( const F& ff, const X& xx ) : f(ff), x(xx) {}
   void init( const F& ff, const X& xx ) const {
      Maybe_Var_Inv<V1,F>::remake( f, ff );
      Maybe_Var_Inv<V2,X>::remake( x, xx );
   }
   R operator()() const {
      return Maybe_Var_Inv<V1,F>::clone(f)(
         Maybe_Var_Inv<V2,X>::clone(x),
         Reuser1<V1,V2,F,X>(this) );
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     std::string temp;
     if (debug_traits_has_name<F>()) {
        temp += std::string("Thunk1::")
             + debug_traits_name(f);
        if (debug_traits_has_name<X>()) {
  	   temp += std::string("::")
                + debug_traits_name(x);
	} else {
  	   temp += std::string("( ")
	        + argument_traits_value(x)
                + std::string(" )");
	}
     } else {
        temp += std::string("Thunk1::(?)");
     }
     return temp;
   }
#endif
};

#ifdef FCPP_DEBUG
   namespace traits {

     template <class V1, class V2, class F, class X, class R>
       class debug_traits<Thunk1<V1,V2,F,X,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Thunk1<V1,V2,F,X,R> &f)
           { return f.name(); }
      };
   }
#endif

template <class V1, class V2, class F, class X>
struct Reuser1 {
   typedef typename RT<F,X>::ResultType R;
   typedef Thunk1<V1,V2,F,X,R> M;
   IRef<const M> ref;
   Reuser1(AUniqueTypeForNil) {}
   Reuser1(const M* m) : ref(m) {}
   Fun0<R> operator()( const F& f, const X& x ) {
      if( !ref )   ref = IRef<const M>( new M(f,x) );
      else         ref->init(f,x);
      return Fun0<R>( 1, ref );
   }
   void iter( const F& f, const X& x ) {
      if( ref )    ref->init(f,x);
   }
};

//////////////////////////////////////////////////////////////////////

template <class V1, class V2, class V3, class F, class X, class Y>
struct Reuser2;

template <class V1, class V2, class V3, class F, class X, class Y, class R>
struct Thunk2 : public Fun0Impl<R> {
   mutable F f;
   mutable X x;
   mutable Y y;
   Thunk2( const F& ff, const X& xx, const Y& yy ) : f(ff), x(xx), y(yy) {}
   void init( const F& ff, const X& xx, const Y& yy ) const {
      Maybe_Var_Inv<V1,F>::remake( f, ff );
      Maybe_Var_Inv<V2,X>::remake( x, xx );
      Maybe_Var_Inv<V3,Y>::remake( y, yy );
   }
   R operator()() const {
      return Maybe_Var_Inv<V1,F>::clone(f)(
         Maybe_Var_Inv<V2,X>::clone(x),
         Maybe_Var_Inv<V3,Y>::clone(y),
         Reuser2<V1,V2,V3,F,X,Y>(this) );
   }
#ifdef FCPP_DEBUG
   // This deals with the different cases where the first argument (X)
   // or the second argument (Y) is a functoid with a name.
   std::string name() const {
     std::string temp;
     if (debug_traits_has_name<F>() ) {
        temp += std::string("Thunk2::")
             + debug_traits_name(f)
 	     + std::string("( ");
        temp += debug_name_or_value(x);
	temp += std::string(" , ");
        temp += debug_name_or_value(y);
        temp += std::string(" )");
	//std::cout << "x : " << x << std::endl;
     } else {
        temp += std::string("Thunk2");
     }
     return temp;
   }
#endif
};

#ifdef FCPP_DEBUG
   namespace traits {

     template <class V1, class V2, class V3, class F, class X, class Y, class R>
       class debug_traits<Thunk2<V1,V2,V3,F,X,Y,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Thunk2<V1,V2,V3,F,X,Y,R> &f)
           { return f.name(); }
      };
   }
#endif

template <class V1, class V2, class V3, class F, class X, class Y>
struct Reuser2 {
   typedef typename RT<F,X,Y>::ResultType R;
   typedef Thunk2<V1,V2,V3,F,X,Y,R> M;
   IRef<const M> ref;
   Reuser2(AUniqueTypeForNil) {}
   Reuser2(const M* m) : ref(m) {}
   Fun0<R> operator()( const F& f, const X& x, const Y& y ) {
      if( !ref )   ref = IRef<const M>( new M(f,x,y) );
      else         ref->init(f,x,y);
      return Fun0<R>( 1, ref );
   }
   void iter( const F& f, const X& x, const Y& y ) {
      if( ref )    ref->init(f,x,y);
   }
};

//////////////////////////////////////////////////////////////////////

template <class V1, class V2, class V3, class V4,
          class F, class X, class Y, class Z>
struct Reuser3;

template <class V1, class V2, class V3, class V4,
          class F, class X, class Y, class Z, class R>
struct Thunk3 : public Fun0Impl<R> {
   mutable F f;
   mutable X x;
   mutable Y y;
   mutable Z z;
   Thunk3( const F& ff, const X& xx, const Y& yy, const Z& zz )
      : f(ff), x(xx), y(yy), z(zz) {}
   void init( const F& ff, const X& xx, const Y& yy, const Z& zz ) const {
      Maybe_Var_Inv<V1,F>::remake( f, ff );
      Maybe_Var_Inv<V2,X>::remake( x, xx );
      Maybe_Var_Inv<V3,Y>::remake( y, yy );
      Maybe_Var_Inv<V4,Z>::remake( z, zz );
   }
   R operator()() const {
      return Maybe_Var_Inv<V1,F>::clone(f)(
         Maybe_Var_Inv<V2,X>::clone(x),
         Maybe_Var_Inv<V3,Y>::clone(y),
         Maybe_Var_Inv<V4,Z>::clone(z),
         Reuser3<V1,V2,V3,V4,F,X,Y,Z>(this) );
   }
#ifdef FCPP_DEBUG
   // This deals with the different cases where the first argument (X)
   // or the second argument (Y) is a functoid with a name.
   // It has been extended for the third argument (Z).
   std::string name() const {
     std::string temp;
     if (debug_traits_has_name<F>()) {
        temp += std::string("Thunk3::")
             + debug_traits_name(f)
 	     + std::string("( ");
        temp += debug_name_or_value(x);
	temp += std::string(" , ");
        temp += debug_name_or_value(y);
	temp += std::string(" , ");
        temp += debug_name_or_value(z);
        temp += std::string(" )");
     } else {
        temp += std::string("Thunk3");
     }
     return temp;
   }
#endif
};

template <class V1, class V2, class V3, class V4,
          class F, class X, class Y, class Z>
struct Reuser3 {
   typedef typename RT<F,X,Y,Z>::ResultType R;
   typedef Thunk3<V1,V2,V3,V4,F,X,Y,Z,R> M;
   IRef<const M> ref;
   Reuser3(AUniqueTypeForNil) {}
   Reuser3(const M* m) : ref(m) {}
   Fun0<R> operator()( const F& f, const X& x, const Y& y, const Z& z ) {
      if( !ref )   ref = IRef<const M>( new M(f,x,y,z) );
      else         ref->init(f,x,y,z);
      return Fun0<R>( 1, ref );
   }
   void iter( const F& f, const X& x, const Y& y, const Z& z ) {
      if( ref )    ref->init(f,x,y,z);
   }
};

#ifdef FCPP_DEBUG
   namespace traits {

     template <class V1, class V2, class V3, class V4, class F,
       class X, class Y, class Z, class R>
       class debug_traits<Thunk3<V1,V2,V3,V4,F,X,Y,Z,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Thunk3<V1,V2,V3,V4,F,X,Y,Z,R> &f)
           { return f.name(); }
      };
   }
#endif

#ifdef FCPP152
   // Reuser4
template <class V1, class V2, class V3, class V4, class V5,
          class F, class W, class X, class Y, class Z>
struct Reuser4;

template <class V1, class V2, class V3, class V4, class V5,
          class F, class W, class X, class Y, class Z, class R>
struct Thunk4 : public Fun0Impl<R> {
   mutable F f;
   mutable W w;
   mutable X x;
   mutable Y y;
   mutable Z z;
   Thunk4( const F& ff, const W& ww, const X& xx, const Y& yy, const Z& zz )
     : f(ff), w(ww), x(xx), y(yy), z(zz) {}
   void init( const F& ff, const W& ww, const X& xx, const Y& yy, const Z& zz ) const {
      Maybe_Var_Inv<V1,F>::remake( f, ff );
      Maybe_Var_Inv<V2,W>::remake( w, ww );
      Maybe_Var_Inv<V3,X>::remake( x, xx );
      Maybe_Var_Inv<V4,Y>::remake( y, yy );
      Maybe_Var_Inv<V5,Z>::remake( z, zz );
   }
   R operator()() const {
      return Maybe_Var_Inv<V1,F>::clone(f)(
         Maybe_Var_Inv<V2,W>::clone(w),
         Maybe_Var_Inv<V3,X>::clone(x),
         Maybe_Var_Inv<V4,Y>::clone(y),
         Maybe_Var_Inv<V5,Z>::clone(z),
         Reuser4<V1,V2,V3,V4,V5,F,W,X,Y,Z>(this) );
   }
#ifdef FCPP_DEBUG
   // This deals with the different cases where the first argument (W)
   // or the second argument (X) is a functoid with a name.
   // It has been extended for the third and fourth arguments (Y and Z).
   std::string name() const {
     std::string temp;
     if (debug_traits_has_name<F>()) {
        temp += std::string("Thunk4::")
             + debug_traits_name(f)
 	     + std::string("( ");
        temp += debug_name_or_value(w);
	temp += std::string(" , ");
        temp += debug_name_or_value(x);
	temp += std::string(" , ");
        temp += debug_name_or_value(y);
	temp += std::string(" , ");
        temp += debug_name_or_value(z);
        temp += std::string(" )");
     } else {
        temp += std::string("Thunk4");
     }
      return temp;
   }
#endif
};

template <class V1, class V2, class V3, class V4, class V5,
          class F, class W, class X, class Y, class Z>
struct Reuser4 {
  typedef typename RT<F,W,X,Y,Z>::ResultType R;
  typedef Thunk4<V1,V2,V3,V4,V5,F,W,X,Y,Z,R> M;
   IRef<const M> ref;
   Reuser4(AUniqueTypeForNil) {}
   Reuser4(const M* m) : ref(m) {}
  Fun0<R> operator()( const F& f, const W& w, const X& x, const Y& y, const Z& z ) {
    if( !ref )   ref = IRef<const M>( new M(f,w,x,y,z) );
    else         ref->init(f,w,x,y,z);
      return Fun0<R>( 1, ref );
   }
  void iter( const F& f, const W& w, const X& x, const Y& y, const Z& z ) {
    if( ref )    ref->init(f,w,x,y,z);
   }
};

#ifdef FCPP_DEBUG
   namespace traits {

     template <class V1, class V2, class V3, class V4, class V5, class F,
       class W, class X, class Y, class Z, class R>
       class debug_traits<Thunk4<V1,V2,V3,V4,V5,F,W,X,Y,Z,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Thunk4<V1,V2,V3,V4,V5,F,W,X,Y,Z,R> &f)
           { return f.name(); }
      };
   }
#endif

   //Future Reuser5, etc.
  template <class V1, class V2, class V3, class V4, class V5, class  V6,
  class F, class V, class W, class X, class Y, class Z>
struct Reuser5;

template <class V1, class V2, class V3, class V4, class V5, class  V6,
  class F, class V,class W, class X, class Y, class Z, class R>
struct Thunk5 : public Fun0Impl<R> {
   mutable F f;
   mutable V v;
   mutable W w;
   mutable X x;
   mutable Y y;
   mutable Z z;
 Thunk5( const F& ff, const V& vv, const W& ww, const X& xx, const Y& yy, const Z& zz )
   : f(ff), v(vv), w(ww), x(xx), y(yy), z(zz) {}
   void init( const F& ff, const V& vv,const W& ww, const X& xx, const Y& yy, const Z& zz ) const {
      Maybe_Var_Inv<V1,F>::remake( f, ff );
      Maybe_Var_Inv<V2,V>::remake( v, vv );
      Maybe_Var_Inv<V3,W>::remake( w, ww );
      Maybe_Var_Inv<V4,X>::remake( x, xx );
      Maybe_Var_Inv<V5,Y>::remake( y, yy );
      Maybe_Var_Inv<V6,Z>::remake( z, zz );
   }
   R operator()() const {
      return Maybe_Var_Inv<V1,F>::clone(f)(
         Maybe_Var_Inv<V2,V>::clone(v),
         Maybe_Var_Inv<V3,W>::clone(w),
         Maybe_Var_Inv<V4,X>::clone(x),
         Maybe_Var_Inv<V5,Y>::clone(y),
         Maybe_Var_Inv<V6,Z>::clone(z),
         Reuser5<V1,V2,V3,V4,V5,V6,F,V,W,X,Y,Z>(this) );
   }
#ifdef FCPP_DEBUG
   // This deals with the different cases where the first argument (V)
   // or the second argument (W) is a functoid with a name.
   // It has been extended for the third, fourth and fifth arguments
   // (X, Y and Z).
   std::string name() const {
     std::string temp;
     if (debug_traits_has_name<F>()) {
        temp += std::string("Thunk5::")
             + debug_traits_name(f)
 	     + std::string("( ");
        temp += debug_name_or_value(v);
	temp += std::string(" , ");
        temp += debug_name_or_value(w);
	temp += std::string(" , ");
        temp += debug_name_or_value(x);
	temp += std::string(" , ");
        temp += debug_name_or_value(y);
	temp += std::string(" , ");
        temp += debug_name_or_value(z);
        temp += std::string(" )");
     } else {
        temp += std::string("Thunk5");
     }
     return temp;
   }
#endif
};

 template <class V1, class V2, class V3, class V4, class V5,class V6,
           class F, class V, class W, class X, class Y, class Z>
struct Reuser5 {
  typedef typename RT<F,V,W,X,Y,Z>::ResultType R;
  typedef Thunk5<V1,V2,V3,V4,V5,V6,F,V,W,X,Y,Z,R> M;
   IRef<const M> ref;
   Reuser5(AUniqueTypeForNil) {}
   Reuser5(const M* m) : ref(m) {}
  Fun0<R> operator()( const F& f, const V& v, const W& w, const X& x, const Y& y, const Z& z ) {
    if( !ref )   ref = IRef<const M>( new M(f,v,w,x,y,z) );
    else         ref->init(f,v,w,x,y,z);
      return Fun0<R>( 1, ref );
   }
  void iter( const F& f, const V& v, const W& w, const X& x, const Y& y, const Z& z ) {
    if( ref )    ref->init(f,v,w,x,y,z);
   }
};


#ifdef FCPP_DEBUG
   namespace traits {

     template <class V1, class V2, class V3, class V4, class V5, class V6,
       class F, class V, class W, class X, class Y, class Z, class R>
       class debug_traits<Thunk5<V1,V2,V3,V4,V5,V6,F,V,W,X,Y,Z,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Thunk5<V1,V2,V3,V4,V5,V6,F,V,W,X,Y,Z,R> &f)
           { return f.name(); }
      };
   }
#endif

#endif

}  // namespace fcpp

#endif
