// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_BINDERS6_DOT_H
#define FCPP_CURRY_BINDERS6_DOT_H

//////////////////////////////////////////////////////////////////////
// These are the binders for Full6 and Fun6 - removed from curry.h.
// Concept maps are in curry_concept_maps.h
//////////////////////////////////////////////////////////////////////
// bind 1 argument of 6 - there are 6 possibilities.
// Only the first is implemented for now.
//////////////////////////////////////////////////////////////////////
// Implementation notes for binders (6 and 7)
//
// New binder classes need an entry in the table at the end of full.h
// to be accessible to user code.
//
// Binders with X, XandY etc
// Each binder combination needs a helper function for each combination
// of binders.
// This also needs a corresponding SigN within the binder class
// and also a concept map (in curry_concept_maps.h) 
// for use with concepts.
//
// For use with FullN and FunN the correct helper functions are also 
// needed - see curry_helpers6.h and curry_helpers7.h
// plus Sig and concept map in full.h
// 
//
//////////////////////////////////////////////////////////////////////

template <class Sextinary, class Arg1>
class binder1of6 {
   const Sextinary f;
   const Arg1 x;
public:
   binder1of6( const Sextinary& a, const Arg1& b ) : f(a), x(b) {}

#ifndef FCPP_CONCEPTS
   template <class Arg2, class Arg3, class Arg4, class Arg5,class Arg6 > struct Sig 
   : public FunType<typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg2Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg3Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg4Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg5Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg6Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::ResultType> {};
#endif

   template <class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
#ifdef FCPP_CONCEPTS
     inline ConstCallable6<Sextinary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::result_type
#else
   typename RT<Sextinary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::ResultType
#endif
   operator()( const Arg2& y, const Arg3& z, const Arg4& u, const Arg5& v, const Arg6& w ) const {
      return f(x,y,z,u,v,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Sextinary>() ) {
       std::string bindername = std::string("binder1of6::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(x);
       bindername += std::string(" )");
       return bindername;
     } else
        return std::string("binder1of6");
   }
#endif
};

struct Bind1of6 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1of6");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Sextinary, class Arg1>
   struct Sig 
   : public FunType<Sextinary,Arg1,Full5<binder1of6<Sextinary,Arg1> > > {};
#endif

   template <class Sextinary, class Arg1>
#ifdef FCPP_CONCEPTS
   inline Full5<binder1of6<Sextinary,Arg1> >
#else
   typename Sig<Sextinary,Arg1>::ResultType
#endif
   operator()( const Sextinary& f, const Arg1& x ) const {
      return makeFull5( binder1of6<Sextinary,Arg1>(f,x) );
   }
};

 template <typename Binder,typename R,typename F,typename ArgA,
   typename A,typename B,typename C,typename D, typename E>
struct binderXof6Helper;

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D, typename E>
   struct binderXof6Helper<Bind1,R,F,ArgA,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgA& argA,
                      const A& a, const B& b, const C& c,
                      const D& d, const E& e)
  {
    return f(argA,a,b,c,d,e);
  }
};

 template <typename Binder,typename R,typename F,typename ArgA,
   typename A,typename B,typename C,typename D, typename E>
struct binderXof6Helper;

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D, typename E>
   struct binderXof6Helper<Bind2,R,F,ArgA,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgA& argA,
                      const A& a, const B& b, const C& c,
                      const D& d, const E& e)
  {
    return f(a,argA,b,c,d,e);
  }
};

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D, typename E>
   struct binderXof6Helper<Bind3,R,F,ArgA,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgA& argA,
                      const A& a, const B& b, const C& c,
                      const D& d, const E& e)
  {
    return f(a,b,argA,c,d,e);
  }
};

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D, typename E>
   struct binderXof6Helper<Bind4,R,F,ArgA,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgA& argA,
                      const A& a, const B& b, const C& c,
                      const D& d, const E& e)
  {
    return f(a,b,c,argA,d,e);
  }
};

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D, typename E>
   struct binderXof6Helper<Bind5,R,F,ArgA,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgA& argA,
                      const A& a, const B& b, const C& c,
                      const D& d, const E& e)
  {
    return f(a,b,c,d,argA,e);
  }
};

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D, typename E>
   struct binderXof6Helper<Bind6,R,F,ArgA,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgA& argA,
                      const A& a, const B& b, const C& c,
                      const D& d, const E& e)
  {
    return f(a,b,c,d,e,argA);
  }
};

template <class Sextinary, typename Pair>
class binderXof6
{
     const Sextinary f;
     Pair p;
     // These give the types for later
     typedef typename Pair::first_type Binder;
     typedef typename Pair::second_type ArgA;
public:
   binderXof6( const Sextinary& g,const Pair& q) : f(g), 
     p(q) {}

#ifndef FCPP_CONCEPTS
     template <typename Bind, typename A, typename B, typename C, typename D, typename E>
     struct SigN;

     template <typename A, typename B, typename C, typename D, typename E>
       struct  SigN<Bind1,A,B,C,D,E>
       : public FunType<Sextinary,ArgA,A,B,C,D,E,
       typename RT<Sextinary,ArgA,A,B,C,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D, typename E>
       struct  SigN<Bind2,A,B,C,D,E>
       : public FunType<Sextinary,ArgA,A,B,C,D,E,
       typename RT<Sextinary,A,ArgA,B,C,D,E>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D, typename E>
       struct  SigN<Bind3,A,B,C,D,E>
       : public FunType<Sextinary,ArgA,A,B,C,D,E,
       typename RT<Sextinary,A,B,ArgA,C,D,E>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D, typename E>
       struct  SigN<Bind4,A,B,C,D,E>
       : public FunType<Sextinary,ArgA,A,B,C,D,E,
       typename RT<Sextinary,A,B,C,ArgA,D,E>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D, typename E>
       struct  SigN<Bind5,A,B,C,D,E>
       : public FunType<Sextinary,ArgA,A,B,C,D,E,
       typename RT<Sextinary,A,B,C,D,ArgA,E>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D, typename E>
       struct  SigN<Bind6,A,B,C,D,E>
       : public FunType<Sextinary,ArgA,A,B,C,D,E,
       typename RT<Sextinary,A,B,C,D,E,ArgA>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D, typename E>
       struct Sig : public SigN<Binder,A,B,C,D,E>
     { };
#endif

     template <typename A,typename B,typename C,typename D, typename E>
#ifdef FCPP_CONCEPTS
       inline ConstCallable7<Sextinary,Binder,ArgA,A,B,C,D,E>::result_type
#else
       typename Sig<A,B,C,D,E>::ResultType 
#endif
    operator()
       (const A& a, const B& b, const C& c, const D& d, const E& e) const 
   {
#ifdef FCPP_CONCEPTS
      return binderXof6Helper<Binder,
        typename ConstCallable7<Sextinary,Binder,ArgA,A,B,C,D,E>::result_type,
        Sextinary,ArgA,A,B,C,D,E>::go(f,p.second,a,b,c,d,e);
#else
     return binderXof6Helper<Binder,
       typename Sig<A,B,C,D,E>::ResultType,Sextinary,          
       ArgA,A,B,C,D,E>::go(f,p.second,a,b,c,d,e);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Sextinary>() ) {
       std::string temp = std::string("binderXof6::")
                        + debug_traits_name(f);
       if (p.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(p.first.n);
       temp += std::string(",") + debug_name_or_value(p.second) + std::string(")");
       return temp;
     } else
       return std::string("binderXof6");
    }
#endif
};

struct BindXof6 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXof6");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Sextinary, class Pair>
   struct Sig 
    : public FunType<Sextinary,Pair,Full5<binderXof6<Sextinary,Pair> > > {};
#endif

  template <class Sextinary, class Pair>
#ifdef FCPP_CONCEPTS
    inline Full5<binderXof6<Sextinary,Pair> >
#else
    typename Sig<Sextinary,Pair>::ResultType
#endif
    operator()( const Sextinary& f,const Pair &p ) const {
    return makeFull5( binderXof6<Sextinary,Pair>(f,p) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 2 arguments - there are 15 possibilities.
// Only one is implemented as yet.
//////////////////////////////////////////////////////////////////////

template <class Sextinary, class Arg1, class Arg2>
class binder1and2of6 {
   const Sextinary f;
   const Arg1 a1;
   const Arg2 a2;
public:
   binder1and2of6(const Sextinary& w, const Arg1& x, const Arg2& y) : 
     f(w), a1(x), a2(y) {};

#ifndef FCPP_CONCEPTS
   template <class Arg3, class Arg4, class Arg5, class Arg6>
   struct Sig 
   : public FunType<typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg3Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg4Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg5Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg6Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::ResultType> 
   {};
#endif

   template <class Arg3,class Arg4,class Arg5,class Arg6>
#ifdef FCPP_CONCEPTS
   inline ConstCallable6<Sextinary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::result_type
#else
   typename Sig<Arg3,Arg4,Arg5,Arg6>::ResultType
#endif
   operator()(const Arg3& z, const Arg4& u, const Arg5& v, const Arg6& w) const
  { return f(a1,a2,z,u,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Sextinary>() ) {
       std::string bindername = std::string("binder1and2of6::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1and2of6");
   }
#endif
};

struct Bind1and2of6 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1and2of6");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Sextinary, class A1, class A2>
   struct Sig 
   : public FunType<Sextinary,A1,A2,Full4<binder1and2of6<Sextinary,A1,A2> > > {};
#endif

   template <class Sextinary, class A1, class A2>
   inline Full4<binder1and2of6<Sextinary,A1,A2> >
   operator()( const Sextinary& f, const A1& a1, const A2& a2 ) const {
      return makeFull4( binder1and2of6<Sextinary,A1,A2>(f,a1,a2) );
   }
};

////////////////////////////////////////////////////////////////////////
// binderXandYof6 provides the remainder.
// All that is needed is more specialisations of binderXandYof6Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////
template <typename BinderX,typename BinderY,typename R,typename F,
  typename ArgX, typename ArgY,typename A,typename B,typename C, typename D>
struct binderXandYof6Helper;

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind1,Bind2,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,argY,a,b,c,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind1,Bind3,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,a,argY,b,c,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind1,Bind4,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,a,b,argY,c,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind1,Bind5,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,a,b,c,argY,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind1,Bind6,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,a,b,c,d,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind2,Bind3,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,argX,argY,b,c,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind2,Bind4,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,argX,b,argY,c,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind2,Bind5,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,argX,b,c,argY,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
 typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind2,Bind6,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,argX,b,c,d,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind3,Bind4,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,argX,argY,c,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind3,Bind5,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,argX,c,argY,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind3,Bind6,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,argX,c,d,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind4,Bind5,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,c,argX,argY,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind4,Bind6,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,c,argX,d,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D>
  struct binderXandYof6Helper<Bind5,Bind6,R,F,ArgX,ArgY,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,c,d,argX,argY);
  }
};

template <class Sextinary, typename PairX, typename PairY>
class binderXandYof6
{
     const Sextinary f;
     PairX px;
     PairY py;
     // These give the types for later
     typedef typename PairX::first_type BinderX;
     typedef typename PairX::second_type ArgX;
     typedef typename PairY::first_type BinderY;
     typedef typename PairY::second_type ArgY;

#ifdef FCPP_CXX11
     // Compile time error message if the binders are the same.
     static_assert(not std::is_same<BinderX,BinderY>::value,
     "***** ERROR in binderXandYof6 : BinderX==BinderY : ERROR *****");
#endif
public:
   binderXandYof6( const Sextinary& g,const PairX& qx,const PairY& qy) :
     f(g), px(qx), py(qy) {}

#ifndef FCPP_CONCEPTS
     template <typename BindX, typename BindY, typename A, typename B, typename C, typename D>
     struct SigN;

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind2,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,ArgX,ArgY,A,B,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind3,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,ArgX,A,ArgY,B,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind4,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,ArgX,A,B,ArgY,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind5,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,ArgX,A,B,C,ArgY,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind6,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,ArgX,A,B,C,D,ArgY>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind2,Bind3,A,B,C,D>
       : public FunType<Sextinary,A,ArgX,ArgY,B,C,D,
       typename RT<Sextinary,A,ArgX,ArgY,B,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind2,Bind4,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,A,ArgX,B,ArgY,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind2,Bind5,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,A,ArgX,B,C,ArgY,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind2,Bind6,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,A,ArgX,B,C,D,ArgY>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind3,Bind4,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,A,B,ArgX,ArgY,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind3,Bind5,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,A,B,ArgX,C,ArgY,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind3,Bind6,A,B,C,D>
       : public FunType<Sextinary,A,B,ArgX,C,D,ArgY,
       typename RT<Sextinary,A,B,ArgX,C,D,ArgY>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind4,Bind5,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,A,B,C,ArgX,ArgY,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind4,Bind6,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,A,B,C,ArgX,D,ArgY>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind5,Bind6,A,B,C,D>
       : public FunType<Sextinary,ArgX,ArgY,A,B,C,D,
       typename RT<Sextinary,A,B,C,D,ArgX,ArgY>::ResultType >
       { };

     template <typename A,typename B,typename C, typename D>
       struct Sig : public SigN<BinderX,BinderY,A,B,C,D>
     { };
#endif

     template <typename A,typename B,typename C, typename D>
#ifdef FCPP_CONCEPTS
       inline ConstCallable8<Sextinary,BinderX,BinderY,ArgX,ArgY,A,B,C,D>::result_type
#else
       typename Sig<A,B,C,D>::ResultType
#endif
   operator()(const A& a, const B& b, const C& c, const D& d) const
   {
#ifdef FCPP_CONCEPTS
       return binderXandYof6Helper<BinderX,BinderY,
       typename ConstCallable8<Sextinary,BinderX,BinderY,ArgX,ArgY,A,B,C,D>::result_type,
       Sextinary,ArgX,ArgY,A,B,C,D>::go(f,px.second,py.second,a,b,c,d);
#else
     return binderXandYof6Helper<BinderX,BinderY,
       typename Sig<A,B,C,D>::ResultType,Sextinary,
       ArgX,ArgY,A,B,C,D>::go(f,px.second,py.second,a,b,c,d);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Sextinary>() ) {
       std::string temp = std::string("binderXandYof6::")
                        + debug_traits_name(f);
       if (px.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(px.first.n);
       temp += std::string(",") + debug_name_or_value(px.second) + std::string(")");
       if (py.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(py.first.n);
       temp += std::string(",") + debug_name_or_value(py.second) + std::string(")");
       return temp;
     } else
       return std::string("binderXandYof6");
    }
#endif
};

struct BindXandYof6 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXandYof6");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Sextinary, class PairX, class PairY>
   struct Sig 
    : public FunType<Sextinary,PairX,PairY,
    Full4<binderXandYof6<Sextinary,PairX,PairY> > > {};
#endif

  template <class Sextinary, class PairX, class PairY>
#ifdef FCPP_CONCEPTS
    inline Full4<binderXandYof6<Sextinary,PairX,PairY> >
#else
    typename Sig<Sextinary,PairX,PairY>::ResultType
#endif
    operator()( const Sextinary& f,const PairX &px ,const PairY &py ) const {
    return makeFull4( binderXandYof6<Sextinary,PairX,PairY>(f,px,py) );
   }
};


//////////////////////////////////////////////////////////////////////
// bind 3 arguments - there are 20 possibilities.
// Only one is implemented as yet.
//////////////////////////////////////////////////////////////////////

template <class Sextinary, class Arg1, class Arg2, class Arg3>
class binder1to3of6 {
   const Sextinary f;
   const Arg1 a1;
   const Arg2 a2;
   const Arg3 a3;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg4, class Arg5, class Arg6>
   struct Sig
   : public FunType<
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg4Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg5Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg6Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::ResultType> 
   {};
#endif

   binder1to3of6(const Sextinary& w, const Arg1& x, const Arg2& y, const Arg3& z) :
     f(w), a1(x), a2(y), a3(z) {};
   template <class Arg4,class Arg5, class Arg6>
#ifdef FCPP_CONCEPTS
   inline ConstCallable6<Sextinary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::result_type
#else
   typename Sig<Arg4,Arg5,Arg6>::ResultType
#endif
   operator()(const Arg4& u, const Arg5& v, const Arg6& w) const { return f(a1,a2,a3,u,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Sextinary>() ) {
       std::string bindername = std::string("binder1to3of6::")+
              debug_traits_name(f);
       bindername += std::string("( ") + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1to3of6");
   }
#endif

};

struct Bind1to3of6 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to3of6");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Sextinary, class A1, class A2, class A3>
   struct Sig 
    : public FunType<Sextinary,A1,A2,A3,Full3<binder1to3of6<Sextinary,A1,A2,A3> > > {};
#endif

  template <class Sextinary, class A1, class A2, class A3>
  inline  Full3<binder1to3of6<Sextinary,A1,A2,A3> >
   operator()( const Sextinary& f, const A1& a1, const A2& a2, const A3& a3 ) const {
    return makeFull3( binder1to3of6<Sextinary,A1,A2,A3>(f,a1,a2,a3) );
   }
};

////////////////////////////////////////////////////////////////////////
// binderXtoZof6 provides the remainder.
// All that is needed is more specialisations of binderXtoZof6Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////
template <typename BinderX,typename BinderY,typename BinderZ, typename R,typename F,
      typename ArgX, typename ArgY, typename ArgZ, typename A,typename B,typename C>
struct binderXtoZof6Helper;

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind1,Bind2,Bind3,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                     const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(argX,argY,argZ,a,b,c);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind1,Bind2,Bind4,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(argX,argY,a,argZ,b,c);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind1,Bind2,Bind5,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(argX,argY,a,b,argZ,c);
  }
};
 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind1,Bind2,Bind6,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(argX,argY,a,b,c,argZ);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind1,Bind3,Bind4,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(argX,a,argY,argZ,b,c);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind1,Bind3,Bind5,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(argX,a,argY,b,argZ,c);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind1,Bind3,Bind6,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(argX,a,argY,b,c,argZ);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind1,Bind4,Bind5,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(argX,a,b,argY,argZ,c);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind2,Bind3,Bind4,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(a,argX,argY,argZ,b,c);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind2,Bind3,Bind5,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(a,argX,argY,b,argZ,c);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind2,Bind3,Bind6,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(a,argX,argY,b,c,argZ);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind2,Bind4,Bind5,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(a,argX,b,argY,argZ,c);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind2,Bind4,Bind6,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(a,argX,b,argY,c,argZ);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind2,Bind5,Bind6,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(a,argX,b,c,argY,argZ);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C>
struct binderXtoZof6Helper<Bind3,Bind4,Bind5,R,F,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b, const C& c)
  {
    return f(a,b,argX,argY,argZ,c);
  }
};

template <class Sextinary, typename PairX, typename PairY, typename PairZ>
class binderXtoZof6
{
     const Sextinary f;
     PairX px;
     PairY py;
     PairZ pz;
     // These give the types for later
     typedef typename PairX::first_type BinderX;
     typedef typename PairX::second_type ArgX;
     typedef typename PairY::first_type BinderY;
     typedef typename PairY::second_type ArgY;
     typedef typename PairZ::first_type BinderZ;
     typedef typename PairZ::second_type ArgZ;

#ifdef FCPP_CXX11
     // Compile time error messages if any pairs of the binders are the same.
     static_assert(not std::is_same<BinderX,BinderY>::value,
     "***** ERROR in binderXtoZof6 : BinderX==BinderY : ERROR *****");
     static_assert(not std::is_same<BinderX,BinderZ>::value,
     "***** ERROR in binderXtoZof6 : BinderX==BinderZ : ERROR *****");
     static_assert(not std::is_same<BinderY,BinderZ>::value,
     "***** ERROR in binderXtoZof6 : BinderY==BinderZ : ERROR *****");
#endif
public:
     binderXtoZof6( const Sextinary& g,const PairX& qx,const PairY& qy,
                         const PairZ& qz) :
       f(g), px(qx), py(qy), pz(qz) {}

#ifndef FCPP_CONCEPTS
     template <typename BindX, typename BindY, typename BindZ,
                 typename A, typename B, typename C>
     struct SigN;

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind2,Bind3,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,ArgX,ArgY,ArgZ,A,B,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind2,Bind4,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,ArgX,ArgY,A,ArgZ,B,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind2,Bind5,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,ArgX,ArgY,A,B,ArgZ,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind2,Bind6,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,ArgX,ArgY,A,B,C,ArgZ>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind3,Bind4,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,ArgX,A,ArgY,ArgZ,B,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind3,Bind5,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,ArgX,A,ArgY,B,ArgZ,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind3,Bind6,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,ArgX,A,ArgY,B,C,ArgZ>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind4,Bind5,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,ArgX,A,B,ArgY,ArgZ,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind2,Bind3,Bind4,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,A,ArgX,ArgY,ArgZ,B,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind2,Bind3,Bind5,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,A,ArgX,ArgY,B,ArgZ,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind2,Bind3,Bind6,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,A,ArgX,ArgY,B,C,ArgZ>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind2,Bind4,Bind5,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,A,ArgX,B,ArgY,ArgZ,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind2,Bind4,Bind6,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,A,ArgX,B,ArgY,C,ArgZ>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind2,Bind5,Bind6,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,A,ArgX,B,C,ArgY,ArgZ>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind3,Bind4,Bind5,A,B,C>
       : public FunType<Sextinary,ArgX,ArgY,ArgZ,A,B,C,
       typename RT<Sextinary,A,B,ArgX,ArgY,ArgZ,C>::ResultType >
       { };

     template <typename A,typename B,typename C>
       struct Sig : public SigN<BinderX,BinderY,BinderZ,A,B,C>
     { };
#endif

     template <typename A,typename B,typename C>
#ifdef FCPP_CONCEPTS
       inline ConstCallable9<Sextinary,BinderX,BinderY,BinderZ,ArgX,ArgY,ArgZ,A,B,C>::result_type
#else
       typename Sig<A,B,C>::ResultType
#endif
   operator()(const A& a, const B& b, const C& c) const
   {
#ifdef FCPP_CONCEPTS
     return binderXtoZof6Helper<BinderX,BinderY,BinderZ,
       typename ConstCallable9<Sextinary,BinderX,BinderY,BinderZ,ArgX,ArgY,ArgZ,A,B,C>::result_type,
       Sextinary,ArgX,ArgY,ArgZ,A,B,C>::go(f,px.second,py.second,pz.second,a,b,c);
#else
     return binderXtoZof6Helper<BinderX,BinderY,BinderZ,
       typename Sig<A,B,C>::ResultType,Sextinary,
       ArgX,ArgY,ArgZ,A,B,C>::go(f,px.second,py.second,pz.second,a,b,c);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Sextinary>() ) {
       std::string temp = std::string("binderXtoZof6::")
                        + debug_traits_name(f);
       if (px.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(px.first.n);
       temp += std::string(",") + debug_name_or_value(px.second) + std::string(")");
       if (py.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(py.first.n);
       temp += std::string(",") + debug_name_or_value(py.second) + std::string(")");
       if (pz.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(pz.first.n);
       temp += std::string(",") + debug_name_or_value(pz.second) + std::string(")");
       return temp;
     } else
       return std::string("binderXtoZof6");
    }
#endif
};

struct BindXtoZof6 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXtoZof6");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Sextinary, class PairX, class PairY, class PairZ>
   struct Sig
    : public FunType<Sextinary,PairX,PairY,PairZ,
    Full3<binderXtoZof6<Sextinary,PairX,PairY,PairZ> > > {};
#endif

  template <class Sextinary, class PairX, class PairY, class PairZ>
#ifdef FCPP_CONCEPTS
    inline Full3<binderXtoZof6<Sextinary,PairX,PairY,PairZ> >
#else
    typename Sig<Sextinary,PairX,PairY,PairZ>::ResultType
#endif
    operator()( const Sextinary& f,const PairX &px ,const PairY &py, const PairZ &pz ) const
   {
     return makeFull3( binderXtoZof6<Sextinary,PairX,PairY,PairZ>(f,px,py,pz) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 4 arguments - there are 15 possibilities.
// Only one is implemented as yet.
//////////////////////////////////////////////////////////////////////

template <class Sextinary, class Arg1, class Arg2, class Arg3, class Arg4>
class binder1to4of6 {
   const Sextinary f;
   const Arg1 a1;
   const Arg2 a2;
   const Arg3 a3;
   const Arg4 a4;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg5, class Arg6>
   struct Sig
   : public FunType<
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg5Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg6Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::ResultType>
   {};
#endif

   binder1to4of6(const Sextinary& w, const Arg1& x, const Arg2& y, const Arg3& z,const Arg4& u) :
     f(w), a1(x), a2(y), a3(z), a4(u) {};
   template <class Arg5, class Arg6>
#ifdef FCPP_CONCEPTS
   inline ConstCallable6<Sextinary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::result_type
#else
   typename Sig<Arg5,Arg6>::ResultType
#endif
   operator()(const Arg5& v, const Arg6& w) const { return f(a1,a2,a3,a4,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Sextinary>() ) {
       std::string bindername = std::string("binder1to4of6::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" , ") + debug_name_or_value(a4);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1to4of6");
   }
#endif
};

struct Bind1to4of6 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to4of6");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Sextinary, class A1, class A2, class A3, class A4>
   struct Sig
    : public FunType<Sextinary,A1,A2,A3,A4,Full2<binder1to4of6<Sextinary,A1,A2,A3,A4> > > {};
#endif

  template <class Sextinary, class A1, class A2, class A3, class A4>
    inline Full2<binder1to4of6<Sextinary,A1,A2,A3,A4> >
   operator()( const Sextinary& f, const A1& a1, const A2& a2, const A3& a3, const A4& a4 ) const {
    return makeFull2( binder1to4of6<Sextinary,A1,A2,A3,A4>(f,a1,a2,a3,a4) );
   }
};

////////////////////////////////////////////////////////////////////////
// binderWtoZof6 provides the remainder.
// All that is needed is more specialisations of binderWtoZof6Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////
 template <typename BinderW,typename BinderX,typename BinderY,typename BinderZ, typename R,typename F,
   typename ArgW,typename ArgX, typename ArgY, typename ArgZ, typename A, typename B>
struct binderWtoZof6Helper;

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B>
   struct binderWtoZof6Helper<Bind1,Bind2,Bind3,Bind4,R,F,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,
                      const A& a,const B& b)
  {
    return f(argW,argX,argY,argZ,a,b);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B>
   struct binderWtoZof6Helper<Bind1,Bind2,Bind3,Bind5,R,F,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,
                      const A& a,const B& b)
  {
    return f(argW,argX,argY,a,argZ,b);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B>
   struct binderWtoZof6Helper<Bind1,Bind2,Bind3,Bind6,R,F,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,
                      const A& a,const B& b)
  {
    return f(argW,argX,argY,a,b,argZ);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B>
   struct binderWtoZof6Helper<Bind1,Bind2,Bind4,Bind5,R,F,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,
                      const A& a,const B& b)
  {
    return f(argW,argX,a,argY,argZ,b);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B>
   struct binderWtoZof6Helper<Bind1,Bind2,Bind4,Bind6,R,F,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,
                      const A& a,const B& b)
  {
    return f(argW,argX,a,argY,b,argZ);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B>
   struct binderWtoZof6Helper<Bind1,Bind3,Bind4,Bind5,R,F,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,
                      const A& a,const B& b)
  {
    return f(argW,a,argX,argY,argZ,b);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B>
   struct binderWtoZof6Helper<Bind1,Bind3,Bind4,Bind6,R,F,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,
                      const A& a,const B& b)
  {
    return f(argW,a,argX,argY,b,argZ);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B>
   struct binderWtoZof6Helper<Bind2,Bind3,Bind4,Bind5,R,F,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,
                      const A& a,const B& b)
  {
    return f(a,argW,argX,argY,argZ,b);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B>
   struct binderWtoZof6Helper<Bind2,Bind3,Bind4,Bind6,R,F,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a,const B& b)
  {
    return f(a,argW,argX,argY,b,argZ);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B>
   struct binderWtoZof6Helper<Bind3,Bind4,Bind5,Bind6,R,F,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,
		      const A& a,const B& b)
  {
    return f(a,b,argW,argX,argY,argZ);
  }
};

template <class Sextinary, typename PairW, typename PairX, typename PairY, typename PairZ>
class binderWtoZof6
{
     const Sextinary f;
     PairW pw;
     PairX px;
     PairY py;
     PairZ pz;
     // These give the types for later
     typedef typename PairW::first_type BinderW;
     typedef typename PairW::second_type ArgW;
     typedef typename PairX::first_type BinderX;
     typedef typename PairX::second_type ArgX;
     typedef typename PairY::first_type BinderY;
     typedef typename PairY::second_type ArgY;
     typedef typename PairZ::first_type BinderZ;
     typedef typename PairZ::second_type ArgZ;
public:
     binderWtoZof6( const Sextinary& g, const PairW& qw, const PairX& qx,
                    const PairY& qy, const PairZ& qz) :
       f(g), pw(qw), px(qx), py(qy), pz(qz) {}

#ifndef FCPP_CONCEPTS
       template <typename BindW, typename BindX, typename BindY, typename BindZ, 
       typename A,typename B>
     struct SigN;

       template <typename A,typename B>
       struct  SigN<Bind1,Bind2,Bind3,Bind4,A,B>
       : public FunType<Sextinary,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Sextinary,ArgW,ArgX,ArgY,ArgZ,A,B>::ResultType >
       { };

     template <typename A,typename B>
       struct  SigN<Bind1,Bind2,Bind3,Bind5,A,B>
       : public FunType<Sextinary,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Sextinary,ArgW,ArgX,ArgY,A,ArgZ,B>::ResultType >
       { };

     template <typename A,typename B>
       struct  SigN<Bind1,Bind2,Bind3,Bind6,A,B>
       : public FunType<Sextinary,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Sextinary,ArgW,ArgX,ArgY,A,B,ArgZ>::ResultType >
       { };

     template <typename A,typename B>
       struct  SigN<Bind1,Bind2,Bind4,Bind5,A,B>
       : public FunType<Sextinary,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Sextinary,ArgW,ArgX,A,ArgY,ArgZ,B>::ResultType >
       { };

     template <typename A,typename B>
       struct  SigN<Bind1,Bind2,Bind4,Bind6,A,B>
       : public FunType<Sextinary,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Sextinary,ArgW,ArgX,A,ArgY,B,ArgZ>::ResultType >
       { };

     template <typename A,typename B>
       struct  SigN<Bind1,Bind3,Bind4,Bind5,A,B>
       : public FunType<Sextinary,ArgW,A,ArgX,ArgY,ArgZ,B,
	 typename RT<Sextinary,ArgW,A,ArgX,ArgY,ArgZ,B>::ResultType >
       { };

     template <typename A,typename B>
       struct  SigN<Bind1,Bind3,Bind4,Bind6,A,B>
       : public FunType<Sextinary,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Sextinary,ArgW,A,ArgX,ArgY,B,ArgZ>::ResultType >
       { };

     template <typename A,typename B>
       struct  SigN<Bind2,Bind3,Bind4,Bind5,A,B>
       : public FunType<Sextinary,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Sextinary,A,ArgW,ArgX,ArgY,ArgZ,B>::ResultType >
       { };

     template <typename A,typename B>
       struct  SigN<Bind2,Bind3,Bind4,Bind6,A,B>
       : public FunType<Sextinary,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Sextinary,A,ArgW,ArgX,ArgY,B,ArgZ>::ResultType >
       { };

      template <typename A,typename B>
       struct  SigN<Bind3,Bind4,Bind5,Bind6,A,B>
	: public FunType<Sextinary,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Sextinary,A,B,ArgW,ArgX,ArgY,ArgZ>::ResultType >
       { };

     template <typename A,typename B>
       struct Sig : public SigN<BinderW,BinderX,BinderY,BinderZ,A,B>
     { };
#endif


       template <typename A, typename B>
#ifdef FCPP_CONCEPTS
       inline ConstCallable10<Sextinary,BinderW,BinderX,BinderY,BinderZ,ArgW,ArgX,ArgY,ArgZ,A,B>::result_type
#else
       typename Sig<A,B>::ResultType 
#endif
       operator()(const A& a,const B& b) const 
   { 
#ifdef FCPP_CONCEPTS
     return binderWtoZof6Helper<BinderW,BinderX,BinderY,BinderZ,
       typename ConstCallable10<Sextinary,BinderW,BinderX,BinderY,BinderZ,ArgW,ArgX,ArgY,ArgZ,A,B>::result_type,
       Sextinary,ArgW,ArgX,ArgY,ArgZ,A,B>::go(f,pw.second,px.second,py.second,pz.second,a,b);
#else
     return binderWtoZof6Helper<BinderW,BinderX,BinderY,BinderZ,
       typename Sig<A,B>::ResultType,Sextinary,          
       ArgW,ArgX,ArgY,ArgZ,A,B>::go(f,pw.second,px.second,py.second,pz.second,a,b);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Sextinary>() ) {
       std::string temp = std::string("binderWtoZof6::")
                        + debug_traits_name(f);
       if (pw.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(pw.first.n);
       temp += std::string(",") + debug_name_or_value(pw.second) + std::string(")");
       if (px.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(px.first.n);
       temp += std::string(",") + debug_name_or_value(px.second) + std::string(")");
       if (py.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(py.first.n);
       temp += std::string(",") + debug_name_or_value(py.second) + std::string(")");
       if (pz.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(pz.first.n);
       temp += std::string(",") + debug_name_or_value(pz.second) + std::string(")");
       return temp;
     } else
       return std::string("binderWtoZof6");
    }
#endif
};

struct BindWtoZof6 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindWtoZof6");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Sextinary,class PairW,class PairX, class PairY, class PairZ>
   struct Sig 
    : public FunType<Sextinary,PairW,PairX,PairY,PairZ,
    Full2<binderWtoZof6<Sextinary,PairW,PairX,PairY,PairZ> > > {};
#endif

  template <class Sextinary,class PairW,class PairX, class PairY, class PairZ>
#ifdef FCPP_CONCEPTS
    inline Full2<binderWtoZof6<Sextinary,PairW,PairX,PairY,PairZ> >
#else
    typename Sig<Sextinary,PairW,PairX,PairY,PairZ>::ResultType
#endif
    operator()( const Sextinary& f,const PairW &pw ,const PairX &px ,const PairY &py, const PairZ &pz ) const 
   {
     return makeFull2( binderWtoZof6<Sextinary,PairW,PairX,PairY,PairZ>(f,pw,px,py,pz) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 5 arguments - there are 6 possibilities.
// Only one is implemented as yet.
//////////////////////////////////////////////////////////////////////

template <class Sextinary, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
class binder1to5of6 {
   const Sextinary f;
   const Arg1 a1;
   const Arg2 a2;
   const Arg3 a3;
   const Arg4 a4;
   const Arg5 a5;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg6>
   struct Sig 
   : public FunType<
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::Arg6Type,
                    typename Sextinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::ResultType> 
   {};
#endif

   binder1to5of6(const Sextinary& w, const Arg1& x, const Arg2& y,
                               const Arg3& z,const Arg4& u,const Arg5& v) :
     f(w), a1(x), a2(y), a3(z), a4(u), a5(v) {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "binder1to5of6 constructor called" << std::endl;
#endif
     };
   template <class Arg6>
#ifdef FCPP_CONCEPTS
   inline ConstCallable6<Sextinary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6>::result_type
#else
   typename Sig<Arg6>::ResultType
#endif
   operator()(const Arg6& w) const {
#ifdef FCPP_PRINT_HELPERS6
     std::cout << "binder1to5of6 operator() called" << std::endl;
#endif
      return f(a1,a2,a3,a4,a5,w);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     if ( debug_traits_has_name<Sextinary>() ) {
       std::string bindername = std::string("binder1to5of6::")
         + debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" , ") + debug_name_or_value(a4);
       bindername += std::string(" , ") + debug_name_or_value(a5);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1to5of6");
   }
#endif
};

struct Bind1to5of6 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to5of6");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Sextinary, class A1, class A2, class A3, class A4, class A5>
   struct Sig
    : public FunType<Sextinary,A1,A2,A3,A4,A5,Full1<binder1to5of6<Sextinary,A1,A2,A3,A4,A5> > > {};
#endif

  template <class Sextinary, class A1, class A2, class A3, class A4, class A5>
    inline Full1<binder1to5of6<Sextinary,A1,A2,A3,A4,A5> >
   operator()( const Sextinary& f, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5 ) const {
    return makeFull1( binder1to5of6<Sextinary,A1,A2,A3,A4,A5>(f,a1,a2,a3,a4,a5) );
   }
};


////////////////////////////////////////////////////////////////////////
// binderVtoZof6 provides the remainder.
// All that is needed is more specialisations of binderVtoZof6Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////
 template <typename BinderV,typename BinderW,typename BinderX,
           typename BinderY,typename BinderZ, typename R,typename F,
           typename ArgV,typename ArgW,typename ArgX,typename ArgY,
           typename ArgZ, typename A>
struct binderVtoZof6Helper;

 template <typename R,typename F,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A>
   struct binderVtoZof6Helper<Bind1,Bind2,Bind3,Bind4,Bind5,R,F,
                                           ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f,const ArgV& argV,const ArgW& argW,
              const ArgX& argX, const ArgY& argY, const ArgZ& argZ,const A& a) 
  {
    return f(argV,argW,argX,argY,argZ,a);
  }
};

 template <typename R,typename F,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A>
   struct binderVtoZof6Helper<Bind1,Bind2,Bind3,Bind4,Bind6,R,F,
                                           ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f,const ArgV& argV,const ArgW& argW,
              const ArgX& argX, const ArgY& argY, const ArgZ& argZ,const A& a) 
  {
    return f(argV,argW,argX,argY,a,argZ);
  }
};

 template <typename R,typename F,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A>
   struct binderVtoZof6Helper<Bind1,Bind2,Bind3,Bind5,Bind6,R,F,
                                           ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f,const ArgV& argV,const ArgW& argW,
              const ArgX& argX, const ArgY& argY, const ArgZ& argZ,const A& a) 
  {
    return f(argV,argW,argX,a,argY,argZ);
  }
};

 template <typename R,typename F,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A>
   struct binderVtoZof6Helper<Bind1,Bind2,Bind4,Bind5,Bind6,R,F,
                                           ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f,const ArgV& argV,const ArgW& argW,
              const ArgX& argX, const ArgY& argY, const ArgZ& argZ,const A& a) 
  {
    return f(argV,argW,a,argX,argY,argZ);
  }
};

 template <typename R,typename F,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A>
   struct binderVtoZof6Helper<Bind1,Bind3,Bind4,Bind5,Bind6,R,F,
                                           ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f,const ArgV& argV,const ArgW& argW,
              const ArgX& argX, const ArgY& argY, const ArgZ& argZ,const A& a) 
  {
    return f(argV,a,argW,argX,argY,argZ);
  }
};

 template <typename R,typename F,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A>
   struct binderVtoZof6Helper<Bind2,Bind3,Bind4,Bind5,Bind6,R,F,
                                           ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f,const ArgV& argV,const ArgW& argW,
              const ArgX& argX, const ArgY& argY, const ArgZ& argZ,const A& a) 
  {
    return f(a,argV,argW,argX,argY,argZ);
  }
};


 template <class Sextinary, typename PairV,typename PairW, typename PairX,
                            typename PairY,typename PairZ>
class binderVtoZof6
{
     const Sextinary f;
     PairV pv;
     PairW pw;
     PairX px;
     PairY py;
     PairZ pz;
     // These give the types for later
     typedef typename PairV::first_type BinderV;
     typedef typename PairV::second_type ArgV;
     typedef typename PairW::first_type BinderW;
     typedef typename PairW::second_type ArgW;
     typedef typename PairX::first_type BinderX;
     typedef typename PairX::second_type ArgX;
     typedef typename PairY::first_type BinderY;
     typedef typename PairY::second_type ArgY;
     typedef typename PairZ::first_type BinderZ;
     typedef typename PairZ::second_type ArgZ;
public:
     binderVtoZof6( const Sextinary& g,const PairV& qv,const PairW& qw,
                    const PairX& qx,const PairY& qy,const PairZ& qz) :
       f(g), pv(qv), pw(qw), px(qx), py(qy), pz(qz) {}

#ifndef FCPP_CONCEPTS
       template <typename BindV,typename BindW, typename BindX, typename BindY, typename BindZ, 
       typename A>
     struct SigN;

       template <typename A>
	 struct  SigN<Bind1,Bind2,Bind3,Bind4,Bind5,A>
	 : public FunType<Sextinary,ArgV,ArgW,ArgX,ArgY,ArgZ,A,
	 typename RT<Sextinary,ArgV,ArgW,ArgX,ArgY,ArgZ,A>::ResultType >
       { };

       template <typename A>
	 struct  SigN<Bind1,Bind2,Bind3,Bind4,Bind6,A>
	 : public FunType<Sextinary,ArgV,ArgW,ArgX,ArgY,ArgZ,A,
	 typename RT<Sextinary,ArgV,ArgW,ArgX,ArgY,A,ArgZ>::ResultType >
       { };

       template <typename A>
	 struct  SigN<Bind2,Bind3,Bind4,Bind5,Bind6,A>
	 : public FunType<Sextinary,ArgV,ArgW,ArgX,ArgY,ArgZ,A,
	 typename RT<Sextinary,A,ArgV,ArgW,ArgX,ArgY,ArgZ>::ResultType >
       { };

     template <typename A>
       struct Sig : public SigN<BinderV,BinderW,BinderX,BinderY,BinderZ,A>
     { };
#endif


       template <typename A>
#ifdef FCPP_CONCEPTS
	 inline ConstCallable11<Sextinary,BinderV,BinderW,BinderX,BinderY,BinderZ,
              	                          ArgV,ArgW,ArgX,ArgY,ArgZ,A>::result_type
#else
       typename Sig<A>::ResultType
#endif
       operator()(const A& a) const
   {
#ifdef FCPP_CONCEPTS
     return binderVtoZof6Helper<BinderV,BinderW,BinderX,BinderY,BinderZ,
       typename ConstCallable11<Sextinary,BinderV,BinderW,BinderX,BinderY,BinderZ,
                                          ArgV,ArgW,ArgX,ArgY,ArgZ,A>::result_type,
       Sextinary,ArgV,ArgW,ArgX,ArgY,ArgZ,A>::go(f,pv.second,pw.second,
                                          px.second,py.second,pz.second,a);
#else
     return binderVtoZof6Helper<BinderV,BinderW,BinderX,BinderY,BinderZ,
       typename Sig<A>::ResultType,Sextinary,
       ArgV,ArgW,ArgX,ArgY,ArgZ,A>::go(f,pv.second,pw.second,
                                       px.second,py.second,pz.second,a);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Sextinary>() ) {
       std::string temp = std::string("binderVtoZof6::")
                        + debug_traits_name(f);
       if (pv.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(pv.first.n);
       temp += std::string(",") + debug_name_or_value(pv.second) + std::string(")");
       if (pw.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(pw.first.n);
       temp += std::string(",") + debug_name_or_value(pw.second) + std::string(")");
       if (px.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(px.first.n);
       temp += std::string(",") + debug_name_or_value(px.second) + std::string(")");
       if (py.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(py.first.n);
       temp += std::string(",") + debug_name_or_value(py.second) + std::string(")");
       if (pz.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(pz.first.n);
       temp += std::string(",") + debug_name_or_value(pz.second) + std::string(")");
       return temp;
     } else
       return std::string("binderVtoZof6");
    }
#endif
};

struct BindVtoZof6 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindVtoZof6");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Sextinary,class PairV,class PairW,class PairX, class PairY, class PairZ>
   struct Sig 
    : public FunType<Sextinary,PairV,PairW,PairX,PairY,PairZ,
    Full1<binderVtoZof6<Sextinary,PairV,PairW,PairX,PairY,PairZ> > > {};
#endif

  template <class Sextinary,class PairV,class PairW,class PairX, class PairY, class PairZ>
#ifdef FCPP_CONCEPTS
    inline Full1<binderVtoZof6<Sextinary,PairV,PairW,PairX,PairY,PairZ> >
#else
    typename Sig<Sextinary,PairV,PairW,PairX,PairY,PairZ>::ResultType
#endif
    operator()( const Sextinary& f,const PairV &pv ,const PairW &pw ,
                const PairX &px ,const PairY &py, const PairZ &pz ) const
   {
     return makeFull1( binderVtoZof6<Sextinary,PairV,PairW,PairX,PairY,PairZ>
                       (f,pv,pw,px,py,pz) );
   }
};


//////////////////////////////////////////////////////////////////////
// bind all 6 arguments
// Implementation involves Full7.
/////////////////////////////////////////////////////////////////////

 template <class Sextinary, class A1, class A2, class A3, class A4, class A5, class A6>
class binder1to6of6
#ifndef FCPP_CONCEPTS
   : public CFunType<typename RT<Sextinary,A1,A2,A3,A4,A5,A6>::ResultType>
#endif
{
   const Sextinary f;
   const A1 a1;
   const A2 a2;
   const A3 a3;
   const A4 a4;
   const A5 a5;
   const A6 a6;
public:
   binder1to6of6( const Sextinary& g,
    const A1& x, const A2& y, const A3& z, const A4& v, const A5& w, const A6& u )
  : f(g), a1(x), a2(y), a3(z), a4(v), a5(w), a6(u) {}

#ifdef FCPP_CONCEPTS
  inline ConstCallable6<Sextinary,A1,A2,A3,A4,A5,A6>::result_type
#else
 typename RT<Sextinary,A1,A2,A3,A4,A5,A6>::ResultType
#endif
   operator()() const
    {
#ifdef FCPP_PRINT_HELPERS6
       std::cout << "call to binder1to6of6" << std::endl;
#endif
       return f(a1,a2,a3,a4,a5,a6);
    }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     if ( debug_traits_has_name<Sextinary>() ) {
       std::string bindername = std::string("binder1to6of6::")
         + debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" , ") + debug_name_or_value(a4);
       bindername += std::string(" , ") + debug_name_or_value(a5);
       bindername += std::string(" , ") + debug_name_or_value(a6);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1to6of6");
   }
#endif
};

struct Bind1to6of6 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to6of6");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Sextinary, class A1, class A2, class A3, class A4, class A5, class A6>
   struct Sig
    : public FunType<Sextinary,A1,A2,A3,A4,A5,A6,
    Full0<binder1to6of6<Sextinary,A1,A2,A3,A4,A5,A6> > > {};
#endif

  template <class Sextinary, class A1, class A2, class A3, class A4, class A5,class A6>
    inline Full0<binder1to6of6<Sextinary,A1,A2,A3,A4,A5,A6> >
    operator()( const Sextinary& f, const A1& a1, const A2& a2, const A3& a3,
               const A4& a4, const A5& a5, const A6& a6) const {
#ifdef FCPP_PRINT_HELPERS6
    std::cout << "Bind1to6of6 operator() called" << std::endl;
#endif
    return makeFull0( binder1to6of6<Sextinary,A1,A2,A3,A4,A5,A6>
		      (f,a1,a2,a3,a4,a5,a6) );
   }
};

// Defined only for the full binding.
struct Curry6 {

#ifndef FCPP_CONCEPTS
  template <class Sextinary, class A1, class A2 = void, class A3 = void,
                        class A4 = void, class A5 = void, class A6 = void>
    struct Sig  // Now available.
    : public FunType<Sextinary, A1, A2, A3, A4, A5, A6,
    Full0<binder1to6of6<Sextinary,A1,A2,A3,A4,A5,A6> > > {};

  template <class Sextinary>
  struct Sig<Sextinary, void >
    : public Full6<Sextinary> {};

  template <class Sextinary, class A1> struct Sig<Sextinary, A1>
    : public FunType<Sextinary, A1, void, void, void, void,
                   Full5<binder1of6<Sextinary,A1> > > {};

  template <class Sextinary, class A1, class A2>
  struct Sig<Sextinary, A1, A2>
    : public FunType<Sextinary, A1, A2, void, void, void ,
                   Full4<binder1and2of6<Sextinary,A1,A2> > > {};

  template <class Sextinary, class A1, class A2, class A3>
      struct Sig<Sextinary, A1, A2, A3>
    : public FunType<Sextinary, A1, A2, A3, void, void ,
    Full3<binder1to3of6<Sextinary,A1,A2,A3> > > {};

  template <class Sextinary, class A1, class A2, class A3, class A4>
    struct Sig<Sextinary, A1, A2, A3, A4>
    : public FunType<Sextinary, A1, A2, A3, A4, void ,
    Full2<binder1to4of6<Sextinary,A1,A2,A3,A4> > > {};

  template <class Sextinary, class A1, class A2, class A3, class A4, class A5>
    struct Sig<Sextinary, A1, A2, A3, A4, A5>
    : public FunType<Sextinary, A1, A2, A3, A4, A5, void,
    Full1<binder1to5of6<Sextinary,A1,A2,A3,A4,A5> > > {};
#endif

  template <class Sextinary>
  inline Sextinary
  operator()( const Sextinary& f) const {
    bool b = &FunctoidTraits<Sextinary>::template ensure_accepts<6>::args;
    assert(FunctoidTraits_max_args<Sextinary>() == 6);
    return f;
  }

  template <class Sextinary, class A1>
   inline Full5<binder1of6<Sextinary,A1> >
  operator()( const Sextinary& f, const A1& a1 ) const {
    return makeFull5( binder1of6<Sextinary,A1>(f,a1) );
  }

  template <class Sextinary, class A1, class A2>
    inline Full4<binder1and2of6<Sextinary,A1,A2> >
  operator()( const Sextinary& f, const A1& a1, const A2& a2 ) const {
    return makeFull4( binder1and2of6<Sextinary,A1,A2>(f,a1,a2) );
  }

  template <class Sextinary, class A1, class A2, class A3>
    inline Full3<binder1to3of6<Sextinary,A1,A2,A3> >
  operator()( const Sextinary& f, const A1& a1, const A2& a2, const A3& a3 ) const {
    return makeFull3( binder1to3of6<Sextinary,A1,A2,A3>(f,a1,a2,a3) );
  }

 template <class Sextinary, class A1, class A2, class A3, class A4>
#ifdef FCPP_CONCEPTS
  inline Full2<binder1to4of6<Sextinary,A1,A2,A3,A4> >
#else
  typename Sig<Sextinary, A1, A2, A3, A4>::ResultType
#endif
  operator()( const Sextinary& f, const A1& a1, const A2& a2,
              const A3& a3, const A4& a4 ) const {
#ifdef FCPP_PRINT_HELPERS6
    std::cout << "Curry6 called for 4 args" << std::endl;
#endif
    return makeFull2( binder1to4of6<Sextinary,A1,A2,A3,A4>
                      (f,a1,a2,a3,a4) );
  }

  template <class Sextinary, class A1, class A2, class A3, class A4, class A5>
#ifdef FCPP_CONCEPTS
  inline Full1<binder1to5of6<Sextinary,A1,A2,A3,A4,A5> >
#else
  typename Sig<Sextinary, A1, A2, A3, A4, A5>::ResultType
#endif
  operator()( const Sextinary& f, const A1& a1, const A2& a2,
              const A3& a3, const A4& a4, const A5& a5) const {
    return makeFull1( binder1to5of6<Sextinary,A1,A2,A3,A4,A5>
		      (f,a1,a2,a3,a4,a5) );
  }

  template <class Sextinary, class A1, class A2, class A3, class A4, class A5, class A6>
#ifdef FCPP_CONCEPTS
  inline Full0<binder1to6of6<Sextinary,A1,A2,A3,A4,A5,A6> >
#else
  typename Sig<Sextinary, A1, A2, A3, A4, A5, A6>::ResultType
#endif
  operator()( const Sextinary& f, const A1& a1, const A2& a2,
              const A3& a3, const A4& a4, const A5& a5, const A6& a6 ) const {
    return makeFull0( binder1to6of6<Sextinary,A1,A2,A3,A4,A5,A6>
		      (f,a1,a2,a3,a4,a5,a6) );
  }

#ifdef FCPP_DEBUG
   std::string name() const {
       return std::string("Curry6");
   }
#endif

};


#endif
