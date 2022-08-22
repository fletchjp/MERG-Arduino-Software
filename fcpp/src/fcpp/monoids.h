//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_MONOIDS_DOT_H
#define FCPP_MONOIDS_DOT_H

//#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Monoid operations based on ideas from https://bartoszmilewski.com/2014/12/05/categories-great-and-small/
/// and also from Learn You a Haskell for Great Good! p.252.
///
/// Haskell code:
///
/// class Monoid m where
///    mempty  :: m
///    mappend :: m -> m -> m
///    mconcat :: [m] -> m
///    mconcat = foldr mappend mempty
///    mnot    :: m -> !m  - implemented to negate bool values.
///
/// Note(1): mconcat is only in Learn You a Haskell for Great Good! p.252.
///
/// Note(2): mnot is something I have invented and added to my implementation.
///
/// Following Monads in monad.h what I need first are the free functions.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace fcpp {
	
/// Infer structure from the equivalent for Monad in monad.h
template <class Rep> struct ThisTypeIsNotAnInstanceOfAnInferrableMonoid {};
template <class T> struct MonoidError { inline static void error() {} };
template <class T>
struct MonoidError<ThisTypeIsNotAnInstanceOfAnInferrableMonoid<T> > {};
template <class Rep> struct MonoidTraitsSpecializer {
   typedef ThisTypeIsNotAnInstanceOfAnInferrableMonoid<Rep> Monoid;
};
template <class Rep> struct MonoidTraits {
   typedef typename MonoidTraitsSpecializer<Rep>::Monoid Monoid;
   inline static void ensure_is_instance_of_monoid() {
      MonoidError<Monoid>::error();
   }
};

//////////////////////////////////////////////////////////////////////
/// Free functions for Monoids
///    mempty  :: m
///    mappend :: m -> m -> m
///    mconcat :: [m] -> m
///    mnot    :: m -> !m  - implemented to negate bool values.
//////////////////////////////////////////////////////////////////////
/// mempty<Monoid>()();
///
/// mappend<Monoid>()(m1,m2);
/// mmappend(m1,m2) - now inferrable for Mstring.
///
///
/// mconcat<Monoid>()(lm) where lm has the type List<Monoid>
/// mmconcat(lm)        - now inferrable for Mstring.
///
/// mmnot(m)            - now inferrable for MonoidT<MonoidType> types.
//////////////////////////////////////////////////////////////////////

namespace impl {
  // I have used the example of IntZero in prelude.h line 3947 as a basis.
   template <class Monoid>
   struct XMempty : public CFunType<typename Monoid::Rep::Type> {
    using MonoidResultType = typename Monoid::Rep::Type;
       MonoidResultType
      operator()() const {
         return Monoid::mempty()();
      }

   };

   template <class Monoid>
   struct XMappend {
      using MonoidResultType = typename Monoid::Rep::Type;
      //typedef typename MonoidResultType::MonoidType T;
	  // This case can occur e.g. with Mendo.
      template <class A, class B> struct Sig
	  : public FunType<A,B,typename Monoid::Mappend::template Sig<A,B>::ResultType> {};

      // The case where the monoids have the same type
      template <class A> struct Sig<A,A>
      : public FunType<A,A,typename Monoid::Mappend::template Sig<A,A>::ResultType> {};

      template <class A>
      typename Sig<A,A>::ResultType
      operator()( const A& a1 , const A& a2) const {
         return Monoid::mappend()(a1,a2);
      }

      // Type checks will be done at the next level down.
      template<class A,class B>
      typename Sig<A,B>::ResultType operator()(const A &a,const B &b) const {
        return Monoid::mappend()(a,b);
 	  }
   };

   template <class Monoid>
   struct XMconcat {
      template <class L> struct Sig
      : public FunType<L,typename Monoid::Mconcat::template Sig<L>::ResultType> {};

      template <class L>
      typename Sig<L>::ResultType
      operator()( const L& l ) const {
         return Monoid::mconcat()(l);
      }

   };

   template <class Monoid>
   struct XMnot {
      template <class A> struct Sig
      : public FunType<A,A> {};

      template <class T>
      T operator()( const T& t ) const {
         return Monoid::mnot()(t);
      }

   };


  }

template <class Monoid> Full0<impl::XMempty<Monoid> > mempty()
{ return makeFull0( impl::XMempty<Monoid>() ); }
template <class Monoid> struct Mempty
{ typedef Full0<impl::XMempty<Monoid> > Type; };

template <class Monoid> Full2<impl::XMappend<Monoid> > mappend()
{ return makeFull2( impl::XMappend<Monoid>() ); }
template <class Monoid> struct Mappend
{ typedef Full2<impl::XMappend<Monoid> > Type; };


template <class Monoid> Full1<impl::XMconcat<Monoid> > mconcat()
{ return makeFull1( impl::XMconcat<Monoid>() ); }
template <class Monoid> struct Mconcat
{ typedef Full1<impl::XMconcat<Monoid> > Type; };

template <class Monoid> Full1<impl::XMnot<Monoid> > mnot()
{ return makeFull1( impl::XMnot<Monoid>() ); }
template <class Monoid> struct Mnot
{ typedef Full1<impl::XMnot<Monoid> > Type; };


////////////////////////////////////////////////////////////////////
// The next thing is to infer mmappend(m1,m2)
// based on bind in monad.h from line 206
// I have now done mmconcat(lm) as well.
// mmnot is also available for MonoidT<MonoidType> types
// These are still to added here.
////////////////////////////////////////////////////////////////////

namespace impl {
   template <bool b> struct AppendError {};
   template <> struct AppendError<true> { static inline void error() {} };
   template <class M, class K>
   struct AppendHelper {
      typedef typename MonoidTraits<M>::Monoid MonoidA;
      //typedef typename MonadA::template UnRep<M>::Type A;
      typedef typename MonoidA::Mappend::template Sig<M,K>::ResultType ResultType;
      typedef typename MonoidTraits<ResultType>::Monoid MonoidB;
      inline static void ensure_m_and_k_are_in_the_same_monoid_instance() {
         AppendError<Conversion<MonoidA,MonoidB>::sameType>::error();
      }
   };
   struct XMMappend {
      template <class M, class K> struct Sig
      : public FunType<M,K,typename AppendHelper<M,K>::ResultType> {};

      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         AppendHelper<M,K>::ensure_m_and_k_are_in_the_same_monoid_instance();
         typedef typename AppendHelper<M,K>::MonoidA MM;
         return MM::mappend()(m,k);
      }

      // This specialisation is to handle cases such as mmappend(inc,inc)
	  // for Mendo without actually accessing the Mendo monoid.
	  // The arguments are type Full1<F>.
      template <class F,class G>
      struct Sig<Full1<F>,Full1<G>> : public FunType<Full1<F>,Full1<G>,
	  typename impl::XCompose::Helper<FunctoidTraits<Full1<G>>::max_args,
	           Full1<F>,Full1<G>>::Result> {};

      template <class F,class G>
      typename Sig<Full1<F>,Full1<G>>::ResultType
      operator()(const Full1<F> &f,const Full1<G> &g) const {
         return f ^dot^ g;
      }

      // This specialisation is to handle cases such as mmappend(finc,finc)
	  // for Mendo without actually accessing the Mendo monoid.
	  // The arguments are type Fun1<T,T> e.g. Full1<int,int> finc = inc
	  /* This breaks other things.
     template <class T>
      struct Sig<Fun1<T,T>,Fun1<T,T>> : public FunType<Fun1<T,T>,Fun1<T,T>,
	  typename impl::XCompose::Helper<FunctoidTraits<Fun1<T,T>>::max_args,
	           Fun1<T,T>,Fun1<T,T>>::Result> {};

      template <class T>
      typename Sig<Fun1<T,T>,Fun1<T,T>>::ResultType
      operator()(const Fun1<T,T> &f,const Fun1<T,T> &g) const {
         return f ^dot^ g;
      }
       */


   };
   template <class L>
   struct ConcatHelper {
      //EnsureListLike<L>();
      typedef typename L::ElementType M;
      typedef typename MonoidTraits<M>::Monoid MonoidA;
      //typedef typename MonadA::template UnRep<M>::Type A;
      typedef typename MonoidA::Mconcat::template Sig<M>::ResultType ResultType;
      typedef typename MonoidTraits<ResultType>::Monoid MonoidB;
      inline static void ensure_m_and_k_are_in_the_same_monoid_instance() {
         AppendError<Conversion<MonoidA,MonoidB>::sameType>::error();
      }
   };
   struct XMMconcat {
      template <class L> struct Sig
      : public FunType<L,typename ConcatHelper<L>::ResultType> {};

      template <class L>
      typename Sig<L>::ResultType
      operator()( const L& l ) const {
         //AppendHelper<M,K>::ensure_m_and_k_are_in_the_same_monoid_instance();
         typedef typename ConcatHelper<L>::MonoidA MM;
         return MM::mconcat()(l);
      }

   };

   template <class T>
   struct NotHelper {
     typedef typename MonoidTraits<T>::Monoid Monoid;
   };

   struct XMMnot {
      template <class A> struct Sig
      : public FunType<A,A> {};

      template <class T>
      T operator()( const T& t ) const {
         //AppendHelper<M,K>::ensure_m_and_k_are_in_the_same_monoid_instance();
         typedef typename NotHelper<T>::Monoid MM;
         return MM::mnot()(t);
      }

   };

}

typedef Full2<impl::XMMappend> MMappend;
typedef Full1<impl::XMMconcat> MMconcat;
typedef Full1<impl::XMMnot>    MMnot;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN MMappend mmappend;
FCPP_MAYBE_EXTERN MMconcat mmconcat;
FCPP_MAYBE_EXTERN MMnot    mmnot;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
/// A first construct for an instance of a Monoid.
/// I also want to make other things e.g. List become Monoids as well.
/// This has been extended.
/// (1) to provide for it to hold the string.
/// (2) to have a constructor from a string.
/// (3) to be able to output the string.
struct Mstring {
   struct Rep { typedef Mstring Type; };
   typedef Mstring MonoidType;
   //struct UnRep { typedef typename std::string Type; };
private:
     std::string _string;
public:
    // constructors 
    Mstring() : _string(std::string()) { }
    Mstring(const std::string &s) : _string(s) { }
    Mstring(const Mstring& m) : _string(m._string) { } 
    Mstring operator= (const Mstring &a) {
      _string = a._string; return *this;
    }
    std::string operator()() const { return _string; }
    std::string get_string() const { return _string; }
    Mstring operator+ (const Mstring &a)
    {
      return Mstring(this->_string + a._string);
    }
    struct XMempty : public CFunType<Mstring> {      
      Mstring operator()() const {
         return Mstring();
      }
    };
    typedef Full0<XMempty> Mempty;
    static Mempty& mempty() {static Mempty f; return f;}

    struct XMappend : public CFunType<Mstring,Mstring,Mstring> {
      Mstring operator()(const Mstring &a,const Mstring &b) const {
         return Mstring( a + b );
      }
    };
    typedef Full2<XMappend> Mappend;
    static Mappend& mappend() {static Mappend f; return f;}

    struct XMconcat {
      template <class L> struct Sig : public FunType<L,Mstring> {};
      template <class L> 
      typename Sig<L>::ResultType operator()( const L& l) const {
         EnsureListLike<L>();
         L ltail = l;
         Mstring res = Mstring();
         while (!null(ltail)) {
            res = XMconcatHelper(res,ltail);
            ltail = tail(ltail);
         }
         return res;
      }
    };
    typedef Full1<XMconcat> Mconcat;
    static Mconcat& mconcat() {static Mconcat f; return f;}


    friend Mstring operator+ (const Mstring &a,const Mstring &b);
};


    Mstring operator+ (const Mstring &a,const Mstring &b)
    {
       return Mstring(a._string + b._string);
    }
	
    template <class L> Mstring XMconcatHelper(const Mstring &m, const L& l)
    {
       if (null(l)) return m;
       else return Mstring::mappend()(m,l.head()); 
    }


    template <> struct MonoidTraitsSpecializer<Mstring > {
       typedef Mstring Monoid;
    };

//////////////////////////////////////////////////////////////////////
/// A construct for an instance of a Monoid based on List<T>.
/////////////////////////////////////////////////////////////////////
template <class T>
struct Mlist : public List<T> {
   struct Rep { typedef Mlist<T> Type; };
   typedef Mlist<T> MonoidType;
    Mlist()  : List<T>() { }
    Mlist(const List<T>&l)  : List<T>(l) { }
    Mlist(const Mlist<T>&m) {       //List<T>::operator=(m);
       *this = m;
    } 
    Mlist<T> &operator= (const Mlist<T> &a) {
       List<T>::operator=(a); return *this;
    }
    Mlist<T> &operator= (const List<T> &a) {
       List<T>::operator=(a); return *this;
       return *this;
    }
    struct XMempty : public CFunType<Mlist<T> > {      
      Mstring operator()() const {
         return Mlist<T>();
      }
    };
    typedef Full0<XMempty> Mempty;
    static Mempty& mempty() {static Mempty f; return f;}

    struct XMappend {
 
      template<class A,class B> struct Sig;
      template <class A>
      struct Sig<A,A> : public FunType<A,A,A> {};

      Mlist<T> operator()(const Mlist<T> &a,const Mlist<T> &b) const {
          List<Mlist<T>> lm;
          lm = cons(a,lm);
          lm = snoc(b,lm);
          return Mlist<T>(concat( lm ));
      }
    };
    typedef Full2<XMappend> Mappend;
    static Mappend& mappend() {static Mappend f; return f;}

    struct XMconcat {
      template <class L> struct Sig : public FunType<L,Mlist<T>> {};
      template <class L> 
      typename Sig<L>::ResultType operator()( const L& l) const {
         EnsureListLike<L>();
         return Mlist<T>(concat( l ));
      }
    };
    typedef Full1<XMconcat> Mconcat;
    static Mconcat& mconcat() {static Mconcat f; return f;}

};

template <class T> struct MonoidTraitsSpecializer<Mlist<T> > {
   typedef Mlist<T> Monoid;
};

/////////////////////////////////////////////////////////////////////
// Idea for a structure which can be a Monoid for different types.
// The type needs to have a zero and an op for append.
// This now works such that the operations specific to the 
// type and operator are in MonoidType and MonoidT handles the interface.
// This makes it very easy to generate new instances.
// It now has mconcat as well.
// I want to explore interoperability of types with the same underlying T.
//
// In comparison with the examples above, this separates the two parts of the code.
//
// MonoidType<T,Op> contains the specific data for the type, 
// including static values for the zero element of type T
// and the already existing FC++ operator type Op
// which have to be declared for each use of the type.
//
// MonoidT<MonoidType> then provides the generalised definitions of
// mempty, mappend, mconcat and mnot which apply to all instances.
//
// mnot :: m -> !m  is an extra which I have invented to suit my usage.
//
/////////////////////////////////////////////////////////////////////

template <class T, class Op>
struct MonoidType {
   static T zero;
   static Op op;
   typedef T Mtype;
   T value;
   MonoidType() : value(zero) {}
   MonoidType(const T& t) : value(t) {}
   MonoidType(const MonoidType& m) : value(m.value) {}
   T operator()() { return value; }
};

///////////////////////////////////////////////////////////////////////////
// Some instances of MonoidType.
///////////////////////////////////////////////////////////////////////////
typedef MonoidType<int,Plus> MonoidPlus;
template <> int MonoidType<int,Plus>::zero = 0;
template <> fcpp::Plus MonoidPlus::op = fcpp::plus;

typedef MonoidType<int,Multiplies> MonoidMultiplies;
template <> int MonoidType<int,Multiplies>::zero = 1;
template <> fcpp::Multiplies MonoidMultiplies::op = fcpp::multiplies;

typedef MonoidType<bool,Or2> MonoidAny;
template <> bool MonoidType<bool,Or2>::zero = false;
template <> fcpp::Or2 MonoidAny::op = fcpp::or2;

typedef MonoidType<bool,XOr2> MonoidXor;
template <> bool MonoidType<bool,XOr2>::zero = false;
template <> fcpp::XOr2 MonoidXor::op = fcpp::xor2;

typedef MonoidType<bool,And2> MonoidAll;
template <> bool MonoidType<bool,And2>::zero = true;
template <> fcpp::And2 MonoidAll::op = fcpp::and2;

///////////////////////////////////////////////////////////////
// MonoidT<MonoidType>
///////////////////////////////////////////////////////////////

template <class T>
struct MonoidT
{ 
     typedef typename T::Mtype Mtype;
     typedef T MonoidType;
     struct Rep { typedef MonoidT<T> Type; };
     
     struct XMempty : public CFunType<T> {      
      T operator()() const {
         return T();
      }
    };
    typedef Full0<XMempty> Mempty;
    static Mempty& mempty() {static Mempty f; return f;}

    template<class A,class B>
    struct XMappendHelper {
      typedef typename A::Mtype Atype;
      typedef typename B::Mtype Btype;
      inline static void ensure_a_and_b_have_the_same_data_type() {
         impl::AppendError<Conversion<Atype,Btype>::sameType>::error();
      }
     
    };
		    
    struct XMappend {
      // force the return of the same type as T
      template<class A,class B> struct Sig : public FunType<A,B,T> {};

      template <class A>
      struct Sig<A,A> : public FunType<A,A,T> {};

      T operator()(const T &a,const T &b) const {
          return T(T::op(a.value,b.value));
      }

      template<class A,class B>
      typename Sig<A,B>::ResultType operator()(const A &a,const B &b) const {
        // Check that both A and B are have the same type as T.
         typedef typename XMappendHelper<A,T>::Atype Atype;
         typedef typename XMappendHelper<T,B>::Btype Btype;
         return T(T::op(a.value,b.value));
      }

    
    };
    typedef Full2<XMappend> Mappend;
    static Mappend& mappend() {static Mappend f; return f;}

    struct XMconcat {
      template <class L> struct Sig : public FunType<L,T> {};
      template <class L> 
      typename Sig<L>::ResultType operator()( const L& l) const {
         EnsureListLike<L>();
         T res;
         L ltail = l;
         while (!null(ltail)) {
            res = T(T::op(res.value,ltail.head().value));
            ltail = tail(ltail);
         }
         return res;
      }
    };
    typedef Full1<XMconcat> Mconcat;
    static Mconcat& mconcat() {static Mconcat f; return f;}

    struct XMnot{
      template <class A>
      struct Sig : public FunType<A,A> {};
      T operator()(const T &t) const {
         return T(!t.value);
      }
    };
    typedef Full1<XMnot> Mnot;
    static Mnot& mnot() {static Mnot f; return f;}

};

template <class T> struct MonoidTraitsSpecializer<MonoidT<T> > {
   typedef MonoidT<T> Monoid;
};

// Note that this uses two levels of template to set up the trait.
template <class T, class Op> struct MonoidTraitsSpecializer<MonoidType<T,Op> > {
   typedef MonoidT<MonoidType<T,Op>> Monoid;
};



} // namespace fcpp

#endif
