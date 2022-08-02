//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FUNCTION6_DOT_H
#define FCPP_FUNCTION6_DOT_H

//////////////////////////////////////////////////////////////////////
// Fun6 implementation (under development). See function.h.
//////////////////////////////////////////////////////////////////////

 template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result>
class Fun6Impl;

template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result>
struct Fun6;

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class Rd,
          class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class Rs>
  Fun6Impl<A1d,A2d,A3d,A4d,A5d,A6d,Rd>*
  convert6( const IRef<const Fun6Impl<A1s,A2s,A3s,A4s,A5s,A6s,Rs> >& f);

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class Rd, class DF>
struct Fun6Constructor;

// Special function for the default constructor to call.
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result>
  Result nullfun6(Arg1 a, Arg2 b, Arg3 c, Arg4 d, Arg5 e,Arg6 f) { return Result(); }

// The "Guts" class helps us implement currying; Fun6 floats gently atop
// Fun6Guts and adds currying.
template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result>
  class Fun6Guts : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result> {
   typedef IRef<const Fun6Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result> > RefImpl;
   RefImpl ref;

   template <class A, class B, class C, class D, class E, class F, class G> friend class Fun6;
   
   template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class Rd, class DF>
   friend struct Fun6Constructor;

   template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class Rd, 
             class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class Rs> 
     friend Fun6<A1d,A2d,A3d,A4d,A5d,A6d,Rd> 
     explicit_convert6( const Fun6<A1s,A2s,A3s,A4s,A5s,A6s,Rs>& f );
public:
   typedef Fun6Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>* Impl;

   Fun6Guts( int, Impl i ) : ref(i) {}
   Result operator()( const Arg1& x, const Arg2& y, const Arg3& z, const Arg4& u ,const Arg5& v, const Arg6& w) const
   { return ref->operator()(x,y,z,u,v,w); }

   template <class DF>
   Fun6Guts( const DF& df )
     : ref( Fun6Constructor<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result,DF>::make(df) ) {}

   Fun6Guts( const Fun6Guts& x ) : ref(x.ref) {}
   Fun6Guts& operator=( const Fun6Guts& x ) { ref = x.ref; return *this; }
   Fun6Guts() { 
     Fun6Guts tmp = 
       ptr_to_fun( &nullfun6<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>);
     ref = tmp.ref;
   }
};

 template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result>
   class Fun6 :  public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>, public SmartFunctoid6 {
  template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class Rd, class DF>
   friend struct Fun6Constructor;

   template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class Rd, 
             class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class Rs> 
     friend Fun6<A1d,A2d,A3d,A4d,A5d,A6d,Rd> 
     explicit_convert6( const Fun6<A1s,A2s,A3s,A4s,A5s,A6s,Rs>& f );

   Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result> rep;
public:
   typedef Fun6Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>* Impl;

   Fun6() {} 
   Fun6( int, Impl i ) : rep(1,i) {}

   template <class DF> 
   Fun6( const DF& df ) : rep(df) {}

   Fun6( const Fun6& x ) : rep(x.rep) {}
   Fun6& operator=( const Fun6& x ) { rep = x.rep; return *this; }

#ifndef FCPP_CONCEPTS
   typedef Full6<Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result> > SigHelp;
   template <class A, class B=void, class C=void, class D=void, class E=void, class F=void>
     struct Sig : public SigHelp::template Sig<A,B,C,D,E,F> {};
#endif

   template <class A, class B, class C, class D, class E, class F>
#ifdef FCPP_CONCEPTS
     inline ConstCallable6<Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C,D,E,F >::result_type
     operator()( const A& x, const B& y, const C& z, const D& u, const E& v, const F& w ) const 
      { 
	// std::cout << "ConstCallable6 used in Fun6" << std::endl;
 	// Use helper to cover the cases with autocurrying.
     return impl::Curryable6Helper<typename ConstCallable6<
       Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C,D,E,F>::result_type,
       Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C,D,E,F>::go(rep,x,y,z,u,v,w);
      }
#else 
   typename Sig<A,B,C,D,E,F>::ResultType
   operator()( const A& x, const B& y, const C& z, const D& u, const E& v, const F& w ) const 
     { return makeFull6(rep)(x,y,z,u,v,w); } 
#endif

   template <class A, class B, class C, class D, class E>
#ifdef FCPP_CONCEPTS
     inline ConstCallable6<Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C,D,E >::result_type
     operator()( const A& x, const B& y, const C& z, const D& u, const E& v ) const 
      { 
 	// Use helper to cover the cases with autocurrying.
     return impl::Curryable6Helper2<typename ConstCallable6<
       Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C,D,E>::result_type,
       Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C,D,E>::go(rep,x,y,z,u,v);
      }
#else 
   typename Sig<A,B,C,D,E>::ResultType
   operator()( const A& x, const B& y, const C& u, const D& v, const E& w  ) const 
     {  
       return ::fcpp::makeFull6(rep)(x,y,u,v,w);
     }
#endif

   template <class A, class B, class C, class D>
#ifdef FCPP_CONCEPTS
     inline ConstCallable6<Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C,D >::result_type
   operator()( const A& x, const B& y, const C& z, const D& v ) const 
      { 
 	// Use helper to cover the cases with autocurrying.
     return impl::Curryable6Helper3<typename ConstCallable6<
       Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C,D>::result_type,
       Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C,D>::go(rep,x,y,z,v);
      }
#else 
   typename Sig<A,B,C,D>::ResultType
   operator()( const A& x, const B& y, const C& v, const D& w ) const 
     {  
       return ::fcpp::makeFull6(rep)(x,y,v,w);
     }
#endif


   template <class A, class B, class C>
#ifdef FCPP_CONCEPTS
     inline ConstCallable6<Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C >::result_type
   operator()( const A& x, const B& y, const C& z ) const 
      { 
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable6Helper4<typename ConstCallable6<
       Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C>::result_type,
       Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B,C>::go(rep,x,y,z);
      }
#else
   typename Sig<A,B,C>::ResultType
   operator()( const A& x, const B& y, const C& z ) const 
     { 
       return ::fcpp::makeFull6(rep)(x,y,z);
     }
#endif

   template <class A, class B>
#ifdef FCPP_CONCEPTS
     inline ConstCallable6<Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B >::result_type
   operator()( const A& x, const B& y ) const 
      { 
	// Use helper to cover the cases with autocurrying.
     return impl::Curryable6Helper5<typename ConstCallable6<
       Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B>::result_type,
       Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A,B>::go(rep,x,y);
      }
#else
   typename Sig<A,B>::ResultType
   operator()( const A& x, const B& y ) const 
     { 
        return ::fcpp::makeFull6(rep)(x,y);
     }
#endif

#ifdef FCPP_CONCEPTS
   // This is an important change as the result is a full functoid
   // not a function object.
   //inline ConstCallable5<Fun5Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Result>,Arg1 >::result_type
   //operator()( const Arg1& x) const {
   // Changed to make argument a templated type.
   template <class A>
     inline ConstCallable6<Fun6Guts<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>,A >::result_type
   operator()( const A& x) const 
     {
       //std::cout << "Used modified operator() " << std::endl;
       return bind1of6(rep, x);
     }
#else
   template <class A>
   typename Sig<A>::ResultType
   operator()( const A& x ) const 
     {
       // std::cout << "calling curry6(rep,x)" << std::endl;
       //return curry6(rep,x); 
       return bind1of6(rep, x);
     }
#endif

#ifdef FCPP_ENABLE_LAMBDA
   typedef Fun6 This;
   template <class A> typename fcpp_lambda::BracketCallable<This,A>::Result
   operator[]( const A& a ) const
   { return fcpp_lambda::BracketCallable<This,A>::go( *this, a ); }
#endif

#ifdef FCPP_DEBUG
   std::string name() const {
     return debug_string_and_name_or_string("Fun6::",*(rep.ref),"Fun6");
   }
#endif

};

 template <class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Result>
   class Fun6Impl : public CFunType<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result> , public IRefable {
  //   mutable RefCountType refC;
public:
   Fun6Impl() {}

   virtual Result operator()( const Arg1&, const Arg2&, const Arg3&, const Arg4&, const Arg5&, const Arg6&) const =0;
   virtual ~Fun6Impl() {}
#ifdef FCPP_DEBUG
   virtual std::string name() const {
        return std::string("Fun6Impl<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result>(something)");
   }
#endif
};

 template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class Rd, 
           class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class Rs> 
   class Fun6Converter : public Fun6Impl<A1d,A2d,A3d,A4d,A5d,A6d,Rd> {
  typedef IRef<const Fun6Impl<A1s,A2s,A3s,A4s,A5s,A6s,Rs> > MyFun;
   MyFun f;
public:
   Fun6Converter( const MyFun& g ) : f(g) {}
     Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& u, const A5d& v, const A6d& w ) const {
       return f->operator()( x, y, z, u, v, w );
   }
};

 template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d,  class Rd, 
           class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class Rs> 
   Fun6Impl<A1d,A2d,A3d,A4d,A5d,A6d,Rd>* 
   convert6(  const IRef<const Fun6Impl<A1s,A2s,A3s,A4s,A5s,A6s,Rs> >& f
         ) {
   return new Fun6Converter<A1d,A2d,A3d,A4d,A5d,A6d,Rd,A1s,A2s,A3s,A4s,A5s,A6s,Rs>( f );
}

 template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class Rd, 
           class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class Rs> 
   class Fun6ExplicitConverter : public Fun6Impl<A1d,A2d,A3d,A4d,A5d,A6d,Rd> {
  typedef IRef<const Fun6Impl<A1s,A2s,A3s,A4s,A5s,A6s,Rs> > MyFun;
   MyFun f;
public:
   Fun6ExplicitConverter( const MyFun& g ) : f(g) {}
     Rd operator()( const A1d& x, const A2d& y, const A3d& z, const A4d& u , const A5d& v, const A6d& w) const {
      return static_cast<Rd>( f->operator()(  static_cast<A1s>(x), static_cast<A2s>(y), 
      static_cast<A3s>(z), static_cast<A4s>(u), static_cast<A5s>(v), static_cast<A6s>(w)  ) );
   }
};

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class Rs> 
  Fun6<A1d,A2d,A3d,A4d,A5d,A6d,Rd> explicit_convert6( const Fun6<A1s,A2s,A3s,A4s,A5s,A6s,Rs>& f ) {
  return Fun6<A1d,A2d,A3d,A4d,A5d,A6d,Rd>( 1, 
     new Fun6ExplicitConverter<A1d,A2d,A3d,A4d,A5d,A6d,Rd,A1s,A2s,A3s,A4s,A5s,A6s,Rs>(f.rep.ref) );
}

template <class Gen>
class Gen6 : public Fun6Impl<
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::first_argument_type,
   typename monomorphic_traits<Gen>::second_argument_type,
   typename monomorphic_traits<Gen>::third_argument_type,
   typename monomorphic_traits<Gen>::fourth_argument_type,
   typename monomorphic_traits<Gen>::fifth_argument_type,
   typename monomorphic_traits<Gen>::sixth_argument_type,
   typename monomorphic_traits<Gen>::result_type>
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type, 
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type,
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType> 
#endif
{
   Gen g;
public:
   Gen6( Gen x ) : g(x) {}
#ifdef FCPP_CONCEPTS
   typename monomorphic_traits<Gen>::result_type operator()( 
      const typename monomorphic_traits<Gen>::first_argument_type&  x,
      const typename monomorphic_traits<Gen>::second_argument_type& y,
      const typename monomorphic_traits<Gen>::third_argument_type&  z,
      const typename monomorphic_traits<Gen>::fourth_argument_type& u,
      const typename monomorphic_traits<Gen>::fifth_argument_type&  v,
      const typename monomorphic_traits<Gen>::sixth_argument_type&  w ) const
#else
   typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType
   operator()( 
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type& x,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type& y,
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type& z, 
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type& u, 
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type& v, 
   const typename Gen::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type& w 
 ) const
#endif
   { 
     return g(x,y,z,u,v,w); 
   }
#ifdef FCPP_DEBUG
   std::string name() const {
      return debug_string_and_name_or_string("Gen6::",g,"Gen6");
   }
#endif
};

#ifndef FCPP_CONCEPTS
template <class Sextinary>
Fun6<typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type, 
     typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
     typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
     typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
     typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
     typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type,
     typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType> 
makeFun6( const Sextinary& g ) {
#ifdef FCPP_DEBUG
	std::cout << "Used old style makeFun6" << std::endl;
#endif
   return 
   Fun6<typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg1Type,
        typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg2Type,
        typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg3Type,
        typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg4Type,
        typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg5Type,
        typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::Arg6Type,
        typename Sextinary::template Sig<AnyType,AnyType,AnyType,AnyType,AnyType,AnyType>::ResultType> 
        (1,new Gen6<Sextinary>(g));
}
#endif

template <class Sextinary>
Gen6<Sextinary>* makeFun6Ref( const Sextinary& g ) {
   return new Gen6<Sextinary>(g);
}

 template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class Rd, class DF>
struct Fun6Constructor {
  static Fun6Impl<A1d,A2d,A3d,A4d,A5d,A6d,Rd>* make( const DF& df ) {
    return makeFun6Ref( ::fcpp::monomorphize6<A1d,A2d,A3d,A4d,A5d,A6d,Rd>(df) );
   }
};

template <class A1d, class A2d, class A3d, class A4d, class A5d, class A6d, class Rd, 
          class A1s, class A2s, class A3s, class A4s, class A5s, class A6s, class Rs> 
  struct Fun6Constructor<A1d,A2d,A3d,A4d,A5d,A6d,Rd,Fun6<A1s,A2s,A3s,A4s,A5s,A6s,Rs> > {
    static Fun6Impl<A1d,A2d,A3d,A4d,A5d,A6d,Rd>* make( const Fun6<A1s,A2s,A3s,A4s,A5s,A6s,Rs>& f ) {
      return convert6<A1d,A2d,A3d,A4d,A5d,A6d,Rd>(f.rep.ref);
   }
};


#endif

