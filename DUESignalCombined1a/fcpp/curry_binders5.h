// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_BINDERS5_DOT_H
#define FCPP_CURRY_BINDERS5_DOT_H

//////////////////////////////////////////////////////////////////////
// These are the binders for Full5 and Fun5 - removed from curry.h.
// Concept maps are in curry_concept_maps.h
//////////////////////////////////////////////////////////////////////
// bind 1 argument of 5 - there are 5 possibilities.
// Only the first two are implemented for now, to do full5 for
// four argument member functions, and bind1to4of4.
// Also now bindAof5 to do the remainder.
// Now extended to bindXof5.
//////////////////////////////////////////////////////////////////////

template <class Quintinary, class Arg1>
class binder1of5 {
   const Quintinary f;
   const Arg1 x;
public:
   binder1of5( const Quintinary& a, const Arg1& b ) : f(a), x(b) {}

#ifndef FCPP_CONCEPTS
   template <class Arg2, class Arg3, class Arg4, class Arg5> struct Sig
   : public FunType<typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg2Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg3Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg4Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg5Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::ResultType> {};
#endif

   template <class Arg2, class Arg3, class Arg4, class Arg5>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<Quintinary,Arg1,Arg2,Arg3,Arg4,Arg5>::result_type
#else
   typename RT<Quintinary,Arg1,Arg2,Arg3,Arg4,Arg5>::ResultType
#endif
   operator()( const Arg2& y, const Arg3& z, const Arg4& v , const Arg5& w) const {
      return f(x,y,z,v,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quintinary>() ) {
       std::string bindername = std::string("binder1of5::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(x);
       bindername += std::string(" )");
       return bindername;
     } else
        return std::string("binder1of5");
   }
#endif
};

struct Bind1of5 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1of5");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Quintinary, class Arg1>
   struct Sig
   : public FunType<Quintinary,Arg1,Full4<binder1of5<Quintinary,Arg1> > > {};
#endif

   template <class Quintinary, class Arg1>
#ifdef FCPP_CONCEPTS
   inline Full4<binder1of5<Quintinary,Arg1> >
#else
   typename Sig<Quintinary,Arg1>::ResultType
#endif
   operator()( const Quintinary& f, const Arg1& x ) const {
      return makeFull4( binder1of5<Quintinary,Arg1>(f,x) );
   }
};

template <class Quintinary, class Arg2>
class binder2of5 {
   const Quintinary f;
   const Arg2 y;
public:
   binder2of5( const Quintinary& a, const Arg2& b ) : f(a), y(b) {}

#ifndef FCPP_CONCEPTS
   template <class Arg1, class Arg3, class Arg4, class Arg5> struct Sig 
   : public FunType<typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg1Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg3Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg4Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg5Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::ResultType> {};
#endif

   template <class Arg1, class Arg3, class Arg4, class Arg5>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<Quintinary,Arg1,Arg2,Arg3,Arg4,Arg5>::result_type
#else
   typename RT<Quintinary,Arg1,Arg2,Arg3,Arg4,Arg5>::ResultType
#endif
   operator()( const Arg1& x, const Arg3& z, const Arg4& v , const Arg5& w) const {
      return f(x,y,z,v,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quintinary>() ) {
       std::string bindername = std::string("binder2of5::")+
              debug_traits_name(f);
       bindername += std::string("( _ , ")  + debug_name_or_value(y);
       bindername += std::string(" )");
       return bindername;
     } else
        return std::string("binder2of5");
   }
#endif
};

struct Bind2of5 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind2of5");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Quintinary, class Arg2>
   struct Sig
   : public FunType<Quintinary,Arg2,Full4<binder2of5<Quintinary,Arg2> > > {};
#endif

   template <class Quintinary, class Arg2>
#ifdef FCPP_CONCEPTS
   inline Full4<binder2of5<Quintinary,Arg2> >
#else
   typename Sig<Quintinary,Arg2>::ResultType
#endif
   operator()( const Quintinary& f, const Arg2& y ) const {
      return makeFull4( binder2of5<Quintinary,Arg2>(f,y) );
   }
};

 template <typename Binder,typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D>
struct binderAof5Helper;

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D>
   struct binderAof5Helper<Bind1,R,F,ArgA,A,B,C,D>
{
  static inline R go( const F& f, const ArgA& argA,
      const A& a, const B& b, const C& c, const D& d) 
  {
    return f(argA,a,b,c,d);
  }
};

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D>
   struct binderAof5Helper<Bind2,R,F,ArgA,A,B,C,D>
{
  static inline R go( const F& f, const ArgA& argA,
      const A& a, const B& b, const C& c, const D& d) 
  {
    return f(a,argA,b,c,d);
  }
};

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D>
   struct binderAof5Helper<Bind3,R,F,ArgA,A,B,C,D>
{
  static inline R go( const F& f, const ArgA& argA,
      const A& a, const B& b, const C& c, const D& d) 
  {
    return f(a,b,argA,c,d);
  }
};

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D>
   struct binderAof5Helper<Bind4,R,F,ArgA,A,B,C,D>
{
  static inline R go( const F& f, const ArgA& argA,
      const A& a, const B& b, const C& c, const D& d) 
  {
    return f(a,b,c,argA,d);
  }
};

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D>
   struct binderAof5Helper<Bind5,R,F,ArgA,A,B,C,D>
{
  static inline R go( const F& f, const ArgA& argA,
      const A& a, const B& b, const C& c, const D& d) 
  {
    return f(a,b,c,d,argA);
  }
};

template <class Quintinary, typename Binder,typename ArgA>
class binderAof5
{
  // This does not store the arg as a tuple - see binderAofN for that.
   const Quintinary f;
   const Binder binder;
   const ArgA argA;
public:
     binderAof5( const Quintinary& g,const Binder& b , const ArgA& a) : f(g),
     binder(b),argA(a) {}

#ifndef FCPP_CONCEPTS

     template <typename Bind, typename A, typename B, typename C, typename D>
     struct SigN;

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,A,B,C,D>
       : public FunType<Quintinary,ArgA,A,B,C,D,
         typename RT<Quintinary,ArgA,A,B,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind2,A,B,C,D>
       : public FunType<Quintinary,ArgA,A,B,C,D,
       typename RT<Quintinary,A,ArgA,B,C,D>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D>
       struct  SigN<Bind3,A,B,C,D>
       : public FunType<Quintinary,ArgA,A,B,C,D,
       typename RT<Quintinary,A,B,ArgA,C,D>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D>
       struct  SigN<Bind4,A,B,C,D>
       : public FunType<Quintinary,ArgA,A,B,C,D,
       typename RT<Quintinary,A,B,C,ArgA,D>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D>
       struct  SigN<Bind5,A,B,C,D>
       : public FunType<Quintinary,ArgA,A,B,C,D,
       typename RT<Quintinary,A,B,C,D,ArgA>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D>
     struct Sig : public SigN<Binder,A,B,C,D>
     { };

#endif

     template <typename A,typename B,typename C,typename D>
#ifdef FCPP_CONCEPTS
       inline ConstCallable6<Quintinary,Binder,ArgA,A,B,C,D>::result_type
#else
       typename Sig<A,B,C,D>::ResultType
#endif
       operator()(const A& a, const B& b, const C& c, const D& d) const
   {
#ifdef FCPP_CONCEPTS
      return binderAof5Helper<Binder,
        typename ConstCallable6<Quintinary,Binder,ArgA,A,B,C,D>::result_type,
        Quintinary,ArgA,A,B,C,D>::go(f,argA,a,b,c,d);
#else
      return binderAof5Helper<Binder,
       typename Sig<A,B,C,D>::ResultType,Quintinary,
       ArgA,A,B,C,D>::go(f,argA,a,b,c,d);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Quintinary>() ) {
       std::string temp = std::string("binderAof5::")
                        + debug_traits_name(f);
       if (binder.n > 0)
          temp += std::string("(Arg") + argument_traits_value(binder.n);
       temp += std::string(",") + debug_name_or_value(argA) + std::string(")");
       return temp;
     } else
       return std::string("binderAof5");
    }
#endif
};

struct BindAof5 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindAof5");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Quintinary, class Binder,class ArgA>
   struct Sig 
    : public FunType<Quintinary,Binder,ArgA,Full4<binderAof5<Quintinary,Binder,ArgA> > > {};
#endif

  template <class Quintinary, class Binder,class ArgA>
#ifdef FCPP_CONCEPTS
    inline Full4<binderAof5<Quintinary,Binder,ArgA> >
#else
    typename Sig<Quintinary,Binder,ArgA>::ResultType
#endif
    operator()( const Quintinary& f,const Binder &b, const ArgA& a ) const {
    return makeFull4( binderAof5<Quintinary,Binder,ArgA>(f,b,a) );
   }
};

//////////////////////////////////////////////////////////////////////////////
// Idea - make the binder and the arg a std::pair.
// This will reduce the no of args to be passed for the
// case with more than one.
// This defines its own sig and can use the helper functions
// of binderAof5.
//////////////////////////////////////////////////////////////////////////////
template <class Quintinary, typename Pair>
class binderXof5
{
     const Quintinary f;
     Pair p;
     // These give the types for later
     typedef typename Pair::first_type Binder;
     typedef typename Pair::second_type ArgA;
public:
   binderXof5( const Quintinary& g,const Pair& q) : f(g), 
     p(q) {}

#ifndef FCPP_CONCEPTS

     template <typename Bind, typename A, typename B, typename C, typename D>
     struct SigN;

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,A,B,C,D>
       : public FunType<Quintinary,ArgA,A,B,C,D,
         typename RT<Quintinary,ArgA,A,B,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind2,A,B,C,D>
       : public FunType<Quintinary,ArgA,A,B,C,D,
       typename RT<Quintinary,A,ArgA,B,C,D>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D>
       struct  SigN<Bind3,A,B,C,D>
       : public FunType<Quintinary,ArgA,A,B,C,D,
       typename RT<Quintinary,A,B,ArgA,C,D>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D>
       struct  SigN<Bind4,A,B,C,D>
       : public FunType<Quintinary,ArgA,A,B,C,D,
       typename RT<Quintinary,A,B,C,ArgA,D>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D>
       struct  SigN<Bind5,A,B,C,D>
       : public FunType<Quintinary,ArgA,A,B,C,D,
       typename RT<Quintinary,A,B,C,D,ArgA>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D>
     struct Sig : public SigN<Binder,A,B,C,D>
     { };

#endif

     template <typename A,typename B,typename C,typename D>
#ifdef FCPP_CONCEPTS
       inline ConstCallable6<Quintinary,Binder,ArgA,A,B,C,D>::result_type
#else
       typename Sig<A,B,C,D>::ResultType
#endif
   operator()(const A& a, const B& b, const C& c, const D& d) const
   {
#ifdef FCPP_CONCEPTS
      return binderAof5Helper<Binder,
        typename ConstCallable6<Quintinary,Binder,ArgA,A,B,C,D>::result_type,
        Quintinary,ArgA,A,B,C,D>::go(f,p.second,a,b,c,d);
#else
     return binderAof5Helper<Binder,
       typename Sig<A,B,C,D>::ResultType,Quintinary,
       ArgA,A,B,C,D>::go(f,p.second,a,b,c,d);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Quintinary>() ) {
       std::string temp = std::string("binderXof5::")
                        + debug_traits_name(f);
       if (p.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(p.first.n);
       temp += std::string(",") + debug_name_or_value(p.second) + std::string(")");
       return temp;
     } else
       return std::string("binderXof5");
    }
#endif
};

struct BindXof5 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXof5");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Quintinary, class Pair>
   struct Sig 
    : public FunType<Quintinary,Pair,Full4<binderXof5<Quintinary,Pair> > > {};
#endif

  template <class Quintinary, class Pair>
#ifdef FCPP_CONCEPTS
    inline Full4<binderXof5<Quintinary,Pair> >
#else
    typename Sig<Quintinary,Pair>::ResultType
#endif
    operator()( const Quintinary& f,const Pair &p ) const {
    return makeFull4( binderXof5<Quintinary,Pair>(f,p) );
   }
};

//////////////////////////////////////////////////////////////////////
// bind 2 arguments - there are 10 possibilities.
// Only one is implemented as yet.
// binderXandYof5 provides the remainder.
//////////////////////////////////////////////////////////////////////

template <class Quintinary, class Arg1, class Arg2>
class binder1and2of5 {
   const Quintinary f;
   const Arg1 a1;
   const Arg2 a2;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg3, class Arg4, class Arg5>
   struct Sig 
   : public FunType<typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg3Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg4Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg5Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::ResultType> 
   {};
#endif

   binder1and2of5(const Quintinary& w, const Arg1& x, const Arg2& y) :
     f(w), a1(x), a2(y) {};
   template <class Arg3,class Arg4,class Arg5>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<Quintinary,Arg1,Arg2,Arg3,Arg4,Arg5>::result_type
#else
   typename Sig<Arg3,Arg4,Arg5>::ResultType
#endif
   operator()(const Arg3& z, const Arg4& v, const Arg5& w) const { return f(a1,a2,z,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quintinary>() ) {
       std::string bindername = std::string("binder1and2of5::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1and2of5");
   }
#endif
};

struct Bind1and2of5 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1and2of5");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Quintinary, class A1, class A2>
   struct Sig
   : public FunType<Quintinary,A1,A2,Full3<binder1and2of5<Quintinary,A1,A2> > > {};
#endif

   template <class Quintinary, class A1, class A2>
   Full3<binder1and2of5<Quintinary,A1,A2> >
   inline operator()( const Quintinary& f, const A1& a1, const A2& a2 ) const {
      return makeFull3( binder1and2of5<Quintinary,A1,A2>(f,a1,a2) );
   }
};

////////////////////////////////////////////////////////////////////////
// binderXandYof5 provides the remainder.
// All that is needed is more specialisations of binderXandYof5Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////
template <typename BinderX,typename BinderY,typename R,typename F,
  typename ArgX, typename ArgY,typename A,typename B,typename C>
struct binderXandYof5Helper;

template <typename R,typename F,typename ArgX,typename ArgY,
           typename A,typename B,typename C>
  struct binderXandYof5Helper<Bind1,Bind2,R,F,ArgX,ArgY,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
      const A& a, const B& b, const C& c)
  {
    return f(argX,argY,a,b,c);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
           typename A,typename B,typename C>
  struct binderXandYof5Helper<Bind1,Bind3,R,F,ArgX,ArgY,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
      const A& a, const B& b, const C& c)
  {
    return f(argX,a,argY,b,c);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
           typename A,typename B,typename C>
  struct binderXandYof5Helper<Bind1,Bind4,R,F,ArgX,ArgY,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
      const A& a, const B& b, const C& c)
  {
    return f(argX,a,b,argY,c);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
           typename A,typename B,typename C>
  struct binderXandYof5Helper<Bind1,Bind5,R,F,ArgX,ArgY,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
      const A& a, const B& b, const C& c)
  {
    return f(argX,a,b,c,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
           typename A,typename B,typename C>
  struct binderXandYof5Helper<Bind2,Bind3,R,F,ArgX,ArgY,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
      const A& a, const B& b, const C& c)
  {
    return f(a,argX,argY,b,c);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
           typename A,typename B,typename C>
  struct binderXandYof5Helper<Bind2,Bind4,R,F,ArgX,ArgY,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
      const A& a, const B& b, const C& c)
  {
    return f(a,argX,b,argY,c);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
           typename A,typename B,typename C>
  struct binderXandYof5Helper<Bind2,Bind5,R,F,ArgX,ArgY,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
      const A& a, const B& b, const C& c)
  {
    return f(a,argX,b,c,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
           typename A,typename B,typename C>
  struct binderXandYof5Helper<Bind3,Bind4,R,F,ArgX,ArgY,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
      const A& a, const B& b, const C& c)
  {
    return f(a,b,argX,argY,c);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
           typename A,typename B,typename C>
  struct binderXandYof5Helper<Bind3,Bind5,R,F,ArgX,ArgY,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
      const A& a, const B& b, const C& c)
  {
    return f(a,b,argX,c,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
           typename A,typename B,typename C>
  struct binderXandYof5Helper<Bind4,Bind5,R,F,ArgX,ArgY,A,B,C>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
      const A& a, const B& b, const C& c)
  {
    return f(a,b,c,argX,argY);
  }
};

template <class Quintinary, typename PairX, typename PairY>
class binderXandYof5
{
     const Quintinary f;
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
     "***** ERROR in binderXandYof5 : BinderX==BinderY : ERROR *****");
#endif
public:
   binderXandYof5( const Quintinary& g,const PairX& qx,const PairY& qy) :
     f(g), px(qx), py(qy)
     {
       //This run time failure mechanism replaced by compile time error message.
       //assert(px.first.n != py.first.n);
     }

#ifndef FCPP_CONCEPTS

     template <typename BindX, typename BindY, typename A, typename B, typename C>
     struct SigN;

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind2,A,B,C>
       : public FunType<Quintinary,ArgX,ArgY,A,B,C,
       typename RT<Quintinary,ArgX,ArgY,A,B,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind3,A,B,C>
       : public FunType<Quintinary,ArgX,ArgY,A,B,C,
       typename RT<Quintinary,ArgX,A,ArgY,B,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind4,A,B,C>
       : public FunType<Quintinary,ArgX,ArgY,A,B,C,
       typename RT<Quintinary,ArgX,A,B,ArgY,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind1,Bind5,A,B,C>
       : public FunType<Quintinary,ArgX,ArgY,A,B,C,
       typename RT<Quintinary,ArgX,A,B,C,ArgY>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind2,Bind3,A,B,C>
       : public FunType<Quintinary,ArgX,ArgY,A,B,C,
       typename RT<Quintinary,A,ArgX,ArgY,B,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind2,Bind4,A,B,C>
       : public FunType<Quintinary,ArgX,ArgY,A,B,C,
       typename RT<Quintinary,A,ArgX,B,ArgY,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind2,Bind5,A,B,C>
       : public FunType<Quintinary,ArgX,ArgY,A,B,C,
       typename RT<Quintinary,A,ArgX,B,C,ArgY>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind3,Bind4,A,B,C>
       : public FunType<Quintinary,ArgX,ArgY,A,B,C,
       typename RT<Quintinary,A,B,ArgX,ArgY,C>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind3,Bind5,A,B,C>
       : public FunType<Quintinary,ArgX,ArgY,A,B,C,
       typename RT<Quintinary,A,B,ArgX,C,ArgY>::ResultType >
       { };

     template <typename A, typename B, typename C>
       struct  SigN<Bind4,Bind5,A,B,C>
       : public FunType<Quintinary,ArgX,ArgY,A,B,C,
       typename RT<Quintinary,A,B,C,ArgX,ArgY>::ResultType >
       { };

     template <typename A,typename B,typename C>
     struct Sig : public SigN<BinderX,BinderY,A,B,C>
     { };

#endif

     template <typename A,typename B,typename C>
#ifdef FCPP_CONCEPTS
       inline ConstCallable7<Quintinary,BinderX,BinderY,ArgX,ArgY,A,B,C>::result_type
#else
     typename Sig<A,B,C>::ResultType
#endif
   operator()(const A& a, const B& b, const C& c) const
   {
#ifdef FCPP_CONCEPTS
     return binderXandYof5Helper<BinderX,BinderY,
     typename ConstCallable7<Quintinary,BinderX,BinderY,ArgX,ArgY,A,B,C>::result_type,
        Quintinary,ArgX,ArgY,A,B,C>::go(f,px.second,py.second,a,b,c);
#else
     return binderXandYof5Helper<BinderX,BinderY,
       typename Sig<A,B,C>::ResultType,Quintinary,
       ArgX,ArgY,A,B,C>::go(f,px.second,py.second,a,b,c);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Quintinary>() ) {
       std::string temp = std::string("binderXandYof5::")
                        + debug_traits_name(f);
       if (px.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(px.first.n);
       temp += std::string(",") + debug_name_or_value(px.second) + std::string(")");
       if (py.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(py.first.n);
       temp += std::string(",") + debug_name_or_value(py.second) + std::string(")");
       return temp;
     } else
       return std::string("binderXandYof5");
    }
#endif
};

struct BindXandYof5 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXandYof5");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Quintinary, class PairX, class PairY>
   struct Sig 
    : public FunType<Quintinary,PairX,PairY,
    Full3<binderXandYof5<Quintinary,PairX,PairY> > > {};
#endif

  template <class Quintinary, class PairX, class PairY>
#ifdef FCPP_CONCEPTS
    inline Full3<binderXandYof5<Quintinary,PairX,PairY> >
#else
    typename Sig<Quintinary,PairX,PairY>::ResultType
#endif
    operator()( const Quintinary& f,const PairX &px ,const PairY &py ) const {
    return makeFull3( binderXandYof5<Quintinary,PairX,PairY>(f,px,py) );
   }
};

//////////////////////////////////////////////////////////////////////
// bind 3 arguments - there are 10 possibilities.
// Only one is implemented as yet.
//////////////////////////////////////////////////////////////////////

template <class Quintinary, class Arg1, class Arg2, class Arg3>
class binder1to3of5 {
   const Quintinary f;
   const Arg1 a1;
   const Arg2 a2;
   const Arg3 a3;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg4, class Arg5>
   struct Sig
   : public FunType<
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg4Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg5Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::ResultType> 
   {};
#endif

   binder1to3of5(const Quintinary& w, const Arg1& x, const Arg2& y, const Arg3& z) :
     f(w), a1(x), a2(y), a3(z) {};
   template <class Arg4,class Arg5>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<Quintinary,Arg1,Arg2,Arg3,Arg4,Arg5>::result_type
#else
   typename Sig<Arg4,Arg5>::ResultType
#endif
   operator()(const Arg4& v, const Arg5& w) const { return f(a1,a2,a3,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quintinary>() ) {
       std::string bindername = std::string("binder1to3of5::")+
              debug_traits_name(f);
       bindername += std::string("( ") + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1to3of5");
   }
#endif
};

struct Bind1to3of5 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to3of5");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Quintinary, class A1, class A2, class A3>
   struct Sig
    : public FunType<Quintinary,A1,A2,A3,Full2<binder1to3of5<Quintinary,A1,A2,A3> > > {};
#endif

  template <class Quintinary, class A1, class A2, class A3>
    Full2<binder1to3of5<Quintinary,A1,A2,A3> >
   inline operator()( const Quintinary& f, const A1& a1, const A2& a2, const A3& a3 ) const {
    return makeFull2( binder1to3of5<Quintinary,A1,A2,A3>(f,a1,a2,a3) );
   }
};
////////////////////////////////////////////////////////////////////////
// binderXtoZof5 provides the remainder.
// All that is needed is more specialisations of binderXtoZof5Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////
template <typename BinderX,typename BinderY,typename BinderZ, typename R,typename F,
      typename ArgX, typename ArgY, typename ArgZ, typename A,typename B>
struct binderXtoZof5Helper;

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B>
struct binderXtoZof5Helper<Bind1,Bind2,Bind3,R,F,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a, const B& b)
  {
    return f(argX,argY,argZ,a,b);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B>
struct binderXtoZof5Helper<Bind1,Bind2,Bind4,R,F,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a, const B& b)
  {
    return f(argX,argY,a,argZ,b);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B>
struct binderXtoZof5Helper<Bind1,Bind2,Bind5,R,F,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a, const B& b)
  {
    return f(argX,argY,a,b,argZ);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B>
struct binderXtoZof5Helper<Bind1,Bind3,Bind4,R,F,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a, const B& b)
  {
    return f(argX,a,argY,argZ,b);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B>
struct binderXtoZof5Helper<Bind1,Bind3,Bind5,R,F,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a, const B& b)
  {
    return f(argX,a,argY,b,argZ);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B>
struct binderXtoZof5Helper<Bind1,Bind4,Bind5,R,F,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a, const B& b)
  {
    return f(argX,a,b,argY,argZ);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B>
struct binderXtoZof5Helper<Bind2,Bind3,Bind4,R,F,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a, const B& b)
  {
    return f(a,argX,argY,argZ,b);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B>
struct binderXtoZof5Helper<Bind2,Bind3,Bind5,R,F,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a, const B& b)
  {
    return f(a,argX,argY,b,argZ);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B>
struct binderXtoZof5Helper<Bind2,Bind4,Bind5,R,F,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a, const B& b)
  {
    return f(a,argX,b,argY,argZ);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B>
struct binderXtoZof5Helper<Bind3,Bind4,Bind5,R,F,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a, const B& b)
  {
    return f(a,b,argX,argY,argZ);
  }
};

template <class Quintinary, typename PairX, typename PairY, typename PairZ>
class binderXtoZof5
{
     const Quintinary f;
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
     "***** ERROR in binderXtoZof5 : BinderX==BinderY : ERROR *****");
     static_assert(not std::is_same<BinderX,BinderZ>::value,
     "***** ERROR in binderXtoZof5 : BinderX==BinderZ : ERROR *****");
     static_assert(not std::is_same<BinderY,BinderZ>::value,
     "***** ERROR in binderXtoZof5 : BinderY==BinderZ : ERROR *****");
#endif
public:
     binderXtoZof5( const Quintinary& g,const PairX& qx,const PairY& qy,
                         const PairZ& qz) :
       f(g), px(qx), py(qy), pz(qz) {}

#ifndef FCPP_CONCEPTS
     template <typename BindX, typename BindY, typename BindZ,
       typename A, typename B>
     struct SigN;

     template <typename A, typename B>
       struct  SigN<Bind1,Bind2,Bind3,A,B>
       : public FunType<Quintinary,ArgX,ArgY,ArgZ,A,B,
       typename RT<Quintinary,ArgX,ArgY,ArgZ,A,B>::ResultType >
       { };

     template <typename A, typename B>
       struct  SigN<Bind1,Bind2,Bind4,A,B>
       : public FunType<Quintinary,ArgX,ArgY,ArgZ,A,B,
       typename RT<Quintinary,ArgX,ArgY,A,ArgZ,B>::ResultType >
       { };

     template <typename A, typename B>
       struct  SigN<Bind1,Bind2,Bind5,A,B>
       : public FunType<Quintinary,ArgX,ArgY,ArgZ,A,B,
       typename RT<Quintinary,ArgX,ArgY,A,B,ArgZ>::ResultType >
       { };

     template <typename A, typename B>
       struct  SigN<Bind1,Bind3,Bind4,A,B>
       : public FunType<Quintinary,ArgX,ArgY,ArgZ,A,B,
       typename RT<Quintinary,ArgX,A,ArgY,ArgZ,B>::ResultType >
       { };

     template <typename A, typename B>
       struct  SigN<Bind1,Bind3,Bind5,A,B>
       : public FunType<Quintinary,ArgX,ArgY,ArgZ,A,B,
       typename RT<Quintinary,ArgX,A,ArgY,B,ArgZ>::ResultType >
       { };

     template <typename A, typename B>
       struct  SigN<Bind1,Bind4,Bind5,A,B>
       : public FunType<Quintinary,ArgX,ArgY,ArgZ,A,B,
       typename RT<Quintinary,ArgX,A,B,ArgY,ArgZ>::ResultType >
       { };

     template <typename A, typename B>
       struct  SigN<Bind2,Bind3,Bind4,A,B>
       : public FunType<Quintinary,ArgX,ArgY,ArgZ,A,B,
       typename RT<Quintinary,A,ArgX,ArgY,ArgZ,B>::ResultType >
       { };

     template <typename A, typename B>
       struct  SigN<Bind2,Bind3,Bind5,A,B>
       : public FunType<Quintinary,ArgX,ArgY,ArgZ,A,B,
       typename RT<Quintinary,A,ArgX,ArgY,B,ArgZ>::ResultType >
       { };

     template <typename A, typename B>
       struct  SigN<Bind2,Bind4,Bind5,A,B>
       : public FunType<Quintinary,ArgX,ArgY,ArgZ,A,B,
       typename RT<Quintinary,A,ArgX,B,ArgY,ArgZ>::ResultType >
       { };

     template <typename A, typename B>
       struct  SigN<Bind3,Bind4,Bind5,A,B>
       : public FunType<Quintinary,ArgX,ArgY,ArgZ,A,B,
       typename RT<Quintinary,A,B,ArgX,ArgY,ArgZ>::ResultType >
       { };

     template <typename A,typename B>
       struct Sig : public SigN<BinderX,BinderY,BinderZ,A,B>
     { };
#endif


     template <typename A,typename B>
#ifdef FCPP_CONCEPTS
       inline ConstCallable8<Quintinary,BinderX,BinderY,BinderZ,ArgX,ArgY,ArgZ,A,B>::result_type
#else
       typename Sig<A,B>::ResultType
#endif
   operator()(const A& a, const B& b) const
   {
#ifdef FCPP_CONCEPTS
     return binderXtoZof5Helper<BinderX,BinderY,BinderZ,
       typename ConstCallable8<Quintinary,BinderX,BinderY,BinderZ,ArgX,ArgY,ArgZ,A,B>::result_type,
       Quintinary,ArgX,ArgY,ArgZ,A,B>::go(f,px.second,py.second,pz.second,a,b);
#else
     return binderXtoZof5Helper<BinderX,BinderY,BinderZ,
       typename Sig<A,B>::ResultType,Quintinary,
       ArgX,ArgY,ArgZ,A,B>::go(f,px.second,py.second,pz.second,a,b);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Quintinary>() ) {
       std::string temp = std::string("binderXtoZof5::")
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
       return std::string("binderXtoZof5");
    }
#endif
};

struct BindXtoZof5 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXtoZof5");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Quintinary, class PairX, class PairY, class PairZ>
   struct Sig
    : public FunType<Quintinary,PairX,PairY,PairZ,
    Full2<binderXtoZof5<Quintinary,PairX,PairY,PairZ> > > {};
#endif

  template <class Quintinary, class PairX, class PairY, class PairZ>
#ifdef FCPP_CONCEPTS
    inline Full2<binderXtoZof5<Quintinary,PairX,PairY,PairZ> >
#else
    typename Sig<Quintinary,PairX,PairY,PairZ>::ResultType
#endif
    operator()( const Quintinary& f,const PairX &px ,const PairY &py, const PairZ &pz ) const
   {
     return makeFull2( binderXtoZof5<Quintinary,PairX,PairY,PairZ>(f,px,py,pz) );
   }
};

//////////////////////////////////////////////////////////////////////
// bind 4 arguments - there are 5 possibilities.
// One is implemented.
//////////////////////////////////////////////////////////////////////

 template <class Quintinary, class Arg1, class Arg2, class Arg3, class Arg4>
class binder1to4of5 {
   const Quintinary f;
   const Arg1 a1;
   const Arg2 a2;
   const Arg3 a3;
   const Arg4 a4;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg5>
   struct Sig
   : public FunType<
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::Arg5Type,
                    typename Quintinary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5>::ResultType>
   {};
#endif

   binder1to4of5(const Quintinary& w, const Arg1& x, const Arg2& y, const Arg3& z,
                      const Arg4& p) :
     f(w), a1(x), a2(y), a3(z), a4(p) {};
   template <class Arg5>
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<Quintinary,Arg1,Arg2,Arg3,Arg4,Arg5>::result_type
#else
   typename Sig<Arg5>::ResultType
#endif
     operator()(const Arg5& v) const { return f(a1,a2,a3,a4,v); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Quintinary>() ) {
       std::string bindername = std::string("binder1to4of5::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" , ") + debug_name_or_value(a4);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1to4of5");
   }
#endif
};

struct Bind1to4of5 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to4of5");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Quintinary, class A1, class A2, class A3, class A4>
   struct Sig
    : public FunType<Quintinary,A1,A2,A3,A4,
    Full1<binder1to4of5<Quintinary,A1,A2,A3,A4> > > {};
#endif

  template <class Quintinary, class A1, class A2, class A3, class A4>
    inline  Full1<binder1to4of5<Quintinary,A1,A2,A3,A4> >
    operator()( const Quintinary& f, const A1& a1, const A2& a2, const A3& a3,
               const A4& a4 ) const {
    return makeFull1( binder1to4of5<Quintinary,A1,A2,A3,A4>(f,a1,a2,a3,a4) );
   }
};

////////////////////////////////////////////////////////////////////////
// binderWtoZof5 provides the remainder.
// All that is needed is more specialisations of binderWtoZof5Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////
 template <typename BinderW,typename BinderX,typename BinderY,typename BinderZ, typename R,typename F,
   typename ArgW,typename ArgX, typename ArgY, typename ArgZ, typename A>
struct binderWtoZof5Helper;

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A>
   struct binderWtoZof5Helper<Bind1,Bind2,Bind3,Bind4,R,F,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a)
  {
    return f(argW,argX,argY,argZ,a);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A>
   struct binderWtoZof5Helper<Bind1,Bind2,Bind3,Bind5,R,F,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a)
  {
    return f(argW,argX,argY,a,argZ);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A>
   struct binderWtoZof5Helper<Bind1,Bind2,Bind4,Bind5,R,F,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a)
  {
    return f(argW,argX,a,argY,argZ);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A>
   struct binderWtoZof5Helper<Bind1,Bind3,Bind4,Bind5,R,F,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a)
  {
    return f(argW,a,argX,argY,argZ);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A>
   struct binderWtoZof5Helper<Bind2,Bind3,Bind4,Bind5,R,F,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a)
  {
    return f(a,argW,argX,argY,argZ);
  }
};

template <class Quintinary, typename PairW, typename PairX, typename PairY, typename PairZ>
class binderWtoZof5
{
     const Quintinary f;
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
     binderWtoZof5( const Quintinary& g,const PairW& qw,const PairX& qx,const PairY& qy,
                         const PairZ& qz) :
       f(g), pw(qw), px(qx), py(qy), pz(qz) {}

#ifndef FCPP_CONCEPTS
       template <typename BindW, typename BindX, typename BindY, typename BindZ, 
       typename A>
     struct SigN;

     template <typename A>
       struct  SigN<Bind1,Bind2,Bind3,Bind4,A>
       : public FunType<Quintinary,ArgW,ArgX,ArgY,ArgZ,A,
         typename RT<Quintinary,ArgW,ArgX,ArgY,ArgZ,A>::ResultType >
       { };

     template <typename A>
       struct  SigN<Bind1,Bind2,Bind3,Bind5,A>
       : public FunType<Quintinary,ArgW,ArgX,ArgY,ArgZ,A,
         typename RT<Quintinary,ArgW,ArgX,ArgY,A,ArgZ>::ResultType >
       { };

     template <typename A>
       struct  SigN<Bind1,Bind2,Bind4,Bind5,A>
       : public FunType<Quintinary,ArgW,ArgX,ArgY,ArgZ,A,
         typename RT<Quintinary,ArgW,ArgX,A,ArgY,ArgZ>::ResultType >
       { };

     template <typename A>
       struct  SigN<Bind1,Bind3,Bind4,Bind5,A>
       : public FunType<Quintinary,ArgW,ArgX,ArgY,ArgZ,A,
         typename RT<Quintinary,ArgW,A,ArgX,ArgY,ArgZ>::ResultType >
       { };

     template <typename A>
       struct  SigN<Bind2,Bind3,Bind4,Bind5,A>
       : public FunType<Quintinary,ArgW,ArgX,ArgY,ArgZ,A,
         typename RT<Quintinary,A,ArgW,ArgX,ArgY,ArgZ>::ResultType >
       { };

     template <typename A>
       struct Sig : public SigN<BinderW,BinderX,BinderY,BinderZ,A>
     { };
#endif


     template <typename A>
#ifdef FCPP_CONCEPTS
       inline ConstCallable9<Quintinary,BinderW,BinderX,BinderY,BinderZ,ArgW,ArgX,ArgY,ArgZ,A>::result_type
#else
       typename Sig<A>::ResultType
#endif
   operator()(const A& a) const
   {
#ifdef FCPP_CONCEPTS
     return binderWtoZof5Helper<BinderW,BinderX,BinderY,BinderZ,
       typename ConstCallable9<Quintinary,BinderW,BinderX,BinderY,BinderZ,ArgW,ArgX,ArgY,ArgZ,A>::result_type,
       Quintinary,ArgW,ArgX,ArgY,ArgZ,A>::go(f,pw.second,px.second,py.second,pz.second,a);
#else
     return binderWtoZof5Helper<BinderW,BinderX,BinderY,BinderZ,
       typename Sig<A>::ResultType,Quintinary,
       ArgW,ArgX,ArgY,ArgZ,A>::go(f,pw.second,px.second,py.second,pz.second,a);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Quintinary>() ) {
       std::string temp = std::string("binderWtoZof5::")
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
       return std::string("binderWtoZof5");
    }
#endif
};

struct BindWtoZof5 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindWtoZof5");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Quintinary,class PairW,class PairX, class PairY, class PairZ>
   struct Sig
    : public FunType<Quintinary,PairW,PairX,PairY,PairZ,
    Full1<binderWtoZof5<Quintinary,PairW,PairX,PairY,PairZ> > > {};
#endif

  template <class Quintinary,class PairW,class PairX, class PairY, class PairZ>
#ifdef FCPP_CONCEPTS
    inline Full1<binderWtoZof5<Quintinary,PairW,PairX,PairY,PairZ> >
#else
    typename Sig<Quintinary,PairW,PairX,PairY,PairZ>::ResultType
#endif
    operator()( const Quintinary& f,const PairW &pw ,const PairX &px ,const PairY &py, const PairZ &pz ) const 
   {
     return makeFull1( binderWtoZof5<Quintinary,PairW,PairX,PairY,PairZ>(f,pw,px,py,pz) );
   }
};

//////////////////////////////////////////////////////////////////////
// bind all 5 arguments
// Implementation involves Full6.
/////////////////////////////////////////////////////////////////////

 template <class Quintinary, class A1, class A2, class A3, class A4, class A5>
class binder1to5of5
#ifndef FCPP_CONCEPTS
   : public CFunType<typename RT<Quintinary,A1,A2,A3,A4,A5>::ResultType> 
#endif
{
   const Quintinary f;
   const A1 a1;
   const A2 a2;
   const A3 a3;
   const A4 a4;
   const A5 a5;
public:
   binder1to5of5( const Quintinary& g, 
       const A1& x, const A2& y, const A3& z, const A4& v, const A5& w )
     : f(g), a1(x), a2(y), a3(z), a4(v), a5(w) {}
#ifdef FCPP_CONCEPTS
     inline ConstCallable5<Quintinary,A1,A2,A3,A4,A5>::result_type
#else
     typename RT<Quintinary,A1,A2,A3,A4,A5>::ResultType 
#endif
       operator()() const { return f(a1,a2,a3,a4,a5); }

#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     if ( debug_traits_has_name<Quintinary>() ) {
       std::string bindername = std::string("binder1to5of5::")
         + debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" , ") + debug_name_or_value(a4);
       bindername += std::string(" , ") + debug_name_or_value(a5);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1to5of5");
   }
#endif
};

struct Bind1to5of5 {
#ifndef FCPP_CONCEPTS
  template <class Quintinary, class A1, class A2, class A3, class A4, class A5>
   struct Sig 
    : public FunType<Quintinary,A1,A2,A3,A4,A5, 
    Full0<binder1to5of5<Quintinary,A1,A2,A3,A4,A5> > > {};
#endif

  template <class Quintinary, class A1, class A2, class A3, class A4, class A5>
    Full0<binder1to5of5<Quintinary,A1,A2,A3,A4,A5> >
   operator()( const Quintinary& f, const A1& a1, 
               const A2& a2, const A3& a3, const A4& a4, const A5& a5) const {
#ifdef FCPP_PRINT_HELPERS5
    std::cout << "Bind1to5of5 operator() called" << std::endl;
#endif
    return makeFull0( binder1to5of5<Quintinary,A1,A2,A3,A4,A5>
                     (f,a1,a2,a3,a4,a5) );
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to5of5");
   }
#endif
};

struct Curry5 {

#ifndef FCPP_CONCEPTS
  template <class Quintinary, class A1, class A2 = void, class A3 = void, class A4 = void, class A5 = void>
    struct Sig  // Now available.
    : public FunType<Quintinary, A1, A2, A3, A4, A5,
    Full0<binder1to5of5<Quintinary,A1,A2,A3,A4,A5> > > {};

  template <class Quintinary>
  struct Sig<Quintinary, void >
    : public Full5<Quintinary> {};

  template <class Quintinary, class A1>
  struct Sig<Quintinary, A1>
    : public FunType<Quintinary, A1, void, void, void, void,
                   Full4<binder1of5<Quintinary,A1> > > {};

  template <class Quintinary, class A1, class A2>
  struct Sig<Quintinary, A1, A2>
    : public FunType<Quintinary, A1, A2, void, void, void ,
                   Full3<binder1and2of5<Quintinary,A1,A2> > > {};

  template <class Quintinary, class A1, class A2, class A3>
      struct Sig<Quintinary, A1, A2, A3>
    : public FunType<Quintinary, A1, A2, A3, void, void ,
    Full2<binder1to3of5<Quintinary,A1,A2,A3> > > {};

  template <class Quintinary, class A1, class A2, class A3, class A4>
    struct Sig<Quintinary, A1, A2, A3, A4>
    : public FunType<Quintinary, A1, A2, A3, A4, void ,
    Full1<binder1to4of5<Quintinary,A1,A2,A3,A4> > > {};


 #endif

  template <class Quintinary>
  inline Quintinary
  operator()( const Quintinary& f) const {
    bool b = &FunctoidTraits<Quintinary>::template ensure_accepts<5>::args;
    assert(FunctoidTraits_max_args<Quintinary>() == 5);
    return f;
  }

  template <class Quintinary, class A1>
   inline Full4<binder1of5<Quintinary,A1> >
  operator()( const Quintinary& f, const A1& a1 ) const {
    return makeFull4( binder1of5<Quintinary,A1>(f,a1) );
  }

  template <class Quintinary, class A1, class A2>
    inline Full3<binder1and2of5<Quintinary,A1,A2> >
  operator()( const Quintinary& f, const A1& a1, const A2& a2 ) const {
    return makeFull3( binder1and2of5<Quintinary,A1,A2>(f,a1,a2) );
  }

  template <class Quintinary, class A1, class A2, class A3>
    inline Full2<binder1to3of5<Quintinary,A1,A2,A3> >
  operator()( const Quintinary& f, const A1& a1, const A2& a2, const A3& a3 ) const {
    return makeFull2( binder1to3of5<Quintinary,A1,A2,A3>(f,a1,a2,a3) );
  }

  template <class Quintinary, class A1, class A2, class A3, class A4>
#ifdef FCPP_CONCEPTS
  inline Full1<binder1to4of5<Quintinary,A1,A2,A3,A4> >
#else
  typename Sig<Quintinary, A1, A2, A3, A4>::ResultType
#endif
  operator()( const Quintinary& f, const A1& a1, const A2& a2, 
              const A3& a3, const A4& a4 ) const {
#ifdef FCPP_PRINT_HELPERS5
    std::cout << "Curry5 called for 4 args" << std::endl;
#endif
    return makeFull1( binder1to4of5<Quintinary,A1,A2,A3,A4>
                      (f,a1,a2,a3,a4) );
  }

  template <class Quintinary, class A1, class A2, class A3, class A4, class A5>
#ifdef FCPP_CONCEPTS
  inline Full0<binder1to5of5<Quintinary,A1,A2,A3,A4,A5> >
#else
  typename Sig<Quintinary, A1, A2, A3, A4, A5>::ResultType
#endif
  operator()( const Quintinary& f, const A1& a1, const A2& a2, 
              const A3& a3, const A4& a4, const A5& a5 ) const {
#ifdef FCPP_PRINT_HELPERS5
    std::cout << "Curry 5 called for binder1to5of5" << std::endl;
#endif
    return makeFull0( binder1to5of5<Quintinary,A1,A2,A3,A4,A5>
                      (f,a1,a2,a3,a4,a5) );
  }

#ifdef FCPP_DEBUG
   std::string name() const {
       return std::string("Curry5");
   }
#endif

};

#endif
