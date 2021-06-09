// fcpptuple.h

// Collecting the FC++ material for boost tuple (now TR1 tuple).

// This file ONLY works with C++11 (variadic templates)

// This has now been changed to use std::tuple instead of boost::tuple
// I have done this using TR1::tuple.
// There now are no references to boost in this file and the header
// has been commented out.
// I have added makeTuple1 for use in some testing.

// Collected from testmonad11.cpp

#ifndef FCPP_PRELUDE_DOT_H
// Needed for FC++ Lambda and Monad expressions.
// These would be too late if fcpp/prelude.h has already been read.
#ifndef FCPP_ENABLE_LAMBDA
#define FCPP_ENABLE_LAMBDA
#endif

// FC++ includes for version 1.5
#ifndef FCPP152
#define FCPP152 // Required for tCurry3 which is a Full4
#endif
#include "prelude.h"
#endif
#if (defined FCPP_CONCEPTS)
#include "boost/tuple/tuple.hpp"
#define TR1 boost
#else
#define TR1 std
#endif

#include <string>

//#define TR1 std

#ifndef FCPP_TUPLE_H
#define FCPP_TUPLE_H


// I will leave this in for now.
using namespace fcpp;
using namespace std;

// Define this if you do NOT want the default list output operator.
#define FCPPHASLISTCOUT
// ===========================================
// Output operators for Maybe and ByNeed
#include "fcppcout.h"
// ===========================================

namespace tupleimpl {

    struct XMakeTuple1 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("MakeTuple1");
  }
#endif
#ifndef FCPP_CONCEPTS
   template <class A>
   struct Sig : public FunType<A,TR1::tuple<A> > {};
#endif
   template <class A>
   TR1::tuple<A> operator()( const A& a ) const {
      return TR1::make_tuple(a);
   }
};

struct XMakeTuple2 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("MakeTuple2");
  }
#endif
#ifndef FCPP_CONCEPTS
   template <class A, class B>
   struct Sig : public FunType<A,B,TR1::tuple<A,B> > {};
#endif

   template <class A, class B>
   TR1::tuple<A,B> operator()( const A& a, const B& b ) const {
      return TR1::make_tuple(a,b);
   }
};

// added makeTuple3 as well
struct XMakeTuple3 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("MakeTuple3");
  }
#endif
#ifndef FCPP_CONCEPTS
   template <class A, class B, class C>
   struct Sig : public FunType<A,B,C,TR1::tuple<A,B,C> > {};
#endif

   template <class A, class B, class C>
   TR1::tuple<A,B,C> operator()( const A& a, const B& b, const C& c ) const {
      return TR1::make_tuple(a,b,c);
   }
};

#ifdef FCPP152
  // added makeTuple4 as well
struct XMakeTuple4 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("MakeTuple4");
  }
#endif
#ifndef FCPP_CONCEPTS
   template <class A, class B, class C, class D>
   struct Sig : public FunType<A,B,C,D,TR1::tuple<A,B,C,D> > {};
#endif

   template <class A, class B, class C, class D>
   TR1::tuple<A,B,C,D> operator()
      ( const A& a, const B& b, const C& c , const D& d) const {
      return TR1::make_tuple(a,b,c,d);
   }
};

  // added makeTuple5 as well
struct XMakeTuple5 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("MakeTuple5");
  }
#endif
#ifndef FCPP_CONCEPTS
   template <class A, class B, class C, class D, class E>
   struct Sig : public FunType<A,B,C,D,E,TR1::tuple<A,B,C,D,E> > {};
#endif

   template <class A, class B, class C, class D, class E>
   TR1::tuple<A,B,C,D,E> operator()
      ( const A& a, const B& b, const C& c , const D& d, const E& e) const {
      return TR1::make_tuple(a,b,c,d,e);
   }
};

  // added makeTuple6 as well
struct XMakeTuple6 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("MakeTuple6");
  }
#endif
#ifndef FCPP_CONCEPTS
  template <class A, class B, class C, class D, class E, class F>
    struct Sig : public FunType<A,B,C,D,E,F,TR1::tuple<A,B,C,D,E,F> > {};
#endif

  template <class A, class B, class C, class D, class E, class F>
    TR1::tuple<A,B,C,D,E,F> operator()
    ( const A& a, const B& b, const C& c , const D& d, const E& e, const F& f) const {
    return TR1::make_tuple(a,b,c,d,e,f);
   }
};

  // added makeTuple7 as well
struct XMakeTuple7 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("MakeTuple7");
  }
#endif
#ifndef FCPP_CONCEPTS
  template <class A, class B, class C, class D, class E, class F, class G>
    struct Sig : public FunType<A,B,C,D,E,F,G,TR1::tuple<A,B,C,D,E,F,G> > {};
#endif

  template <class A, class B, class C, class D, class E, class F, class G>
    TR1::tuple<A,B,C,D,E,F,G> operator()
    ( const A& a, const B& b, const C& c , const D& d,
      const E& e, const F& f, const G& g) const {
    return TR1::make_tuple(a,b,c,d,e,f,g);
   }
};

#endif
}
typedef Full1<tupleimpl::XMakeTuple1> MakeTuple1;
typedef Full2<tupleimpl::XMakeTuple2> MakeTuple2;
typedef Full3<tupleimpl::XMakeTuple3> MakeTuple3;
//FCPP_MAYBE_NAMESPACE_OPEN
//FCPP_MAYBE_EXTERN MakeTuple2 makeTuple2;
//FCPP_MAYBE_EXTERN MakeTuple3 makeTuple3;
//FCPP_MAYBE_NAMESPACE_CLOSE
MakeTuple1 makeTuple1;
MakeTuple2 makeTuple2;
MakeTuple3 makeTuple3;
#ifdef FCPP152
typedef Full4<tupleimpl::XMakeTuple4> MakeTuple4;
MakeTuple4 makeTuple4;
typedef Full5<tupleimpl::XMakeTuple5> MakeTuple5;
MakeTuple5 makeTuple5;
typedef Full6<tupleimpl::XMakeTuple6> MakeTuple6;
MakeTuple6 makeTuple6;
typedef Full7<tupleimpl::XMakeTuple7> MakeTuple7;
MakeTuple7 makeTuple7;
#endif

// Modified code to use tuples as well
// From: Dr Mark H Phillips <mark@austrics.com.au>
//using boost::tuple;
//using boost::get;
// Now modified to a variadic version.

  struct Fsty {
    template<typename U> struct Sig;
    template<typename A,typename B> struct Sig<std::pair<A,B> >:
    public
        FunType<std::pair<A,B>,A> {};
#ifndef FCPP_CONCEPTS
    template<typename A,typename... Rest>
    struct Sig<TR1::tuple<A,Rest...>
        >: public FunType<TR1::tuple<A,Rest...>,A> {};
#endif
    template<typename A,typename B> A operator()(

        std::pair<A,B> const& r) const {
      return r.first;
    }
#ifndef FCPP_CONCEPTS
    template<typename A,typename... Rest> A operator()(
        TR1::tuple<A,Rest...> const& u) const {
      return TR1::get<0>(u);
    }
#else
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename J> A operator()(
	TR1::tuple<A,B,C,D,E,F,G,H,I,J> const& u) const {
      return TR1::get<0>(u);
    }
#endif
    static Full1<Fsty>& full() {static Full1<Fsty> f; return f;}
  };
  static Full1<Fsty>& fsty=Fsty::full();

// Version for the second argument
  struct Sndy {
    template<typename U> struct Sig;
    template<typename A,typename B> struct Sig<std::pair<A,B> >:
    public
        FunType<std::pair<A,B>,B> {};
#ifndef FCPP_CONCEPTS
    template<typename A,typename B,typename... Rest>
    struct Sig<TR1::tuple<A,B,Rest...>
        >: public FunType<TR1::tuple<A,B,Rest...>,B> {};
#endif
    template<typename A,typename B> B operator()(
        std::pair<A,B> const& r) const {
      return r.second;
    }
#ifndef FCPP_CONCEPTS
    template<typename A,typename B,typename... Rest> B operator()(
        TR1::tuple<A,B,Rest...> const& u) const {
      return TR1::get<1>(u);
    }
#else
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename J> B operator()(
	TR1::tuple<A,B,C,D,E,F,G,H,I,J> const& u) const {
      return TR1::get<1>(u);
    }
#endif

    static Full1<Sndy>& full() {static Full1<Sndy> f; return f;}
  };
  static Full1<Sndy>& sndy=Sndy::full();

// Version for the third argument (tuple only)
  struct Third {
#ifndef FCPP_CONCEPTS
    template<typename U> struct Sig;
    template<typename A,typename B,typename C,typename... Rest>
    struct Sig<TR1::tuple<A,B,C,Rest...>
        >: public FunType<TR1::tuple<A,B,C,Rest...>,C> {};
    template<typename A,typename B,typename C,typename... Rest> C operator()(
        TR1::tuple<A,B,C,Rest...> const& u) const {
      return TR1::get<2>(u);
    }
#else
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename J> C operator()(
	TR1::tuple<A,B,C,D,E,F,G,H,I,J> const& u) const {
      return TR1::get<2>(u);
    }
#endif
    static Full1<Third>& full() {static Full1<Third> f; return f;}
  };
  static Full1<Third>& third=Third::full();

// Version for the fourth argument (tuple only)
  struct Fourth {
#ifndef FCPP_CONCEPTS
    template<typename U> struct Sig;
    template<typename A,typename B,typename C,typename D,
       typename... Rest>
    struct Sig<TR1::tuple<A,B,C,D,Rest...>
        >: public FunType<TR1::tuple<A,B,C,D,Rest...>,D> {};
    template<typename A,typename B,typename C,typename D,
        typename... Rest> D operator()(
        TR1::tuple<A,B,C,D,Rest...> const& u) const {
      return TR1::get<3>(u);
    }
#else
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename J> D operator()(
	TR1::tuple<A,B,C,D,E,F,G,H,I,J> const& u) const {
      return TR1::get<3>(u);
    }
#endif
    static Full1<Fourth>& full() {static Full1<Fourth> f; return f;}
  };
  static Full1<Fourth>& fourth=Fourth::full();

// Version for the fifth argument (tuple only)
  struct Fifth {
#ifndef FCPP_CONCEPTS
    template<typename U> struct Sig;
    template<typename A,typename B,typename C,typename D,
       typename E,typename... Rest>
    struct Sig<TR1::tuple<A,B,C,D,E,Rest...>
        >: public FunType<TR1::tuple<A,B,C,D,E,Rest...>,E> {};
    template<typename A,typename B,typename C,typename D,
        typename E,typename... Rest> E operator()(
        TR1::tuple<A,B,C,D,E,Rest...> const& u) const {
      return TR1::get<4>(u);
    }
#else
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename J> E operator()(
	TR1::tuple<A,B,C,D,E,F,G,H,I,J> const& u) const {
      return TR1::get<4>(u);
    }
#endif
    static Full1<Fifth>& full() {static Full1<Fifth> f; return f;}
  };
  static Full1<Fifth>& fifth=Fifth::full();
// Version for the sixth argument (tuple only)
  struct Sixth {
#ifndef FCPP_CONCEPTS
    template<typename U> struct Sig;
    template<typename A,typename B,typename C,typename D,
       typename E,typename F,typename... Rest>
    struct Sig<TR1::tuple<A,B,C,D,E,F,Rest...>
        >: public FunType<TR1::tuple<A,B,C,D,E,F,Rest...>,F> {};
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename... Rest> F operator()(
        TR1::tuple<A,B,C,D,E,F,Rest...> const& u) const {
      return get<5>(u);
    }
#else
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename J> E operator()(
        TR1::tuple<A,B,C,D,E,F,G,H,I,J> const& u) const {
      return TR1::get<5>(u);
    }
#endif
    static Full1<Sixth>& full() {static Full1<Sixth> f; return f;}
  };
  static Full1<Sixth>& sixth=Sixth::full();

// Version for the seventh argument (tuple only)
  struct Seventh {
#ifndef FCPP_CONCEPTS
    template<typename U> struct Sig;
    template<typename A,typename B,typename C,typename D,
       typename E,typename F,typename G,typename... Rest>
    struct Sig<TR1::tuple<A,B,C,D,E,F,G,Rest...>
        >: public FunType<TR1::tuple<A,B,C,D,E,F,G,Rest...>,G> {};
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename... Rest> G operator()(
        TR1::tuple<A,B,C,D,E,F,G,Rest...> const& u) const {
      return TR1::get<6>(u);
    }
#else
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename J> E operator()(
	TR1::tuple<A,B,C,D,E,F,G,H,I,J> const& u) const {
      return TR1::get<6>(u);
    }
#endif
    static Full1<Seventh>& full() {static Full1<Seventh> f; return f;}
  };
  static Full1<Seventh>& seventh=Seventh::full();

// Version for the eighth argument (tuple only)
  struct Eighth {
#ifndef FCPP_CONCEPTS
    template<typename U> struct Sig;
    template<typename A,typename B,typename C,typename D,
       typename E,typename F,typename G,typename H,
       typename... Rest>
    struct Sig<TR1::tuple<A,B,C,D,E,F,G,H,Rest...>
        >: public FunType<TR1::tuple<A,B,C,D,E,F,G,H,Rest...>,H> {};
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename... Rest> H operator()(
        TR1::tuple<A,B,C,D,E,F,G,H,Rest...> const& u) const {
      return TR1::get<7>(u);
    }
#else
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename J> E operator()(
	TR1::tuple<A,B,C,D,E,F,G,H,I,J> const& u) const {
      return TR1::get<7>(u);
    }
#endif
    static Full1<Eighth>& full() {static Full1<Eighth> f; return f;}
  };
  static Full1<Eighth>& eighth=Eighth::full();

// Version for the ninth argument (tuple only)
  struct Ninth {
#ifndef FCPP_CONCEPTS
    template<typename U> struct Sig;
    template<typename A,typename B,typename C,typename D,
       typename E,typename F,typename G,typename H,
       typename I,typename... Rest>
    struct Sig<TR1::tuple<A,B,C,D,E,F,G,H,I,Rest...>
        >: public FunType<TR1::tuple<A,B,C,D,E,F,G,H,I,Rest...>,I> {};
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename... Rest> I operator()(
        TR1::tuple<A,B,C,D,E,F,G,H,I,Rest...> const& u) const {
      return TR1::get<8>(u);
    }
#else
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename J> E operator()(
	TR1::tuple<A,B,C,D,E,F,G,H,I,J> const& u) const {
      return TR1::get<8>(u);
    }
#endif
    static Full1<Ninth>& full() {static Full1<Ninth> f; return f;}
  };
  static Full1<Ninth>& ninth=Ninth::full();

#include <boost/static_assert.hpp>

// Version for the tenth argument (tuple only)
  struct Tenth {
#ifndef FCPP_CONCEPTS
    template<typename U> struct Sig;
    template<typename A,typename B,typename C,typename D,
       typename E,typename F,typename G,typename H,
       typename I,typename J, typename... Rest>
    struct Sig<TR1::tuple<A,B,C,D,E,F,G,H,I,J,Rest...>
        >: public FunType<TR1::tuple<A,B,C,D,E,F,G,H,I,J,Rest...>,J> {};
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename J, typename... Rest> J operator()(
        TR1::tuple<A,B,C,D,E,F,G,H,I,J,Rest...> const& u) const {
      // Attempt to generate a shorter error message, but it didn't work.
      //typedef TR1::tuple<A,B,C,D,E,F,G,H,I,J,Rest...> tuple_t;
      //BOOST_STATIC_ASSERT((boost::tuples::length<tuple_t >::value >= 10));
      return TR1::get<9>(u);
    }
#else
    template<typename A,typename B,typename C,typename D,
        typename E,typename F,typename G,typename H,
        typename I,typename J> E operator()(
	TR1::tuple<A,B,C,D,E,F,G,H,I,J> const& u) const {
      // Attempt to generate a shorter error message, but it didn't work.
      typedef TR1::tuple<A,B,C,D,E,F,G,H,I,J> tuple_t;
      //BOOST_STATIC_ASSERT((boost::tuples::length<tuple_t >::value >= 10));
      return TR1::get<9>(u);
    }
#endif
   static Full1<Tenth>& full() { static Full1<Tenth> f; return f; }
  };
  static Full1<Tenth>& tenth=Tenth::full();

// Extending ideas in debugm.h to a tuple and hence to a variadic tuple.

template <class FTuple2, class Tuple2>
Tuple2 operateT2(const FTuple2& fp, const Tuple2 &p)
{
  return makeTuple2(fsty(fp)(fsty(p)),sndy(fp)(sndy(p)));
}

template <class FTuple3, class Tuple3>
Tuple3 operateT3(const FTuple3& fp, const Tuple3 &p)
{
  return makeTuple3(fsty(fp)(fsty(p)),sndy(fp)(sndy(p)),third(fp)(third(p)));
}

#ifdef FCPP152
template <class FTuple4, class Tuple4>
Tuple4 operateT4(const FTuple4& fp, const Tuple4 &p)
{
  return makeTuple4(fsty(fp)(fsty(p)),sndy(fp)(sndy(p)),third(fp)(third(p)),
                    fourth(fp)(fourth(p)));
}

template <class FTuple5, class Tuple5>
Tuple5 operateT5(const FTuple5& fp, const Tuple5 &p)
{
  return makeTuple5(fsty(fp)(fsty(p)),sndy(fp)(sndy(p)),third(fp)(third(p)),
                    fourth(fp)(fourth(p)),fifth(fp)(fifth(p)));
}
#endif

// Haskell has curry() and uncurry() for pairs; we call these hCurry()
// and hUncurry().
// These equivalents for boost tuple are tCurry and tUncurry
// with aliases tCurry2 and tUncurry2
// also tCurry3 and tUncurry3 for three member tuples.
namespace myimpl {
   struct XTCurry {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("TCurry");
  }
#endif
      template <class F, class X, class Y> struct Sig : public FunType<F,X,Y,
	typename RT<F,TR1::tuple<X,Y> >::ResultType> {};
      template <class F, class X, class Y>
#ifdef FCPP_CONCEPTS
   inline ConstCallable1<F,TR1::tuple<X,Y> >::result_type
#else
       typename Sig<F,X,Y>::ResultType
#endif
      operator()( const F& f, const X& x, const Y& y ) const {
	//std::cout << "XTCurry : " << x << "," << y << std::endl;
         return f( makeTuple2(x,y) );
      }
   };

#ifdef FCPP152
   struct XTCurry3 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("TCurry3");
  }
#endif
     template <class F, class X, class Y, class Z> struct Sig :
        public FunType<F,X,Y,Z,
       typename RT<F,TR1::tuple<X,Y,Z> >::ResultType> {};
     template <class F, class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
       inline ConstCallable1<F,TR1::tuple<X,Y,Z> >::result_type
#else
       typename Sig<F,X,Y,Z>::ResultType
#endif
       operator()( const F& f, const X& x, const Y& y, const Z& z ) const {
       //std::cout << "XTCurry3 : " << x << "," << y << "," << z << std::endl;
      return f( makeTuple3(x,y,z) );
      }
   };

   struct XTCurry4 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("TCurry4");
  }
#endif
     template <class F, class W,class X, class Y, class Z> struct Sig :
        public FunType<F,W,X,Y,Z,
     typename RT<F,TR1::tuple<W,X,Y,Z> >::ResultType> {};

     template <class F, class W, class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
       inline ConstCallable1<F,TR1::tuple<W,X,Y,Z> >::result_type
#else
       typename Sig<F,W,X,Y,Z>::ResultType
#endif
       operator()( const F& f, const W& w,const X& x, const Y& y, const Z& z ) const {
       //std::cout << "XTCurry3 : " << x << "," << y << "," << z << std::endl;
       return f( makeTuple4(w,x,y,z) );
      }
   };

   struct XTCurry5 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("TCurry5");
  }
#endif

     template <class F,class V,class W,class X, class Y, class Z> struct Sig :
        public FunType<F,V,W,X,Y,Z,
    typename RT<F,TR1::tuple<V,W,X,Y,Z> >::ResultType> {};

     template <class F, class V,class W, class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
       inline ConstCallable1<F,TR1::tuple<V,W,X,Y,Z> >::result_type
#else
       typename Sig<F,V,W,X,Y,Z>::ResultType
#endif
       operator()( const F& f, const V& v,const W& w,const X& x, const Y& y, const Z& z ) const {
       //std::cout << "XTCurry3 : " << x << "," << y << "," << z << std::endl;
       return f( makeTuple5(v,w,x,y,z) );
      }
   };

   struct XTCurry6 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("TCurry6");
  }
#endif
     template <class F,class U,class V,class W,class X, class Y, class Z>
     struct Sig : public FunType<F,U,V,W,X,Y,Z,
    typename RT<F,TR1::tuple<U,V,W,X,Y,Z> >::ResultType> {};

     template <class F,class U,class V,class W,class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
       inline ConstCallable1<F,TR1::tuple<U,V,W,X,Y,Z> >::result_type
#else
      typename Sig<F,U,V,W,X,Y,Z>::ResultType
#endif
       operator()( const F& f, const U& u,const V& v,const W& w,const X& x, const Y& y, const Z& z ) const {
       return f( makeTuple6(u,v,w,x,y,z) );
      }
   };

   struct XTCurry7 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("TCurry7");
  }
#endif
     template <class F,class T,class U,class V,class W,class X, class Y, class Z>
    struct Sig : public FunType<F,T,U,V,W,X,Y,Z,
    typename RT<F,TR1::tuple<T,U,V,W,X,Y,Z> >::ResultType> {};

     template <class F,class T,class U,class V,class W,class X, class Y, class Z>
#ifdef FCPP_CONCEPTS
       inline ConstCallable1<F,TR1::tuple<T,U,V,W,X,Y,Z> >::result_type
#else
       typename Sig<F,T,U,V,W,X,Y,Z>::ResultType
#endif
       operator()( const F& f, const T& t,const U& u,const V& v,
                  const W& w,const X& x, const Y& y, const Z& z ) const {
       return f( makeTuple7(t,u,v,w,x,y,z) );
      }
   };

#endif
}
typedef Full3<myimpl::XTCurry> TCurry;
#ifdef FCPP152
typedef Full4<myimpl::XTCurry3> TCurry3;
typedef Full5<myimpl::XTCurry4> TCurry4;
typedef Full6<myimpl::XTCurry5> TCurry5;
typedef Full7<myimpl::XTCurry6> TCurry6;
typedef Full8<myimpl::XTCurry7> TCurry7;
#endif
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN TCurry tCurry;
FCPP_MAYBE_EXTERN TCurry tCurry2;
#ifdef FCPP152
FCPP_MAYBE_EXTERN TCurry3 tCurry3;
FCPP_MAYBE_EXTERN TCurry4 tCurry4;
FCPP_MAYBE_EXTERN TCurry5 tCurry5;
FCPP_MAYBE_EXTERN TCurry6 tCurry6;
FCPP_MAYBE_EXTERN TCurry7 tCurry7;
#endif
FCPP_MAYBE_NAMESPACE_CLOSE

namespace myimpl {
   struct XTUncurry {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("TUncurry");
  }
#endif
     template<typename F,typename U> struct Sig;
     template<typename  F, typename A, typename B >
     struct Sig<F,TR1::tuple<A,B> > :
     public FunType<F,TR1::tuple<A,B>,typename
         RT<F,A,B>::ResultType> {};
     template <class F, class A, class B>
#ifdef FCPP_CONCEPTS
       inline ConstCallable2<F,A,B>::result_type
#else
       typename Sig<F,TR1::tuple<A,B> >::ResultType
#endif
      operator()( const F& f, const TR1::tuple<A,B>& p ) const {
         return f( fsty(p), sndy(p) );
      }
   };

   struct XTUncurry3 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("TUncurry3");
  }
#endif
     template<typename F,typename U> struct Sig;
     template<typename  F, typename A, typename B, typename C >
       struct Sig<F,TR1::tuple<A,B,C> > :
       public FunType<F,TR1::tuple<A,B,C>,typename
       RT<F,A,B,C>::ResultType> {};
     template <class F, class A, class B,class C>
#ifdef FCPP_CONCEPTS
       inline ConstCallable3<F,A,B,C>::result_type
#else
       typename Sig<F,TR1::tuple<A,B,C> >::ResultType
#endif
       operator()( const F& f, const TR1::tuple<A,B,C>& p ) const {
       return f( fsty(p), sndy(p), third(p) );
      }
   };

   struct XTUncurry4 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("TUncurry4");
  }
#endif
     template<typename F,typename U> struct Sig;
     template<typename  F, typename A, typename B, typename C, typename D >
       struct Sig<F,TR1::tuple<A,B,C,D> > :
       public FunType<F,TR1::tuple<A,B,C,D>,typename
       RT<F,A,B,C,D>::ResultType> {};

     template <class F, class A, class B,class C, class D>
#ifdef FCPP_CONCEPTS
       inline ConstCallable4<F,A,B,C,D>::result_type
#else
       typename Sig<F,TR1::tuple<A,B,C,D> >::ResultType
#endif
       operator()( const F& f, const TR1::tuple<A,B,C,D>& p ) const {
       return f( fsty(p), sndy(p), third(p), fourth(p) );
      }
   };


   struct XTUncurry5 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("TUncurry5");
  }
#endif
     template<typename F,typename U> struct Sig;
     template<typename  F, typename A, typename B, typename C, typename D, typename E >
       struct Sig<F,TR1::tuple<A,B,C,D,E> > :
       public FunType<F,TR1::tuple<A,B,C,D,E>,typename
       RT<F,A,B,C,D,E>::ResultType> {};

     template <class F, class A, class B,class C, class D, class E>
#ifdef FCPP_CONCEPTS
       inline ConstCallable5<F,A,B,C,D,E>::result_type
#else
       typename Sig<F,TR1::tuple<A,B,C,D,E> >::ResultType
#endif
       operator()( const F& f, const TR1::tuple<A,B,C,D,E>& p ) const {
       return f( fsty(p), sndy(p), third(p), fourth(p), fifth(p) );
      }
   };

   struct XTUncurry6 {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("TUncurry6");
  }
#endif
     template<typename F,typename U> struct Sig;
     template<typename  F, typename A, typename B, typename C, typename D, typename E, typename FF >
       struct Sig<F,TR1::tuple<A,B,C,D,E,FF> > :
       public FunType<F,TR1::tuple<A,B,C,D,E,FF>,typename
       RT<F,A,B,C,D,E,FF>::ResultType> {};

     template <class F, class A, class B,class C, class D, class E,class FF>
#ifdef FCPP_CONCEPTS
       inline ConstCallable6<F,A,B,C,D,E,FF>::result_type
#else
       typename Sig<F,TR1::tuple<A,B,C,D,E,FF> >::ResultType
#endif
       operator()( const F& f, const TR1::tuple<A,B,C,D,E,FF>& p ) const {
       return f( fsty(p), sndy(p), third(p), fourth(p), fifth(p), sixth(p) );
      }
   };

}
typedef Full2<myimpl::XTUncurry>  TUncurry;
typedef Full2<myimpl::XTUncurry3> TUncurry3;
typedef Full2<myimpl::XTUncurry4> TUncurry4;
typedef Full2<myimpl::XTUncurry5> TUncurry5;
typedef Full2<myimpl::XTUncurry6> TUncurry6;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN TUncurry  tUncurry;
FCPP_MAYBE_EXTERN TUncurry  tUncurry2;
FCPP_MAYBE_EXTERN TUncurry3 tUncurry3;
FCPP_MAYBE_EXTERN TUncurry4 tUncurry4;
FCPP_MAYBE_EXTERN TUncurry5 tUncurry5;
FCPP_MAYBE_EXTERN TUncurry6 tUncurry6;
FCPP_MAYBE_NAMESPACE_CLOSE

// ================================================================
// Mathematics for FC++
// There is so far nothing in FC++ to handle common functions
// in real arithmetic as functoids.
// I am putting these in fcpptuple.h for now.
// Design decision: use long names to distinguish from
// the standard ones.
// ================================================================

#include <cmath>

namespace myimpl {
struct XCosine {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("Cosine");
  }
#endif

   template <class T>
   struct Sig : public FunType<T,T> {};
   template <class T>
   T operator()( const T& x ) const {
      return cos( x );
   }
};

struct XSine {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("Sine");
  }
#endif
   template <class T>
   struct Sig : public FunType<T,T> {};

   template <class T>
   T operator()( const T& x ) const {
      return sin( x );
   }
};

struct XTangent {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("Tangent");
  }
#endif
   template <class T>
   struct Sig : public FunType<T,T> {};

   template <class T>
   T operator()( const T& x ) const {
      return tan( x );
   }
};

}
typedef Full1<myimpl::XCosine> Cosine;
typedef Full1<myimpl::XSine> Sine;
typedef Full1<myimpl::XTangent> Tangent;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Cosine cosine;
FCPP_MAYBE_EXTERN Sine sine;
FCPP_MAYBE_EXTERN Tangent tangent;
FCPP_MAYBE_NAMESPACE_CLOSE

// ======================================================================
// FCPP_DEBUG traits.
// These traits tell the debug system which functoids now have a
// name declated in the name() member function.
// This has been set up so that testing and use need not wait
// until all functoids have been given a name function.
// The whole system except for the basic output operators for FullN
// is turned on by -DFCPP_DEBUG.
// ======================================================================
#ifdef FCPP_DEBUG
namespace fcpp {
  namespace traits {
    template<>
      class debug_traits<tupleimpl::XMakeTuple2>
      {
      public:
       static const bool has_name = true;
       static std::string name(const tupleimpl::XMakeTuple2 &f) { return f.name(); }
      };
    template<>
      class debug_traits<tupleimpl::XMakeTuple3>
      {
      public:
       static const bool has_name = true;
       static std::string name(const tupleimpl::XMakeTuple3 &f) { return f.name(); }
      };
#ifdef FCPP152
    template<>
      class debug_traits<tupleimpl::XMakeTuple4>
      {
      public:
       static const bool has_name = true;
       static std::string name(const tupleimpl::XMakeTuple4 &f) { return f.name(); }
      };
    template<>
      class debug_traits<tupleimpl::XMakeTuple5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const tupleimpl::XMakeTuple5 &f) { return f.name(); }
      };
    template<>
      class debug_traits<tupleimpl::XMakeTuple6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const tupleimpl::XMakeTuple6 &f) { return f.name(); }
      };
    template<>
      class debug_traits<tupleimpl::XMakeTuple7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const tupleimpl::XMakeTuple7 &f) { return f.name(); }
      };
#endif
    template<>
      class debug_traits<myimpl::XTCurry>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTCurry &f) { return f.name(); }
      };
    template<>
#ifdef FCPP152
      class debug_traits<myimpl::XTCurry3>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTCurry3 &f) { return f.name(); }
      };
    template<>
      class debug_traits<myimpl::XTCurry4>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTCurry4 &f) { return f.name(); }
      };
    template<>
      class debug_traits<myimpl::XTCurry5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTCurry5 &f) { return f.name(); }
      };
    template<>
      class debug_traits<myimpl::XTCurry6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTCurry6 &f) { return f.name(); }
      };
    template<>
      class debug_traits<myimpl::XTCurry7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTCurry7 &f) { return f.name(); }
      };
#endif
    template<>
      class debug_traits<myimpl::XTUncurry>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTUncurry &f) { return f.name(); }
      };
    template<>
      class debug_traits<myimpl::XTUncurry3>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTUncurry3 &f) { return f.name(); }
      };
    template<>
      class debug_traits<myimpl::XTUncurry4>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTUncurry4 &f) { return f.name(); }
      };

    template<>
      class debug_traits<myimpl::XTUncurry5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTUncurry5 &f) { return f.name(); }
      };

    template<>
      class debug_traits<myimpl::XTUncurry6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTUncurry6 &f) { return f.name(); }
      };

    template<>
      class debug_traits<myimpl::XCosine>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XCosine &f) { return f.name(); }
      };
    template<>
      class debug_traits<myimpl::XSine>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XSine &f) { return f.name(); }
      };
    template<>
      class debug_traits<myimpl::XTangent>
      {
      public:
       static const bool has_name = true;
       static std::string name(const myimpl::XTangent &f) { return f.name(); }
      };
  } // namespace traits
} // namespace fcpp
#endif

// ====================================================================
// Debug output from FC++.
// It has long been a problem when developing code with FC++
// that sometimes a line is put into code such as
//
// cout << object << endl;
//
// when the object is a FullN object which needs more parameters.
// The result is a lot of compiler error messages giving alternative
// definitions of << operators.
//
// This is an attempt to get useful run time output in these cases,
// rather than a lot of useless compile time errors.
//
// At the moment this is experimental and has several stages.
// 1. (DEFAULT)
//    Output a message to say that a call has been made.
//    At present execution continues. Note that if this file
//    is not included then the old behaviour will continue.
//
// 2. (Requires -DFCPP_DEBUG)
//    Output also the name of the functoid. At present this is just
//    a call to name() of the FullN object (all implemented).
//    It is envisaged that this will chain on to give the names of all the
//    things which are in the chain, but this requires name() to be
//    implemented in all the objects. (example in XCosine).
//
// 3. (Requires -DFCPP_DEBUG)
//    If it were possible to define a trait for this it could be implemented
//    in stages. (Now implemented - see fcpp_debug_traits.h)
//    Note that a specialisation has to be declared for each functoid
//    provided with a name() operator. See examples in this file.
//    It is suggested those be local to the functoid definition.
//
// 4. At some stage this can be expanded to include bound parameters.
//
// 5. Also control over continued execution can be implemented.
//
// ====================================================================
template <class TBBV>
std::ostream & operator << (std::ostream &o, const fcpp_lambda::LambdaThingy<TBBV> &f)
{
#ifdef FCPP_DEBUG
    if (fcpp::traits::debug_traits<fcpp_lambda::LambdaThingy<TBBV> >::has_name)
      o << "**** ERROR: Attempt to output "
        << fcpp::traits::debug_traits<fcpp_lambda::LambdaThingy<TBBV> >::name(f);
    else
      o << "**** ERROR: Attempt to output a LambdaThingy ****";
#else
  o << "**** ERROR: Attempt to output a LambdaThingy ****";
#endif
  return o;
}

template <class F>
std::ostream & operator << (std::ostream &o, const Full0<F> &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full0 functoid ****";
#endif
  return o;
}

template <class F>
std::ostream & operator << (std::ostream &o, const Full1<F> &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full1 functoid ****";
#endif
  return o;
}

template <class F>
std::ostream & operator << (std::ostream &o, const Full2<F> &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full2 functoid ****";
#endif
  return o;
}

template <class F>
std::ostream & operator << (std::ostream &o, const Full3<F> &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full3 functoid ****";
#endif
  return o;
}

#ifdef FCPP152
template <class F>
std::ostream & operator << (std::ostream &o, const Full4<F> &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full4 functoid ****";
#endif
  return o;
}

template <class F>
std::ostream & operator << (std::ostream &o, const Full5<F> &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full5 functoid ****";
#endif
  return o;
}

template <class F>
std::ostream & operator << (std::ostream &o, const Full6<F> &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full6 functoid ****";
#endif
  return o;
}

template <class F>
std::ostream & operator << (std::ostream &o, const Full7<F> &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full7 functoid ****";
#endif
  return o;
}

template <class A, class B>
  std::ostream & operator << (std::ostream &o, const std::pair<A,B> &p)
{
  o << "**** ERROR: Attempt to output std::pair ****";
  return o;
}

template <class A>
  std::ostream & operator << (std::ostream &o, const TR1::tuple<A> &p)
{
  o << "**** ERROR: Attempt to output TR1::tuple(1) ****";
  return o;
}

template <class A, class B>
  std::ostream & operator << (std::ostream &o, const TR1::tuple<A,B> &p)
{
  o << "**** ERROR: Attempt to output TR1::tuple(2) ****";
  return o;
}

template <class A, class B, class C>
  std::ostream & operator << (std::ostream &o, const TR1::tuple<A,B,C> &p)
{
  o << "**** ERROR: Attempt to output TR1::tuple(3) ****";
  return o;
}

template <class A, class B, class C, class D>
  std::ostream & operator << (std::ostream &o, const TR1::tuple<A,B,C,D> &p)
{
  o << "**** ERROR: Attempt to output TR1::tuple(4) ****";
  return o;
}

#endif
#endif
