//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_PATTERN_DOT_H
#define FCPP_PATTERN_DOT_H

#include <map>

namespace fcpp {
  ////////////////////////////////////////////////////////////
  // The following were in arrow.h
  //
  // first(f,p) and second(f,p) with p a pair (x,y)
  // first  applies f to x and returns a pair f(x) and y
  // second applies f to y and returns a pair x and f(y)
  // I am doing this rather than swapping x and y
  // as that only works if they are the same type.
  //
  // Example of use:
  // hUncurry(plus,compose(first(inc),second(dec))(makePair(x,x)))
  // This takes an argument (x) and makes a pair of two copies.
  // It increments the first and decrements the second.
  // It then uncurries the pair and feeds it to plus,
  // adding them together.
  // All the other functoids are in prelude or operator.
  ///////////////////////////////////////////////////////////////////////////
  // The rest of what is in this file has come from developments
  // based on the work of Jeremy Gibbons.
  // See in particular:
  //
  // http://patternsinfp.wordpress.com/2010/12/31/stream-monad/
  //
  // which has provided the starting point for Stream<T>
  // and the StreamM monad and a lot of other things to be
  // found here.
  // Examples of use are in patternsinfp.cpp and understanding_list.cpp
  //
  // John Fletcher, December 2013.
  ///////////////////////////////////////////////////////////////////////////
  
/////////////////////////////////////////////////////////////////////////
// I am adding to this file the work which I have done as a result of
// reading "Purely Functional Data Structures" by Chris Okasaki.
// Further down this file are two new classes based on ListM<T>.
/////////////////////////////////////////////////////////////////////////
// Queue<T> object based on "Purely Functional Data Structures"
// Chapter 5 page 42
/////////////////////////////////////////////////////////////////////////
// BootList<T> which implements the ideas on p.152.
// "Purely Functional Data Structures" by Chris Okasaki.
// For more information see example file structures.cpp
//
// John Fletcher, January 2016.
/////////////////////////////////////////////////////////////////////////
// Now with code for Monad transformer MaybeT and associated code.
// Real World Haskell Chapter 18 p.429
//
// Monad Transformers transform one monad into another adding properties.
// They are part of Haskell which I had not heard of before.
// I think it should be possible to add these to FC++.
//
// John Fletcher, February 2016.
/////////////////////////////////////////////////////////////////////////

namespace impl {
     struct XFirst {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("First");
       }
#endif
       template <class F, class P> struct Sig
       {
          typedef typename P::first_type X;
          typedef typename P::second_type Y;
          typedef F Arg1Type;
          typedef P Arg2Type;
          typedef typename std::pair<typename RT<F,X>::ResultType,Y> ResultType;
       };

       template <class F, class P>
       typename Sig<F,P>::ResultType
       operator()( const F& f,const P& p ) const {
           return makePair(f(p.first),p.second);
       }
     };

     struct XSecond {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Second");
       }
#endif
       template <class F, class P> struct Sig
       {
          typedef typename P::first_type X;
          typedef typename P::second_type Y;
          typedef F Arg1Type;
          typedef P Arg2Type;
          typedef typename std::pair<X,typename RT<F,Y>::ResultType> ResultType;
       };

         template <class F, class P>
         typename Sig<F,P>::ResultType
         operator()( const F& f,const P& p ) const {
             return makePair(p.first,f(p.second));
         }
     };
  }
 typedef Full2<impl::XFirst> First;
 typedef Full2<impl::XSecond> Second;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN First first;
 FCPP_MAYBE_EXTERN Second second;
 FCPP_MAYBE_NAMESPACE_CLOSE

  namespace impl {

    struct XParallel {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Parallel");
       }
#endif
      template <class P,class Q>
      struct Sig : public FunType<P,Q,
	     std::pair<typename RT<typename P::first_type,
                                   typename Q::first_type>::ResultType,
		       typename RT<typename P::second_type,
                                   typename Q::second_type>::ResultType > > { };

      template <class P,class Q>
      typename Sig<P,Q>::ResultType operator()
      (const P& p,const Q &q) const
      {
	return std::make_pair(p.first(q.first),p.second(q.second));
      }
    };


    struct XOut {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Out");
       }
#endif
      template <class L>
      struct Sig : public FunType<L,
	     std::pair<typename RT<Head,L>::ResultType,
		       typename RT<Tail,L>::ResultType > > { };

      template <class L>
      typename Sig<L>::ResultType operator()
      (const L& l) const
      {
	return fork_(head,tail,l);
      }
    };

    struct XUnfold1 {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Unfold1");
       }
#endif
      template <class P>
      struct Sig : public FunType<P,typename P::second_type> { };

      template <class P>
      typename Sig<P>::ResultType operator()
      (const P& p) const
      {
	return cons(p.first,p.second);
      }
    };

    // Another version which is I think closer to
    // the model from Jeremy Gibbons
    struct XUnfold2 {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Unfold2");
       }
#endif
      template <class F,class L>
      struct Sig : public FunType<F,L,
		   OddList<typename L::ElementType> > {};

      template <class F, class L>
      OddList<typename L::ElementType>
      operator() (const F& f,const L& l,
       Reuser2<Inv,Inv,Var,XUnfold2,F,List<typename L::ElementType> >
               r = NIL ) const
      {
        if( null( l ) )
           return NIL;
        else {
	  //std::cout << head(l) << std::endl;
   	  std::pair<typename L::ElementType,L> fl = f(l);
	  return cons( fl.first, r( XUnfold2(), f, fl.second ) );
        }
      }
    };
 
   }
 typedef Full2<impl::XParallel> Parallel;
 typedef Full1<impl::XOut>      Out;
 typedef Full1<impl::XUnfold1>  Unfold1;
 typedef Full2<impl::XUnfold2>  Unfold2;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Parallel parallel, prod; // prod is Jeremy's name for this.
 FCPP_MAYBE_EXTERN Out      out;
 FCPP_MAYBE_EXTERN Unfold1  unfold1;
 FCPP_MAYBE_EXTERN Unfold2  unfold2, unfold;
 FCPP_MAYBE_NAMESPACE_CLOSE

  namespace impl {
    struct XDoublef{
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Doublef");
       }
#endif
     template <class F,class A>
      struct Sig : public FunType < F,A,
	std::pair< typename RT<F,A>::ResultType,
	           typename RT<F,A>::ResultType > > { };

      template <class F,class A>
      typename Sig<F,A>::ResultType operator()
      (const F& f, const A& a) const
      {
	typename RT<F,A>::ResultType fa = f(a);
	return makePair(fa,fa);
      }
    };

  }
  typedef Full2<impl::XDoublef> Doublef;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN Doublef doublef;
  FCPP_MAYBE_NAMESPACE_CLOSE


  namespace impl {
    // This implements what I thought was Jeremy's double
    // using doublef as double1
    struct XDouble1{
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Double1");
       }
#endif
      template <class A>
      struct Sig : public FunType < A,
		   typename RT<Doublef,Id,A>::ResultType > {};

      template <class A>
      typename Sig<A>::ResultType operator()
      (const A& a) const
      {
	return doublef(id,a);
      }
    };

  }
  typedef Full1<impl::XDouble1> Double1;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN Double1 double1;
  FCPP_MAYBE_NAMESPACE_CLOSE

  // I first attempted to get this to work using a Reuser.
  // That seems not to work inside a std::pair.
  // So I have defined it in terms of out and repeat, which works.
  namespace impl {
     struct XDouble {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Double");
       }
#endif
      template <class A>
        struct Sig : public FunType<A,typename std::pair<A,List<A> > > {};

        template <class A>
        typename Sig<A>::ResultType
        operator()( const A& a) const
       {
	 return out(repeat(a));
        }
      };
  }
  typedef Full1<impl::XDouble> Double;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN Double double_;
  FCPP_MAYBE_NAMESPACE_CLOSE

  namespace impl {
    // Copy of repeat from prelude as OddList<OddList> is not supported.
    struct XRepeatS {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("RepeatS");
       }
#endif
     template <class T>
     struct Sig : public FunType<T,List<T> > {};
      // Use repeat and not repeatS for the iterations....
     template <class T>
     List<T> operator()( const T& x,
                          Reuser1<Inv,Inv,XRepeat,T> r = NIL ) const {
        return cons( x, r( XRepeat(), x ) );
     }
    };
  }
  typedef Full1<impl::XRepeatS> RepeatS;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN RepeatS repeatS;
  FCPP_MAYBE_NAMESPACE_CLOSE

  namespace impl {
    // This is following example in XEFH in prelude.h
    // Experimental and not yet in use.
    // I do not think I need this.
    /* template <class L>
    struct XDiagH : public Fun0Impl< OddList<typename L::ElementType> > {
      mutable L l;
      XDiagH( const L& ll) : l(ll) {}
      OddList<typename L::ElementType> operator()() const {

      }
      };*/
     // This is now the new version of XDiag
     struct XDiag {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Diag");
       }
#endif
       // SigHelper to sort out the return type.
       template <class L>
       struct SigHelper {
	 typedef typename L::ElementType ListType;
         typedef typename ListType::ElementType ElementType;
         typedef OddList<ElementType> ResultType;
       };

       template <class L>
       struct Sig : public FunType<L,typename SigHelper<L>::ResultType> {};

        template <class L >
        typename Sig<L>::ResultType
        operator()( const L& l,
               Reuser1<Inv,Var,XDiag,L >
               r = NIL ) const {
	  return cons( head(head(l)), r( XDiag(), tail(map(tail, l)) ) );
        }
     };
 
    // There is something wrong with Diag.
    // It works but does not correctly use List and OddList.
    // I think I can do better using sprint() to diagnose it.
    // I am going to start a new version Diag2 which will
    // hopefully replace this version soon.
    // This has now become DiagS and is only used where essential for the
    // code to compile. That is only in bind() in StreamM.
    // I don't know why and hope to resolve that.
    struct XDiagS {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("DiagS");
       }
#endif
       template <class L>
       struct Sig : public FunType<L,typename L::ElementType> {};
       //OddList<typename L::ElementType> {};
      // This now uses Diag internally.
        template <class L >
        typename Sig<L>::ResultType
        //OddList<typename L::ElementType>
        operator()( const L& l,
               Reuser1<Inv,Var,XDiag,L >
               r = NIL ) const {
	  //std::cout << "diag : " << head(head(l)) << std::endl;
	  // changed the last element from map(tail, l) following
	  // Jeremy's definition. I need to throw away the rest
	  // of the first list.
	  return cons( head(head(l)), r( XDiag(), tail(map(tail, l)) ) );
        }
     };

   }

  typedef Full1<impl::XDiagS> DiagS;
  typedef Full1<impl::XDiag> Diag;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN DiagS diagS;
  FCPP_MAYBE_EXTERN Diag diag2, diag;
  FCPP_MAYBE_NAMESPACE_CLOSE


  namespace impl {
     struct XDiagRep {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("DiagRep");
       }
#endif

       template <class L>
       struct Sig : public FunType<L,L> {};

        template <class L >
        typename Sig<L>::ResultType
        operator()( const L& l) const
        {
	    return diag(repeat(l));
        }
     };
   }

  typedef Full1<impl::XDiagRep> DiagRep;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN DiagRep diagRep;
  FCPP_MAYBE_NAMESPACE_CLOSE

  namespace impl {
    struct XMapS {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("MapS");
   }
#endif

       template <class F, class L>
       struct Sig : public FunType<F,L,
       List<typename RT<F,typename L::ElementType>::ResultType> > {};
      // Use Map not MapS internally.
       template <class F, class L>
       List<typename RT<F,typename L::ElementType>::ResultType>
       operator()( const F& f, const L& l,
               Reuser2<Inv,Inv,Var,XMap,F,List<typename L::ElementType> >
               r = NIL ) const {
          if( null(l) )
             return NIL;
          else
	    return cons( f(head(l)), r( XMap(), f, tail(l) ) );
       }
    };
  }
  typedef Full2<impl::XMapS> MapS;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN MapS mapS;
  FCPP_MAYBE_NAMESPACE_CLOSE


    //////////////////////////////////////////////////////////////////
    // For information on snoc see "Purely Functional Data Structures"
    // Chris Okasaki p.42
    // The definition of snoc has now been moved to list.h
    // to make it available in prelude.h
    // I have moved it back here as in list.h there are problems
    // with the constructor in SnocHelp for reasons I do not understand.
    // This version works O.K.
    // The one in list.h (now known as snoc2) needs to have
    // friend declarations for Snoc2Help as is the case for ConsHelp.
    // I have not figured out why this does not need them.
    // It could be how it is used in reverse which is the problem.
    // Now it does not need it! Still mysterious.
    // I have now swapped snoc and snoc2 so that the default
    // implementation is in list.h and snoc2 is a duplicate here.
    //////////////////////////////////////////////////////////////////
namespace impl {
    struct XSnoc2;
    template <class F, class T>
    struct Snoc2Help
    {
      OddList<T> operator()( const F &f, const T& x) const {
        
	return cat( List<T>( ListRaw(), new Cache<T>(f) ),cons(x,NIL));
      }
    };

struct XSnoc2 {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Snoc2");
       }
#endif
 template <class T, class L>
	 struct Sig : public FunType<T,L,OddList<T> > {};

   template <class T>
   OddList<T> operator()(const T& x, const List<T>& l ) const {
     return cat(l,cons(x,NIL));
   }
   template <class T>
   OddList<T> operator()(const T& x, const OddList<T>& l) const {
     return cat(l,cons(x,NIL));
   }
   template <class T>
   OddList<T> operator()( const T& x, const AUniqueTypeForNil&) const {
     return OddList<T>(x,NIL);
   }
   // This now puts the value x after the action of f.
   template <class T, class F>
     OddList<T> operator()(const T& x, const F& f) const {
     return Snoc2Help<F,T>()(f,x);
   }
};
}
typedef Full2<impl::XSnoc2> Snoc2;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Snoc2 snoc2;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XReverse2 {
   template <class L>
   struct Sig : public FunType<L,OddList<typename L::ElementType> > {};

   template <class T>
   OddList<T> operator()( const List<T>& l ) const {
     // Put in for safety. Only called once. Once is enough.
     // This is O.K. as long as the compiler does not
     // optimise it away.
     size_t x = length(l);
     OddList<T> temp = foldr(snoc, OddList<T>(), l );
     return temp;
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Reverse2");
   }
#endif
};
}
typedef Full1<impl::XReverse2> Reverse2;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Reverse2 reverse2;
FCPP_MAYBE_NAMESPACE_CLOSE

  // iterateSum is new. iterateSum(f,a,s)[i] = a + sigma(f(s[i]))
  namespace impl {
    struct XIterateSum {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("IterateSum");
       }
#endif
      template <class F, class T, class S> struct Sig : public FunType<F,T,S,
       OddList<typename RT<F,typename S::ElementType>::ResultType> > {};
      template <class F, class S,class T>
      typename Sig<F,T,S>::ResultType
      operator()( const F& f, const T& t, const S& s1,
		  Reuser3<Inv,Inv,Var,Var,XIterateSum,F,T,
			  List<typename S::ElementType> >
               r = NIL ) const
       {
         T sum = t + f(head(s1));
	 List<typename S::ElementType> temp = cons( sum,
		          r( XIterateSum(), f, sum, tail(s1) ) );
         return temp;
       }

    };

  }
  typedef Full3<impl::XIterateSum> IterateSum;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN IterateSum iterateSum;
  FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
  // Iterate2 does this: iterate2(f,t1,t2) -> (t2,t3=f(t1,t2),t4=f(t2,t3),...)
  // i.e. it puts t2 into the list and passes it as the next t1.
  // If you want t1 at the front you have to add it with
  // hold_back(t1) (default is 0).
struct XIterate2 {
  template <class F, class T1, class T2> struct Sig;
   template <class F, class T>
   struct Sig<F,T,T> : public FunType<F,T,T,OddList<T> > {};

   template <class F, class T>
   OddList<T> operator()( const F& f, const T& x1, const T&x2,
                          Reuser3<Inv,Inv,Var,Var,XIterate2,F,T,T> r = NIL ) const {
     return cons( x2, r( XIterate2(), f, x2, f(x1,x2) ) );
   }
#ifdef FCPP_DEBUG
   std::string name() const {
        return std::string("Iterate2");
   }
#endif
};
}
typedef Full3<impl::XIterate2> Iterate2;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Iterate2 iterate2;
FCPP_MAYBE_NAMESPACE_CLOSE


/////////////////////////////////////////////////////////////////////////
// Ideas for a Queue object based on "Purely Functional Data Structures"
// Chapter 5 page 42
/////////////////////////////////////////////////////////////////////////

 template <class T> class Queue {

   // I could treat this as a std::pair of lists.
   // For the moment I will not.
   List<T> front_;
   List<T> rear_;
 public:
   Queue() { }
   Queue(const Queue &q) : front_(q.front_), rear_(q.rear_) { }
   Queue(const List<T> &l) : front_(l) { }
   Queue(const List<T> &f,const List<T> &r) : front_(f), rear_(r) { }
   Queue<T> checkf() const {
     if (null(front_) ) return Queue<T>(reverse2(rear_) );
     else return *this;
   }
   T head() const { return head(front_); }
   Queue<T> tail() const
   {  Queue<T> temp(front_.tail(),rear_);
      return temp.checkf();
   }
   //friend Queue consq(const T& t, const Queue<T> q);
   //friend Queue<T> snocq(const T& t, const Queue<T> q);
   List<T> front() const { return front_; }
   List<T> rear() const { return rear_; }
 };

 template <class T> Queue<T> consq(const T& t, const Queue<T> q)
 {
   return Queue<T>(cons(t,q.front()),q.rear());
 }

 template <class T> Queue<T> snocq(const T& t, const Queue<T> q)
 {
   return Queue<T>(q.front(),cons(t,q.rear())).checkf();
 }

#ifndef FCPP_ARDUINO
template <class T>
std::ostream & operator << (std::ostream &o, const Queue<T> &q)
{
  o << q.front() << ":" << q.rear();
  return o;
}
#endif

/////////////////////////////////////////////////////////////////////////
// BootList<T> which implements the ideas on p.152.
// "Purely Functional Data Structures"
// For more information see example file structures.cpp
/////////////////////////////////////////////////////////////////////////


  template <class T> class BootList {

    T front_;
    List<BootList<T> > rear_;
    bool empty_; // Only true for null constructor
    // or when constructed from an empty List<BootList<T> >.
  public:
    typedef T ElementType;
    BootList( AUniqueTypeForNil) :  empty_(true), rear_(NIL)
      /*, front_(T(0)) */ { }
    BootList() : empty_(true), rear_(NIL) { }
    BootList(const BootList &b) : front_(b.front_), rear_(b.rear_),
      empty_(b.empty_) { }
    BootList(const T& f) : front_(f), empty_(false),
      rear_(List<BootList<T> >()) { }
    BootList(const List<T> &l)
    {
      empty_ = null(l);
      if (!empty_) {
	front_ = head(l);
        List<T> ll = reverse2(tail(l));
        List<BootList<T> > temp;
        while (!null(ll)) {
	  temp = cons(BootList<T>(head(ll)),temp);
          ll = tail(ll);
          }
        rear_ = temp;
      }
    }
    BootList(const T& f,const BootList<T> &b) : front_(f), empty_(false),
	rear_(List<BootList<T> >(cons(BootList<T>( b.front_ ),b.rear_))) { }
    BootList(const BootList<T>& b1,const BootList<T> &b2)
    : empty_((b1.empty_ && b2.empty_)) {
      if ( empty_ ) { }
      else
	if (b2.is_empty())
         {  front_ = b1.front_; rear_ = b1.rear_; }
      else
	if (b1.is_empty())
         {  front_ = b2.front_; rear_ = b2.rear_; }
      else {
	   front_ = b1.front_;
           rear_ = snoc(b2,b1.rear_);
         }
    }
    BootList(const T& f,const List<BootList<T> > &r) : front_(f), rear_(r),
         empty_(false) { }
    BootList(const List<BootList<T> > &bl) {
      // Construct a BootList<T> from List<BootList<T> >
      // This is needed to walk down the list for output.
      // It may be this can be done more efficiently.
      if (null(bl)) empty_ = true;
      else {
        empty_ = false;
        front_ = head(bl).front_;
        List<BootList<T> > temp1 = head(bl).rear_;
        List<BootList<T> > temp2 = tail(bl);
        List<BootList<T> > temp3 = reverse2(temp1);
        while (!null(temp3)) {
	  temp2 = cons(head(temp3),temp2);
          temp3 = tail(temp3);
          }
        rear_ = temp2;
      }
    }
    T front() const { return front_; }
    List<BootList<T> > rear_list() const { return rear_; }
    BootList<T> rear() const {
      if (null(rear_)) return BootList<T>();
      else return BootList<T>(rear_); }
    bool check() const { return null(rear_); }
    bool is_empty() const { return empty_; }
    List<T> list() const
      {
        if (empty_) return List<T>();
	List<T> temp = makeList1(front_);
        List<BootList<T> > tempr = rear_;
        while (!null(tempr)) {
          List<T> temp2 = head(tempr).list();
          temp = cat(temp,temp2);
          tempr = tail(tempr);
          }
        return temp;
      }
  };

 template <class T> BootList<T> consb(const T& t, const BootList<T> &b)
 {
   return BootList<T>(t,b);
 }

 template <class T> BootList<T> combineb(const BootList<T> &b1,
                                         const BootList<T> &b2)
 {
   return BootList<T>(b1,b2);
 }

 template <class T, class K> BootList<T> mapb(const BootList<T> &b,const K &k)
   {
     // Take the front of b successively and build a new BootList.
     BootList<T> temp1(b);
     BootList<T> temp2;
     while (!temp1.is_empty()) {
       BootList<T> temp3(k(temp1.front()));
       temp2 = combineb(temp2,temp3);
       temp1 = temp1.rear();
     }
     return temp2;
   }

 // This allows mapb to be applied to List<T>, returning BootList<T>
 template <class T, class K> BootList<T> mapb(const List<T> &l,const K &k)
   {
     List<T> temp1(l);
     BootList<T> temp2;
     while (!null(temp1)) {
       BootList<T> temp3(k(head(temp1)));
       temp2 = combineb(temp2,temp3);
       temp1 = tail(temp1);
     }
     return temp2;
   }


namespace impl {

    struct XMapB {
    #ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("mapB");
       }
    #endif

   template <class T, class K>
   struct Sig : public FunType<BootList<T>,K,BootList<T> > {};

      template <class T, class K>
   BootList<T> operator()(const BootList<T>& b, const K& k ) const {
     return mapb(b,k);
   }
  };
}
typedef Full2<impl::XMapB> MapB;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN MapB mapB;
FCPP_MAYBE_NAMESPACE_CLOSE


#ifndef FCPP_ARDUINO
template <class T>
std::ostream & operator << (std::ostream &o, const BootList<T> &b)
{
  o << b.front();
  if (!b.check()) o << " : " << b.rear();
  return o;
}
#endif

#ifdef FCPP_ENABLE_LAMBDA

//////////////////////////////////////////////////////////////////////
// BootListM monad as an experiment
//
// BootListM now has unit, bind, plus and zero, all with tests.
//////////////////////////////////////////////////////////////////////

struct BootListM {
   template <class A> struct Rep { typedef BootList<A> Type; };
   template <class MA> struct UnRep { typedef typename MA::ElementType Type; };

#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("BootListM");
   }
#endif

   struct XUnit {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("BootListM::Unit");
      }
#endif
      template <class A> struct Sig : public FunType<A,BootList<A> > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
	return BootList<A>(x);
      }
   };
   typedef Full1<XUnit> Unit;
   static Unit& unit() {static Unit f; return f;}
  //   static Unit unit;

   struct XBind {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("BootListM::Bind");
      }
#endif
     template <class M, class K> struct Sig : public FunType<M,K,
	BootList<typename M::ElementType> >{};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
	return mapb(m,k);
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}
  //   static Bind bind;

   typedef AUniqueTypeForNil Zero;
   static Zero& zero() {static Zero f; return f;}
  //   static Zero zero;

   struct XPlus {
#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("BootListM::Plus");
   }
#endif
      template <class MA, class MA2> struct Sig : public FunType<MA,MA2,MA> {};
      template <class MA, class MA2>
      typename Sig<MA,MA2>::ResultType
      operator()( const MA& x, const MA2& y ) const {
        return combineb(x,y);
      }
   };
   typedef Full2<XPlus> Plus;
  //   static Plus plus;
   static Plus& plus() {static Plus f; return f;}
};

template <class T> struct MonadTraitsSpecializer<BootList<T> > {
   typedef BootListM Monad;
};

// This specialisation fools bind into thinking BootListM and ListM
// are the same.
// This is a work around of the problem in bind with StreamM.
// It works for Stream<T> because Stream<T> inherits from List<T>.
// The code being used is in fcpp/signature.h
// I have taken this out - it may not be needed at all.
/*
template <> struct Conversion<BootListM,ListM >
{
  static const bool value = true;
  static const bool sameType = true;
};
*/

//////////////////////////////////////////////////////////////////////

#endif

/////////////////////////////////////////////////////////////////////////
// Map and Bimap objects - experimental
/////////////////////////////////////////////////////////////////////////
// For Map see Real World Haskell p.290 and following.
/////////////////////////////////////////////////////////////////////////
// Funny old world.
// I already have Map and Multimap in map.h
// using List<T> as a basis which is not going to be efficient.
// It certainly is not. I am sure I can do better using material
// from Purely Functional Data Structures.
// I have now found some codes written by Bartosz Milewski
// https://github.com/BartoszMilewski
//
// However, if the STD map is based on a Red-Black Tree,
// why reinvent the wheel?  I need to make it lazy.
// See p.332 Section 7.8.1 in The C++ Standard Library (2nd edition)
/////////////////////////////////////////////////////////////////////////

// Do I make the std::map a member or just inherit it?
// What extra things do I need?
// I would like to make use of MayBe<T> for failures.
// That would point toward member rather than inherit.
 template <class K, class V> class StdMap : public std::map<K,V>
{
  // std::map<K,V> map_;
 public:
  //StdMap() {};

};

// Map where results are returned as Maybe<T>
// This needs some thought.
 template <class K, class V> class MaybeMap //: public std::map<K,V>
{
 std::map<K,V> map_;
 public:
 typedef Maybe<K> MaybeK;
 typedef Maybe<V> MaybeV;
  MaybeMap() {};

};




/////////////////////////////////////////////////////////////////////////


  //  template <class T> class Stream;


  // New idea - class Stream<T>
  // The idea is that it can implement operators
  // on Streams.
  template <class T>
  class Stream : public List<T> {

  public:
    // Start with the public constructors for List<T>
    Stream( AUniqueTypeForNil) : List<T>( NIL ) { }
    Stream() : List<T>() { }
    template <class F>
    Stream( const F& f ) : List<T>(f) { }
    // Now add one for a List<T> itself
    Stream( const List<T> &l) : List<T>(l) { }
    template <class It>
    Stream( const It& begin, const It& end ) : List<T>(begin,end) { }
    Stream( const OddList<T> &e) : List<T>(e) { }

    // I am not sure if these are needed.
    operator List<T>() { return List<T>(*this); }
    operator OddList<T>() { return OddList<T>(*this); }

    // This works on a Stream taken as a List.
    // I wanted to call it delay, but that is used in List<T>.
    // The optional argument is inserted at the start of the list.
    Stream<T> hold_back(const T& t = T(0)) const {
      List<T> temp = (*this).delay();
      return cons(t,temp);
    }

    Stream<T> sum(const T& t = T(0)) const {
      List<T> l = (*this).delay();
      List<T> temp = list_with(t);
      temp = snoc(foldl(plus,t,l),temp);
      return temp;
    }


    Stream<T> operator += (const Stream<T> &s)
    {
      *this = *this + s ; return *this;
    }
    Stream<T> operator -= (const Stream<T> &s)
    {
      *this = *this - s; return *this;
    }
    Stream<T> operator *= (const Stream<T> &s)
    {
       *this = *this * s; return *this;
    }
    Stream<T> operator /= (const Stream<T> &s)
    {
       *this = *this / s; return *this;
    }
    Stream<T> operator += (const T &t)
    {
       *this = *this + t; return *this;
    }
    Stream<T> operator -= (const T &t)
    {
       *this = *this - t; return *this;
    }
    Stream<T> operator *= (const T &t)
    {
       *this = *this * t; return *this;
    }
    Stream<T> operator /= (const T &t)
    {
       *this = *this / t; return *this;
    }

#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp = std::string("Stream");
     temp += impl::List<T>::seq_string();
     return temp;
   }
#endif
  };

  namespace impl {
    // For the moment some internals are List<T> instead of Stream<T>.
    // This needs some further work.
    // I will leave it as it is for now.
    // The plan is for StreamM to work with Stream<type> and become
    // inferrable.
    // This provides the basis for the operators in Stream<type>
    // relating two streams.
    // This needs to be looked at for list types.
    struct XTwoStreams {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("TwoStreams");
       }
#endif
#ifdef FCPP_USE_PROMOTE
      template <class F, class S, class T>
      struct Sig : public FunType<F,S,T,
       OddList<typename RT<F,typename S::ElementType,
                          typename T::ElementType>::ResultType> > {};
      template <class F, class S, class T>
       typename Sig<F,S,T>::ResultType
       operator()( const F& f, const S& s1,const T& s2,
		   Reuser3<Inv,Inv,Var,Var,XTwoStreams,F,
              List<typename S::ElementType>,List<typename T::ElementType> >
               r = NIL ) const
       {
          return cons( f(head(s1),head(s2)),
                r( XTwoStreams(), f, tail(s1), tail(s2) ) );
       }
#else
      template <class F, class S, class T> struct Sig;
#endif
      template <class F, class S>
      struct Sig<F,S,S> : public FunType<F,S,S,
       OddList<typename RT<F,typename S::ElementType,
                          typename S::ElementType>::ResultType> > {};
       template <class F, class S>
       typename Sig<F,S,S>::ResultType
       operator()( const F& f, const S& s1,const S& s2,
		   Reuser3<Inv,Inv,Var,Var,XTwoStreams,F,
              List<typename S::ElementType>,List<typename S::ElementType> >
               r = NIL ) const
       {
	 List<typename S::ElementType> temp = cons( f(head(s1),head(s2)),
                r( XTwoStreams(), f, tail(s1), tail(s2) ) );
         return temp;
       }

    };

  }
  typedef Full3<impl::XTwoStreams> TwoStreams;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN TwoStreams twoStreams;
  FCPP_MAYBE_NAMESPACE_CLOSE


  namespace impl {
    struct XOpStream {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("OpStream");
       }
#endif
      template <class F, class S> struct Sig : public FunType<F,S,
       OddList<typename RT<F,typename S::ElementType>::ResultType> > {};
       template <class F, class S>
       typename Sig<F,S>::ResultType
       operator()( const F& f, const S& s1,
		   Reuser2<Inv,Inv,Var,XOpStream,F,
              List<typename S::ElementType> >
               r = NIL ) const
       {
	 List<typename S::ElementType> temp = cons( f(head(s1)),
                r( XOpStream(), f, tail(s1) ) );
         return temp;
       }

    };

  }
  typedef Full2<impl::XOpStream> OpStream;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN OpStream opStream;
  FCPP_MAYBE_NAMESPACE_CLOSE


  // New functoid to do an infinite geometric series
  // Needs attention to list types
  namespace impl {
    struct XGeomS {

       template <class A, class R>
       struct Sig : public FunType<Multiplies,A,R,
		    List<typename RT<Multiplies,A,R>::ResultType> > {};

       template <class A, class R>
       typename Sig<A,R>::ResultType
       operator()( const A& a, const R& rr,
		   Reuser2<Inv,Var,Inv,XGeomS,A,R >
               r = NIL ) const {
	 return cons( a, r( XGeomS(), multiplies(a,rr), rr ) );
       }
    };

    struct XGeom {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Geom");
       }
#endif
       template <class A, class R>
       struct Sig : public FunType<Multiplies,A,R,
		    OddList<typename RT<Multiplies,A,R>::ResultType> > {};

       template <class A, class R>
       typename Sig<A,R>::ResultType
       operator()( const A& a, const R& rr,
		   Reuser2<Inv,Var,Inv,XGeom,A,R >
               r = NIL ) const {
	 return cons( a, r( XGeom(), multiplies(a,rr), rr ) );
       }
    };


  // New functoid to do an infinite arithmetic series
    // List types here too.
    struct XArithS {

       template <class A, class R>
       struct Sig : public FunType<Plus,A,R,
		    List<typename RT<Plus,A,R>::ResultType> > {};

       template <class A, class R>
       typename Sig<A,R>::ResultType
       operator()( const A& a, const R& rr,
		   Reuser2<Inv,Var,Inv,XArithS,A,R >
               r = NIL ) const {
	 return cons( a, r( XArithS(), plus(a,rr), rr ) );
       }
    };

    struct XArith {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Arith");
       }
#endif

       template <class A, class R>
       struct Sig : public FunType<Plus,A,R,
		    OddList<typename RT<Plus,A,R>::ResultType> > {};

       template <class A, class R>
       typename Sig<A,R>::ResultType
       operator()( const A& a, const R& rr,
		   Reuser2<Inv,Var,Inv,XArith,A,R >
               r = NIL ) const {
	 return cons( a, r( XArith(), plus(a,rr), rr ) );
       }
    };

  }
  typedef Full2<impl::XGeomS> GeomS;
  typedef Full2<impl::XArithS> ArithS;
  typedef Full2<impl::XGeom> Geom;
  typedef Full2<impl::XArith> Arith;
  FCPP_MAYBE_NAMESPACE_OPEN
  FCPP_MAYBE_EXTERN GeomS geomS;
  FCPP_MAYBE_EXTERN ArithS arithS;
  FCPP_MAYBE_EXTERN Geom geom;
  FCPP_MAYBE_EXTERN Arith arith;
  FCPP_MAYBE_NAMESPACE_CLOSE


#ifdef FCPP_ENABLE_LAMBDA
    // I have modified StreamM to make the action of Bind is the same
    // as for ListM. This is experimental and I am still checking it out.
    // Unit is different as it constructs a unending stream
    // from its argument, while the ListM unit constructs a -> [a]
  struct StreamM {
   template <class A> struct Rep { typedef Stream<A> Type; };
   template <class MA> struct UnRep { typedef typename MA::ElementType Type; };

#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("StreamM");
   }
#endif

    // List types and use repeat instead of repeatS
   struct XUnit {
#ifdef FCPP_DEBUG
      std::string name() const
      {
         return std::string("Unit");
      }
#endif
      template <class A> struct Sig : public FunType<A,OddList<A> > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
	return repeat(x);
      }
   };
   typedef Full1<XUnit> Unit;
   static Unit& unit() {static Unit f; return f;}
   static Unit& return_() {static Unit f; return f;}

   struct XBind {
     // This still needs the old version of Diag, now called DiagS
     // This needs further work to resolve the list types.
      template <class M, class K> struct Sig : public FunType<M,K,
         typename RT<DiagRep,typename RT<MapS,K,M>::ResultType>::ResultType> {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
	return diagRep( mapS(k,m) );
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}

  };

template <class T> struct MonadTraitsSpecializer<Stream<T> > {
   typedef StreamM Monad;
};

//////////////////////////////////////////////////////////////////////
// ParallelM monad from category_fcpp.cpp
// Monad working on pairs.
//////////////////////////////////////////////////////////////////////

  // This is so that ParallelM can be inferrable.
  // At the moment the only use of this is in ParallelM::Unrep<P>
  template <class A, class B>
  class Pair {
    A a_;
    B b_;
  public:
    typedef std::pair<A,B> ElementType;
    Pair() {}
    Pair(const A& a, const B& b) : a_(a), b_(b) {}
    std::pair<A,B> operator()() const { return std::make_pair(a_,b_); }
  };

struct ParallelM {
  template <class A, class B>  struct Rep { typedef std::pair<A,B> Type; };
  template <class P> struct UnRep {
    // This makes the Pair::ElementType from the std::pair!!
    typedef typename Pair<typename P::first_type,typename P::second_type>::ElementType Type;
  };

#ifdef FCPP_DEBUG
   std::string name() const
   {
       return std::string("ParallelM");
   }
#endif
  // Copied from IdentityM - unit is exactly the same.
   struct XUnit {
     template <class P> struct SigHelp {
        typedef typename Pair<typename P::first_type,
                        typename P::second_type>::ElementType ResultType;
        typedef typename Pair<typename P::first_type,
                        typename P::second_type>::ElementType Arg1Type;
     };
      template <class A> struct Sig {
         typedef typename SigHelp<A>::ResultType ResultType;
         typedef typename SigHelp<A>::Arg1Type Arg1Type;
      };
      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a ) const { return id(a); }
   };
   typedef Full1<XUnit> Unit;
    static Unit& unit() {static Unit f; return f;}

   struct XBind {
      template <class M, class K> struct Sig : public FunType<M,K,
		typename RT<Parallel,K,M>::ResultType>  {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
	return parallel(k,m);
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}

};

  template <class A, class B>
struct MonadTraitsSpecializer<std::pair<A,B> > {
   typedef ParallelM Monad;
};

//////////////////////////////////////////////////////////////////////
// Monad Transformer MaybeT puts Maybe on the inside.
// Usage is MaybeT<Monad>
//////////////////////////////////////////////////////////////////////

template <typename Monad>
struct MaybeT
{

   template <class A> struct MRep {
     typedef typename Monad::template Rep<A>::Type Type;
   };

   template <class A>  struct Rep   {
     typedef typename MRep<Maybe<typename A::ElementType > >::Type Type;
   };

  // This gets back Maybe<A> from the transformed monad.
  // and then gets back the type from that.
  // ElementType defines the lowest type.
  // Type defines the original monad type.
   template <class MA> struct UnRep {
     typedef typename Monad::template UnRep<MA>::Type MType;
     typedef typename MType::ElementType ElementType;
     typedef typename MRep<ElementType>::Type Type;
   };

  typedef MaybeM OuterType; // Experiments for LiftT
  typedef Monad  InnerType;

  // template <class MA> struct MUnRep {
  //   typedef typename Monad::template UnRep<MA>::Type Type;
  // };
  // This may be wrong.
  //template <class MA> struct UnRep { typedef typename MUnRep<MA>::Type Type; };

#ifdef FCPP_DEBUG
   std::string name() const
   {
     if (fcpp::traits::debug_traits<Monad>::has_name)
       {
         Monad m; // Need to create an object for the call.
	 std::string temp = std::string("MaybeT<")
            + fcpp::traits::debug_traits<Monad>::name(m)
	    + std::string("> ");
         return temp;
       } else
     return std::string("MaybeT<Monad>");
   }
#endif
 
   struct XUnit {
#ifdef FCPP_DEBUG
      std::string name(const std::string& monad) const
      {
         return std::string("MaybeT<") + monad + std::string(">::Unit");
      }
#endif
   template <class A> struct Sig : public FunType<A,typename Rep<A>::Type > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
	return unitM<Monad>()(x);
      }
   };
   typedef Full1<XUnit> Unit;
   static Unit& unit() {static Unit f; return f;}

   struct XBind {
#ifdef FCPP_DEBUG
      std::string name(const std::string& monad) const
      {
	return std::string("MaybeT<") + monad + std::string(">::Bind");
      }
#endif
     template <class A, class K> struct Sig : public FunType<A,K,A> {};
     template <class A,class K>
      typename Sig<A,K>::ResultType
     //operator()( const typename Rep<A>::Type& m, const K& k ) const {
     operator()( const A& m, const K& k ) const {
       // This needs some elaboration to deal with new
       // failing cases generated by the operation of k.
       return liftM<Monad>()(liftM<MaybeM>()(k))(m);
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}

  struct XBindT {
#ifdef FCPP_DEBUG
      std::string name(const std::string& monad) const
      {
	return std::string("MaybeT<") + monad + std::string(">::BindT");
      }
#endif
     template <class A, class K> struct Sig : public FunType<A,K,A> {};
     template <class A,class K>
      typename Sig<A,K>::ResultType
     //operator()( const typename Rep<A>::Type& m, const K& k ) const {
     operator()( const A& m, const K& k ) const {
       // This needs some elaboration to deal with new
       // failing cases generated by the operation of k.
       return liftM<Monad>()(k)(m);
      }
   };
   typedef Full2<XBindT> BindT;
   static BindT& bindT() {static BindT f; return f;}

};

// template template parameter needed, see C++ Templates p.50
template <template < class ELEM> class M, class T>
 struct MonadTraitsSpecializer<MaybeT<M<T> > > {
  typedef MaybeT<M<T> > Monad;
};

//////////////////////////////////////////////////////////////////////
// EitherT monad transformer following MaybeT
//////////////////////////////////////////////////////////////////////

template <typename Monad>
struct EitherT {
   template <class A> struct ERep {
     typedef typename Monad::template Rep<A>::Type Type;
   };

   template <class A>  struct Rep   {
     typedef typename ERep<Either<typename A::ElementType > >::Type Type;
   };

  // This gets back Either<A> from the transformed monad.
  // and then gets back the type from that.
  // ElementType defines the lowest type.
  // Type defines the original monad type.
   template <class EA> struct UnRep {
     typedef typename Monad::template UnRep<EA>::Type EType;
     typedef typename EType::ElementType ElementType;
     typedef typename ERep<ElementType>::Type Type;
   };

  typedef EitherM OuterType; // Experiments for LiftT
  typedef Monad   InnerType;

  // template <class EA> struct MUnRep {
  //   typedef typename Monad::template UnRep<EA>::Type Type;
  // };
  // This may be wrong.
//template <class EA> struct UnRep { typedef typename MUnRep<EA>::Type Type; };

#ifdef FCPP_DEBUG
   std::string name() const
   {
     if (fcpp::traits::debug_traits<Monad>::has_name)
       {
         Monad m; // Need to create an object for the call.
	 std::string temp = std::string("EitherT<")
            + fcpp::traits::debug_traits<Monad>::name(m)
	    + std::string("> ");
         return temp;
       } else
     return std::string("EitherT<Monad>");
   }
#endif

   struct XUnit {
#ifdef FCPP_DEBUG
      std::string name(const std::string& monad) const
      {
         return std::string("EitherT<") + monad + std::string(">::Unit");
      }
#endif
   template <class A> struct Sig : public FunType<A,typename Rep<A>::Type > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& x ) const {
	return unitM<Monad>()(x);
      }
   };
   typedef Full1<XUnit> Unit;
   static Unit& unit() {static Unit f; return f;}

   struct XBind {
#ifdef FCPP_DEBUG
      std::string name(const std::string& monad) const
      {
         return std::string("EitherT<") + monad + std::string(">::Bind");
      }
#endif
     template <class A, class K> struct Sig : public FunType<A,K,A> {};
      template <class A, class K>
      typename Sig<A,K>::ResultType
      operator()( const A& m, const K& k ) const {
       return liftM<Monad>()(liftM<EitherM>()(k))(m);
      }
   };
   typedef Full2<XBind> Bind;
   static Bind& bind() {static Bind f; return f;}
  //   static Bind bind;
  struct XBindT {
#ifdef FCPP_DEBUG
      std::string name(const std::string& monad) const
      {
         return std::string("EitherT<") + monad + std::string(">::BindT");
      }
#endif
     template <class A, class K> struct Sig : public FunType<A,K,A> {};
     template <class A,class K>
      typename Sig<A,K>::ResultType
     //operator()( const typename Rep<A>::Type& m, const K& k ) const {
     operator()( const A& m, const K& k ) const {
       // This needs some elaboration to deal with new
       // failing cases generated by the operation of k.
       return liftM<Monad>()(k)(m);
      }
   };
   typedef Full2<XBindT> BindT;
   static BindT& bindT() {static BindT f; return f;}

};

template <template < class ELEM> class M, class T>
struct MonadTraitsSpecializer<EitherT<M<T> > > {
  typedef EitherT<M<T> > Monad;
};

///////////////////////////////////////////////////////////////////////
// BindT is a near copy of BindM.
// It uses bindT instead of bindM in the monad transformer.
///////////////////////////////////////////////////////////////////////
namespace impl {
   template <class Monad>
   struct XBindT {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("BindT<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("BindT");
     }
     return temp;
   }
#endif
      template <class M, class K> struct Sig : public FunType<M,K,
         typename Monad::BindT::template Sig<M,K>::ResultType> {};
   
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         return Monad::bindT()(m,k);
	 //         return Monad::bind(m,k);
      }
   };
}
template <class Monad> Full2<impl::XBindT<Monad> > bindT()
{ return makeFull2( impl::XBindT<Monad>() ); }
template <class Monad> struct BindT
{ typedef Full2<impl::XBindT<Monad> > Type; };

#ifdef FCPP_DEBUG
    namespace traits {
    template<class Monad>
       class debug_traits<impl::XBindT<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XBindT<Monad> &f) { return f.name(); }
       };
    }
#endif

//////////////////////////////////////////////////////////////////////
// MapT and ForT for use with Monad transformers.
//////////////////////////////////////////////////////////////////////


namespace impl {
   template <class Monad>
   struct XMapT {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("MapT<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("MapT");
     }
     return temp;
   }
#endif
      template <class F, class M> struct Sig : FunType<F,M,
         typename RT<typename ::fcpp::BindM<Monad>::Type,M,typename LEType<
            LAM<LV<1>,CALL<F,LV<1> > > >::Type>::ResultType> {};
      template <class F, class M>
      typename Sig<F,M>::ResultType
      operator()( const F& f, const M& m ) const {
         LambdaVar<1> A;
         return bindM<Monad>()( m, lambda(A)[ f[A] ] );
         //return bindM<Monad>()( m, lambda(A)[ unitM<Monad>()[f[A]] ] );
      }
   };
}
template <class Monad> Full2<impl::XMapT<Monad> > mapT()
{ return makeFull2( impl::XMapT<Monad>() ); }
template <class Monad> struct MapT
{ typedef Full2<impl::XMapT<Monad> > Type; };

namespace traits {
#ifndef FCPP_ARDUINO
    template <>
      class argument_traits <Void> {
    public:
      static std::string value(Void)
      {
        return std::string("Void");
      }
    };
#endif
}

namespace impl {
   template <class Monad>
   struct XForT {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("ForT<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("ForT");
     }
     return temp;
   }
#endif
      template <class M, class F> struct Sig : FunType<M,F,
	 typename RT<typename ::fcpp::BindM<Monad>::Type,M,typename LEType<
            LAM<LV<1>,CALL<F,LV<1> > > >::Type>::ResultType> {};
      template <class M, class F>
      typename Sig<M,F>::ResultType
      operator()( const M& m, const F& f ) const {
         LambdaVar<1> A;
         return bindM<Monad>()( m, lambda(A)[ f[A] ] );
      }
   };
}
template <class Monad> Full2<impl::XForT<Monad> > forT()
{ return makeFull2( impl::XForT<Monad>() ); }
template <class Monad> struct ForT
{ typedef Full2<impl::XForT<Monad> > Type; };

///////////////////////////////////////////////////////////////////////
// The idea of liftT is that I need something different
// to work with transformed monads.
// This now does a double lift calling liftM twice.
///////////////////////////////////////////////////////////////////////
namespace impl {
template <class Monad>
struct XLiftT {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("LiftT<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("LiftT");
     }
     return temp;
   }
#endif
  // These are only defined in monad transformers
  // so this will cause a failure if it is misused.
  typedef typename Monad::OuterType OuterMonad;
  typedef typename Monad::InnerType InnerMonad;
  // The tricky bit is to get the return type from the Sigs of LiftM.
  // Note that the outer monad becomes the inner call.
  template <class F> struct Sig : public FunType<F,
    typename impl::XLiftM<InnerMonad>::template Sig<
    typename impl::XLiftM<OuterMonad>::template Sig<F>::ResultType>::ResultType
  >{};

  template <class F>
  typename Sig<F>::ResultType operator()( const F& f ) const {
    return impl::XLiftM<InnerMonad>()(impl::XLiftM<OuterMonad>()(f));
  }
};

}
template <class Monad> Full1<impl::XLiftT<Monad> > liftT()
{ return makeFull1( impl::XLiftT<Monad>() ); }
template <class Monad> struct LiftT
{ typedef Full1<impl::XLiftT<Monad> > Type; };

namespace impl {
template <class Monad>
struct XLiftT2 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("LiftT2<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("LiftT2");
     }
     return temp;
   }
#endif
  // These are only defined in monad transformers
  // so this will cause a failure if it is misused.
  typedef typename Monad::OuterType OuterMonad;
  typedef typename Monad::InnerType InnerMonad;
  // The tricky bit is to get the return type from the Sigs of LiftM.
  // Note that the outer monad becomes the inner call.
  template <class F> struct Sig : public FunType<F,
    typename impl::XLiftM2<InnerMonad>::template Sig<
    typename impl::XLiftM2<OuterMonad>::template Sig<F>::ResultType>::ResultType
  >{};

  template <class F>
  typename Sig<F>::ResultType operator()( const F& f ) const {
    return impl::XLiftM2<InnerMonad>()(impl::XLiftM2<OuterMonad>()(f));
  }
};

}
template <class Monad> Full1<impl::XLiftT2<Monad> > liftT2()
{ return makeFull1( impl::XLiftT2<Monad>() ); }
template <class Monad> struct LiftT2
{ typedef Full1<impl::XLiftT2<Monad> > Type; };

namespace impl {
template <class Monad>
struct XLiftT3 {
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     if (fcpp::traits::debug_traits<Monad>::has_name) {
       Monad m; // Need to create an object for the call.
       temp = std::string("LiftT3<");
       temp += fcpp::traits::debug_traits<Monad >::name(m);
       temp += std::string(">()");
     } else {
       temp = std::string("LiftT3");
     }
     return temp;
   }
#endif
  // These are only defined in monad transformers
  // so this will cause a failure if it is misused.
  typedef typename Monad::OuterType OuterMonad;
  typedef typename Monad::InnerType InnerMonad;
  // The tricky bit is to get the return type from the Sigs of LiftM.
  // Note that the outer monad becomes the inner call.
  template <class F> struct Sig : public FunType<F,
    typename impl::XLiftM3<InnerMonad>::template Sig<
    typename impl::XLiftM3<OuterMonad>::template Sig<F>::ResultType>::ResultType
  >{};

  template <class F>
  typename Sig<F>::ResultType operator()( const F& f ) const {
    return impl::XLiftM3<InnerMonad>()(impl::XLiftM3<OuterMonad>()(f));
  }
};

}
template <class Monad> Full1<impl::XLiftT3<Monad> > liftT3()
{ return makeFull1( impl::XLiftT3<Monad>() ); }
template <class Monad> struct LiftT3
{ typedef Full1<impl::XLiftT3<Monad> > Type; };

  //////////////////////////////////////////////////////////////////
  // unit for inferrable monads.
  // This provides the missing Unit/unit for inferrable monads.
  //////////////////////////////////////////////////////////////////
  // This is a bit of a fudge for the following reason.
  // The monad operation UnitM<monad> will take a value and return the
  // correct wrapped value for the monad type
  //  e.g. a -> [a] for List<T>.
  //
  // The unit has no way to deduce the type of the monad except from the
  // type of the value it is given, so it has to be given a wrapped value.
  // This has to be a value of the correct type or it cannot deduce
  // what type to make. It unwraps it and then takes appropriate action.
  // In some cases that simply wraps it again, which means this does not
  // do a lot, but may be useful in some situations.
  //
  // I have defined functions called return_ for each type which do the
  // correct analysis. For each type it returns a wrapped value of the
  // correct type for a valid value. For an invalid value it throws
  // an fcpp_exception with an approptiate message.
  //
  // So it does [a] -> [head(a)] for List<T> which is not
  // useful although it does work. If the list is NUL it throws.
  //
  // For Stream<T> it is more useful as unit generates an infinite Stream
  // from a single value. This works also.
  //
  // The operation of unit  is similar to bind but needs only one argument,
  // so that the comparison of the monad types is not needed.
  // Tests of the different types are in understand_list.cpp
  //
  ////////////////////////////////////////////////////////////////////////
  // NOTE: There is no default behaviour for unit(x) with a simple type.
  //       This is in contrast to pure(x) where it is the ONLY behaviour.
  //       I want to integrate these two things which are quite different.
  ////////////////////////////////////////////////////////////////////////

namespace impl {
   template <bool b> struct UnitError {};
   template <> struct UnitError<true> { static inline void error() {} };
   template <class M>
   struct UnitHelper {
      typedef typename MonadTraits<M>::Monad MonadA;
      typedef typename MonadA::template UnRep<M>::Type A;
      typedef typename MonadA::Unit::template Sig<M>::ResultType ResultType;
   };

   template <class T>
   T return_(const Stream<T> &t) {
#ifdef FCPP_USE_EXCEPTIONS	   
     if (null(t) )  throw fcpp_exception("unit: Stream : NULL");
#endif
     return head(t);
   }

   template <class T>
   T return_(const List<T> &t) {
#ifdef FCPP_USE_EXCEPTIONS	   
     if (null(t) )  throw fcpp_exception("unit: List : NULL");
#endif		 
     return head(t);
   }

   template <class T>
   T return_(const BootList<T> &t) {
#ifdef FCPP_USE_EXCEPTIONS	   
     if (t.is_empty())  throw fcpp_exception("unit: BootList : NULL");
#endif	 
     return t.front();
   }
  
   template <class T>
   T return_(const Either<T> &t) {
#ifdef FCPP_USE_EXCEPTIONS	   
     if (t.is_error()) throw fcpp_exception(std::string("unit: Either : ")+
                             t.left());
#endif		 
     return t.right();
   }

   template <class T>
   T return_(const Maybe<T> &t) {
#ifdef FCPP_USE_EXCEPTIONS	   
     if (t.is_nothing()) throw fcpp_exception("unit: Maybe : NOTHING");
#endif
     return t.value();
   }

   template <class T>
   T return_(const Identity<T> &t) { return id(t); }

   template <class A,class B>
     std::pair<A,B> return_(const std::pair<A,B>& p) { return p; }

   struct XUnit {
      template <class M> struct Sig
	: public FunType<M,typename UnitHelper<M>::ResultType::ElementType> {};

      template <class M>
      typename Sig<M>::ResultType
      operator()( const M& m) const {
         typedef typename UnitHelper<M>::MonadA MM;
	 // There is an appropriate return_ for each inferrable monad type.
         return MM::unit()(return_(m));
      }
   };
}
typedef Full1<impl::XUnit> Unit;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Unit unit;
FCPP_MAYBE_NAMESPACE_CLOSE

// This specialisation fools bind into thinking StreamM and ListM are the same.
// This is a work around of the problem in bind with StreamM.
// It works because Stream<T> inherits from List<T>.
// The code being used is in fcpp/signature.h
template <> struct Conversion<StreamM,ListM >
{
  static const bool value = true;
  static const bool sameType = true;
};


 template <class F,class T>
   Stream<T> funAX(const F& f, const T& a, const Stream<T> &x)
   {
    LambdaVar<1> X;
    return lambda(X) [bindM<StreamM>()
	       [ X , f(a) ] ](x);
   }

// Operators for class Stream<T> which use lambda constructs.
  template <class T>
  Stream<T> operator+(const Stream<T>& x, const T& t)
  {
    LambdaVar<1> X;
    //LambdaVar<2> Y;
    return lambda(X) [bindM<StreamM>()
		      [ X , plus(_,t) ] ](x);
  }


  template <class T>
  Stream<T> operator-(const Stream<T>& x, const T& t)
  {
    LambdaVar<1> X;
    //LambdaVar<2> Y;
    return lambda(X) [bindM<StreamM>()
		      [ X , minus(_,t) ] ](x);
  }

  template <class T>
  Stream<T> operator*(const Stream<T>& x, const T& t)
  {
    LambdaVar<1> X;
    //LambdaVar<2> Y;
    return lambda(X) [bindM<StreamM>()
		      [ X , multiplies(_,t) ] ](x);
  }

  template <class T>
  Stream<T> operator/(const Stream<T>& x, const T& t)
  {
    LambdaVar<1> X;
    //LambdaVar<2> Y;
    return lambda(X) [bindM<StreamM>()
		      [ X , divides(_,t) ] ](x);
  }


  template <class T>
  Stream<T> operator+(const T& t, const Stream<T>& x)
  {
    LambdaVar<1> X;
    //LambdaVar<2> Y;
    return lambda(X) [bindM<StreamM>()
		      [ X , plus(t) ] ](x);
  }

  template <class T>
  Stream<T> operator-(const T& t, const Stream<T>& x)
  {
    LambdaVar<1> X;
    //LambdaVar<2> Y;
    return lambda(X) [bindM<StreamM>()
		      [ X , minus(t) ] ](x);
  }

  template <class T>
  Stream<T> operator*(const T& t, const Stream<T>& x)
  {
    LambdaVar<1> X;
    //LambdaVar<2> Y;
    return lambda(X) [bindM<StreamM>()
		      [ X , multiplies(t) ] ](x);
  }

  template <class T>
  Stream<T> operator/(const T& t, const Stream<T>& x)
  {
    LambdaVar<1> X;
    //LambdaVar<2> Y;
    return lambda(X) [bindM<StreamM>()
		      [ X , divides(t) ] ](x);
  }

#endif //#ifdef FCPP_ENABLE_LAMBDA
  // The remaining Stream<T> operators do not use lambda.

  template <class T>
  Stream<T> operator+(const Stream<T>& x, const Stream<T>& y)
  {
      return twoStreams(plus,x,y);
  }

  template <class T>
  Stream<T> operator-(const Stream<T>& x, const Stream<T>& y)
  {
      return twoStreams(minus,x,y);
  }

  template <class T>
  Stream<T> operator*(const Stream<T>& x, const Stream<T>& y)
  {
      return twoStreams(multiplies,x,y);
  }

  template <class T>
  Stream<T> operator/(const Stream<T>& x, const Stream<T>& y)
  {
      return twoStreams(divides,x,y);
  }

  // Operators on two different stream types.
  template <class T, class U>
  Stream<T> operator+(const Stream<T>& x, const Stream<U>& y)
  {
      return twoStreams(plus,x,y);
  }

  template <class T, class U>
  Stream<T> operator-(const Stream<T>& x, const Stream<U>& y)
  {
    return twoStreams(minus,x,y);
  }

  template <class T, class U>
  Stream<T> operator*(const Stream<T>& x, const Stream<U>& y)
  {
      return twoStreams(multiplies,x,y);
  }

  template <class T, class U>
  Stream<T> operator/(const Stream<T>& x, const Stream<U>& y)
  {
    return twoStreams(divides,x,y);
  }

  // Difference, first step to doing differential arithmetic.
  template <class T>
  Stream<T> diff(const Stream<T> &s)
  {
    return s - s.hold_back();
  }
  // Central difference
  template <class T>
  Stream<T> central_diff(const Stream<T> &s)
  {
    Stream<T> temp = diff(s);
    return temp + temp.hold_back();
  }
  // Second difference
  template <class T>
  Stream<T> diff2(const Stream<T> &s)
  {
    Stream<T> temp = diff(s);
    return temp - temp.hold_back();
  }

}

// Includes argument traits needed without FCPP_DEBUG
#include "pattern_debug.h"

#endif
