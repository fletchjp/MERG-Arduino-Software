//===========================================================
// variadic_operator.h
//===========================================================
//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
// Copyright (c) 2007-2016 John Fletcher
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

// Operator applications for variadic FC++

#ifndef FCPP_VARIADIC_OPERATOR_H
#define FCPP_VARIADIC_OPERATOR_H

namespace variadic_fcpp {

////////////////////////////////////////////////////////////////////////
// MultipliesN
// Functoid which can multiply a variable number of arguments,
// currently 1 to 6.
////////////////////////////////////////////////////////////////////////
 struct MultipliesN  : public SmartFunctoidN
  {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("MultipliesN");
  }
#endif
    // These are put in for the lambda curry problem.
    // Now use ArgN for the argument types.
    template <int N,typename...XYZ>
     struct SigN;

     template <class T>
     struct SigN<1,T>
     {
       typedef T ResultType;
     };

     template <class T, class U>
     struct SigN<2,T,U>
     {
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)* *(U*)(0)) ResultType;
#endif
     };

    template <class T, class U, class V>
    struct SigN<3,T,U,V>
     {
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)* *(U*)(0)* *(V*)(0)) ResultType;
#endif
     };

    template <class T, class U, class V, class W>
    struct SigN<4,T,U,V,W>
     {
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)* *(U*)(0)* *(V*)(0)* *(W*)(0)) ResultType;
#endif
     };

    template <class T, class U, class V, class W, class X>
      struct SigN<5,T,U,V,W,X>
     {
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)* *(U*)(0)* *(V*)(0)* *(W*)(0)* *(X*)(0)) ResultType;
#endif
     };

    template <class T, class U, class V, class W, class X, class Y>
      struct SigN<6,T,U,V,W,X,Y>
     {
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)* *(U*)(0)* *(V*)(0)* *(W*)(0)+ *(X*)(0)* *(Y*)(0)) ResultType;
#endif
      };

    template <typename... XYZ>
    struct Sig : public SigN<sizeof... (XYZ),XYZ...>,
                        ArgN<sizeof... (XYZ),XYZ...>
// : public FunType<XYZ...,typename PromotionN<XYZ...>::ResultT>
    {
#ifndef FCPP_CXX11
      public:
      typedef typename PromotionN<XYZ...>::ResultT ResultType;
#endif
    };

    template <int N,typename... Types>
    struct MultipliesNHelper;

#ifndef FCPP_CXX11
    // Case with zero argument - do I need this at all?
    template <typename... Types>
    struct MultipliesNHelper<0,Types...>
    {
      static typename PromotionN<>::ResultT go(Types...) {
         return horrible_problem; } // I added a return for 440
         // Clang insists on one of the correct type.
    };
#endif

    // Trivial case with one argument
    template <class T>
    struct MultipliesNHelper<1,T>{
      static T go
      ( const T& t)
      {
        return  t;
      }
    };

#ifdef FCPP_CXX11
   template <class T, class U>
   struct MultipliesNHelper<2,T,U>{

   static auto go
#ifdef FCPP_CXX14
     ( const T& t, const U& u ) -> decltype(auto)
#else
     ( const T& t, const U& u ) -> decltype(t*u)
#endif
     {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "MultipliesNHelper<2,T,U> called with "
               << t << " and " << u << std::endl;
#endif
     return  t * u;
     }
   };
#else
    template <class T, class U>
    struct MultipliesNHelper<2,T,U>{
      static typename Promotion<T,U>::ResultT go
      ( const T& t, const U& u )
      {
        typedef typename Promotion<T,U>::ResultT R;
        return  (R)t * (R)u;
      }
    };
#endif

#ifdef FCPP_CXX11
    template <class T, class U, class V>
    struct MultipliesNHelper<3,T,U,V>{

    static auto go
#ifdef FCPP_CXX14
     ( const T& t, const U& u, const V& v   ) -> decltype(auto)
#else
     ( const T& t, const U& u, const V& v  ) -> decltype(t*u*v)
#endif
     {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "MultipliesNHelper<3,T,U,V> called with "
               << t << " , " << u <<" and " << v << std::endl;
#endif
     return  t * u * v;
     }
   };
#else
    template <class T, class U, class V>
    struct MultipliesNHelper<3,T,U,V>{
      static typename Promotion3<T,U,V>::ResultT go
      ( const T& x, const U& y, const V& z )
      {
        typedef typename Promotion3<T,U,V>::ResultT R;
        return  (R)x * (R)y * (R)z;
      }
    };
#endif

#ifdef FCPP_CXX11
    template <class T, class U, class V, class W>
    struct MultipliesNHelper<4,T,U,V,W>{

    static auto go
#ifdef FCPP_CXX14
     ( const T& t, const U& u, const V& v, const W& w ) -> decltype(auto)
#else
     ( const T& t, const U& u, const V& v, const W& w ) -> decltype(t*u*v*w)
#endif
     {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "MultipliesNHelper<4,T,U,V,W> called with "
               << t << " , " << u << " , " << v << " and " << w << std::endl;
#endif
     return  t * u * v * w;
     }
   };
#else
    template <class T, class U, class V, class W>
    struct MultipliesNHelper<4,T,U,V,W>{
      static typename Promotion4<T,U,V,W>::ResultT go
      ( const T& t, const U& u, const V& v, const W& w )
      {
        typedef typename Promotion4<T,U,V,W>::ResultT R;
        return  (R)t * (R)u * (R)v *(R)w;
      }
    };
#endif

#ifdef FCPP_CXX11
    template <class T, class U, class V, class W, class X>
    struct MultipliesNHelper<5,T,U,V,W,X>{

    static auto go
#ifdef FCPP_CXX14
     ( const T& t, const U& u, const V& v, const W& w, const X& x)
     -> decltype(auto)
#else
     ( const T& t, const U& u, const V& v, const W& w, const X& x)
     -> decltype(t*u*v*w*x)
#endif
     {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "MultipliesNHelper<4,T,U,V,W> called with "
               << t << " , " << u << " , " << v << " , " << w
               << " and " << x << std::endl;
#endif
     return  t * u * v * w * x;
     }
   };
#else
    template <class T, class U, class V, class W, class X>
      struct MultipliesNHelper<5,T,U,V,W,X>{
	static typename Promotion5<T,U,V,W,X>::ResultT go
	  ( const T& t, const U& u, const V& v, const W& w, const X& x )
      {
        typedef typename Promotion5<T,U,V,W,X>::ResultT R;
        return  (R)t * (R)u * (R)v * (R)w * (R)x;
      }
    };
#endif

#ifdef FCPP_CXX11
    template <class T, class U, class V, class W, class X, class Y>
      struct MultipliesNHelper<6,T,U,V,W,X,Y>{

    static auto go
#ifdef FCPP_CXX14
     ( const T& t, const U& u, const V& v, const W& w, const X& x, const Y& y)
     -> decltype(auto)
#else
     ( const T& t, const U& u, const V& v, const W& w, const X& x, const Y& y)
     -> decltype(t*u*v*w*x*y)
#endif
     {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "MultipliesNHelper<4,T,U,V,W> called with "
               << t << " , " << u << " , " << v << " , " << w << " , "
               << x <<" and " << y << std::endl;
#endif
     return  t * u * v * w * x * y;
     }
   };
#else
    template <class T, class U, class V, class W, class X, class Y>
      struct MultipliesNHelper<6,T,U,V,W,X,Y>{
	static typename Promotion6<T,U,V,W,X,Y>::ResultT go
	  ( const T& t, const U& u, const V& v, const W& w, const X& x,
            const Y& y )
      {
        typedef typename Promotion6<T,U,V,W,X,Y>::ResultT R;
        return  (R)t * (R)u * (R)v *(R)w * (R)x *(R)y;
      }
    };
#endif

#ifndef FCPP_CXX11
     template <typename... Types>
     struct Result
     {
       public:
       typedef typename PromotionN<Types...>::ResultT Type;
     };
#endif

     template <typename... Types>
#ifdef FCPP_CXX11
     typename Sig<Types...>::ResultType operator()
#else
     // First use of this generalisation...
     typename PromotionN<Types...>::ResultT operator()
#endif
       (const Types&... args) const
    {
      //typedef typename PromotionN<Types...>::ResultT R;
      return MultipliesNHelper<sizeof... (Types),Types...>::go(args...);
    }

     template <typename... Types>
     typename Sig<Types...>::ResultType go_to_it
       (const Types&... args) const
    {
      return MultipliesNHelper<sizeof... (Types),Types...>::go(args...);
    }

     static FullN<MultipliesN>& full() {static FullN<MultipliesN> f; return f;}

     template <typename R>
     static FullNR<MultipliesN,R>& fullR() {static FullNR<MultipliesN,R> f; return f;}
 };
  static FullN<MultipliesN>& multipliesN=MultipliesN::full();


////////////////////////////////////////////////////////////////////////
// PlusN
// Functoid which can add up a variable number of arguments,
// currently 1 to 7.
////////////////////////////////////////////////////////////////////////
 struct PlusN  : public SmartFunctoidN
  {
#ifdef FCPP_DEBUG
  std::string name() const
  {
    return std::string("PlusN");
  }
#endif

    // These are put in for the lambda curry problem.
    // Now use ArgN for the argument types.
    template <int N,typename...XYZ>
     struct SigN;

     template <class T>
     struct SigN<1,T>
     {
      typedef T ResultType;
     };

     template <class T, class U>
     struct SigN<2,T,U>
     {
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)+ *(U*)(0)) ResultType;
#endif
     };

    template <class T, class U, class V>
    struct SigN<3,T,U,V>
     {
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)+ *(U*)(0)+ *(V*)(0)) ResultType;
#endif
     };

    template <class T, class U, class V, class W>
    struct SigN<4,T,U,V,W>
     {
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)+ *(U*)(0)+ *(V*)(0)+ *(W*)(0)) ResultType;
#endif
     };

    template <class T, class U, class V, class W>
    struct SigN<5,T,U,V,W,MoreToCome>
     {
      //#ifndef FCPP_CXX14
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)+ *(U*)(0)+ *(V*)(0)+ *(W*)(0)) ResultType;
#endif
      //#endif
     };

    template <class T, class U, class V, class W, class X>
      struct SigN<5,T,U,V,W,X>
     {
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)+ *(U*)(0)+ *(V*)(0)+ *(W*)(0)+ *(X*)(0)) ResultType;
#endif
     };

    template <class T, class U, class V, class W, class X, class Y>
      struct SigN<6,T,U,V,W,X,Y>
     {
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)+ *(U*)(0)+ *(V*)(0)+ *(W*)(0)+ *(X*)(0)+ *(Y*)(0)) ResultType;
#endif
      };

    template <class T, class U, class V, class W, class X, class Y, class Z>
      struct SigN<7,T,U,V,W,X,Y,Z>
     {
#ifdef FCPP_CXX11
      typedef decltype(*(T*)(0)+ *(U*)(0)+ *(V*)(0)+ *(W*)(0)+ *(X*)(0)+ *(Y*)(0)+ *(Z*)(0)) ResultType;
#endif
     };
     template <typename... XYZ>
       struct Sig : public SigN<sizeof... (XYZ),XYZ...>,
                           ArgN<sizeof... (XYZ),XYZ...>
     {
#ifndef FCPP_CXX11
      public:
      typedef typename PromotionN<XYZ...>::ResultT ResultType;
#endif
     };

    template <int N,typename... Types>
    struct PlusNHelper;

#ifndef FCPP_CXX11
    // Case with zero argument - do I need this at all?
    template <typename... Types>
    struct PlusNHelper<0,Types...>
    {
      static typename PromotionN<>::ResultT go(Types...) {
         return horrible_problem; } // I added a return for 440
         // Clang insists on one of the correct type.
    };
#endif

    // Trivial case with one argument
    template <class T>
    struct PlusNHelper<1,T>{
      static T go
      ( const T& t)
      {
        return  t;
      }
    };

#ifdef FCPP_CXX11
   template <class T, class U>
   struct PlusNHelper<2,T,U>{

   static auto go
#ifdef FCPP_CXX14
     ( const T& t, const U& u ) -> decltype(auto)
#else
     ( const T& t, const U& u ) -> decltype(t+u)
#endif
     {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "plusNHelper<2,T,U> called with " << t << " and " << u
               << std::endl;
#endif
     return  t + u;
     }
   };
#else
    template <class T, class U>
    struct PlusNHelper<2,T,U>{
      static typename Promotion<T,U>::ResultT go
      ( const T& t, const U& u )
      {
        typedef typename Promotion<T,U>::ResultT R;
        return  (R)t + (R)u;
      }
    };
#endif

#ifdef FCPP_CXX11
    template <class T, class U, class V>
    struct PlusNHelper<3,T,U,V>{

    static auto go
#ifdef FCPP_CXX14
     ( const T& t, const U& u, const V& v ) -> decltype(auto)
#else
     ( const T& t, const U& u, const V& v ) -> decltype(t+u+v)
#endif
     {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "plusNHelper<3,T,U,V> called with "
               << t << " , " << u <<" and " << v << std::endl;
#endif
     return  t + u + v;
     }
   };
#else
    template <class T, class U, class V>
    struct PlusNHelper<3,T,U,V>{
      static typename Promotion3<T,U,V>::ResultT go
      ( const T& t, const U& u, const V& v )
      {
        typedef typename Promotion3<T,U,V>::ResultT R;
        return  (R)t + (R)u + (R)v;
      }
    };
#endif

#ifdef FCPP_CXX11
    template <class T, class U, class V, class W>
    struct PlusNHelper<4,T,U,V,W>{

    static auto go
#ifdef FCPP_CXX14
     ( const T& t, const U& u, const V& v, const W& w ) -> decltype(auto)
#else
     ( const T& t, const U& u, const V& v, const W& w ) -> decltype(t+u+v+w)
#endif
     {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "plusNHelper<4,T,U,V,W> called with "
               << t << " , " << u << " , " << v << " and " << w << std::endl;
#endif
     return  t + u + v + w;
     }
   };
#else
    template <class T, class U, class V, class W>
    struct PlusNHelper<4,T,U,V,W>{
      static typename Promotion4<T,U,V,W>::ResultT go
      ( const T& t, const U& u, const V& v, const W& w )
      {
        typedef typename Promotion4<T,U,V,W>::ResultT R;
        return  (R)t + (R)u + (R)v +(R)w;
      }
    };
#endif

#ifdef FCPP_CXX11
    template <class T, class U, class V, class W, class X>
    struct PlusNHelper<5,T,U,V,W,X>{

    static auto go
#ifdef FCPP_CXX14
     ( const T& t, const U& u, const V& v, const W& w, const X& x )
      -> decltype(auto)
#else
     ( const T& t, const U& u, const V& v, const W& w, const X& x )
      -> decltype(t+u+v+w+x)
#endif
     {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "plusNHelper<5,T,U,V,W,X> called with "
               << t << " , " << u << " , " << v << " , " << w << " and " << x
               << std::endl;
#endif
     return  t + u + v + w + x;
     }
   };
#else
    template <class T, class U, class V, class W, class X>
      struct PlusNHelper<5,T,U,V,W,X>{
	static typename Promotion5<T,U,V,W,X>::ResultT go
	  ( const T& t, const U& u, const V& v, const W& w, const X& x )
      {
        typedef typename Promotion5<T,U,V,W,X>::ResultT R;
        return  (R)t + (R)u + (R)v + (R)w + (R)x;
      }
    };
#endif

#ifdef FCPP_CXX11
    template <class T, class U, class V, class W, class X, class Y>
      struct PlusNHelper<6,T,U,V,W,X,Y>{

    static auto go
#ifdef FCPP_CXX14
     ( const T& t, const U& u, const V& v, const W& w, const X& x, const Y& y )
     -> decltype(auto)
#else
     ( const T& t, const U& u, const V& v, const W& w, const X& x, const Y& y )
     -> decltype(t+u+v+w+x+y)
#endif
     {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "plusNHelper<6,T,U,V,W,X,Y> called with "
               << t << " , " << u << " , " << v << " , " << w << " , " << x
               <<" and " << y << std::endl;
#endif
     return  t + u + v + w + x + y;
     }
   };
#else
    template <class T, class U, class V, class W, class X, class Y>
      struct PlusNHelper<6,T,U,V,W,X,Y>{
	static typename Promotion6<T,U,V,W,X,Y>::ResultT go
	  ( const T& t, const U& u, const V& v, const W& w, const X& x, const Y& y )
      {
        typedef typename Promotion6<T,U,V,W,X,Y>::ResultT R;
        return  (R)t + (R)u + (R)v +(R)w + (R)x + (R)y;
      }
    };
#endif

#ifndef FCPP_CXX11
     template <typename... Types>
     struct Result
     {
       public:
       typedef typename PromotionN<Types...>::ResultT Type;
     };
#endif

    //#ifdef FCPP_LAMBDA7

#ifdef FCPP_CXX11
     template <class T, class U, class V, class W, class X, class Y, class Z>
      struct PlusNHelper<7,T,U,V,W,X,Y,Z>{

    static auto go
#ifdef FCPP_CXX14
      ( const T& t, const U& u, const V& v, const W& w, const X& x, const Y& y,
        const Z& z ) -> decltype(auto)
#else
      ( const T& t, const U& u, const V& v, const W& w, const X& x, const Y& y,
        const Z& z ) -> decltype(t+u+v+w+x+y+z)
#endif
     {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "plusNHelper<7,T,U,V,W,X,Y,Z> called with "
               << t << " , " << u << " , " << v << " , " << w << " , " << x <<" , " << y << " and " <<  z 
               << std::endl;
#endif
     return  t + u + v + w + x + y + z;
     }
   };
#else
    template <class T, class U, class V, class W, class X, class Y, class Z>
      struct PlusNHelper<7,T,U,V,W,X,Y,Z>{
      static typename Promotion7<T,U,V,W,X,Y,Z>::ResultT go
	( const T& t, const U& u, const V& v, const W& w, const X& x, const Y& y, const Z& z )
      {
        typedef typename Promotion7<T,U,V,W,X,Y,Z>::ResultT R;
        return  (R)t + (R)u + (R)v +(R)w + (R)x + (R)y + (R)z;
      }
    };
#endif
    //#endif

     template <typename... Types>
#ifdef FCPP_CXX11
     typename Sig<Types...>::ResultType operator()
#else
     // First use of this generalisation...
     typename PromotionN<Types...>::ResultT operator()
#endif
       (const Types&... args) const
    {
      //typedef typename PromotionN<Types...>::ResultT R;
      return PlusNHelper<sizeof... (Types),Types...>::go(args...);
    }

     template <typename... Types>
     typename Sig<Types...>::ResultType go_to_it
       (const Types&... args) const
    {
      return PlusNHelper<sizeof... (Types),Types...>::go(args...);
    }

     static FullN<PlusN>& full() {static FullN<PlusN> f; return f;}

     template <typename R>
     static FullNR<PlusN,R>& fullR() {static FullNR<PlusN,R> f; return f;}
  };
  static FullN<PlusN>& plusN=PlusN::full();

////////////////////////////////////////////////////////////////////////
// TupleN
// Functoid which can make a tuple from a variable number of arguments,
// currently 1 to 4.
////////////////////////////////////////////////////////////////////////

using TR1::tuple;

  template <typename... Types>
  struct TupleN
  {
    public:
    typedef typename TR1::tuple<Types...> ResultType;
  };

  struct MakeTupleN : public SmartFunctoidN
  {

#ifdef FCPP_DEBUG
    std::string name() const
    {
      return std::string("MakeTupleN");
    }
#endif

    template <typename... Types>
    struct Result
    {
      public:
      typedef typename TupleN<Types...>::ResultType Type;
    };

    /*
    template <int N, typename... Types> struct SigN;
    */

    // Use ArgN in place of SigN
    template <typename... Types>
    struct Sig : public ArgN<sizeof...(Types),Types...>
    /*: public FunType<Types...,typename TupleN<Types...>::ResultType > */
    {
      typedef typename TupleN<Types...>::ResultType ResultType;
    };

    /*
    template <int N,typename... Types>
    struct MakeTupleNHelper;

    template <class T>
    struct MakeTupleNHelper<1,T>{
      static tuple<T> go
      ( const T& x)
      {
        return  TR1::make_tuple(x);
      }
    };
    */

     template <typename... Types>
     typename TupleN<Types...>::ResultType operator()
     //TR1::tuple<Types...> operator()
       (const Types&... args) const
    {
      typedef tuple<Types...> Type;
      //return MakeTupleNHelper<sizeof... (Types),Types...>::go(args...);
      return TR1::make_tuple(args...);
    }

     static FullN<MakeTupleN>& full() {static FullN<MakeTupleN> f; return f;}


  };

  static FullN<MakeTupleN>& makeTupleN=MakeTupleN::full();


} // end of variadic_fcpp

#endif
