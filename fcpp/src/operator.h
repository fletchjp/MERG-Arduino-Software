 //
 // Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
 //
 // Permission to use, copy, modify, distribute and sell this software
 // and its documentation for any purpose is granted without fee,
 // provided that the above copyright notice and this permission notice
 // appear in all source code copies and supporting documentation. The
 // software is provided "as is" without any express or implied
 // warranty.

 #ifndef FCPP_OPERATOR_DOT_H
 #define FCPP_OPERATOR_DOT_H

 //////////////////////////////////////////////////////////////////////
 // The goal here is to provide functoids for most C++ operators (e.g.
 // Plus, Greater, ...) as well as conversions between representations.
 // The conversions include ptr_to_fun, for turning function pointers into
 // functoids, stl_to_funN, for turning STL functoids into our functoids,
 // and monomorphizeN, for converting polymorphic direct functoids into
 // monomorphic ones.
 //
 // There's also some miscellaneous stuff at both the beginning and the 
 // end of this file, for lack of a better place to put it.
 //////////////////////////////////////////////////////////////////////
 // Very little has yet been done on this file to convert it for concepts.
 // I have now taken sigs out of ptr_to_fun as part of investigating
 // why it does not work with the development version of the concepts
 // compiler.
 //////////////////////////////////////////////////////////////////////

 #include <utility>
 #include <iterator>
 #include <iostream>
 #include "lambda.h"

 // FC++ version of promotion.
 #ifdef FCPP_USE_PROMOTE
 #include "promote.h"
 #endif
 // LOKI or BOOST traits if asked for.
 // LOKI traits now disabled.
 #include "traits.h"

 namespace fcpp {

 //////////////////////////////////////////////////////////////////////
 // syntactic sugar for infix operators
 //////////////////////////////////////////////////////////////////////
 // The syntax
 //    arg1 ^fun^ arg2     means     fun( arg1, arg2 )
 // like Haskell's backquotes.
 //
 // I feel justified in this convenient abuse of operator overloading in
 // that it's complete nonsense for someone to try to XOR a value with a
 // 2-argument full functoid.  Put another way, I own Full2<F>s; I can do 
 // whatever I want with them, darn it!  :)
 //
 // Note that it also works on Full3s (with currying).

 /////////////////////////////////////////////////////////////////
 // Note that this has not been implemented for autocurrying.
 // This means that this works:
 // (2 ^fcpp::multiplies^ 2 ) ^fcpp::minus^ 7
 // but this does not:
 // fcpp::map( (_ ^fcpp::multiplies^ 2 ) ^fcpp::minus^ 7 ,li2)
 // This does work:
 // fcpp::map(compose( _ ^fcpp::minus^ 7 , _ ^fcpp::multiplies^ 2) ,li2);
 // The equivalent lambda operation does work:
 // X %fcpp::multiplies% 2 %fcpp::minus% 7
 //
 // I have been experimenting with ways to get around this problem.
 // John Fletcher November 2009
 /////////////////////////////////////////////////////////////////

 /////////////////////////////////////////////////////////////////
 // There is no debug information for infix ^ or %
 /////////////////////////////////////////////////////////////////
 template <class LHS, class Fun>
 struct InfixOpThingy {
    // Note that storing const&s here relies on the fact that temporaries
    // are guaranteed to live for the duration of the full-expression in
    // which they are created.  There's no need to create copies.
    const LHS& lhs;
    const Fun& f;
    InfixOpThingy( const LHS& l, const Fun& ff ) : lhs(l), f(ff) {}
 };

 template <class LHS, class F>
 inline InfixOpThingy<LHS,Full2<F> >
 operator^( const LHS& lhs, const Full2<F>& f ) {
    return InfixOpThingy<LHS,Full2<F> >(lhs,f);
 }

 /*
 template <class F>
 inline InfixOpThingy<AutoCurryType,Full2<F> >
   operator^( const AutoCurryType& lhs, const Full2<F>& f ) {
    return InfixOpThingy<AutoCurryType,Full2<F> >(lhs,f);
 }
 */

 template <class LHS, class F>
 inline InfixOpThingy<LHS,Full3<F> >
 operator^( const LHS& lhs, const Full3<F>& f ) {
    return InfixOpThingy<LHS,Full3<F> >(lhs,f);
 }

 template <class LHS, class FF, class RHS>
 inline typename RT<FF,LHS,RHS>::ResultType
 operator^( const InfixOpThingy<LHS,FF>& x, const RHS& rhs ) {
    return x.f( x.lhs, rhs );
 }

 /*
 template <class FF, class RHS>
 inline typename RT<FF,AutoCurryType,RHS>::ResultType
 operator^( const InfixOpThingy<AutoCurryType,FF>& x, const RHS& rhs ) {
   return x.f( _, rhs );
 }
 */

 template <class LHS, class FF, class G>
 inline typename RT<FF,LHS,Full2<G> >::ResultType
 operator^( const InfixOpThingy<LHS,FF>& x, const Full2<G>& rhs ) {
    return x.f( x.lhs, rhs );
 }

 template <class LHS, class FF, class G>
 inline typename RT<FF,LHS,Full3<G> >::ResultType
 operator^( const InfixOpThingy<LHS,FF>& x, const Full3<G>& rhs ) {
    return x.f( x.lhs, rhs );
 }

 // Extend infix syntax to Full4 and Full5
 // examples, where f4 is a Full4 and f5 a Full5:
 // f4(a,b,c,d)   can be  c ^ ( a ^ f4 ^ b ) ^ d
 // f5(a,b,c,d,e) can be (c ^ ( a ^ f5 ^ b ) ^ d)(e)
 // FC++.1.5.2
 #ifdef FCPP152
 template <class LHS, class F>
 inline InfixOpThingy<LHS,Full4<F> >
 operator^( const LHS& lhs, const Full4<F>& f ) {
    return InfixOpThingy<LHS,Full4<F> >(lhs,f);
 }

 template <class LHS, class FF, class G>
 inline typename RT<FF,LHS,Full4<G> >::ResultType
 operator^( const InfixOpThingy<LHS,FF>& x, const Full4<G>& rhs ) {
    return x.f( x.lhs, rhs );
 }

 template <class LHS, class F>
 inline InfixOpThingy<LHS,Full5<F> >
 operator^( const LHS& lhs, const Full5<F>& f ) {
    return InfixOpThingy<LHS,Full5<F> >(lhs,f);
 }

 template <class LHS, class FF, class G>
 inline typename RT<FF,LHS,Full5<G> >::ResultType
 operator^( const InfixOpThingy<LHS,FF>& x, const Full5<G>& rhs ) {
    return x.f( x.lhs, rhs );
 }

 #endif
 // Furthermore, I just can't help myself from making
 //    arg1 %fun% arg2     mean     fun[ arg1 ][ arg2 ]
 // for use in lambda expressions.  % is a good choice because it binds
 // more tightly than <=, so it's less likely to interfere with "gets"
 // bindings.

 #ifdef FCPP_ENABLE_LAMBDA

 template <class LHS, class Fun>
 struct InfixOpWhatzit {
    // See comment in InfixOpThingy
    const LHS& lhs;
    const Fun& f;
    InfixOpWhatzit( const LHS& l, const Fun& ff ) : lhs(l), f(ff) {}
 };

 template <class LHS, class F>
 inline InfixOpWhatzit<LHS,Full2<F> >
 operator%( const LHS& lhs, const Full2<F>& f ) {
    return InfixOpWhatzit<LHS,Full2<F> >(lhs,f);
 }

 template <class LHS, class F>
 inline InfixOpWhatzit<LHS,Full3<F> >
 operator%( const LHS& lhs, const Full3<F>& f ) {
    return InfixOpWhatzit<LHS,Full3<F> >(lhs,f);
 }

 template <class LHS, class FF, class RHS>
 inline typename LEType<CALL<CALL<FF,LHS>,RHS> >::Type
 operator%( const InfixOpWhatzit<LHS,FF>& x, const RHS& rhs ) {
    return x.f[ x.lhs ][ rhs ];
 }

 template <class LHS, class FF, class G>
 inline typename LEType<CALL<CALL<FF,LHS>,Full2<G> > >::Type
 operator%( const InfixOpWhatzit<LHS,FF>& x, const Full2<G>& rhs ) {
    return x.f[ x.lhs ][ rhs ];
 }

 template <class LHS, class FF, class G>
 inline typename LEType<CALL<CALL<FF,LHS>,Full3<G> > >::Type
 operator%( const InfixOpWhatzit<LHS,FF>& x, const Full3<G>& rhs ) {
    return x.f[ x.lhs ][ rhs ];
 }

 #endif

 //////////////////////////////////////////////////////////////////////
 // operators
 //////////////////////////////////////////////////////////////////////

 namespace impl {
 struct XMakePair {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("MakePair");
   }
 #endif
    template <class A, class B>
    struct Sig : public FunType<A,B,std::pair<A,B> > {};

    template <class A, class B>
    std::pair<A,B> operator()( const A& a, const B& b ) const {
       return std::make_pair(a,b);
    }
 };
 }
 typedef Full2<impl::XMakePair> MakePair;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN MakePair makePair;
 FCPP_MAYBE_NAMESPACE_CLOSE

 #ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XMakePair>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XMakePair &f) { return f.name(); }
       };
    }
 #endif

 namespace impl {
 struct XMin {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Min");
   }
 #endif
#ifdef FCPP_CXX11
   // Note: I have to use + for the operator type here and in
   // the decltype. Using < makes the return type a bool.
   template <class T, class U>
   struct Sig : public FunType<T,U,decltype(*(T*)(0) + *(U*)(0))> { };

   template <class T, class U>
   auto operator()
     ( const T& x, const U& y ) const -> decltype(x+y)
   {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "min<T,U> called with "
               << argument_traits_value(x) << " and "
               << argument_traits_value(y) << std::endl;
#endif
     return  x < y ? x : y;
   }
#else
#ifdef FCPP_USE_PROMOTE
   template <class T, class U>
     struct Sig : public FunType<T,U,typename Promotion<T,U>::ResultT> { };

   template <class T, class U>
     typename Promotion<T,U>::ResultT operator()
     ( const T& x, const U& y ) const
   {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "min<T,U> called with "
               << argument_traits_value(x) << " and "
               << argument_traits_value(y) << std::endl;
#endif
     //typename Promotion<T,U>::ResultT V;
     return  x < y ? x : y;
   }
#else
    template <class T, class U> struct Sig;
#endif
#endif

    template <class T>
    struct Sig<T,T> : public FunType<T,T,T> {};

    template <class T>
    T operator()( const T& x, const T& y ) const {
       return std::less<T>()( x, y ) ? x : y;
    }
 };
 }
 typedef Full2<impl::XMin> Min;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Min min;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
 struct XMax {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Max");
   }
 #endif
#ifdef FCPP_CXX11
   template <class T, class U>
   struct Sig : public FunType<T,U,decltype(*(T*)(0) + *(U*)(0))> { };

   template <class T, class U>
   auto operator()
     ( const T& x, const U& y ) const -> decltype(x+y)
   {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "max<T,U> called with " << x << " and " << y << std::endl;
     //if (x < y ) std::cout << " x < y " << std::endl;
#endif
     //if (x < y ) return y; else return x;
     return  x < y ? y : x;
   }
#else
#ifdef FCPP_USE_PROMOTE
   template <class T, class U>
     struct Sig : public FunType<T,U,typename Promotion<T,U>::ResultT> { };

   template <class T, class U>
     typename Promotion<T,U>::ResultT operator()
     ( const T& x, const U& y ) const
   {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "max<T,U> called with " << x << " and " << y << std::endl;
#endif
     //typename Promotion<T,U>::ResultT V;
     return  x < y ? y : x;
   }
#else
    template <class T, class U> struct Sig;
#endif
#endif

    template <class T>
    struct Sig<T,T> : public FunType<T,T,T> {};

    template <class T>
    T operator()( const T& x, const T& y ) const {
       return std::less<T>()( x, y ) ? y : x;
    }
 };
 }
 typedef Full2<impl::XMax> Max;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Max max;
 FCPP_MAYBE_NAMESPACE_CLOSE

 #ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XMin>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XMin &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XMax>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XMax &f) { return f.name(); }
       };
    }
 #endif



 namespace impl {
 struct XPlus {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Plus");
   }
 #endif
 #ifdef FCPP_CXX11
   template <class T, class U>
   struct Sig : public FunType<T,U,decltype(*(T*)(0)+ *(U*)(0))> { };

   template <class T, class U>
   auto operator()
     ( const T& x, const U& y ) const -> decltype(x+y)
   {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "plus<T,U> called with " << x << " and " << y << std::endl;
#endif
     return  x + y;
   }
 #else
 #ifdef FCPP_USE_PROMOTE
   template <class T, class U>
     struct Sig : public FunType<T,U,typename Promotion<T,U>::ResultT> { };

   template <class T, class U>
     typename Promotion<T,U>::ResultT operator()
     ( const T& x, const U& y ) const
   {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "plus<T,U> called with " << x << " and " << y << std::endl;
#endif
     //typename Promotion<T,U>::ResultT V;
     return  x + y;
   }

 #else
   template <class T, class U> struct Sig;
 #endif
 #endif
    template <class T>
    struct Sig<T,T> : public FunType<T,T,T> {};

    template <class T>
    T operator()( const T& x, const T& y ) const {
#ifdef FCPP_PRINT_HELPERS
       std::cout << "plus<T,T> called with " << x << " and " << y << std::endl;
#endif
      return std::plus<T>()( x, y );
    }
 };
 }
 typedef Full2<impl::XPlus> Plus;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Plus plus;
 FCPP_MAYBE_NAMESPACE_CLOSE

 #ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XPlus>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XPlus &f) { return f.name(); }
       };
    }
 #endif


 namespace impl {
 struct XMinus {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Minus");
   }
 #endif
 #ifdef FCPP_CXX11
   template <class T, class U>
   struct Sig : public FunType<T,U,decltype(*(T*)(0)- *(U*)(0))> { };

   template <class T, class U>
   auto operator()
     ( const T& x, const U& y ) const -> decltype(x-y)
   {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "minus<T,U> called with " << x << " and " << y << std::endl;
#endif
     return  x - y;
   }
 #else
 #ifdef FCPP_USE_PROMOTE
   template <class T, class U>
     struct Sig : public FunType<T,U,typename Promotion<T,U>::ResultT> { };

   template <class T, class U>
     typename Promotion<T,U>::ResultT operator()
     ( const T& x, const U& y ) const
   {
     //typename Promotion<T,U>::ResultT V;
     return  x - y;
   }

 #else
    template <class T, class U> struct Sig;
 #endif
 #endif
   /* Attempt to make minus(multiplies(_,2),7) work. Needs more thought.
   template <class G, class T> struct Sig<Full1<G>,T>
     //     : public FunType<Full1<G>,T,Full1<impl::binder2of2<G,T> > >
   {
     typedef Full1<G> Arg1Type;
     typedef T        Arg2Type;
     typedef Full1<impl::binder2of2<G,T> > ResultType;
   };
   */

    template <class T>
    struct Sig<T,T> : public FunType<T,T,T> {};

    template <class T>
    T operator()( const T& x, const T& y ) const {
       return std::minus<T>()( x, y );
    }

   /*
   template <class G, class T> 
      typename Sig<Full1<G>,T>::ResultType operator() (const Full1<G>& g, const T& t)
   {
     return Full1<binder2of2<G,T> > (g,t);
   }
   */
 };
 
 /* Another idea bites the dust.
 struct XMinusF {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("MinusF");
   }
 #endif
     template <class T, class U> struct Sig;

   // Attempt to make minus(multiplies(_,2),7) work. Needs more thought. 
   template <class G, class T> struct Sig<Full1<G>,T>
     //     : public FunType<Full1<G>,T,Full1<impl::binder2of2<G,T> > > 
   {
     typedef Full1<G> Arg1Type;
     typedef T        Arg2Type;
     typedef Full1<G,T> ResultType;
   };

   
   template <class G, class T> 
      typename Sig<Full1<G>,T>::ResultType operator() (const Full1<G>& g, const T& t)
   {
     return Full1<binder2of2<G,T> > (g,t);
   }
};
*/
}
 typedef Full2<impl::XMinus> Minus;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Minus minus;
 FCPP_MAYBE_NAMESPACE_CLOSE

 #ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XMinus>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XMinus &f) { return f.name(); }
       };
    }
 #endif

 namespace impl {
 struct XMultiplies {
  #ifdef FCPP_CXX11
   template <class T, class U>
   struct Sig : public FunType<T,U,decltype(*(T*)(0)* *(U*)(0))> { };

   template <class T, class U>
   auto operator()
     ( const T& x, const U& y ) const -> decltype(x*y)
   {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "multiplies<T,U> called with " << x << " and " << y << std::endl;
#endif
     return  x * y;
   }
 #else
 #ifdef FCPP_USE_PROMOTE
   template <class T, class U>
     struct Sig : public FunType<T,U,typename Promotion<T,U>::ResultT> { };

   template <class T, class U>
     typename Promotion<T,U>::ResultT operator()
     ( const T& x, const U& y ) const 
   {
     //typename Promotion<T,U>::ResultT V;
     return  x * y;
   }

 #else
    template <class T, class U> struct Sig;
 #endif
 #endif
    template <class T>
    struct Sig<T,T> : public FunType<T,T,T> {};

    template <class T>
    T operator()( const T& x, const T& y ) const {
       return std::multiplies<T>()( x, y );
    }
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Multiplies");
   }
 #endif
 };
 }
 typedef Full2<impl::XMultiplies> Multiplies;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Multiplies multiplies;
 FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XMultiplies>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XMultiplies &f) { return f.name(); }
       };
    }
 #endif

 namespace impl {
 struct XDivides {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Divides");
   }
 #endif
   #ifdef FCPP_CXX11
   template <class T, class U>
   struct Sig : public FunType<T,U,decltype(*(T*)(0)/ *(U*)(0))> { };

   template <class T, class U>
   auto operator()
     ( const T& x, const U& y ) const -> decltype(x/y)
   {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "divides<T,U> called with " << x << " and " << y << std::endl;
#endif
     return  x / y;
   }
 #else
 #ifdef FCPP_USE_PROMOTE
   template <class T, class U>
     struct Sig : public FunType<T,U,typename Promotion<T,U>::ResultT> { };

   template <class T, class U>
     typename Promotion<T,U>::ResultT operator()
     ( const T& x, const U& y ) const 
   {
     //typename Promotion<T,U>::ResultT V;
     return  x / y;
   }

 #else
    template <class T, class U> struct Sig;
 #endif
 #endif
    template <class T>
    struct Sig<T,T> : public FunType<T,T,T> {};

    template <class T>
    T operator()( const T& x, const T& y ) const {
#ifndef FCPP_ARDUINO
      if (y == T(0))
         throw fcpp_exception("About to divide by zero");
#endif
      return std::divides<T>()( x, y );
    }
 };
 }
 typedef Full2<impl::XDivides> Divides;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Divides divides;
 FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XDivides>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XDivides &f) { return f.name(); }
       };
    }
 #endif

 namespace impl {
 struct XModulus {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Modulus");
   }
 #endif
    template <class T, class U> struct Sig;

    template <class T>
    struct Sig<T,T> : public FunType<T,T,T> {};

    template <class T>
    T operator()( const T& x, const T& y ) const {
       return std::modulus<T>()( x, y );
    }
 };
 }
 typedef Full2<impl::XModulus> Modulus;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Modulus modulus;
 FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XModulus>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XModulus &f) { return f.name(); }
       };
    }
 #endif

 namespace impl {
 struct XNegate {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Negate");
   }
 #endif
    template <class T>
    struct Sig : public FunType<T,T> {};

    template <class T>
    T operator()( const T& x ) const {
       return std::negate<T>()( x );
    }
 };
 }
 typedef Full1<impl::XNegate> Negate;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Negate negate;
 FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XNegate>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNegate &f) { return f.name(); }
       };
    }
 #endif

 namespace impl {
 struct XEqual {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Equal");
   }
 #endif

    template <class T, class U> struct Sig;

    template <class T>
    struct Sig<T,T> : public FunType<T,T,bool> {};

    template <class T>
    bool operator()( const T&x, const T&y ) const {
       return std::equal_to<T>()( x, y );
    }
 };
 }
 typedef Full2<impl::XEqual> Equal;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Equal equal;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
 struct XNotEqual {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("NotEqual");
   }
 #endif

    template <class T, class U> struct Sig;

    template <class T>
    struct Sig<T,T> : public FunType<T,T,bool> {};

    template <class T>
    bool operator()( const T&x, const T&y ) const {
       return std::not_equal_to<T>()( x, y );
    }
 };
 }
 typedef Full2<impl::XNotEqual> NotEqual;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN NotEqual notEqual;
 FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XEqual>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XEqual &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XNotEqual>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNotEqual &f) { return f.name(); }
       };
    }
 #endif


 namespace impl {
 struct XGreater {
    template <class T, class U> struct Sig;

    template <class T>
    struct Sig<T,T> : public FunType<T,T,bool> {};

    template <class T>
    bool operator()( const T&x, const T&y ) const {
       return std::greater<T>()( x, y );
    }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Greater");
    }
#endif
};
 }
 typedef Full2<impl::XGreater> Greater;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Greater greater;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
 struct XLess {
    template <class T, class U> struct Sig;

    template <class T>
    struct Sig<T,T> : public FunType<T,T,bool> {};

    template <class T>
    bool operator()( const T&x, const T&y ) const {
      //std::cout << "less called with " << x << " and " << y << std::endl;
       return std::less<T>()( x, y );
    }
 #ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Less");
    }
#endif
};
 }
 typedef Full2<impl::XLess> Less;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Less less;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
 struct XGreaterEqual {
    template <class T, class U> struct Sig;

    template <class T>
    struct Sig<T,T> : public FunType<T,T,bool> {};

    template <class T>
    bool operator()( const T&x, const T&y ) const {
       return std::greater_equal<T>()( x, y );
    }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("GreaterEqual");
    }
#endif
 };
 }
 typedef Full2<impl::XGreaterEqual> GreaterEqual;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN GreaterEqual greaterEqual;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
 struct XLessEqual {
    template <class T, class U> struct Sig;

    template <class T>
    struct Sig<T,T> : public FunType<T,T,bool> {};

    template <class T>
    bool operator()( const T&x, const T&y ) const {
       return std::less_equal<T>()( x, y );
    }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("LessEqual");
    }
#endif
 };
 }
 typedef Full2<impl::XLessEqual> LessEqual;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN LessEqual lessEqual;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
 struct XLogicalAnd {
    template <class T, class U> struct Sig;

    template <class T>
    struct Sig<T,T> : public FunType<T,T,bool> {};

    template <class T>
    bool operator()( const T&x, const T&y ) const {
       return std::logical_and<T>()( x, y );
    }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("LogicalAnd");
    }
#endif
 };
 }
 typedef Full2<impl::XLogicalAnd> LogicalAnd;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN LogicalAnd logicalAnd;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
 struct XLogicalOr {
    template <class T, class U> struct Sig;

    template <class T>
    struct Sig<T,T> : public FunType<T,T,bool> {};

    template <class T>
    bool operator()( const T&x, const T&y ) const {
       return std::logical_or<T>()( x, y );
    }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("LogicalOr");
    }
#endif
  };
}
 typedef Full2<impl::XLogicalOr> LogicalOr;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN LogicalOr logicalOr;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
 struct XLogicalNot {
    template <class T>
    struct Sig : public FunType<T,bool> {};

    template <class T>
    bool operator()( const T&x ) const {
       return std::logical_not<T>()( x );
    }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("LogicalNot");
    }
#endif
 };
}
 typedef Full1<impl::XLogicalNot> LogicalNot;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN LogicalNot logicalNot;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
 struct XDereference {
    template <class T> struct Sig 
    : public FunType<T,typename std::iterator_traits<T>::value_type> {};

    template <class T>
    typename Sig<T>::ResultType operator()( const T& p ) const {
       return *p;
    }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Dereference");
    }
#endif
};
 }
 typedef Full1<impl::XDereference> Dereference;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Dereference dereference;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
 struct XAddressOf {
    template <class T>
    struct Sig : public FunType<T,const T*> {};

    template <class T>
    const T* operator()( const T& x ) const {
       return &x;
    }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("AddressOf");
    }
#endif
 };
 }
 typedef Full1<impl::XAddressOf> AddressOf;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN AddressOf addressOf;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
    struct XDelete_ {
       template <class T> struct Sig : public FunType<T,void> {};
       template <class T> void operator()( T* p ) const { delete p; }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Delete_");
    }
#endif
     };
}
 typedef Full1<impl::XDelete_> Delete_;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Delete_ delete_;
 FCPP_MAYBE_NAMESPACE_CLOSE

 namespace impl {
    template <class Dest>
    struct XDynamicCast {
       template <class Src> struct Sig : public FunType<Src,Dest> {};
       template <class Src>
       Dest operator()( const Src& s ) const {
	  return dynamic_cast<Dest>(s);
       }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("DynamicCast");
    }
#endif
    };
 }
 template <class T>
 struct DynamicCast { typedef Full1<impl::XDynamicCast<T> > Type; };
 template <class T> Full1<impl::XDynamicCast<T> > dynamicCast()
 { return makeFull1( impl::XDynamicCast<T>() ); }

 // outStream is the << stream operator, but takes a stream*
 //    e.g.   &cout ^outStream^ x
 namespace impl {
    struct XOutStream {
       template <class StreamP, class Data> struct Sig
	  : public FunType<StreamP,Data,StreamP> {};
       template <class StreamP, class Data>
       StreamP operator()( StreamP s, const Data& x ) const {
	  (*s) << x;
	  return s;
       }
 #ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("OutStream");
    }
#endif
   };
 }
 typedef Full2<impl::XOutStream> OutStream;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN OutStream outStream;
 FCPP_MAYBE_NAMESPACE_CLOSE

 // inStream is the >> stream operator, but takes stream* and data*
 //    e.g.   &cin ^inStream^ &x
 namespace impl {
    struct XInStream {
       template <class StreamP, class DataP> struct Sig
	  : public FunType<StreamP,DataP,StreamP> {};
       template <class StreamP, class DataP>
       StreamP operator()( StreamP s, DataP x ) const {
         (*s) >> (*x);
         return s;
      }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("InStream");
    }
#endif
   };
}
typedef Full2<impl::XInStream> InStream;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN InStream inStream;
FCPP_MAYBE_NAMESPACE_CLOSE

#ifndef FCPP_I_AM_GCC2
// makeManip(aStream)(aManip) returns the manipulator for that stream
// (The C++ std stream manipulators have a crazy interface which
// necessitates this ugliness.)
//    e.g.  &cout ^outStream^ makeManip(cout)(endl)
template <class C, class T>
struct ManipMaker {
   std::basic_ostream<C,T>& (*
   operator()( std::basic_ostream<C,T>& (*pfn)( std::basic_ostream<C,T>&) )
         const )( std::basic_ostream<C,T>& ) { return pfn; }
   std::basic_ios<C,T>& (*
   operator()( std::basic_ios<C,T>& (*pfn)( std::basic_ios<C,T>& ) )
         const )( std::basic_ios<C,T>& ) { return pfn; }
   std::ios_base& (*
   operator()( std::ios_base& (*pfn)( std::ios_base& ) )
         const )( std::ios_base& ) { return pfn; }
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("ManipMaker");
    }
#endif
};
template <class C, class T>
ManipMaker<C,T> makeManip( std::basic_ios<C,T>& ) { return ManipMaker<C,T>(); }
#endif

//////////////////////////////////////////////////////////////////////
// STL conversions
//////////////////////////////////////////////////////////////////////
// Note that these are template functions, not functoids.  I'm lazy.  

namespace impl {
template <class Op>
class Xstl1 : public CFunType<typename Op::argument_type,
                       typename Op::result_type> {
   Op f;
public:
   Xstl1( const Op& o ) : f(o) {}
   typename Op::result_type 
   operator()( const typename Op::argument_type& x ) const {
      return f(x);
   }
};
}
template <class Op>
Full1<impl::Xstl1<Op> > stl_to_fun1( const Op& o ) {
   return makeFull1( impl::Xstl1<Op>(o) );
}

namespace impl {
template <class Op>
class Xstl2 : public CFunType<typename Op::first_argument_type,
                       typename Op::second_argument_type,
                       typename Op::result_type> {
   Op f;
public:
   Xstl2( const Op& o ) : f(o) {}
   typename Op::result_type 
   operator()( const typename Op::first_argument_type& x, 
               const typename Op::second_argument_type& y ) const {
      return f(x,y);
   }
};
}
template <class Op>
Full2<impl::Xstl2<Op> > stl_to_fun2( const Op& o ) {
   return makeFull2(impl::Xstl2<Op>(o));
}


//////////////////////////////////////////////////////////////////////
// boost lambda conversions (experimental)
// and not in fact useful - action needs to be taken in full.h
//////////////////////////////////////////////////////////////////////

 namespace impl {
 
   // This one does not take an argument at build time
   template <typename Op>
 struct toboostll0_1 
   {
    Op f;
public:
    toboostll0_1( const Op& o) : f(o) {}

   template <class T>
   struct Sig 
   : public FunType<typename Op::template Sig<T>::Arg1Type,
                    typename Op::template Sig<T>::ResultType> {};

   template <class T>
   struct sig {
     typedef T result_type;
   };

    // But instead at run time
    template <typename T>
      typename RT<Op,T>::ResultType 
    operator()(const T& x) const {
      return f(x);
   }
};

   // This is not the correct object.
   template <typename Op, typename Arg1>
 struct toboostll1 : public CFunType<typename RT<Op,Arg1>::ResultType>
   {
    Op f;
    Arg1 x;
public:
    toboostll1( const Op& o, const Arg1& a ) : f(o), x(a) {}

    typename RT<Op,Arg1>::ResultType 
    operator()() const {
      return f(x);
   }
};

 struct XBooster01 {

   template <typename Op>
     struct Sig : public FunType<Op,Full1<toboostll0_1<Op> > > {};

   template <typename Op>
   inline Full1<toboostll0_1<Op> >
     operator() ( const Op& op) const
   {
     return makeFull1( toboostll0_1<Op>(op) );
   }
 };

 // This follows the structure of a Bind object
 // It is intended to help communicate with boost but does not yet work as intended.
 struct XBooster1 {

   template <typename Op, typename Arg1>
     struct Sig : public FunType<Op,Arg1,Full0<toboostll1<Op,Arg1> > > {};

   template <typename Op, typename Arg1>
   inline Full0<toboostll1<Op,Arg1> >
     operator() ( const Op& op, const Arg1& a) const
   {
     return makeFull0( toboostll1<Op,Arg1>(op,a) );
   }
 };
  
}
 typedef Full1<impl::XBooster01> Booster01;
 typedef Full2<impl::XBooster1> Booster1;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Booster01 booster01;
 FCPP_MAYBE_EXTERN Booster1 booster1;
 FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP_USE_BOOST_LAMBDA

namespace impl {
   // This had never occurred to me before to be of any use.
   // I am using it with LC++.
   // I am not sure I ever used it there as I have no working examples.
   // I also cannot sort it out with C++11.
template <class Op>
  class Xboostll0
#ifdef FCPP_CXX11
  //  : public CFunType<decltype (*(Op*)()) >
  : public CFunType<typename RT<Op>::ResultType>
#endif
 //  typename Op::sig::type>
  {
   Op f;
public:
   Xboostll0( const Op& o ) : f(o) {}

   //   template <typename T> struct Sig;

 #ifndef FCPP_CXX11
    template <typename T>
    struct Sig {
     //typedef typename
     //Op::inherited::
     //template sig<typename T::tail_type>::type type;
     // typedef typename boost::tuples::element<1, Op>::type types;
     // typedef typename boost::tuples::element<1, types>::type type;
     //typedef T type;
     // #ifdef FCPP_CXX11
      //typedef decltype(f()) ResultType;
      //#else
     typedef T type;
     typedef type ResultType;
      //#endif
     //typedef type Arg1Type;
   };
 #endif
   //template <>
   //  struct Sig<> { typedef int ResultType; }
   // I am not entirely sure why this works but it does.
   // More investigation may be needed.
 #ifdef FCPP_CXX11
     template <typename T>
     auto operator()() const -> decltype(f())
   {
      return f();
   }
 #else
   template <typename T>
   T  operator()() const {
      return f();
   }
 #endif
};
}
template <class Op>
Full0<impl::Xboostll0<Op> > boostll_to_fun0( const Op& o ) {
   return makeFull0( impl::Xboostll0<Op>(o) );
}

// I need to figure out what the argument type and the result type are called.
// I think I am going to have to remake this as a functoid,
// which I think will help.
// At the moment it fails to locate the call from Full1 to the operator() here.
// I have got over that but I am not at all sure why this works.
// This is now much improved using FC++11 auto and decltype.
namespace impl {
template <class Op>
  class Xboostll1
  {
   Op f;
public:
   Xboostll1( const Op& o ) : f(o) {}

   template <typename T>
   struct Sig {
     typedef T type;
 #ifdef FCPP_CXX11
     typedef decltype(f(*(T*)(0))) ResultType;
 #else
     typedef type ResultType;
 #endif
     typedef type Arg1Type;
   };
 #ifdef FCPP_CXX11
    template <typename T>
   auto operator()( const T&  x ) const -> decltype(f(x))
   {
      return f(x);
   }
 #else
   template <typename T>
   T  operator()( const T&  x ) const {
      return f(x);
   }
 #endif
};
}
template <class Op>
Full1<impl::Xboostll1<Op> > boostll_to_fun1( const Op& o ) {
   return makeFull1( impl::Xboostll1<Op>(o) );
}

namespace impl {
template <class Op>
  class Xboostll2 
  {
   Op f;
public:
   Xboostll2( const Op& o ) : f(o) {}
 #ifdef FCPP_CXX11
   template <class T, class U>
     struct Sig {
     typedef decltype(f(*(T*)(0),*(U*)(0))) ResultType;
     typedef T Arg1Type;
     typedef U Arg2Type;
   };

   template <class T, class U>
   auto operator()
     ( const T& x, const U& y ) const -> decltype(f(x,y))
   {
     return f(x,y);
   }
 #else
   template < typename T, typename U > struct Sig;
 #endif
   template <typename T>
     struct Sig<T,T> {
     typedef T type;
 #ifdef FCPP_CXX11
     typedef decltype(f(*(T*)(0),*(T*)(0))) ResultType;
 #else
     typedef type ResultType;
 #endif
     typedef type Arg1Type;
     typedef type Arg2Type;
   };
 #ifdef FCPP_CXX11
    template <typename T>
   auto operator()( const T&  x, const T& y ) const -> decltype(f(x,y))
   {
     return f(x,y);
   }
 #else
   template <typename T>
     T operator()( const T&  x, const T& y ) const {
     return f(x,y);
   }
 #endif
};
}
template <class Op>
Full2<impl::Xboostll2<Op> > boostll_to_fun2( const Op& o ) {
   return makeFull2( impl::Xboostll2<Op>(o) );
}

namespace impl {
template <class Op>
  class Xboostll3
  //: public CFunType<typename Op::sig::type,
  //  typename Op::sig::type> 
  {
   Op f;
public:
   Xboostll3( const Op& o ) : f(o) {}
 #ifdef FCPP_CXX11
   template <class T, class U, class V>
     struct Sig {
     typedef decltype(f(*(T*)(0),*(U*)(0),*(V*)(0))) ResultType;
     typedef T Arg1Type;
     typedef U Arg2Type;
     typedef V Arg3Type;
   };

   template <class T, class U, class V>
   auto operator()
     ( const T& x, const U& y, const V& z ) const -> decltype(f(x,y,z))
   {
     return f(x,y,z);
   }
 #else
   template < typename T, typename U, typename V > struct Sig;
 #endif
  //template <template < typename S> class T >
   // This is only working for three arguments of the same type - for now.
   template <typename T>
     struct Sig<T,T,T> {
     //typedef typename 
     //Op::inherited:: 
     //template sig<typename T::tail_type>::type type;
     // typedef typename boost::tuples::element<1, Op>::type types;
     // typedef typename boost::tuples::element<1, types>::type type;
     typedef T type;
 #ifdef FCPP_CXX11
     typedef decltype(f(*(T*)(0),*(T*)(0),*(T*)(0))) ResultType;
 #else
     typedef type ResultType;
 #endif
     typedef type Arg1Type;
     typedef type Arg2Type;
     typedef type Arg3Type;
   };
   // I am not entirely sure why this works but it does.
   // More investigation may be needed.
 #ifdef FCPP_CXX11
    template <typename T>
    auto operator()( const T&  x, const T& y, const T &z ) const
      -> decltype(f(x,y,z))
   {
     return f(x,y,z);
   }
 #else
   template <typename T>
     T  operator()( const T&  x, const T& y, const T& z ) const {
     return f(x,y,z);
   }
 #endif
};
}
template <class Op>
Full3<impl::Xboostll3<Op> > boostll_to_fun3( const Op& o ) {
   return makeFull3( impl::Xboostll3<Op>(o) );
}

#endif

#ifdef FCPP_USE_BOOST_PHOENIX
// New section for interaction with BOOST_PHOENIX
namespace impl {
template <class Op>
  class Xboostph1
  {
   Op f;
public:
   Xboostph1( const Op& o ) : f(o) {}

   template <typename T>
   struct Sig {
     typedef T type;
 #ifdef FCPP_CXX11
     typedef decltype(f(*(T*)(0))) ResultType;
 #else
     typedef type ResultType;
 #endif
     typedef type Arg1Type;
   };
 #ifdef FCPP_CXX11
    template <typename T>
   auto operator()( const T&  x ) const -> decltype(f(x))
   {
      return f(x);
   }
 #else
   template <typename T>
   T  operator()( const T&  x ) const {
      return f(x);
   }
 #endif
};
}
template <class Op>
Full1<impl::Xboostph1<Op> > boostph_to_fun1( const Op& o ) {
   return makeFull1( impl::Xboostph1<Op>(o) );
}

namespace impl {
template <class Op>
  class Xboostph2
  {
   Op f;
public:
   Xboostph2( const Op& o ) : f(o) {}
 #ifdef FCPP_CXX11
   template <class T, class U>
     struct Sig {
     typedef decltype(f(*(T*)(0),*(U*)(0))) ResultType;
     typedef T Arg1Type;
     typedef U Arg2Type;
   };

   template <class T, class U>
   auto operator()
     ( const T& x, const U& y ) const -> decltype(f(x,y))
   {
     return f(x,y);
   }
 #else
   template < typename T, typename U > struct Sig;
 #endif
   template <typename T>
     struct Sig<T,T> {
     typedef T type;
 #ifdef FCPP_CXX11
     typedef decltype(f(*(T*)(0),*(T*)(0))) ResultType;
 #else
     typedef type ResultType;
 #endif
     typedef type Arg1Type;
     typedef type Arg2Type;
   };
 #ifdef FCPP_CXX11
    template <typename T>
   auto operator()( const T&  x, const T& y ) const -> decltype(f(x,y))
   {
     return f(x,y);
   }
 #else
   template <typename T>
     T operator()( const T&  x, const T& y ) const {
     return f(x,y);
   }
 #endif
};
}
template <class Op>
Full2<impl::Xboostph2<Op> > boostph_to_fun2( const Op& o ) {
   return makeFull2( impl::Xboostph2<Op>(o) );
}

namespace impl {
template <class Op>
  class Xboostph3
  //: public CFunType<typename Op::sig::type,
  //  typename Op::sig::type>
  {
   Op f;
public:
   Xboostph3( const Op& o ) : f(o) {}
 #ifdef FCPP_CXX11
   template <class T, class U, class V>
     struct Sig {
     typedef decltype(f(*(T*)(0),*(U*)(0),*(V*)(0))) ResultType;
     typedef T Arg1Type;
     typedef U Arg2Type;
     typedef V Arg3Type;
   };

   template <class T, class U, class V>
   auto operator()
     ( const T& x, const U& y, const V& z ) const -> decltype(f(x,y,z))
   {
     return f(x,y,z);
   }
 #else
   template < typename T, typename U, typename V > struct Sig;
 #endif
  //template <template < typename S> class T >
   // This is only working for three arguments of the same type - for now.
   template <typename T>
     struct Sig<T,T,T> {
     //typedef typename
     //Op::inherited::
     //template sig<typename T::tail_type>::type type;
     // typedef typename boost::tuples::element<1, Op>::type types;
     // typedef typename boost::tuples::element<1, types>::type type;
     typedef T type;
 #ifdef FCPP_CXX11
     typedef decltype(f(*(T*)(0),*(T*)(0),*(T*)(0))) ResultType;
 #else
     typedef type ResultType;
 #endif
     typedef type Arg1Type;
     typedef type Arg2Type;
     typedef type Arg3Type;
   };
   // I am not entirely sure why this works but it does.
   // More investigation may be needed.
 #ifdef FCPP_CXX11
    template <typename T>
    auto operator()( const T&  x, const T& y, const T &z ) const
      -> decltype(f(x,y,z))
   {
     return f(x,y,z);
   }
 #else
   template <typename T>
     T  operator()( const T&  x, const T& y, const T& z ) const {
     return f(x,y,z);
   }
 #endif
};
}
template <class Op>
Full3<impl::Xboostph3<Op> > boostph_to_fun3( const Op& o ) {
   return makeFull3( impl::Xboostph3<Op>(o) );
}

#endif

//////////////////////////////////////////////////////////////////////
// monomorphizing conversions
//////////////////////////////////////////////////////////////////////
// Note that these are template functions, not functoids.  I'm lazy.  

// FC++.1.5.2
#ifdef FCPP152
namespace impl {
  template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Res, class F>
    class XMonomorphicWrapper8 : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Res> {
   F f;
public:
   XMonomorphicWrapper8( const F& g ) : f(g) {}
   Res operator()( const Arg1& x, const Arg2& y, const Arg3& z, const Arg4& t, const Arg5& u, const Arg6& v, const Arg7 & w, const Arg8 & s ) const {
     return f(x,y,z,t,u,v,w,s);
   }
};
}
 template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Res, class F>
   Full8<impl::XMonomorphicWrapper8<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Res,F> >
monomorphize8( const F& f ) {
   return makeFull8(impl::XMonomorphicWrapper8<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Res,F>( f ));
}

namespace impl {
  template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Res, class F>
    class XMonomorphicWrapper7 : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Res> {
   F f;
public:
   XMonomorphicWrapper7( const F& g ) : f(g) {}
     Res operator()( const Arg1& x, const Arg2& y, const Arg3& z, const Arg4& t, const Arg5& u, const Arg6& v, const Arg7 & w ) const {
       return f(x,y,z,t,u,v,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("MonoMorphicWrapper7::")
             +fcpp::traits::debug_traits<F>::name(f);
     else
        return std::string("MonoMorphicWrapper7 ");
   }
#endif
};
}
 template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Res, class F>
   Full7<impl::XMonomorphicWrapper7<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Res,F> > 
monomorphize7( const F& f ) {
   return makeFull7(impl::XMonomorphicWrapper7<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Res,F>( f ));
}

namespace impl {
  template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Res, class F>
    class XMonomorphicWrapper6 : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Res> {
   F f;
public:
   XMonomorphicWrapper6( const F& g ) : f(g) {}
     Res operator()( const Arg1& x, const Arg2& y, const Arg3& z, const Arg4& u, const Arg5& v, const Arg6& w ) const {
       return f(x,y,z,u,v,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("MonoMorphicWrapper6::")
             +fcpp::traits::debug_traits<F>::name(f);
     else
        return std::string("MonoMorphicWrapper6 ");
   }
#endif
};
}
 template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Res, class F>
   Full6<impl::XMonomorphicWrapper6<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Res,F> > 
monomorphize6( const F& f ) {
   return makeFull6(impl::XMonomorphicWrapper6<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Res,F>( f ));
}

namespace impl {
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Res, class F>
class XMonomorphicWrapper5 : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Res> {
   F f;
public:
   XMonomorphicWrapper5( const F& g ) : f(g) {}
   Res operator()( const Arg1& x, const Arg2& y, const Arg3& z, const Arg4& w, const Arg5& v ) const {
      return f(x,y,z,w,v);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("MonoMorphicWrapper5::")
             +fcpp::traits::debug_traits<F>::name(f);
     else
        return std::string("MonoMorphicWrapper5 ");
   }
#endif
};
}
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Res, class F>
Full5<impl::XMonomorphicWrapper5<Arg1,Arg2,Arg3,Arg4,Arg5,Res,F> > 
monomorphize5( const F& f ) {
   return makeFull5(impl::XMonomorphicWrapper5<Arg1,Arg2,Arg3,Arg4,Arg5,Res,F>( f ));
}

namespace impl {
template <class Arg1, class Arg2, class Arg3, class Arg4, class Res, class F>
class XMonomorphicWrapper4 : public CFunType<Arg1,Arg2,Arg3,Arg4,Res> {
   F f;
public:
   XMonomorphicWrapper4( const F& g ) : f(g) {}
   Res operator()( const Arg1& x, const Arg2& y, const Arg3& z, const Arg4& w ) const {
      return f(x,y,z,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("MonoMorphicWrapper4::")
             +fcpp::traits::debug_traits<F>::name(f);
     else
        return std::string("MonoMorphicWrapper4 ");
   }
#endif
};
}
template <class Arg1, class Arg2, class Arg3, class Arg4, class Res, class F>
Full4<impl::XMonomorphicWrapper4<Arg1,Arg2,Arg3,Arg4,Res,F> > 
monomorphize4( const F& f ) {
   return makeFull4(impl::XMonomorphicWrapper4<Arg1,Arg2,Arg3,Arg4,Res,F>( f ));
}

#endif

namespace impl {
template <class Arg1, class Arg2, class Arg3, class Res, class F>
class XMonomorphicWrapper3 : public CFunType<Arg1,Arg2,Arg3,Res> {
   F f;
public:
   XMonomorphicWrapper3( const F& g ) : f(g) {}
   Res operator()( const Arg1& x, const Arg2& y, const Arg3& z ) const {
      return f(x,y,z);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("MonoMorphicWrapper3::")
             +fcpp::traits::debug_traits<F>::name(f);
     else
        return std::string("MonoMorphicWrapper3 ");
   }
#endif
};
}
template <class Arg1, class Arg2, class Arg3, class Res, class F>
Full3<impl::XMonomorphicWrapper3<Arg1,Arg2,Arg3,Res,F> > 
monomorphize3( const F& f ) {
   return makeFull3(impl::XMonomorphicWrapper3<Arg1,Arg2,Arg3,Res,F>( f ));
}

namespace impl {
template <class Arg1, class Arg2, class Res, class F>
class XMonomorphicWrapper2 : public CFunType<Arg1,Arg2,Res> {
   F f;
public:
   XMonomorphicWrapper2( const F& g ) : f(g) {}
   Res operator()( const Arg1& x, const Arg2& y ) const {
      return f(x,y);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
     if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("MonoMorphicWrapper2::")
             +fcpp::traits::debug_traits<F>::name(f);
     else
        return std::string("MonoMorphicWrapper2 ");
   }
#endif
};
}
template <class Arg1, class Arg2, class Res, class F>
Full2<impl::XMonomorphicWrapper2<Arg1,Arg2,Res,F> > 
monomorphize2( const F& f ) {
   return makeFull2(impl::XMonomorphicWrapper2<Arg1,Arg2,Res,F>( f ));
}

namespace impl {
  // This is an attempt to sort out the problem with concepts
  // where it objects to a nonvoid return value.
  // It does not work with concepts.
  // It works with clang 3.6 although not needed.
#ifdef FCPP_USE_BOOST_VOID
  template <class F,class A1,class R,bool test_void> struct Mono1_impl;

  template <class F, class A1, class R>
    struct Mono1_impl<F,A1,R,true>
  {
    static void do_it(const F& f, const A1& a1)
    {  f(a1); }
  };

  template <class F, class A1, class R>
    struct Mono1_impl<F,A1,R,false>
  {
   static R do_it(const F& f, const A1& a1)
   {
     return f(a1);
   }
#ifdef FCPP_ALLOW_REF_ARGUMENT
  static R do_it(const F& f, A1& a1)
   {
     return f(a1);
   }
#endif
   };

#endif

template <class Arg1, class Res, class F>
class XMonomorphicWrapper1 : public CFunType<Arg1,Res> {
   F f;
public:
   XMonomorphicWrapper1( const F& g ) : f(g) {}
   Res operator()( const Arg1& x ) const {
#ifdef FCPP_USE_BOOST_VOID
#ifdef FCPP_PRINT_HELPERS
       std::cout << "MonomorphicWrapper1::do_it(f,x) called" << std::endl;
#endif
       return Mono1_impl<F,Arg1,Res,
       boost::is_void<Res>::value >::do_it(f,x);
#else
      return f(x);
#endif
   }
#ifdef FCPP_ALLOW_REF_ARGUMENT
   //XMonomorphicWrapper1( F& g ) : f(g) {}
   Res operator()( Arg1& x ) {
#ifdef FCPP_USE_BOOST_VOID
#ifdef FCPP_PRINT_HELPERS
       std::cout << "MonomorphicWrapper1::do_it(f,x) called" << std::endl;
#endif
       return Mono1_impl<F,Arg1,Res,
       boost::is_void<Res>::value >::do_it(f,x);
#else
      return f(x);
#endif
   }
#endif
#ifdef FCPP_DEBUG
   std::string name() const {
     if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("MonoMorphicWrapper1::")
             +fcpp::traits::debug_traits<F>::name(f);
     else
        return std::string("MonoMorphicWrapper1");
   }
#endif
};

  /* Attempt to make a functoid - I am not sure how to do it. */
struct MonomorphicWrapper1
{
  template <class Arg1, class Res, class F> struct Sig
    : public FunType<F, Full1<XMonomorphicWrapper1<Arg1,Res,F> > > {};
  template <class Arg1, class Res, class F>
    inline Full1<XMonomorphicWrapper1<Arg1,Res,F> > operator() (const F& f) const {
    return makeFull1( XMonomorphicWrapper1<Arg1,Res,F>( f ) );
  }

};

}


typedef Full1<impl::MonomorphicWrapper1> MonomorphicWrapper1;
//FCPP_MAYBE_EXTERN MonomorphicWrapper1 monomorphize1;
//static MonomorphicWrapper1& monomorphize1() {static MonomorphicWrapper1 f; return f;}


template <class Arg1, class Res, class F>
Full1<impl::XMonomorphicWrapper1<Arg1,Res,F> > monomorphize1( const F& f ) {
   return makeFull1( impl::XMonomorphicWrapper1<Arg1,Res,F>( f ) );
}


namespace impl {

template <class Res, class F>
class XMonomorphicWrapper0 : public CFunType<Res> {
   F f;
public:
   XMonomorphicWrapper0( const F& g ) : f(g) {}
   Res operator()() const {
         return f();
      }
#ifdef FCPP_DEBUG
   std::string name() const {
     if (fcpp::traits::debug_traits<F>::has_name)
        return std::string("MonoMorphicWrapper0::")
             +fcpp::traits::debug_traits<F>::name(f);
     else
        return std::string("MonoMorphicWrapper0");
   }
#endif
   };

  /* Attempt to make a functoid - I am not sure how to do it. */
  /* These do not work as they need to be deployed as template functoids. */
  //template <class Res>
struct MonomorphicWrapper0
{
  template <class Res,class F> struct Sig
    : public FunType<F, Full0<XMonomorphicWrapper0<Res,F> > > {};
  template <class Res,class F>
    Full0<XMonomorphicWrapper0<Res,F> > operator()(const F& f) const {
    return makeFull0( XMonomorphicWrapper0<Res,F>( f ) );
  }
};
  

}

typedef Full1<impl::MonomorphicWrapper0> MonomorphicWrapper0;
//FCPP_MAYBE_EXTERN MonomorphicWrapper0 monomorphize0;
//static MonomorphicWrapper0& m0() {static MonomorphicWrapper0 f; return f;}


template <class Res, class F>
Full0<impl::XMonomorphicWrapper0<Res,F> > monomorphize0( const F& f ) {
   return makeFull0( impl::XMonomorphicWrapper0<Res,F>( f ) );
}


//////////////////////////////////////////////////////////////////////
// ptr_to_fun
//////////////////////////////////////////////////////////////////////
// ptr_to_fun is now a functoid -- hurray!

namespace impl {

template <class Result>
class Xptr_to_nullary_function : public CFunType<Result> {
#ifdef FCPP_DEBUG
  public:
    std::string name() const
    {
       return std::string("ptr_to_nullary_function");
    }
  private:
#endif
    Result (*ptr)();
public:
    explicit Xptr_to_nullary_function(Result (*x)()) : ptr(x) {}
    Result operator()() const { return ptr(); }
};

template <class Arg, class Result>
class Xptr_to_unary_function
#ifndef FCPP_CONCEPTS
  : public CFunType<Arg, Result>
#endif
{
#ifdef FCPP_DEBUG
  public:
    std::string name() const
    {
       return std::string("ptr_to_unary_function");
    }
  private:
#endif
    Result (*ptr)(Arg);
public:
    //#ifdef FCPP_CXX11
    // Experiment does not work yet - taken out of use.
    //explicit Xptr_to_unary_function(Result (*x)(Arg )) : ptr(x) {}
    //Result operator()(Arg&& x) const {
    //#else
    explicit Xptr_to_unary_function(Result (*x)(Arg)) : ptr(x) {}
    Result operator()(Arg x) const {
    //#endif
#ifdef FCPP_PRINT_HELPERS
     std::cout << "ptr_to_unary_function operator() const called with "
               << argument_traits_value(x) << std::endl;
#endif
       //#ifdef FCPP_CXX11
       //return ptr(std::forward<Arg>(x));
       //#else
       return ptr(x);
       //#endif
    }
    //#ifdef FCPP_CUDA_COMPATIBLE
    //Result direct(Arg x) const { return ptr(x); }
    //#endif
};

template <class Arg1, class Arg2, class Result>
class Xptr_to_binary_function : public CFunType<Arg1, Arg2, Result> {
#ifdef FCPP_DEBUG
  public:
    std::string name() const
    {
       return std::string("ptr_to_binary_function");
    }
  private:
#endif
    Result (*ptr)(Arg1, Arg2);
public:
    explicit Xptr_to_binary_function(Result (*x)(Arg1, Arg2)) : ptr(x) {}
    //#ifdef DEVEL
    //inline ConstCallable2<Result,Arg1,Arg2>
    //#else
    Result 
      //#endif
    operator()(Arg1 x, Arg2 y) const { return ptr(x, y); }
};

template <class Arg1, class Arg2, class Arg3, class Result>
class Xptr_to_ternary_function : public CFunType<Arg1, Arg2, Arg3, Result> {
#ifdef FCPP_DEBUG
  public:
    std::string name() const
    {
       return std::string("ptr_to_ternary_function");
    }
  private:
#endif
    Result (*ptr)(Arg1, Arg2, Arg3);
public:
    explicit Xptr_to_ternary_function(Result (*x)(Arg1, Arg2, Arg3)) : ptr(x) {}
    Result operator()(Arg1 x, Arg2 y, Arg3 z) const { return ptr(x,y,z); }
};

// FC++.1.5.2
#ifdef FCPP152
template <class Arg1, class Arg2, class Arg3, class Arg4, class Result>
class Xptr_to_quaternary_function : public CFunType<Arg1, Arg2, Arg3, Arg4, Result> {
#ifdef FCPP_DEBUG
  public:
    std::string name() const
    {
       return std::string("ptr_to_quaternary_function");
    }
  private:
#endif
    Result (*ptr)(Arg1, Arg2, Arg3, Arg4);
public:
    explicit
    Xptr_to_quaternary_function(Result (*x)(Arg1, Arg2, Arg3, Arg4)) : ptr(x) {}
    Result operator()(Arg1 x, Arg2 y, Arg3 z, Arg4 v) const { return ptr(x,y,z,v); }
};

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result>
class Xptr_to_quintinary_function : public CFunType<Arg1, Arg2, Arg3, Arg4, Arg5, Result> {
#ifdef FCPP_DEBUG
  public:
    std::string name() const
    {
       return std::string("ptr_to_quintinary_function");
    }
  private:
#endif
    Result (*ptr)(Arg1, Arg2, Arg3, Arg4, Arg5);
public:
    explicit
    Xptr_to_quintinary_function(Result (*x)(Arg1, Arg2, Arg3, Arg4, Arg5)) : ptr(x) {}
    Result operator()(Arg1 x, Arg2 y, Arg3 z, Arg4 v, Arg5 w) const
    { return ptr(x,y,z,v,w); }
};

 template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result>
   class Xptr_to_sextinary_function : public CFunType<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Result> {
#ifdef FCPP_DEBUG
  public:
    std::string name() const
    {
       return std::string("ptr_to_sextinary_function");
    }
  private:
#endif
    Result (*ptr)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
public:
    explicit
      Xptr_to_sextinary_function(Result (*x)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)) : ptr(x) {}
      Result operator()(Arg1 x, Arg2 y, Arg3 z, Arg4 u, Arg5 v, Arg6 w) const
      { return ptr(x,y,z,u,v,w); }
};

 template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Result>
   class Xptr_to_septimary_function : public CFunType<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Result> {
#ifdef FCPP_DEBUG
  public:
    std::string name() const
    {
       return std::string("ptr_to_septimary_function");
    }
  private:
#endif
    Result (*ptr)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
public:
    explicit
      Xptr_to_septimary_function(Result (*x)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)) : ptr(x) {}
      Result operator()(Arg1 x, Arg2 y, Arg3 z, Arg4 u, Arg5 v, Arg6 w, Arg7 s) const
      { return ptr(x,y,z,u,v,w,s); }
};

  template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Result>
    class Xptr_to_octavary_function : public CFunType<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Result> {
#ifdef FCPP_DEBUG
  public:
    std::string name() const
    {
       return std::string("ptr_to_octavary_function");
    }
  private:
#endif
    Result (*ptr)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
public:
    explicit
      Xptr_to_octavary_function(Result (*x)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)) : ptr(x) {}
    Result operator()(Arg1 x, Arg2 y, Arg3 z, Arg4 u, Arg5 v, Arg6 w, Arg7 s, Arg8 p) const
    { return ptr(x,y,z,u,v,w,s,p); }
};

#endif

//////////////////////////////////////////////////////////////////////
// Turn member functions into normal functions which take a Receiver*
// (or a smart pointer) as their first (extra) argument.  Note that we 
// disallow reference parameters.
//////////////////////////////////////////////////////////////////////
// Further note by John Fletcher
// Reference parameters can be overcome by the use of traits
// to handle the reference to reference problem.
// See the example below - Xptr_to_mem_unary_function
// There are two ways demonstrated - one using Loki traits and
// one using Boost traits.  
// Neither of these has been fully implemented, pending discussion
// about the dependency on other code which this introduces.
// In any case, developments in C++0x may cause this problem to 
// go away.  October 2007
//////////////////////////////////////////////////////////////////////

// FC++.1.5.2
#ifdef FCPP152
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5,
  class Arg6, class Arg7, class Result>
class Xptr_to_mem_sextinary_function {
  Result (Arg1::*ptr)(Arg2,Arg3,Arg4,Arg5,Arg6,Arg7);
public:
    explicit Xptr_to_mem_sextinary_function
      (Result (Arg1::*x)(Arg2,Arg3,Arg4,Arg5,Arg6,Arg7))
       : ptr(x) {}
  // These apparently unused template arguments are needed to set up
  // the correct argument types. They are placeholders.
  template <class P, class X, class Y, class Z, class U, class V, class W>
    struct Sig : public FunType<P,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result> {};
    template <class P>
      Result operator()(const P& p, const Arg2& x, const Arg3& y, const Arg4& z,
                      const Arg5& u, const Arg6& v, const Arg7& w) const
      { return ((*p).*ptr)(x,y,z,u,v,w); }
};

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5,
  class Arg6, class Arg7, class Result>
class Xptr_to_const_mem_sextinary_function {
  Result (Arg1::*ptr)(Arg2,Arg3,Arg4,Arg5,Arg6,Arg7) const;
public:
    explicit Xptr_to_const_mem_sextinary_function
      (Result (Arg1::*x)(Arg2,Arg3,Arg4,Arg5,Arg6,Arg7) const)
       : ptr(x) {}
  // These apparently unused template arguments are needed to set up
  // the correct argument types. They are placeholders.
  template <class P, class X, class Y, class Z, class U, class V, class W>
    struct Sig : public FunType<P,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result> {};
    template <class P>
      Result operator()(const P& p, const Arg2& x, const Arg3& y, const Arg4& z,
                      const Arg5& u, const Arg6& v, const Arg7& w) const
      { return ((*p).*ptr)(x,y,z,u,v,w); }
};

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5,
  class Arg6, class Result>
class Xptr_to_mem_quintinary_function {
  Result (Arg1::*ptr)(Arg2,Arg3,Arg4,Arg5,Arg6);
public:
    explicit Xptr_to_mem_quintinary_function
      (Result (Arg1::*x)(Arg2,Arg3,Arg4,Arg5,Arg6))
       : ptr(x) {}
  // These apparently unused template arguments are needed to set up
  // the correct argument types. They are placeholders.
  template <class P, class Y, class Z, class W,class V,class U>
    struct Sig : public FunType<P,Arg2,Arg3,Arg4,Arg5,Arg6,Result> {};
    template <class P>
    Result operator()(const P& x, const Arg2& y, const Arg3& z,
                      const Arg4& w, const Arg5& v, const Arg6& u) const
      { return ((*x).*ptr)(y,z,w,v,u); }
};

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5,
  class Arg6, class Result>
class Xptr_to_const_mem_quintinary_function {
  Result (Arg1::*ptr)(Arg2,Arg3,Arg4,Arg5,Arg6) const ;
public:
    explicit Xptr_to_const_mem_quintinary_function
      (Result (Arg1::*x)(Arg2,Arg3,Arg4,Arg5,Arg6) const)
       : ptr(x) {}
    template <class P, class Y, class Z, class W,class V, class U>
    struct Sig : public FunType<P,Arg2,Arg3,Arg4,Arg5,Arg6,Result> {};
    template <class P>
    Result operator()(const P& x, const Arg2& y, const Arg3& z,
                      const Arg4& w, const Arg5& v, const Arg6& u) const
      { return ((*x).*ptr)(y,z,w,v,u); }
};

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5,
          class Result>
class Xptr_to_mem_quaternary_function {
    Result (Arg1::*ptr)(Arg2,Arg3,Arg4,Arg5);
public:
    explicit Xptr_to_mem_quaternary_function
          (Result (Arg1::*x)(Arg2,Arg3,Arg4,Arg5))
       : ptr(x) {}
    template <class P, class Y, class Z, class V, class U>
    struct Sig : public FunType<P,Arg2,Arg3,Arg4,Arg5,Result> {};
    template <class P>
    Result operator()(const P& x, const Arg2& y, const Arg3& z,
                      const Arg4& v, const Arg5& u) const
       { return ((*x).*ptr)(y,z,v,u); }
};

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5,
          class Result>
class Xptr_to_const_mem_quaternary_function {
    Result (Arg1::*ptr)(Arg2,Arg3,Arg4,Arg5) const;
public:
    explicit Xptr_to_const_mem_quaternary_function
          (Result (Arg1::*x)(Arg2,Arg3,Arg4,Arg5) const)
       : ptr(x) {}
    template <class P, class Y, class Z, class V, class U>
    struct Sig : public FunType<P,Arg2,Arg3,Arg4,Arg5,Result> {};
    template <class P>
    Result operator()(const P& x, const Arg2& y, const Arg3& z,
                      const Arg4& v, const Arg5& u) const
       { return ((*x).*ptr)(y,z,v,u); }
};

template <class Arg1, class Arg2, class Arg3, class Arg4, class Result>
class Xptr_to_mem_ternary_function {
    Result (Arg1::*ptr)(Arg2,Arg3, Arg4);
public:
    explicit Xptr_to_mem_ternary_function(Result (Arg1::*x)(Arg2,Arg3,Arg4))
       : ptr(x) {}
    template <class P, class Y, class Z, class V>
    struct Sig : public FunType<P,Arg2,Arg3,Arg4,Result> {};
    template <class P>
    Result operator()(const P& x, const Arg2& y, const Arg3& z, const Arg4& v) const
       { return ((*x).*ptr)(y,z,v); }
};

template <class Arg1, class Arg2, class Arg3, class Arg4, class Result>
class Xptr_to_const_mem_ternary_function {
    Result (Arg1::*ptr)(Arg2,Arg3,Arg4) const;
public:
    explicit Xptr_to_const_mem_ternary_function(
          Result (Arg1::*x)(Arg2,Arg3,Arg4) const) : ptr(x) {}
    template <class P, class Y, class Z, class V>
    struct Sig : public FunType<P,Arg2,Arg3,Arg4,Result> {};
    template <class P>
    Result operator()(const P& x, const Arg2& y, const Arg3& z, const Arg4& v) const
       { return ((*x).*ptr)(y,z,v); }
};


#endif

template <class Arg1, class Arg2, class Arg3, class Result>
class Xptr_to_mem_binary_function {
    Result (Arg1::*ptr)(Arg2,Arg3);
public:
    explicit Xptr_to_mem_binary_function(Result (Arg1::*x)(Arg2,Arg3))
       : ptr(x) {}
    template <class P, class Y, class Z>
    struct Sig : public FunType<P,Arg2,Arg3,Result> {};
    //#ifdef FCPP_USE_LOKI_TRAITS
    //typedef typename Loki::TypeTraits<Arg2>::ParameterType A2;
    //typedef typename Loki::TypeTraits<Arg3>::ParameterType A3;
    //template <class P>
    //  Result operator()(const P& x, A2 y, A3 z) const
    //#else
#ifdef FCPP_USE_BOOST_TRAITS
    typedef typename boost::call_traits<Arg2>::param_type A2;
    typedef typename boost::call_traits<Arg3>::param_type A3;
    template <class P>
    Result operator()(const P& x, A2 y, A3 z) const

#else
    template <class P>
    Result operator()(const P& x, const Arg2& y, const Arg3& z) const
#endif
      //#endif
       //{ return (x->*ptr)(y,z); }
       { return ((*x).*ptr)(y,z); }
};

template <class Arg1, class Arg2, class Arg3, class Result>
class Xptr_to_const_mem_binary_function {
    Result (Arg1::*ptr)(Arg2,Arg3) const;
public:
    explicit Xptr_to_const_mem_binary_function(
          Result (Arg1::*x)(Arg2,Arg3) const) : ptr(x) {}
    template <class P, class Y, class Z>
    struct Sig : public FunType<P,Arg2,Arg3,Result> {};
    template <class P>
    Result operator()(const P& x, const Arg2& y, const Arg3& z) const
       //{ return (x->*ptr)(y,z); }
       { return ((*x).*ptr)(y,z); }
};

template <class Arg1, class Arg2, class Result>
class Xptr_to_mem_unary_function {
    Result (Arg1::*ptr)(Arg2);
public:
    explicit Xptr_to_mem_unary_function(Result (Arg1::*x)(Arg2)) : ptr(x) {}
    template <class P, class Y> 
    struct Sig : public FunType<P,Arg2,Result> {};
    // This is the one I have messed about with to demonstrate one 
    // solution to the problem using Loki::TypeTraits2 
    // to take Arg and give the type A2 to use for the type of the 
    // argument in the operator().
    // Make the use of Loki conditional.
    // It looks as though boost/call_traits.hpp will offer
    // an exact equivalent to this.
    //#ifdef FCPP_USE_LOKI_TRAITS
    //typedef typename Loki::TypeTraits<Arg2>::ParameterType A2;
    //template <class P>
    //Result operator()(const P& x, A2 y) const
    //#else
#ifdef FCPP_USE_BOOST_TRAITS
    typedef typename boost::call_traits<Arg2>::param_type A2;
    template <class P>
    Result operator()(const P& x, A2 y) const
#else
#ifdef SPECIAL_VERSION
    template <class P>
    Result operator()(const P& x,  Arg2 y)
#else
    // original version
    template <class P>
    Result operator()(const P& x, const Arg2& y) const
#endif
#endif
    //#endif
    // next try.
    // Result operator()(const P& x, Arg2 y) const
    //{ return (x->*ptr)(y); }
      { // fstd::cout << "Used special ";
	// The above line was used to check it was working.
         return ((*x).*ptr)(y); }
};

template <class Arg1, class Arg2, class Result>
class Xptr_to_const_mem_unary_function {
    Result (Arg1::*ptr)(Arg2) const;
public:
    explicit Xptr_to_const_mem_unary_function(Result (Arg1::*x)(Arg2) const)
       : ptr(x) {}
    template <class P, class Y>
    struct Sig : public FunType<P,Arg2,Result> {};
    //#ifdef FCPP_USE_LOKI_TRAITS
    //typedef typename Loki::TypeTraits<Arg2>::ParameterType A2;
    //template <class P>
    //Result operator()(const P& x, A2 y) const
    //#else
#ifdef FCPP_USE_BOOST_TRAITS
    typedef typename boost::call_traits<Arg2>::param_type A2;
    template <class P>
    Result operator()(const P& x, A2 y) const
#else
   template <class P>
    Result operator()(const P& x, const Arg2& y) const
#endif
    //#endif
     //{ return (x->*ptr)(y); }
       { return ((*x).*ptr)(y); }
};

template <class Arg1, class Result>
class Xptr_to_mem_nullary_function {
    Result (Arg1::*ptr)();
public:
    explicit Xptr_to_mem_nullary_function(Result (Arg1::*x)()) : ptr(x) {}
    template <class P>
    struct Sig : public FunType<P,Result> {};
   template <class P>
    //Result operator()(const P& x) const { return (x->*ptr)(); }
    Result operator()(const P& x) const { return ((*x).*ptr)(); }
};

template <class Arg1, class Result>
class Xptr_to_const_mem_nullary_function {
    Result (Arg1::*ptr)() const;
public:
    explicit Xptr_to_const_mem_nullary_function(Result (Arg1::*x)() const) 
       : ptr(x) {}
    template <class P> 
    struct Sig : public FunType<P,Result> {};
    template <class P>
    //Result operator()(const P& x) const { return (x->*ptr)(); }
    Result operator()(const P& x) const { return ((*x).*ptr)(); }
};

#ifdef WASDEVEL
// This was part of trying to solve a problem but not needed now.
}

// These compile but do not solve the problem....
template <class A1,class Result>
  concept_map ConstCallable1<Result (*)(A1), A1>
{
  typedef Full1<impl::Xptr_to_unary_function<A1,Result> > result_type;
};

 template <class A1,class A2,class Result>
   concept_map ConstCallable2<Result (*)(A1,A2), A1,A2>
{
  typedef Result result_type;
};


namespace impl {
#endif

struct XPtrToFun {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("ptr_to_fun ");
   }
 #endif
   template <class P> struct Sig;

   // This still has Sig code - not touched yet by concepts.
   // It works with the boostcon concepts compiler but it now 
   // broken with the development version.
   // Trying taking out a few sigs to see if this breaks something...
   // All sigs taken out when using concepts. fcpptest6.cpp 
   // still compiles and runs.

   // non-member functions
#ifndef FCPP_CONCEPTS
   template <class Result>
   struct Sig< Result (*)() > : public FunType< Result (*)(), 
      Full0<Xptr_to_nullary_function<Result> > > {};
   template <class A1, class Result>
   struct Sig< Result (*)(A1) > : public FunType< Result (*)(A1), 
      Full1<Xptr_to_unary_function<A1,Result> > > {};
   template <class A1, class A2, class Result>
   struct Sig< Result (*)(A1,A2) > : public FunType< Result (*)(A1,A2), 
      Full2<Xptr_to_binary_function<A1,A2,Result> > > {};
   template <class A1, class A2, class A3, class Result>
   struct Sig< Result (*)(A1,A2,A3) > : public FunType< Result (*)(A1,A2,A3), 
      Full3<Xptr_to_ternary_function<A1,A2,A3,Result> > > {};
#endif
// FC++.1.5.2
#ifdef FCPP152
#ifndef FCPP_CONCEPTS
   template <class A1, class A2, class A3, class A4, class Result>
   struct Sig< Result (*)(A1,A2,A3,A4) > : public FunType< Result (*)(A1,A2,A3,A4),
      Full4<Xptr_to_quaternary_function<A1,A2,A3,A4,Result> > > {};
   template <class A1, class A2, class A3, class A4, class A5, class Result>
   struct Sig< Result (*)(A1,A2,A3,A4,A5) > : public FunType< Result (*)(A1,A2,A3,A4,A5),
      Full5<Xptr_to_quintinary_function<A1,A2,A3,A4,A5,Result> > > {};
  template <class A1, class A2, class A3, class A4, class A5, class A6, class Result>
    struct Sig< Result (*)(A1,A2,A3,A4,A5,A6) > : public FunType< Result (*)(A1,A2,A3,A4,A5,A6),
    Full6<Xptr_to_sextinary_function<A1,A2,A3,A4,A5,A6,Result> > > {};
  template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class Result>
    struct Sig< Result (*)(A1,A2,A3,A4,A5,A6,A7) > : public FunType< Result (*)(A1,A2,A3,A4,A5,A6,A7),
    Full7<Xptr_to_septimary_function<A1,A2,A3,A4,A5,A6,A7,Result> > > {};
  template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class Result>
    struct Sig< Result (*)(A1,A2,A3,A4,A5,A6,A7,A8) > : public FunType< Result (*)(A1,A2,A3,A4,A5,A6,A7,A8),
    Full8<Xptr_to_octavary_function<A1,A2,A3,A4,A5,A6,A7,A8,Result> > > {};
#endif
#endif
   // member functions
// FC++.1.5.2
#ifndef FCPP_CONCEPTS
#ifdef FCPP152
  template <class A1, class A2, class A3, class A4, class A5, class A6,
    class A7,class Result>
    struct Sig< Result (A1::*)(A2,A3,A4,A5,A6,A7) > :
    public FunType< Result (A1::*)(A2,A3,A4,A5,A6,A7),
    Full7<Xptr_to_mem_sextinary_function<A1, A2, A3, A4, A5, A6, A7, Result> > >
          {};
  template <class A1, class A2, class A3, class A4, class A5, class A6,
    class A7,class Result>
    struct Sig< Result (A1::*)(A2,A3,A4,A5,A6,A7) const > :
    public FunType< Result (A1::*)(A2,A3,A4,A5,A6,A7) const ,
    Full7<Xptr_to_const_mem_sextinary_function<A1, A2, A3, A4, A5, A6, A7, Result> > >
          {};
   template <class A1, class A2, class A3, class A4, class A5, class A6,
            class Result>
    struct Sig< Result (A1::*)(A2,A3,A4,A5,A6) > :
    public FunType< Result (A1::*)(A2,A3,A4,A5,A6),
    Full6<Xptr_to_mem_quintinary_function<A1, A2, A3, A4, A5, A6, Result> > >
          {};
    template <class A1, class A2, class A3, class A4, class A5, class A6,
            class Result>
     struct Sig< Result (A1::*)(A2,A3,A4,A5,A6) const > :
     public FunType< Result (A1::*)(A2,A3,A4,A5,A6) const ,
     Full6<Xptr_to_const_mem_quintinary_function<A1, A2, A3, A4, A5, A6, Result> > > {};

  template <class A1, class A2, class A3, class A4, class A5, class Result>
    struct Sig< Result (A1::*)(A2,A3,A4,A5) > : public FunType< Result (A1::*)(A2,A3,A4,A5) ,
      Full5<Xptr_to_mem_quaternary_function<A1, A2, A3, A4, A5, Result> > > {};
   template <class A1, class A2, class A3, class A4, class A5, class Result>
     struct Sig< Result (A1::*)(A2,A3,A4,A5) const > : public FunType< Result (A1::*)(A2,A3,A4,A5) const,
      Full5<Xptr_to_const_mem_quaternary_function<A1, A2, A3, A4, A5, Result> > > {};
   template <class A1, class A2, class A3, class A4, class Result>
     struct Sig< Result (A1::*)(A2,A3,A4) > : public FunType<Result (A1::*)(A2,A3,A4),
      Full4<Xptr_to_mem_ternary_function<A1, A2, A3, A4, Result> > > {};
   template <class A1, class A2, class A3, class A4, class Result>
     struct Sig< Result (A1::*)(A2,A3,A4) const > : public FunType< Result (A1::*)(A2,A3,A4) const,
      Full4<Xptr_to_const_mem_ternary_function<A1, A2, A3, A4, Result> > > {};
#endif
   template <class A1, class A2, class A3, class Result>
   struct Sig< Result (A1::*)(A2,A3) > : public FunType<
      Result (A1::*)(A2,A3),
      Full3<Xptr_to_mem_binary_function<A1, A2, A3, Result> > > {};
   template <class A1, class A2, class A3, class Result>
   struct Sig< Result (A1::*)(A2,A3) const > : public FunType<
      Result (A1::*)(A2,A3) const,
      Full3<Xptr_to_const_mem_binary_function<A1, A2, A3, Result> > > {};
   template <class A1, class A2, class Result>
   struct Sig< Result (A1::*)(A2) > : public FunType<
      Result (A1::*)(A2),
      Full2<Xptr_to_mem_unary_function<A1, A2, Result> > > {};
   template <class A1, class A2, class Result>
   struct Sig< Result (A1::*)(A2) const > : public FunType<
      Result (A1::*)(A2) const,
      Full2<Xptr_to_const_mem_unary_function<A1, A2, Result> > > {};
   template <class A1, class Result>
   struct Sig< Result (A1::*)() > : public FunType<
      Result (A1::*)(),
      Full1<Xptr_to_mem_nullary_function<A1, Result> > > {};
   template <class A1, class Result>
   struct Sig< Result (A1::*)() const > : public FunType<
      Result (A1::*)() const,
      Full1<Xptr_to_const_mem_nullary_function<A1, Result> > > {};
#endif

   // non-member functions
   template <class Result>
   inline Full0<Xptr_to_nullary_function<Result> >
   operator()(Result (*x)()) const {
     return makeFull0( Xptr_to_nullary_function<Result>(x) );
   }
   template <class A, class Result>
   inline Full1<Xptr_to_unary_function<A, Result> >
   operator()(Result (*x)(A)) const {
     return makeFull1( Xptr_to_unary_function<A, Result>(x) );
   }
   template <class A1, class A2, class Result>
   inline Full2<Xptr_to_binary_function<A1, A2, Result> >
   operator()(Result (*x)(A1, A2)) const {
     return makeFull2( Xptr_to_binary_function<A1, A2, Result>(x) );
   }
   template <class A1, class A2, class A3, class Result>
   inline Full3<Xptr_to_ternary_function<A1, A2, A3, Result> >
   operator()(Result (*x)(A1, A2, A3)) const {
     return makeFull3( Xptr_to_ternary_function<A1,A2,A3,Result>(x) );
   }
// FC++.1.5.2
#ifdef FCPP152
   template <class A1, class A2, class A3, class A4, class Result>
   inline Full4<Xptr_to_quaternary_function<A1, A2, A3, A4, Result> >
   operator()(Result (*x)(A1, A2, A3, A4)) const {
     return makeFull4( Xptr_to_quaternary_function<A1,A2,A3,A4,Result>(x) );
   }
   template <class A1, class A2, class A3, class A4, class A5, class Result>
   inline Full5<Xptr_to_quintinary_function<A1, A2, A3, A4, A5, Result> >
   operator()(Result (*x)(A1, A2, A3, A4, A5)) const {
     return makeFull5( Xptr_to_quintinary_function<A1,A2,A3,A4,A5,Result>(x) );
   }
  template <class A1, class A2, class A3, class A4, class A5, class A6, class Result>
     inline Full6<Xptr_to_sextinary_function<A1, A2, A3, A4, A5, A6, Result> >
     operator()(Result (*x)(A1, A2, A3, A4, A5, A6)) const {
    return makeFull6( Xptr_to_sextinary_function<A1,A2,A3,A4,A5,A6,Result>(x) );
   }
  template <class A1, class A2, class A3, class A4, class A5, class A6, class A7,class Result>
    inline Full7<Xptr_to_septimary_function<A1, A2, A3, A4, A5, A6, A7, Result> >
    operator()(Result (*x)(A1, A2, A3, A4, A5, A6, A7)) const {
    return makeFull7( Xptr_to_septimary_function<A1,A2,A3,A4,A5,A6,A7,Result>(x) );
   }
  template <class A1, class A2, class A3, class A4, class A5, class A6, class A7,class A8,class Result>
    inline Full8<Xptr_to_octavary_function<A1, A2, A3, A4, A5, A6, A7, A8, Result> >
    operator()(Result (*x)(A1, A2, A3, A4, A5, A6, A7, A8)) const {
    return makeFull8( Xptr_to_octavary_function<A1,A2,A3,A4,A5,A6,A7,A8,Result>(x) );
   }
#endif
   // member functions
// FC++.1.5.2
#ifdef FCPP152
  template <class A1, class A2, class A3, class A4, class A5, class A6,
    class A7,class Result>
    inline Full7<Xptr_to_mem_sextinary_function<A1, A2, A3, A4, A5, A6, A7, Result> >
    operator()(Result (A1::*x)(A2,A3,A4,A5,A6,A7)) const {
     return makeFull7(
     Xptr_to_mem_sextinary_function<A1, A2, A3, A4, A5, A6, A7, Result>(x) );
   }
  template <class A1, class A2, class A3, class A4, class A5, class A6,
    class A7,class Result>
    inline Full7<Xptr_to_const_mem_sextinary_function<A1, A2, A3, A4, A5, A6, A7, Result> >
    operator()(Result (A1::*x)(A2,A3,A4,A5,A6,A7) const) const {
     return makeFull7(
     Xptr_to_const_mem_sextinary_function<A1, A2, A3, A4, A5, A6, A7, Result>(x) );
   }
  template <class A1, class A2, class A3, class A4, class A5, class A6,
            class Result>
    inline Full6<Xptr_to_mem_quintinary_function<A1, A2, A3, A4, A5, A6, Result> >
    operator()(Result (A1::*x)(A2,A3,A4,A5,A6)) const {
     return makeFull6(
     Xptr_to_mem_quintinary_function<A1, A2, A3, A4, A5, A6, Result>(x) );
   }
  template <class A1, class A2, class A3, class A4, class A5, class A6,
            class Result>
    inline Full6<Xptr_to_const_mem_quintinary_function<A1, A2, A3, A4, A5, A6, Result> >
    operator()(Result (A1::*x)(A2,A3,A4,A5,A6) const) const {
     return makeFull6(
		      Xptr_to_const_mem_quintinary_function<A1, A2, A3, A4, A5, A6, Result>(x) );
   }
   template <class A1, class A2, class A3, class A4, class A5, class Result>
   inline Full5<Xptr_to_mem_quaternary_function<A1, A2, A3, A4, A5, Result> >
   operator()(Result (A1::*x)(A2,A3,A4,A5)) const {
     return makeFull5(
               Xptr_to_mem_quaternary_function<A1, A2, A3, A4, A5, Result>(x) );
   }
   template <class A1, class A2, class A3, class A4, class A5, class Result>
   inline Full5<Xptr_to_const_mem_quaternary_function<A1, A2, A3, A4, A5, Result> >
   operator()(Result (A1::*x)(A2,A3,A4,A5) const) const {
     return makeFull5(
               Xptr_to_const_mem_quaternary_function<A1, A2, A3, A4, A5, Result>(x) );
   }
   template <class A1, class A2, class A3, class A4, class Result>
   inline Full4<Xptr_to_mem_ternary_function<A1, A2, A3, A4, Result> >
   operator()(Result (A1::*x)(A2,A3, A4)) const {
     return makeFull4(
               Xptr_to_mem_ternary_function<A1, A2, A3, A4, Result>(x) );
   }
   template <class A1, class A2, class A3, class A4, class Result>
   inline Full4<Xptr_to_const_mem_ternary_function<A1, A2, A3, A4, Result> >
   operator()(Result (A1::*x)(A2,A3,A4) const) const {
     return makeFull4(
        Xptr_to_const_mem_ternary_function<A1, A2, A3, A4, Result>(x) );
   }
#endif
   template <class A1, class A2, class A3, class Result>
   inline Full3<Xptr_to_mem_binary_function<A1, A2, A3, Result> >
   operator()(Result (A1::*x)(A2,A3)) const {
     return makeFull3(
               Xptr_to_mem_binary_function<A1, A2, A3, Result>(x) );
   }
   template <class A1, class A2, class A3, class Result>
   inline Full3<Xptr_to_const_mem_binary_function<A1, A2, A3, Result> >
   operator()(Result (A1::*x)(A2,A3) const) const {
     return makeFull3(
        Xptr_to_const_mem_binary_function<A1, A2, A3, Result>(x) );
   }
   template <class A1, class A2, class Result>
   inline Full2<Xptr_to_mem_unary_function<A1, A2, Result> >
   operator()(Result (A1::*x)(A2)) const {
     return makeFull2( Xptr_to_mem_unary_function<A1, A2, Result>(x) );
   }
   template <class A1, class A2, class Result>
   inline Full2<Xptr_to_const_mem_unary_function<A1, A2, Result> >
   operator()(Result (A1::*x)(A2) const) const {
     return makeFull2( 
       Xptr_to_const_mem_unary_function<A1, A2, Result>(x) );
   }
   template <class A1, class Result>
   inline Full1<Xptr_to_mem_nullary_function<A1, Result> >
   operator()(Result (A1::*x)()) const {
     return makeFull1( Xptr_to_mem_nullary_function<A1, Result>(x) );
   }
   template <class A1, class Result>
   inline Full1<Xptr_to_const_mem_nullary_function<A1, Result> >
   operator()(Result (A1::*x)() const) const {
     return makeFull1( Xptr_to_const_mem_nullary_function<A1, Result>(x) );
   }
};
   
}  // end namespace impl
typedef Full1<impl::XPtrToFun> PtrToFun;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN PtrToFun ptr_to_fun;
FCPP_MAYBE_NAMESPACE_CLOSE

 #ifdef FCPP_DEBUG
    namespace traits {

  template <>
  class debug_traits<impl::XGreater>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XGreater &f) { return f.name(); }
      };

  template <>
  class debug_traits<impl::XGreaterEqual>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XGreaterEqual &f) { return f.name(); }
      };

  template <>
  class debug_traits<impl::XLess>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XLess &f) { return f.name(); }
      };

  template <>
  class debug_traits<impl::XLessEqual>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XLessEqual &f) { return f.name(); }
      };

  template <>
  class debug_traits<impl::XLogicalAnd>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XLogicalAnd &f) { return f.name(); }
      };

  template <>
  class debug_traits<impl::XLogicalOr>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XLogicalOr &f) { return f.name(); }
      };

  template <>
  class debug_traits<impl::XLogicalNot>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XLogicalNot &f) { return f.name(); }
      };

  template <>
  class debug_traits<impl::XDereference>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XDereference &f) { return f.name(); }
      };

  template <>
  class debug_traits<impl::XAddressOf>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XAddressOf &f) { return f.name(); }
      };

  template <>
  class debug_traits<impl::XDelete_>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XDelete_ &f) { return f.name(); }
      };

  template <class D>
  class debug_traits<impl::XDynamicCast<D> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XDynamicCast<D> &f) { return f.name(); }
      };

  template <>
  class debug_traits<impl::XOutStream>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XOutStream &f) { return f.name(); }
      };

  template <>
  class debug_traits<impl::XInStream>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XInStream &f) { return f.name(); }
      };

  template <class C, class T>
    class debug_traits<ManipMaker<C,T> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const ManipMaker<C,T> &f) { return f.name(); }
      };

  template<class Result, class F>
    class debug_traits<impl::XMonomorphicWrapper0<Result,F> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::XMonomorphicWrapper0<Result,F> &f)
        { return f.name(); }
       };

  template<class Arg1, class Result, class F>
    class debug_traits<impl::XMonomorphicWrapper1<Arg1,Result,F> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::XMonomorphicWrapper1<Arg1,Result,F> &f)
        { return f.name(); }
       };

  template<class Arg1, class Arg2, class Result, class F>
    class debug_traits<impl::XMonomorphicWrapper2<Arg1,Arg2,Result,F> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::XMonomorphicWrapper2<Arg1,Arg2,Result,F> &f)
        { return f.name(); }
       };

  template<class Arg1, class Arg2, class Arg3, class Result, class F>
    class debug_traits<impl::XMonomorphicWrapper3<Arg1,Arg2,Arg3,Result,F> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::XMonomorphicWrapper3<Arg1,Arg2,Arg3,Result,F> &f)
        { return f.name(); }
       };
 
#ifdef FCPP152
 template<class Arg1, class Arg2, class Arg3, class Arg4, class Result, class F>
 class debug_traits<impl::XMonomorphicWrapper4<Arg1,Arg2,Arg3,Arg4,Result,F> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::XMonomorphicWrapper4<Arg1,Arg2,Arg3,Arg4,Result,F> &f)
        { return f.name(); }
       };

 template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result, class F>
   class debug_traits<impl::XMonomorphicWrapper5<Arg1,Arg2,Arg3,Arg4,Arg5,Result,F> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::XMonomorphicWrapper5<Arg1,Arg2,Arg3,Arg4,Arg5,Result,F> &f)
        { return f.name(); }
       };

 template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6,class Result, class F>
   class debug_traits<impl::XMonomorphicWrapper6<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result,F> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::XMonomorphicWrapper6<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result,F> &f)
        { return f.name(); }
       };

 template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6,class Arg7, class Result, class F>
   class debug_traits<impl::XMonomorphicWrapper7<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result,F> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::XMonomorphicWrapper7<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result,F> &f)
        { return f.name(); }
       };

#endif

    template<class Result>
       class debug_traits<impl::Xptr_to_nullary_function<Result> >
       {
       public:
	static const bool has_name = true;
	static std::string name
            (const impl::Xptr_to_nullary_function<Result> &f)
        { return f.name(); }
       };

    template<class Arg1, class Result>
       class debug_traits<impl::Xptr_to_unary_function<Arg1,Result> >
       {
       public:
	static const bool has_name = true;
	static std::string name
            (const impl::Xptr_to_unary_function<Arg1,Result> &f)
        { return f.name(); }
       };

    template<class Arg1, class Arg2, class Result>
      class debug_traits<impl::Xptr_to_binary_function<Arg1,Arg2,Result> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::Xptr_to_binary_function<Arg1,Arg2,Result> &f)
        { return f.name(); }
       };
 
    template<class Arg1, class Arg2, class Arg3, class Result>
      class debug_traits<impl::Xptr_to_ternary_function<Arg1,Arg2,Arg3,Result> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::Xptr_to_ternary_function<Arg1,Arg2,Arg3,Result> &f)
        { return f.name(); }
       };

// FC++.1.5.2
#ifdef FCPP152
    template<class Arg1, class Arg2, class Arg3, class Arg4, class Result>
      class debug_traits<impl::Xptr_to_quaternary_function<Arg1,Arg2,Arg3,Arg4,Result> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::Xptr_to_quaternary_function<Arg1,Arg2,Arg3,Arg4,Result> &f)
        { return f.name(); }
       };

    template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Result>
      class debug_traits<impl::Xptr_to_quintinary_function<Arg1,Arg2,Arg3,Arg4,Arg5,Result> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::Xptr_to_quintinary_function<Arg1,Arg2,Arg3,Arg4,Arg5,Result> &f)
        { return f.name(); }
       };

    template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result>
      class debug_traits<impl::Xptr_to_sextinary_function<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::Xptr_to_sextinary_function<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result> &f)
        { return f.name(); }
       };

    template<class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Result>
      class debug_traits<impl::Xptr_to_septimary_function<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result> >
       {
       public:
	static const bool has_name = true;
	static std::string name
	  (const impl::Xptr_to_septimary_function<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result> &f)
        { return f.name(); }
       };

#endif
 
    template<>
       class debug_traits<impl::XPtrToFun>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XPtrToFun &f) { return f.name(); }
       };
 
    }
 #endif


//////////////////////////////////////////////////////////////////////
//  funify moved to funify.h and called after function
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Misc stuff
//////////////////////////////////////////////////////////////////////
// FIX THIS: add dec?  pre/post versions?
// synonyms added - inc can be succ
//                  dec can be prev
// as these are preferred in a world which allows side effects.
// Are there any others which imply a side effect which is not there?
// John Fletcher June 2007.
//////////////////////////////////////////////////////////////////////

namespace impl {
struct XInc {
#ifdef FCPP_DEBUG
    std::string name() const
    {
// Space inserted after name as this is often the end of a chain.
       return std::string("Inc");
    }
#endif

    /* See Note 17 at the start of full.h */
#ifndef FCPP_CONCEPTS
    template <class T>
    struct Sig : public FunType<T,T> {};
#endif

    template <class T>
    T operator()(const T& x) const 
    { 
#ifdef FCPP_PRINT_HELPERS
     std::cout << "Inc called " << std::endl;
#endif
      T y = x; return ++y;
    }

};

struct XDec {
#ifdef FCPP_DEBUG
    std::string name() const
    {
// Space inserted after name as this is often the end of a chain.
       return std::string("Dec");
    }
#endif
    template <class T>
    struct Sig : public FunType<T,T> {};

    template <class T>
    T operator()(const T& x) const { T y = x; return --y; }
};
}
typedef Full1<impl::XInc> Inc;
typedef Full1<impl::XDec> Dec;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Inc inc, succ;
FCPP_MAYBE_EXTERN Dec dec, prev;
FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XInc>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XInc &f) { return f.name(); }
       };
     template<>
       class debug_traits<impl::XDec>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XDec &f) { return f.name(); }
       };
   }
#endif


// These were considered obsolete;
// ignore(const_(true)) and ignore(const_(false))
// do the same thing.  Hurray for combinators!
// Imported back from the Boost version of FC++
// konst(true)(x)  -> true
// konst(false)(x) -> false
// These provide equivalents.
// I have also provided alternate names of yes and no.
// See examples in maxandmin.cpp
namespace impl {
struct XAlways1 {
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Always1");
    }
#endif
    template <class T>
    struct Sig : public FunType<T,bool> {};

    template <class T>
    bool operator()(const T&) const { return true; }
};
}
typedef Full1<impl::XAlways1> Always1;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Always1 always1, yes;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XNever1 {
#ifdef FCPP_DEBUG
    std::string name() const
    {
       return std::string("Never1");
    }
#endif
    template <class T>
    struct Sig : public FunType<T,bool> {};

    template <class T>
    bool operator()(const T&) const { return false; }
};
}
typedef Full1<impl::XNever1> Never1;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Never1 never1, no;
FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XAlways1>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XAlways1 &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XNever1>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNever1 &f) { return f.name(); }
       };
    }
 #endif


/* This is not usable in this form as the Full structure passes const refs
   to its arguments.
namespace impl {
struct XSwap {

   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,void> {};

    template <class T>
    void operator()(T& t1, T& t2)
  {
    if (t1 == t2) return;
    T temp = t1;
    t1 = t2;
    t2 = temp;
  }
};
}
typedef Full2<impl::XSwap> Swap;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Swap swap;
FCPP_MAYBE_NAMESPACE_CLOSE
*/

} // end namespace fcpp

#endif
