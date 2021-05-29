//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_LIST_DOT_H
#define FCPP_LIST_DOT_H

///////////////////////////////////////////////////////////////////////////
// Here we implement (lazy) lists in the List class.  There are also a
// number of functions associated with lists:
//  - head, tail, cons, cat, null
///////////////////////////////////////////////////////////////////////////

// Order-of-initialization debugging help
// Note that you only might need this with the FCPP_1_3_LIST_IMPL version
#ifdef FCPP_OOI_DEBUG
#include <iostream>
#include <typeinfo>
#endif

#include <string>
#include <exception>
#include <new>
#include <cstdlib>

// Moved here to make it available to operator.hpp
namespace fcpp {
struct fcpp_exception : public std::exception {
   const char* s;
   fcpp_exception( const char* ss ) : s(ss) {}
   fcpp_exception( const std::string &str ) : s(str.c_str()) {}
   const char* what() const throw() { return s; }
};
}

#include "reuse.h"

//#ifdef FCPP_IMPLICIT_COPY_CONSTRUCTOR_SOLUTION
#include "detect_pair.h"
//#endif

namespace fcpp {

  // Defined for use in checking for exceptions
  // e.g. length or last of non terminating List<T>.
#define MAX_LIST_LENGTH 100000

struct ListLike {};   // This lets us use is_base_and_derived() to see
   // (at compile-time) what classes are user-defined lists.
//template <class T> class StrictList; leave this for now.

namespace impl {
struct XCons; struct XHead; struct XTail; struct XNull; struct XCat;

struct CacheEmpty {};
struct CacheDummy {};

template <class T> struct Cache;
template <class T> struct OddList;
template <class T> struct ListIterator;
template <class T, class It> struct ListItHelp;
template <class U,class F> struct cvt;
template <class T, class F, class R> struct ListHelp;
template <class T> Cache<T>* xempty_helper();
template <class T, class F, class R> struct ConsHelp;
template <class F, class T> struct Snoc2Help;

struct ListRaw {};

template <class T>
class List  : public ListLike {
   IRef<Cache<T> > rep;   // never NIL, unless an empty OddList

   template <class U> friend class Cache;
   template <class U> friend class OddList;
   template <class U, class F, class R> friend struct ConsHelp;
   template <class U,class F> friend struct cvt;
   template <class F, class U> friend struct Snoc2Help;

   List( const IRef<Cache<T> >& p ) : rep(p) {}
   List( ListRaw, Cache<T>* p ) : rep(p) {}

   bool priv_isEmpty() const { 
      return rep->cache().second.rep == Cache<T>::XNIL(); 
   }
   T priv_head() const { 
#ifdef FCPP_DEBUG
      if( priv_isEmpty() )
         throw fcpp_exception("Tried to take head() of empty List");
#endif
#ifdef FCPP_IMPLICIT_COPY_CONSTRUCTOR_SOLUTION
      //if (is_integral<T>())
      // return rep->cache().first(); 
      //else if (is_pair<T>()) {
       return std::move(rep->cache().first());
       //}
#else
      return rep->cache().first(); 
#endif
   }
   List<T> priv_tail() const { 
#ifdef FCPP_DEBUG
      if( priv_isEmpty() )
         throw fcpp_exception("Tried to take tail() of empty List");
#endif
      return rep->cache().second; 
   }
public:
   typedef T ElementType;

   List( AUniqueTypeForNil ) : rep( Cache<T>::XEMPTY() ) {}
   List() : rep( Cache<T>::XEMPTY() ) {}

   template <class F>  // works on both ()->OddList and ()->List 
   List( const F& f )
   : rep( ListHelp<T,F,typename F::ResultType>()(f) ) {}

   // Note:  this constructor is still part of List and thus still lazy;
   // the iterators may not get evaluated until much later.  This is a
   // feature, not a bug.  So if the iterators are going to be invalidated
   // before you finish using the list, then you'd better force evaluation 
   // of the entire list before the iterators go away.
   template <class It>
   List( const It& begin, const It& end )
   : rep( new Cache<T>( ListItHelp<T,It>(begin,end) ) ) {}

   List( const OddList<T>& e )
   : rep( (e.second.rep != Cache<T>::XNIL()) ? 
          new Cache<T>(e) : Cache<T>::XEMPTY() ) 
   {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "List<T> constructor from OddList<T> called." << std::endl;
#endif
   }

#ifdef FCPP_SAFE_LIST
   // Long lists create long recursions of destructors that blow the
   // stack.  So we have an iterative destructor.  It is quite tricky to
   // get right.  The danger is that, when "bypassing" a node to be
   // unlinked and destructed, that node's 'next' pointer is, in fact, a
   // List object, whose destructor will be called.  As a result, as you
   // bypass a node, you need to see if its refC is down to 1, and if
   // so, mutate its next pointer so that when its destructor is called,
   // it won't cause a recursive cascade.  
   ~List() {
     while (rep->refC==1&&rep!=Cache<T>::XBAD()&&rep!=Cache<T>::XNIL()) {
       rep=rep->val.second.rep;
     }
//       while( rep != Cache<T>::XNIL() && rep != Cache<T>::XBAD() ) {
//          if( rep->refC == 1 ) {
//             // This is a rotate(), but this sequence is actually faster
//             // than rotate(), so we do it explicitly
//             IRef<Cache<T> > tmp( rep );
//             rep = rep->val.second.rep;
//             tmp->val.second.rep = Cache<T>::XNIL();
//          }
//          else
//             rep = rep->val.second.rep;
//       }
   }
#endif

   operator bool() const { return !priv_isEmpty(); }
   const OddList<T>& force() const { return rep->cache(); }
   const List<T>& delay() const { return *this; }
   // Note: force returns a reference; implicit conversion now returns a copy.
   operator OddList<T>() const { 
#ifdef FCPP_PRINT_HELPERS
     std::cout << "conversion called " << std::endl;
#endif
     return force(); 
   }

   // VC++7.1 says line below makes "return l;" (when l is a List and
   // function returns an OddList) illegal, and I think it's right.
   //operator const OddList<T>&() const { return force(); }

   T head() const { return priv_head(); }
   List<T> tail() const { return priv_tail(); }

   // The following helps makes List almost an STL "container"
   typedef T value_type;
   typedef ListIterator<T> const_iterator;
   typedef const_iterator iterator;         // List is immutable
   iterator begin() const { return ListIterator<T>( *this ); }
   iterator end() const   { return ListIterator<T>(); }
   /* These functions return the actual states needed
      without performing any evaluation.
      That is what is needed here.
      This is done by checking using the Cache pointer. */
   bool is_empty()  const {  return rep->is_empty(); }
   bool has_value() const
      { return !is_empty() && rep->has_value(); }
   std::string seq_string() const; // Now located at the end of the class.
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp = std::string("List");
     temp += seq_string();
     return temp;
   }
#endif
};

#ifdef FCPP_DEBUG
   template <class T>
     std::string List<T>::seq_string() const {
     std::string temp;
     const std::string string_F("F");
     const std::string string_open("[ "),string_close(" ] ");
     const std::string string_open_close("[ ]"),string_comma(" , ");
     if( is_empty() ) { // Progress - empty list case.
         temp +=  string_open_close;
     } else {
       if (has_value()) { // This does not force evaluation.
          temp += string_open;
          temp += debug_name_or_value(head());
	  List<T> ll = tail();
          // Print values if there are any
          while (ll.has_value()) {
	    temp += string_comma;
            temp += debug_name_or_value(ll.head());
            ll = ll.tail();
          }
	  // Print this if there is still a function.
          if (!ll.is_empty() )
	    {
	      temp += string_comma;
              temp += debug_name_or_string(ll.rep->fxn,string_F);
	    }
          temp += string_close; // Finish off here
       }  else {
         temp += string_open;
         temp += debug_name_or_string(rep->fxn,string_F);
         temp += string_close;
      }
     }
     return temp;
     }

#else // !FCPP_DEBUG
   template <class T>
     std::string List<T>::seq_string() const {
     std::string temp;
     const std::string string_F("F");
     const std::string string_open("[ "),string_close(" ] ");
     const std::string string_open_close("[ ]"),string_comma(" , ");
     const std::string string_open_dash_close("[ _ ]"),
                       string_comma_dash(" ,  _");
     if( is_empty() ) { // Progress - empty list case.
         temp += string_open_close;
     } else {
       if (has_value()) { // This does not force evaluation.
	  temp += string_open;
          temp += argument_traits_value(head());
	  List<T> ll = tail();
          // Print values if there are any
          while (ll.has_value()) {
	    temp += string_comma;
	    temp += argument_traits_value(ll.head());
            ll = ll.tail();
          }
          if (!ll.is_empty() )
	      temp +=  string_comma_dash;
          temp += string_close; // Finish off here
       }  else {
	  temp += string_open_dash_close; // Unevaluated function.
       }
     }
     return temp;
     }
#endif // FCPP_DEBUG

struct OddListDummyX {};
struct OddListDummyY {};

namespace misc_types {
   struct Argh { virtual int f() {return 0;} };
   typedef int (*PtrToFxn)();
   typedef int (Argh::*PtrToMember);
   typedef int (Argh::*PtrToMemberFxn)();
}

template <class T>
class OddList  : public ListLike {
   // We need to make sure that "fst" is properly aligned to hold a "T"
   // object, so we do the 'standard' hack.
#ifdef FCPP_IMPLICIT_COPY_CONSTRUCTOR_SOLUTION
   T fst;
   bool is_pair = fcpp::traits::pair_traits<T>::is_pair;
#else
   union {
      unsigned char fst[ sizeof(T) ];   // The real variable
      // a bunch of dummies of every conceivable type
      long z1, *pz1; long double z2, *pz2; void *z3, **pz3; 
      misc_types::PtrToFxn z4, *pz4;
      misc_types::Argh *pz5;
      int z6, *pz6; char z7, *pz7; double z8, *pz8; 
      misc_types::PtrToMember z9, *pz9;
      misc_types::PtrToMemberFxn z10, *pz10;
   };
#endif
#ifdef FCPP_IMPLICIT_COPY_CONSTRUCTOR_SOLUTION
   const T& first() const { 
     if (is_integral<T>())
      return *static_cast<const T*>(static_cast<const void*>(&fst)); 
     else if (is_pair) {
       return std::make_pair(*static_cast<T*>(static_cast<void*>(&fst)).first,
			     *static_cast<T*>(static_cast<void*>(&fst)).second);
     }
   }
   T& first() {
     if (is_integral<T>())
      return *static_cast<T*>(static_cast<void*>(&fst));
     else if (is_pair) {
       return std::make_pair(*static_cast<T*>(static_cast<void*>(&fst)).first,
			     *static_cast<T*>(static_cast<void*>(&fst)).second);
     }
   }
#else
   const T& first() const { 
      return *static_cast<const T*>(static_cast<const void*>(&fst)); 
   }
   T& first() { 
      return *static_cast<T*>(static_cast<void*>(&fst));
   }
#endif
   List<T>  second;   // If XNIL, then this OddList is NIL

   template <class U> friend class List;
   template <class U> friend class Cache;

   OddList( OddListDummyX ) : second( Cache<T>::XNIL() ) { }

   OddList( OddListDummyY )
   : second( Cache<T>::XBAD() ) { }

   void init( const T& x ) {
#ifdef FCPP_IMPLICIT_COPY_CONSTRUCTOR_SOLUTION
     new (static_cast<void*>(&fst)) T(std::move(x));
#else
      new (static_cast<void*>(&fst)) T(x);
#endif
   } 

   bool fst_is_valid() const {
      if( second.rep != Cache<T>::XNIL() )
         if( second.rep != Cache<T>::XBAD() )
            return true;
      return false;
   }

   bool priv_isEmpty() const { return second.rep == Cache<T>::XNIL(); }
   T priv_head() const { 
#ifdef FCPP_DEBUG
      if( priv_isEmpty() )
         throw fcpp_exception("Tried to take head() of empty OddList");
#endif
#ifdef FCPP_IMPLICIT_COPY_CONSTRUCTOR_SOLUTION
        if (fcpp::traits::pair_traits<T>::is_pair) 
	else return first();
#else
      return first();
#endif
   }
   List<T> priv_tail() const { 
#ifdef FCPP_DEBUG
      if( priv_isEmpty() )
         throw fcpp_exception("Tried to take tail() of empty OddList");
#endif
      return second; 
   }

public:
   typedef T ElementType;
   //struct Sig : public FunType<T> { } // experimental
   OddList() : second( Cache<T>::XNIL() ) { }
   OddList( AUniqueTypeForNil ) : second( Cache<T>::XNIL() ) { }
   OddList( const T& x, const List<T>& y ) : second(y) { init(x); }
   OddList( const T& x, AUniqueTypeForNil ) 
   : second(Cache<T>::XEMPTY()) { init(x); }

   OddList( const OddList<T>& x ) : second(x.second) {
     if( fst_is_valid() ) { // Why not check x.fst_is_valid() as well?
         init( x.first() );
      }
   }

   OddList<T>& operator=( const OddList<T>& x ) {
      if( this == &x ) return *this;  
      if( fst_is_valid() ) {
         if( x.fst_is_valid() )
#ifdef FCPP_IMPLICIT_COPY_CONSTRUCTOR_SOLUTION
            first() = std::move(x.first());
#else
            first() = x.first();
#endif
         else
            first().~T();
      }
      else {
         if( x.fst_is_valid() )
            init( x.first() );
      }
      second = x.second;
      return *this;
   }

   ~OddList() {
      if( fst_is_valid() ) {
         first().~T();
      }
   }

   operator bool() const { return !priv_isEmpty(); }
   const OddList<T>& force() const { return *this; }
   List<T> delay() const { return List<T>(*this); }

   T head() const { return priv_head(); }
   List<T> tail() const { return priv_tail(); }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp = std::string("OddList");
     temp += delay().seq_string();
     return temp;
   }
#endif
   // Put in to fix a bug.
   std::string seq_string() const
     {
       return delay().seq_string();// std::string("OddList");
     }

};

// This converts ()->List<T> to ()->OddList<T>.
// In other words, here is the 'extra work' done when using the
// unoptimized interface.
template <class U,class F>
struct cvt : public CFunType<OddList<U> >
//  public Fun0Impl<OddList<U> > makes no difference.
{
   F f;
   cvt( const F& ff ) : f(ff) {}
   OddList<U> operator()() const {
#ifdef FCPP_PRINT_HELPERS
     std::cout << "cvt called " << std::endl;
#endif
      List<U> l = f();
      return l.force();
   }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp;
     List<U> l = f();
     // This is interesting but not accurate as l is forced in the return.
     //if (fcpp::traits::debug_traits<F>::has_name)
     //	 temp += fcpp::traits::debug_traits<F>::name(f);
     //else
     temp += std::string("cvt");
     temp += l.seq_string();
     return temp;
   }
#endif
};

// I malloc a RefCountType to hold the refCount and init it to 1 to ensure the
// refCount will never get to 0, so the destructor-of-global-object
// order at the end of the program is a non-issue.  In other words, the
// memory allocated here is only reclaimed by the operating system.
template <class T> 
Cache<T>* xnil_helper() {
   void *p = std::malloc( sizeof(RefCountType) );
#ifdef FCPP_OOI_DEBUG
   std::cout << "making a nil/bad:" << typeid(T).name() 
             << " at address " << p << std::endl;
#endif
   *((RefCountType*)p) = 1;
   return static_cast<Cache<T>*>( p );
}

template <class T> 
Cache<T>* xempty_helper() {
#ifdef FCPP_1_3_LIST_IMPL
   (void) Cache<T>::xnil;   // Make sure xnil exists before moving forward
#endif
   return new Cache<T>( CacheEmpty() );
}

template <class T>
class Cache {
// This is mutable in BoostFC++ and also in reference info for IRef class.
   mutable RefCountType refC;
   mutable Fun0<OddList<T> >   fxn;
   mutable OddList<T>          val;
   // val.second.rep can be XBAD, XNIL, or a valid ptr
   //  - XBAD: val is invalid (fxn is valid)
   //  - XNIL: this is the empty list
   //  - anything else: val.first() is head, val.second is tail()

   // Caches are not copyable or assignable
   Cache( const Cache<T>& );
   void operator=( Cache<T> );

   // This functoid should never be called; it represents a
   // self-referent Cache, which should be impossible under the current
   // implementation.  Nonetheless, we need a 'dummy' function object to
   // represent invalid 'fxn's (val.second.rep!=XBAD), and this
   // implementation seems to be among the most reasonable.
#ifdef FCPP_CUDA_COMPATIBLE
   struct blackhole_helper : CFunType< OddList<T> >  {
     //     typedef OddList<T> ResultType;
     //template <class Dummy1=Void, class Dummy2=Void>
     //struct Sig : public FunType<WrongNumberOfSigArgs> {};
     //template <class Dummy>
     // struct Sig<Void,Dummy> : public FunType<ResultType> {};

      OddList<T> operator()() const {
         throw fcpp_exception("You have entered a black hole.");
      }
   };
#else
   struct blackhole_helper : CFunType< OddList<T> > {
      OddList<T> operator()() const {
         throw fcpp_exception("You have entered a black hole.");
      }
   };
#endif
#ifdef FCPP_1_3_LIST_IMPL
   static IRef<Cache<T> > xnil, xbad;
   static IRef<Cache<T> > xempty;
#endif

   // Don't get rid of these XFOO() functions; they impose no overhead,
   // and provide a useful place to add debugging code for tracking down
   // before-main()-order-of-initialization problems.
   static const IRef<Cache<T> >& XEMPTY() {
#ifndef FCPP_1_3_LIST_IMPL
      static IRef<Cache<T> > xempty( xempty_helper<T>() );
#endif
#ifdef FCPP_OOI_DEBUG
      static bool b = true;
      if(b) {
         std::cout << "access xempty:" << typeid(T).name() << std::endl;
         b = false;
      }
#endif
      return xempty;
   }
   static const IRef<Cache<T> >& XNIL() {    // this list is nil
#ifndef FCPP_1_3_LIST_IMPL
      static IRef<Cache<T> > xnil( xnil_helper<T>() );
#endif
#ifdef FCPP_OOI_DEBUG
      static bool b = true;
      if(b) {
         std::cout << "access xnil:" << typeid(T).name() << std::endl;
         b = false;
      }
#endif
      return xnil;
   }
   static const IRef<Cache<T> >& XBAD() {    // the pair is invalid; use fxn
#ifndef FCPP_1_3_LIST_IMPL
      static IRef<Cache<T> > xbad( xnil_helper<T>() );
#endif
#ifdef FCPP_OOI_DEBUG
      static bool b = true;
      if(b) {
         std::cout << "access xbad:" << typeid(T).name() << std::endl;
         b = false;
      }
#endif
      return xbad;
   }

   static Fun0<OddList<T> > the_blackhole;
#ifdef FCPP_CUDA_COMPATIBLE
   static Fun0<OddList<T> >& blackhole() {
#ifndef FCPP_1_3_LIST_IMPL
      static Fun0<OddList<T> > the_blackhole( makeFun0( blackhole_helper() ) );
#endif
      return the_blackhole;
   }
#else
   static Fun0<OddList<T> >& blackhole() {
#ifndef FCPP_1_3_LIST_IMPL
      static Fun0<OddList<T> > the_blackhole( makeFun0( blackhole_helper() ) );
#endif
      return the_blackhole;
   }
#endif
   // These are added for use by List.seq_string().
   bool is_empty() const { return val.second.rep == XNIL(); }
   bool has_value() const { return val.second.rep != XBAD(); }
   OddList<T>& cache() const {
      if( val.second.rep == XBAD() ) {
#ifdef FCPP_IMPLICIT_COPY_CONSTRUCTOR_SOLUTION
	val = std::move(fxn());
#else
         val = fxn();
#endif
         fxn = blackhole();
      }
      return val;
   }

   template <class U> friend class List;
   template <class U> friend class OddList;
   template <class U, class F, class R> friend struct ConsHelp;
   template <class U,class F> friend struct cvt;
   template <class U, class F, class R> friend struct ListHelp;
   template <class U> friend Cache<U>* xempty_helper();
   //This is no longer needed.
   //template <class F, class U> friend struct SnocHelp;

   Cache( CacheEmpty ) : refC(0), fxn(blackhole()), val() {}
#ifdef FCPP_IMPLICIT_COPY_CONSTRUCTOR_SOLUTION
   Cache( const OddList<T>& x ) : refC(0), fxn(blackhole()), val(x) {}
   //   Cache( const OddList<T>& x ) : refC(0), fxn(blackhole()), val(std::move(x)) {}
#else 
   Cache( const OddList<T>& x ) : refC(0), fxn(blackhole()), val(x) {}
#endif
   Cache( const T& x, const List<T>& l ) : refC(0),fxn(blackhole()),val(x,l) {}
   Cache( CacheDummy ) : refC(0), fxn(blackhole()), val( OddListDummyX() ) {}

   Cache( const Fun0<OddList<T> >& f )
   : refC(0), fxn(f), val( OddListDummyY() ) {}

   template <class F>
   Cache( const F& f )    // ()->OddList
   : refC(0), fxn(makeFun0(f)), val( OddListDummyY() ) {}

   // This is for ()->List<T> to ()->OddList<T>
   struct CvtFxn {};
   template <class F>
   Cache( CvtFxn, const F& f )    // ()->List
   : refC(0), fxn(makeFun0(cvt<T,F>(f))), val( OddListDummyY() ) {
     //#ifdef FCPP_DEBUG
     // This is part of debugging reverse.
     //std::cout << " cvt<T,F>(f) ";
     //#endif
   }

public:
   void incref() { ++refC; }
   void decref() { if (!--refC) delete this; }
};

#ifdef FCPP_1_3_LIST_IMPL
template <class T>
Fun0<OddList<T> > Cache<T>::the_blackhole( makeFun0( blackhole_helper() ) );

template <class T> IRef<Cache<T> > Cache<T>::xnil( xnil_helper<T>() );
template <class T> IRef<Cache<T> > Cache<T>::xbad( xnil_helper<T>() );
template <class T> IRef<Cache<T> > Cache<T>::xempty( xempty_helper<T>() );
#endif

// Rest of List's stuff

template <class T, class F> struct ListHelp<T,F,List<T> > {
   IRef<Cache<T> > operator()( const F& f ) const {
      return IRef<Cache<T> >(new Cache<T>(typename Cache<T>::CvtFxn(),f));
   }
};
template <class T, class F> struct ListHelp<T,F,OddList<T> > {
   IRef<Cache<T> > operator()( const F& f ) const {
      return IRef<Cache<T> >(new Cache<T>(f));
   }
};

template <class T, class It>
struct ListItHelp : public CFunType<OddList<T> > {
   It begin, end;
   ListItHelp( const It& b, const It& e ) : begin(b), end(e) {}
   OddList<T> operator()() const;
};
   
template <class T>
#ifdef FCPP_CXX17
// Inheritance from std::iterator is deprecated in C++17.
// It is replaced here by typedefs.
class ListIterator
#else
#ifdef FCPP_NO_STD_ITER
class ListIterator : public std::input_iterator<T,ptrdiff_t>
#else
// std::ptrdiff_t for gcc 4.6
class ListIterator : public std::iterator<std::input_iterator_tag,T,std::ptrdiff_t>
#endif
#endif
{ // Apparent unbalanced bracket caused by alternative definition!!
  // This has been in here since FC++ 1.5
   List<T> l;
   bool is_nil;
   void advance() {
      l = l.tail();
      if( !l )
         is_nil = true;
   }
   class Proxy {  // needed for operator->
      const T x;
      friend class ListIterator;
      Proxy( const T& xx ) : x(xx) {}
   public:
      const T* operator->() const { return &x; }
   };
public:
#ifdef FCPP_CXX17
   typedef T value_type;
   typedef ptrdiff_t difference_type;
   typedef T* pointer;
   typedef T& reference;
#endif
   ListIterator() : l(), is_nil(true) {}
   explicit ListIterator( const List<T>& ll ) : l(ll), is_nil(!ll) {}

   const T operator*() const { return l.head(); }
   const Proxy operator->() const { return Proxy(l.head()); }
   ListIterator<T>& operator++() {
      advance();
      return *this;
   }
   const ListIterator<T> operator++(int) {
      ListIterator<T> i( *this );
      advance();
      return i;
   }
   bool operator==( const ListIterator<T>& i ) const {
      return is_nil && i.is_nil;
   }
   bool operator!=( const ListIterator<T>& i ) const {
      return ! this->operator==(i);
   }
   bool is_null() const { return is_nil; }
#ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp = std::string("ListIterator");
     if (is_nil) {
        temp += std::string(" -> NILL");
     } else {
        temp += std::string(" -> ")
             + fcpp::traits::argument_traits<T>::value(**this);
     }
     return temp;
   }
#endif
};

// To test ListLike
template <class T>
class NotList {
 public:
   typedef T ElementType;
   NotList() {}
   
};

}

using impl::List;
using impl::OddList;
using impl::ListIterator;
using impl::NotList;


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*
concept ListLike: Given a list representation type L

L<T> inherits ListLike and has
   // typedefs just show typical values
   typedef T value_type
   typedef L<T> force_result_type
   typedef L<T> delay_result_type
   typedef L<T> tail_result_type
   template <class UU> struct cons_rebind {
      typedef L<UU> type;        // force type
      typedef L<UU> delay_type;  // delay type
   };

   L()
   L( a_unique_type_for_nil )
   template <class F> L(F)       // F :: ()->L

   constructor: force_result_type( T, L<T> )
   template <class F>
   constructor: force_result_type( T, F )  // F :: ()->L

   template <class It>
   L( It, It )

   // FIX THIS instead of operator bool(), does Boost have something better?
   operator bool() const
   force_result_type force() const
   delay_result_type delay() const
   T head() const
   tail_result_type tail() const

   static const bool is_lazy;   // true if can represent infinite lists

   typedef const_iterator;
   typedef const_iterator iterator;  // ListLikes are immutable
   iterator begin() const
   iterator end() const
*/

//////////////////////////////////////////////////////////////////////

template <class L, bool b>
struct EnsureListLikeHelp {
   static void trying_to_call_a_list_function_on_a_non_list() {}
};
template <class L> struct EnsureListLikeHelp<L,false> { };
template <class L>
void EnsureListLike() {
  //   EnsureListLikeHelp<L,boost::is_base_and_derived<ListLike,L>::value>::
  // I have used the FC++ way of doing this here.
  EnsureListLikeHelp<L,Inherits<L,ListLike>::value>::
   trying_to_call_a_list_function_on_a_non_list();
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

namespace impl{
struct XHead {
   template <class LT>
   struct Sig : public FunType<LT,typename LT::ElementType> {};

   template <class LT>
   typename Sig<LT>::ResultType operator()( const LT& l ) const {
      EnsureListLike<LT>();
      return l.head();
      }
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp = std::string("Head");
     return temp;
   }
 #endif
};
}
typedef Full1<impl::XHead> Head;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Head head;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XTail {
   template <class LT>
   struct Sig : public FunType<LT,List<typename LT::ElementType> > {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
      return l.tail();
   }
   template <class T>
   List<T> operator()( const OddList<T>& l ) const {
      return l.tail();
   }
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp = std::string("Tail");
     return temp;
   }
 #endif
};
}
typedef Full1<impl::XTail> Tail;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Tail tail;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XNull {
   template <class LT>
   struct Sig : public FunType<LT,bool> {};

   template <class T>
   bool operator()( const List<T>& l ) const {
      return !l;
   }
   template <class T>
   bool operator()( const OddList<T>& l ) const {
      return !l;
   }
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     std::string temp = std::string("Null");
     return temp;
   }
 #endif
};
}
typedef Full1<impl::XNull> Null;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Null null;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
template <class T, class F> struct ConsHelp<T,F,List<T> > {
   OddList<T> operator()( const T& x, const F& f ) const {
     return OddList<T>(x, List<T>(
         // Needed typename here. cf line 532 which has it.
         // Found running example isort.cpp
         IRef<Cache<T> >(new Cache<T>(typename Cache<T>::CvtFxn(),f))));
   }
};
template <class T, class F> struct ConsHelp<T,F,OddList<T> > {
   OddList<T> operator()( const T& x, const F& f ) const {
      return OddList<T>(x, List<T>( ListRaw(), new Cache<T>(f) ));
   }
};
struct XCons {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Cons");
       }
#endif
   template <class T, class L>
   struct Sig : public FunType<T,L,OddList<T> > {};

   template <class T>
   OddList<T> operator()( const T& x, const List<T>& l ) const {
      return OddList<T>(x,l);
   }
   template <class T>
   OddList<T> operator()( const T& x, const OddList<T>& l ) const {
      return OddList<T>(x,l);
   }
   template <class T>
   OddList<T> operator()( const T& x, const AUniqueTypeForNil& ) const {
      return OddList<T>(x,NIL);
   }

   template <class T, class F>
   OddList<T> operator()( const T& x, const F& f ) const {
      return ConsHelp<T,F,typename F::ResultType>()(x,f);
   }
};
}
typedef Full2<impl::XCons> Cons;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Cons cons;
FCPP_MAYBE_NAMESPACE_CLOSE

template <class T, class It>
OddList<T> impl::ListItHelp<T,It>::operator()() const {
   if( begin == end ) return NIL;
   It tmp = begin;
   T x( *begin );
   return cons( x, ListItHelp<T,It>( ++tmp, end ) );
}

namespace impl {
struct XCat {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Cat");
       }
#endif
   // The Intel compiler doesn't like it when I overload this function,
   // so I just used class template partial specialization in a nested
   // helper class to code around it.
   template <class L, class M>
   struct Helper : public CFunType<L,M,OddList<typename L::ElementType> > {
      OddList<typename L::ElementType> 
      operator()( const L& l, const M& m, 
             Reuser2<Inv,Var,Inv,Helper,List<typename L::ElementType>,M>
             r = NIL ) const {
         if( null(l) ) 
            return m().force();
         else
            return cons( head(l), r( *this, tail(l), m ) );
      }
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Cat::Helper<L,M>");
       }
#endif
   };
   template <class L, class T>
   struct Helper<L,List<T> >
   : public CFunType<L,List<T>,OddList<typename L::ElementType> > {
      OddList<T> 
      operator()( const L& l, const List<T>& m,
             Reuser2<Inv,Var,Inv,Helper,List<typename L::ElementType>,List<T> >
             r = NIL ) const {
         if( null(l) ) 
            return m.force();
         else
            return cons( head(l), r( *this, tail(l), m ) );
      }
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Cat::Helper<L,List<T> >");
       }
#endif
   };
   template <class L, class T>
   struct Helper<L,OddList<T> >
   : public CFunType<L,OddList<T>,OddList<typename L::ElementType> > {
      OddList<T>
      operator()( const L& l, const OddList<T>& m,
           Reuser2<Inv,Var,Inv,Helper,List<typename L::ElementType>,OddList<T> >
           r = NIL ) const {
         if( null(l) ) 
            return m;
         else
            return cons( head(l), r( *this, tail(l), m ) );
      }
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("CatHelper<L,OddList<T> >");
       }
#endif
   };
   template <class L>
   struct Helper<L,AUniqueTypeForNil>
   : public CFunType<L,AUniqueTypeForNil,OddList<typename L::ElementType> > {
      OddList<typename L::ElementType>
      operator()( const L& l, const AUniqueTypeForNil& ) const {
         return l;
      }
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Cat::Helper<L,NIL >");
       }
#endif
   };
public:
   template <class L, class M>
   struct Sig : public FunType<L,M,OddList<typename L::ElementType> > {};

   // Note: first arg must be a list, but second arg can be either a list
   // or a function that returns a list.
   template <class L, class M>
   OddList<typename L::ElementType>
   operator()( const L& l, const M& m ) const {
      EnsureListLike<L>();
      return Helper<L,M>()(l,m);
   }
};
}
typedef Full2<impl::XCat> Cat;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Cat cat;
FCPP_MAYBE_NAMESPACE_CLOSE

///////////////////////////////////////////////////////////////////
// snoc moved from pattern.h to make it more widely useful.
// In particular it can now be used in prelude.h
// I have moved it back to pattern.h as here there are problems
// with the constructor in SnocHelp for reasons I do not understand.
// I must have resolved this as it is back here in list.h!
// See more comments in pattern.h.
///////////////////////////////////////////////////////////////////
// For information on snoc see "Purely Functional Data Structures"
// Chris Okasaki p.42
///////////////////////////////////////////////////////////////////

namespace impl {
    struct XSnoc;
    template <class F, class T>
    struct SnocHelp
    {
      OddList<T> operator()( const F &f, const T& x) const {
        
	return cat( List<T>( ListRaw(), new Cache<T>(f) ),cons(x,NIL));
      }
    };

struct XSnoc {
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
typedef Full2<impl::XSnoc> Snoc;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Snoc snoc;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XDelay {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Delay");
       }
#endif
   template <class L>
   struct Sig : public FunType<L,List<typename L::ElementType> > {};

   template <class L>
   List<typename L::ElementType> operator()( const L& l ) const {
      EnsureListLike<L>();
      return l.delay();
   }
};
}
typedef Full1<impl::XDelay> Delay;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Delay delay;
FCPP_MAYBE_NAMESPACE_CLOSE

namespace impl {
struct XForce {
#ifdef FCPP_DEBUG
       std::string name() const
       {
           return std::string("Force");
       }
#endif
   template <class L>
   struct Sig : public FunType<L,OddList<typename L::ElementType> > {};

   template <class L>
   OddList<typename L::ElementType> operator()( const L& l ) const {
      EnsureListLike<L>();
      return l.force();
   }
};
}
typedef Full1<impl::XForce> Force;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Force force;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// op== and op<, overloaded for all combos of List, OddList, and NIL
//////////////////////////////////////////////////////////////////////

template <class T>
bool operator==( const OddList<T>& a, AUniqueTypeForNil ) {
   return null(a);
}
template <class T>
bool operator==( const List<T>& a, AUniqueTypeForNil ) {
   return null(a);
}
template <class T>
bool operator==( AUniqueTypeForNil, const OddList<T>& a ) {
   return null(a);
}
template <class T>
bool operator==( AUniqueTypeForNil, const List<T>& a ) {
   return null(a);
}
template <class T>
bool operator==( const List<T>& a, const List<T>& b ) {
   if( null(a) && null(b) )
      return true;
   if( null(a) || null(b) )
      return false;
   return (head(a)==head(b)) && (tail(a)==tail(b));
}
template <class T>
bool operator==( const OddList<T>& a, const OddList<T>& b ) {
   if( null(a) && null(b) )
      return true;
   if( null(a) || null(b) )
      return false;
   return (head(a)==head(b)) && (tail(a)==tail(b));
}
template <class T>
bool operator==( const List<T>& a, const OddList<T>& b ) {
   if( null(a) && null(b) )
      return true;
   if( null(a) || null(b) )
      return false;
   return (head(a)==head(b)) && (tail(a)==tail(b));
}
template <class T>
bool operator==( const OddList<T>& a, const List<T>& b ) {
   if( null(a) && null(b) )
      return true;
   if( null(a) || null(b) )
      return false;
   return (head(a)==head(b)) && (tail(a)==tail(b));
}

template <class T>
bool operator<( const List<T>& a, const List<T>& b ) {
   if( null(a) && !null(b) )  return true;
   if( null(b) )              return false;
   if( head(b) < head(a) )    return false;
   if( head(a) < head(b) )    return true;
   return (tail(a) < tail(b));
}
template <class T>
bool operator<( const OddList<T>& a, const List<T>& b ) {
   if( null(a) && !null(b) )  return true;
   if( null(b) )              return false;
   if( head(b) < head(a) )    return false;
   if( head(a) < head(b) )    return true;
   return (tail(a) < tail(b));
}
template <class T>
bool operator<( const List<T>& a, const OddList<T>& b ) {
   if( null(a) && !null(b) )  return true;
   if( null(b) )              return false;
   if( head(b) < head(a) )    return false;
   if( head(a) < head(b) )    return true;
   return (tail(a) < tail(b));
}
template <class T>
bool operator<( const OddList<T>& a, const OddList<T>& b ) {
   if( null(a) && !null(b) )  return true;
   if( null(b) )              return false;
   if( head(b) < head(a) )    return false;
   if( head(a) < head(b) )    return true;
   return (tail(a) < tail(b));
}
template <class T>
bool operator<( const OddList<T>&, AUniqueTypeForNil ) {
   return false;
}
template <class T>
bool operator<( const List<T>&, AUniqueTypeForNil ) {
   return false;
}
template <class T>
bool operator<( AUniqueTypeForNil, const OddList<T>& b ) {
   return !null(b);
}
template <class T>
bool operator<( AUniqueTypeForNil, const List<T>& b ) {
   return !null(b);
}

//////////////////////////////////////////////////////////////////////
// Handy functoids for making list literals
// The list_with functions (see below) are not functoids and
// so won't work with [ ] and LambdaVars.
// These are called makeList(for 2) and makeList1 to makeList6
//////////////////////////////////////////////////////////////////////
namespace impl {

    struct XMakeList1 {

    template <class T>
    struct Sig : public FunType<T,List<T> > {};

    template <class T>
    List<T> operator()( const T& a) const {
       List<T> l;
       l = cons( a, l );
       return l;
    }
  };

  struct XMakeList {

    template <class T, class U> struct Sig;
    template <class T>
    struct Sig<T,T> : public FunType<T,T,List<T> > {};

    template <class T>
    List<T> operator()( const T& a, const T& b ) const {
       List<T> l;
       l = cons( b, l );
       l = cons( a, l );
       return l;
    }
  };

  struct XMakeList3 {

    template <class T, class U, class V> struct Sig;
    template <class T>
    struct Sig<T,T,T> : public FunType<T,T,T,List<T> > {};

    template <class T>
    List<T> operator()( const T& a, const T& b, const T& c ) const {
       List<T> l;
       l = cons( c, l );
       l = cons( b, l );
       l = cons( a, l );
       return l;
    }
  };

// FC++.1.5.2
#ifdef FCPP152
  struct XMakeList4 {

    template <class T, class U, class V, class W> struct Sig;
    template <class T>
    struct Sig<T,T,T,T> : public FunType<T,T,T,T,List<T> > {};

    template <class T>
    List<T> operator()( const T& a, const T& b, const T& c, const T& d ) const {
       List<T> l;
       l = cons( d, l );
       l = cons( c, l );
       l = cons( b, l );
       l = cons( a, l );
       return l;
    }
  };

  struct XMakeList5 {

    template <class T, class U, class V, class W, class X> struct Sig;
    template <class T>
    struct Sig<T,T,T,T,T> : public FunType<T,T,T,T,T,List<T> > {};

    template <class T>
    List<T> operator()
    ( const T& a, const T& b, const T& c, const T& d, const T& e ) const
    {
       List<T> l;
       l = cons( e, l );
       l = cons( d, l );
       l = cons( c, l );
       l = cons( b, l );
       l = cons( a, l );
       return l;
    }
  };

  struct XMakeList6 {

    template <class T, class U, class V, class W, class X, class Y> struct Sig;
    template <class T>
    struct Sig<T,T,T,T,T,T> : public FunType<T,T,T,T,T,T,List<T> > {};

    template <class T>
    List<T> operator()
    ( const T& a, const T& b, const T& c, const T& d, const T& e, const T& f ) const
    {
       List<T> l;
       l = cons( f, l );
       l = cons( e, l );
       l = cons( d, l );
       l = cons( c, l );
       l = cons( b, l );
       l = cons( a, l );
       return l;
    }
  };

  struct XMakeList7 {

    template <class T, class U, class V, class W, class X, class Y, class Z>
    struct Sig;
    template <class T>
    struct Sig<T,T,T,T,T,T,T> : public FunType<T,T,T,T,T,T,T,List<T> > {};

    template <class T>
    List<T> operator()
    ( const T& a, const T& b, const T& c, const T& d, const T& e, const T& f,
      const T& g ) const
    {
       List<T> l;
       l = cons( g, l );
       l = cons( f, l );
       l = cons( e, l );
       l = cons( d, l );
       l = cons( c, l );
       l = cons( b, l );
       l = cons( a, l );
       return l;
    }
  };

  struct XMakeList8 {

    template <class T, class U, class V, class W, class X, class Y,
              class Z, class A>
    struct Sig;
    template <class T>
    struct Sig<T,T,T,T,T,T,T,T> : public FunType<T,T,T,T,T,T,T,T,List<T> > {};

    template <class T>
    List<T> operator()
    ( const T& a, const T& b, const T& c, const T& d, const T& e, const T& f,
      const T& g, const T& h ) const
    {
       List<T> l;
       l = cons( h, l );
       l = cons( g, l );
       l = cons( f, l );
       l = cons( e, l );
       l = cons( d, l );
       l = cons( c, l );
       l = cons( b, l );
       l = cons( a, l );
       return l;
    }
  };

#endif

}

typedef Full1<impl::XMakeList1> MakeList1;
typedef Full2<impl::XMakeList > MakeList2;
typedef Full3<impl::XMakeList3> MakeList3;
#ifdef FCPP152
typedef Full4<impl::XMakeList4> MakeList4;
typedef Full5<impl::XMakeList5> MakeList5;
typedef Full6<impl::XMakeList6> MakeList6;
typedef Full7<impl::XMakeList7> MakeList7;
typedef Full8<impl::XMakeList8> MakeList8;
#endif
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN MakeList1 makeList1;
FCPP_MAYBE_EXTERN MakeList2 makeList2, makeList;
FCPP_MAYBE_EXTERN MakeList3 makeList3;
#ifdef FCPP152
FCPP_MAYBE_EXTERN MakeList4 makeList4;
FCPP_MAYBE_EXTERN MakeList5 makeList5;
FCPP_MAYBE_EXTERN MakeList6 makeList6;
FCPP_MAYBE_EXTERN MakeList7 makeList7;
FCPP_MAYBE_EXTERN MakeList8 makeList8;
#endif
FCPP_MAYBE_NAMESPACE_CLOSE


//////////////////////////////////////////////////////////////////////
// Handy functions for making list literals
//////////////////////////////////////////////////////////////////////
// Yes, these aren't functoids, they're just template functions.  I'm
// lazy and created these mostly to make it easily to make little lists
// in the sample code snippets that appear in papers.
// There are now functoid equivalents, makeList1 to makeList4.
// See above in this file.
//////////////////////////////////////////////////////////////////////

template <class T>
List<T> list_with( const T& a ) {
   List<T> l;
   l = cons( a, l );
   return l;
}

template <class T>
List<T> list_with( const T& a, const T& b ) {
   List<T> l;
   l = cons( b, l );
   l = cons( a, l );
   return l;
}

template <class T>
List<T> list_with( const T& a, const T& b, const T& c ) {
   List<T> l;
   l = cons( c, l );
   l = cons( b, l );
   l = cons( a, l );
   return l;
}

template <class T>
List<T> list_with( const T& a, const T& b, const T& c, const T& d ) {
   List<T> l;
   l = cons( d, l );
   l = cons( c, l );
   l = cons( b, l );
   l = cons( a, l );
   return l;
}

template <class T>
List<T>
list_with( const T& a, const T& b, const T& c, const T& d, const T& e ) {
   List<T> l;
   l = cons( e, l );
   l = cons( d, l );
   l = cons( c, l );
   l = cons( b, l );
   l = cons( a, l );
   return l;
}

// Extended to make some longer lists (up to 10 items).
template <class T>
List<T>
  list_with( const T& a, const T& b, const T& c, const T& d, const T& e,
             const T& f ) {
   List<T> l;
   l = cons( f, l );
   l = cons( e, l );
   l = cons( d, l );
   l = cons( c, l );
   l = cons( b, l );
   l = cons( a, l );
   return l;
}

template <class T>
List<T>
  list_with( const T& a, const T& b, const T& c, const T& d, const T& e,
             const T& f, const T& g ) {
  List<T> l = list_with( a, b, c );
  return cat(l, list_with( d, e, f, g ) );
}


template <class T>
List<T>
  list_with( const T& a, const T& b, const T& c, const T& d, const T& e,
             const T& f, const T& g, const T& h ) {
  List<T> l = list_with( a, b, c );
  return cat(l, list_with( d, e, f, g, h ) );
}


template <class T>
List<T>
  list_with( const T& a, const T& b, const T& c, const T& d, const T& e,
             const T& f, const T& g, const T& h, const T& i ) {
  List<T> l = list_with( a, b, c, d );
  return cat(l, list_with( e, f, g, h, i ) );
}

template <class T>
List<T>
  list_with( const T& a, const T& b, const T& c, const T& d, const T& e,
             const T& f, const T& g, const T& h, const T& i, const T& j ) {
  List<T> l = list_with( a, b, c, d, e );
  return cat(l, list_with( f, g, h, i, j ) );
 }

template <class T>
List<T>
  list_with( const T& a, const T& b, const T& c, const T& d, const T& e,
             const T& f, const T& g, const T& h, const T& i, const T& j,
             const T& k ) {
  List<T> l = list_with( a, b, c, d, e );
  return cat(l, list_with( f, g, h, i, j, k ) );
 }

template <class T>
List<T>
  list_with( const T& a, const T& b, const T& c, const T& d, const T& e,
             const T& f, const T& g, const T& h, const T& i, const T& j,
             const T& k, const T& ll ) {
  List<T> l = list_with( a, b, c, d, e, f );
  return cat(l, list_with( g, h, i, j, k, ll ) );
 }

} // namespace fcpp

// Includes argument traits needed without FCPP_DEBUG
#include "list_debug.h"

#endif
