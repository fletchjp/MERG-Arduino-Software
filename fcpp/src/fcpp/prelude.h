//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_PRELUDE_DOT_H
#define FCPP_PRELUDE_DOT_H

//////////////////////////////////////////////////////////////////////
// The modifications under the definition FCPP_CONCEPTS
// are experimental and concerned with getting FC++ to work
// with the experimental concepts compiler. 
// All now restored now that full.h has been modified
// to use concepts to determine the return types.
// JPF  May 2007.
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Note that this header file includes all the other FC++ header files,
// so including this one (prelude.h) is sufficient to suck in the whole
// library. 
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Here we define a bunch of functions from the Haskell Standard
// Prelude (HSP).  For documentation of their behaviors, see 
//    http://haskell.org/onlinereport/standard-prelude.html
//
// A number of the functions are not from HSP, but seemed natural/useful.
//
// The implementations #ifdef-ed out (FCPP_SIMPLE_PRELUDE) are mostly
// just here to look at, as they are often more readable than their
// optimized counterparts.
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// FC++ 1.5.2 - Modifications to increase functoid arguments to 4.
// These mods will be commented with FC++.1.5.2
// and guarded with 
// #ifdef FCPP152
// John Fletcher (J.P.Fletcher@aston.ac.uk) February 2006
//
// Some things (but not all) have been taken beyond 4.
//
// I have carried out a lot of experiments on different parts of FC++.
// Some of these have been driven by particular needs for some
// functoids with a lot of arguments. I have also begun to include
// things which are in Haskell but not in the original FC++.
// One example of this is Either which is mentioned in
// Real World Haskell p.452
//
// John Fletcher July 2011
//////////////////////////////////////////////////////////////////////
//
// I want to use FC++ with the NVIDIA compiler for CUDA
// The initial experiments show that prelude.h does not
// compile and I am trying to find out why this is.
// I have found the solution to one problem in smart.h
// and modifications are guarded with FCPP_CUDA_COMPATIBLE
//
// John Fletcher   February 2012
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
// Work is in progress to catalogue and complete the debug
// information brought into action when FCPP_DEBUG is defined.
// This has grown up on a piecemeal basis for many years.
// I have started a file called debug_notes.txt which documents
// what has already been done and also will provide a complete
// index to all the functoids when it is completed.
//
// I have now also begun some work on refactoring of the debug
// code using extra functions defined in debug_traits.h
// and argument_traits.h. This is resulting in code which is much easier
// to read and understand. I have applied this in compose and related
// functoids at the moment. I have also applied this to reuse.h
//
// I have now applied the ideas to this file and also moved all argument_traits and
// debug_traits definitions from this file to a new file prelude_debug.h
//
// As a result this file is a lot smaller.
//
// I have added some new debug information at the same time.
// There is still not complete FCPP_DEBUG information. In the past this was added
// as it was needed for example cases.
//
// John Fletcher   December 2015 and October 2016
//////////////////////////////////////////////////////////////////////

// To remove iostream which is not supported.
#define FCPP_ARDUINO

#ifdef FCPP_USE_BOOST_VOID
#include <boost/type_traits/is_void.hpp>
#endif
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_void.hpp>
#endif
#ifdef FCPP_USE_BOOST_TTI
#include <boost/tti/tti.hpp>
#include <boost/mpl/placeholders.hpp>
#if BOOST_PP_VARIADICS > 0
#define FCPP_USE_BOOST_TTI_VARIADICS
// Switch to variadic definition of this macro.
BOOST_TTI_HAS_TEMPLATE(Sig)
// and this is how to do it for a variadic Sig.
BOOST_TTI_TRAIT_HAS_TEMPLATE(has_variadic_Sig,Sig,typename...)
#else
BOOST_TTI_HAS_TEMPLATE(Sig,BOOST_PP_NIL)
#endif
#endif

// Make these includes once here and take them out everywhere else.
#ifndef FCPP_ARDUINO
#include "argument_traits.h"
#ifdef FCPP_DEBUG
#include "debug_traits.h"
#endif
#endif

#include "list.h"
//#include "map.h" moved to the end.
// New file for Map and MultiMap data structures.

namespace fcpp {

namespace impl {
struct XId {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Id");
   }
#endif
   template <class T> struct Sig : public FunType<T,T> {};

   template <class T>
   T operator()( const T& x ) const {
      return x;
   }
};
}
typedef Full1<impl::XId> Id;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Id id, value;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {

#ifdef FCPP_DEBUG

      // The complexity is to cover the case where
      // both, either, or neither of F and G have
      // the name defined.
      // This one is used for Compose and invCompose
    template <class F,class G>
      std::string name_F_G(const std::string &name_string,
                           const F& f, const G& g)
    {
      std::string temp = name_string + std::string("::(");
      temp += debug_name_and_string_or_string(f,",","f,");
      temp += debug_name_and_string_or_string(g,")","g)");
      return temp;
    }

#endif

template <class F, class G>
class XCompose0Helper : public CFunType<
typename F::template Sig<typename RT<G>::ResultType>::ResultType> {
   F f;
   G g;
public:
   XCompose0Helper( const F& a, const G& b ) : f(a), g(b) {}

   typename F::template Sig<typename RT<G>::ResultType>::ResultType
   operator()() const {
      return f( g() );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      // The complexity is to cover the case where
      // both, either, or neither of F and G have
      // the name defined.
      return name_F_G("compose0Helper",f,g);
    }
#endif
};
template <class F, class G>
class XCompose1Helper {
   F f;
   G g;
public:
   XCompose1Helper( const F& a, const G& b ) : f(a), g(b) {}
#ifndef FCPP_CONCEPTS
   template <class X> struct Sig : public FunType<
      typename RT<G,X>::Arg1Type,
      typename RT<F,typename RT<G,X>::ResultType>::ResultType> {};
#endif
   template <class X>
#ifdef FCPP_CONCEPTS
   inline typename ConstCallable1<F,typename ConstCallable1<G,X>::result_type>::result_type
#else
   typename Sig<X>::ResultType 
#endif
      operator()( const X& x ) const {
      return f( g(x) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      // The complexity is to cover the case where
      // both, either, or neither of F and G have
      // the name defined.
      return name_F_G("compose1Helper",f,g);
    }
#endif
};
template <class F, class G>
class XCompose2Helper {
   F f;
   G g;
public:
   XCompose2Helper( const F& a, const G& b ) : f(a), g(b) {}
   template <class X, class Y> struct Sig : public FunType<
      typename RT<G,X,Y>::Arg1Type,
      typename RT<G,X,Y>::Arg2Type,
      typename RT<F,typename RT<G,X,Y>::ResultType>::ResultType> {};
   template <class X, class Y>
   typename Sig<X,Y>::ResultType operator()( const X& x, const Y& y ) const {
      return f( g(x,y) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      // The complexity is to cover the case where
      // both, either, or neither of F and G have
      // the name defined.
      return name_F_G("compose2Helper",f,g);
    }
#endif
};
template <class F, class G>
class XCompose3Helper {
   F f;
   G g;
public:
   XCompose3Helper( const F& a, const G& b ) : f(a), g(b) {}
   template <class X, class Y, class Z> struct Sig : public FunType<
      typename RT<G,X,Y,Z>::Arg1Type,
      typename RT<G,X,Y,Z>::Arg2Type,
      typename RT<G,X,Y,Z>::Arg3Type,
      typename RT<F,typename RT<G,X,Y,Z>::ResultType>::ResultType> {};
   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType 
   operator()( const X& x, const Y& y, const Z& z ) const {
      return f( g(x,y,z) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      // The complexity is to cover the case where
      // both, either, or neither of F and G have
      // the name defined.
      return name_F_G("compose3Helper",f,g);
    }
#endif
};
// FC++.1.5.2
#ifdef FCPP152
template <class F, class G>
class XCompose4Helper {
   F f;
   G g;
public:
   XCompose4Helper( const F& a, const G& b ) : f(a), g(b) {}
   template <class X, class Y, class Z, class W> struct Sig : public FunType<
      typename RT<G,X,Y,Z,W>::Arg1Type,
      typename RT<G,X,Y,Z,W>::Arg2Type,
      typename RT<G,X,Y,Z,W>::Arg3Type,
      typename RT<G,X,Y,Z,W>::Arg4Type,
      typename RT<F,typename RT<G,X,Y,Z,W>::ResultType>::ResultType> {};
   template <class X, class Y, class Z, class W>
   typename Sig<X,Y,Z,W>::ResultType 
   operator()( const X& x, const Y& y, const Z& z, const W& w ) const {
      return f( g(x,y,z,w) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      // The complexity is to cover the case where
      // both, either, or neither of F and G have
      // the name defined.
      return name_F_G("compose4Helper",f,g);
    }
#endif
};
#endif
//
// Compose is Haskell's operator (.)
// compose(f,g)()        = f( g() )
// compose(f,g)(x)       = f( g(x) )
// compose(f,g)(x,y)     = f( g(x,y) )
// compose(f,g)(x,y,z)   = f( g(x,y,z) )
// compose(f,g)(x,y,z,w) = f( g(x,y,z,w) )
//
// Synonym: dot
//
class XCompose {
 protected: // Needed for variadic FC++
   template <int i, class F, class G> struct Helper;
   template <class F, class G>
   struct Helper<0,F,G> {
      typedef Full0<XCompose0Helper<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull0(XCompose0Helper<F,G>(f,g)); }
   };
   template <class F, class G>
   struct Helper<1,F,G> {
      typedef Full1<XCompose1Helper<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull1(XCompose1Helper<F,G>(f,g)); }
   };
   template <class F, class G>
   struct Helper<2,F,G> {
      typedef Full2<XCompose2Helper<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull2(XCompose2Helper<F,G>(f,g)); }
   };
   template <class F, class G>
   struct Helper<3,F,G> {
      typedef Full3<XCompose3Helper<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull3(XCompose3Helper<F,G>(f,g)); }
   };
// FC++.1.5.2
#ifdef FCPP152
   template <class F, class G>
   struct Helper<4,F,G> {
      typedef Full4<XCompose4Helper<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull4(XCompose4Helper<F,G>(f,g)); }
   };
#endif
public:
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("compose");
    }
#endif
#ifndef FCPP_CONCEPTS
   template <class F, class G> struct Sig : public
   FunType<F,G,typename Helper<FunctoidTraits<G>::max_args,F,G>::Result> {};
#endif

   template <class F, class G>
#ifdef FCPP_CONCEPTS
   inline typename Helper<FunctoidTraits<G>::max_args,F,G>::Result
#else
   typename Sig<F,G>::ResultType
#endif
   operator()( const F& f, const G& g ) const 
   {
#ifdef FCPP_PRINT_HELPERS
#ifdef FCPP_DEBUG
     // This is a different way to use the name() members.
     std::cout << "compose(" ;
     if (debug_traits_has_name<F>() ) {
       std::cout << debug_traits_name(f);
     } else {
       std::cout << "f";
     }
     std::cout << ",";
     if (debug_traits_has_name<G>() ) {
       std::cout << debug_traits_name(g);
       //std::cout << g.name();
     } else {
       std::cout << "g";
     }
     std::cout << ") called." << std::endl;
#else
      std::cout << "compose called" << std::endl;
#endif
#endif
      return Helper<FunctoidTraits<G>::max_args,F,G>::go( f, g );
   }
};

template <class F, class G>
class XInvCompose0Helper : public CFunType<
typename G::template Sig<typename RT<F>::ResultType>::ResultType> {
   F f;
   G g;
public:
   XInvCompose0Helper( const F& a, const G& b ) : f(a), g(b) {}

   typename G::template Sig<typename RT<F>::ResultType>::ResultType
   operator()() const {
      return g( f() );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      return name_F_G("invCompose0Helper",f,g);
    }
#endif
};

template <class F, class G>
class XInvCompose1Helper {
   F f;
   G g;
public:
   XInvCompose1Helper( const F& a, const G& b ) : f(a), g(b) {}
#ifndef FCPP_CONCEPTS
   template <class X> struct Sig : public FunType<
      typename RT<F,X>::Arg1Type,
      typename RT<G,typename RT<F,X>::ResultType>::ResultType> {};
#endif
   template <class X>
#ifdef FCPP_CONCEPTS
   inline typename ConstCallable1<G,typename ConstCallable1<F,X>::result_type>::result_type
#else
   typename Sig<X>::ResultType
#endif
      operator()( const X& x ) const {
      return g( f(x) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      return name_F_G("invCompose1Helper",f,g);
    }
#endif
};

template <class F, class G>
class XInvCompose2Helper {
   F f;
   G g;
public:
   XInvCompose2Helper( const F& a, const G& b ) : f(a), g(b) {}
   template <class X, class Y> struct Sig : public FunType<
      typename RT<F,X,Y>::Arg1Type,
      typename RT<F,X,Y>::Arg2Type,
      typename RT<G,typename RT<F,X,Y>::ResultType>::ResultType> {};
   template <class X, class Y>
   typename Sig<X,Y>::ResultType operator()( const X& x, const Y& y ) const {
      return g( f(x,y) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      // The complexity is to cover the case where
      // both, either, or neither of G and F have
      // the name defined.
      return name_F_G("invCompose2Helper",f,g);
    }
#endif
};

template <class F, class G>
class XInvCompose3Helper {
   F f;
   G g;
public:
   XInvCompose3Helper( const F& a, const G& b ) : f(a), g(b) {}
   template <class X, class Y, class Z> struct Sig : public FunType<
      typename RT<F,X,Y,Z>::Arg1Type,
      typename RT<F,X,Y,Z>::Arg2Type,
      typename RT<F,X,Y,Z>::Arg3Type,
      typename RT<G,typename RT<F,X,Y,Z>::ResultType>::ResultType> {};
   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType 
   operator()( const X& x, const Y& y, const Z& z ) const {
      return g( f(x,y,z) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      return name_F_G("invCompose3Helper",f,g);
    }
#endif
};

// Attempt to create inverse composition as suggested on
// http://c2.com/cgi/wiki?FunctionalComposition
//
// f >>> g = \x -> g (f x) -- in Haskell's Control.Category
//
// so far with 0, 1, 2 or 3 parameters.
// invcompose(f,g)()      = g( f() )
// invcompose(f,g)(x)     = g( f(x) )
// invcompose(f,g)(x,y)   = g( f(x,y) )
// invcompose(f,g)(x,y,z) = g( f(x,y,z) )
//
// Synonyms: invdot, dash
//
class XInvCompose {
   template <int i, class F, class G> struct Helper;
   template <class F, class G> 
   struct Helper<0,F,G> {
      typedef Full0<XInvCompose0Helper<F,G> > Result;
      static Result go( const F& f, const G& g ) 
      { return makeFull0(XInvCompose0Helper<F,G>(f,g)); }
   };
   template <class F, class G> 
   struct Helper<1,F,G> {
      typedef Full1<XInvCompose1Helper<F,G> > Result;
      static Result go( const F& f, const G& g ) 
      { return makeFull1(XInvCompose1Helper<F,G>(f,g)); }
   };
   template <class F, class G> 
   struct Helper<2,F,G> {
      typedef Full2<XInvCompose2Helper<F,G> > Result;
      static Result go( const F& f, const G& g ) 
      { return makeFull2(XInvCompose2Helper<F,G>(f,g)); }
   };
   template <class F, class G> 
   struct Helper<3,F,G> {
      typedef Full3<XInvCompose3Helper<F,G> > Result;
      static Result go( const F& f, const G& g ) 
      { return makeFull3(XInvCompose3Helper<F,G>(f,g)); }
   };

 public:
#ifndef FCPP_CONCEPTS
   template <class F, class G> struct Sig : public
   FunType<F,G,typename Helper<FunctoidTraits<F>::max_args,F,G>::Result> {};
#endif
   template <class F, class G>
#ifdef FCPP_CONCEPTS
   inline typename Helper<FunctoidTraits<F>::max_args,F,G>::Result
#else
   typename Sig<F,G>::ResultType
#endif
   operator()( const F& f, const G& g ) const 
   {
      return Helper<FunctoidTraits<F>::max_args,F,G>::go( f, g );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("InvCompose::");
    }
#endif
};
}
typedef Full2<impl::XCompose> Compose;
typedef Full2<impl::XInvCompose> InvCompose;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Compose compose, dot; // fmap;
FCPP_MAYBE_EXTERN InvCompose invcompose, invdot, dash;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// Now we can create "of", so that
//    f ^of^ g
// means
//    compose( funify(f), funify(g) )
//////////////////////////////////////////////////////////////////////

namespace impl {
struct XOf {
   template <class F, class G> struct Sig : public FunType<F,G,
      typename RT<Compose,typename RT<Funify,F>::ResultType,
      typename RT<Funify,G>::ResultType>::ResultType> {};
   template <class F, class G>
   typename Sig<F,G>::ResultType
   operator()( const F& f, const G& g ) const {
      return compose( funify(f), funify(g) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Of::");
    }
#endif

};

struct XInvOf {
   template <class F, class G> struct Sig : public FunType<F,G,
      typename RT<InvCompose,typename RT<Funify,F>::ResultType,
      typename RT<Funify,G>::ResultType>::ResultType> {};
   template <class F, class G>
   typename Sig<F,G>::ResultType
   operator()( const F& f, const G& g ) const {
      return invcompose( funify(f), funify(g) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("InvOf::");
    }
#endif
};

}
typedef Full2<impl::XOf> Of;
typedef Full2<impl::XInvOf> InvOf;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Of of;
FCPP_MAYBE_EXTERN InvOf invof, then;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {

#ifdef FCPP_DEBUG

      // The complexity is to cover the case where
      // all or some of F G and H have the name defined.
    template <class F, class G, class H>
    std::string name_F_G_H(const std::string &name_string,
                           const F& f, const G& g, const H& h)
    {
      std::string temp = name_string + std::string("::(");
      temp += debug_name_and_string_or_string(f,",","f,");
      temp += debug_name_and_string_or_string(g,",","g,");
      temp += debug_name_and_string_or_string(h,")","h)");
      return temp;
    }

    template <class F, class G, class H,class I>
    std::string name_F_G_H_I(const std::string &name_string,
                             const F& f, const G& g, const H& h, const I &i)
    {
      std::string temp = name_string + std::string("::(");
      temp += debug_name_and_string_or_string(f,",","f,");
      temp += debug_name_and_string_or_string(g,",","g,");
      temp += debug_name_and_string_or_string(h,",","h,");
      temp += debug_name_and_string_or_string(i,")","i)");
      return temp;
    }

    template <class F, class G, class H,class I,class J>
    std::string name_F_G_H_I_J(const std::string &name_string,
         const F& f, const G& g, const H& h, const I &i, const J& j)
    {
      std::string temp = name_string + std::string("::(");
      temp += debug_name_and_string_or_string(f,",","f,");
      temp += debug_name_and_string_or_string(g,",","g,");
      temp += debug_name_and_string_or_string(h,",","h,");
      temp += debug_name_and_string_or_string(i,",","i,");
      temp += debug_name_and_string_or_string(j,")","j)");
      return temp;
    }

#endif

//      compose2(f,g,h)() == f( g(), h() )
// Note that this inherits from CFunType.
template <class F, class G, class H>
class XXCompose0Helper : public CFunType<typename F::template
Sig<typename RT<G>::ResultType,typename RT<H>::ResultType>::ResultType>
{
   F f;
   G g;
   H h;
public:
   XXCompose0Helper( const F& a, const G& b, const H& c) : f(a), g(b), h(c) {}

   typename F::template Sig<typename RT<G>::ResultType,
                            typename RT<H>::ResultType>::ResultType
   operator()() const
   {
       return f( g(), h() );
   }
#ifdef FCPP_DEBUG
   // This gives good diagnosis of the examples of
   // compose(Full0(),Full0()).
   // This is needed because the helper is turned into
   // a Full0 functoid on the fly.
   // This could also be put into the other helpers.
   // In that case also add extra items in the traits
   // namespace after the Compose definition.
    std::string name() const
    {
      return name_F_G_H("compose2Helper0",f,g,h);
    }
#endif

};

  // This is needed for variadic_fcpp.h.
  // It works better here in this namespace.
template <class F, class G, class H, class I>
class X3Compose0Helper : public CFunType<typename F::template
Sig<typename RT<G>::ResultType,typename RT<H>::ResultType,
    typename RT<I>::ResultType>::ResultType>
{
   F f;
   G g;
   H h;
   I i;
public:
 X3Compose0Helper( const F& a, const G& b, const H& c, const I& d)
   : f(a), g(b), h(c), i(d) {}

   typename F::template Sig<typename RT<G>::ResultType,
     typename RT<H>::ResultType,
     typename RT<I>::ResultType>::ResultType
   operator()() const
   {
     return f( g(), h(), i() );
   }
#ifdef FCPP_DEBUG
   std::string name() const
    {
       return name_F_G_H_I("compose3Helper0",f,g,h,i);
    }
#endif
};

  // This is needed for variadic_fcpp.h.
  // It works better here in this namespace.
  template <class F, class G, class H, class I, class J>
class X4Compose0Helper : public CFunType<typename F::template
Sig<typename RT<G>::ResultType,typename RT<H>::ResultType,
    typename RT<I>::ResultType,typename RT<J>::ResultType>::ResultType>
{
   F f;
   G g;
   H h;
   I i;
   J j;
public:
 X4Compose0Helper( const F& a, const G& b, const H& c,
                   const I& d, const J& e)
   : f(a), g(b), h(c), i(d), j(e) {}

   typename F::template Sig<typename RT<G>::ResultType,
     typename RT<H>::ResultType,typename RT<I>::ResultType,
     typename RT<J>::ResultType>::ResultType
   operator()() const
   {
     return f( g(), h(), i(), j() );
   }
#ifdef FCPP_DEBUG
   // This gives good diagnosis of the examples of
   // compose(Full0(),Full0()).
   // This is needed because the helper is turned into
   // a Full0 functoid on the fly.
   // This could also be put into the other helpers.
   // In that case also add extra items in the traits
   // namespace after the Compose definition.
    std::string name() const
    {
      // The complexity is to cover the case where
      // all or some of F G and H have the name defined.
      return name_F_G_H_I_J("compose4Helper0",f,g,h,i,j);
    }
#endif
};

//      compose2(f,g,h)(x) == f( g(x), h(x) )
template <class F, class G, class H>
class XXCompose1Helper {
   F f;
   G g;
   H h;
public:
   XXCompose1Helper( const F& a, const G& b, const H& c) : f(a), g(b), h(c) {}

   template <class T>
   struct Sig : public FunType<
   typename G::template Sig<T>::Arg1Type,
   typename F::template Sig<typename G::template Sig<T>::ResultType,
                            typename H::template Sig<T>::ResultType>::ResultType
   > {};

   template <class T>
   typename F::template Sig<typename G::template Sig<T>::ResultType,
                            typename H::template Sig<T>::ResultType>::ResultType
   operator()( const T& x ) const {
      return f( g(x), h(x) );
   }

#ifdef FCPP_DEBUG
    std::string name() const
    {
      return name_F_G_H("compose2Helper1",f,g,h);
    }
#endif
};

//      compose2(f,g,h)(x,y) == f( g(x,y), h(x,y) )
//      Both arguments to both functions.
template <class F, class G, class H>
class XXCompose2Helper {
   F f;
   G g;
   H h;
public:
   XXCompose2Helper( const F& a, const G& b, const H& c) : f(a), g(b), h(c) {}
  
   template <class T,class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<
     typename G::template Sig<T,T>::Arg1Type,
     typename G::template Sig<T,T>::Arg2Type,
     typename F::template Sig<typename G::template Sig<T,T>::ResultType,
     typename H::template Sig<T,T>::ResultType>::ResultType
   > {};

   template <class T>
   typename F::template Sig<typename G::template Sig<T,T>::ResultType,
   typename H::template Sig<T,T>::ResultType>::ResultType
   operator()( const T& x, const T & y) const {
     return f( g(x,y), h(x,y) );
   }

#ifdef FCPP_DEBUG
    std::string name() const
    {
      return name_F_G_H("compose2Helper2",f,g,h);
    }
#endif
};

/////////////////////////////////////////////////////////////////////////////
// compose2xy allows the two functions g and h to have different arguments.
// This started with 1 argument each, hence the name.
// It has now been extended to support the following numbers of argumentes:
// (1,1) (1,2) (2,1) (2,2).
// All the arguments are assumed to have the same type.
// The name is changed from compose21.
/////////////////////////////////////////////////////////////////////////////
//      compose2xy(f,g,h)(x,y) == f( g(x), h(y) )
//      One argument to each function.
template <class F, class G, class H>
class XX2Compose1Helper {
   F f;
   G g;
   H h;
public:
   XX2Compose1Helper( const F& a, const G& b, const H& c) : f(a), g(b), h(c) {}

   template <class T,class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<
   typename G::template Sig<T>::Arg1Type, typename H::template Sig<T>::Arg1Type,
   typename F::template Sig<typename G::template Sig<T>::ResultType,
                            typename H::template Sig<T>::ResultType>::ResultType
   > {};

   // Trial of new idea allowing each of g and h to have a separate argument.
   // This is a separate functoid compose2xy
   template <class T>
   typename F::template Sig<typename G::template Sig<T>::ResultType,
                            typename H::template Sig<T>::ResultType>::ResultType
     operator()( const T& x, const T& y ) const {
      return f( g(x), h(y) );
   }

#ifdef FCPP_DEBUG
    std::string name() const
    {
      // The complexity is to cover the case where
      // all or some of F G and H have the name defined.
      return name_F_G_H("compose2xyHelper11",f,g,h);
    }
#endif
};

//      compose2xy(f,g,h)(x,y,z) == f( g(x), h(y,z) )
//      One argument to g, two to h.
template <class F, class G, class H>
class XX2Compose12Helper {
   F f;
   G g;
   H h;
public:
   XX2Compose12Helper( const F& a, const G& b, const H& c) : f(a), g(b), h(c) {}

   template <class T,class U,class V> struct Sig;

   template <class T>
     struct Sig<T,T,T> : public FunType<
     typename G::template Sig<T>::Arg1Type,
     typename H::template Sig<T,T>::Arg1Type,
     typename H::template Sig<T,T>::Arg2Type,
     typename F::template Sig<typename G::template Sig<T>::ResultType,
                            typename H::template Sig<T,T>::ResultType
                            >::ResultType
   > {};

   // Trial of new idea allowing each of g and h to have a separate argument.
   // This is a separate functoid compose2xy
   template <class T>
   typename F::template Sig<typename G::template Sig<T>::ResultType,
                          typename H::template Sig<T,T>::ResultType>::ResultType
     operator()( const T& x, const T& y, const T& z ) const {
     return f( g(x), h(y,z) );
   }

#ifdef FCPP_DEBUG
    std::string name() const
    {
      // The complexity is to cover the case where
      // all or some of F G and H have the name defined.
      return name_F_G_H("compose2xyHelper12",f,g,h);
    }
#endif
};

//      compose2xy(f,g,h)(x,y,z) == f( g(x,y), h(z) )
//      Two arguments to g, one to h.
template <class F, class G, class H>
class XX2Compose21Helper {
   F f;
   G g;
   H h;
public:
   XX2Compose21Helper( const F& a, const G& b, const H& c) : f(a), g(b), h(c) {}

   template <class T,class U,class V> struct Sig;

   template <class T>
     struct Sig<T,T,T> : public FunType<
     typename G::template Sig<T,T>::Arg1Type,
     typename G::template Sig<T,T>::Arg2Type,
     typename H::template Sig<T>::Arg1Type,
     typename F::template Sig<typename G::template Sig<T,T>::ResultType,
                              typename H::template Sig<T>::ResultType
                            >::ResultType
   > {};

   // Trial of new idea allowing each of g and h to have a separate argument.
   // This is a separate functoid compose2xy
   template <class T>
     typename F::template Sig<typename G::template Sig<T,T>::ResultType,
                          typename H::template Sig<T>::ResultType>::ResultType
     operator()( const T& x, const T& y, const T& z ) const {
     return f( g(x,y), h(z) );
   }

#ifdef FCPP_DEBUG
    std::string name() const
    {
      // The complexity is to cover the case where
      // all or some of F G and H have the name defined.
      return name_F_G_H("compose2xyHelper21",f,g,h);
    }
#endif
};

#ifdef FCPP152
//      compose2xy(f,g,h)(x,y,p,q) == f( g(x,y), h(p,q) )
//      Two arguments to each function.
//      This needs Full4
template <class F, class G, class H>
class XX2Compose2Helper {
   F f;
   G g;
   H h;
public:
   XX2Compose2Helper( const F& a, const G& b, const H& c) : f(a), g(b), h(c) {}

   template <class T,class U, class V, class W> struct Sig;

   template <class T>
     struct Sig<T,T,T,T> : public FunType<
     typename G::template Sig<T,T>::Arg1Type,
     typename G::template Sig<T,T>::Arg2Type,
     typename H::template Sig<T,T>::Arg1Type,
     typename H::template Sig<T,T>::Arg2Type,
     typename F::template Sig<typename G::template Sig<T,T>::ResultType,
     typename H::template Sig<T,T>::ResultType>::ResultType
   > {};

   // Trial of new idea allowing each of g and h to have a separate argument.
   // This is a separate functoid compose2xy
   template <class T>
     typename F::template Sig<typename G::template Sig<T,T>::ResultType,
     typename H::template Sig<T,T>::ResultType>::ResultType
     operator()( const T& x, const T& y, const T& p, const T& q ) const {
     return f( g(x,y), h(p,q) );
   }

#ifdef FCPP_DEBUG
    std::string name() const
    {
      // The complexity is to cover the case where
      // all or some of F G and H have the name defined.
      return name_F_G_H("compose2xyHelper22",f,g,h);
    }
#endif
};
#endif

//      compose2(f,g,h)(x) == f( g(x), h(x) )
// Compose2 composes a two argument function with two one-argument
// functions (taking the same type). This is quite useful for the
// common case of binary operators.  Use lambda for more-complicated stuff.
// This is now extended to the two argument case.
//      compose2(f,g,h)(x,y) == f( g(x,y), h(x,y) )
struct XCompose2 {
  // Now test both G and H for no of args.
  // This will fail unless the no is the same.
  template <int i, int j, class F, class G, class H> struct Helper;
  template <class F, class G, class H>
    struct Helper<0,0,F,G,H> {
    typedef Full0<XXCompose0Helper<F,G,H> > Result;
    static Result go( const F& f, const G& g, const H& h )
    { return makeFull0(XXCompose0Helper<F,G,H>(f,g,h)); }
   };
  template <class F, class G, class H>
    struct Helper<1,1,F,G,H> {
     typedef Full1<XXCompose1Helper<F,G,H> > Result;
      static Result go( const F& f, const G& g, const H& h )
      { return makeFull1(XXCompose1Helper<F,G,H>(f,g,h)); }
   };
  template <class F, class G, class H>
    struct Helper<2,2,F,G,H> {
     typedef Full2<XXCompose2Helper<F,G,H> > Result;
      static Result go( const F& f, const G& g, const H& h )
      { return makeFull2(XXCompose2Helper<F,G,H>(f,g,h)); }
   };
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Compose2");
    }
#endif
  template <class F, class G, class H> struct Sig : public
      FunType<F,G,H,typename Helper<FunctoidTraits<G>::max_args,
      FunctoidTraits<H>::max_args,F,G,H>::Result>{};

   template <class F, class G, class H>
   typename Sig<F,G,H>::ResultType
   //Full1<XXCompose1Helper<F,G,H> >
   operator()(const F& f, const G& g, const H& h) const {
     //return makeFull1( XXCompose1Helper<F,G,H>( f, g, h ) );
     return Helper<FunctoidTraits<G>::max_args,
     FunctoidTraits<H>::max_args,F,G,H>::go( f, g, h );
   }
};

//      compose2xy(f,g,h)(x) == f( g(x), h(y) )
// Compose2xy composes a two argument function with two one-argument
// functions (with separate arguments of the same type).
// See comment above for the other options.
struct XCompose2xy {
  // Now test both G and H for no of args.
  // This will fail unless the no is the same.
  template <int i, int j, class F, class G, class H> struct Helper;
  template <class F, class G, class H>
    struct Helper<1,1,F,G,H> {
     typedef Full2<XX2Compose1Helper<F,G,H> > Result;
      static Result go( const F& f, const G& g, const H& h )
      { return makeFull2(XX2Compose1Helper<F,G,H>(f,g,h)); }
   };
  template <class F, class G, class H>
    struct Helper<1,2,F,G,H> {
     typedef Full3<XX2Compose12Helper<F,G,H> > Result;
      static Result go( const F& f, const G& g, const H& h )
      { return makeFull3(XX2Compose12Helper<F,G,H>(f,g,h)); }
   };
  template <class F, class G, class H>
    struct Helper<2,1,F,G,H> {
     typedef Full3<XX2Compose21Helper<F,G,H> > Result;
      static Result go( const F& f, const G& g, const H& h )
      { return makeFull3(XX2Compose21Helper<F,G,H>(f,g,h)); }
   };
#ifdef FCPP152
  template <class F, class G, class H>
    struct Helper<2,2,F,G,H> {
     typedef Full4<XX2Compose2Helper<F,G,H> > Result;
      static Result go( const F& f, const G& g, const H& h )
      { return makeFull4(XX2Compose2Helper<F,G,H>(f,g,h)); }
   };
#endif
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Compose2xy");
    }
#endif
  template <class F, class G, class H> struct Sig : public
      FunType<F,G,H,typename Helper<FunctoidTraits<G>::max_args,
      FunctoidTraits<H>::max_args,F,G,H>::Result>{};

   template <class F, class G, class H>
   typename Sig<F,G,H>::ResultType
   //Full1<XXCompose1Helper<F,G,H> >
   operator()(const F& f, const G& g, const H& h) const {
     //return makeFull1( XXCompose1Helper<F,G,H>( f, g, h ) );
     return Helper<FunctoidTraits<G>::max_args,
     FunctoidTraits<H>::max_args,F,G,H>::go( f, g, h );
   }
};

}
typedef Full3<impl::XCompose2> Compose2;
typedef Full3<impl::XCompose2xy> Compose2xy;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Compose2 compose2, dot2;
FCPP_MAYBE_EXTERN Compose2xy compose2xy, compose21, combine;
FCPP_MAYBE_NAMESPACE_CLOSE


namespace impl {
struct XUntil {
   template <class Pred, class Unary, class T>
   struct Sig : public FunType<Pred,Unary,T,T> {};

   template <class Pred, class Unary, class T>
   T operator()( const Pred& p, const Unary& op, T start ) const {
      while( !p(start) ) {
         T tmp( start );
         start.~T();
         new (&start) T( op(tmp) );
      }
      return start;
   }
};

struct XWhile {
   template <class Pred, class Unary, class T>
   struct Sig : public FunType<Pred,Unary,T,T> {};

   template <class Pred, class Unary, class T>
   T operator()( const Pred& p, const Unary& op, T start ) const {
      while( p(start) ) {
         T tmp( start );
         start.~T();
         new (&start) T( op(tmp) );
      }
      return start;
   }
};
}
typedef Full3<impl::XUntil> Until;
typedef Full3<impl::XWhile> While;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Until until;
FCPP_MAYBE_EXTERN While while_;
FCPP_MAYBE_NAMESPACE_CLOSE

  // last and init will not terminate if given a list which does not
  // terminate e.g. enumFrom(1).
  // There are some others e.g. reverse which will have the same problem.
namespace impl {
struct XLast {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& ll ) const {
     size_t x = 0;
     List<typename L::ElementType> l = ll;
     while( !null( tail(l) ) ) {
         l = tail(l);
         ++x;
         if (x > MAX_LIST_LENGTH)
            break;
     }
#ifdef FCPP_USE_EXCEPTIONS
     if (x > MAX_LIST_LENGTH)
           throw fcpp_exception("Your list is too long!!");
#endif
     return head(l);
   }
};
}
typedef Full1<impl::XLast> Last;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Last last;
FCPP_MAYBE_NAMESPACE_CLOSE


  // init is difficult to make safe.
  // I don't want to have to add the overhead of length(l) at each step.
  // The implementation is recursive so counting is difficult.
namespace impl {
#ifdef FCPP_SIMPLE_PRELUDE
struct XInit {
   template <class L>
   struct Sig : public FunType<L,List<typename L::ElementType> > {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
      if( null( tail( l ) ) )
         return NIL;
      else
         return cons( head(l), curry( XInit(), tail(l) ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Init");
   }
#endif
};
#else
struct XInit {
   template <class L>
   struct Sig : public FunType<L,OddList<typename L::ElementType> > {};

   template <class L>
   OddList<typename L::ElementType>
   operator()( const L& l,
               Reuser1<Inv,Var,XInit,List<typename L::ElementType> >
               r = NIL ) const {
      if( null( tail( l ) ) )
         return NIL;
      else {
        //std::cout << head(l) << std::endl;
         return cons( head(l), r( XInit(), tail(l) ) );
      }
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Init");
   }
#endif
};
#endif
}
typedef Full1<impl::XInit> Init;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Init init;
FCPP_MAYBE_NAMESPACE_CLOSE


namespace impl {
struct XLength {
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Length");
    }
#endif
   template <class L>
   struct Sig : public FunType<L,size_t> {};

   template <class L>
   size_t operator()( const L& ll ) const {
      size_t x = 0;
     {
      List<typename L::ElementType> l = ll;
      while( !null(l) ) {
         l = tail(l);
         ++x;
         if (x > MAX_LIST_LENGTH)
            break;
      }
     }
#ifdef FCPP_USE_EXCEPTIONS
     if (x > MAX_LIST_LENGTH)
           throw fcpp_exception("Your list is too long!!");
#endif
     return x;
   }
};
}
typedef Full1<impl::XLength> Length;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Length length;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
// At is Haskell's operator (!!)
// What should this do when out of range?
// At the moment it fails with attempt to take head of empty list.
struct XAt {
   template <class L, class N>
   struct Sig : public FunType<L,N,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( L l, size_t n ) const {
      while( n!=0 ) {
         l = tail(l);
#ifdef FCPP_USE_EXCEPTIONS
         if (null(l) )
           throw fcpp_exception("at(l,n) is beyond end of list");
#endif
         --n;
      }
#ifdef FCPP_USE_EXCEPTIONS
      if (null(l) )
           throw fcpp_exception("at(l,n) is out of range");
#endif
      return head(l);
   }
};
}
typedef Full2<impl::XAt> At;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN At at;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
#ifdef FCPP_SIMPLE_PRELUDE
struct XFilter {
   template <class P, class LT>
   struct Sig : public FunType<P,LT,List<typename LT::ElementType> > {};

   template <class P, class T>
   List<T> operator()( const P& p, const List<T>& l ) const {
      if( null(l) )
         return l;
      else if( p(head(l)) )
        return cons( head(l), curry2( XFilter(), p, tail(l) ) );
      else
         return XFilter()( p, tail(l) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Filter");
   }
#endif
};
#else
template <class P, class T>
struct XFilterHelp : public Fun0Impl< OddList<T> > {
   P p;
   mutable List<T> l;
   XFilterHelp( const P& pp, const List<T>& ll ) : p(pp), l(ll) {}
   OddList<T> operator()() const {
      while(1) {
         if( null(l) )
            return NIL;
         else if( p( head(l) ) ) {
            T x = head(l);
            l = tail(l);
            return cons( x, Fun0< OddList<T> >(1,this) );
         }
         else
            l = tail(l);
      }
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (debug_traits_has_name<P>()) {
        temp += std::string("Filter::")
             + debug_traits_name(p);
        if (debug_traits_has_name<List<T> >() ) {
          temp += std::string("::")
                + debug_traits_name(l);
        } else {
          temp += std::string("( ")
               + argument_traits_value(l)
               + std::string(" )");
        }
     } else {
        temp += std::string("Filter( ")
          + std::string(" )");
     }
     return temp;
   }
#endif
};
template <class P, class T>
struct XFilterHelpNot : public Fun0Impl< OddList<T> > {
   P p;
   mutable List<T> l;
   XFilterHelpNot( const P& pp, const List<T>& ll ) : p(pp), l(ll) {}
   OddList<T> operator()() const {
      while(1) {
         if( null(l) )
            return NIL;
         else if( !p( head(l) ) ) {
            T x = head(l);
            l = tail(l);
            return cons( x, Fun0< OddList<T> >(1,this) );
         }
         else
            l = tail(l);
      }
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("FilterNot( ")
          + std::string(" )");
   }
#endif
};
struct XFilter {
   template <class P, class L>
   struct Sig : public FunType<P,L,List<typename L::ElementType> > {};

   template <class P, class L>
   List<typename L::ElementType>
   operator()( const P& p, L l ) const {
     // An extra pair of brackets are optional here!!
      return Fun0< OddList<typename L::ElementType> >(1,
             new XFilterHelp<P,typename L::ElementType>(p,l) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Filter");
   }
#endif
};

// These are based on the simple prelude version for now.
/* No longer used.
struct XFilter2Inner {
   template <class P, class LT>
   struct Sig : public FunType<P,LT,List<typename LT::ElementType> > {};

   template <class P, class T>
   List<T> operator()( const P& p, const List<T>& l ) const {
      if( null(l) )
         return l;
      else if( p(head(l)) )
        return cons( head(l), curry2( XFilter2Inner(), p, tail(l) ) );
      else
         return XFilter2Inner()( p, tail(l) );
   }
};

struct XFilter2InnerNot {
   template <class P, class LT>
   struct Sig : public FunType<P,LT,List<typename LT::ElementType> > {};

   template <class P, class T>
   List<T> operator()( const P& p, const List<T>& l ) const {
      if( null(l) )
         return l;
      else if( !p(head(l)) )
        return cons( head(l), curry2( XFilter2InnerNot(), p, tail(l) ) );
      else
         return XFilter2InnerNot()( p, tail(l) );
   }
};
*/

/* I have not found a way to avoid traversing the list twice.
   It does now use the same method as XFilterHelp, and the same
   code for the true case.
   For the false case I have to define XFilterHelpNot.
 */
template <class P, class T>
  struct XFilterHelp2 : public Fun0Impl< std::pair<List<T>,List<T> > > {
   P p;
   mutable List<T> l,l0,l1,l2;
   XFilterHelp2( const P& pp, const List<T>& ll ) : p(pp), l(ll) {}
   // Construct iteratively for each list separately.
   std::pair<List<T>,List<T> > operator()() const {
     l0 = l;
     //l1 = XFilter2Inner()(p,l);
     l1 = Fun0< OddList<T> >(1,new XFilterHelp<P,T>(p,l))();
     l = l0;
     //l2 = XFilter2InnerNot()(p,l);
     l2 = Fun0< OddList<T> >(1,new XFilterHelpNot<P,T>(p,l))();
     return std::make_pair(l1,l2);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("FilterHelp2");
   }
#endif
};

// filter2 returns a std::pair which is the pair of lists for the
// test predicate p being true and false.
// It is implemented using XFilterHelp2.
struct XFilter2 {
   template <class P, class L>
     struct Sig : public FunType<P,L,
     std::pair<List<typename L::ElementType>,List<typename L::ElementType> > > {};

   template <class P, class L>
   std::pair<List<typename L::ElementType>,List<typename L::ElementType> >
   operator()( const P& p, L l ) const {
     return Fun0<std::pair< List<typename L::ElementType>,
                            List<typename L::ElementType> > >
       // I don't know why this needs an extra () at the end
       // not needed for XFilterHelp in the same situation.
       // The brackets are optional there.
       (1,new XFilterHelp2<P,typename L::ElementType>(p,l))();
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Filter2");
   }
#endif
};
/* For filter, the version with a Reuser is just not as good as the
hand-coded reuse version, which is why this is commented out.
struct XFilter {
   template <class P, class L>
   struct Sig : public FunType<P,L,OddList<typename L::ElementType> > {};

   template <class P, class T>
   OddList<T> operator()( const P& p, List<T> l,
                            Reuser2<Inv,Inv,Var,XFilter,P,List<T> > 
                            r = NIL ) const {
      while(1) {
         if( null(l) )
            return NIL;
         else if( p(head(l)) )
            return cons( head(l), r( XFilter(), p, tail(l) ) );
         else
            r.iter( XFilter(), p, l = tail(l) );
      }
   }
};
*/
#endif
}
typedef Full2<impl::XFilter> Filter;
typedef Full2<impl::XFilter2> Filter2;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Filter filter;
FCPP_MAYBE_EXTERN Filter2 filter2;
FCPP_MAYBE_NAMESPACE_CLOSE

/* I have not at first been able to get this to work. */
/* What is needed is to make a functoid which will
   apply not to the result of the application of x.
   this works when combined with compose e.g.
   compose(not_,even)
 */
namespace impl {
 struct XNot {
    
    template<class T>
    struct Sig : public FunType<T,bool> {};

    template <class T>
    bool operator()( const T& x ) const {
      return !x;
    }
 };
 // There is already an And to work on lists.
 struct XAnd2 {
   template<class T,class U> struct Sig;

   template<class T>
   struct Sig<T,T> : public FunType<T,T,bool> {};

    template <class T>
    bool operator()( const T& x, const T& y ) const {
      return x && y;
    }
 };
 struct XOr2 {
   template<class T,class U> struct Sig;

   template<class T>
   struct Sig<T,T> : public FunType<T,T,bool> {};

    template <class T>
    bool operator()( const T& x, const T& y ) const {
      return x || y;
    }
 };
}
typedef Full1<impl::XNot> Not_;
typedef Full2<impl::XAnd2> And2;
typedef Full2<impl::XOr2> Or2;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Not_ not_;
FCPP_MAYBE_EXTERN And2 and2;
FCPP_MAYBE_EXTERN Or2 or2;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
  struct XNumber {
    template <class N, class L>
   struct Sig : public FunType<N, L,size_t> {};

    template <class N,class L>
   size_t operator()( const N& item, const L& ll ) const {
     return length(filter( bind2of2( equal, item ))(ll) );
   }
  };
}

typedef Full2<impl::XNumber> Number;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Number number;
FCPP_MAYBE_NAMESPACE_CLOSE


namespace impl {
  struct XContains {
    template <class N, class L>
   struct Sig : public FunType<N, L, bool> {};

    template <class N,class L>
    bool operator()( const N& item, const L& ll ) const {
      return number(item, ll) > 0;
   }
  };
}

typedef Full2<impl::XContains> Contains;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Contains contains, in;
FCPP_MAYBE_NAMESPACE_CLOSE



namespace impl {
#ifdef FCPP_SIMPLE_PRELUDE
struct XConcat {
   template <class LLT>
   struct Sig : public FunType<LLT, typename LLT::ElementType> {};

   template <class T>
   List<T> operator()( const List<List<T> >& l ) const {
      if( null(l) )
         return List<T>();
      else
         return cat( head(l), curry(XConcat(),tail(l)) );
   }
};
#else
struct XConcat {
   template <class LLT>
   struct Sig : public FunType<LLT, 
      OddList<typename LLT::ElementType::ElementType> > {};

   template <class L>
   OddList<typename L::ElementType::ElementType> 
   operator()( const L& l, 
               Reuser1<Inv,Var,XConcat,List<typename L::ElementType> > 
               r = NIL ) const {
      if( null(l) )
         return NIL;
      else
         return cat( head(l), r(XConcat(),tail(l)) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Concat");
    }
#endif
};
#endif
}
typedef Full1<impl::XConcat> Concat;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Concat concat;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
// Note: this isn't lazy (even if 'op' is 'cons').
struct XFoldr {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,E> {};

   template <class Op, class E, class L>
   E operator()( const Op& op, const E& e, const L& l ) const {
      if( null(l) )
         return e;
      else
         return op( head(l), XFoldr()( op, e, tail(l) ) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Foldr");
    }
#endif
};
}
typedef Full3<impl::XFoldr> Foldr;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Foldr foldr;
FCPP_MAYBE_NAMESPACE_CLOSE


namespace impl {
struct XFoldr1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,typename L::ElementType> {};

   template <class Op, class L>
   typename L::ElementType operator()( const Op& op, const L& l ) const {
#ifdef FCPP_DEBUG
#ifdef FCPP_USE_EXCEPTIONS
      if( null(l) )
         throw fcpp_exception("Tried to foldr1() an empty List");
#endif
#endif
      return foldr( op, head(l), tail(l) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Foldr1");
    }
#endif
};
}
typedef Full2<impl::XFoldr1> Foldr1;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Foldr1 foldr1;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XFoldl {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,E> {};

   template <class Op, class E, class L>
   E operator()( const Op& op, E e, const L& ll ) const {
      List<typename L::ElementType> l = ll;
      while( !null(l) ) {
         E tmp( e );
         e.~E();
         new (&e) E( op(tmp,head(l)) );
         l = tail(l);
      }
      return e;
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Foldl");
    }
#endif
};
}
typedef Full3<impl::XFoldl> Foldl;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Foldl foldl;
FCPP_MAYBE_NAMESPACE_CLOSE


namespace impl {
struct XFoldl1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,typename L::ElementType> {};

   template <class Op, class L>
   typename L::ElementType operator()( const Op& op, const L& l ) const {
#ifdef FCPP_DEBUG
#ifdef FCPP_USE_EXCEPTIONS
      if( null(l) )
         throw fcpp_exception("Tried to foldl1() an empty List");
#endif
#endif
       return foldl( op, head(l), tail(l) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Foldl1");
    }
#endif
};
}
typedef Full2<impl::XFoldl1> Foldl1;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Foldl1 foldl1;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XScanr {
#ifdef FCPP_CXX11
  // This works but does not enable one of the problem cases.
   template <class Op, class E, class L>
     struct Sig : public FunType<Op,E,L,
     OddList<decltype(*(E*)(0)+ *(typename L::ElementType*)(0))> > {};
 
  template <class Op, class E, class L>
    //  auto
    OddList<decltype(*(E*)(0)+ *(typename L::ElementType*)(0))>
    operator()( const Op& op, const E& e, const L& l ) const
    //                -> decltype(e+head(l))
  {
      if( null(l) )
         return cons( e, NIL );
      else {
         OddList<decltype(*(E*)(0)+ *(typename L::ElementType*)(0))> temp
             = XScanr()( op, e, tail(l) );
         return cons( op( head(l), head(temp) ), temp );
      }
#else
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,OddList<E> > {};

   template <class Op, class E, class L>
   OddList<E> operator()( const Op& op, const E& e, const L& l ) const {
      if( null(l) )
         return cons( e, NIL );
      else {
         OddList<E> temp = XScanr()( op, e, tail(l) );
         return cons( op( head(l), head(temp) ), temp );
      }
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Scanr");
    }
#endif
};
// This is a variation of scanr where the type of the list elements
// are assumed to be typename L::ElementType and not E.
// This works with FCPP_CXX11 while the other one does not.
// How to do it copied from scanr1 see below.
struct XScanrl {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,OddList<typename L::ElementType> > {};

   template <class Op, class E, class L>
   OddList<typename L::ElementType> operator()
      ( const Op& op, const E& e, const L& l ) const {
     if( null(l) ) {
       // Assumption is made that this conversion will work.
       typename L::ElementType temp = e;
       return cons( temp, NIL );
     } else {
         OddList<typename L::ElementType> temp = XScanrl()( op, e, tail(l) );
         return cons( op( head(l), head(temp) ), temp );
      }
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Scanrl");
    }
#endif
};
}
typedef Full3<impl::XScanr> Scanr;
typedef Full3<impl::XScanrl> Scanrl;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Scanr scanr;
FCPP_MAYBE_EXTERN Scanrl scanrl;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XScanr1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,OddList<typename L::ElementType> > {};

   template <class Op, class L>
   OddList<typename L::ElementType> 
   operator()( const Op& op, const L& l ) const {
      if( null( tail(l) ) )
         return l.force();
      else {
         OddList<typename L::ElementType> temp = XScanr1()( op, tail(l) );
         return cons( op( head(l), head(temp) ), temp );
      }
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Scanr1");
    }
#endif
};
}
typedef Full2<impl::XScanr1> Scanr1;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Scanr1 scanr1;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
#ifdef FCPP_SIMPLE_PRELUDE
struct XScanl {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,List<E> > {};

   template <class Op, class E, class T>
   List<E> operator()( const Op& op, const E& e, const List<T>& l ) const {
      if( null(l) )
         return cons( e, NIL );
      else
         return cons( e, curry3( XScanl(), op, op(e,head(l)), tail(l) ));
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Scanl");
    }
#endif
};
#else
struct XScanl {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,OddList<E> > {};

   template <class Op, class E, class L>
   OddList<E> operator()( const Op& op, const E& e, const L& l,
         Reuser3<Inv,Inv,Var,Var,XScanl,Op,E,List<typename L::ElementType> >
         r = NIL ) const {
      if( null(l) )
         return cons( e, NIL );
      else
         return cons( e, r( XScanl(), op, op(e,head(l)), tail(l) ) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Scanl");
    }
#endif
};
#endif
}
typedef Full3<impl::XScanl> Scanl;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Scanl scanl;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XScanl1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,OddList<typename L::ElementType> > {};

   template <class Op, class L>
   OddList<typename L::ElementType> 
   operator()( const Op& op, const L& l ) const {
      return scanl( op, head(l), tail(l) );
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Scanl1");
    }
#endif
};
}
typedef Full2<impl::XScanl1> Scanl1;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Scanl1 scanl1;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
#ifdef FCPP_SIMPLE_PRELUDE
struct XIterate {
   template <class F, class T>
   struct Sig : public FunType<F,T,List<T> > {};

   template <class F, class T>
   List<T> operator()( const F& f, const T& x ) const {
      return cons( x, curry2( XIterate(), f, f(x) ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("Iterate");
   }
};
#endif
#else
struct XIterate {
   template <class F, class T>
   struct Sig : public FunType<F,T,OddList<T> > {};

   template <class F, class T>
   OddList<T> operator()( const F& f, const T& x,
                          Reuser2<Inv,Inv,Var,XIterate,F,T> r = NIL ) const {
      return cons( x, r( XIterate(), f, f(x) ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("Iterate");
   }
#endif
};
#endif
}
typedef Full2<impl::XIterate> Iterate;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Iterate iterate;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
#ifdef FCPP_SIMPLE_PRELUDE
struct XRepeat {
   template <class T>
   struct Sig : public FunType<T,List<T> > {};

   template <class T>
   List<T> operator()( const T& x ) const {
      return cons( x, curry( XRepeat(), x ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Repeat");
   }
#endif
};
#else
struct XRepeat {
   template <class T>
   struct Sig : public FunType<T,OddList<T> > {};

   template <class T>
   OddList<T> operator()( const T& x, 
                          Reuser1<Inv,Inv,XRepeat,T> r = NIL ) const {
      return cons( x, r( XRepeat(), x ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Repeat");
   }
#endif
};
#endif
}
typedef Full1<impl::XRepeat> Repeat;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Repeat repeat;
FCPP_MAYBE_NAMESPACE_CLOSE


namespace impl {
#ifdef FCPP_SIMPLE_PRELUDE
struct XMap {
   template <class F, class L>
   struct Sig : public FunType<F,L,
      List<typename F::template Sig<typename L::ElementType>::ResultType> > {};

   template <class F, class T>
   List<typename F::template Sig<T>::ResultType>
   operator()( const F& f, const List<T>& l ) const {
      if( null(l) )
         return NIL;
      else
         return cons( f(head(l)), curry2( XMap(), f, tail(l) ) );
   }
};
#else
struct XMap {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Map");
   }
#endif

     /* See Note 17 at the start of full.h */
#ifndef FCPP_CONCEPTS
   template <class F, class L>
   struct Sig : public FunType<F,L,
      OddList<typename RT<F,typename L::ElementType>::ResultType> > {};
#endif

   template <class F, class L>
     /* See Note 17 at the start of full.h */
#ifdef FCPP_CONCEPTS
   OddList<typename ConstCallable1<F,typename L::ElementType>::result_type>
#else
   OddList<typename RT<F,typename L::ElementType>::ResultType>
#endif
   operator()( const F& f, const L& l,
               Reuser2<Inv,Inv,Var,XMap,F,List<typename L::ElementType> >
               r = NIL ) const {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Map called " << std::endl;
#endif
     //#ifdef FCPP_DEBUG
     //std::cout << "XX Special: " << debug_name_or_value(f) << " XX ";
     //#endif
      if( null(l) )
         return NIL;
      else
         return cons( f(head(l)), r( XMap(), f, tail(l) ) );
   }
};

// Attempt to map two lists term by term
// e.g. map2(plus,l1,l2) where l1 and l2 are two lists of the same length.
// Compare with XMap above to see what changes are needed.
// Map2 is a Full3 functoid.
struct XMap2 {
  template <class F, class L, class M>
    struct Sig : public FunType<F,L,M,
    OddList<typename RT<F,typename L::ElementType,typename M::ElementType>::ResultType> > {};

  // Specialisation for the two lists having the same element type.
  template <class F, class L>
    struct Sig<F,L,L> : public FunType<F,L,L,
      OddList<typename RT<F,typename L::ElementType,typename L::ElementType>::ResultType> > {};

  // This one works on two lists with elements of the same type.
   template <class F, class L>
     OddList<typename RT<F,typename L::ElementType,typename L::ElementType>::ResultType>
     operator()( const F& f, const L& l1, const L& l2,
                Reuser3<Inv,Inv,Var,Var,XMap2,F,List<typename L::ElementType>,
                          List<typename L::ElementType> >
               r = NIL ) const {
     if( null(l1) || null(l2) )
         return NIL;
      else
        return cons( f(head(l1), head(l2)), r( XMap2(), f, tail(l1), tail(l2) ) );
   }

  // This one works on two lists with elements of different types.
  // This depends on the functoid F being able to handle this. Plus and Minus can do so.
  template <class F, class L, class M>
     OddList<typename RT<F,typename L::ElementType,typename M::ElementType>::ResultType>
     operator()( const F& f, const L& l1, const M& l2,
            Reuser3<Inv,Inv,Var,Var,XMap2,F,List<typename L::ElementType>,
                          List<typename M::ElementType> >
               r = NIL ) const {
     if( null(l1) || null(l2) )
         return NIL;
      else
        return cons( f(head(l1), head(l2)), r( XMap2(), f, tail(l1), tail(l2) ) );
   }
};
#endif
}
typedef Full2<impl::XMap> Map;
typedef Full3<impl::XMap2> Map2;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Map map;
FCPP_MAYBE_EXTERN Map2 map2;
FCPP_MAYBE_NAMESPACE_CLOSE

  /* See Note 17 at the start of full.h */
  /*  
#ifdef FCPP_CONCEPTS
  template <class F,class X >
  concept_map ConstCallable2<Map,F,List<X> >
  {
    typedef ConstCallable2<Map,F,OddList<X>> result_type;
  };
#endif
  */

  // for_ == flip(map) equivalent of forM<ListM>()
namespace impl {
  struct XFor {
#ifndef FCPP_CONCEPTS
   template <class L, class F>
   struct Sig : public FunType<L,F,
      OddList<typename RT<F,typename L::ElementType>::ResultType> > {};
#endif
   template <class L, class F>
#ifdef FCPP_CONCEPTS
    OddList<typename ConstCallable1<F,typename L::ElementType>::result_type>
#else
     OddList<typename RT<F,typename L::ElementType>::ResultType>
#endif
     operator()( const L& l, const F& f ) const {
     return map(f,l);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("For");
   }
#endif
  };
}

typedef Full2<impl::XFor> For;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN For for_;
FCPP_MAYBE_NAMESPACE_CLOSE


namespace impl {
#ifdef FCPP_SIMPLE_PRELUDE
struct XTake {
   template <class N,class L>
   struct Sig : public FunType<N,L,List<typename L::ElementType> > {};

   template <class T>
   List<T> operator()( size_t n, const List<T>& l ) const {
      if( n==0 || null(l) )
         return NIL;
      else
         return cons( head(l), curry2( XTake(), n-1, tail(l) ) );
   }
};
#else
struct XTake {
   template <class N,class L>
   struct Sig : public FunType<N,L,OddList<typename L::ElementType> > {};

   template <class L>
   OddList<typename L::ElementType> 
   operator()( size_t n, const L& l,
               Reuser2<Inv,Var,Var,XTake,size_t,List<typename L::ElementType> > 
               r = NIL ) const {
      if( n==0 || null(l) )
         return NIL;
      else
         return cons( head(l), r( XTake(), n-1, tail(l) ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("Take");
   }
#endif
};
#endif
}
typedef Full2<impl::XTake> Take;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Take take;
FCPP_MAYBE_NAMESPACE_CLOSE


namespace impl {
struct XDrop {
   template <class Dummy, class L>
   struct Sig : public FunType<size_t,L,List<typename L::ElementType> > {};
   
   template <class L>
   List<typename L::ElementType> operator()( size_t n, const L& ll ) const {
      List<typename L::ElementType> l = ll;
      while( n!=0 && !null(l) ) {
         --n;
         l = tail(l);
      }
      return l;
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Drop");
   }
#endif
};
}
typedef Full2<impl::XDrop> Drop;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Drop drop;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
#ifdef FCPP_SIMPLE_PRELUDE
struct XTakeWhile {
   template <class P, class L>
   struct Sig : public FunType<P,L,List<typename L::ElementType> > {};

   template <class P, class T>
   List<T> operator()( const P& p, const List<T>& l ) const {
      if( null(l) || !p( head(l) ) )
         return NIL;
      else
         return cons( head(l), curry2( XTakeWhile(), p, tail(l) ) );
   }
};
#else
struct XTakeWhile {
   template <class P, class L>
   struct Sig : public FunType<P,L,OddList<typename L::ElementType> > {};

   template <class P, class L>
   OddList<typename L::ElementType>
   operator()( const P& p, const L& l,
               Reuser2<Inv,Inv,Var,XTakeWhile,P,List<typename L::ElementType> >
               r = NIL ) const {
      if( null(l) || !p( head(l) ) )
         return NIL;
      else
         return cons( head(l), r( XTakeWhile(), p, tail(l) ) );
   }
};

// JPF new based on TakeWhile.
struct XTakeUntil {
   template <class P, class L>
   struct Sig : public FunType<P,L,OddList<typename L::ElementType> > {};

   template <class P, class L>
   OddList<typename L::ElementType>
   operator()( const P& p, const L& l,
               Reuser2<Inv,Inv,Var,XTakeUntil,P,List<typename L::ElementType> >
               r = NIL ) const {
      if( null(l) || p( head(l) ) )
         return NIL;
      else
         return cons( head(l), r( XTakeUntil(), p, tail(l) ) );
   }
};

#endif
}
typedef Full2<impl::XTakeWhile> TakeWhile;
typedef Full2<impl::XTakeUntil> TakeUntil;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN TakeWhile takeWhile;
FCPP_MAYBE_EXTERN TakeUntil takeUntil;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {

struct XDropWhile {
   template <class P, class L>
   struct Sig : public FunType<P,L,List<typename L::ElementType> > {};

   template <class P, class L>
   List<typename L::ElementType> operator()( const P& p, const L& ll ) const {
      List<typename L::ElementType> l = ll;
      while( !null(l) && p( head(l) ) )
         l = tail(l);
      return l;
   }
};

struct XDropUntil {
   template <class P, class L>
   struct Sig : public FunType<P,L,List<typename L::ElementType> > {};

   template <class P, class L>
   List<typename L::ElementType> operator()( const P& p, const L& ll ) const {
      List<typename L::ElementType> l = ll;
      while( !null(l) && !p( head(l) ) )
         l = tail(l);
      return l;
   }
};

// For FCPP_CONCEPTS
// I have put this together because the calls at 1137 and 1153
// will not compile with a curried argument. This works.
struct XDropWhileEqual {
   template <class L>
   struct Sig : public FunType<L,List<typename L::ElementType> > {};

  template <class T, class L>
    List<typename L::ElementType> operator()( const T& t, const L& ll ) const {
      List<typename L::ElementType> l = ll;
      while( !null(l) && equal( t, head(l) ) )
         l = tail(l);
      return l;
   }
};


}
typedef Full2<impl::XDropWhile> DropWhile;
typedef Full2<impl::XDropUntil> DropUntil;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN DropWhile dropWhile;
FCPP_MAYBE_EXTERN DropUntil dropUntil;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XReplicate {
   template <class N, class T>
   struct Sig : public FunType<N,T,OddList<T> > {};

   template <class T>
   OddList<T> operator()( size_t n, const T& x ) const {
      return take( n, repeat(x) );
   }
};
}
typedef Full2<impl::XReplicate> Replicate;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Replicate replicate;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
#ifdef FCPP_SIMPLE_PRELUDE
struct XCycle {
   template <class L>
   struct Sig : public FunType<L,List<typename L::ElementType> > {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
      return cat( l, curry( XCycle(), l ) );
   }
};
#else
struct XCycle {
   template <class L>
   struct Sig : public FunType<L,OddList<typename L::ElementType> > {};

   template <class L>
   OddList<typename L::ElementType> 
   operator()( const L& l, Reuser1<Inv,Inv,XCycle,L> r = NIL ) const {
      return cat( l, r( XCycle(), l ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Cycle");
   }
#endif
};
#endif
}
typedef Full1<impl::XCycle> Cycle;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Cycle cycle;
FCPP_MAYBE_NAMESPACE_CLOSE


namespace impl {
struct XSplitAt {
   template <class N, class L>
   struct Sig : public FunType<N,L,std::pair<List<typename
      L::ElementType>,List<typename L::ElementType> > > {};

   template <class T>
   std::pair<List<T>,List<T> > operator()( size_t n, const List<T>& l ) const {
      if( n==0 || null(l) )
         return std::make_pair( List<T>(), l );
      else {
         std::pair<List<T>,List<T> > temp = XSplitAt()( n-1, tail(l) );
         List<T> tl = cons( head(l), temp.first );
         return std::make_pair( tl, temp.second );
      }
   }
};
}
typedef Full2<impl::XSplitAt> SplitAt;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN SplitAt splitAt;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XSpan {
   template <class P, class L>
   struct Sig : public FunType<P,L,std::pair<List<typename
      L::ElementType>,List<typename L::ElementType> > > {};

   template <class P, class T>
   std::pair<List<T>,List<T> > 
   operator()( const P& p, const List<T>& l ) const {
      if( null(l) || !p(head(l)) )
         return std::make_pair( List<T>(), l );
      else {
         std::pair<List<T>,List<T> > temp = XSpan()( p, tail(l) );
         List<T> tl = cons(head(l),temp.first);
         return std::make_pair( tl, temp.second );
      }
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Span");
   }
#endif
};
}
typedef Full2<impl::XSpan> Span;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Span span;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XBreak {
   template <class P, class L>
   struct Sig : public FunType<P,L,std::pair<List<typename
      L::ElementType>,List<typename L::ElementType> > > {};

   template <class P, class T>
   std::pair<List<T>,List<T> > 
   operator()( const P& p, const List<T>& l ) const {
      return span( Compose()( LogicalNot(), p ), l );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Break");
   }
#endif
};
}
typedef Full2<impl::XBreak> Break;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Break break_; // C++ keyword, so add trailing underscore
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
template <class Binary>
class XFlipHelper {
   Binary op;
public:
   XFlipHelper( const Binary& b ) : op(b) {}

#ifndef FCPP_CONCEPTS
   template <class Y, class X>
   struct Sig : public FunType<Y,X,
      typename Binary::template Sig<X,Y>::ResultType > {};
#endif

   template <class Y, class X>
#ifdef FCPP_CONCEPTS
   typename ConstCallable2<Binary,X,Y>::result_type
#else
   typename Binary::template Sig<X,Y>::ResultType
#endif
   operator()( const Y& y, const X& x ) const {
      return op( x, y );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
      std::string temp;
      if (fcpp::traits::debug_traits<Binary>::has_name) {
        temp +=  std::string("Flip<Binary>::")
             + fcpp::traits::debug_traits<Binary>::name(op);
      } else {
        temp += std::string("Flip<Binary>");
      }
      return temp;
   }
#endif
};
struct XFlip {
#ifndef FCPP_CONCEPTS
   template <class Binary>
   struct Sig : public FunType<Binary,Full2<XFlipHelper<Binary> > > {};
#endif

  template <class Binary>
  Full2<XFlipHelper<Binary> > 
  operator()( const Binary& op ) const {
      return makeFull2( XFlipHelper<Binary>( op ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Flip");
   }
#endif
};
}
typedef Full1<impl::XFlip> Flip;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Flip flip;
FCPP_MAYBE_NAMESPACE_CLOSE

// Operation of reverse. This returns a List<T> rather than OddList<T>.
// Examination with sprint reveals that what is returned is a list
// containing a Fun0 object containing the result list.
// This is because the foldl operation has to work all the way down the list.
// When the head of the list is taken the result is just the result list.
// There are examples of this in understanding_list.cpp.
// This is the only example I have found which uses the cvt struct in list.h
// to do a conversion from List<T> to OddList<T>.
//
// In pattern.h there is a functoid reverse2 which uses a different strategy
// using snoc and foldr which is more truly lazy working out results on demand.
namespace impl {

struct XReverse {
   template <class L>
   struct Sig : public FunType<L,List<typename L::ElementType> > {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
     // Put in for safety. Only called once. Once is enough.
     // This is O.K. as long as the compiler does not
     // optimise it away.
     size_t x = length(l);
     // This does not work with FCPP_TICK and gcc 6.0.1 and 5.2
     // until changes have been made in full.h
     return curry3( foldl, flip(cons), List<T>(), l );
     }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Reverse");
   }
#endif
};

// This version of reverse returns an OddList<T> from the same
// internal operation. This gives a cleaner interface.
// For a more truly lazy version see reverse2 in pattern.h
// which uses snoc (cons backwards!).
struct XReverse1 {
   template <class L>
   struct Sig : public FunType<L,OddList<typename L::ElementType> > {};
   template <class T>
   OddList<T> operator()( const List<T>& l ) const {
     // Put in for safety. Only called once. Once is enough.
     // This is O.K. as long as the compiler does not
     // optimise it away.
     size_t x = length(l);
     // This does not work with FCPP_TICK and gcc 6.0.1 and 5.2
     // until changes have been made in full.h
     List<T> temp =  curry3( foldl, flip(cons), List<T>(), l );
     return temp;
     }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Reverse1");
   }
#endif
};

}

typedef Full1<impl::XReverse> Reverse;
typedef Full1<impl::XReverse1> Reverse1;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Reverse reverse;
FCPP_MAYBE_EXTERN Reverse1 reverse1;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// Not HSP but close
//////////////////////////////////////////////////////////////////////

// These next two are defined as _lazy_ versions of these operators on lists
// For some reason these are a problem on the Arduino.
// It was not the Arduino = the compiler was finding a bug in the previous code.

namespace impl {
struct XAnd : public CFunType<List<bool>,bool> {
   bool operator()( const List<bool>& l ) const {
      return null(dropWhile( equal(true), l ));
   }
};
}
typedef Full1<impl::XAnd> And;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN And and_;
FCPP_MAYBE_NAMESPACE_CLOSE



namespace impl {
struct XOr : public CFunType<List<bool>,bool> {
   bool operator()( const List<bool>& l ) const {
      return !null(dropWhile( equal(false), l )); // There was an extra () here after dropWhile.
   }
};
}
typedef Full1<impl::XOr> Or;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Or or_;
FCPP_MAYBE_NAMESPACE_CLOSE


//////////////////////////////////////////////////////////////////////
// Back to HSP
//////////////////////////////////////////////////////////////////////

namespace impl {
struct XAll {
   template <class P, class L>
   struct Sig : public FunType<P,L,bool> {};

   template <class P, class L>
   bool operator()( const P& p, const L& l ) const {
      return and_( map( p, l ) );
   }
};
}
typedef Full2<impl::XAll> All;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN All all;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XAny {
   template <class P, class L>
   struct Sig : public FunType<P,L,bool> {};

   template <class P, class L>
   bool operator()( const P& p, const L& l ) const {
      return or_( map( p, l ) );
   }
};
}
typedef Full2<impl::XAny> Any;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Any any;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XElem {
   template <class T, class L>
   struct Sig : public FunType<T,L,bool> {};

   template <class T, class L>
   bool operator()( const T& x, const L& l ) const {
      return any( equal(x), l );
   }
};
}
typedef Full2<impl::XElem> Elem;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Elem elem;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XNotElem {
   template <class T, class L>
   struct Sig : public FunType<T,L,bool> {};

   template <class T, class L>
   bool operator()( const T& x, const L& l ) const {
      return all( notEqual(x), l );
   }
};
}
typedef Full2<impl::XNotElem> NotElem;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN NotElem notElem;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XSum {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};
  // Cast of the 0 to the element type added for thunk experiments.
   template <class L>
   typename L::ElementType operator()( const L& l ) const {
     return foldl( plus, (typename L::ElementType)0, l );
   }
};
}
typedef Full1<impl::XSum> Sum;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Sum sum;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XProduct {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& l ) const {
      return foldl( multiplies, 1, l );
   }
};
}
typedef Full1<impl::XProduct> Product;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Product product;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XMinimum {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& l ) const {
      return foldl1( min, l );
   }
};
}
typedef Full1<impl::XMinimum> Minimum;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Minimum minimum;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XMaximum {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& l ) const {
      return foldl1( max, l );
   }
};
}
typedef Full1<impl::XMaximum> Maximum;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Maximum maximum;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
#ifdef FCPP_SIMPLE_PRELUDE
struct XZipWith {
   template <class Z, class LA, class LB>
   struct Sig : public FunType<Z,LA,LB,
   List<typename Z::template Sig<typename LA::ElementType,
                                 typename LB::ElementType>::ResultType> > {};

   template <class Z, class A, class B>
   List<typename RT<Z,A,B>::ResultType> 
   operator()( const Z& z, const List<A>& a, const List<B>& b) const {
      if( null(a) || null(b) )
         return List<typename RT<Z,A,B>::ResultType>();
      else
         return cons( z(head(a),head(b)),
            curry3( XZipWith(), z, tail(a), tail(b) ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("ZipWith");
   }
#endif
};
#else
struct XZipWith {
   template <class Z, class LA, class LB>
   struct Sig : public FunType<Z,LA,LB,
   OddList<typename RT<Z,typename LA::ElementType,
                         typename LB::ElementType>::ResultType> > {};

   template <class Z, class LA, class LB>
   OddList<typename RT<Z,typename LA::ElementType,
                         typename LB::ElementType>::ResultType> 
   operator()( const Z& z, const LA& a, const LB& b,
               Reuser3<Inv,Inv,Var,Var,XZipWith,Z,
                  List<typename LA::ElementType>,
                  List<typename LB::ElementType> > r = NIL ) const {
      if( null(a) || null(b) )
         return NIL;
      else
         return cons( z(head(a),head(b)),
            r( XZipWith(), z, tail(a), tail(b) ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("ZipWith");
   }
#endif
};
#endif
}
typedef Full3<impl::XZipWith> ZipWith;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN ZipWith zipWith;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XZip {
   template <class LA, class LB>
   struct Sig 
   : public FunType<LA,LB,OddList<std::pair<typename LA::ElementType,
                                              typename LB::ElementType> > > {};

   template <class LA, class LB>
   OddList<std::pair<typename LA::ElementType, typename LB::ElementType> >
   operator()( const LA& a, const LB& b ) const {
      return zipWith( MakePair(), a, b );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Zip");
   }
#endif
};
}
typedef Full2<impl::XZip> Zip;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Zip zip;
FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP152
  namespace impl {

  struct XZipWith3 {
  template <class Z, class LA, class LB, class LC>
    struct Sig : public FunType<Z,LA,LB,LC,
   OddList<typename RT<Z,typename LA::ElementType,
                         typename LB::ElementType,
                         typename LC::ElementType>::ResultType> > {};

  template <class Z, class LA, class LB, class LC>
   OddList<typename RT<Z,typename LA::ElementType,
                         typename LB::ElementType,
                         typename LC::ElementType>::ResultType>
     operator()( const Z& z, const LA& a, const LB& b,const LC& c,
                Reuser4<Inv,Inv,Var,Var,Var,XZipWith3,Z,
                  List<typename LA::ElementType>,
                  List<typename LB::ElementType>,
                  List<typename LC::ElementType> > r = NIL ) const {
     if( null(a) || null(b) || null(c) )
         return NIL;
      else
         return cons( z(head(a),head(b),head(c) ),
                r( XZipWith3(), z, tail(a), tail(b), tail(c) ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("ZipWith3");
   }
#endif
  };

struct XZipWith4 {
  template <class Z, class LA, class LB, class LC, class LD>
    struct Sig : public FunType<Z,LA,LB,LC,LD,
   OddList<typename RT<Z,typename LA::ElementType,
                         typename LB::ElementType,
                         typename LC::ElementType,
                         typename LD::ElementType>::ResultType> > {};

  template <class Z, class LA, class LB, class LC, class LD>
   OddList<typename RT<Z,typename LA::ElementType,
                         typename LB::ElementType,
                         typename LC::ElementType,
                         typename LD::ElementType>::ResultType>
    operator()( const Z& z, const LA& a, const LB& b,const LC& c,const LD& d,
               Reuser5<Inv,Inv,Var,Var,Var,Var,XZipWith4,Z,
                  List<typename LA::ElementType>,
                  List<typename LB::ElementType>,
                  List<typename LC::ElementType>,
                  List<typename LD::ElementType> > r = NIL ) const {
    if( null(a) || null(b) || null(c) || null(d) )
        return NIL;
    else
      return cons( z(head(a),head(b),head(c),head(d) ),
                r( XZipWith4(), z, tail(a), tail(b), tail(c), tail(d) ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("ZipWith4");
   }
#endif
};

}
 typedef Full4<impl::XZipWith3> ZipWith3;
 typedef Full5<impl::XZipWith4> ZipWith4;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN ZipWith3 zipWith3;
 FCPP_MAYBE_EXTERN ZipWith4 zipWith4;
 FCPP_MAYBE_NAMESPACE_CLOSE

#endif

#ifdef FCPP_DEBUG
   namespace traits {

template <>
  class debug_traits<impl::XZipWith>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XZipWith &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XZip>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XZip &f) { return f.name(); }
      };

#ifdef FCPP152

template <>
  class debug_traits<impl::XZipWith3>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XZipWith3 &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XZipWith4>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XZipWith4 &f) { return f.name(); }
      };
#endif

   }
#endif


namespace impl {
struct XFst {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Fst");
   }
#endif
   template <class P>
   struct Sig : public FunType<P,typename P::first_type> {};

   template <class A, class B>
   A operator()( const std::pair<A,B>& p ) const {
      return p.first;
   }
};
}
typedef Full1<impl::XFst> Fst;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Fst fst;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XSnd {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Snd");
   }
#endif
   template <class P>
   struct Sig : public FunType<P,typename P::second_type> {};

   template <class A, class B>
   B operator()( const std::pair<A,B>& p ) const {
      return p.second;
   }
};
}
typedef Full1<impl::XSnd> Snd;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Snd snd;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XUnzip {
   template <class LPT>
   struct Sig : public FunType<LPT,std::pair<
      List<typename LPT::ElementType::first_type>,
      List<typename LPT::ElementType::second_type> > > {};

   template <class LPT>
   std::pair<
      List<typename LPT::ElementType::first_type>,
      List<typename LPT::ElementType::second_type> >
   operator()( const LPT& l ) const {
      typedef typename LPT::ElementType::first_type F;
      typedef typename LPT::ElementType::second_type S;
      return std::make_pair( List<F>(curry2(map,fst,l)), 
                             List<S>(curry2(map,snd,l))  );
   }
};
}
typedef Full1<impl::XUnzip> Unzip;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Unzip unzip;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XGcdPrime {
   template <class T, class U> struct Sig;
   template <class T>
   struct Sig<T,T> : public FunType<T,T,T> {};

   template <class T>
   T operator()( T x, T y ) const {
      while( y!=0 ) {
         T tmp( x%y );
         x = y;
         y = tmp;
      }
      return x;
   }
};
struct XGcd {
   template <class T, class U> struct Sig;
   template <class T>
   struct Sig<T,T> : public FunType<T,T,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
#ifdef FCPP_USE_EXCEPTIONS
      if( x==0 && y==0 )
         throw fcpp_exception("Gcd error: x and y both 0");
#endif
      return XGcdPrime()( x<0?-x:x, y<0?-y:y );
   }
};
}
typedef Full2<impl::XGcd> Gcd;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Gcd gcd;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XOdd {
   template <class T>
   struct Sig : public FunType<T,bool> {};

   template <class T>
   bool operator()( const T& x ) const {
      return x%2==1;
   }
};
}
typedef Full1<impl::XOdd> Odd;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Odd odd;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XEven {
   template <class T>
   struct Sig : public FunType<T,bool> {};

   template <class T>
   bool operator()( const T& x ) const {
      return x%2==0;
   }
};
}
typedef Full1<impl::XEven> Even;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Even even;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// Not HSP but close
//////////////////////////////////////////////////////////////////////

// For some unknown reason, g++2.95.2 (for Solaris, at least) generates
// poor code when these next two functoids are templates.  (g++3 does
// fine, regardless.)  As a result, we make them just work with ints,
// unless the user #defines the flag below.
namespace impl {
  // FC++ 1.5.2
  // Change to make this the default.
  //#ifdef FCPP_TEMPLATE_ENUM
#ifndef FCPP_NO_TEMPLATE_ENUM
template <class T>
struct XEFH : public Fun0Impl< OddList<T> > {
   mutable T x;
   XEFH( const T& xx ) : x(xx) {}
   OddList<T> operator()() const {
      ++x;
      return cons( x-1, Fun0<OddList<T> >(1,this) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("EnumFrom( ")
          + argument_traits_value(x)
          + std::string(" )");
   }
#endif
};
struct XEnumFrom {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("EnumFrom");
   }
#endif
   template <class T>
   struct Sig : FunType<T,List<T> > {};

   template <class T>
   List<T> operator()( const T& x ) const {
      return Fun0<OddList<T> >(1, new XEFH<T>(x) );
   }
};
#else
struct XEFH : public Fun0Impl< OddList<int> > {
   mutable int x;
   XEFH( int xx ) : x(xx) {}
   OddList<int> operator()() const {
      ++x;
      return cons( x-1, Fun0<OddList<int> >(1,this) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("EnumFrom");
   }
#endif
};
struct XEnumFrom : CFunType<int,List<int> > {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("EnumFrom");
   }
#endif
   List<int> operator()( int x ) const {
      return Fun0<OddList<int> >(1, new XEFH(x) );
   }
};
#endif
}
typedef Full1<impl::XEnumFrom> EnumFrom;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN EnumFrom enumFrom;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
  // FC++ 1.5.2
  // Change to make this the default.
  //#ifdef FCPP_TEMPLATE_ENUM
#ifndef FCPP_NO_TEMPLATE_ENUM
template <class T>
struct XEFTH : public Fun0Impl<OddList<T> > {
   mutable T x;
   T y;
   XEFTH( const T& xx, const T& yy ) : x(xx), y(yy) {}
   OddList<T> operator()() const {
      if( x > y )
         return NIL;
      ++x;
      return cons( x-1, Fun0<OddList<T> >( 1, this ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("EnumFromTo( ")
          + argument_traits_value(x)
          + std::string(" , ")
          + argument_traits_value(y)
          + std::string(" )");
   }
#endif
};
struct XEnumFromTo {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("EnumFromTo");
   }
#endif
   template <class T, class U> struct Sig;
   template <class T>
   struct Sig<T,T> : FunType<T,T,List<T> > {};

   template <class T>
   List<T> operator()( const T& x, const T& y ) const {
      return Fun0<OddList<T> >( 1, new XEFTH<T>(x,y) );
   }
};
#else
struct XEFTH : public Fun0Impl<OddList<int> > {
   mutable int x;
   int y;
   XEFTH( const int& xx, const int& yy ) : x(xx), y(yy) {}
   OddList<int> operator()() const {
      if( x > y )
         return NIL;
      ++x;
      return cons( x-1, Fun0<OddList<int> >( 1, this ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("EnumFromTo");
   }
#endif
};
struct XEnumFromTo : CFunType<int,int,List<int> > {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("EnumFromTo");
   }
#endif
   List<int> operator()( const int& x, const int& y ) const {
      return Fun0<OddList<int> >( 1, new XEFTH(x,y) );
   }
};
#endif
}
typedef Full2<impl::XEnumFromTo> EnumFromTo;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN EnumFromTo enumFromTo;
FCPP_MAYBE_NAMESPACE_CLOSE


namespace impl {
// Not HSP
struct XListUntil {
   template <class P, class F, class T>
   struct Sig : public FunType<P,F,T,List<T> > {};

   template <class Pred, class Unary, class T>
   List<T> operator()( const Pred& p, const Unary& f, const T& x ) const {
      return takeWhile( Compose()(logicalNot,p), iterate(f,x) );
   }
};
}
typedef Full3<impl::XListUntil> ListUntil;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN ListUntil listUntil;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
// Not HSP, JPF opposite of listUntil
struct XListWhile {
   template <class P, class F, class T>
   struct Sig : public FunType<P,F,T,List<T> > {};

   template <class Pred, class Unary, class T>
   List<T> operator()( const Pred& p, const Unary& f, const T& x ) const {
      return takeWhile( p, iterate(f,x) );
   }
};
}
typedef Full3<impl::XListWhile> ListWhile;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN ListWhile listWhile;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// The "Maybe" type, from Haskell
//////////////////////////////////////////////////////////////////////

struct AUniqueTypeForNothing {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Nothing");
   }
#endif
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN AUniqueTypeForNothing NOTHING;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////////
// MaybeLike is copied from ListLike to check on the arguments of unjust()
//////////////////////////////////////////////////////////////////////////
struct MaybeLike {};

template <class T>
class Maybe  : public MaybeLike {
   OddList<T> rep;
public:
   typedef T ElementType;

   Maybe( AUniqueTypeForNothing ) {}
   Maybe() {}                                    // the Nothing constructor
   Maybe( const T& x ) : rep( cons(x,NIL) ) {}   // the Just constructor

   bool is_nothing() const { return null(rep); }
   // Do not use this if is_nothing() returns true.
   T value() const { return head(rep); }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (is_nothing())
       temp += std::string("NOTHING");
     else {
       temp += std::string("Just[ ");
       if ( debug_traits_has_name<T>() ) {
          temp += debug_traits_name(rep.head());
       } else {
          temp += argument_traits_value(rep.head());
       }
       temp +=  std::string(" ]");
     }
       return temp;
   }
#endif
};


namespace impl {
   struct XJust {
      template <class T> struct Sig : public FunType<T,Maybe<T> > {};
   
      template <class T>
      typename Sig<T>::ResultType
      operator()( const T& x ) const {
         return Maybe<T>( x );
      }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Just");
   }
#endif
   };
}
typedef Full1<impl::XJust> Just;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Just just;
FCPP_MAYBE_NAMESPACE_CLOSE

template <class M, bool b>
struct EnsureMaybeHelp {
   static void trying_to_call_unjust_function_on_a_non_maybe_type() {}
};
template <class M> struct EnsureMaybeHelp<M,false> { };
template <class M>
void EnsureMaybe() {
  //   EnsureListLikeHelp<L,boost::is_base_and_derived<ListLike,L>::value>::
  // I have used the FC++ way of doing this here.
  EnsureMaybeHelp<M,Inherits<M,MaybeLike>::value>::
   trying_to_call_unjust_function_on_a_non_maybe_type();
}

namespace impl {
   struct XUnJust {
      template <class T> struct Sig : public FunType<T,typename T::ElementType> {};
   
      template <class T>
      typename Sig<T>::ResultType
      operator()( const T& x ) const {
         EnsureMaybe<T>();
         return x.value();
      }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("UnJust");
   }
#endif
   };
}
typedef Full1<impl::XUnJust> UnJust;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN UnJust unjust;
FCPP_MAYBE_NAMESPACE_CLOSE


//////////////////////////////////////////////////////////////////////
// The "Either" type, from Haskell
// See Real World Haskell p.452
// This also defines "right" for the correct result.
// This corresponds with "just" for Maybe.
// Like Maybe this is a template type definition not a functoid.
// It can store a value and an error string.
// There is an output operator in the test program.
// Added by John Fletcher (J.P.Fletcher@aston.ac.uk) July 2011
//////////////////////////////////////////////////////////////////////
// This is only the simplest in RWH. There is more to come.
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// EitherLike is copied from ListLike to check on the arguments of unright()
////////////////////////////////////////////////////////////////////////////
struct EitherLike {};

// Wrapper for the error string
// to distinguish it from a string.
  struct EitherError {
private:
  String message_;
public:
  EitherError() { message_ = String("nothing"); }
  EitherError(const EitherError &ee) : message_(ee.message_) {}
  EitherError(const String &m) : message_(m) {}
  // I have made this return std::string for ease of output.
  // There is no constructor of an Either direct from
  // std::string. That was the original problem.
  // When needed it has to be wrapped explictly.
  // See examples in testeither.cpp.
  String operator()() const { return message_; }
};

template <class T>
class Either : public EitherLike {
   // Order reversed to deal with warning.
   OddList<T> rep;
   EitherError error;
public:
   typedef T ElementType;

   Either() {}                       // Default constructor
   Either( AUniqueTypeForNothing ) {} // the Nothing constructor
   // The error constructor which ignores the rep and leaves it null.
   // This means that it can be detected by is_error() in calling program.
   // See usage in DivBy and example in the test program.
 Either( const EitherError &error_message ) : error(error_message) { }
 Either( const T& x, const EitherError &something =
       EitherError(String("success")) )
     : rep( cons(x,NIL) ), error(something) {} // default string supplied
 Either(const Either<T> &e) : rep(e.rep), error(e.error) { }
   void set_error(const EitherError &something) { error = something; }
   bool is_error() const { return null(rep); }
   T right() const { return head(rep); }
   String left() const { return error(); }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (is_error())
       temp += left();
     else {
       temp += std::string("Right[ ");
       if ( debug_traits_has_name<T>() ) {
          temp += debug_traits_name(rep.head());
       } else {
          temp += argument_traits_value(rep.head());
       }
       temp +=  std::string(" ]");
     }
       return temp;
   }
#endif
};

namespace impl {
   struct XRight {
      template <class T> struct Sig : public FunType<T,Either<T> > {};
   
      template <class T>
      typename Sig<T>::ResultType
      operator()( const T& x) const {
         return Either<T>( x );
      }
#ifdef FCPP_DEBUG
      std::string name() const
      {
          return std::string("Right");
      }
#endif
   };
}
typedef Full1<impl::XRight> Right;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Right right;
FCPP_MAYBE_NAMESPACE_CLOSE

template <class E, bool b>
struct EnsureEitherHelp {
   static void trying_to_call_unright_function_on_a_non_either_type() {}
};
template <class E> struct EnsureEitherHelp<E,false> { };
template <class E>
void EnsureEither() {
  //   EnsureListLikeHelp<L,boost::is_base_and_derived<ListLike,L>::value>::
  // I have used the FC++ way of doing this here.
  EnsureEitherHelp<E,Inherits<E,EitherLike>::value>::
   trying_to_call_unright_function_on_a_non_either_type();
}

namespace impl {
   struct XUnRight {
      template <class T> struct Sig : public FunType<T,typename T::ElementType> {};
   
      template <class T>
      typename Sig<T>::ResultType
      operator()( const T& x ) const {
         EnsureEither<T>();
         return x.right();
      }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("UnRight");
   }
#endif
   };
}

typedef Full1<impl::XUnRight> UnRight;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN UnRight unright;
FCPP_MAYBE_NAMESPACE_CLOSE



////////////////////////////////////////////////////////////////////
// I am making this to help make IdentityM into an inferrable monad.
////////////////////////////////////////////////////////////////////
template <class T>
class Identity {
  OddList<T> rep;
 public:
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (is_error())
       temp += std::string("ERROR");
     else {
       temp += std::string("Identity[ ");
       if ( debug_traits_has_name<T>() ) {
          temp += debug_traits_name(rep.head());
       } else {
          temp +=  argument_traits_value(rep.head());
       }
       temp +=  std::string(" ]");
     }
     return temp;
     //return std::string("Identity");
   }
#endif
 typedef T ElementType;
  Identity( AUniqueTypeForNothing ) {}
  Identity() { }
  Identity(const T& t) : rep(cons(t,NIL) ) {}
  T value() const { return head(rep); }
  bool is_error() const { return null(rep); }
  operator T() const { return head(rep); } // needed for transparency.
};

namespace impl {
   struct XIdent {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Ident");
   }
#endif
      template <class T> struct Sig : public FunType<T,Identity<T> > {};
   
      template <class T>
      typename Sig<T>::ResultType
      operator()( const T& x ) const {
         return Identity<T>( x );
      }
   };
}
typedef Full1<impl::XIdent> Ident;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Ident ident;
FCPP_MAYBE_NAMESPACE_CLOSE

// This works as a trait to distinguish an Identity<A>.
// It is used by IdentityInfM monad.
// Default case
template <class A> struct IsIdentity
{
  typedef A Type;
  static A value(const A& a) { return a; }
};
  // Specialised case for Identity<A>
template <class A> struct IsIdentity <Identity<A> >
{
  typedef A Type;
  static A value(const Identity<A>& ia) { return ia.value(); }
};

// Haskell's "()" type/value - described as an empty tuple.
// Now decorated with debug information.
struct Empty {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("()");
   }
#endif
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Empty empty;
FCPP_MAYBE_NAMESPACE_CLOSE

// Haskell has curry() and uncurry() for pairs; we call these hCurry()
// and hUncurry().
namespace impl {
   struct XHCurry {
      template <class F, class X, class Y> struct Sig : public FunType<F,X,Y,
         typename RT<F,std::pair<X,Y> >::ResultType> {};
      template <class F, class X, class Y>
#ifdef FCPP_CONCEPTS
      inline ConstCallable1<F,std::pair<X,Y> >::result_type
#else
      typename Sig<F,X,Y>::ResultType
#endif
      operator()( const F& f, const X& x, const Y& y ) const {
         return f( makePair(x,y) );
      }
   };
}
typedef Full3<impl::XHCurry> HCurry;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN HCurry hCurry;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
   struct XHUncurry {
     //#ifndef FCPP_CONCEPTS
      template <class F, class P> struct Sig : public FunType<F,P,typename 
         RT<F,typename P::first_type,typename P::second_type>::ResultType> {};
     //#endif
      template <class F, class P>
#ifdef FCPP_CONCEPTS
      typename ConstCallable2<F,typename P::first_type,typename P::second_type>::result_type
#else
      typename Sig<F,P>::ResultType
#endif
      operator()( const F& f, const P& p ) const {
         return f( fst(p), snd(p) );
      }
   };
}
typedef Full2<impl::XHUncurry> HUncurry;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN HUncurry hUncurry;
FCPP_MAYBE_NAMESPACE_CLOSE

/////////////////////////////////////////////////////////////////////
// That's the end of the Haskell stuff; on to made-just-for-FC++
/////////////////////////////////////////////////////////////////////

// This is ported from Boost FC++ with appropriate changes of name.
// between(lower,upper)(x)  =  x>=lower && x<=upper
namespace impl {
struct XBetween {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Between");
   }
#endif
   template <class T, class U, class V> struct Sig;
  // Specialization to ensure all arguments are the same type.
   template <class T> struct Sig<T,T,T>
     : public FunType<T,T,T,bool> {};
   template <class T>
   bool operator()( const T& lower, const T& upper, const T& goal ) const {
      return lessEqual(goal,upper) && greaterEqual(goal,lower);
   }
};
}
typedef Full3<impl::XBetween> Between;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Between between;
FCPP_MAYBE_NAMESPACE_CLOSE

 #ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XBetween>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XBetween &f) { return f.name(); }
       };
    }
 #endif

/////////////////////////////////////////////////////////////////////
// The next group are inspired by
// http://patternsinfp.wordpress.com/2010/12/31/stream-monad/
// by Jeremy Gibbons who defines fork:
// fork_(f,g,a)        -> pair(f(a),g(a)) (Note: fork_ not fork)
// fork2(f,g,a,b)      -> pair(f(a,b),g(a,b)) (FC++ 1.5.2 only)
// forkp(pair(f,g), a) -> pair(f(a),g(a))
// The next ones are some I have dreampt up as a result.
// apply(f,a)          -> f(a)           (hardly needed)
// apply2(f,a,b)       -> pair(f(a),f(b))
// applyp(f,pair(a,b)) -> pair(f(a),f(b))
/////////////////////////////////////////////////////////////////////
// Added because of work on contrafunctors
// apply2fg(f,g,a,b)   -> f(g(a),g(b)) (FC++ 1.5.2 only)
// apply2gf(g,f,a,b)   -> f(g(a),g(b)) (FC++ 1.5.2 only)
/////////////////////////////////////////////////////////////////////

namespace impl {
    struct XFork {
      template <class F,class G,class A>
      struct Sig : public FunType<F,G,A,
                   std::pair<typename RT<F,A>::ResultType,
                             typename RT<G,A>::ResultType > > { };

      template <class F,class G,class A>
      typename Sig<F,G,A>::ResultType operator()
              (const F& f,const G& g,const A &a) const
      {
         return std::make_pair(f(a),g(a));
      }
    };
#ifdef FCPP152
    struct XFork2 {
      template <class F,class G,class A,class B>
      struct Sig : public FunType<F,G,A,B,
                   std::pair<typename RT<F,A,B>::ResultType,
                             typename RT<G,A,B>::ResultType > > { };

      template <class F,class G,class A,class B>
      typename Sig<F,G,A,B>::ResultType operator()
      (const F& f,const G& g,const A &a,const B& b) const
      {
         return std::make_pair(f(a,b),g(a,b));
      }
    };
#endif
    struct XForkp {
      template <class P,class A>
      struct Sig : public FunType<P,A,
                   std::pair<typename RT<typename P::first_type,A>::ResultType,
                   typename RT<typename P::second_type,A>::ResultType > > { };

      template <class P,class A>
      typename Sig<P,A>::ResultType operator()
      (const P& p,const A &a) const
      {
         return std::make_pair(p.first(a),p.second(a));
      }
    };
  }
 typedef Full3<impl::XFork> Fork;
#ifdef FCPP152
 typedef Full4<impl::XFork2> Fork2;
#endif
 typedef Full2<impl::XForkp> Forkp;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Fork fork_;
#ifdef FCPP152
 FCPP_MAYBE_EXTERN Fork2 fork2;
#endif
 FCPP_MAYBE_EXTERN Forkp forkp;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
    struct XApply {
      template <class F,class A>
      struct Sig : public FunType<F,A,typename RT<F,A>::ResultType > { };

      template <class F,class A>
      typename Sig<F,A>::ResultType operator()
      (const F& f,const A &a) const
      {
        return f(a);
      }
    };
    struct XApply2 {
      template <class F,class A,class B>
      struct Sig : public FunType<F,A,B,
                   std::pair<typename RT<F,A>::ResultType,
                             typename RT<F,B>::ResultType > > { };

      template <class F,class A,class B>
      typename Sig<F,A,B>::ResultType operator()
      (const F& f,const A &a,const B& b) const
      {
         return std::make_pair(f(a),f(b));
      }
    };
    struct XApplyp {
      template <class F,class P>
      struct Sig : public FunType<F,P,
             std::pair<typename RT<F,typename P::first_type>::ResultType,
                       typename RT<F,typename P::second_type>::ResultType > >
             { };

      template <class F,class P>
      typename Sig<F,P>::ResultType operator()
      (const F& f,const P &p) const
      {
          return std::make_pair(f(p.first),f(p.second));
      }
    };
  
    struct XApply2FG {
      template <class F,class G, class A,class B>
      struct Sig : public FunType<F,G,A,B,
                          typename RT<F,typename RT<G,A>::ResultType,
                          typename RT<G,B>::ResultType >::ResultType >
      { };

      template <class F,class G,class A,class B>
      typename Sig<F,G,A,B>::ResultType operator()
      (const F& f,const G& g, const A &a,const B& b) const
      {
          return f(g(a),g(b));
      }
    };

    struct XApply2GF {
      template <class G,class F, class A,class B>
      struct Sig : public FunType<F,G,A,B,
                   typename RT<F,typename RT<G,A>::ResultType,
                   typename RT<G,B>::ResultType >::ResultType >
      { };

      template <class F,class G,class A,class B>
      typename Sig<G,F,A,B>::ResultType operator()
      (const G& g,const F& f, const A &a,const B& b) const
      {
	return f(g(a),g(b));
      }
    };

}
 typedef Full2<impl::XApply>  Apply;
 typedef Full3<impl::XApply2> Apply2;
 typedef Full2<impl::XApplyp> Applyp;
#ifdef FCPP152
 typedef Full4<impl::XApply2FG> Apply2FG;
 typedef Full4<impl::XApply2GF> Apply2GF;
#endif
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Apply  apply;
 FCPP_MAYBE_EXTERN Apply2 apply2;
 FCPP_MAYBE_EXTERN Applyp applyp;
#ifdef FCPP152
 FCPP_MAYBE_EXTERN Apply2FG apply2fg;
 FCPP_MAYBE_EXTERN Apply2GF apply2gf;
#endif
 FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// Useful effect combinators
//////////////////////////////////////////////////////////////////////
// Here we define some combinators for statement sequencing:
//    before(f,g)(args) = { f(); return g(args); }
//    after(f,g)(args)  = { r = f(args); g(); return r; }
// That is, before() prepends a thunk onto a functoid, and after()
// appends the thunk onto the back of a functoid.  Finally, noOp
// results in a thunk that does nothing, and serves as the left/right
// identity element for before/after thusly:
//    f  =  before( noOp, f )  =  after( f, noOp )
// Note: the effect happens when all of the functoid's expected
// arguments finally arrive (which, thanks to currying, might not be at
// the "next call".  So if g() expects two arguments, note that
//    before( f, g )( x )
// will not call f() now, rather it waits for another argument.

  // The problem here is that NVCC cannot compile CFunType<int>
  // so I have taken noOp and intZero out of use for now.
  // The problem line is this in rt.h (line 119)
  // typedef typename T::template Sig<>::ResultType ResultType;
namespace impl {
struct XNoOp : public CFunType<void> {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("NoOp");
   }
#endif
   void operator()() const {}
};
}
typedef Full0<impl::XNoOp> NoOp;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN NoOp noOp;
FCPP_MAYBE_NAMESPACE_CLOSE

// This is created to help with testing of invcompose.
// It could be useful somewhere else.
// So I have made a couple more for fun.
namespace impl {
struct XIntZero : public CFunType<int> {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("IntZero");
   }
#endif
  int operator()() const { return 0; }
};
struct XIntOne : public CFunType<int> {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("IntOne");
   }
#endif
  int operator()() const { return 1; }
};
struct XIntTwo : public CFunType<int> {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("IntTwo");
   }
#endif
  int operator()() const { return 2; }
};
}
typedef Full0<impl::XIntZero> IntZero;
typedef Full0<impl::XIntOne>  IntOne;
typedef Full0<impl::XIntTwo>  IntTwo;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN  IntZero intZero;
FCPP_MAYBE_EXTERN  IntOne  intOne;
FCPP_MAYBE_EXTERN  IntTwo  intTwo;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
template <class F, class G>
class XBeforer0
#ifndef FCPP_CONCEPTS
: public CFunType<typename RT<G>::ResultType>
#endif
{
   F f;
   G g;
public:
   XBeforer0( const F& ff, const G& gg ) : f(ff), g(gg) {}
#ifdef FCPP_CONCEPTS
  inline ConstCallable0<G>::result_type
#else
   typename RT<G>::ResultType
#endif
   operator()() const { f(); return g(); }
};
template <class F, class G>
class XBeforer1 { 
   F f;
   G g;
public:
   XBeforer1( const F& ff, const G& gg ) : f(ff), g(gg) {}
#ifndef FCPP_CONCEPTS
   template <class X> struct Sig
   : public FunType<typename RT<G,X>::Arg1Type,typename RT<G,X>::ResultType> {};
#endif
   template <class X>
#ifdef FCPP_CONCEPTS
     inline ConstCallable1<G,X>::result_type
#else
   typename Sig<X>::ResultType
#endif
   operator()( const X& x ) const { f(); return g(x); }
};
template <class F, class G>
class XBeforer2 { 
   F f;
   G g;
public:
   XBeforer2( const F& ff, const G& gg ) : f(ff), g(gg) {}
#ifndef FCPP_CONCEPTS
   template <class X, class Y> struct Sig
   : public FunType<typename RT<G,X,Y>::Arg1Type,
                    typename RT<G,X,Y>::Arg2Type,
                    typename RT<G,X,Y>::ResultType> {};
#endif
   template <class X, class Y>
#ifdef FCPP_CONCEPTS
     inline ConstCallable2<G,X,Y>::result_type
#else
   typename Sig<X,Y>::ResultType
#endif
   operator()( const X& x, const Y& y ) const { f(); return g(x,y); }
};
template <class F, class G>
class XBeforer3 { 
   F f;
   G g;
public:
   XBeforer3( const F& ff, const G& gg ) : f(ff), g(gg) {}
#ifndef FCPP_CONCEPTS
   template <class X, class Y, class Z> struct Sig
   : public FunType<typename RT<G,X,Y,Z>::Arg1Type,
                    typename RT<G,X,Y,Z>::Arg2Type,
                    typename RT<G,X,Y,Z>::Arg3Type,
                    typename RT<G,X,Y,Z>::ResultType> {};
#endif
   template <class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
     inline ConstCallable3<G,X,Y,Z>::result_type
#else
   typename Sig<X,Y,Z>::ResultType
#endif 
   operator()( const X& x, const Y& y, const Z& z ) const 
   { f(); return g(x,y,z); }
};

// FC++.1.5.2
#ifdef FCPP152
template <class F, class G>
class XBeforer4 { 
   F f;
   G g;
public:
   XBeforer4( const F& ff, const G& gg ) : f(ff), g(gg) {}
#ifndef FCPP_CONCEPTS
   template <class X, class Y, class Z, class W> struct Sig 
   : public FunType<typename RT<G,X,Y,Z,W>::Arg1Type,
                    typename RT<G,X,Y,Z,W>::Arg2Type,
                    typename RT<G,X,Y,Z,W>::Arg3Type,
                    typename RT<G,X,Y,Z,W>::Arg4Type,
                    typename RT<G,X,Y,Z,W>::ResultType> {};
#endif
   template <class X, class Y, class Z, class W>
#ifdef FCPP_CONCEPTS
     inline ConstCallable4<G,X,Y,Z,W>::result_type
#else
   typename Sig<X,Y,Z,W>::ResultType 
#endif
   operator()( const X& x, const Y& y, const Z& z, const W& w ) const 
   { f(); return g(x,y,z,w); }
};
#endif

class XBefore {
 protected: // Needed for variadic FC++
   template <int i, class F, class G> struct Helper;
   template <class F, class G>
   struct Helper<0,F,G> {
      typedef Full0<XBeforer0<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull0( XBeforer0<F,G>(f,g) ); }
   };
   template <class F, class G>
   struct Helper<1,F,G> {
      typedef Full1<XBeforer1<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull1( XBeforer1<F,G>(f,g) ); }
   };
   template <class F, class G>
   struct Helper<2,F,G> {
      typedef Full2<XBeforer2<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull2( XBeforer2<F,G>(f,g) ); }
   };
   template <class F, class G>
   struct Helper<3,F,G> {
      typedef Full3<XBeforer3<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull3( XBeforer3<F,G>(f,g) ); }
   };
// FC++.1.5.2
#ifdef FCPP152
   template <class F, class G>
   struct Helper<4,F,G> {
      typedef Full4<XBeforer4<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull4( XBeforer4<F,G>(f,g) ); }
   };
#endif
public:
#ifndef FCPP_CONCEPTS
   template <class F, class G> struct Sig : public FunType<F,G,
      typename Helper<FunctoidTraits<G>::max_args,F,G>::Result> {};
#endif
   template <class F, class G>
#ifdef FCPP_CONCEPTS
   inline typename Helper<FunctoidTraits<G>::max_args,F,G>::Result
#else
   typename Sig<F,G>::ResultType
#endif
   operator()( const F& f, const G& g ) const {
      return Helper<FunctoidTraits<G>::max_args,F,G>::go( f, g );
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     return std::string("Before");
   }
#endif
};
}
typedef Full2<impl::XBefore> Before;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Before before;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {

#ifdef FCPP_USE_BOOST_VOID
  template <class F, class G,bool test_void> struct XAfterer0_impl;

  template <class F, class G>
  struct XAfterer0_impl<F,G,true>
  {
    static void do_it(const F& f, const G& g)
    {  f(); g(); }
  };

  template <class F, class G>
  struct XAfterer0_impl<F,G,false>
  {
    // Defining a typename saves multiple #ifdef's within this
    // and some other function bodies.
#ifdef FCPP_CONCEPTS
    typedef typename ConstCallable0<F>::result_type Result_Type;
#else
    typedef typename RT<F>::ResultType Result_Type;
#endif
   static Result_Type do_it(const F& f, const G& g)
   {
     Result_Type tmp = f();
     g();
     return tmp;
    }
  };
#endif

#ifdef FCPP_USE_BOOST_MPL_MESSAGES
  // Diagnostic message
template <typename T>
struct FCPP_ERROR_VOID_RETURN_FROM_f_IN_after_f_g_use_DFCPP_USE_BOOST_VOID
  : boost::is_void<T> {};
#endif

template <class F, class G>
class XAfterer0
#ifndef FCPP_CONCEPTS
: public CFunType<typename RT<F>::ResultType>
#endif
{
   F f;
   G g;
public:
   XAfterer0( const F& ff, const G& gg ) : f(ff), g(gg) {}
#ifdef FCPP_CONCEPTS
    typedef typename ConstCallable0<F>::result_type Result_Type;
#else
    typedef typename RT<F>::ResultType Result_Type;
#endif

  inline Result_Type 
  operator()() const 
  { 
    // This test checks if the return type of the first function is void.
    // If so, the <true> version of XAfterer0_impl is used to skip the failure
    // caused by attempting to store a void intermediate result.
    // Otherwise, the <false> version follows the previous method.
#ifdef FCPP_USE_BOOST_VOID
    return XAfterer0_impl<F,G,
           boost::is_void<Result_Type>::value >::do_it(f,g);
#else
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
    // This can be used for diagnosis of the problem.
    // Call to diagnostic.
    BOOST_MPL_ASSERT_NOT((FCPP_ERROR_VOID_RETURN_FROM_f_IN_after_f_g_use_DFCPP_USE_BOOST_VOID<Result_Type>));
#endif
    // The previous way of doing this which is still the default.
     Result_Type tmp = f();
     g();
     return tmp;
#endif
  }
};

#ifdef FCPP_USE_BOOST_VOID
  template <class F, class G, class X,bool test_void> struct XAfterer1_impl;

  template <class F, class G, class X>
  struct XAfterer1_impl<F,G,X,true>
  {
    static void do_it(const F& f, const G& g, const X& x)
    {  f(x); g(); }
  };

  template <class F, class G, class X>
    struct XAfterer1_impl<F,G,X,false>
  {
#ifdef FCPP_CONCEPTS
    typedef typename ConstCallable1<F,X>::result_type Result_Type;
#else
    typedef typename RT<F,X>::ResultType Result_Type;
#endif
   static Result_Type do_it(const F& f, const G& g, const X& x)
   {
     Result_Type tmp = f(x);
     g();
     return tmp;
    }
  };
#endif

template <class F, class G>
class XAfterer1 { 
   F f;
   G g;
public:
   XAfterer1( const F& ff, const G& gg ) : f(ff), g(gg) {}
#ifndef FCPP_CONCEPTS
   template <class X> struct Sig
   : public FunType<typename RT<F,X>::Arg1Type,
                    typename RT<F,X>::ResultType> {};
#endif
   template <class X>
#ifdef FCPP_CONCEPTS
    inline typename ConstCallable1<F,X>::result_type
#else
    inline typename RT<F,X>::ResultType
#endif
   operator()( const X& x ) const
   {
#ifdef FCPP_CONCEPTS
    typedef typename ConstCallable1<F,X>::result_type Result_Type;
#else
    typedef typename RT<F,X>::ResultType Result_Type;
#endif
#ifdef FCPP_USE_BOOST_VOID
     return XAfterer1_impl<F,G,X,
       boost::is_void<Result_Type>::value >::do_it(f,g,x);
#else
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
    // This can be used for diagnosis of the problem.
    // Call to diagnostic.
    BOOST_MPL_ASSERT_NOT((FCPP_ERROR_VOID_RETURN_FROM_f_IN_after_f_g_use_DFCPP_USE_BOOST_VOID<Result_Type>));
#endif
      Result_Type tmp = f(x); g(); return tmp;
#endif
   }
};

#ifdef FCPP_USE_BOOST_VOID
 template <class F, class G, class X,class Y,bool test_void> struct XAfterer2_impl;

 template <class F, class G, class X, class Y>
   struct XAfterer2_impl<F,G,X,Y,true>
  {
    static void do_it(const F& f, const G& g, const X& x, const Y& y)
    {  f(x,y); g(); }
  };

 template <class F, class G, class X, class Y>
   struct XAfterer2_impl<F,G,X,Y,false>
  {
#ifdef FCPP_CONCEPTS
    typedef typename ConstCallable2<F,X,Y>::result_type Result_Type;
#else
    typedef typename RT<F,X,Y>::ResultType Result_Type;
#endif
   static Result_Type do_it(const F& f, const G& g, const X& x, const Y& y)
   {
     Result_Type tmp = f(x,y);
     g();
     return tmp;
    }
  };
#endif

template <class F, class G>
class XAfterer2 { 
   F f;
   G g;
public:
   XAfterer2( const F& ff, const G& gg ) : f(ff), g(gg) {}
#ifndef FCPP_CONCEPTS
   template <class X, class Y> struct Sig
   : public FunType<typename RT<F,X,Y>::Arg1Type,
                    typename RT<F,X,Y>::Arg2Type,
                    typename RT<F,X,Y>::ResultType> {};
#endif
   template <class X, class Y>
#ifdef FCPP_CONCEPTS
     inline typename ConstCallable2<F,X,Y>::result_type
#else
   typename Sig<X,Y>::ResultType
#endif
   operator()( const X& x, const Y& y ) const
   {
#ifdef FCPP_CONCEPTS
     typedef typename ConstCallable2<F,X,Y>::result_type Result_Type;
#else
    typedef typename Sig<X,Y>::ResultType Result_Type;
#endif
#ifdef FCPP_USE_BOOST_VOID
    return XAfterer2_impl<F,G,X,Y,
       boost::is_void<Result_Type>::value >::do_it(f,g,x,y);
#else
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
    // This can be used for diagnosis of the problem.
    // Call to diagnostic.
    BOOST_MPL_ASSERT_NOT((FCPP_ERROR_VOID_RETURN_FROM_f_IN_after_f_g_use_DFCPP_USE_BOOST_VOID<Result_Type>));
#endif
     Result_Type tmp = f(x,y); g(); return tmp;
#endif
   }
};

#ifdef FCPP_USE_BOOST_VOID
 template <class F, class G, class X,class Y,class Z,bool test_void> struct XAfterer3_impl;

 template <class F, class G, class X, class Y, class Z>
   struct XAfterer3_impl<F,G,X,Y,Z,true>
  {
    static void do_it(const F& f, const G& g, const X& x, const Y& y, const Z& z)
    {  f(x,y,z); g(); }
  };

 template <class F, class G, class X, class Y, class Z>
   struct XAfterer3_impl<F,G,X,Y,Z,false>
  {
#ifdef FCPP_CONCEPTS
    typedef typename ConstCallable3<F,X,Y,Z>::result_type Result_Type;
#else
    typedef typename RT<F,X,Y,Z>::ResultType Result_Type;
#endif
   static Result_Type do_it(const F& f, const G& g, const X& x, const Y& y, const Z& z)
   {
     Result_Type tmp = f(x,y,z);
     g();
     return tmp;
    }
  };
#endif

template <class F, class G>
class XAfterer3 { 
   F f;
   G g;
public:
   XAfterer3( const F& ff, const G& gg ) : f(ff), g(gg) {}
#ifndef FCPP_CONCEPTS
   template <class X, class Y, class Z> struct Sig
   : public FunType<typename RT<F,X,Y,Z>::Arg1Type,
                    typename RT<F,X,Y,Z>::Arg2Type,
                    typename RT<F,X,Y,Z>::Arg3Type,
                    typename RT<F,X,Y,Z>::ResultType> {};
#endif
   template <class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
     inline typename ConstCallable3<F,X,Y,Z>::result_type
#else
   typename Sig<X,Y,Z>::ResultType
#endif
   operator()( const X& x, const Y& y, const Z& z ) const
   {
#ifdef FCPP_CONCEPTS
     typedef typename ConstCallable3<F,X,Y,Z>::result_type Result_Type;
#else
     typedef typename Sig<X,Y,Z>::ResultType Result_Type;
#endif
#ifdef FCPP_USE_BOOST_VOID
     return XAfterer3_impl<F,G,X,Y,Z,
       boost::is_void<Result_Type>::value >::do_it(f,g,x,y,z);
#else
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
    // This can be used for diagnosis of the problem.
    // Call to diagnostic.
    BOOST_MPL_ASSERT_NOT((FCPP_ERROR_VOID_RETURN_FROM_f_IN_after_f_g_use_DFCPP_BOOST_VOID<Result_Type>));
#endif
     Result_Type tmp = f(x,y,z); g(); return tmp;
#endif
   }
};
// FC++.1.5.2
#ifdef FCPP152

#ifdef FCPP_USE_BOOST_VOID
 template <class F, class G, class X,class Y,class Z,class W,bool test_void> 
 struct XAfterer4_impl;

 template <class F, class G, class X, class Y, class Z, class W>
   struct XAfterer4_impl<F,G,X,Y,Z,W,true>
  {
    static void do_it(const F& f, const G& g, const X& x, const Y& y, const Z& z, const W& w)
    {  f(x,y,z,w); g(); }
  };

 template <class F, class G, class X, class Y, class Z, class W>
   struct XAfterer4_impl<F,G,X,Y,Z,W,false>
  {
#ifdef FCPP_CONCEPTS
    typedef typename ConstCallable4<F,X,Y,Z,W>::result_type Result_Type;
#else
    typedef typename RT<F,X,Y,Z,W>::ResultType Result_Type;
#endif
   static Result_Type do_it(const F& f, const G& g, const X& x, const Y& y, const Z& z, const W& w)
   {
     Result_Type tmp = f(x,y,z,w);
     g();
     return tmp;
    }
  };
#endif

template <class F, class G>
class XAfterer4 { 
   F f;
   G g;
public:
   XAfterer4( const F& ff, const G& gg ) : f(ff), g(gg) {}
#ifndef FCPP_CONCEPTS
   template <class X, class Y, class Z, class W> struct Sig 
   : public FunType<typename RT<F,X,Y,Z,W>::Arg1Type,
                    typename RT<F,X,Y,Z,W>::Arg2Type,
                    typename RT<F,X,Y,Z,W>::Arg3Type,
                    typename RT<F,X,Y,Z,W>::Arg4Type,
                    typename RT<F,X,Y,Z,W>::ResultType> {};
#endif
   template <class X, class Y, class Z, class W>
#ifdef FCPP_CONCEPTS
     inline typename ConstCallable4<F,X,Y,Z,W>::result_type
#else
   typename Sig<X,Y,Z,W>::ResultType 
#endif
   operator()( const X& x, const Y& y, const Z& z, const W& w ) const 
   { 
#ifdef FCPP_CONCEPTS
     typedef typename ConstCallable4<F,X,Y,Z,W>::result_type Result_Type;
#else
     typedef typename Sig<X,Y,Z,W>::ResultType Result_Type;
#endif
#ifdef FCPP_USE_BOOST_VOID
     return XAfterer4_impl<F,G,X,Y,Z,W,
       boost::is_void<Result_Type>::value >::do_it(f,g,x,y,z,w);
#else
#ifdef FCPP_USE_BOOST_MPL_MESSAGES
    // This can be used for diagnosis of the problem.
    // Call to diagnostic.
    BOOST_MPL_ASSERT_NOT((FCPP_ERROR_VOID_RETURN_FROM_f_IN_after_f_g_use_DFCPP_BOOST_VOID<Result_Type>));
#endif
     Result_Type tmp = f(x,y,z,w); g(); return tmp;
#endif
   }
};
#endif
class XAfter {
 protected:  // Needed for variadic FC++
   template <int i, class F, class G> struct Helper;
   template <class F, class G>
   struct Helper<0,F,G> {
      typedef Full0<XAfterer0<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull0( XAfterer0<F,G>(f,g) ); }
   };
   template <class F, class G>
   struct Helper<1,F,G> {
      typedef Full1<XAfterer1<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull1( XAfterer1<F,G>(f,g) ); }
   };
   template <class F, class G>
   struct Helper<2,F,G> {
      typedef Full2<XAfterer2<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull2( XAfterer2<F,G>(f,g) ); }
   };
   template <class F, class G>
   struct Helper<3,F,G> {
      typedef Full3<XAfterer3<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull3( XAfterer3<F,G>(f,g) ); }
   };
// FC++.1.5.2
#ifdef FCPP152
   template <class F, class G>
   struct Helper<4,F,G> {
      typedef Full4<XAfterer4<F,G> > Result;
      static Result go( const F& f, const G& g )
      { return makeFull4( XAfterer4<F,G>(f,g) ); }
   };
#endif
public:
#ifndef FCPP_CONCEPTS
   template <class F, class G> struct Sig : public FunType<F,G,
      typename Helper<FunctoidTraits<F>::max_args,F,G>::Result> {};
#endif
   template <class F, class G>
#ifdef FCPP_CONCEPTS
   inline typename Helper<FunctoidTraits<F>::max_args,F,G>::Result
#else
   typename Sig<F,G>::ResultType 
#endif
   operator()( const F& f, const G& g ) const 
   {
      return Helper<FunctoidTraits<F>::max_args,F,G>::go( f, g );
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     return std::string("After");
   }
#endif
};
}
typedef Full2<impl::XAfter> After;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN After after;
FCPP_MAYBE_NAMESPACE_CLOSE

// emptify(f)(yadda) == f(yadda), but throws away f's result (even if
//    it's void) and returns Empty instead.  This is useful because
//       length( map( emptify(effectFunctoid), someList ) )
//    is an easy way to do something with each element of someList.
namespace impl {
template <class F>
class XEmptifier0 : public CFunType<Empty> {
   F f;
public:
   XEmptifier0( const F& ff ) : f(ff) {}
   Empty operator()() const { f(); return Empty(); }
};
template <class F>
class XEmptifier1 { 
   F f;
public:
   XEmptifier1( const F& ff ) : f(ff) {}
   template <class X> struct Sig 
   : public FunType<typename RT<F,X>::Arg1Type,Empty> {};
   template <class X>
   typename Sig<X>::ResultType 
   operator()( const X& x ) const { f(x); return Empty(); }
};
template <class F>
class XEmptifier2 { 
   F f;
public:
   XEmptifier2( const F& ff ) : f(ff) {}
   template <class X, class Y> struct Sig 
   : public FunType<typename RT<F,X,Y>::Arg1Type,
                    typename RT<F,X,Y>::Arg2Type,
                    Empty> {};
   template <class X, class Y>
   typename Sig<X,Y>::ResultType 
   operator()( const X& x, const Y& y ) const { f(x,y); return Empty(); }
};
template <class F>
class XEmptifier3 { 
   F f;
public:
   XEmptifier3( const F& ff ) : f(ff) {}
   template <class X, class Y, class Z> struct Sig 
   : public FunType<typename RT<F,X,Y,Z>::Arg1Type,
                    typename RT<F,X,Y,Z>::Arg2Type,
                    typename RT<F,X,Y,Z>::Arg3Type,
                    Empty> {};
   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType 
   operator()( const X& x, const Y& y, const Z& z ) const 
   { f(x,y,z); return Empty(); }
};
// FC++.1.5.2
#ifdef FCPP152
template <class F>
class XEmptifier4 { 
   F f;
public:
   XEmptifier4( const F& ff ) : f(ff) {}
   template <class X, class Y, class Z, class W> struct Sig 
   : public FunType<typename RT<F,X,Y,Z,W>::Arg1Type,
                    typename RT<F,X,Y,Z,W>::Arg2Type,
                    typename RT<F,X,Y,Z,W>::Arg3Type,
                    typename RT<F,X,Y,Z,W>::Arg4Type,
                    Empty> {};
   template <class X, class Y, class Z, class W>
   typename Sig<X,Y,Z,W>::ResultType 
   operator()( const X& x, const Y& y, const Z& z, const W& w ) const 
   { f(x,y,z,w); return Empty(); }
};
#endif
class XEmptify {
   template <int i, class F> struct Helper;
   template <class F> struct Helper<0,F> {
      typedef Full0<XEmptifier0<F> > Result;
      static Result go( const F& f ) 
      { return makeFull0( XEmptifier0<F>(f) ); }
   };
   template <class F> struct Helper<1,F> {
      typedef Full1<XEmptifier1<F> > Result;
      static Result go( const F& f ) 
      { return makeFull1( XEmptifier1<F>(f) ); }
   };
   template <class F> struct Helper<2,F> {
      typedef Full2<XEmptifier2<F> > Result;
      static Result go( const F& f ) 
      { return makeFull2( XEmptifier2<F>(f) ); }
   };
   template <class F> struct Helper<3,F> {
      typedef Full3<XEmptifier3<F> > Result;
      static Result go( const F& f ) 
      { return makeFull3( XEmptifier3<F>(f) ); }
   };
// FC++.1.5.2
#ifdef FCPP152
   template <class F> struct Helper<4,F> {
      typedef Full4<XEmptifier4<F> > Result;
      static Result go( const F& f ) 
      { return makeFull4( XEmptifier4<F>(f) ); }
   };
#endif
public:
   template <class F> struct Sig : public FunType<F,
      typename Helper<FunctoidTraits<F>::max_args,F>::Result> {};
   template <class F>
   typename Sig<F>::ResultType operator()( const F& f ) const {
      return Helper<FunctoidTraits<F>::max_args,F>::go( f );
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     return std::string("Emptify");
   }
#endif
 };
}
typedef Full1<impl::XEmptify> Emptify;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Emptify emptify;
FCPP_MAYBE_NAMESPACE_CLOSE

#ifndef FCPP_I_AM_GCC2
namespace impl {
struct XThunkFuncToFunc {
   ///////////////////////////////////////////////////////////////////////
   // tftf(f)(yadda) = f()(yadda)
   // f is a thunk that returns a functoid; this combinator 'hides' the thunk.
   // can be useful to break what would otherwise be infinite recursion.
   ///////////////////////////////////////////////////////////////////////
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("ThunkFuncToFunc");
   }
#endif
  template <class F> class TFTFH0 /* public was missing in next line */
   : public CFunType<typename RT<typename RT<F>::ResultType>::ResultType> {
      F f;
   public:
      TFTFH0( const F& ff ) : f(ff) {}
      //template <class X> **BUG** this is incorrect here.
      typename RT<typename RT<F>::ResultType>::ResultType
      operator()() const {
         return f()();
      }
#ifdef FCPP_DEBUG
   std::string name() const
   {
      return debug_string_name_and_string_or_string("TFTFH0::(",f,")","TFTFH0");
   }
#endif
   };
   template <class F> class TFTFH1 {
      F f;
   public:
      TFTFH1( const F& ff ) : f(ff) {}
      template <class X> struct Sig : public FunType<
         typename RT<typename RT<F>::ResultType,X>::Arg1Type,
         typename RT<typename RT<F>::ResultType,X>::ResultType   > {};
      template <class X>
      typename Sig<X>::ResultType
      operator()( const X& x ) const {
         return f()( x );
      }
#ifdef FCPP_DEBUG
   std::string name() const
   {
      return debug_string_name_and_string_or_string("TFTFH1::(",f,")","TFTFH1");
   }
#endif
   };
   template <class F> class TFTFH2 {
      F f;
   public:
      TFTFH2( const F& ff ) : f(ff) {}
      template <class X, class Y> struct Sig : public FunType<
         typename RT<typename RT<F>::ResultType,X,Y>::Arg1Type,
         typename RT<typename RT<F>::ResultType,X,Y>::Arg2Type,
         typename RT<typename RT<F>::ResultType,X,Y>::ResultType   > {};
      template <class X, class Y>
      typename Sig<X,Y>::ResultType
      operator()( const X& x, const Y& y ) const {
         return f()( x, y );
      }
#ifdef FCPP_DEBUG
   std::string name() const
   {
      return debug_string_name_and_string_or_string("TFTFH2::(",f,")","TFTFH2");
   }
#endif
   };
   template <class F> class TFTFH3 {
      F f;
   public:
      TFTFH3( const F& ff ) : f(ff) {}
      template <class X, class Y, class Z> struct Sig : public FunType<
         typename RT<typename RT<F>::ResultType,X,Y,Z>::Arg1Type,
         typename RT<typename RT<F>::ResultType,X,Y,Z>::Arg2Type,
         typename RT<typename RT<F>::ResultType,X,Y,Z>::Arg3Type,
         typename RT<typename RT<F>::ResultType,X,Y,Z>::ResultType   > {};
      template <class X, class Y, class Z>
      typename Sig<X,Y,Z>::ResultType
      operator()( const X& x, const Y& y, const Z& z ) const {
         return f()( x, y, z );
      }
#ifdef FCPP_DEBUG
   std::string name() const
   {
      return debug_string_name_and_string_or_string("TFTFH3::(",f,")","TFTFH3");
   }
#endif
   };
// FC++.1.5.2
#ifdef FCPP152
   template <class F> class TFTFH4 {
      F f;
   public:
      TFTFH4( const F& ff ) : f(ff) {}
      template <class X, class Y, class Z, class W> struct Sig :
      public FunType<
         typename RT<typename RT<F>::ResultType,X,Y,Z,W>::Arg1Type,
         typename RT<typename RT<F>::ResultType,X,Y,Z,W>::Arg2Type,
         typename RT<typename RT<F>::ResultType,X,Y,Z,W>::Arg3Type,
         typename RT<typename RT<F>::ResultType,X,Y,Z,W>::Arg4Type,
         typename RT<typename RT<F>::ResultType,X,Y,Z,W>::ResultType   > {};
      template <class X, class Y, class Z, class W>
      typename Sig<X,Y,Z,W>::ResultType
      operator()( const X& x, const Y& y, const Z& z, const W& w ) const {
         return f()( x, y, z, w );
      }
#ifdef FCPP_DEBUG
   std::string name() const
   {
      return debug_string_name_and_string_or_string("TFTFH4::(",f,")","TFTFH4");
   }
#endif
   };
   template <class F> class TFTFH5 {
      F f;
   public:
      TFTFH5( const F& ff ) : f(ff) {}
      template <class X, class Y, class Z, class W, class V> struct Sig :
      public FunType<
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V>::Arg1Type,
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V>::Arg2Type,
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V>::Arg3Type,
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V>::Arg4Type,
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V>::Arg5Type,
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V>::ResultType   > {};
      template <class X, class Y, class Z, class W, class V>
	typename Sig<X,Y,Z,W,V>::ResultType
      operator()( const X& x, const Y& y, const Z& z, const W& w, const V& v ) const {
	return f()( x, y, z, w, v );
      }
#ifdef FCPP_DEBUG
   std::string name() const
   {
      return debug_string_name_and_string_or_string("TFTFH5::(",f,")","TFTFH5");
   }
#endif
   };
   template <class F> class TFTFH6 {
      F f;
   public:
      TFTFH6( const F& ff ) : f(ff) {}
      template <class X, class Y, class Z, class W, class V, class U> struct Sig :
      public FunType<
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V,U>::Arg1Type,
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V,U>::Arg2Type,
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V,U>::Arg3Type,
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V,U>::Arg4Type,
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V,U>::Arg5Type,
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V,U>::Arg6Type,
	typename RT<typename RT<F>::ResultType,X,Y,Z,W,V,U>::ResultType   > {};
      template <class X, class Y, class Z, class W, class V, class U>
	typename Sig<X,Y,Z,W,V,U>::ResultType
      operator()( const X& x, const Y& y, const Z& z, const W& w, const V& v, const U& u ) const {
	return f()( x, y, z, w, v, u );
      }
#ifdef FCPP_DEBUG
   std::string name() const
   {
      return debug_string_name_and_string_or_string("TFTFH6::(",f,")","TFTFH6");
   }
#endif
   };
#endif
   template <int i, class F> struct Helper;
   template <class F> struct Helper<0,F> {
      typedef Full0<TFTFH0<F> > Result;
      static Result go( const F& f )
      { return makeFull0( TFTFH0<F>(f) ); }
   };
   template <class F> struct Helper<1,F> {
      typedef Full1<TFTFH1<F> > Result;
      static Result go( const F& f )
      { return makeFull1( TFTFH1<F>(f) ); }
   };
   template <class F> struct Helper<2,F> {
      typedef Full2<TFTFH2<F> > Result;
      static Result go( const F& f )
      { return makeFull2( TFTFH2<F>(f) ); }
   };
   template <class F> struct Helper<3,F> {
      typedef Full3<TFTFH3<F> > Result;
      static Result go( const F& f )
      { return makeFull3( TFTFH3<F>(f) ); }
   };
// FC++.1.5.2
#ifdef FCPP152
   template <class F> struct Helper<4,F> {
      typedef Full4<TFTFH4<F> > Result;
      static Result go( const F& f )
      { return makeFull4( TFTFH4<F>(f) ); }
   };
   template <class F> struct Helper<5,F> {
      typedef Full5<TFTFH5<F> > Result;
      static Result go( const F& f )
      { return makeFull5( TFTFH5<F>(f) ); }
   };
   template <class F> struct Helper<6,F> {
      typedef Full6<TFTFH6<F> > Result;
      static Result go( const F& f )
      { return makeFull6( TFTFH6<F>(f) ); }
   };
#endif
   template <class F> struct Sig : public FunType<F, typename Helper<
      FunctoidTraits<typename RT<F>::ResultType>::max_args,F>::Result> {};
   template <class F>
   typename Sig<F>::ResultType
   operator()( const F& f ) const {
      return Helper<FunctoidTraits<typename RT<F>::ResultType>
         ::max_args,F>::go(f);
   }
};
}
typedef Full1<impl::XThunkFuncToFunc> ThunkFuncToFunc;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN ThunkFuncToFunc thunkFuncToFunc, tFTF;
FCPP_MAYBE_NAMESPACE_CLOSE
#endif

//////////////////////////////////////////////////////////////////////
// uncurry
//////////////////////////////////////////////////////////////////////
// Sometimes FC++ expressions result in functoids where, for example,
//    f(x)(y)
// is legal but
//    f(x,y)
// is not, owing to the fact that (in the example) f() is a one-argument
// functoid the returns another one-argument functoid, which is 
// different from a two-argument functoid.  (In Haskell, the two types
// are identical.)  uncurry() wraps a functoid in a magical cloak which
// splits up its arguments, so that, for example,
//    uncurry(f)(x,y,z) = f(x)(y)(z)
// It rarely arises that you need this, but when you do, you can't live
// without it.
//
// FIX THIS: Note that uncurry() (as well as curryN()) means something
// different in FC++ than what it does in Haskell.
// Renamed as split_args in Boost FC++, so splitArgs added as a synonym here.
// NOTE: uncurry has not been extended beyond three arguments.
//////////////////////////////////////////////////////////////////////

namespace impl {
template <class F>
class XUncurryable : public SmartFunctoid3 {
   F f;
public:
   XUncurryable( const F& ff ) : f(ff) { 
      FunctoidTraits<F>::template ensure_accepts<1>::args();
   }

   template <class X, class Y=Void, class Z=Void, class Dummy=Void>
   struct Sig : public FunType<typename RT<F,X>::Arg1Type,
            typename RT<typename RT<F,X>::ResultType,Y>::Arg1Type,
typename RT<typename RT<typename RT<F,X>::ResultType,Y>::ResultType,Z>
   ::Arg1Type,
typename RT<typename RT<typename RT<F,X>::ResultType,Y>::ResultType,Z>
   ::ResultType> {};

   template <class X, class Y, class Dummy>
   struct Sig<X,Y,Void,Dummy> : public FunType<typename RT<F,X>::Arg1Type,
            typename RT<typename RT<F,X>::ResultType,Y>::Arg1Type,
            typename RT<typename RT<F,X>::ResultType,Y>::ResultType> {};

   template <class X, class Dummy>
   struct Sig<X,Void,Void,Dummy> : public FunType<typename RT<F,X>::Arg1Type,
            typename RT<F,X>::ResultType> {};

   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType
   operator()( const X& x, const Y& y, const Z& z ) const { return f(x)(y)(z); }

   template <class X, class Y>
   typename Sig<X,Y>::ResultType
   operator()( const X& x, const Y& y ) const { return f(x)(y); }

   template <class X>
   typename Sig<X>::ResultType
   operator()( const X& x ) const { return f(x); }
#ifdef FCPP_ENABLE_LAMBDA
   typedef XUncurryable This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Uncurryable::",f,"Uncurryable");
   }
#endif
};
struct XUncurry {
   template <class F>
   struct Sig : FunType<F,XUncurryable<F> > {};

   template <class F>
   XUncurryable<F> operator()( const F& f ) const { return XUncurryable<F>(f); }
#ifdef FCPP_DEBUG
   std::string name() const {
     return std::string("Uncurry");
   }
#endif
};

}
typedef Full1<impl::XUncurry> Uncurry;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Uncurry uncurry, splitArgs;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// Uncurry0 is truly a different case from uncurry:
//    uncurry0(f)(x,y,z) = f()(x)(y)(z)
//////////////////////////////////////////////////////////////////////
namespace impl {
template <class F>
  class XUncurryable0 : public SmartFunctoid1 { //SmartFunctoid1 was missing.
   F f;
public:
   XUncurryable0( const F& ff ) : f(ff) {
      FunctoidTraits<F>::template ensure_accepts<0>::args();
   }

   template <class X> struct Sig
   : public FunType<typename RT<typename RT<F>::ResultType,X>::Arg1Type,
            typename RT<typename RT<F>::ResultType,X>::ResultType> {};

   template <class X>
   typename Sig<X>::ResultType
   operator()( const X& x ) const { return f()(x); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<F>() )
        return std::string("Uncurryable0::")+debug_traits_name(f);
      else
        return std::string("Uncurryable0");
   }
#endif
};
struct XUncurry0 {
   template <class F>
   struct Sig : FunType<F,XUncurryable<XUncurryable0<F> > > {};

   template <class F>
   XUncurryable<XUncurryable0<F> > operator()( const F& f ) const
   { return ::fcpp::uncurry( XUncurryable0<F>(f) ); }
#ifdef FCPP_DEBUG
   std::string name() const {
     return std::string("Uncurry0");
   }
#endif
};
}
typedef Full1<impl::XUncurry0> Uncurry0;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Uncurry0 uncurry0;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// duplicate() and ignore()
//////////////////////////////////////////////////////////////////////
// duplicate() duplicates the first argument of a functoid, whereas
// ignore() ignores it:
//    duplicate(f)(x)    = f(x)(x)
//    ignore(f)(x)(args) = f(args)
//    ignore2(f)(x,y)(args) = f(x,args) must have at least 1 arg.

namespace impl {
template <class F>
class XDuplicater {
   F f;
public:
   XDuplicater( const F& ff ) : f(ff) {}

   template <class X>
   struct Sig : public FunType<X,typename RT<typename RT<F,X>::ResultType,
                                             X>::ResultType> {};
   template <class X>
   typename Sig<X>::ResultType
   operator()( const X& x ) const {
      return f(x)(x);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return debug_string_and_name_or_string("Duplicater::",f,"Duplicater");
   }
#endif
};
struct XDuplicate {
   template <class F>
   struct Sig : public FunType<F,Full1<XDuplicater<F> > > {};

   template <class F>
   Full1<XDuplicater<F> > operator()( const F& f ) const {
      return makeFull1( XDuplicater<F>(f) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Duplicate");
   }
#endif
};
}
typedef Full1<impl::XDuplicate> Duplicate;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Duplicate duplicate;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
template <class F>
class XIgnorer1 {
   F f;
public:
   XIgnorer1( const F& ff ) : f(ff) {}

   template <class X>
   struct Sig : public FunType<X,typename RT<F>::ResultType> {};

   template <class X>
   typename Sig<X>::ResultType operator()( const X& ) const {
      return f();
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return debug_string_and_name_or_string("Ignorer1::",f,"Ignorer1");
   }
#endif
};
template <class F>
class XIgnorer2 {
   F f;
public:
   XIgnorer2( const F& ff ) : f(ff) {}

   template <class X, class Y>
   struct Sig : public FunType<X,
      typename RT<F,Y>::Arg1Type,typename RT<F,Y>::ResultType> {};

   template <class X, class Y>
   typename Sig<X,Y>::ResultType
   operator()( const X&, const Y& y ) const {
      return f(y);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return debug_string_and_name_or_string("Ignorer2::",f,"Ignorer2");
   }
#endif
};
template <class F>
class XIgnorer3 {
   F f;
public:
   XIgnorer3( const F& ff ) : f(ff) {}

   template <class X, class Y, class Z>
   struct Sig : public FunType<X,
      typename RT<F,Y,Z>::Arg1Type,
      typename RT<F,Y,Z>::Arg2Type,
      typename RT<F,Y,Z>::ResultType> {};

   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType
   operator()( const X&, const Y& y, const Z& z ) const {
      return f(y,z);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return debug_string_and_name_or_string("Ignorer3::",f,"Ignorer3");
   }
#endif
};
// FC++.1.5.2
#ifdef FCPP152
template <class F>
class XIgnorer4 {
   F f;
public:
   XIgnorer4( const F& ff ) : f(ff) {}

   template <class X, class Y, class Z, class W>
     struct Sig : public FunType<X,
      typename RT<F,Y,Z,W>::Arg1Type,
      typename RT<F,Y,Z,W>::Arg2Type,
      typename RT<F,Y,Z,W>::Arg3Type,
      typename RT<F,Y,Z,W>::ResultType> {};

   template <class X, class Y, class Z, class W>
   typename Sig<X,Y,Z,W>::ResultType
   operator()( const X&, const Y& y, const Z& z, const W &w ) const {
      return f(y,z,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return debug_string_and_name_or_string("Ignorer4::",f,"Ignorer4");
   }
#endif
};

template <class F>
class XIgnorer5 {
   F f;
public:
   XIgnorer5( const F& ff ) : f(ff) {}

     template <class X, class Y, class Z, class V,class W>
     struct Sig : public FunType<X,
       typename RT<F,Y,Z,V,W>::Arg1Type,
       typename RT<F,Y,Z,V,W>::Arg2Type,
       typename RT<F,Y,Z,V,W>::Arg3Type,
       typename RT<F,Y,Z,V,W>::Arg4Type,
       typename RT<F,Y,Z,V,W>::ResultType> {};

   template <class X, class Y, class Z, class V, class W>
     typename Sig<X,Y,Z,V,W>::ResultType
     operator()( const X&, const Y& y, const Z& z, const V &v, const W &w ) const {
     return f(y,z,v,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return debug_string_and_name_or_string("Ignorer5::",f,"Ignorer5");
   }
#endif
};

#endif
class XIgnore {
 protected:
   template<int n, class F> struct Helper;
   template<class F> struct Helper<0,F> {
      typedef Full1< XIgnorer1<F> > Result;
      static inline Result go( const F& f ) {
         return makeFull1( XIgnorer1<F>(f) );
      }
   };
   template<class F> struct Helper<1,F> {
      typedef Full2< XIgnorer2<F> > Result;
      static inline Result go( const F& f ) {
         return makeFull2( XIgnorer2<F>(f) );
      }
   };
   template<class F> struct Helper<2,F> {
      typedef Full3< XIgnorer3<F> > Result;
      static inline Result go( const F& f ) {
         return makeFull3( XIgnorer3<F>(f) );
      }
   };
// FC++.1.5.2
#ifdef FCPP152
   template<class F> struct Helper<3,F> {
      typedef Full4< XIgnorer4<F> > Result;
      static inline Result go( const F& f ) {
         return makeFull4( XIgnorer4<F>(f) );
      }
   };
   template<class F> struct Helper<4,F> {
      typedef Full5< XIgnorer5<F> > Result;
      static inline Result go( const F& f ) {
         return makeFull5( XIgnorer5<F>(f) );
      }
   };
#endif
public:
   template <class F>
   struct Sig : public FunType<F,
      typename Helper<FunctoidTraits<F>::max_args,F>::Result> {};

   template <class F>
   typename Sig<F>::ResultType operator()( const F& f ) const {
      return Helper<FunctoidTraits<F>::max_args,F>::go(f);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Ignore");
   }
#endif
};

// These are for ignore2 which ignores the second argument.
template <class F>
class XIgnorer22 {
   F f;
public:
   XIgnorer22( const F& ff ) : f(ff) {}

   template <class X, class Y>
     struct Sig : public FunType<typename RT<F,X>::Arg1Type,Y,
                      typename RT<F,X>::ResultType> {};

   template <class X, class Y>
   typename Sig<X,Y>::ResultType
   operator()( const X& x, const Y& ) const {
      return f(x);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return debug_string_and_name_or_string("Ignorer22::",f,"Ignorer22");
   }
#endif
};
template <class F>
class XIgnorer23 {
   F f;
public:
   XIgnorer23( const F& ff ) : f(ff) {}

   template <class X, class Y, class Z>
   struct Sig : public FunType<
     typename RT<F,X,Z>::Arg1Type,Y,
      typename RT<F,X,Z>::Arg2Type,
      typename RT<F,X,Z>::ResultType> {};

   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType
     operator()( const X& x,const Y&, const Z& z ) const {
      return f(x,z);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return debug_string_and_name_or_string("Ignorer23::",f,"Ignorer23");
   }
#endif
};
// FC++.1.5.2
#ifdef FCPP152
template <class F>
class XIgnorer24 {
   F f;
public:
   XIgnorer24( const F& ff ) : f(ff) {}

   template <class X, class Y, class Z,class W>
   struct Sig : public FunType<
     typename RT<F,X,Z,W>::Arg1Type,Y,
     typename RT<F,X,Z,W>::Arg2Type,
     typename RT<F,X,Z,W>::Arg3Type,
     typename RT<F,X,Z,W>::ResultType> {};

   template <class X, class Y, class Z, class W>
     typename Sig<X,Y,Z,W>::ResultType
     operator()( const X& x,const Y&, const Z& z, const W& w ) const {
     return f(x,z,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return debug_string_and_name_or_string("Ignorer24::",f,"Ignorer24");
   }
#endif
};
template <class F>
class XIgnorer25 {
   F f;
public:
   XIgnorer25( const F& ff ) : f(ff) {}

   template <class X, class Y, class Z,class W,class V>
   struct Sig : public FunType<
     typename RT<F,X,Z,W,V>::Arg1Type,Y,
     typename RT<F,X,Z,W,V>::Arg2Type,
     typename RT<F,X,Z,W,V>::Arg3Type,
     typename RT<F,X,Z,W,V>::Arg4Type,
     typename RT<F,X,Z,W,V>::ResultType> {};

   template <class X, class Y, class Z, class W, class V>
     typename Sig<X,Y,Z,W,V>::ResultType
     operator()( const X& x,const Y&, const Z& z, const W& w, const V& v ) const {
     return f(x,z,w,v);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return debug_string_and_name_or_string("Ignorer25::",f,"Ignorer25");
   }
#endif
};

#endif

class XIgnore2 {
 protected:
   template<int n, class F> struct Helper;
   template<class F> struct Helper<1,F> {
      typedef Full2< XIgnorer22<F> > Result;
      static inline Result go( const F& f ) {
         return makeFull2( XIgnorer22<F>(f) );
      }
   };
   template<class F> struct Helper<2,F> {
      typedef Full3< XIgnorer23<F> > Result;
      static inline Result go( const F& f ) {
         return makeFull3( XIgnorer23<F>(f) );
      }
   };
// FC++.1.5.2
#ifdef FCPP152
   template<class F> struct Helper<3,F> {
      typedef Full4< XIgnorer24<F> > Result;
      static inline Result go( const F& f ) {
         return makeFull4( XIgnorer24<F>(f) );
      }
   };
   template<class F> struct Helper<4,F> {
      typedef Full5< XIgnorer25<F> > Result;
      static inline Result go( const F& f ) {
         return makeFull5( XIgnorer25<F>(f) );
      }
   };
#endif
public:
   template <class F>
   struct Sig : public FunType<F,
      typename Helper<FunctoidTraits<F>::max_args,F>::Result> {};

   template <class F>
   typename Sig<F>::ResultType operator()( const F& f ) const {
      return Helper<FunctoidTraits<F>::max_args,F>::go(f);
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Ignore2");
   }
#endif
};

}
typedef Full1<impl::XIgnore> Ignore;
typedef Full1<impl::XIgnore2> Ignore2;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Ignore ignore;
FCPP_MAYBE_EXTERN Ignore2 ignore2;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// ConstructN
//////////////////////////////////////////////////////////////////////
// C++ constructors are not functions, and thus cannot easily be turned
// into functoids.  So we write these helpers.  For example,
//    construct2<Foo>()(x,y) = Foo(x,y)    // Foo is a type name
// Note also that construct1 also serves the role of an explicit
// converter; if Foos (or any type) can be converted into Bars, then we 
// could use a construct1 functoid to capture the conversion function:
//    construct1<Bar>()       // functoid that converts arg into a Bar
//    construct1<Bar>()(x) = Bar(x)
// Note also that these are template functions returning full functoids,
// and we have template structs which name the types of the functoids.

namespace impl {
template <class T>
struct XConstruct0 : public CFunType<T> {
   T operator()() const { return T(); }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Construct0");
   }
#endif
};
}
template <class T> Full0<impl::XConstruct0<T> > construct0()
{ return makeFull0( impl::XConstruct0<T>() ); }
template <class T> struct Construct0
{ typedef Full0<impl::XConstruct0<T> > Type; };

namespace impl {
template <class T>
struct XConstruct1 {
   template <class X> struct Sig : FunType<X,T> {};
   template <class X>
   T operator()( const X& x ) const { return T(x); }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Construct1");
   }
#endif
};
}
template <class T> Full1<impl::XConstruct1<T> > construct1()
{ return makeFull1( impl::XConstruct1<T>() ); }
template <class T> struct Construct1
{ typedef Full1<impl::XConstruct1<T> > Type; };
   
namespace impl {
template <class T>
struct XConstruct2 {
   template <class X, class Y> struct Sig : FunType<X,Y,T> {};
   template <class X, class Y>
   T operator()( const X& x, const Y& y ) const { return T(x,y); }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Construct2");
   }
#endif
};
}
template <class T> Full2<impl::XConstruct2<T> > construct2()
{ return makeFull2( impl::XConstruct2<T>() ); }
template <class T> struct Construct2
{ typedef Full2<impl::XConstruct2<T> > Type; };
   
namespace impl {
template <class T>
struct XConstruct3 {
   template <class X, class Y, class Z> struct Sig : FunType<X,Y,Z,T> {};
   template <class X, class Y, class Z>
   T operator()( const X& x, const Y& y, const Z& z ) const { return T(x,y,z); }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Construct3");
   }
#endif
};
}
template <class T> Full3<impl::XConstruct3<T> > construct3()
{ return makeFull3( impl::XConstruct3<T>() ); }
template <class T> struct Construct3
{ typedef Full3<impl::XConstruct3<T> > Type; };
// FC++.1.5.2
#ifdef FCPP152
namespace impl {
template <class T>
struct XConstruct4 {
   template <class X, class Y, class Z, class W> struct Sig : FunType<X,Y,Z,W,T> {};
   template <class X, class Y, class Z, class W>
   T operator()( const X& x, const Y& y, const Z& z, const W& w ) const
   { return T(x,y,z,w); }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Construct4");
   }
#endif
};
}
template <class T> Full4<impl::XConstruct4<T> > construct4()
{ return makeFull4( impl::XConstruct4<T>() ); }
template <class T> struct Construct4
{ typedef Full4<impl::XConstruct4<T> > Type; };

namespace impl {
template <class T>
struct XConstruct5 {
  template <class X, class Y, class Z, class V, class W> struct Sig :
  FunType<X,Y,Z,V,W,T> {};
  template <class X, class Y, class Z, class V, class W>
    T operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w )
    const { return T(x,y,z,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Construct5");
   }
#endif
};
}
template <class T> Full5<impl::XConstruct5<T> > construct5()
{ return makeFull5( impl::XConstruct5<T>() ); }
template <class T> struct Construct5
{ typedef Full5<impl::XConstruct5<T> > Type; };

namespace impl {
template <class T>
struct XConstruct6 {
  template <class X, class Y, class Z, class U, class V, class W> struct Sig :
  FunType<X,Y,Z,U,V,W,T> {};
  template <class X, class Y, class Z, class U, class V, class W>
    T operator()( const X& x, const Y& y, const Z& z,
                  const U& u, const V& v, const W& w )
    const { return T(x,y,z,u,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Construct6");
   }
#endif
};
}
template <class T> Full6<impl::XConstruct6<T> > construct6()
{ return makeFull6( impl::XConstruct6<T>() ); }
template <class T> struct Construct6
{ typedef Full6<impl::XConstruct6<T> > Type; };

namespace impl {
template <class T>
struct XConstruct7 {
  template <class X, class Y, class Z, class S, class U, class V, class W> struct Sig :
  FunType<X,Y,Z,S,U,V,W,T> {};
  template <class X, class Y, class Z, class S, class U, class V, class W>
    T operator()( const X& x, const Y& y, const Z& z, const S& s,
                  const U& u, const V& v, const W& w )
    const { return T(x,y,z,s,u,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Construct7");
   }
#endif
};
}
template <class T> Full7<impl::XConstruct7<T> > construct7()
{ return makeFull7( impl::XConstruct7<T>() ); }
template <class T> struct Construct7
{ typedef Full7<impl::XConstruct7<T> > Type; };

#endif

//////////////////////////////////////////////////////////////////////
// NewN works like ConstructN but "new"s it and returns the ptr
//////////////////////////////////////////////////////////////////////
namespace impl {
template <class T>
struct XNew0 : public CFunType<T*> {
   T* operator()() const { return new T(); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("New0");
   }
#endif
};
}
template <class T> Full0<impl::XNew0<T> > new0()
{ return makeFull0( impl::XNew0<T>() ); }
template <class T> struct New0
{ typedef Full0<impl::XNew0<T> > Type; };

namespace impl {
template <class T>
struct XNew1 {
   template <class X> struct Sig : FunType<X,T*> {};
   template <class X>
   T* operator()( const X& x ) const { return new T(x); }
#ifdef FCPP_DEBUG
   std::string name() const {
     std::string temp;
     // This is not possible for lack of an object of type T!
     //if (fcpp::traits::debug_traits<T>::has_name) {
     //   temp += std::string("New1::")
     //        + fcpp::traits::debug_traits<T>::name(f);
     //} else {
        temp =  std::string("New1");
     //}
     return temp;
   }
#endif
};
}
template <class T> Full1<impl::XNew1<T> > new1()
{ return makeFull1( impl::XNew1<T>() ); }
template <class T> struct New1
{ typedef Full1<impl::XNew1<T> > Type; };
   
namespace impl {
template <class T>
struct XNew2 {
   template <class X, class Y> struct Sig : FunType<X,Y,T*> {};
   template <class X, class Y>
   T* operator()( const X& x, const Y& y ) const { return new T(x,y); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("New2");
   }
#endif
};
}
template <class T> Full2<impl::XNew2<T> > new2()
{ return makeFull2( impl::XNew2<T>() ); }
template <class T> struct New2
{ typedef Full2<impl::XNew2<T> > Type; };
   
namespace impl {
template <class T>
struct XNew3 {
   template <class X, class Y, class Z> struct Sig : FunType<X,Y,Z,T*> {};
   template <class X, class Y, class Z>
   T* operator()( const X& x, const Y& y, const Z& z ) const
   { return new T(x,y,z); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("New3");
   }
#endif
};
}
template <class T> Full3<impl::XNew3<T> > new3()
{ return makeFull3( impl::XNew3<T>() ); }
template <class T> struct New3
{ typedef Full3<impl::XNew3<T> > Type; };

// FC++.1.5.2
#ifdef FCPP152
namespace impl {
template <class T>
struct XNew4 {
   template <class X, class Y, class Z, class W> struct Sig : FunType<X,Y,Z,W,T*> {};
   template <class X, class Y, class Z, class W>
   T* operator()( const X& x, const Y& y, const Z& z, const W& w ) const
   { return new T(x,y,z,w); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("New4");
   }
#endif
};
}
template <class T> Full4<impl::XNew4<T> > new4()
{ return makeFull4( impl::XNew4<T>() ); }
template <class T> struct New4
{ typedef Full4<impl::XNew4<T> > Type; };

namespace impl {
template <class T>
struct XNew5 {
  template <class X, class Y, class Z, class V,class W> struct Sig : FunType<X,Y,Z,V,W,T*> {};
  template <class X, class Y, class Z, class V,class W>
    T* operator()( const X& x, const Y& y, const Z& z, const V& v, const W& w ) const
  { return new T(x,y,z,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("New5");
   }
#endif
};
}
template <class T> Full5<impl::XNew5<T> > new5()
{ return makeFull5( impl::XNew5<T>() ); }
template <class T> struct New5
{ typedef Full5<impl::XNew5<T> > Type; };

namespace impl {
template <class T>
struct XNew6 {
  template <class X, class Y, class Z, class U, class V, class W> struct Sig : FunType<X,Y,Z,U,V,W,T*> {};
  template <class X, class Y, class Z, class U, class V, class W>
    T* operator()( const X& x, const Y& y, const Z& z, const U& u, const V& v, const W& w ) const
  { return new T(x,y,z,u,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("New6");
   }
#endif
};
}
template <class T> Full6<impl::XNew6<T> > new6()
{ return makeFull6( impl::XNew6<T>() ); }
template <class T> struct New6
{ typedef Full6<impl::XNew6<T> > Type; };

namespace impl {
template <class T>
struct XNew7 {
  template <class X, class Y, class Z, class S, class U, class V, class W> struct Sig :
    FunType<X,Y,Z,S,U,V,W,T*> {};
  template <class X, class Y, class Z, class S, class U, class V, class W>
    T* operator()( const X& x, const Y& y, const Z& z, const S& s, const U& u, const V& v, const W& w ) const
  { return new T(x,y,z,s,u,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("New7");
   }
#endif
};
}
template <class T> Full7<impl::XNew7<T> > new7()
{ return makeFull7( impl::XNew7<T>() ); }
template <class T> struct New7
{ typedef Full7<impl::XNew7<T> > Type; };

#endif

} // end namespace fcpp

// Debug traits now in a separate file.

// Includes argument traits needed without FCPP_DEBUG
#ifndef FCPP_ARDUINO
#include "prelude_debug.h"
#endif

#include "map.h"  // New file for Map and MultiMap data structures.

#ifdef FCPP_ENABLE_LAMBDA
#include "monad.h"
#endif

#include "fcppcout.h" // File with output operators.

// Moved here to sort out sprint problem and made conditional.
#ifdef FCPP_PATTERN
#include "pattern.h"
#endif

#ifndef FCPP_ARDUINO
#include "sprint.h"
#endif

#endif
