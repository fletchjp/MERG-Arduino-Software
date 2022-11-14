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

// variadic_debug.h is to take all the argument_traits (if any) and 
// debug_traits definitions from variadic_fcpp.h

#ifndef FCPP_VARIADIC_DEBUG_DOT_H
#define FCPP_VARIADIC_DEBUG_DOT_H

// Traits are in namespace fcpp for interoperability.
namespace fcpp {
  namespace traits {

     // argument traits now defined outside FCPP_DEBUG
       template<class F>
	 class argument_traits<variadic_fcpp::FullN<F> >
       {
       public:
	 //static const bool has_name = true;
	 static std::string value(const variadic_fcpp::FullN<F> &f)
	{ std::string null; return null;}
       };
       /* Defined in curry_debug.h
       template<>
	 class argument_traits<fcpp::AutoCurryType >
       {
       public:
	 //static const bool has_name = true;
	 static std::string value(const fcpp::AutoCurryType &f)
	 { return std::string("_"); }
       };
       */
       template<>
	 class argument_traits<variadic_fcpp::MoreToCome >
       {
       public:
	 //static const bool has_name = true;
	 static std::string value(const variadic_fcpp::MoreToCome &f)
	 { return std::string("_abc"); }
       };

#ifdef FCPP_DEBUG

      template <class F>
      class debug_traits<variadic_fcpp::FullN<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::FullN<F> &f) { return f.name(); }
      };

      template <class Nary, class Arg1>
      class debug_traits<variadic_fcpp::binder1ofN<Nary,Arg1> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1ofN<Nary,Arg1> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg1, class Arg2>
	class debug_traits<variadic_fcpp::binder1and2ofN<Nary,Arg1,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1and2ofN<Nary,Arg1,Arg2> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg1, class Arg3>
	class debug_traits<variadic_fcpp::binder1and3ofN<Nary,Arg1,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1and3ofN<Nary,Arg1,Arg3> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg1, class Arg4>
	class debug_traits<variadic_fcpp::binder1and4ofN<Nary,Arg1,Arg4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1and4ofN<Nary,Arg1,Arg4> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg1, class Arg5>
	class debug_traits<variadic_fcpp::binder1and5ofN<Nary,Arg1,Arg5> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1and5ofN<Nary,Arg1,Arg5> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg2, class Arg3>
	class debug_traits<variadic_fcpp::binder2and3ofN<Nary,Arg2,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder2and3ofN<Nary,Arg2,Arg3> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg2, class Arg4>
	class debug_traits<variadic_fcpp::binder2and4ofN<Nary,Arg2,Arg4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder2and4ofN<Nary,Arg2,Arg4> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg3, class Arg4>
	class debug_traits<variadic_fcpp::binder3and4ofN<Nary,Arg3,Arg4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder3and4ofN<Nary,Arg3,Arg4> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg1, class Arg2, class Arg3>
	class debug_traits<variadic_fcpp::binder1and2and3ofN<Nary,Arg1,Arg2,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1and2and3ofN<Nary,Arg1,Arg2,Arg3> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg1, class Arg2, class Arg4>
	class debug_traits<variadic_fcpp::binder1and2and4ofN<Nary,Arg1,Arg2,Arg4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1and2and4ofN<Nary,Arg1,Arg2,Arg4> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg1, class Arg2, class Arg5>
	class debug_traits<variadic_fcpp::binder1and2and5ofN<Nary,Arg1,Arg2,Arg5> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1and2and5ofN<Nary,Arg1,Arg2,Arg5> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg1, class Arg3, class Arg4>
	class debug_traits<variadic_fcpp::binder1and3and4ofN<Nary,Arg1,Arg3,Arg4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1and3and4ofN<Nary,Arg1,Arg3,Arg4> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg1, class Arg3, class Arg5>
	class debug_traits<variadic_fcpp::binder1and3and5ofN<Nary,Arg1,Arg3,Arg5> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1and3and5ofN<Nary,Arg1,Arg3,Arg5> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg1, class Arg4, class Arg5>
	class debug_traits<variadic_fcpp::binder1and4and5ofN<Nary,Arg1,Arg4,Arg5> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1and4and5ofN<Nary,Arg1,Arg4,Arg5> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg2, class Arg3, class Arg4>
	class debug_traits<variadic_fcpp::binder2and3and4ofN<Nary,Arg2,Arg3,Arg4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder2and3and4ofN<Nary,Arg2,Arg3,Arg4> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg1, class Arg2, class Arg3, class Arg5>
	class debug_traits<variadic_fcpp::binder1and2and3and5ofN<Nary,Arg1,Arg2,Arg3,Arg5> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder1and2and3and5ofN<Nary,Arg1,Arg2,Arg3,Arg5> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg2, class Arg3, class Arg4, class Arg5>
	class debug_traits<variadic_fcpp::binder2and3and4and5ofN<Nary,Arg2,Arg3,Arg4,Arg5> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder2and3and4and5ofN<Nary,Arg2,Arg3,Arg4,Arg5> &f)
            { return f.name(); }
       };

      template <class Nary, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
	class debug_traits<variadic_fcpp::binder2and3and4and5and6ofN<Nary,Arg2,Arg3,Arg4,Arg5,Arg6> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binder2and3and4and5and6ofN<Nary,Arg2,Arg3,Arg4,Arg5,Arg6> &f)
            { return f.name(); }
       };

      template <class Nary, class Binder, class Arg1>
	class debug_traits<variadic_fcpp::binderAofN<Nary,Binder,Arg1> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binderAofN<Nary,Binder,Arg1> &f)
            { return f.name(); }
       };

      template <class Nary, class Pair>
	class debug_traits<variadic_fcpp::binderXofN<Nary,Pair> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binderXofN<Nary,Pair> &f)
            { return f.name(); }
       };

      template <class Nary, typename... Args>
      class debug_traits<variadic_fcpp::binderMofN<Nary,Args...> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::binderMofN<Nary,Args...> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<variadic_fcpp::PlusN>
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::PlusN &f) { return f.name(); }
      };

      template<>
      class debug_traits<variadic_fcpp::PtrToFunN>
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::PtrToFunN &f) { return f.name(); }
      };

      template<>
      class debug_traits<variadic_fcpp::Bind1ofN>
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::Bind1ofN &f) { return f.name(); }
      };

      template<>
      class debug_traits<variadic_fcpp::MakeTupleN>
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::MakeTupleN &f)
       { return f.name(); }
      };

      template<>
      class debug_traits<variadic_fcpp::ComposeN>
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::ComposeN &f)
       { return f.name(); }
      };

      template<>
      class debug_traits<variadic_fcpp::ComposeN2>
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::ComposeN2 &f)
       { return f.name(); }
      };

      template<>
      class debug_traits<variadic_fcpp::ComposeNN>
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::ComposeNN &f)
       { return f.name(); }
      };

      template<class F,class G>
	class debug_traits<variadic_fcpp::XComposeN<F,G> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::XComposeN<F,G> &f) { return f.name(); }
      };

      template<class F,class G, class H>
	class debug_traits<variadic_fcpp::XComposeN2<F,G,H> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::XComposeN2<F,G,H> &f) { return f.name(); }
      };

      template<class F,class G, class H, class I>
	class debug_traits<variadic_fcpp::XComposeN3<F,G,H,I> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::XComposeN3<F,G,H,I> &f) { return f.name(); }
      };

      template<class F,class G, class H, class I, class J>
	class debug_traits<variadic_fcpp::XComposeN4<F,G,H,I,J> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const variadic_fcpp::XComposeN4<F,G,H,I,J> &f) { return f.name(); }
      };


#endif


  }  //  end namespace traits
} // end namespace fcpp

#endif
