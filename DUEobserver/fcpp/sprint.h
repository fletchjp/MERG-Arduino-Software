//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_SPRINT_DOT_H
#define FCPP_SPRINT_DOT_H

// This was developed as seq.h

namespace fcpp {

 namespace traits {

// Avoid defs involving ByNeed and Maybe when not using LAMBDA.
#ifdef FCPP_ENABLE_LAMBDA
  template <class F>
  class ByNeed_traits
  {
    public:
       static const bool is_ByNeed = false;
       static std::string value(const F& f) { return std::string(""); }
       static void call(const F& f) { }
};

  template <class T>
  class ByNeed_traits<ByNeed<T> >
  {
    public:
       static const bool is_ByNeed = true;
       static std::string value(const ByNeed<T>& f) { return f.seq_string(); }
       static void call(const ByNeed<T>& f) { f.force(); }
  };
#endif

  template <class F>
  class List_traits
  {
    public:
       static const bool is_List = false;
       static std::string value(const F& f) { return std::string(""); }
       static void call(const F& f) { }
};

  template <class T>
  class List_traits<List<T> >
  {
    public:
       static const bool is_List = true;
       static std::string value(const List<T>& f)
       { return f.seq_string(); }
       static void call(const List<T>& f) { f.force(); }
  };

  template <class T>
  class List_traits<ListIterator<T> >
  {
    public:
       static const bool is_List = true;
       static std::string value(const ListIterator<T>& it)
       {
          if (it.is_null()) {
             return std::string("-> NILL");
          } else {
             return std::string("-> ")
                  + argument_traits_value(*it);
          }
       }
       static void call(const ListIterator<T>& it) { }
  };

  template <class T>
  class List_traits<OddList<T> >
  {
    public:
       static const bool is_List = true;
       static std::string value(const OddList<T>& f)
       { return List<T>(f).seq_string(); }
       static void call(const OddList<T>& f) { List<T>(f).force(); }
  };

#ifdef FCPP_PATTERN
  // Not tested.
  template <class T>
  class List_traits<Queue<T> >
  {
    public:
       static const bool is_List = true;
       static std::string value(const Queue<T>& q)
       { return List<T>(q.front()).seq_string()
         + List<T>(q.rear()).seq_string(); }
       static void call(const Queue<T>& q) { List<T>(q.front()).force(); }
  };
  // Something similar may be needed for BootList<T>.

  template <class T>
  class List_traits<BootList<T> >
  {
    public:
       static const bool is_List = true;
       static std::string value(const BootList<T>& b)
       {
         if (b.is_empty()) return std::string("NIL");
         else return std::string("");
       }
       static void call(const BootList<T>& b) { }
  };
#endif

  /* sprint.h moved into prelude and pattern is still experimental.
     As a result this was not seen here.
     I have moved pattern into prelude conditional on FCPP_PATTERN
  */
#ifdef FCPP_PATTERN
  template <class T>
  class List_traits<Stream<T> >
  {
    public:
       static const bool is_List = true;
       static std::string value(const Stream<T>& f)
       {
         return f.seq_string();
       }
       static void call(const Stream<T>& f) { f.force(); }
  };
#endif

  /* Moved to argument_traits.h
  template<class S,class T>
  class argument_traits<std::pair<S,T> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const std::pair<S,T> &p)
         {
           std::string temp;
           temp += std::string("{ ");
           temp += argument_traits_value(p.first);
           temp += std::string(" , ");
           temp += argument_traits_value(p.second);
           temp += std::string(" }");
           return temp;
         }
       };
  */

  template<>
    class argument_traits<AUniqueTypeForNothing >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const AUniqueTypeForNothing &f)
         { std::string null; return null;}
       };

  template<class S,class T>
    class argument_traits<List<std::pair<S,T> > >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const List<std::pair<S,T> > &f)
         { std::string null; return null;}
       };

       template<class F>
       class argument_traits<LazyPtrProxy<F> >
       {
       public:
        //static const bool has_name = true;
        static std::string value(const LazyPtrProxy<F> &f)
        { std::string null; return null;}
       };

       template<class F, class A1>
       class argument_traits<Proxy1<F,A1> >
       {
       public:
        //static const bool has_name = true;
         static std::string value(const Proxy1<F,A1> &f)
         { std::string null; return null;}
       };

       template<class F, class A1, class A2>
       class argument_traits<Proxy2<F,A1,A2> >
       {
       public:
        //static const bool has_name = true;
          static std::string value(const Proxy2<F,A1,A2> &f)
          { std::string null; return null;}
       };

       template<class F, class A1, class A2, class A3>
       class argument_traits<Proxy3<F,A1,A2,A3> >
       {
       public:
        //static const bool has_name = true;
          static std::string value(const Proxy3<F,A1,A2,A3> &f)
          { std::string null; return null;}
       };
#ifdef FCPP152
       template<class F, class A1, class A2, class A3, class A4>
       class argument_traits<Proxy4<F,A1,A2,A3,A4> >
       {
       public:
        //static const bool has_name = true;
          static std::string value(const Proxy4<F,A1,A2,A3,A4> &f)
          { std::string null; return null;}
       };

#endif
       template<class F>
       class argument_traits<Fun0<F> >
       {
       public:
        //static const bool has_name = true;
        static std::string value(const Fun0<F> &f)
        { std::string null; return null;}
       };

       template<class A, class B>
       class argument_traits<Fun1<A,B> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const Fun1<A,B> &f)
         { std::string null; return null;}
       };

       template<class A, class B, class C>
       class argument_traits<Fun2<A,B,C> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const Fun2<A,B,C> &f)
         { std::string null; return null;}
       };

       template<class A, class B, class C, class D>
       class argument_traits<Fun3<A,B,C,D> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const Fun3<A,B,C,D> &f)
         { std::string null; return null;}
       };

       template<class A, class B, class C, class D>
       class argument_traits<Fun3Guts<A,B,C,D> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const Fun3Guts<A,B,C,D> &f)
         { std::string null; return null;}
       };

#ifdef FCPP152
       template<class A, class B, class C, class D, class E>
       class argument_traits<Fun4<A,B,C,D,E> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const Fun4<A,B,C,D,E> &f)
         { std::string null; return null;}
       };

       template<class A, class B, class C, class D, class E>
       class argument_traits<Fun4Guts<A,B,C,D,E> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const Fun4Guts<A,B,C,D,E> &f)
         { std::string null; return null;}
       };

       template<class A, class B, class C, class D, class E, class F>
	 class argument_traits<Fun5<A,B,C,D,E,F> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const Fun5<A,B,C,D,E,F> &f)
         { std::string null; return null;}
       };

       template<class A, class B, class C, class D, class E, class F,
                class G>
	 class argument_traits<Fun6<A,B,C,D,E,F,G> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const Fun6<A,B,C,D,E,F,G> &f)
         { std::string null; return null;}
       };

       template<class A, class B, class C, class D, class E, class F,
	 class G, class H>
	 class argument_traits<Fun7<A,B,C,D,E,F,G,H> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const Fun7<A,B,C,D,E,F,G,H> &f)
         { std::string null; return null;}
       };

#endif
#ifdef FCPP_ENABLE_LAMBDA

       template<class F>
       class argument_traits<ByNeed<F> >
       {
       public:
        //static const bool has_name = true;
        static std::string value(const ByNeed<F> &f)
        { return f.seq_string(); }
       };

       template<class F>
       class argument_traits<std::pair<ByNeed<F>,ByNeed<F> > >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const std::pair<ByNeed<F>,ByNeed<F> > &f)
         { std::string null; return null;}
       };


       template<class F>
       class argument_traits<Maybe<F> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const Maybe<F> &f)
         { std::string null; return null;}
       };
#endif

       template<class F>
       class argument_traits<Identity<F> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const Identity<F> &f)
         { std::string null; return null;}
       };

       // This specialisation is needed to sort out OddList<T> with sprint.
       template <class T>
       class pair_traits<OddList<T> >
       {
       public:
        static const bool is_pair = false;
        static std::string value(const OddList<T>& f) { return std::string(""); }
       };

#ifdef FCPP_PATTERN
    template <class T >
      class argument_traits<Stream<T> > {
    public:
      //static const bool is_List = true;
      static std::string value(const Stream<T>& s)
      {
#ifdef FCPP_DEBUG
        return s.name();
#else
        //return std::string("Stream");
        return s.seq_string();
#endif
      }
    };


       template <class T>
       class argument_traits<Queue<T> >
       {
       public:
       static const bool is_List = true;
       static std::string value(const Queue<T>& q)
          {
             return
             argument_traits_value(q.front()) + std::string(" : ")
           + argument_traits_value(q.rear());
          }
       };

       template <class T>
       class argument_traits<BootList<T> >
       {
       public:
       static const bool is_List = true;
       static std::string value(const BootList <T>& b)
          {
            if (b.is_empty()) return std::string("NIL");
            std::ostringstream o;
            o << b;
            return o.str();
          }
       };
#endif
 }

  // New routines to help with access
  template <class T>
  bool List_traits_is_List()
  {
    return traits::List_traits<T>::is_List;
  }

  template <class T>
  std::string List_traits_value (const T& t)
  {
      return traits::List_traits<T>::value(t);
  }

#ifdef FCPP_ENABLE_LAMBDA
  template <class T>
  bool ByNeed_traits_is_ByNeed()
  {
    return traits::ByNeed_traits<T>::is_ByNeed;
  }

  template <class T>
  std::string ByNeed_traits_value (const T& t)
  {
      return traits::ByNeed_traits<T>::value(t);
  }

#endif

 template <class F>
 std::string sprint(const F&f)
 {
#ifdef FCPP_DEBUG
   if ( debug_traits_has_name<F>() )
     return debug_traits_name(f);
   else {
     if (pair_traits_is_pair<F>()) {
       return pair_traits_value(f);
     } else
       return argument_traits_value(f);
   }
    // return std::string("no_name");
#else
#ifdef FCPP_ENABLE_LAMBDA
   if (ByNeed_traits_is_ByNeed<F>()) {
      return ByNeed_traits_value(f);
    } else {
#endif
      if (Full0_traits_is_Full0<F>() ) {
        return std::string("( _ )");
      } else {
        if (pair_traits_is_pair<F>()) {
          return pair_traits_value(f);
        } else {
          if (List_traits_is_List<F>()) {
        //std::cout << "Reached List_traits" << std::endl;
             return List_traits_value(f);
          } else {
        //std::cout << "Reached final location" << std::endl;
            // After all these alternatives, it is a value
            // which can be output directly.
             return argument_traits_value(f);
          }
        }
         // return std::string("sprint names not available");
      }
#ifdef FCPP_ENABLE_LAMBDA
   }
#endif
#endif
 }

  namespace impl {
    /////////////////////////////////////////////////////////////
    // Attempt to build the seq functoid as described in
    // Real World Haskell p.108-9.
    // seq :: a -> t -> t
    // It forces the first argument to be evaluated and then returns
    // the second argument.
    // See the following which makes this somewhat more complicated.
    // http://www.haskell.org/haskellwiki/Seq
    //////////////////////////////////////////////////////////////
    // The description of seq given in "Parallel and Concurrent
    // Programming in Haskell" p11 and onwards is different.
    // It says that the evaluation of the first argument is to make
    // it be in "Weak Head Normal Form" (WHNF) which implies partial
    // evaluation "to the first constructor".
    // At the moment I don't know how to do this in FC++.
    //////////////////////////////////////////////////////////////
    struct XSeq
    {
 #ifdef FCPP_DEBUG
   std::string name() const
   {
     return std::string("Seq");
   }
 #endif
      template <class T, class U> struct Sig : public FunType<T,U,U> {};

      template <class T, class U>
      U operator()( const T& x, const U& y ) const {
        // evaluate x;
        // The book says it should do a run time check
        // to see if evaluation is needed. That reminds me
        // of the list/odd_list situation.
        // I need a check here that T is a Full0.
        // This tests if it is a Full0 and calls it if it is.
        if (fcpp::traits::Full0_traits<T>::is_Full0)
            fcpp::traits::Full0_traits<T>::call(x); // void return
#ifdef FCPP_ENABLE_LAMBDA
        if (fcpp::traits::ByNeed_traits<T>::is_ByNeed)
            fcpp::traits::ByNeed_traits<T>::call(x); // void return
#endif
        // I think I should check for an OddList and force it, so that the
        // list is updated. This now works for both List and OddList.
        if (fcpp::traits::List_traits<T>::is_List)
            fcpp::traits::List_traits<T>::call(x); // void return
        // If it is anything else I am ignoring it for now....
        // What other cases are needed?  Either?
        return y;
      }
    };
  }
 typedef Full2<impl::XSeq> Seq;
 FCPP_MAYBE_NAMESPACE_OPEN
 FCPP_MAYBE_EXTERN Seq seq;
 FCPP_MAYBE_NAMESPACE_CLOSE

   // #ifdef FCPP_DEBUG
    namespace traits {
#ifdef FCPP_DEBUG
    template<>
       class debug_traits<impl::XSeq>
       {
       public:
         static const bool has_name = true;
         static std::string name(const impl::XSeq &f) { return f.name(); }
       };

 #endif

    // This specialization is here where sprint is known.
    template <class F>
      class pair_value_traits <F,true>
    {
      public:
       static std::string value(const F& f) {
          std::string temp("( ");
          temp += fcpp::sprint(f.first) + std::string(" , ");
          temp += fcpp::sprint(f.second) + std::string(" ) ");
          return temp;
       }
    };

 }

}

#endif
