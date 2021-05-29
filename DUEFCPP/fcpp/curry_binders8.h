// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_BINDERS8_DOT_H
#define FCPP_CURRY_BINDERS8_DOT_H


//////////////////////////////////////////////////////////////////////
// These are the binders for Full8 and possible Fun8
// - removed from curry_binders7.h.
// Concept maps (if any) are in curry_concept_maps.h
//////////////////////////////////////////////////////////////////////
// bind 1 argument of 8 - there are 8 possibilities.
// Only the first is implemented for now.
//////////////////////////////////////////////////////////////////////


template <class Octavary, class Arg1>
class binder1of8 {
   const Octavary f;
   const Arg1 x;
public:
   binder1of8( const Octavary& a, const Arg1& b ) : f(a), x(b) {}

#ifndef FCPP_CONCEPTS
     template <class Arg2, class Arg3, class Arg4, class Arg5,class Arg6,class Arg7, class Arg8 > struct Sig
   : public FunType<typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                         Arg5,Arg6,Arg7,Arg8>::Arg2Type,
         typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                         Arg5,Arg6,Arg7,Arg8>::Arg3Type,
         typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                         Arg5,Arg6,Arg7,Arg8>::Arg4Type,
         typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                         Arg5,Arg6,Arg7,Arg8>::Arg5Type,
         typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                         Arg5,Arg6,Arg7,Arg8>::Arg6Type,
         typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                         Arg5,Arg6,Arg7,Arg8>::Arg7Type,
         typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                         Arg5,Arg6,Arg7,Arg8>::Arg8Type,
         typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                         Arg5,Arg6,Arg7,Arg8>::ResultType> {};
#endif

   template <class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8 >
#ifdef FCPP_CONCEPTS
     inline ConstCallable8<Octavary,Arg1,Arg2,Arg3,Arg4,
                                    Arg5,Arg6,Arg7,Arg8>::result_type
#else
     typename RT<Octavary,Arg1,Arg2,Arg3,Arg4,
                          Arg5,Arg6,Arg7,Arg8>::ResultType
#endif
     operator()(const Arg2& y, const Arg3& z, const Arg4& s,
                const Arg5& t, const Arg6& u, const Arg7&v, const Arg8&w ) const {
     return f(x,y,z,s,t,u,v,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Octavary>() ) {
       if ( debug_traits_has_name<Arg1>() ) {
	  return std::string("binder1of8::") + debug_traits_name(f)
	    + std::string ("::") + debug_traits_name(x);
       } else
	  return std::string("binder1of8::") + debug_traits_name(f);
   } else
        return std::string("binder1of8");
   }
#endif
};

struct Bind1of8 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1of8");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Octavary, class Arg1>
   struct Sig
   : public FunType<Octavary,Arg1,Full7<binder1of8<Octavary,Arg1> > > {};
#endif

   template <class Octavary, class Arg1>
#ifdef FCPP_CONCEPTS
   inline Full7<binder1of8<Octavary,Arg1> >
#else
   typename Sig<Octavary,Arg1>::ResultType
#endif
   operator()( const Octavary& f, const Arg1& x ) const {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "full8 binder1of8 called" << std::endl;
#endif
      return makeFull7( binder1of8<Octavary,Arg1>(f,x) );
   }
};

////////////////////////////////////////////////////////////////////////
// binderXof8 provides the remainder.
// All that is needed is more specialisations of binderXof8Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////

 template <typename Binder,typename R,typename F,typename ArgA,
   typename A,typename B,typename C,typename D, typename E, typename G, typename H>
struct binderXof8Helper;

 template <typename R,typename F,typename ArgA,
   typename A,typename B,typename C,typename D, typename E, typename G,
   typename H>
   struct binderXof8Helper<Bind1,R,F,ArgA,A,B,C,D,E,G,H>
{
  static inline R go( const F& f, const ArgA& argA,
		      const A& a, const B& b, const C& c, const D& d, const E& e, const G& g, const H& h)
  {
    return f(argA,a,b,c,d,e,g,h);
  }
};

 template <typename R,typename F,typename ArgA,
   typename A,typename B,typename C,typename D, typename E, typename G,
   typename H>
   struct binderXof8Helper<Bind2,R,F,ArgA,A,B,C,D,E,G,H>
{
  static inline R go( const F& f, const ArgA& argA,
		      const A& a, const B& b, const C& c, const D& d, const E& e, const G& g, const H& h)
  {
    return f(a,argA,b,c,d,e,g,h);
  }
};

 template <typename R,typename F,typename ArgA,
   typename A,typename B,typename C,typename D, typename E, typename G,
   typename H>
   struct binderXof8Helper<Bind3,R,F,ArgA,A,B,C,D,E,G,H>
{
  static inline R go( const F& f, const ArgA& argA,
		      const A& a, const B& b, const C& c, const D& d,
                      const E& e, const G& g, const H& h)
  {
    return f(a,b,argA,c,d,e,g,h);
  }
};

template <class Octavary, typename Pair>
class binderXof8
{
     const Octavary f;
     Pair p;
    // These give the types for later
     typedef typename Pair::first_type Binder;
     typedef typename Pair::second_type ArgA;
public:
   binderXof8( const Octavary& g,const Pair& q) : f(g),
     p(q) {}

#ifndef FCPP_CONCEPTS
     template <typename Bind, typename A, typename B, typename C, typename D,
                              typename E, typename G, typename H>
     struct SigN;

     template <typename A, typename B, typename C, typename D,
               typename E, typename G, typename H>
       struct  SigN<Bind1,A,B,C,D,E,G,H>
       : public FunType<Octavary,ArgA,A,B,C,D,E,G,H,
       typename RT<Octavary,ArgA,A,B,C,D,E,G,H>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,
               typename E, typename G, typename H>
       struct  SigN<Bind2,A,B,C,D,E,G,H>
       : public FunType<Octavary,ArgA,A,B,C,D,E,G,H,
       typename RT<Octavary,A,ArgA,B,C,D,E,G,H>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D,
                          typename E, typename G, typename H>
       struct  SigN<Bind3,A,B,C,D,E,G,H>
       : public FunType<Octavary,ArgA,A,B,C,D,E,G,H,
       typename RT<Octavary,A,B,ArgA,C,D,E,G,H>::ResultType >
       { };

     template <typename A,typename B,typename C,typename D,
               typename E, typename G, typename H>
       struct Sig : public SigN<Binder,A,B,C,D,E,G,H>
     { };

#endif

     template <typename A,typename B,typename C,typename D,
               typename E, typename G, typename H>
#ifdef FCPP_CONCEPTS
       inline ConstCallable9<Octavary,Binder,ArgA,A,B,C,D,E,G,H>::result_type
#else
       typename Sig<A,B,C,D,E,G,H>::ResultType
#endif
   operator()
       (const A& a, const B& b, const C& c, const D& d,
        const E& e, const G& g, const H& h) const
   {
#ifdef FCPP_CONCEPTS
      return binderXof8Helper<Binder,
	typename ConstCallable9<Octavary,Binder,ArgA,A,B,C,D,E,G,H>::result_type,
        Octavary,ArgA,A,B,C,D,E,G,H>::go(f,p.second,a,b,c,d,e,g,h);
#else
     return binderXof8Helper<Binder,
       typename Sig<A,B,C,D,E,G,H>::ResultType,Octavary,
       ArgA,A,B,C,D,E,G,H>::go(f,p.second,a,b,c,d,e,g,h);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Octavary>() ) {
       std::string temp = std::string("binderXof8::")
                        + debug_traits_name(f);
       if (p.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(p.first.n);
       temp += std::string(",") + debug_name_or_value(p.second) + std::string(")");
       return temp;
     } else
       return std::string("binderXof8");
    }
#endif
};


struct BindXof8 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXof8");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Octavary, class Pair>
   struct Sig
    : public FunType<Octavary,Pair,Full7<binderXof8<Octavary,Pair> > > {};
#endif

  template <class Octavary, class Pair>
#ifdef FCPP_CONCEPTS
    inline Full7<binderXof8<Octavary,Pair> >
#else
    typename Sig<Octavary,Pair>::ResultType
#endif
    operator()( const Octavary& f,const Pair &p ) const {
    return makeFull7( binderXof8<Octavary,Pair>(f,p) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 2 arguments - there are 28 possibilities.
// Only one is implemented as yet.
//////////////////////////////////////////////////////////////////////

template <class Octavary, class Arg1, class Arg2>
class binder1and2of8 {
   const Octavary f;
   const Arg1 a1;
   const Arg2 a2;
public:
#ifndef FCPP_CONCEPTS
   template <class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8>
   struct Sig
       : public FunType<
     typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                     Arg5,Arg6,Arg7,Arg8>::Arg3Type,
     typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                     Arg5,Arg6,Arg7,Arg8>::Arg4Type,
     typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                     Arg5,Arg6,Arg7,Arg8>::Arg5Type,
     typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                     Arg5,Arg6,Arg7,Arg8>::Arg6Type,
     typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                     Arg5,Arg6,Arg7,Arg8>::Arg7Type,
     typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                     Arg5,Arg6,Arg7,Arg8>::Arg8Type,
     typename Octavary::template Sig<Arg1,Arg2,Arg3,Arg4,
                                     Arg5,Arg6,Arg7,Arg8>::ResultType> {};
#endif
   
   binder1and2of8(const Octavary& w, const Arg1& x, const Arg2& y) :
     f(w), a1(x), a2(y) {};
     template <class Arg3,class Arg4,class Arg5,class Arg6,class Arg7,class Arg8>
#ifdef FCPP_CONCEPTS
       inline ConstCallable8<Octavary,Arg1,Arg2,Arg3,Arg4,
                                      Arg5,Arg6,Arg7,Arg8>::result_type
#else
       typename Sig<Arg3,Arg4,Arg5,Arg6,Arg7,Arg8>::ResultType
#endif
       operator()(const Arg3& z, const Arg4& s, const Arg5& u, const Arg6& v, const Arg7 &w, const Arg8 &x) const
       { return f(a1,a2,z,s,u,v,w,x); }
#ifdef FCPP_DEBUG
   std::string name() const {
     if ( debug_traits_has_name<Octavary>() ) {
       std::string bindername = std::string("binder1and2of8::")+
              debug_traits_name(f);
       bindername += std::string("( ")  + debug_name_or_value(a1);
       bindername += std::string(" , ") + debug_name_or_value(a2);
       bindername += std::string(" )");
       return bindername;
   } else
        return std::string("binder1and2of8");
   }
#endif
};

struct Bind1and2of8 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Bind1and2of8");
   }
#endif
#ifndef FCPP_CONCEPTS
   template <class Octavary, class A1, class A2>
   struct Sig
   : public FunType<Octavary,A1,A2,Full6<binder1and2of8<Octavary,A1,A2> > > {};
#endif

   template <class Octavary, class A1, class A2>
   inline Full6<binder1and2of8<Octavary,A1,A2> >
   operator()( const Octavary& f, const A1& a1, const A2& a2 ) const {
      return makeFull6( binder1and2of8<Octavary,A1,A2>(f,a1,a2) );
   }
};

////////////////////////////////////////////////////////////////////////
// binderXandYof8 provides the remainder.
// All that is needed is more specialisations of binderXandYof8Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////
template <typename BinderX,typename BinderY,typename R,typename F,
  typename ArgX,typename ArgY,typename A,typename B,typename C,typename D,
  typename E, typename G>
struct binderXandYof8Helper;

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C,typename D, typename E,typename G>
  struct binderXandYof8Helper<Bind1,Bind2,R,F,ArgX,ArgY,A,B,C,D,E,G>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d, const E& e,const G& g)
  {
    return f(argX,argY,a,b,c,d,e,g);
  }
};


template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C,typename D,typename E, typename G>
  struct binderXandYof8Helper<Bind1,Bind3,R,F,ArgX,ArgY,A,B,C,D,E,G>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d,
                      const E& e, const G& g)
  {
    return f(argX,a,argY,b,c,d,e,g);
  }
};

template <typename R,typename F,typename ArgX,typename ArgY,
  typename A,typename B,typename C,typename D,typename E,typename G>
  struct binderXandYof8Helper<Bind2,Bind3,R,F,ArgX,ArgY,A,B,C,D,E,G>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
		      const A& a, const B& b, const C& c, const D& d,
                      const E& e,const G& g)
  {
    return f(a,argX,argY,b,c,d,e,g);
  }
};

template <class Octavary, typename PairX, typename PairY>
class binderXandYof8
{
     const Octavary f;
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
     "***** ERROR in binderXandYof8 : BinderX==BinderY : ERROR *****");
#endif
public:
   binderXandYof8( const Octavary& g,const PairX& qx,const PairY& qy) :
     f(g), px(qx), py(qy) {}

#ifndef FCPP_CONCEPTS

     template <typename BindX, typename BindY, typename A, typename B,
     typename C, typename D, typename E, typename G>
     struct SigN;

     template <typename A, typename B, typename C, typename D,typename E,
       typename G>
       struct  SigN<Bind1,Bind2,A,B,C,D,E,G>
       : public FunType<Octavary,ArgX,ArgY,A,B,C,D,E,G,
       typename RT<Octavary,ArgX,ArgY,A,B,C,D,E,G>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E,
     typename G>
       struct  SigN<Bind1,Bind3,A,B,C,D,E,G>
       : public FunType<Octavary,ArgX,ArgY,A,B,C,D,E,G,
       typename RT<Octavary,ArgX,A,ArgY,B,C,D,E,G>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E,
     typename G>
       struct  SigN<Bind2,Bind3,A,B,C,D,E,G>
       : public FunType<Octavary,ArgX,ArgY,A,B,C,D,E,G,
       typename RT<Octavary,A,ArgX,ArgY,B,C,D,E,G>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D,typename E,
       typename G>
       struct  SigN<Bind2,Bind4,A,B,C,D,E,G>
       : public FunType<Octavary,ArgX,ArgY,A,B,C,D,E,G,
       typename RT<Octavary,A,ArgX,B,ArgY,C,D,E,G>::ResultType >
       { };

     template <typename A,typename B,typename C, typename D,typename E,
     typename G>
       struct Sig : public SigN<BinderX,BinderY,A,B,C,D,E,G>
     { };
#endif

     template <typename A,typename B,typename C, typename D,typename E,
     typename G>
#ifdef FCPP_CONCEPTS
       inline ConstCallable10<Octavary,BinderX,BinderY,ArgX,ArgY,
                              A,B,C,D,E,G>::result_type
#else
       typename Sig<A,B,C,D,E,G>::ResultType
#endif
       operator()(const A& a, const B& b, const C& c, const D& d,
                  const E& e, const G& g) const
   {
#ifdef FCPP_CONCEPTS
       return binderXandYof8Helper<BinderX,BinderY,
	 typename ConstCallable10<Octavary,BinderX,BinderY,ArgX,ArgY,
                                  A,B,C,D,E,G>::result_type,
	 Octavary,ArgX,ArgY,A,B,C,D,E,G>::go(f,px.second,py.second,a,b,c,d,e,g);
#else
     return binderXandYof8Helper<BinderX,BinderY,
       typename Sig<A,B,C,D,E,G>::ResultType,Octavary,
       ArgX,ArgY,A,B,C,D,E,G>::go(f,px.second,py.second,a,b,c,d,e,g);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Octavary>() ) {
       std::string temp = std::string("binderXandYof8::")
                        + debug_traits_name(f);
       if (px.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(px.first.n);
       temp += std::string(",") + debug_name_or_value(px.second) + std::string(")");
       if (py.first.n > 0)
          temp += std::string("(Arg") + argument_traits_value(py.first.n);
       temp += std::string(",") + debug_name_or_value(py.second) + std::string(")");
       return temp;
     } else
       return std::string("binderXandYof8");
    }
#endif
};


struct BindXandYof8 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXandYof8");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Octavary, class PairX, class PairY>
   struct Sig
    : public FunType<Octavary,PairX,PairY,
    Full6<binderXandYof8<Octavary,PairX,PairY> > > {};
#endif

  template <class Octavary, class PairX, class PairY>
#ifdef FCPP_CONCEPTS
    inline Full6<binderXandYof8<Octavary,PairX,PairY> >
#else
    typename Sig<Octavary,PairX,PairY>::ResultType
#endif
    operator()( const Octavary& f,const PairX &px ,const PairY &py ) const {
    return makeFull6( binderXandYof8<Octavary,PairX,PairY>(f,px,py) );
   }
};

////////////////////////////////////////////////////////////////////////
// binderXtoZof8 provides all the three parameter bindings.
// All that is needed is more specialisations of binderXtoZof8Helper
// and the corresponding SigN definitions.
///////////////////////////////////////////////////////////////////////
// Note: For concepts, each binder combination also needs a new
//       concept map to be added in curry_concept_maps.h
///////////////////////////////////////////////////////////////////////

template <typename BinderX,typename BinderY,typename BinderZ, typename R,typename F,
  typename ArgX, typename ArgY, typename ArgZ,
  typename A,typename B,typename C,typename D, typename E>
struct binderXtoZof8Helper;

 template <typename R,typename F,typename ArgX,typename ArgY,typename ArgZ,
   typename A,typename B,typename C,typename D,typename E>
   struct binderXtoZof8Helper<Bind1,Bind2,Bind3,R,F,ArgX,ArgY,ArgZ,A,B,C,D,E>
{
  static inline R go( const F& f, const ArgX& argX, const ArgY& argY,
                      const ArgZ& argZ, const A& a, const B& b,
                      const C& c, const D& d, const E& e)
  {
    return f(argX,argY,argZ,a,b,c,d,e);
  }
};

template <class Octavary, typename PairX, typename PairY, typename PairZ>
class binderXtoZof8
{
     const Octavary f;
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
     "***** ERROR in binderXtoZof8 : BinderX==BinderY : ERROR *****");
     static_assert(not std::is_same<BinderX,BinderZ>::value,
     "***** ERROR in binderXtoZof8 : BinderX==BinderZ : ERROR *****");
     static_assert(not std::is_same<BinderY,BinderZ>::value,
     "***** ERROR in binderXtoZof8 : BinderY==BinderZ : ERROR *****");
#endif
public:
     binderXtoZof8( const Octavary& g,const PairX& qx,const PairY& qy,
                         const PairZ& qz) :
       f(g), px(qx), py(qy), pz(qz) {}

#ifndef FCPP_CONCEPTS
     template <typename BindX, typename BindY, typename BindZ,
       typename A, typename B, typename C, typename D, typename E>
     struct SigN;

     template <typename A, typename B, typename C, typename D, typename E>
       struct  SigN<Bind1,Bind2,Bind3,A,B,C,D,E>
       : public FunType<Octavary,ArgX,ArgY,ArgZ,A,B,C,D,E,
       typename RT<Octavary,ArgX,ArgY,ArgZ,A,B,C,D,E>::ResultType >
       { };

     template <typename A, typename B, typename C, typename D, typename E>
       struct Sig : public SigN<BinderX,BinderY,BinderZ,A,B,C,D,E>
     { };
#endif

     template <typename A,typename B,typename C,typename D,typename E>
#ifdef FCPP_CONCEPTS
       inline ConstCallable11<Octavary,BinderX,BinderY,BinderZ,ArgX,ArgY,ArgZ,
                              A,B,C,D,E>::result_type
#else
       typename Sig<A,B,C,D,E>::ResultType
#endif
       operator()(const A& a, const B& b, const C& c, const D& d, const E& e) const
   {
#ifdef FCPP_CONCEPTS
     return binderXtoZof8Helper<BinderX,BinderY,BinderZ,
       typename ConstCallable11<Octavary,BinderX,BinderY,BinderZ,ArgX,ArgY,ArgZ,
                                A,B,C,D,E>::result_type,
       Octavary,ArgX,ArgY,ArgZ,A,B,C,D,E>::go(f,px.second,py.second,pz.second,
                                              a,b,c,d,e);
#else
     return binderXtoZof8Helper<BinderX,BinderY,BinderZ,
       typename Sig<A,B,C,D,E>::ResultType,Octavary,
       ArgX,ArgY,ArgZ,A,B,C,D,E>::go(f,px.second,py.second,pz.second,a,b,c,d,e);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Octavary>() ) {
       std::string temp = std::string("binderXtoZof8::")
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
       return std::string("binderXtoZof8");
    }
#endif
};

struct BindXtoZof8 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindXtoZof8");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Octavary, class PairX, class PairY, class PairZ>
   struct Sig
    : public FunType<Octavary,PairX,PairY,PairZ,
    Full5<binderXtoZof8<Octavary,PairX,PairY,PairZ> > > {};
#endif

  template <class Octavary, class PairX, class PairY, class PairZ>
#ifdef FCPP_CONCEPTS
    inline Full5<binderXtoZof8<Octavary,PairX,PairY,PairZ> >
#else
    typename Sig<Octavary,PairX,PairY,PairZ>::ResultType
#endif
    operator()( const Octavary& f,const PairX &px ,const PairY &py,
                const PairZ &pz ) const
   {
     return makeFull5( binderXtoZof8<Octavary,PairX,PairY,PairZ>(f,px,py,pz) );
   }
};

/////////////////////////////////////////////////////////////////////
// bind 4 arguments using BindWtoZof8
// All that is needed is more specialisations of binderWtoZof8Helper
// and the corresponding SigN definitions and concept maps.
///////////////////////////////////////////////////////////////////////
// Note: For concepts, each binder combination also needs a new
//       concept map to be added in curry_concept_maps.h
///////////////////////////////////////////////////////////////////////

 template <typename BinderW,typename BinderX,typename BinderY,typename BinderZ, typename R,typename F,
   typename ArgW,typename ArgX, typename ArgY, typename ArgZ, typename A, typename B,typename C, typename D>
struct binderWtoZof8Helper;

 template <typename R,typename F,typename ArgW,typename ArgX,typename ArgY,typename ArgZ,
   typename A, typename B, typename C, typename D>
   struct binderWtoZof8Helper<Bind1,Bind2,Bind3,Bind4,R,F,ArgW,ArgX,ArgY,ArgZ,A,B,C,D>
{
  static inline R go( const F& f, const ArgW& argW,const ArgX& argX, const ArgY& argY, const ArgZ& argZ,
		      const A& a,const B& b,const C& c, const D& d)
  {
    return f(argW,argX,argY,argZ,a,b,c,d);
  }
};

template <class Octavary, typename PairW, typename PairX, typename PairY, typename PairZ>
class binderWtoZof8
{
     const Octavary f;
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
     binderWtoZof8( const Octavary& g,const PairW& qw,const PairX& qx,const PairY& qy,
                         const PairZ& qz) :
       f(g), pw(qw), px(qx), py(qy), pz(qz) {}

#ifndef FCPP_CONCEPTS
       template <typename BindW, typename BindX, typename BindY, typename BindZ,
	 typename A,typename B,typename C, typename D>
     struct SigN;

       template <typename A,typename B,typename C, typename D>
	 struct  SigN<Bind1,Bind2,Bind3,Bind4,A,B,C,D>
	 : public FunType<Octavary,ArgW,ArgX,ArgY,ArgZ,A,B,C,D,
	 typename RT<Octavary,ArgW,ArgX,ArgY,ArgZ,A,B,C,D>::ResultType >
       { };

       template <typename A,typename B, typename C, typename D>
	 struct Sig : public SigN<BinderW,BinderX,BinderY,BinderZ,A,B,C,D>
     { };

#endif

       template <typename A, typename B, typename C, typename D>
#ifdef FCPP_CONCEPTS
	 inline ConstCallable12<Octavary,BinderW,BinderX,BinderY,BinderZ,
                ArgW,ArgX,ArgY,ArgZ,A,B,C,D>::result_type
#else
	 typename Sig<A,B,C,D>::ResultType
#endif
	 operator()(const A& a,const B& b,const C& c,const D& d) const
   {
#ifdef FCPP_CONCEPTS
     return binderWtoZof8Helper<BinderW,BinderX,BinderY,BinderZ,
     typename ConstCallable12<Octavary,BinderW,BinderX,BinderY,BinderZ,
                              ArgW,ArgX,ArgY,ArgZ,A,B,C,D>::result_type,
     Octavary,ArgW,ArgX,ArgY,ArgZ,A,B,C,D>::go
     (f,pw.second,px.second,py.second,pz.second,a,b,c,d);
#else
     return binderWtoZof8Helper<BinderW,BinderX,BinderY,BinderZ,
       typename Sig<A,B,C,D>::ResultType,Octavary,
       ArgW,ArgX,ArgY,ArgZ,A,B,C,D>::go(f,pw.second,px.second,py.second,pz.second,
                                        a,b,c,d);
#endif
   }
#ifdef FCPP_DEBUG
    std::string name() const
    {
      if ( debug_traits_has_name<Octavary>() ) {
       std::string temp = std::string("binderWtoZof8::")
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
       return std::string("binderWtoZof8");
    }
#endif

};

struct BindWtoZof8 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("BindWtoZof8");
   }
#endif
#ifndef FCPP_CONCEPTS
  template <class Octavary,class PairW,class PairX, class PairY, class PairZ>
   struct Sig
    : public FunType<Octavary,PairW,PairX,PairY,PairZ,
    Full4<binderWtoZof8<Octavary,PairW,PairX,PairY,PairZ> > > {};
#endif

  template <class Octavary,class PairW,class PairX, class PairY, class PairZ>
#ifdef FCPP_CONCEPTS
    inline Full4<binderWtoZof8<Octavary,PairW,PairX,PairY,PairZ> >
#else
    typename Sig<Octavary,PairW,PairX,PairY,PairZ>::ResultType
#endif
    operator()( const Octavary& f,const PairW &pw ,const PairX &px ,const PairY &py, const PairZ &pz ) const
   {
     return makeFull4( binderWtoZof8<Octavary,PairW,PairX,PairY,PairZ>(f,pw,px,py,pz) );
   }
};


#endif
