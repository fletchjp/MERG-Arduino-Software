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

// functors_debug.h is to take all the argument_traits and
// debug_traits definitions from functors.h

#ifndef FCPP_FUNCTORS_DEBUG_DOT_H
#define FCPP_FUNCTORS_DEBUG_DOT_H

namespace fcpp {
  namespace traits {

#ifdef FCPP_DEBUG

template <>
  class debug_traits<impl::XFmap>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XFmap &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XBimap>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XBimap &f) { return f.name(); }
      };

#ifdef FCPP152

     template<>
      class debug_traits<impl::XCoFmap>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XCoFmap &f)
        { return f.name(); }
       };
    template<>
       class debug_traits<impl::XContraFmap>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XContraFmap &f)
        { return f.name(); }
       };
    template<>
       class debug_traits<impl::XContraFmap2>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XContraFmap2 &f)
        { return f.name(); }
       };
    template<>
       class debug_traits<impl::XXmap>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XXmap &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XXmap2>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XXmap2 &f) { return f.name(); }
       };

#endif //FCPP152

    template<>
       class debug_traits<impl::XStar>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XStar &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XPure>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XPure &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XPureM>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XPureM &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XPureE>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XPureE &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XPureL>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XPureL &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XPure2>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XPure2 &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XStar2>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XStar2 &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XStar2a>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XStar2a &f) { return f.name(); }
       };

    template<class Functor>
       class debug_traits<impl::XPureA<Functor> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XPureA<Functor> &f)
        { return f.name(); }
       };

    template<class Functor>
       class debug_traits<impl::XStarA<Functor> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XStarA<Functor> &f)
        { return f.name(); }
       };

    template<>
       class debug_traits<impl::XNothing>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XNothing &f) { return f.name(); }
       };
    template<>
       class debug_traits<MaybeA>
       {
       public:
        static const bool has_name = true;
        static std::string name(const MaybeA &f) { return f.name(); }
       };
 
    template<>
      class debug_traits<MaybeA::XStar>
       {
       public:
        static const bool has_name = true;
        static std::string name(const MaybeA::XStar &f) { return f.name(); }
       };

#ifdef FCPP_UNIFY
    template<>
      class debug_traits<MaybeA::XBind>
       {
       public:
        static const bool has_name = true;
        static std::string name(const MaybeA::XBind &f) { return f.name(); }
       };

    template<>
      class debug_traits<MaybeA::XJoin>
       {
       public:
	static const bool has_name = true;
	static std::string name(const MaybeA::XJoin &f) { return f.name(); }
       };

    template<>
      class debug_traits<MaybeA::XPlus>
       {
       public:
        static const bool has_name = true;
        static std::string name(const MaybeA::XPlus &f) { return f.name(); }
       };

    template<>
      class debug_traits<EitherA::XBind>
       {
       public:
        static const bool has_name = true;
        static std::string name(const EitherA::XBind &f) { return f.name(); }
       };

    template<>
      class debug_traits<EitherA::XPlus>
       {
       public:
        static const bool has_name = true;
        static std::string name(const EitherA::XPlus &f) { return f.name(); }
       };
#endif

    template<>
       class debug_traits<EitherA>
       {
       public:
        static const bool has_name = true;
        static std::string name(const EitherA &f) { return f.name(); }
       };

    template<>
      class debug_traits<EitherA::XStar>
       {
       public:
        static const bool has_name = true;
        static std::string name(const EitherA::XStar &f) { return f.name(); }
       };

    template<>
       class debug_traits<ListA>
       {
       public:
        static const bool has_name = true;
        static std::string name(const ListA &f) { return f.name(); }
       };

    template<>
      class debug_traits<ListA::XPure>
       {
       public:
        static const bool has_name = true;
        static std::string name(const ListA::XPure &f) { return f.name(); }
       };
 
    template<>
      class debug_traits<ListA::XStar>
       {
       public:
        static const bool has_name = true;
        static std::string name(const ListA::XStar &f) { return f.name(); }
       };

#ifdef FCPP_UNIFY
    template<>
      class debug_traits<ListA::XUnit>
       {
       public:
        static const bool has_name = true;
        static std::string name(const ListA::XUnit &f) { return f.name(); }
       };

    template<>
      class debug_traits<ListA::XBind>
       {
       public:
        static const bool has_name = true;
        static std::string name(const ListA::XBind &f) { return f.name(); }
       };

    template<>
      class debug_traits<ListA::XBind_>
       {
       public:
	static const bool has_name = true;
	static std::string name(const ListA::XBind_ &f) { return f.name(); }
       };
#endif

    template<class Functor>
    class debug_traits<EitherF<Functor> >
       {
       public:
         static const bool has_name = true;
         static std::string name(const EitherF<Functor> &f) { return f.name(); }
       };

    template<class Functor>
    class debug_traits<MaybeF<Functor> >
       {
       public:
         static const bool has_name = true;
         static std::string name(const MaybeF<Functor> &f) { return f.name(); }
       };

      // This is how to do the debug_traits for these.
      // There has to be a separate trait for each functor.
      // I have made the name() return the string for the name.
      // It is the first time I have done this.
    template<>
    class debug_traits<typename MaybeF<ListA>::XPure >
       {
       public:
         static const bool has_name = true;
         static std::string name(const typename MaybeF<ListA>::XPure &f) { return f.name("ListA"); }
       };

    template<>
    class debug_traits<typename MaybeF<ListA>::XStar >
       {
       public:
         static const bool has_name = true;
         static std::string name(const typename MaybeF<ListA>::XStar &f) { return f.name("ListA"); }
       };

      // This is how to do the debug_traits for these.
      // There has to be a separate trait for each functor.
      // I have made the name() return the string for the name.
      // It is the first time I have done this.
    template<>
    class debug_traits<typename EitherF<ListA>::XPure >
       {
       public:
         static const bool has_name = true;
         static std::string name(const typename EitherF<ListA>::XPure &f) { return f.name("ListA"); }
       };

    template<>
    class debug_traits<typename EitherF<ListA>::XStar >
       {
       public:
         static const bool has_name = true;
         static std::string name(const typename EitherF<ListA>::XStar &f) { return f.name("ListA"); }
       };
 
#endif //FCPP_DEBUG

  }  //  end namespace traits
} // end namespace fcpp

#endif
