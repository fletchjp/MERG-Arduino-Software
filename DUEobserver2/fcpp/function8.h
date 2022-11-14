//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FUNCTION8_DOT_H
#define FCPP_FUNCTION8_DOT_H

//////////////////////////////////////////////////////////////////////
// Fun8 implementation (under development). See function.h.
//////////////////////////////////////////////////////////////////////

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Result>
class Fun8Impl;

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Result>
struct Fun8;

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class A8d, class Rd,
  class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class A8s, class Rs>
  Fun8Impl<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd>*
  convert8( const IRef<const Fun8Impl<A1s,A2s,A3s,A4s,A5s,A6s,A7s,A8s,Rs> >& f);

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class A8d, class Rd, class DF>
struct Fun8Constructor;

// Special function for the default constructor to call.
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Result>
  Result nullfun8(Arg1 a, Arg2 b, Arg3 c, Arg4 d, Arg5 e,Arg6 f, Arg7 g, Arg8 h) { return Result(); }

// The "Guts" class helps us implement currying; Fun8 floats gently atop
// Fun8Guts and adds currying.
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Result>
  class Fun8Guts : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result> {
    typedef IRef<const Fun8Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result> > RefImpl;
   RefImpl ref;

   template <class A, class B, class C, class D, class E, class F, class G, class H, class I> friend class Fun8;

   template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class A8d, class Rd, class DF>
   friend struct Fun8Constructor;

   template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class A8d, class Rd,
     class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class A8s, class Rs>
     friend Fun8<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd>
     explicit_convert8( const Fun8<A1s,A2s,A3s,A4s,A5s,A6s,A7s,A8s,Rs>& f );

public:
   typedef Fun8Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>* Impl;

   Fun8Guts( int, Impl i ) : ref(i) {}
   Result operator()( const Arg1& x, const Arg2& y, const Arg3& z, const Arg4& t, const Arg5& u ,const Arg6& v, const Arg7& w, const Arg8& s) const
   { return ref->operator()(x,y,z,t,u,v,w,s); }


   template <class DF>
   Fun8Guts( const DF& df )
     : ref( Fun8Constructor<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result,DF>::make(df) ) {}

   Fun8Guts( const Fun8Guts& x ) : ref(x.ref) {}
   Fun8Guts& operator=( const Fun8Guts& x ) { ref = x.ref; return *this; }

   Fun8Guts() {
     Fun8Guts tmp =
       ptr_to_fun( &nullfun8<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>);
     ref = tmp.ref;
   }

};

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Result>
  class Fun8 :  public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>, public SmartFunctoid8 {
    template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class A8d, class Rd, class DF>
   friend struct Fun8Constructor;

    template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class A8d, class Rd,
      class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class A8s, class Rs>
      friend Fun8<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd>
      explicit_convert8( const Fun8<A1s,A2s,A3s,A4s,A5s,A6s,A7s,A8s,Rs>& f );

    Fun8Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result> rep;
 public:
   typedef Fun7Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>* Impl;

   Fun8() {}
   Fun8( int, Impl i ) : rep(1,i) {}

   template <class DF>
   Fun8( const DF& df ) : rep(df) {}

   Fun8( const Fun8& x ) : rep(x.rep) {}
   Fun8& operator=( const Fun8& x ) { rep = x.rep; return *this; }

#ifndef FCPP_CONCEPTS
   // Note that this pulls in Sig from Full8 so implementation can be done
   // once there.
   // The usage neatly uses voids to be able to handle all cases.
   typedef Full8<Fun8Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result> > SigHelp;
   template <class A, class B=void, class C=void, class D=void, class E=void, class F=void, class G=void, class H=void>
     struct Sig : public SigHelp::template Sig<A,B,C,D,E,F,G,H> {};
#endif


   template <class A, class B, class C, class D, class E, class F, class G, class H>
#ifdef FCPP_CONCEPTS
     inline ConstCallable8<Fun8Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>,A,B,C,D,E,F,G,H >::result_type
     operator()( const A& x, const B& y, const C& z, const D& t, const E& u, const F& v, const G& w, const H& s ) const
      {
	// std::cout << "ConstCallable8 used in Fun8" << std::endl;
 	// Use helper to cover the cases with autocurrying.
     return impl::Curryable8Helper<typename ConstCallable8<
       Fun8Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>,A,B,C,D,E,F,G,H>::result_type,
       Fun8Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>,A,B,C,D,E,F,G,H>::go(rep,x,y,z,t,u,v,w,s);
      }
#else
   typename Sig<A,B,C,D,E,F,G,H>::ResultType
     operator()( const A& x, const B& y, const C& z, const D& t, const E& u, const F& v, const G& w, const H& s ) const
     { return makeFull8(rep)(x,y,z,t,u,v,w,s); }
#endif

  template <class A, class B, class C, class D>
#ifdef FCPP_CONCEPTS
    inline ConstCallable8<Fun8Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>,A,B,C,D >::result_type
     operator()( const A& a, const B& b, const C& c,const D& d ) const
      {
	// Use helper to cover the cases with autocurrying.
	return impl::Curryable8Helper5<typename ConstCallable8<
	Fun8Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>,A,B,C,D>::result_type,
       Fun8Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>,A,B,C,D>::go(rep,a,b,c,d);

	// return ::fcpp::makeFull8(rep)(a,b,c,d);
      }
#else
   typename Sig<A,B,C,D>::ResultType
   operator()( const A& a, const B& b, const C& c,const D& d ) const
     {
       return ::fcpp::makeFull8(rep)(a,b,c,d);
     }
#endif

   template <class A, class B, class C>
#ifdef FCPP_CONCEPTS
     inline ConstCallable8<Fun8Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>,A,B,C >::result_type
   operator()( const A& x, const B& y, const C& z ) const
     {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "fun8 calls makefull8(rep)(x,y,z)" << std::endl;
#endif
      return ::fcpp::makeFull8(rep)(x,y,z);
     }
#else
   typename Sig<A,B,C>::ResultType
   operator()( const A& x, const B& y, const C& z ) const
     {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "fun8 calls makefull8(rep)(x,y,z)" << std::endl;
#endif
       return ::fcpp::makeFull8(rep)(x,y,z);
     }
#endif

   template <class A, class B>
#ifdef FCPP_CONCEPTS
     inline ConstCallable8<Fun8Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>,A,B >::result_type
   operator()( const A& x, const B& y ) const
      {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "fun8 calls makefull8(rep)(x,y)" << std::endl;
#endif
        return ::fcpp::makeFull8(rep)(x,y);
      }
#else
   typename Sig<A,B>::ResultType
   operator()( const A& x, const B& y ) const
     {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "fun8 calls makefull8(rep)(x,y)" << std::endl;
 #endif
        return ::fcpp::makeFull8(rep)(x,y);
     }
#endif

#ifdef FCPP_CONCEPTS
   template <class A>
     inline ConstCallable8<Fun8Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result>,A >::result_type
   operator()( const A& x) const
     {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "fun8 calls binder1of8" << std::endl;
#endif
       return bind1of8(rep, x);
     }
#else
   template <class A>
   typename Sig<A>::ResultType
   operator()( const A& x ) const
     {
#ifdef FCPP_PRINT_HELPERS8
      std::cout << "fun8 calls binder1of8" << std::endl;
#endif
       return bind1of8(rep, x);
     }
#endif

#ifdef FCPP_ENABLE_LAMBDA
   typedef Fun8 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("Fun8");
   }
#endif


};

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Arg8, class Result>
  class Fun8Impl : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Result> , public IRefable {
  //   mutable RefCountType refC;
public:
   Fun8Impl() {}

   virtual Result operator()( const Arg1&, const Arg2&, const Arg3&, const Arg4&, const Arg5&, const Arg6&, const Arg7&, const Arg8& ) const =0;
   virtual ~Fun8Impl() {}
};

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class A8d, class Rd,
  class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class A8s, class Rs>
  class Fun8Converter : public Fun8Impl<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd> {
    typedef IRef<const Fun8Impl<A1s,A2s,A3s,A4s,A5s,A6s,A7s,A8s,Rs> > MyFun;
   MyFun f;
public:
   Fun8Converter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& t, const A5d& u, const A6d& v, const A7d& w, const A8d& s) const {
     return f->operator()( x, y, z, t, u, v, w, s );
   }
};

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class A8d, class Rd,
  class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class A8s, class Rs>
  class Fun8ExplicitConverter : public Fun8Impl<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd> {
    typedef IRef<const Fun8Impl<A1s,A2s,A3s,A4s,A5s,A6s,A7s,A8s,Rs> > MyFun;
   MyFun f;
public:
   Fun8ExplicitConverter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& t , const A5d& u, const A6d& v, const A7d& w, const A8d& s) const {
      return static_cast<Rd>( f->operator()(  static_cast<A1s>(x), static_cast<A2s>(y),
					      static_cast<A3s>(z), static_cast<A4s>(t), static_cast<A5s>(u), static_cast<A6s>(v), static_cast<A7s>(w), static_cast<A8s>(s) ) );
   }
};

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class A8d, class Rd,
  class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class A8s, class Rs>
  Fun8<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd> explicit_convert8( const Fun8<A1s,A2s,A3s,A4s,A5s,A6s,A7s,A8s,Rs>& f ) {
  return Fun8<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd>( 1,
  new Fun8ExplicitConverter<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd,A1s,A2s,A3s,A4s,A5s,A6s,A7s,A8s,Rs>(f.rep.ref) );
}

template <class Gen>
class Gen8 : public Fun8Impl<
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::first_argument_type,
   typename monomorphic_traits<Gen>::second_argument_type,
   typename monomorphic_traits<Gen>::third_argument_type,
   typename monomorphic_traits<Gen>::fourth_argument_type,
   typename monomorphic_traits<Gen>::fifth_argument_type,
   typename monomorphic_traits<Gen>::sixth_argument_type,
   typename monomorphic_traits<Gen>::seventh_argument_type,
   typename monomorphic_traits<Gen>::eighth_argument_type,
   typename monomorphic_traits<Gen>::result_type>
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg7Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg8Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType>
#endif
{
   Gen g;
public:
   Gen8( Gen x ) : g(x) {}
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::result_type operator()(
      const typename monomorphic_traits<Gen>::first_argument_type&    x,
      const typename monomorphic_traits<Gen>::second_argument_type&   y,
      const typename monomorphic_traits<Gen>::third_argument_type&    z,
      const typename monomorphic_traits<Gen>::fourth_argument_type&   t,
      const typename monomorphic_traits<Gen>::fifth_argument_type&    u,
      const typename monomorphic_traits<Gen>::sixth_argument_type&    v,
      const typename monomorphic_traits<Gen>::seventh_argument_type&  w,
      const typename monomorphic_traits<Gen>::eighth_argument_type&  s ) const
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType
   operator()(
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type& x,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type& y,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type& z,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type& t,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type& u,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type& v,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg7Type& w,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg8Type& s
 ) const
#endif
   {
     return g(x,y,z,t,u,v,w,s);
   }
};

#ifndef FCPP_CONCEPTS
template <class Octavary>
Fun8<typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type,
     typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
     typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
     typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
     typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
     typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type,
     typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg7Type,
     typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg8Type,
     typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType>
makeFun8( const Octavary& g ) {
	std::cout << "Used old style makeFun8" << std::endl;
   return
   Fun8<typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type,
        typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
        typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
        typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
        typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
        typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type,
        typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg7Type,
        typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg8Type,
        typename Octavary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType>
        (1,new Gen8<Octavary>(g));
}
#endif

template <class Octavary>
Gen8<Octavary>* makeFun8Ref( const Octavary& g ) {
   return new Gen8<Octavary>(g);
}

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class A8d, class Rd, class DF>
struct Fun8Constructor {
  static Fun8Impl<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd>* make( const DF& df ) {
    return makeFun8Ref( ::fcpp::monomorphize8<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd>(df) );
   }
};

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class A8d, class Rd,
  class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class A8s, class Rs>
  struct Fun8Constructor<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd,Fun8<A1s,A2s,A3s,A4s,A5s,A6s,A7s,A8s,Rs> > {
  static Fun8Impl<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd>* make( const Fun8<A1s,A2s,A3s,A4s,A5s,A6s,A7s,A8s,Rs>& f ) {
    return convert8<A1d,A2d,A3d,A4d,A5d,A6d,A7d,A8d,Rd>(f.rep.ref);
   }
};

//////////////////////////////////////////////////////////////////////

#endif
