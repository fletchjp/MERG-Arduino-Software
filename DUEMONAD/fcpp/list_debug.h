//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
// Copyright (c) 2016      John Fletcher
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

// prelude_debug.h is to take all the argument_traits and 
// debug_traits definitions from prelude.h

#ifndef FCPP_LIST_DEBUG_DOT_H
#define FCPP_LIST_DEBUG_DOT_H

namespace fcpp {
  namespace traits {

     template <class T >
     class argument_traits<List<T> > {
    public:
      static std::string value(const List<T>& l)
      {
#ifdef FCPP_DEBUG
        return l.name();
#else
        return l.seq_string();
#endif
	//	   std::ostringstream o;
	//   o << l;
        //   return o.str();
      }
    };

    template <class T >
      class argument_traits<ListIterator<T> > {
    public:
      static std::string value(const ListIterator<T>& it)
      {
#ifdef FCPP_DEBUG
        //return *it.name();
	return "list_iterator_internal";
#else
	return "list_iterator_internal";
        //return *it.seq_string();
#endif
	//	   std::ostringstream o;
	//   o << l;
        //   return o.str();
      }
    };

    template <class T >
      class argument_traits<OddList<T> > {
    public:
      static std::string value(const OddList<T>& l)
      {
#ifdef FCPP_DEBUG
        return l.name();
#else
        //return std::string("XXXXXXX");
        return l.seq_string();
#endif
	//	   std::ostringstream o;
	//   o << l;
        //   return o.str();
      }
    };

#ifdef FCPP_DEBUG

      template<class U, class F>
	class debug_traits<impl::cvt<U,F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::cvt<U,F> &f)
               { return f.name(); }
       };

    template<class T>
      class debug_traits<impl::List<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::List<T> &f)
               { return f.name(); }
       };

    template<class T>
      class debug_traits<impl::OddList<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::OddList<T> &f)
               { return f.name(); }
       };

    template<class T>
      class debug_traits<impl::ListIterator<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::ListIterator<T> &f)
               { return f.name(); }
       };

       template<>
	 class debug_traits<impl::XHead >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XHead &f) { return f.name(); }
       };

        template<>
	 class debug_traits<impl::XTail >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XTail &f) { return f.name(); }
       };

        template<>
	 class debug_traits<impl::XNull >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNull &f) { return f.name(); }
       };

    template<>
      class debug_traits<impl::XCons >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XCons &f) { return f.name(); }
       };

    template<>
      class debug_traits<impl::XCat >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XCat &f) { return f.name(); }
       };

    template<class L, class M>
      class debug_traits<impl::XCat::Helper<L,M> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XCat::Helper<L,M> &f) { return f.name(); }
       };

    template<class L, class T>
      class debug_traits<impl::XCat::Helper<L,List<T> > >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XCat::Helper<L,List<T> > &f) { return f.name(); }
       };

    template<class L, class T>
      class debug_traits<impl::XCat::Helper<L,OddList<T> > >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XCat::Helper<L,OddList<T> > &f) { return f.name(); }
       };

    template<>
      class debug_traits<impl::XDelay >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XDelay &f) { return f.name(); }
       };

    template<>
      class debug_traits<impl::XForce >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XForce &f) { return f.name(); }
       };

    template<>
      class debug_traits<impl::XSnoc >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XSnoc &f) { return f.name(); }
       };

#endif

  }  //  end namespace traits
} // end namespace fcpp

#endif
