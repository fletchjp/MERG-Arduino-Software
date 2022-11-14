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

// monad_debug.h is to take all the argument_traits and
// debug_traits definitions from monad.h

#ifndef FCPP_MONAD_DEBUG_DOT_H
#define FCPP_MONAD_DEBUG_DOT_H

namespace fcpp {
  namespace traits {

    template <typename Monad>
    class argument_traits <fcpp_lambda::compM<Monad> > {
    public:
      static std::string value(const fcpp_lambda::compM<Monad> &cm)
      {
        return std::string("compM");
      }
    };


#ifdef FCPP_DEBUG
    template<class Monad>
       class debug_traits<impl::XUnitM<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XUnitM<Monad> &f)
        { return f.name(); }
       };

    template<class Monad>
       class debug_traits<impl::XBindM<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XBindM<Monad> &f)
        { return f.name(); }
       };

    template<class Monad>
       class debug_traits<impl::XBindM_<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XBindM_<Monad> &f)
        { return f.name(); }
       };

    template<class Monad>
       class debug_traits<ZeroM<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const ZeroM<Monad> &f) { return f.name(); }
       };

    template<class Monad>
      class debug_traits<impl::XPlusM<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XPlusM<Monad> &f)
        { return f.name(); }
       };

template <>
  class debug_traits<impl::XBind>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XBind &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XBind_>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XBind_ &f) { return f.name(); }
      };

    template<class Monad>
      class debug_traits<impl::XMapM<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XMapM<Monad> &f)
        { return f.name(); }
       };

    template<class Monad>
      class debug_traits<impl::XForM<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XForM<Monad> &f)
        { return f.name(); }
       };

    template<class Monad>
      class debug_traits<impl::XJoinM<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XJoinM<Monad> &f)
        { return f.name(); }
       };

template <>
  class debug_traits<impl::XJoin>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XJoin &f) { return f.name(); }
      };

template <typename Monad>
  class debug_traits<fcpp_lambda::compM<Monad> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const fcpp_lambda::compM<Monad> &f)
       { return f.name(); }
      };

    template<>
      class debug_traits<ListM>
       {
       public:
	static const bool has_name = true;
	static std::string name(const ListM &f) { return f.name(); }
	// This enables a name without an instance.
	//static std::string nameM() { return std::string("ListM"); }
       };

    template<>
      class debug_traits<ListM::XUnit>
       {
       public:
        static const bool has_name = true;
        static std::string name(const ListM::XUnit &f)
        { return f.name(); }
       };

   template<>
      class debug_traits<ListM::XBind>
       {
       public:
        static const bool has_name = true;
        static std::string name(const ListM::XBind &f)
        { return f.name(); }
       };

   template<>
      class debug_traits<ListM::XBind_>
       {
       public:
        static const bool has_name = true;
        static std::string name(const ListM::XBind_ &f)
        { return f.name(); }
       };

    template<>
      class debug_traits<MaybeM>
       {
       public:
	static const bool has_name = true;
	static std::string name(const MaybeM &f) { return f.name(); }
       };

    template<>
      class debug_traits<MaybeM::XBind>
       {
       public:
	static const bool has_name = true;
	static std::string name(const MaybeM::XBind &f) { return f.name(); }
       };

    template<>
      class debug_traits<MaybeM::XJoin>
       {
       public:
	static const bool has_name = true;
	static std::string name(const MaybeM::XJoin &f) { return f.name(); }
       };

    template<>
      class debug_traits<MaybeM::XPlus>
       {
       public:
	static const bool has_name = true;
	static std::string name(const MaybeM::XPlus &f) { return f.name(); }
       };

    template<>
      class debug_traits<EitherM>
       {
       public:
	static const bool has_name = true;
	static std::string name(const EitherM &f) { return f.name(); }
       };

    template<>
      class debug_traits<EitherM::XBind>
       {
       public:
	static const bool has_name = true;
	static std::string name(const EitherM::XBind &f) { return f.name(); }
       };

    template<>
      class debug_traits<EitherM::XPlus>
       {
       public:
	static const bool has_name = true;
	static std::string name(const EitherM::XPlus &f) { return f.name(); }
       };

    template<>
      class debug_traits<IdentityM>
       {
       public:
	static const bool has_name = true;
	static std::string name(const IdentityM &f) { return f.name(); }
       };

    template<>
      class debug_traits<IdentityM::XUnit>
       {
       public:
        static const bool has_name = true;
        static std::string name(const IdentityM::XUnit &f)
        { return f.name(); }
       };

    template<>
      class debug_traits<IdentityM::XBind>
       {
       public:
        static const bool has_name = true;
        static std::string name(const IdentityM::XBind &f)
        { return f.name(); }
       };

    template<>
      class debug_traits<IdentityM::XBind_>
       {
       public:
        static const bool has_name = true;
        static std::string name(const IdentityM::XBind_ &f)
        { return f.name(); }
       };

    template<>
      class debug_traits<IdentityInfM>
       {
       public:
	static const bool has_name = true;
	static std::string name(const IdentityInfM &f) { return f.name(); }
       };

    template<class State>
      class debug_traits<StateM<State> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const StateM<State> &f) { return f.name(); }
       };

    template<class Monad>
      class debug_traits<impl::XLiftM<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XLiftM<Monad> &f)
        { return f.name(); }
       };

    template<class Monad>
      class debug_traits<impl::XLiftM2<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XLiftM2<Monad> &f)
        { return f.name(); }
       };
    template<class Monad>
      class debug_traits<impl::XLiftM3<Monad> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XLiftM3<Monad> &f)
        { return f.name(); }
       };
#ifdef FCPP152
    // For LiftM4 and LiftM5 no debug information yet.
#endif

    template <int i, class LE>
	class debug_traits<fcpp_lambda::exp::Gets<i,LE> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::exp::Gets<i,LE> &f)
         { return f.name(); }
       };

    template <class LHS, class RHS>
      class debug_traits<fcpp_lambda::exp::VertPair<LHS,RHS> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::exp::VertPair<LHS,RHS> &f)
         { return f.name(); }
       };

    template <class BFLE>
      class debug_traits<fcpp_lambda::exp::Guard<BFLE> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::exp::Guard<BFLE> &f)
         { return f.name(); }
       };

    template<class T>
       class debug_traits<ByNeed<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const ByNeed<T> &f) { return f.name(); }
       };

     template<>
       class debug_traits<impl::XBLift>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XBLift &f) { return f.name(); }
       };

    template<class F>
       class debug_traits<impl::XBLifter0<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XBLifter0<F> &f) { return f.name(); }
       };

    template<class F>
       class debug_traits<impl::XBLifter1<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XBLifter1<F> &f) { return f.name(); }
       };

    template<class F>
       class debug_traits<impl::XBLifter2<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XBLifter2<F> &f) { return f.name(); }
       };

    template<class F>
       class debug_traits<impl::XBLifter3<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XBLifter3<F> &f) { return f.name(); }
       };

    template<>
      class debug_traits<ByNeedM>
       {
       public:
	static const bool has_name = true;
	static std::string name(const ByNeedM &f) { return f.name(); }
       };

#endif

  }  //  end namespace traits
} // end namespace fcpp

#endif
