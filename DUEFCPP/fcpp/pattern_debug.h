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

// pattern_debug.h is to take all the debug_traits definitions from
// pattern.h

#ifndef FCPP_PATTERN_DEBUG_DOT_H
#define FCPP_PATTERN_DEBUG_DOT_H

namespace fcpp{
  namespace traits {

#ifdef FCPP_DEBUG

template <>
  class debug_traits<impl::XIterateSum>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XIterateSum &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XIterate2>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XIterate2 &f) { return f.name(); }
      };

    template<>
       class debug_traits<impl::XReverse2>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XReverse2 &f) { return f.name(); }
       };

    template<>
      class debug_traits<BootListM>
       {
       public:
	static const bool has_name = true;
	static std::string name(const BootListM &f) { return f.name(); }
       };

    template<class T>
      class debug_traits<Stream<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const Stream<T> &f) { return f.name(); }
       };

    template<>
      class debug_traits<StreamM>
       {
       public:
	static const bool has_name = true;
	static std::string name(const StreamM &f) { return f.name(); }
       };

    template<>
      class debug_traits<ParallelM>
       {
       public:
	static const bool has_name = true;
	static std::string name(const ParallelM &f) { return f.name(); }
       };

    template<class Monad>
    class debug_traits<MaybeT<Monad> >
       {
       public:
         static const bool has_name = true;
         static std::string name(const MaybeT<Monad> &f) { return f.name(); }
       };

      // This is how to do the debug_traits for these.
      // There has to be a separate trait for each monad.
      // I have made the name() return the string for the name.

    template<>
      class debug_traits<typename MaybeT<ListM>::XUnit>
       {
       public:
        static const bool has_name = true;
        static std::string name(const typename MaybeT<ListM>::XUnit &f)
        { return f.name("ListM"); }
       };

    template<>
      class debug_traits<typename MaybeT<ListM>::XBind>
       {
       public:
        static const bool has_name = true;
        static std::string name(const typename MaybeT<ListM>::XBind &f)
        { return f.name("ListM"); }
       };

    template<>
      class debug_traits<typename MaybeT<ListM>::XBindT>
       {
       public:
        static const bool has_name = true;
        static std::string name(const typename MaybeT<ListM>::XBindT &f)
        { return f.name("ListM"); }
       };

    template<typename Monad>
    class debug_traits<EitherT<Monad> >
       {
       public:
         static const bool has_name = true;
         static std::string name(const EitherT<Monad> &f) { return f.name(); }
       };

      // This is how to do the debug_traits for these.
      // There has to be a separate trait for each monad.
      // I have made the name() return the string for the name.

    template<>
      class debug_traits<typename EitherT<ListM>::XUnit>
       {
       public:
        static const bool has_name = true;
        static std::string name(const typename EitherT<ListM>::XUnit &f)
        { return f.name("ListM"); }
       };

    template<>
      class debug_traits<typename EitherT<ListM>::XBind>
       {
       public:
        static const bool has_name = true;
        static std::string name(const typename EitherT<ListM>::XBind &f)
        { return f.name("ListM"); }
       };

    template<>
      class debug_traits<typename EitherT<ListM>::XBindT>
       {
       public:
        static const bool has_name = true;
        static std::string name(const typename EitherT<ListM>::XBindT &f)
        { return f.name("ListM"); }
       };

    template<class Monad>
      class debug_traits<impl::XMapT<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XMapT<Monad> &f)
        { return f.name(); }
       };

    template<class Monad>
      class debug_traits<impl::XForT<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XForT<Monad> &f)
        { return f.name(); }
       };

    template<class Monad>
      class debug_traits<impl::XLiftT<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XLiftT<Monad> &f)
        { return f.name(); }
       };
 
    template<class Monad>
      class debug_traits<impl::XLiftT2<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XLiftT2<Monad> &f)
        { return f.name(); }
       };

    template<class Monad>
      class debug_traits<impl::XLiftT3<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XLiftT3<Monad> &f)
        { return f.name(); }
       };

    template<>
       class debug_traits<impl::XFirst>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XFirst &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XSecond>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XSecond &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XParallel>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XParallel &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XUnfold1>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XUnfold1 &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XUnfold2>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XUnfold2 &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XMapS>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XMapS &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XDiag>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XDiag &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XDiagS>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XDiagS &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XDouble>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XDouble &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XDouble1>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XDouble1 &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XDoublef>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XDoublef &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XRepeatS>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XRepeatS &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XTwoStreams>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XTwoStreams &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XGeom>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XGeom &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XArith>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XArith &f) { return f.name(); }
       };
    
    template<>
      class debug_traits<impl::XSnoc2 >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XSnoc2 &f) { return f.name(); }
       };

#endif

  }
}

#endif
