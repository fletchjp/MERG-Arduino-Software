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

#ifndef FCPP_PRELUDE_DEBUG_DOT_H
#define FCPP_PRELUDE_DEBUG_DOT_H

namespace fcpp {
  namespace traits {

       template<>
       class argument_traits<Empty >
       {
       public:
        static std::string value(const Empty &e)
        { std::string null; return null;}
       };

       template<class F>
	 class argument_traits<impl::XUncurryable<F> >
       {
       public:
	 //static const bool has_name = true;
	 static std::string value(const impl::XUncurryable<F> &f)
	{ std::string null; return null;}
       };

       template<class F>
	 class argument_traits<impl::XUncurryable0<F> >
       {
       public:
        static std::string value(const impl::XUncurryable0<F> &f)
	{ std::string null; return null;}
       };

#ifdef FCPP_DEBUG

       template <class F, class G>
       class debug_traits<impl::XCompose0Helper<F,G> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XCompose0Helper<F,G> &f)
        { return f.name(); }
       };

       template <class F, class G>
       class debug_traits<impl::XCompose1Helper<F,G> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XCompose1Helper<F,G> &f)
        { return f.name(); }
       };

       template <class F, class G>
       class debug_traits<impl::XCompose2Helper<F,G> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XCompose2Helper<F,G> &f)
        { return f.name(); }
       };

       template <class F, class G>
       class debug_traits<impl::XCompose3Helper<F,G> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XCompose3Helper<F,G> &f)
        { return f.name(); }
       };

// FC++.1.5.2
#ifdef FCPP152
       template <class F, class G>
       class debug_traits<impl::XCompose4Helper<F,G> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XCompose4Helper<F,G> &f)
        { return f.name(); }
       };
#endif

       template<>
       class debug_traits<impl::XCompose>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XCompose &f) { return f.name(); }
       };

       template <class F, class G>
       class debug_traits<impl::XInvCompose0Helper<F,G> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XInvCompose0Helper<F,G> &f)
        { return f.name(); }
       };

       template <class F, class G>
       class debug_traits<impl::XInvCompose1Helper<F,G> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XInvCompose1Helper<F,G> &f)
        { return f.name(); }
       };

       template <class F, class G>
       class debug_traits<impl::XInvCompose2Helper<F,G> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XInvCompose2Helper<F,G> &f)
        { return f.name(); }
       };

       template <class F, class G>
       class debug_traits<impl::XInvCompose3Helper<F,G> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XInvCompose3Helper<F,G> &f)
        { return f.name(); }
       };

       template<>
       class debug_traits<impl::XInvCompose>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XInvCompose &f) { return f.name(); }
       };

       template<>
       class debug_traits<impl::XOf>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XOf &f) { return f.name(); }
       };
       template<>

       class debug_traits<impl::XInvOf>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XInvOf &f) { return f.name(); }
       };

  template <class F, class G, class H, class I>
    class debug_traits<impl::X3Compose0Helper<F,G,H,I> >
       {
       public:
        static const bool has_name = true;
        static std::string name
        (const impl::X3Compose0Helper<F,G,H,I> &f)
        { return f.name(); }
       };

  template <class F, class G, class H, class I, class J>
    class debug_traits<impl::X4Compose0Helper<F,G,H,I,J> >
       {
       public:
        static const bool has_name = true;
        static std::string name
        (const impl::X4Compose0Helper<F,G,H,I,J> &f)
        { return f.name(); }
       };

  template <class F, class G, class H>
    class debug_traits<impl::XXCompose0Helper<F,G,H> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XXCompose0Helper<F,G,H> &f)
        { return f.name(); }
       };

  template <class F, class G, class H>
    class debug_traits<impl::XXCompose1Helper<F,G,H> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XXCompose1Helper<F,G,H> &f)
        { return f.name(); }
       };

  template <class F, class G, class H>
    class debug_traits<impl::XXCompose2Helper<F,G,H> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XXCompose2Helper<F,G,H> &f)
        { return f.name(); }
       };

  template <class F, class G, class H>
    class debug_traits<impl::XX2Compose1Helper<F,G,H> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XX2Compose1Helper<F,G,H> &f)
        { return f.name(); }
       };

  template <class F, class G, class H>
    class debug_traits<impl::XX2Compose12Helper<F,G,H> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XX2Compose12Helper<F,G,H> &f)
        { return f.name(); }
       };

  template <class F, class G, class H>
    class debug_traits<impl::XX2Compose21Helper<F,G,H> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XX2Compose21Helper<F,G,H> &f)
        { return f.name(); }
       };

#ifdef FCPP152
  template <class F, class G, class H>
    class debug_traits<impl::XX2Compose2Helper<F,G,H> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XX2Compose2Helper<F,G,H> &f)
        { return f.name(); }
       };
#endif

       template<>
       class debug_traits<impl::XCompose2>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XCompose2 &f) { return f.name(); }
       };

       template<>
       class debug_traits<impl::XCompose2xy>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XCompose2xy &f) { return f.name(); }
       };

template <>
  class debug_traits<impl::XInit>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XInit &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XLength>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XLength &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XConcat>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XConcat &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XFoldr>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XFoldr &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XFoldr1>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XFoldr1 &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XFoldl>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XFoldl &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XFoldl1>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XFoldl1 &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XScanr>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XScanr &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XScanrl>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XScanrl &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XScanr1>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XScanr1 &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XScanl>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XScanl &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XScanl1>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XScanl1 &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XIterate>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XIterate &f) { return f.name(); }
      };

    template<>
       class debug_traits<impl::XId>
       {
       public:
         static const bool has_name = true;
         static std::string name(const impl::XId &f) { return f.name(); }
       };

template <>
  class debug_traits<impl::XRepeat>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XRepeat &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XTake>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XTake &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XDrop>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XDrop &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XCycle>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XCycle &f) { return f.name(); }
      };

    template<>
       class debug_traits<impl::XSpan>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XSpan &f) { return f.name(); }
       };

   template<>
       class debug_traits<impl::XBreak>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XBreak &f) { return f.name(); }
       };

     template<class Binary>
       class debug_traits<impl::XFlipHelper<Binary> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XFlipHelper<Binary> &f)
           { return f.name(); }
       };
   template<>
       class debug_traits<impl::XFlip>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XFlip &f) { return f.name(); }
       };

   template<>
       class debug_traits<impl::XReverse>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XReverse &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XReverse1>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XReverse1 &f) { return f.name(); }
       };

template <>
   class debug_traits<impl::XFst>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XFst &f) { return f.name(); }
      };

template <>
  class debug_traits<impl::XSnd>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XSnd &f) { return f.name(); }
      };

      template<class P, class T>
       class debug_traits<impl::XFilterHelp<P,T> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XFilterHelp<P,T> &e)
        { return e.name(); }
       };

      template<class P, class T>
      class debug_traits<impl::XFilterHelpNot<P,T> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XFilterHelpNot<P,T> &e)
        { return e.name(); }
       };

     template<>
       class debug_traits<impl::XFilter>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XFilter &e) { return e.name(); }
       };

     template<>
       class debug_traits<impl::XFilter2>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XFilter2 &e) { return e.name(); }
       };


     template<class T>
       class debug_traits<impl::XEFH<T> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XEFH<T> &e) { return e.name(); }
       };

     template<>
       class debug_traits<impl::XEnumFrom>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XEnumFrom &e) { return e.name(); }
       };

     template<class T>
       class debug_traits<impl::XEFTH<T> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XEFTH<T> &e) { return e.name(); }
       };

     template<>
       class debug_traits<impl::XEnumFromTo>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XEnumFromTo &e) { return e.name(); }
       };

     template<class T>
       class debug_traits<Maybe<T> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const Maybe<T> &e) { return e.name(); }
       };

     template<>
       class debug_traits<impl::XJust >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XJust &e) { return e.name(); }
       };

     template<>
       class debug_traits<AUniqueTypeForNothing >
       {
       public:
        static const bool has_name = true;
        static std::string name(const AUniqueTypeForNothing &e) { return e.name(); }
       };

       template<class T>
       class debug_traits<Either<T> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const Either<T> &e) { return e.name(); }
       };

       template<>
       class debug_traits<impl::XRight >
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XRight &e) { return e.name(); }
       };

     template<class T>
       class debug_traits<Identity<T> >
       {
       public:
        static const bool has_name = true;
        static std::string name(const Identity<T> &e) { return e.name(); }
       };
     template<>
       class debug_traits<impl::XIdent>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XIdent &f) { return f.name(); }
       };

     template<>
       class debug_traits<Empty>
       {
       public:
        static const bool has_name = true;
        static std::string name(const Empty &e) { return e.name(); }
       };

    template<>
       class debug_traits<impl::XNoOp>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNoOp &f) { return f.name(); }
       };

     template<>
       class debug_traits< impl::XIntZero>
       {
       public:
        static const bool has_name = true;
        static std::string name(const impl::XIntZero &e) { return e.name(); }
       };

     template<>
       class debug_traits< impl::XIntOne>
       {
       public:
        static const bool has_name = true;
        static std::string name(const  impl::XIntOne &e) { return e.name(); }
       };

     template<>
       class debug_traits< impl::XIntTwo>
       {
       public:
        static const bool has_name = true;
        static std::string name(const  impl::XIntTwo &e) { return e.name(); }
       };

       template<>
       class debug_traits<impl::XEmptify>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XEmptify &f) { return f.name(); }
       };

      template <class F>
	class debug_traits<impl::XUncurryable<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XUncurryable<F> &f) { return f.name(); }
      };

       template<>
       class debug_traits<impl::XUncurry>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XUncurry &f) { return f.name(); }
       };

      template <class F>
	class debug_traits<impl::XUncurryable0<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XUncurryable0<F> &f) { return f.name(); }
      };

       template<>
       class debug_traits<impl::XUncurry0>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XUncurry0 &f) { return f.name(); }
       };

       template <class F>
	class debug_traits<impl::XDuplicater<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::XDuplicater<F> &f)
       { return f.name(); }
      };

    template<>
       class debug_traits<impl::XDuplicate>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XDuplicate &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XIgnore>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XIgnore &f) { return f.name(); }
       };
    template<>
       class debug_traits<impl::XIgnore2>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XIgnore &f) { return f.name(); }
       };
    template<class F>
       class debug_traits<impl::XIgnorer1<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XIgnorer1<F> &f) { return f.name(); }
       };
    template<class F>
       class debug_traits<impl::XIgnorer2<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XIgnorer2<F> &f) { return f.name(); }
       };
    template<class F>
       class debug_traits<impl::XIgnorer22<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XIgnorer22<F> &f) { return f.name(); }
       };
    template<class F>
       class debug_traits<impl::XIgnorer3<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XIgnorer3<F> &f) { return f.name(); }
       };
    template<class F>
       class debug_traits<impl::XIgnorer23<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XIgnorer23<F> &f) { return f.name(); }
       };

#ifdef FCPP152
   template<class F>
       class debug_traits<impl::XIgnorer4<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XIgnorer4<F> &f) { return f.name();        }
       };

    template<class F>
       class debug_traits<impl::XIgnorer24<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XIgnorer24<F> &f) { return f.name(); }
       };

   template<class F>
       class debug_traits<impl::XIgnorer5<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XIgnorer5<F> &f) { return f.name();        }
       };

    template<class F>
       class debug_traits<impl::XIgnorer25<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XIgnorer25<F> &f) { return f.name(); }
       };
#endif
    template<class T>
       class debug_traits<impl::XConstruct0<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XConstruct0<T> &f)
        { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XConstruct1<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XConstruct1<T> &f)
        { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XConstruct2<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XConstruct2<T> &f)
        { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XConstruct3<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XConstruct3<T> &f)
        { return f.name(); }
       };
#ifdef FCPP152
    template<class T>
       class debug_traits<impl::XConstruct4<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XConstruct4<T> &f)
        { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XConstruct5<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XConstruct5<T> &f)
        { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XConstruct6<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XConstruct6<T> &f)
        { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XConstruct7<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XConstruct7<T> &f)
        { return f.name(); }
       };
#endif

    template<class T>
       class debug_traits<impl::XNew0<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNew0<T> &f) { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XNew1<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNew1<T> &f) { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XNew2<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNew2<T> &f) { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XNew3<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNew3<T> &f) { return f.name(); }
       };
#ifdef FCPP152
    template<class T>
       class debug_traits<impl::XNew4<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNew4<T> &f) { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XNew5<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNew5<T> &f) { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XNew6<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNew6<T> &f) { return f.name(); }
       };
    template<class T>
       class debug_traits<impl::XNew7<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XNew7<T> &f) { return f.name(); }
       };
#endif

    template<>
       class debug_traits<impl::XMap>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XMap &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XFor>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XFor &f) { return f.name(); }
       };

    template<>
       class debug_traits<impl::XThunkFuncToFunc>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XThunkFuncToFunc &f) { return f.name(); }
       };
    template<class F>
      class debug_traits<impl::XThunkFuncToFunc::TFTFH0<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XThunkFuncToFunc::TFTFH0<F> &f) { return f.name(); }
       };
    template<class F>
      class debug_traits<impl::XThunkFuncToFunc::TFTFH1<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XThunkFuncToFunc::TFTFH1<F> &f) { return f.name(); }
       };
    template<class F>
      class debug_traits<impl::XThunkFuncToFunc::TFTFH2<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XThunkFuncToFunc::TFTFH2<F> &f) { return f.name(); }
       };
    template<class F>
      class debug_traits<impl::XThunkFuncToFunc::TFTFH3<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XThunkFuncToFunc::TFTFH3<F> &f) { return f.name(); }
       };
#ifdef FCPP152
    template<class F>
      class debug_traits<impl::XThunkFuncToFunc::TFTFH4<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XThunkFuncToFunc::TFTFH4<F> &f) { return f.name(); }
       };
    template<class F>
      class debug_traits<impl::XThunkFuncToFunc::TFTFH5<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XThunkFuncToFunc::TFTFH5<F> &f) { return f.name(); }
       };
    template<class F>
      class debug_traits<impl::XThunkFuncToFunc::TFTFH6<F> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XThunkFuncToFunc::TFTFH6<F> &f) { return f.name(); }
       };

#endif
#endif

  }  //  end namespace traits
} // end namespace fcpp

#endif
