// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_BINDERS7_DOT_H
#define FCPP_CURRY_BINDERS7_DOT_H


//////////////////////////////////////////////////////////////////////
// These are the binders for Full7 and Fun7 - removed from curry.h.
// Concept maps are in curry_concept_maps.h
//////////////////////////////////////////////////////////////////////
// bind 1 argument of 7 - there are 7 possibilities.
// Only the first is implemented for now.
//////////////////////////////////////////////////////////////////////

template <class Septimary, class Arg1>
class binder1of7 {
   const Septimary f;
   const Arg1 x;
public:
   binder1of7( const Septimary& a, const Arg1& b ) : f(a), x(b) {}

#ifndef FCPP_CONCEPTS
     template <class Arg2, class Arg3, class Arg4, class Arg5,class Arg6,class Arg7 > struct Sig 
       : public FunType<typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg2Type,
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg3Type,
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg4Type,
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg5Type,
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg6Type,
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg7Type,
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::ResultType> {};
#endif 

   template <class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::result_type
#else
     typename RT<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::ResultType
#endif
     operator()(const Arg2& y, const Arg3& z, const Arg4& t, const Arg5& u, const Arg6& v, const Arg7&w ) const {
     return f(x,y,z,t,u,v,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Septimary>() ) {
       std::string bindername = std::string("binder1of7::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(x);
       bindername += std::string(" )");
       return bindername;
     } else
        return std::string("binder1of7");
   }
#endif
};

struct Bind1of7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1of7");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Septimary, class Arg1>
   struct Sig 
   : public FunType<Septimary,Arg1,Full6<binder1of7<Septimary,Arg1> > > {};
#endif

   template <class Septimary, class Arg1>
#ifdef FCPP_CONCEPTS
   inline Full6<binder1of7<Septimary,Arg1> >
#else
   typename Sig<Septimary,Arg1>::ResultType
#endif
   operator()( const Septimary& f, const Arg1& x ) const {
      return makeFull6( binder1of7<Septimary,Arg1>(f,x) );
   }
};

////////////////////////////////////////////////////////////////////////
// binderXof7 provides the remainder.
// All that is needed is specialisations of binderXof7Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////

 template <typename Binder,typename R,typename F,typename ArgA,
   typename A,typename B,typename C,typename D, typename E, typename G>
struct binderXof7Helper;

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D, typename E, typename G>
   struct binderXof7Helper<Bind1,R,F,ArgA,A,B,C,D,E,G>
{
  static inline R go( const F& f, const ArgA& argA,
		      const A& a, const B& b, const C& c, const D& d, const E& e, const G& g)
  {
    return f(argA,a,b,c,d,e,g);
  }
};

 template <typename R,typename F,typename ArgA,
   typename A,typename B,typename C,typename D, typename E, typename G>
   struct binderXof7Helper<Bind2,R,F,ArgA,A,B,C,D,E,G>
{
  static inline R go( const F& f, const ArgA& argA,
		      const A& a, const B& b, const C& c, const D& d, const E& e, const G& g)
  {
    return f(a,argA,b,c,d,e,g);
  }
};

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D, typename E, typename G>
   struct binderXof7Helper<Bind3,R,F,ArgA,A,B,C,D,E,G>
{
  static inline R go( const F& f, const ArgA& argA,
		      const A& a, const B& b, const C& c, const D& d, const E& e, const G& g)
  {
    return f(a,b,argA,c,d,e,g);
  }
};

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D, typename E, typename G>
   struct binderXof7Helper<Bind4,R,F,ArgA,A,B,C,D,E,G>
{
  static inline R go( const F& f, const ArgA& argA,
		      const A& a, const B& b, const C& c, const D& d, const E& e, const G& g)
  {
    return f(a,b,c,argA,d,e,g);
  }
};

 template <typename R,typename F,typename ArgA,
           typename A,typename B,typename C,typename D, typename E, typename G>
   struct binderXof7Helper<Bind5,R,F,ArgA,A,B,C,D,E,G>
{
  static inline R go( const F& f, const ArgA& argA,
		      const A& a, const B& b, const C& c, const D& d, const E& e, const G& g)
  {
    return f(a,b,c,d,argA,e,g);
  }
};

 template <typename R,typename F,typename ArgA,
   typename A,typename B,typename C,typename D, typename E, typename G>
   struct binderXof7Helper<Bind6,R,F,ArgA,A,B,C,D,E,G>
{
  static inline R go( const F& f, const ArgA& argA,
		      const A& a, const B& b, const C& c, const D& d, const E& e, const G& g)
  {
    return f(a,b,c,d,e,argA,g);
  }
};

 template <typename R,typename F,typename ArgA,
   typename A,typename B,typename C,typename D, typename E, typename G>
   struct binderXof7Helper<Bind7,R,F,ArgA,A,B,C,D,E,G>
{
  static inline R go( const F& f, const ArgA& argA,
		      const A& a, const B& b, const C& c, const D& d, const E& e, const G& g)
  {
    return f(a,b,c,d,e,g,argA);
  }
};

template <class Septimary, typename Pair>
class binderXof7
{
     const Septimary f;
     Pair p;
    // These give the types for later
     typedef typename Pair::first_type Binder;
     typedef typename Pair::second_type ArgA;
public:
   binderXof7( const Septimary& g,const Pair& q) : f(g),
     p(q) {}

#ifndef FCPP_CONCEPTS
     template <typename Bind, typename A, typename B, typename C, typename D, typename E, typename G>
     struct SigN;

     template <typename A, typename B, typename C, typename D, typename E, typename G>
       struct  SigN<Bind1,A,B,C,D,E,G>
       : public FunType<Septimary,ArgA,A,B,C,D,E,G,
       typename RT<Septimary,ArgA,A,B,C,D,E,G>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D, typename E, typename G>
       struct  SigN<Bind2,A,B,C,D,E,G>
       : public FunType<Septimary,ArgA,A,B,C,D,E,G,
       typename RT<Septimary,A,ArgA,B,C,D,E,G>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D, typename E, typename G>
       struct  SigN<Bind3,A,B,C,D,E,G>
       : public FunType<Septimary,ArgA,A,B,C,D,E,G,
       typename RT<Septimary,A,B,ArgA,C,D,E,G>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D, typename E, typename G>
       struct  SigN<Bind4,A,B,C,D,E,G>
       : public FunType<Septimary,ArgA,A,B,C,D,E,G,
       typename RT<Septimary,A,B,C,ArgA,D,E,G>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D, typename E, typename G>
       struct  SigN<Bind5,A,B,C,D,E,G>
       : public FunType<Septimary,ArgA,A,B,C,D,E,G,
       typename RT<Septimary,A,B,C,D,ArgA,E,G>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D, typename E, typename G>
       struct  SigN<Bind6,A,B,C,D,E,G>
       : public FunType<Septimary,ArgA,A,B,C,D,E,G,
       typename RT<Septimary,A,B,C,D,E,ArgA,G>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D, typename E, typename G>
       struct  SigN<Bind7,A,B,C,D,E,G>
       : public FunType<Septimary,ArgA,A,B,C,D,E,G,
       typename RT<Septimary,A,B,C,D,E,G,ArgA>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D, typename E, typename G>
       struct Sig : public SigN<Binder,A,B,C,D,E,G>
     { };
#endif

     template <typename A,typename B,typename C,typename D, typename E, typename G>
#ifdef FCPP_CONCEPTS
     inline ConstCallable8<Septimary,Binder,ArgA,A,B,C,D,E,G>::result_type
#else
       typename Sig<A,B,C,D,E,G>::ResultType
#endif
   operator()
       (const A& a, const B& b, const C& c, const D& d, const E& e, const G& g) const 
   { 
#ifdef FCPP_CONCEPTS
      return binderXof7Helper<Binder,
	typename ConstCallable8<Septimary,Binder,ArgA,A,B,C,D,E,G>::result_type,
        Septimary,ArgA,A,B,C,D,E,G>::go(f,p.second,a,b,c,d,e,g);
#else
     return binderXof7Helper<Binder,
       typename Sig<A,B,C,D,E,G>::ResultType,Septimary,
       ArgA,A,B,C,D,E,G>::go(f,p.second,a,b,c,d,e,g);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Septimary>() ) {
       std::string temp = std::string("binderXof7::")
                        + debug_traits_name(f);
       if (p.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(p.first.n);
       temp += std::string(",") + debug_name_or_value(p.second) + std::string(")");
       return temp;
     } else
       return std::string("binderXof7");
    }
#endif
};

struct BindXof7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXof7");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Septimary, class Pair>
   struct Sig
    : public FunType<Septimary,Pair,Full6<binderXof7<Septimary,Pair> > > {};
#endif

  template <class Septimary, class Pair>
#ifdef FCPP_CONCEPTS
    inline Full6<binderXof7<Septimary,Pair> >
#else
    typename Sig<Septimary,Pair>::ResultType
#endif
    operator()( const Septimary& f,const Pair &p ) const {
    return makeFull6( binderXof7<Septimary,Pair>(f,p) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 2 arguments - there are 21 possibilities.
// Only one is implemented as yet.
//////////////////////////////////////////////////////////////////////

template <class Septimary, class Arg1, class Arg2>
class binder1and2of7 {
   const Septimary f;
   const Arg1 a1;
   const Arg2 a2;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
   struct Sig 
       : public FunType<
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg3Type,
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg4Type,
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg5Type,
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg6Type,
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg7Type,
                    typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::ResultType> {};
#endif
   
   binder1and2of7(const Septimary& w, const Arg1& x, const Arg2& y) : 
     f(w), a1(x), a2(y) {};
     template <class Arg3,class Arg4,class Arg5,class Arg6,class Arg7>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::result_type
#else
       typename Sig<Arg3,Arg4,Arg5,Arg6,Arg7>::ResultType 
#endif
       operator()(const Arg3& z, const Arg4& s, const Arg5& u, const Arg6& v, const Arg7 &w) const 
       { return f(a1,a2,z,s,u,v,w); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Septimary>() ) {
       std::string bindername = std::string("binder1and2of7::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1and2of7");
   }
#endif
};

struct Bind1and2of7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1and2of7");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Septimary, class A1, class A2>
   struct Sig 
   : public FunType<Septimary,A1,A2,Full5<binder1and2of7<Septimary,A1,A2> > > {};
#endif

   template <class Septimary, class A1, class A2>
   inline Full5<binder1and2of7<Septimary,A1,A2> >
   operator()( const Septimary& f, const A1& a1, const A2& a2 ) const {
      return makeFull5( binder1and2of7<Septimary,A1,A2>(f,a1,a2) );
   }
};

////////////////////////////////////////////////////////////////////////
// binderXandYof7 provides the remainder.
// All that is needed is more specialisations of binderXandYof7Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////
template <typename BinderX,typename BinderY,typename R,typename F,
  typename ArgX,typename ArgY,typename A,typename B,typename C,typename D,typename E>
struct binderXandYof7Helper;

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C,typename D, typename E>
  struct binderXandYof7Helper<Bind1,Bind2,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(argX,argY,a,b,c,d,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C,typename D,typename E>
  struct binderXandYof7Helper<Bind1,Bind3,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(argX,a,argY,b,c,d,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind1,Bind4,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(argX,a,b,argY,c,d,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind1,Bind5,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(argX,a,b,c,argY,d,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind1,Bind6,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e) 
  {
    return f(argX,a,b,c,d,argY,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind1,Bind7,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e) 
  {
    return f(argX,a,b,c,d,e,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind2,Bind3,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,argX,argY,b,c,d,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind2,Bind4,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,argX,b,argY,c,d,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind2,Bind5,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e) 
  {
    return f(a,argX,b,c,argY,d,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind2,Bind6,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,argX,b,c,d,argY,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind2,Bind7,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,argX,b,c,d,e,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind3,Bind4,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,b,argX,argY,c,d,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind3,Bind5,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,b,argX,c,argY,d,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind3,Bind6,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,b,argX,c,d,argY,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind3,Bind7,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,b,argX,c,d,e,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind4,Bind5,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,b,c,argX,argY,d,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind4,Bind6,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e) 
  {
    return f(a,b,c,argX,d,argY,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind4,Bind7,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,b,c,argX,d,e,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind5,Bind6,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,b,c,d,argX,argY,e);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind5,Bind7,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e) 
  {
    return f(a,b,c,d,argX,e,argY);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C, typename D,typename E>
  struct binderXandYof7Helper<Bind6,Bind7,R,F,ArgX,ArgY,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e)
  {
    return f(a,b,c,d,e,argX,argY);
  }
};

template <class Septimary, typename PairX, typename PairY>
class binderXandYof7
{
     const Septimary f;
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
     "***** ERROR in binderXandYof7 : BinderX==BinderY : ERROR *****");
#endif
public:
   binderXandYof7( const Septimary& g,const PairX& qx,const PairY& qy) :
     f(g), px(qx), py(qy) {}

#ifndef FCPP_CONCEPTS

     template <typename BindX, typename BindY, typename A, typename B, typename C, 
                                               typename D, typename E>
     struct SigN;

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind1,Bind2,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,ArgX,ArgY,A,B,C,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind1,Bind3,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,ArgX,A,ArgY,B,C,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind1,Bind4,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,ArgX,A,B,ArgY,C,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind1,Bind5,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,ArgX,A,B,C,ArgY,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind1,Bind6,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,ArgX,A,B,C,D,ArgY,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind1,Bind7,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,ArgX,A,B,C,D,E,ArgY>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind2,Bind3,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,A,ArgX,ArgY,B,C,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind2,Bind4,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,A,ArgX,B,ArgY,C,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind2,Bind5,A,B,C,D,E>
       : public FunType<Septimary,A,ArgX,B,C,ArgY,D,E,
       typename RT<Septimary,A,ArgX,B,C,ArgY,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind2,Bind6,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,A,ArgX,B,C,D,ArgY,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind2,Bind7,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,A,ArgX,B,C,D,E,ArgY>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind3,Bind4,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,A,B,ArgX,ArgY,C,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind3,Bind5,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,A,B,ArgX,C,ArgY,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind3,Bind6,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,A,B,ArgX,C,D,ArgY,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind4,Bind5,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,A,B,C,ArgX,ArgY,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind4,Bind6,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,A,B,C,ArgX,D,ArgY,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind5,Bind6,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,A,B,C,D,ArgX,ArgY,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E>
       struct  SigN<Bind6,Bind7,A,B,C,D,E>
       : public FunType<Septimary,ArgX,ArgY,A,B,C,D,E,
       typename RT<Septimary,A,B,C,D,E,ArgX,ArgY>::ResultType >
       { };

     template <typename A,typename B,typename C, typename D,typename E>
       struct Sig : public SigN<BinderX,BinderY,A,B,C,D,E>
     { };
#endif

     template <typename A,typename B,typename C, typename D,typename E>
#ifdef FCPP_CONCEPTS
       inline ConstCallable9<Septimary,BinderX,BinderY,ArgX,ArgY,A,B,C,D,E>::result_type
#else
       typename Sig<A,B,C,D,E>::ResultType
#endif
   operator()(const A& a, const B& b, const C& c, const D& d, const E& e) const 
   { 
#ifdef FCPP_CONCEPTS
       return binderXandYof7Helper<BinderX,BinderY,
	 typename ConstCallable9<Septimary,BinderX,BinderY,ArgX,ArgY,A,B,C,D,E>::result_type,
	 Septimary,ArgX,ArgY,A,B,C,D,E>::go(f,px.second,py.second,a,b,c,d,e);
#else
     return binderXandYof7Helper<BinderX,BinderY,
       typename Sig<A,B,C,D,E>::ResultType,Septimary,
       ArgX,ArgY,A,B,C,D,E>::go(f,px.second,py.second,a,b,c,d,e);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Septimary>() ) {
       std::string temp = std::string("binderXandYof7::")
                        + debug_traits_name(f);
       if (px.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(px.first.n);
       temp += std::string(",") + debug_name_or_value(px.second) + std::string(")");
       if (py.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(py.first.n);
       temp += std::string(",") + debug_name_or_value(py.second) + std::string(")");
       return temp;
     } else
       return std::string("binderXandYof7");
    }
#endif
};

struct BindXandYof7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXandYof7");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Septimary, class PairX, class PairY>
   struct Sig
    : public FunType<Septimary,PairX,PairY,
    Full5<binderXandYof7<Septimary,PairX,PairY> > > {};
#endif

  template <class Septimary, class PairX, class PairY>
#ifdef FCPP_CONCEPTS
    inline Full5<binderXandYof7<Septimary,PairX,PairY> >
#else
    typename Sig<Septimary,PairX,PairY>::ResultType
#endif
    operator()( const Septimary& f,const PairX &px ,const PairY &py ) const {
    return makeFull5( binderXandYof7<Septimary,PairX,PairY>(f,px,py) );
   }
};

//////////////////////////////////////////////////////////////////////
// bind 3 arguments.
// Only the first one is implemented to provide for autocurrying.
//////////////////////////////////////////////////////////////////////

template <class Septimary, class Arg1, class Arg2, class Arg3>
class binder1to3of7 {
   const Septimary f;
   const Arg1 a1;
   const Arg2 a2;
   const Arg3 a3;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg4, class Arg5, class Arg6, class Arg7>
   struct Sig
   : public FunType<
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg4Type,
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg5Type,
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg6Type,
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg7Type,
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::ResultType>
   {};
#endif

   binder1to3of7(const Septimary& w, const Arg1& x, const Arg2& y, const Arg3& z) :
     f(w), a1(x), a2(y), a3(z) {};
     template <class Arg4,class Arg5, class Arg6, class Arg7>
#ifdef FCPP_CONCEPTS
       inline ConstCallable7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::result_type
#else
       typename Sig<Arg4,Arg5,Arg6,Arg7>::ResultType
#endif
       operator()(const Arg4& u, const Arg5& v, const Arg6& w, const Arg7& p)
       const { return f(a1,a2,a3,u,v,w,p); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Septimary>() ) {
       std::string bindername = std::string("binder1to3of7::")+
              debug_traits_name(f);
       bindername += std::string("( ") + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1to3of7");
   }
#endif

};

struct Bind1to3of7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to3of7");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Septimary, class A1, class A2, class A3>
   struct Sig
    : public FunType<Septimary,A1,A2,A3,Full4<binder1to3of7<Septimary,A1,A2,A3> > > {};
#endif

  template <class Septimary, class A1, class A2, class A3>
  inline  Full4<binder1to3of7<Septimary,A1,A2,A3> >
    operator()( const Septimary& f, const A1& a1, const A2& a2, const A3& a3 ) const {
    return makeFull4( binder1to3of7<Septimary,A1,A2,A3>(f,a1,a2,a3) );
   }
};

////////////////////////////////////////////////////////////////////////
// binderXtoZof7 provides all the three parameter bindings.
// All that is needed is more specialisations of binderXtoZof7Helper
// and the corresponding SigN definitions.
// Unfortunately this is not true for autocurry bindings.
///////////////////////////////////////////////////////////////////////
template <typename BinderX,typename BinderY,typename BinderZ, typename R,typename F,
  typename ArgX, typename ArgY, typename ArgZ, typename A,typename B,typename C,typename D>
struct binderXtoZof7Helper;

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
   struct binderXtoZof7Helper<Bind1,Bind2,Bind3,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,argY,argZ,a,b,c,d);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
   struct binderXtoZof7Helper<Bind1,Bind2,Bind4,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,argY,a,argZ,b,c,d);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
   struct binderXtoZof7Helper<Bind1,Bind2,Bind5,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,argY,a,b,argZ,c,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
   struct binderXtoZof7Helper<Bind1,Bind4,Bind5,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,a,b,argY,argZ,c,d);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
   struct binderXtoZof7Helper<Bind1,Bind2,Bind6,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,argY,a,b,c,argZ,d);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
   struct binderXtoZof7Helper<Bind1,Bind2,Bind7,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,argY,a,b,c,d,argZ);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
   struct binderXtoZof7Helper<Bind1,Bind3,Bind4,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(argX,a,argY,argZ,b,c,d);
  }
};

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
   struct binderXtoZof7Helper<Bind2,Bind3,Bind4,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,argX,argY,argZ,b,c,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
  struct binderXtoZof7Helper<Bind3,Bind4,Bind5,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,argX,argY,argZ,c,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
  struct binderXtoZof7Helper<Bind3,Bind4,Bind6,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,argX,argY,c,argZ,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
  struct binderXtoZof7Helper<Bind3,Bind5,Bind6,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,argX,c,argY,argZ,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
  struct binderXtoZof7Helper<Bind3,Bind5,Bind7,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,argX,c,argY,d,argZ);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
  struct binderXtoZof7Helper<Bind3,Bind6,Bind7,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,argX,c,d,argY,argZ);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
  struct binderXtoZof7Helper<Bind4,Bind5,Bind6,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,c,argX,argY,argZ,d);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
  struct binderXtoZof7Helper<Bind4,Bind5,Bind7,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,c,argX,argY,d,argZ);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
  struct binderXtoZof7Helper<Bind4,Bind6,Bind7,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,c,argX,d,argY,argZ);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D>
  struct binderXtoZof7Helper<Bind5,Bind6,Bind7,R,F,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a, const B& b, const C& c, const D& d)
  {
    return f(a,b,c,d,argX,argY,argZ);
  }
};

template <class Septimary, typename PairX, typename PairY, typename PairZ>
class binderXtoZof7
{
     const Septimary f;
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
     "***** ERROR in binderXtoZof7 : BinderX==BinderY : ERROR *****");
     static_assert(not std::is_same<BinderX,BinderZ>::value,
     "***** ERROR in binderXtoZof7 : BinderX==BinderZ : ERROR *****");
     static_assert(not std::is_same<BinderY,BinderZ>::value,
     "***** ERROR in binderXtoZof7 : BinderY==BinderZ : ERROR *****");
#endif
public:
     binderXtoZof7( const Septimary& g,const PairX& qx,const PairY& qy,
                         const PairZ& qz) :
       f(g), px(qx), py(qy), pz(qz) {}

#ifndef FCPP_CONCEPTS
     template <typename BindX, typename BindY, typename BindZ,
                 typename A, typename B, typename C, typename D>
     struct SigN;

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind2,Bind3,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,ArgX,ArgY,ArgZ,A,B,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind2,Bind4,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,ArgX,ArgY,A,ArgZ,B,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind2,Bind5,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,ArgX,ArgY,A,B,ArgZ,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind4,Bind5,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,ArgX,A,B,ArgY,ArgZ,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind2,Bind6,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,ArgX,ArgY,A,B,C,ArgZ,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind2,Bind7,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,ArgX,ArgY,A,B,C,D,ArgZ>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind1,Bind3,Bind4,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,ArgX,A,ArgY,ArgZ,B,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind2,Bind3,Bind4,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,A,ArgX,ArgY,ArgZ,B,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind3,Bind4,Bind5,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,A,B,ArgX,ArgY,ArgZ,C,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind3,Bind4,Bind6,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,A,B,ArgX,ArgY,C,ArgZ,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind3,Bind5,Bind6,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,A,B,ArgX,C,ArgY,ArgZ,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind3,Bind5,Bind7,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,A,B,ArgX,C,ArgY,D,ArgZ>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind3,Bind6,Bind7,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,A,B,ArgX,C,D,ArgY,ArgZ>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind4,Bind5,Bind6,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,A,B,C,ArgX,ArgY,ArgZ,D>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind4,Bind5,Bind7,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,A,B,C,ArgX,ArgY,D,ArgZ>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind4,Bind6,Bind7,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,A,B,C,ArgX,D,ArgY,ArgZ>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct  SigN<Bind5,Bind6,Bind7,A,B,C,D>
       : public FunType<Septimary,ArgX,ArgY,ArgZ,A,B,C,D,
       typename RT<Septimary,A,B,C,D,ArgX,ArgY,ArgZ>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D>
       struct Sig : public SigN<BinderX,BinderY,BinderZ,A,B,C,D>
     { };
#endif

     template <typename A,typename B,typename C,typename D>
#ifdef FCPP_CONCEPTS
       inline ConstCallable10<Septimary,BinderX,BinderY,BinderZ,ArgX,ArgY,ArgZ,A,B,C,D>::result_type
#else
       typename Sig<A,B,C,D>::ResultType
#endif
   operator()(const A& a, const B& b, const C& c, const D& d) const
   {
#ifdef FCPP_CONCEPTS
     return binderXtoZof7Helper<BinderX,BinderY,BinderZ,
       typename ConstCallable10<Septimary,BinderX,BinderY,BinderZ,ArgX,ArgY,ArgZ,A,B,C,D>::result_type,
       Septimary,ArgX,ArgY,ArgZ,A,B,C,D>::go(f,px.second,py.second,pz.second,a,b,c,d);
#else
     return binderXtoZof7Helper<BinderX,BinderY,BinderZ,
       typename Sig<A,B,C,D>::ResultType,Septimary,
       ArgX,ArgY,ArgZ,A,B,C,D>::go(f,px.second,py.second,pz.second,a,b,c,d);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Septimary>() ) {
       std::string temp = std::string("binderXtoZof7::")
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
       return std::string("binderXtoZof7");
    }
#endif
};


struct BindXtoZof7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXtoZof7");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Septimary, class PairX, class PairY, class PairZ>
   struct Sig
    : public FunType<Septimary,PairX,PairY,PairZ,
    Full4<binderXtoZof7<Septimary,PairX,PairY,PairZ> > > {};
#endif

  template <class Septimary, class PairX, class PairY, class PairZ>
#ifdef FCPP_CONCEPTS
    inline Full4<binderXtoZof7<Septimary,PairX,PairY,PairZ> >
#else
    typename Sig<Septimary,PairX,PairY,PairZ>::ResultType
#endif
    operator()( const Septimary& f,const PairX &px ,const PairY &py, const PairZ &pz ) const
   {
     return makeFull4( binderXtoZof7<Septimary,PairX,PairY,PairZ>(f,px,py,pz) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 4 arguments.
// Only the first is implemented for autocurrying.
//////////////////////////////////////////////////////////////////////

template <class Septimary, class Arg1, class Arg2, class Arg3, class Arg4>
class binder1to4of7 {
   const Septimary f;
   const Arg1 a1;
   const Arg2 a2;
   const Arg3 a3;
   const Arg4 a4;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg5, class Arg6, class Arg7>
   struct Sig
   : public FunType<
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg5Type,
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg6Type,
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg7Type,
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::ResultType>
   {};
#endif

   binder1to4of7(const Septimary& w, const Arg1& x, const Arg2& y, const Arg3& z,const Arg4& u) :
     f(w), a1(x), a2(y), a3(z), a4(u) {};
     template <class Arg5, class Arg6, class Arg7>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::result_type
#else
     typename Sig<Arg5,Arg6,Arg7>::ResultType
#endif
     operator()(const Arg5& v, const Arg6& w, const Arg7& p) const { return f(a1,a2,a3,a4,v,w,p); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Septimary>() ) {
       std::string bindername = std::string("binder1to4of7::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" , ") + debug_name_or_value(a4);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1to4of7");
   }
#endif
};

struct Bind1to4of7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to4of7");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Septimary, class A1, class A2, class A3, class A4>
   struct Sig
    : public FunType<Septimary,A1,A2,A3,A4,Full3<binder1to4of7<Septimary,A1,A2,A3,A4> > > {};
#endif

  template <class Septimary, class A1, class A2, class A3, class A4>
    inline Full3<binder1to4of7<Septimary,A1,A2,A3,A4> >
    operator()( const Septimary& f, const A1& a1, const A2& a2, const A3& a3, const A4& a4 ) const {
    return makeFull3( binder1to4of7<Septimary,A1,A2,A3,A4>(f,a1,a2,a3,a4) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 5 arguments using BindWtoZof7
// All that is needed is more specialisations of binderWtoZof7Helper
// and the corresponding SigN definitions and concept maps.
// This does not yet handle autocurrying of the arguments.
///////////////////////////////////////////////////////////////////////

 template <typename BinderW,typename BinderX,typename BinderY,typename BinderZ, typename R,typename F,
   typename ArgW,typename ArgX, typename ArgY, typename ArgZ, typename A, typename B,typename C>
struct binderWtoZof7Helper;

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B, typename C>
   struct binderWtoZof7Helper<Bind1,Bind2,Bind3,Bind4,R,F,ArgW,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a,const B& b,const C& c)
  {
    return f(argW,argX,argY,argZ,a,b,c);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B, typename C>
   struct binderWtoZof7Helper<Bind1,Bind3,Bind4,Bind5,R,F,ArgW,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a,const B& b,const C& c)
  {
    return f(argW,a,b,argX,argY,argZ,c);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B, typename C>
   struct binderWtoZof7Helper<Bind1,Bind4,Bind5,Bind6,R,F,ArgW,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a,const B& b,const C& c)
  {
    return f(argW,a,argX,argY,argZ,b,c);
  }
};

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B, typename C>
   struct binderWtoZof7Helper<Bind2,Bind3,Bind4,Bind5,R,F,ArgW,ArgX,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a,const B& b,const C& c)
  {
    return f(a,argW,argX,argY,argZ,b,c);
  }
};

/*
 This is difficult as the binders are not known yet for the final args.
 template <typename R,typename F,typename ArgW,typename ArgY,typename ArgZ,
   typename A, typename B, typename C>
   struct binderWtoZof7Helper<Bind1,Bind2,Bind3,Bind4,R,F,ArgW,AutoCurryType,ArgY,ArgZ,A,B,C>
{
  static inline R go( const F& f, const ArgW& argW,const AutoCurryType&,
                      const ArgY& argY, const ArgZ& argZ,
		      const A& a,const B& b,const C& c)
  {
    return binderVZof7(f,argW,argY,argZ,a,b,c);
  }
};
*/


template <class Septimary, typename PairW, typename PairX, typename PairY, typename PairZ>
class binderWtoZof7
{
     const Septimary f;
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

#ifdef FCPP_CXX11
     // Compile time error messages if any pairs of the binders are the same.
     static_assert(not std::is_same<BinderW,BinderX>::value,
     "***** ERROR in binderWtoZof7 : BinderW==BinderX : ERROR *****");
     static_assert(not std::is_same<BinderW,BinderY>::value,
     "***** ERROR in binderWtoZof7 : BinderW==BinderY : ERROR *****");
     static_assert(not std::is_same<BinderW,BinderZ>::value,
     "***** ERROR in binderWtoZof7 : BinderW==BinderZ : ERROR *****");
     static_assert(not std::is_same<BinderX,BinderY>::value,
     "***** ERROR in binderWtoZof7 : BinderX==BinderY : ERROR *****");
     static_assert(not std::is_same<BinderX,BinderZ>::value,
     "***** ERROR in binderWtoZof7 : BinderX==BinderZ : ERROR *****");
     static_assert(not std::is_same<BinderY,BinderZ>::value,
     "***** ERROR in binderWtoZof7 : BinderY==BinderZ : ERROR *****");
#endif
public:
     binderWtoZof7( const Septimary& g,const PairW& qw,const PairX& qx,const PairY& qy,
                         const PairZ& qz) : 
       f(g), pw(qw), px(qx), py(qy), pz(qz) {}

#ifndef FCPP_CONCEPTS
       template <typename BindW, typename BindX, typename BindY, typename BindZ, 
	 typename A,typename B,typename C>
     struct SigN;

       template <typename A,typename B,typename C>
	 struct  SigN<Bind1,Bind2,Bind3,Bind4,A,B,C>
	 : public FunType<Septimary,ArgW,ArgX,ArgY,ArgZ,A,B,C,
	 typename RT<Septimary,ArgW,ArgX,ArgY,ArgZ,A,B,C>::ResultType >
       { };

       template <typename A,typename B,typename C>
	 struct  SigN<Bind1,Bind3,Bind4,Bind5,A,B,C>
	 : public FunType<Septimary,ArgW,ArgX,ArgY,ArgZ,A,B,C,
	 typename RT<Septimary,ArgW,ArgX,ArgY,ArgZ,A,B,C>::ResultType >
       { };

       template <typename A,typename B,typename C>
	 struct  SigN<Bind1,Bind2,Bind4,Bind5,A,B,C>
	 : public FunType<Septimary,ArgW,ArgX,ArgY,ArgZ,A,B,C,
	 typename RT<Septimary,ArgW,ArgX,ArgY,ArgZ,A,B,C>::ResultType >
       { };

       template <typename A,typename B,typename C>
	 struct  SigN<Bind1,Bind2,Bind3,Bind5,A,B,C>
	 : public FunType<Septimary,ArgW,ArgX,ArgY,ArgZ,A,B,C,
	 typename RT<Septimary,ArgW,ArgX,ArgY,ArgZ,A,B,C>::ResultType >
       { };

       template <typename A,typename B,typename C>
	 struct  SigN<Bind2,Bind3,Bind4,Bind5,A,B,C>
	 : public FunType<Septimary,ArgW,ArgX,ArgY,ArgZ,A,B,C,
	 typename RT<Septimary,A,ArgW,ArgX,ArgY,ArgZ,B,C>::ResultType >
       { };

       template <typename A,typename B, typename C>
	 struct Sig : public SigN<BinderW,BinderX,BinderY,BinderZ,A,B,C>
     { };

#endif

       template <typename A, typename B, typename C>
#ifdef FCPP_CONCEPTS
	 inline ConstCallable11<Septimary,BinderW,BinderX,BinderY,BinderZ,ArgW,ArgX,ArgY,ArgZ,A,B,C>::result_type
#else
	 typename Sig<A,B,C>::ResultType
#endif
	 operator()(const A& a,const B& b,const C& c) const
   {
#ifdef FCPP_CONCEPTS
     return binderWtoZof7Helper<BinderW,BinderX,BinderY,BinderZ,
       typename ConstCallable11<Septimary,BinderW,BinderX,BinderY,BinderZ,ArgW,ArgX,ArgY,ArgZ,A,B,C>::result_type,
       Septimary,ArgW,ArgX,ArgY,ArgZ,A,B,C>::go(f,pw.second,px.second,py.second,pz.second,a,b,c);
#else
     return binderWtoZof7Helper<BinderW,BinderX,BinderY,BinderZ,
       typename Sig<A,B,C>::ResultType,Septimary,
       ArgW,ArgX,ArgY,ArgZ,A,B,C>::go(f,pw.second,px.second,py.second,pz.second,a,b,c);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Septimary>() ) {
       std::string temp = std::string("binderWtoZof7::")
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
       return std::string("binderWtoZof7");
    }
#endif
};

struct BindWtoZof7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindWtoZof7");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Septimary,class PairW,class PairX, class PairY, class PairZ>
   struct Sig
    : public FunType<Septimary,PairW,PairX,PairY,PairZ,
    Full3<binderWtoZof7<Septimary,PairW,PairX,PairY,PairZ> > > {};
#endif

  template <class Septimary,class PairW,class PairX, class PairY, class PairZ>
#ifdef FCPP_CONCEPTS
    inline Full3<binderWtoZof7<Septimary,PairW,PairX,PairY,PairZ> >
#else
    typename Sig<Septimary,PairW,PairX,PairY,PairZ>::ResultType
#endif
    operator()( const Septimary& f,const PairW &pw ,const PairX &px ,const PairY &py, const PairZ &pz ) const
   {
     return makeFull3( binderWtoZof7<Septimary,PairW,PairX,PairY,PairZ>(f,pw,px,py,pz) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 5 arguments
// Only the first one is to be implemented.
//////////////////////////////////////////////////////////////////////

template <class Septimary, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
class binder1to5of7 {
   const Septimary f;
   const Arg1 a1;
   const Arg2 a2;
   const Arg3 a3;
   const Arg4 a4;
   const Arg5 a5;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg6, class Arg7>
   struct Sig 
   : public FunType<
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg6Type,
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg7Type,
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::ResultType> 
   {};
#endif

   binder1to5of7(const Septimary& w, const Arg1& x, const Arg2& y,
                               const Arg3& z,const Arg4& u,const Arg5& v) :
     f(w), a1(x), a2(y), a3(z), a4(u), a5(v) {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "binder1to5of7 constructor called" << std::endl;
#endif
     };
     template <class Arg6, class Arg7>
#ifdef FCPP_CONCEPTS
       inline ConstCallable7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::result_type
#else
       typename Sig<Arg6,Arg7>::ResultType
#endif
       operator()(const Arg6& w, const Arg7& x) const {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "binder1to5of7 operator() called" << std::endl;
#endif
     return f(a1,a2,a3,a4,a5,w,x);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     if ( debug_traits_has_name<Septimary>() ) {
       std::string bindername = std::string("binder1to5of7::")
         + debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" , ") + debug_name_or_value(a3);
       bindername += std::string(" , ") + debug_name_or_value(a4);
       bindername += std::string(" , ") + debug_name_or_value(a5);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1to5of7");
   }
#endif
};

struct Bind1to5of7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to5of7");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Septimary, class A1, class A2, class A3, class A4, class A5>
   struct Sig
    : public FunType<Septimary,A1,A2,A3,A4,A5,Full2<binder1to5of7<Septimary,A1,A2,A3,A4,A5> > > {};
#endif

  template <class Septimary, class A1, class A2, class A3, class A4, class A5>
    inline Full2<binder1to5of7<Septimary,A1,A2,A3,A4,A5> >
   operator()( const Septimary& f, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5 ) const {
    return makeFull2( binder1to5of7<Septimary,A1,A2,A3,A4,A5>(f,a1,a2,a3,a4,a5) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 5 arguments using BindVtoZof7
// All that is needed is more specialisations of binderVtoZof7Helper
// and the corresponding SigN definitions and concept maps.
/////////////////////////////////////////////////////////////////////

 template <typename BinderV,typename BinderW,typename BinderX,
           typename BinderY,typename BinderZ, typename R,typename F,
           typename ArgV,typename ArgW,typename ArgX,typename ArgY, 
           typename ArgZ, typename A, typename B>
struct binderVtoZof7Helper;

 template <typename R,typename F,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A, typename B>
   struct binderVtoZof7Helper<Bind1,Bind2,Bind3,Bind4,Bind5,R,F,
   ArgV,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f,const ArgV& argV,const ArgW& argW,
		      const ArgX& argX, const ArgY& argY, const ArgZ& argZ,const A& a,const B& b)
  {
    return f(argV,argW,argX,argY,argZ,a,b);
  }
};

 template <typename R,typename F,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A, typename B>
   struct binderVtoZof7Helper<Bind2,Bind3,Bind4,Bind5,Bind6,R,F,
   ArgV,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f,const ArgV& argV,const ArgW& argW,
		      const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a,const B& b)
  {
    return f(a,argV,argW,argX,argY,argZ,b);
  }
};

template <typename R,typename F,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A, typename B>
   struct binderVtoZof7Helper<Bind1,Bind3,Bind4,Bind5,Bind6,R,F,
  ArgV,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f,const ArgV& argV,const ArgW& argW,
		      const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a,const B& b)
  {
    return f(argV,a,argW,argX,argY,argZ,b);
  }
};

template <typename R,typename F,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A, typename B>
   struct binderVtoZof7Helper<Bind1,Bind4,Bind5,Bind6,Bind7,R,F,
  ArgV,ArgW,ArgX,ArgY,ArgZ,A,B>
{
  static inline R go( const F& f,const ArgV& argV,const ArgW& argW,
		      const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
                      const A& a,const B& b)
  {
    return f(argV,a,b,argW,argX,argY,argZ);
  }
};

 template <class Septimary, typename PairV,typename PairW, typename PairX,
                            typename PairY,typename PairZ>
class binderVtoZof7
{
     const Septimary f;
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
     binderVtoZof7( const Septimary& g,const PairV& qv,const PairW& qw,
                                 const PairX& qx,const PairY& qy,const PairZ& qz) : 
       f(g), pv(qv), pw(qw), px(qx), py(qy), pz(qz) {}

#ifndef FCPP_CONCEPTS
       template <typename BindV,typename BindW, typename BindX, typename BindY, typename BindZ, 
	 typename A, typename B>
     struct SigN;

       template <typename A, typename B>
	 struct  SigN<Bind1,Bind2,Bind3,Bind4,Bind5,A,B>
	 : public FunType<Septimary,ArgV,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Septimary,ArgV,ArgW,ArgX,ArgY,ArgZ,A,B>::ResultType >
       { };

       template <typename A, typename B>
	 struct  SigN<Bind2,Bind3,Bind4,Bind5,Bind6,A,B>
	 : public FunType<Septimary,ArgV,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Septimary,A,ArgV,ArgW,ArgX,ArgY,ArgZ,B>::ResultType >
       { };

       template <typename A, typename B>
	 struct  SigN<Bind1,Bind4,Bind5,Bind6,Bind7,A,B>
	 : public FunType<Septimary,ArgV,ArgW,ArgX,ArgY,ArgZ,A,B,
	 typename RT<Septimary,ArgV,A,B,ArgW,ArgX,ArgY,ArgZ>::ResultType >
       { };

       template <typename A, typename B>
	 struct Sig : public SigN<BinderV,BinderW,BinderX,BinderY,BinderZ,A,B>
     { };

#endif

       template <typename A, typename B>
#ifdef FCPP_CONCEPTS
	 inline ConstCallable12<Septimary,BinderV,BinderW,BinderX,BinderY,BinderZ,
                                          ArgV,ArgW,ArgX,ArgY,ArgZ,A,B>::result_type
#else
       typename Sig<A,B>::ResultType 
#endif
       operator()(const A& a,const B& b) const 
   { 
#ifdef FCPP_CONCEPTS
     return binderVtoZof7Helper<BinderV,BinderW,BinderX,BinderY,BinderZ,
       typename ConstCallable12<Septimary,BinderV,BinderW,BinderX,BinderY,BinderZ,
                                ArgV,ArgW,ArgX,ArgY,ArgZ,A,B>::result_type,
       Septimary,ArgV,ArgW,ArgX,ArgY,ArgZ,A,B>::go(f,pv.second,pw.second,px.second,
                                                     py.second,pz.second,a,b);
#else
     return binderVtoZof7Helper<BinderV,BinderW,BinderX,BinderY,BinderZ,
       typename Sig<A,B>::ResultType,Septimary,          
       ArgV,ArgW,ArgX,ArgY,ArgZ,A,B>::go(f,pv.second,pw.second,px.second,py.second,pz.second,a,b);
#endif
   }

#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Septimary>() ) {
       std::string temp = std::string("binderVtoZof7::")
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
       return std::string("binderVtoZof7");
    }
#endif

};

struct BindVtoZof7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindVtoZof7");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Septimary,class PairV,class PairW,class PairX, class PairY, class PairZ>
   struct Sig
    : public FunType<Septimary,PairV,PairW,PairX,PairY,PairZ,
    Full2<binderVtoZof7<Septimary,PairV,PairW,PairX,PairY,PairZ> > > {};
#endif

  template <class Septimary,class PairV,class PairW,class PairX, class PairY, class PairZ>
#ifdef FCPP_CONCEPTS
    inline Full2<binderVtoZof7<Septimary,PairV,PairW,PairX,PairY,PairZ> >
#else
    typename Sig<Septimary,PairV,PairW,PairX,PairY,PairZ>::ResultType
#endif
    operator()( const Septimary& f,const PairV &pv ,const PairW &pw ,
                const PairX &px ,const PairY &py, const PairZ &pz ) const 
   {
     return makeFull2( binderVtoZof7<Septimary,PairV,PairW,PairX,PairY,PairZ>
                       (f,pv,pw,px,py,pz) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 6 arguments.
// Only the first one is implemented.
//////////////////////////////////////////////////////////////////////

template <class Septimary, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
class binder1to6of7 {
   const Septimary f;
   const Arg1 a1;
   const Arg2 a2;
   const Arg3 a3;
   const Arg4 a4;
   const Arg5 a5;
   const Arg6 a6;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg7>
   struct Sig
   : public FunType<
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::Arg7Type,
     typename Septimary::template Sig<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::ResultType>
   {};
#endif

   binder1to6of7(const Septimary& w, const Arg1& x, const Arg2& y,
		 const Arg3& z,const Arg4& u,const Arg5& v, const Arg6& p) :
   f(w), a1(x), a2(y), a3(z), a4(u), a5(v), a6(p) {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "binder1to6of7 constructor called" << std::endl;
#endif
     };
   template <class Arg7>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7>::result_type
#else
   typename Sig<Arg7>::ResultType
#endif
   operator()(const Arg7& q) const {
#ifdef FCPP_PRINT_HELPERS7
     std::cout << "binder1to6of7 operator() called" << std::endl;
#endif
     return f(a1,a2,a3,a4,a5,a6,q);
   }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     if ( debug_traits_has_name<Septimary>() ) {
       std::string bindername = std::string("binder1to6of7::")
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
        return std::string("binder1to6of7");
   }
#endif
};

struct Bind1to6of7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to6of7");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Septimary, class A1, class A2, class A3,
                             class A4, class A5, class A6>
   struct Sig
    : public FunType<Septimary,A1,A2,A3,A4,A5,A6,
      Full1<binder1to6of7<Septimary,A1,A2,A3,A4,A5,A6> > > {};
#endif

  template <class Septimary, class A1, class A2, class A3,
                             class A4, class A5, class A6>
    inline Full1<binder1to6of7<Septimary,A1,A2,A3,A4,A5,A6> >
    operator()( const Septimary& f, const A1& a1, const A2& a2, const A3& a3,
                const A4& a4, const A5& a5, const A6& a6 ) const {
    return makeFull1( binder1to6of7<Septimary,A1,A2,A3,A4,A5,A6>
                    (f,a1,a2,a3,a4,a5,a6) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 6 arguments using BindUtoZof7
// All that is needed is more specialisations of binderUtoZof7Helper
// and the corresponding SigN definitions and concept maps.
/////////////////////////////////////////////////////////////////////

template <typename BinderU,typename BinderV,typename BinderW,typename BinderX,
  typename BinderY,typename BinderZ, typename R,typename F,
  typename ArgU,typename ArgV,typename ArgW,typename ArgX,typename ArgY, 
  typename ArgZ, typename A>
struct binderUtoZof7Helper;

 template <typename R,typename F,typename ArgU,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A>
   struct binderUtoZof7Helper<Bind1,Bind2,Bind3,Bind4,Bind5,Bind6,R,F,
   ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f,const ArgU& argU,const ArgV& argV,
                      const ArgW& argW, const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,const A& a)
  {
    return f(argU,argV,argW,argX,argY,argZ,a);
  }
};

 template <typename R,typename F,typename ArgU,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A>
   struct binderUtoZof7Helper<Bind2,Bind3,Bind4,Bind5,Bind6,Bind7,R,F,
   ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f,const ArgU& argU,const ArgV& argV,
                      const ArgW& argW, const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,const A& a)
  {
    return f(a,argU,argV,argW,argX,argY,argZ);
  }
};

 template <typename R,typename F,typename ArgU,typename ArgV,typename ArgW,typename ArgX,
   typename ArgY,typename ArgZ,typename A>
   struct binderUtoZof7Helper<Bind1,Bind3,Bind4,Bind5,Bind6,Bind7,R,F,
   ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A>
{
  static inline R go( const F& f,const ArgU& argU,const ArgV& argV,
                      const ArgW& argW, const ArgX& argX,
                      const ArgY& argY, const ArgZ& argZ,const A& a)
  {
    return f(argU,a,argV,argW,argX,argY,argZ);
  }
};

 template <class Septimary, typename PairU,typename PairV,typename PairW,
                            typename PairX,typename PairY,typename PairZ>
class binderUtoZof7
{
     const Septimary f;
     PairU pu;
     PairV pv;
     PairW pw;
     PairX px;
     PairY py;
     PairZ pz;
     // These give the types for later
     typedef typename PairU::first_type BinderU;
     typedef typename PairU::second_type ArgU;
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
     binderUtoZof7( const Septimary& g,const PairU& qu,const PairV& qv,
                    const PairW& qw,const PairX& qx,const PairY& qy,
                    const PairZ& qz) :
       f(g), pu(qu), pv(qv), pw(qw), px(qx), py(qy), pz(qz) {}

#ifndef FCPP_CONCEPTS
       template <typename BindU,typename BindV,typename BindW, typename BindX, typename BindY, typename BindZ,
	 typename A>
     struct SigN;

       template <typename A>
	 struct  SigN<Bind1,Bind2,Bind3,Bind4,Bind5,Bind6,A>
	 : public FunType<Septimary,ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A,
	 typename RT<Septimary,ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A>::ResultType >
       { };

       template <typename A>
	 struct  SigN<Bind1,Bind3,Bind4,Bind5,Bind6,Bind7,A>
	 : public FunType<Septimary,ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A,
	 typename RT<Septimary,ArgU,A,ArgV,ArgW,ArgX,ArgY,ArgZ>::ResultType >
       { };

       template <typename A>
	 struct Sig : public SigN<BinderU,BinderV,BinderW,BinderX,BinderY,BinderZ,A>
     { };

#endif

       template <typename A>
#ifdef FCPP_CONCEPTS
	 inline ConstCallable13<Septimary,BinderU,BinderV,BinderW,BinderX,BinderY,BinderZ,
	 ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A>::result_type
#else
       typename Sig<A>::ResultType
#endif
       operator()(const A& a) const
   {
#ifdef FCPP_CONCEPTS
     return binderUtoZof7Helper<BinderU,BinderV,BinderW,BinderX,BinderY,BinderZ,
       typename ConstCallable13<Septimary,BinderU,BinderV,BinderW,BinderX,BinderY,BinderZ,
       ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A>::result_type,
       Septimary,ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A>::go
       (f,pu.second,pv.second,pw.second,px.second,py.second,pz.second,a);
#else
     return binderUtoZof7Helper<BinderU,BinderV,BinderW,BinderX,BinderY,BinderZ,
       typename Sig<A>::ResultType,Septimary,
       ArgU,ArgV,ArgW,ArgX,ArgY,ArgZ,A>::go
       (f,pu.second,pv.second,pw.second,px.second,py.second,pz.second,a);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Septimary>() ) {
       std::string temp = std::string("binderUtoZof7::")
                        + debug_traits_name(f);
       if (pu.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(pu.first.n);
       temp += std::string(",") + debug_name_or_value(pu.second) + std::string(")");
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
       return std::string("binderUtoZof7");
    }
#endif
};

struct BindUtoZof7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindUtoZof7");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Septimary,class PairU,class PairV,class PairW,
                            class PairX, class PairY, class PairZ>
   struct Sig
    : public FunType<Septimary,PairU,PairV,PairW,PairX,PairY,PairZ,
    Full1<binderUtoZof7<Septimary,PairU,PairV,PairW,PairX,PairY,PairZ> > > {};
#endif

  template <class Septimary,class PairU,class PairV,class PairW,class PairX, class PairY, class PairZ>
#ifdef FCPP_CONCEPTS
    inline Full1<binderUtoZof7<Septimary,PairU,PairV,PairW,PairX,PairY,PairZ> >
#else
    typename Sig<Septimary,PairU,PairV,PairW,PairX,PairY,PairZ>::ResultType
#endif
    operator()( const Septimary& f,const PairU &pu,const PairV &pv ,
                const PairW &pw ,
                const PairX &px ,const PairY &py, const PairZ &pz ) const
   {
     return makeFull1( binderUtoZof7<Septimary,PairU,PairV,PairW,PairX,
                      PairY,PairZ>(f,pu,pv,pw,px,py,pz) );
   }
};

//////////////////////////////////////////////////////////////////////
// bind all 7 arguments
// Implementation involves Full8.
/////////////////////////////////////////////////////////////////////

template <class Septimary, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
class binder1to7of7
#ifndef FCPP_CONCEPTS
  : public CFunType<typename RT<Septimary,A1,A2,A3,A4,A5,A6,A7>::ResultType>
#endif
{
   const Septimary f;
   const A1 a1;
   const A2 a2;
   const A3 a3;
   const A4 a4;
   const A5 a5;
   const A6 a6;
   const A7 a7;
public:
   binder1to7of7( const Septimary& g, 
     const A1& x, const A2& y, const A3& z, const A4& v, const A5& w, const A6& u, const A7 &t )
     : f(g), a1(x), a2(y), a3(z), a4(v), a5(w), a6(u), a7(t) {}

#ifdef FCPP_CONCEPTS
     inline ConstCallable7<Septimary,A1,A2,A3,A4,A5,A6,A7>::result_type
#else
       typename RT<Septimary,A1,A2,A3,A4,A5,A6,A7>::ResultType
#endif
   operator()() const
    {
#ifdef FCPP_PRINT_HELPERS7
       std::cout << "call to binder1to7of7" << std::endl;
#endif
       return f(a1,a2,a3,a4,a5,a6,a7);
    }
#ifdef FCPP_DEBUG
   // This code allows for the fact that the first argument of a binder
   // can be a functoid - so the name is useful.
   std::string name() const {
     if ( debug_traits_has_name<Septimary>() ) {
       std::string temp = std::string("binder1to7of7::")
         + debug_traits_name(f);
      if ( debug_traits_has_name<A1>() ) {
         temp += std::string("::")
              + debug_traits_name(a1) + std::string("( ");
       } else {
         temp += std::string("( ") + argument_traits_value(a1) + std::string(" , ");
       }
       temp += argument_traits_value(a2)
            +  std::string(" , ")
            +  argument_traits_value(a3)
            +  std::string(" , ")
            +  argument_traits_value(a4)
            +  std::string(" , ")
            +  argument_traits_value(a5)
            +  std::string(" , ")
            +  argument_traits_value(a6)
            +  std::string(" , ")
            +  argument_traits_value(a7)
            +  std::string(" )");
       return temp;
   } else
        return std::string("binder1to7of7");
   }
#endif
};

struct Bind1to7of7 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1to7of7");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Septimary, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
   struct Sig
    : public FunType<Septimary,A1,A2,A3,A4,A5,A6,A7,
    Full0<binder1to7of7<Septimary,A1,A2,A3,A4,A5,A6,A7> > > {};
#endif

  template <class Septimary, class A1, class A2, class A3, class A4, class A5,class A6,class A7>
    inline Full0<binder1to7of7<Septimary,A1,A2,A3,A4,A5,A6,A7> >
    operator()( const Septimary& f, const A1& a1, const A2& a2, const A3& a3,
		const A4& a4, const A5& a5, const A6& a6, const A7& a7) const {
#ifdef FCPP_PRINT_HELPERS7
    std::cout << "Bind1to7of7 operator() called" << std::endl;
#endif
    return makeFull0( binder1to7of7<Septimary,A1,A2,A3,A4,A5,A6,A7>
		      (f,a1,a2,a3,a4,a5,a6,a7) );
   }
};


struct Curry7 {

  template <class Septimary, class A1, class A2 = void, class A3 = void,
     class A4 = void, class A5 = void, class A6 = void, class A7 = void>
    struct Sig  // Now available.
    : public FunType<Septimary, A1, A2, A3, A4, A5, A6, A7,
    Full0<binder1to7of7
    <Septimary,A1,A2,A3,A4,A5,A6,A7> > > {};

  template <class Septimary>
  struct Sig<Septimary, void >
    : public Full7<Septimary> {};

  template <class Septimary, class A1> struct Sig<Septimary, A1>
    : public FunType<Septimary, A1, void, void, void, void,
                   Full6<binder1of7<Septimary,A1> > > {};

  template <class Septimary, class A1, class A2>
  struct Sig<Septimary, A1, A2>
    : public FunType<Septimary, A1, A2, void, void, void ,
                   Full5<binder1and2of7<Septimary,A1,A2> > > {};

   template <class Septimary, class A2>
   struct Sig<Septimary,AutoCurryType,A2>
   : public FunType<Septimary,A2, void, void, void ,
          Full6<impl::binderXof7<Septimary,std::pair<Bind2,A2> > > > {};

  template <class Septimary, class A1, class A2, class A3>
      struct Sig<Septimary, A1, A2, A3>
    : public FunType<Septimary, A1, A2, A3, void, void ,
    Full4<binder1to3of7<Septimary,A1,A2,A3> > > {};

  template <class Septimary, class A1, class A2, class A3, class A4>
    struct Sig<Septimary, A1, A2, A3, A4>
    : public FunType<Septimary, A1, A2, A3, A4, void ,
    Full3<binder1to4of7<Septimary,A1,A2,A3,A4> > > {};

  template <class Septimary, class A1, class A2, class A3, class A4, class A5>
    struct Sig<Septimary, A1, A2, A3, A4, A5 >
    : public FunType<Septimary, A1, A2, A3, A4, A5 ,
    Full2<binder1to5of7<Septimary,A1,A2,A3,A4,A5> > > {};

  template <class Septimary, class A1, class A2, class A3,
                             class A4, class A5, class A6>
    struct Sig<Septimary, A1, A2, A3, A4, A5, A6 >
    : public FunType<Septimary, A1, A2, A3, A4, A5 , A6,
    Full1<binder1to6of7<Septimary,A1,A2,A3,A4,A5,A6> > > {};

  template <class Septimary>
  inline Septimary
  operator()( const Septimary& f) const {
    bool b = &FunctoidTraits<Septimary>::template ensure_accepts<7>::args;
    assert(FunctoidTraits_max_args<Septimary>() == 7);
    return f;
  }

  template <class Septimary, class A1>
   inline Full6<binder1of7<Septimary,A1> >
  operator()( const Septimary& f, const A1& a1 ) const {
    return makeFull6( binder1of7<Septimary,A1>(f,a1) );
  }

  template <class Septimary, class A1, class A2>
    inline Full5<binder1and2of7<Septimary,A1,A2> >
  operator()( const Septimary& f, const A1& a1, const A2& a2 ) const {
    return makeFull5( binder1and2of7<Septimary,A1,A2>(f,a1,a2) );
  }

  template <class Septimary, class A2>
    inline Full6<binderXof7<Septimary,std::pair<Bind2,A2> > >
  operator()( const Septimary& f, const AutoCurryType&, const A2& a2 ) const {
    return makeFull6( binderXof7<Septimary,std::pair<Bind2,A2> >(f,std::make_pair(bind_second,a2) ) );
  }

  template <class Septimary, class A1, class A2, class A3>
    inline Full4<binder1to3of7<Septimary,A1,A2,A3> >
  operator()( const Septimary& f, const A1& a1, const A2& a2, const A3& a3 ) const {
    return makeFull4( binder1to3of7<Septimary,A1,A2,A3>(f,a1,a2,a3) );
  }

 template <class Septimary, class A1, class A2, class A3, class A4>
#ifdef FCPP_CONCEPTS
    inline Full3<binder1to4of7<Septimary,A1,A2,A3,A4> >
#else
  typename Sig<Septimary, A1, A2, A3, A4>::ResultType
#endif
    operator()( const Septimary& f, const A1& a1, const A2& a2,
              const A3& a3, const A4& a4 ) const {
#ifdef FCPP_PRINT_HELPERS7
    std::cout << "Curry7 called for 4 args" << std::endl;
#endif
    return makeFull3( binder1to4of7<Septimary,A1,A2,A3,A4>
                      (f,a1,a2,a3,a4) );
  }

  template <class Septimary, class A1, class A2, class A3, class A4,class A5>
   typename Sig<Septimary, A1, A2, A3, A4, A5>::ResultType
    operator()( const Septimary& f, const A1& a1, const A2& a2,
		const A3& a3, const A4& a4, const A5& a5 ) const {
#ifdef FCPP_PRINT_HELPERS7
    std::cout << "Curry7 called for 5 args" << std::endl;
#endif
    return makeFull2( binder1to5of7<Septimary,A1,A2,A3,A4,A5>
                      (f,a1,a2,a3,a4,a5) );
  }

  template <class Septimary, class A1, class A2, class A3,
                             class A4,class A5, class A6>
  typename Sig<Septimary, A1, A2, A3, A4, A5, A6>::ResultType
    operator()( const Septimary& f, const A1& a1, const A2& a2,
	const A3& a3, const A4& a4, const A5& a5, const A6& a6 ) const {
#ifdef FCPP_PRINT_HELPERS7
    std::cout << "Curry7 called for 6 args" << std::endl;
#endif
    return makeFull1( binder1to6of7<Septimary,A1,A2,A3,A4,A5,A6>
                      (f,a1,a2,a3,a4,a5,a6) );
  }

  template <class Septimary, class A1, class A2, class A3, class A4,
                             class A5, class A6, class A7>
    typename Sig<Septimary, A1, A2, A3, A4, A5, A6, A7>::ResultType
  operator()( const Septimary& f, const A1& a1, const A2& a2, const A3& a3,
              const A4& a4, const A5& a5, const A6& a6, const A7& a7) const {
    return makeFull0( binder1to7of7
		      <Septimary,A1,A2,A3,A4,A5,A6,A7>
		      (f,a1,a2,a3,a4,a5,a6,a7) );
  }
#ifdef FCPP_DEBUG
   std::string name() const {
       return std::string("Curry7");
   }
#endif

};

#endif
