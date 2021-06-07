//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FUNCTION7_DOT_H
#define FCPP_FUNCTION7_DOT_H

//////////////////////////////////////////////////////////////////////
// Fun7 implementation (under development). See function.h.
//////////////////////////////////////////////////////////////////////

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Result>
class Fun7Impl;

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Result>
struct Fun7;

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd,
          class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class Rs>
  Fun7Impl<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd>*
  convert7( const IRef<const Fun7Impl<A1s,A2s,A3s,A4s,A5s,A6s,A7s,Rs> >& f);

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd, class DF>
struct Fun7Constructor;

// Special function for the default constructor to call.
 template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Result>
   Result nullfun7(Arg1 a, Arg2 b, Arg3 c, Arg4 d, Arg5 e,Arg6 f, Arg7 g) { return Result(); }

// The "Guts" class helps us implement currying; Fun7 floats gently atop
// Fun7Guts and adds currying.
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Result>
  class Fun7Guts : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result> {
   typedef IRef<const Fun7Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result> > RefImpl;
   RefImpl ref;

   template <class A, class B, class C, class D, class E, class F, class G, class H> friend class Fun7;
   
   template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd, class DF>
   friend struct Fun7Constructor;

   template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd,
             class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class Rs>
     friend Fun7<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd>
     explicit_convert7( const Fun7<A1s,A2s,A3s,A4s,A5s,A6s,A7s,Rs>& f );
public:
   typedef Fun7Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>* Impl;

   Fun7Guts( int, Impl i ) : ref(i) {}
     Result operator()( const Arg1& x, const Arg2& y, const Arg3& z, const Arg4& t, const Arg5& u ,const Arg6& v, const Arg7& w) const
   { return ref->operator()(x,y,z,t,u,v,w); }

   template <class DF>
   Fun7Guts( const DF& df )
     : ref( Fun7Constructor<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result,DF>::make(df) ) {}

   Fun7Guts( const Fun7Guts& x ) : ref(x.ref) {}
   Fun7Guts& operator=( const Fun7Guts& x ) { ref = x.ref; return *this; }
   Fun7Guts() {
     Fun7Guts tmp =
       ptr_to_fun( &nullfun7<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>);
     ref = tmp.ref;
   }
};

 template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Result>
   class Fun7 :  public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>, public SmartFunctoid7 {
   template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd, class DF>
   friend struct Fun7Constructor;

   template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd,
             class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class Rs>
     friend Fun7<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd>
     explicit_convert7( const Fun7<A1s,A2s,A3s,A4s,A5s,A6s,A7s,Rs>& f );

   Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result> rep;
public:
   typedef Fun7Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>* Impl;

   Fun7() {}
   Fun7( int, Impl i ) : rep(1,i) {}

   template <class DF>
   Fun7( const DF& df ) : rep(df) {}

   Fun7( const Fun7& x ) : rep(x.rep) {}
   Fun7& operator=( const Fun7& x ) { rep = x.rep; return *this; }

#ifndef FCPP_CONCEPTS
   // Note that this pulls in Sig from Full8 so implementation can be done
   // once there.
   // The usage neatly uses voids to be able to handle all cases.
   typedef Full7<Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result> > SigHelp;
   template <class A, class B=void, class C=void, class D=void, class E=void, class F=void, class G=void>
     struct Sig : public SigHelp::template Sig<A,B,C,D,E,F,G> {};
#endif

   template <class A, class B, class C, class D, class E, class F, class G>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D,E,F,G >::result_type
     operator()( const A& x, const B& y, const C& z, const D& t, const E& u, const F& v, const G& w ) const
      {
	// std::cout << "ConstCallable7 used in Fun7" << std::endl;
 	// Use helper to cover the cases with autocurrying.
     return impl::Curryable7Helper<typename ConstCallable7<
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D,E,F,G>::result_type,
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D,E,F,G>::go(rep,x,y,z,t,u,v,w);
      }
#else
   typename Sig<A,B,C,D,E,F,G>::ResultType
     operator()( const A& x, const B& y, const C& z, const D& t, const E& u, const F& v, const G& w ) const
     { return makeFull7(rep)(x,y,z,t,u,v,w); }
#endif

   template <class A, class B, class C, class D, class E, class F>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D,E,F >::result_type
     operator()( const A& a, const B& b, const C& c,const D& d, const E& e, const F& f ) const
      {
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable7Helper2<typename ConstCallable7<
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D,E,F>::result_type,
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D,E,F>::go(rep,a,b,c,d,e,f);
      }
#else
   typename Sig<A,B,C,D,E,F>::ResultType
   operator()( const A& a, const B& b, const C& c,const D& d, const E& e, const F& f  ) const
     {
       return ::fcpp::makeFull7(rep)(a,b,c,d,e,f);
     }
#endif

   template <class A, class B, class C, class D, class E>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D,E >::result_type
     operator()( const A& a, const B& b, const C& c,const D& d, const E& e ) const
      {
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable7Helper3<typename ConstCallable7<
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D,E>::result_type,
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D,E>::go(rep,a,b,c,d,e);
      }
#else
   typename Sig<A,B,C,D,E>::ResultType
   operator()( const A& a, const B& b, const C& c,const D& d, const E& e ) const
     {
       return ::fcpp::makeFull7(rep)(a,b,c,d,e);
     }
#endif

   template <class A, class B, class C, class D>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D >::result_type
     operator()( const A& a, const B& b, const C& c,const D& d ) const 
      { 
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable7Helper4<typename ConstCallable7<
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D>::result_type,
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C,D>::go(rep,a,b,c,d);
      }
#else
   typename Sig<A,B,C,D>::ResultType
   operator()( const A& a, const B& b, const C& c,const D& d ) const
     {
       return ::fcpp::makeFull7(rep)(a,b,c,d);
     }
#endif

   template <class A, class B, class C>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C >::result_type
   operator()( const A& x, const B& y, const C& z ) const
      {
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable7Helper5<typename ConstCallable7<
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C>::result_type,
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B,C>::go(rep,x,y,z);
      }
#else
   typename Sig<A,B,C>::ResultType
   operator()( const A& x, const B& y, const C& z ) const
     {
       return ::fcpp::makeFull7(rep)(x,y,z);
     }
#endif

   template <class A, class B>
#ifdef FCPP_CONCEPTS
     inline ConstCallable7<Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B >::result_type
   operator()( const A& x, const B& y ) const
      {
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable7Helper6<typename ConstCallable7<
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B>::result_type,
       Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A,B>::go(rep,x,y);
      }
#else
   typename Sig<A,B>::ResultType
   operator()( const A& x, const B& y ) const
     {
        return ::fcpp::makeFull7(rep)(x,y);
     }
#endif

#ifdef FCPP_CONCEPTS
   template <class A>
   inline ConstCallable7<Fun7Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>,A >::result_type
   operator()( const A& x) const
     {
       return bind1of7(rep, x);
     }
#else
   template <class A>
   typename Sig<A>::ResultType
   operator()( const A& x ) const
     {
       return bind1of7(rep, x);
     }
#endif

#ifdef FCPP_ENABLE_LAMBDA
   typedef Fun7 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif
#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Fun7::",*(rep.ref),"Fun7");
   }
#endif
};


 template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7, class Result>
   class Fun7Impl : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result> , public IRefable {
  //   mutable RefCountType refC;
public:
   Fun7Impl() {}

   virtual Result operator()( const Arg1&, const Arg2&, const Arg3&, const Arg4&, const Arg5&, const Arg6&, const Arg7&) const =0;
   virtual ~Fun7Impl() {}
#ifdef FCPP_DEBUG
   virtual std::string name() const {
        return std::string("Fun7Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result>(something)");
   }
#endif
};

 template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd,
           class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class Rs>
   class Fun7Converter : public Fun7Impl<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd> {
   typedef IRef<const Fun7Impl<A1s,A2s,A3s,A4s,A5s,A6s,A7s,Rs> > MyFun;
   MyFun f;
public:
   Fun7Converter( const MyFun& g ) : f(g) {}
     Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& t, const A5d& u, const A6d& v, const A7d& w ) const {
       return f->operator()( x, y, z, t, u, v, w );
   }
};

 template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd,
           class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class Rs>
   Fun7Impl<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd>*
   convert7(  const IRef<const Fun7Impl<A1s,A2s,A3s,A4s,A5s,A6s,A7s,Rs> >& f
         ) {
   return new Fun7Converter<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd,A1s,A2s,A3s,A4s,A5s,A6s,A7s,Rs>( f );
}

 template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd,
           class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s,class Rs>
   class Fun7ExplicitConverter : public Fun7Impl<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd> {
   typedef IRef<const Fun7Impl<A1s,A2s,A3s,A4s,A5s,A6s,A7s,Rs> > MyFun;
   MyFun f;
public:
   Fun7ExplicitConverter( const MyFun& g ) : f(g) {}
     Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& t , const A5d& u, const A6d& v, const A7d& w) const {
      return static_cast<Rd>( f->operator()(  static_cast<A1s>(x), static_cast<A2s>(y),
      static_cast<A3s>(z), static_cast<A4s>(t), static_cast<A5s>(u), static_cast<A6s>(v), static_cast<A7s>(w)  ) );
   }
};

 template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd,
           class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class Rs>
   Fun7<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd> explicit_convert7( const Fun7<A1s,A2s,A3s,A4s,A5s,A6s,A7s,Rs>& f ) {
   return Fun7<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd>( 1,
      new Fun7ExplicitConverter<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd,A1s,A2s,A3s,A4s,A5s,A6s,A7s,Rs>(f.rep.ref) );
}

template <class Gen>
class Gen7 : public Fun7Impl<
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::first_argument_type,
   typename monomorphic_traits<Gen>::second_argument_type,
   typename monomorphic_traits<Gen>::third_argument_type,
   typename monomorphic_traits<Gen>::fourth_argument_type,
   typename monomorphic_traits<Gen>::fifth_argument_type,
   typename monomorphic_traits<Gen>::sixth_argument_type,
   typename monomorphic_traits<Gen>::seventh_argument_type,
   typename monomorphic_traits<Gen>::result_type>
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg7Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType>
#endif
{
   Gen g;
public:
   Gen7( Gen x ) : g(x) {}
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::result_type operator()(
      const typename monomorphic_traits<Gen>::first_argument_type&    x,
      const typename monomorphic_traits<Gen>::second_argument_type&   y,
      const typename monomorphic_traits<Gen>::third_argument_type&    z,
      const typename monomorphic_traits<Gen>::fourth_argument_type&   t,
      const typename monomorphic_traits<Gen>::fifth_argument_type&    u,
      const typename monomorphic_traits<Gen>::sixth_argument_type&    v,
      const typename monomorphic_traits<Gen>::seventh_argument_type&  w ) const
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType
   operator()(
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type& x,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type& y,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type& z,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type& t,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type& u,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type& v,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg7Type& w
 ) const
#endif
   {
     return g(x,y,z,t,u,v,w);
   }
#ifdef FCPP_DEBUG
   std::string name() const {
      return debug_string_and_name_or_string("Gen7::",g,"Gen7");
   }
#endif
};

#ifndef FCPP_CONCEPTS
template <class Septimary>
Fun7<typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type, 
     typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
     typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
     typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
     typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
     typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type,
     typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg7Type,
     typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType>
makeFun7( const Septimary& g ) {
#ifdef FCPP_DEBUG
	std::cout << "Used old style makeFun7" << std::endl;
#endif
   return
   Fun7<typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type,
        typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
        typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
        typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
        typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
        typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type,
        typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg7Type,
        typename Septimary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType>
        (1,new Gen7<Septimary>(g));
}
#endif

template <class Septimary>
Gen7<Septimary>* makeFun7Ref( const Septimary& g ) {
   return new Gen7<Septimary>(g);
}

 template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd, class DF>
struct Fun7Constructor {
  static Fun7Impl<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd>* make( const DF& df ) {
    return makeFun7Ref( ::fcpp::monomorphize7<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd>(df) );
   }
};

 template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class A7d, class Rd,
           class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class A7s, class Rs>
   struct Fun7Constructor<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd,Fun7<A1s,A2s,A3s,A4s,A5s,A6s,A7s,Rs> > {
     static Fun7Impl<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd>* make( const Fun7<A1s,A2s,A3s,A4s,A5s,A6s,A7s,Rs>& f ) {
       return convert7<A1d,A2d,A3d,A4d,A5d,A6d,A7d,Rd>(f.rep.ref);
   }
};


//////////////////////////////////////////////////////////////////////


#endif

