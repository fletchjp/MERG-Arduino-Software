// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_DEBUG_DOT_H
#define FCPP_CURRY_DEBUG_DOT_H

// These are in namespace fcpp (called from curry.h)

//#ifdef FCPP_DEBUG
   namespace traits {
     // argument traits now defined outside FCPP_DEBUG
     // This is for debug purposes working with CurryN
     // which does not support autocurrying.
       template<>
	 class argument_traits<AutoCurryType >
       {
       public:
	 //static const bool has_name = true;
	static std::string value(const AutoCurryType &f)
	{ return std::string("_"); }
       };

       template<class F>
	 class argument_traits<Curryable2<F> >
       {
       public:
	 //static const bool has_name = true;
	static std::string value(const Curryable2<F> &f)
	{ std::string null; return null;}
       };

       template<class F>
       class argument_traits<Curryable3<F> >
       {
       public:
	 //static const bool has_name = true;
	static std::string value(const Curryable3<F> &f)
	{ std::string null; return null;}
       };

       template<>
	 class argument_traits<impl::Curry1 >
       {
       public:
	 //static const bool has_name = true;
	static std::string value(const Curry1 &f)
	{ std::string null; return null;}
       };
 
       template<>
	 class argument_traits<impl::Curry2 >
       {
       public:
	 //static const bool has_name = true;
	static std::string value(const Curry2 &f)
	{ std::string null; return null;}
       };
 
       template<>
	 class argument_traits<impl::Curry3 >
       {
       public:
	static std::string value(const Curry3 &f)
	{ std::string null; return null;}
       };

#ifdef FCPP152
       template<>
	 class argument_traits<impl::Curry4 >
       {
       public:
	static std::string value(const Curry4 &f)
	{ std::string null; return null;}
       };

         template<>
	 class argument_traits<impl::Curry5 >
       {
       public:
	static std::string value(const Curry5 &f)
	{ std::string null; return null;}
       };

         template<>
	 class argument_traits<impl::Curry6 >
       {
       public:
	static std::string value(const Curry6 &f)
	{ std::string null; return null;}
       };

         template<>
	 class argument_traits<impl::Curry7 >
       {
       public:
	static std::string value(const Curry7&f)
	{ std::string null; return null;}
       };
#endif
   }

std::ostream & operator << (std::ostream &o, const impl::Curry2 &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full0 functoid ****";
#endif
  return o;
}

std::ostream & operator << (std::ostream &o, const impl::Curry3 &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full0 functoid ****";
#endif
  return o;
}

#ifdef FCPP152
std::ostream & operator << (std::ostream &o, const impl::Curry4 &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full0 functoid ****";
#endif
  return o;
}

std::ostream & operator << (std::ostream &o, const impl::Curry5 &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full0 functoid ****";
#endif
  return o;
}

std::ostream & operator << (std::ostream &o, const impl::Curry6 &f)
{
#ifdef FCPP_DEBUG
  o << "**** ERROR: Attempt to output " << f.name();
#else
  o << "**** ERROR: Attempt to output a Full0 functoid ****";
#endif
  return o;
}

#endif

#ifdef FCPP_DEBUG
   namespace traits {

     /* The traits specialisations for FullN should be in full.h */
     /*     template <class F>
      class debug_traits<Full1<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Full1<F> &f) { return f.name(); }
      };

      template <class F>
      class debug_traits<Full2<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Full2<F> &f) { return f.name(); }
      };
     */


    template<>
       class debug_traits<impl::Const>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::Const &f) { return f.name(); }
       };
     template<class T>
       class debug_traits<impl::ConstHelper<T> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::ConstHelper<T> &f) { return f.name(); }
       };

   template<>
       class debug_traits<impl::Konst>
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::Konst &f) { return f.name(); }
       };

    template<class X>
       class debug_traits<impl::XkonstHelp<X> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const impl::XkonstHelp<X> &f) { return f.name(); }
       };

      template <>
      class debug_traits<impl::Curry >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Curry &f)
            { return f.name(); }
       };

      template <>
      class debug_traits<impl::Curry1 >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Curry1 &f)
            { return f.name(); }
       };

      template <>
      class debug_traits<impl::Curry2 >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Curry2 &f)
            { return f.name(); }
       };

      template <>
      class debug_traits<impl::Curry3 >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Curry3 &f)
            { return f.name(); }
       };


      template <class F>
      class debug_traits<Curryable3<F> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Curryable3<F> &f) { return f.name(); }
      };
     
      template <class Unary, class Arg>
      class debug_traits<impl::binder1of1<Unary,Arg> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1of1<Unary,Arg> &f)
            { return f.name(); }
       };

      template <class Binary, class Arg1>
      class debug_traits<impl::binder1of2<Binary,Arg1> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1of2<Binary,Arg1> &f)
            { return f.name(); }
       };

      template <class Binary, class Arg2>
      class debug_traits<impl::binder2of2<Binary,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder2of2<Binary,Arg2> &f)
            { return f.name(); }
       };

      template <class Binary, class Arg1, class Arg2>
	class debug_traits<impl::binder1and2of2<Binary,Arg1,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1and2of2<Binary,Arg1,Arg2> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1of2>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1of2 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind2of2>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind2of2 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1and2of2>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1and2of2 &f)
            { return f.name(); }
      };

      template<>
	class debug_traits<impl::Cycle2>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Cycle2 &f)
            { return f.name(); }
      };

      //#ifdef FCPP_EXTRA_STRONG_CURRY

      template <class Binary>
      class debug_traits<impl::cycler2<Binary> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::cycler2<Binary> &f)
            { return f.name(); }
      };

      //#endif

    template <class Ternary, class Arg1, class Arg2, class Arg3>
      class debug_traits<impl::binder1and2and3of3<Ternary,Arg1,Arg2,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1and2and3of3<Ternary,Arg1,Arg2,Arg3> &f) 
            { return f.name(); }
       };
 
    template <class Ternary, class Arg1, class Arg2>
      class debug_traits<impl::binder1and2of3<Ternary,Arg1,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1and2of3<Ternary,Arg1,Arg2> &f) 
            { return f.name(); }
       };

    template <class Ternary, class Arg2, class Arg3>
      class debug_traits<impl::binder2and3of3<Ternary,Arg2,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder2and3of3<Ternary,Arg2,Arg3> &f) 
            { return f.name(); }
       };

    template <class Ternary, class Arg1, class Arg3>
      class debug_traits<impl::binder1and3of3<Ternary,Arg1,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1and3of3<Ternary,Arg1,Arg3> &f) 
            { return f.name(); }
       };

     template <class Ternary, class Arg1>
      class debug_traits<impl::binder1of3<Ternary,Arg1> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1of3<Ternary,Arg1> &f) 
            { return f.name(); }
       };

      template <class Ternary, class Arg2>
      class debug_traits<impl::binder2of3<Ternary,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder2of3<Ternary,Arg2> &f) 
            { return f.name(); }
       };

      template <class Ternary, class Arg3>
      class debug_traits<impl::binder3of3<Ternary,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder3of3<Ternary,Arg3> &f) 
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1of3>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1of3 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind2of3>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind2of3 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind3of3>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind3of3 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1and2of3>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1and2of3 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1and3of3>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1and3of3 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind2and3of3>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind2and3of3 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1and2and3of3>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1and2and3of3 &f)
            { return f.name(); }
      };

      template <class Ternary>
      class debug_traits<impl::cycler3<Ternary> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::cycler3<Ternary> &f)
            { return f.name(); }
      };

      template <class Ternary>
      class debug_traits<impl::cycler3r<Ternary> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::cycler3r<Ternary> &f)
            { return f.name(); }
      };

#ifdef FCPP152
     template <class Quaternary, class Arg1>
      class debug_traits<impl::binder1of4<Quaternary,Arg1> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1of4<Quaternary,Arg1> &f)
            { return f.name(); }
       };

     template <class Quaternary, class Arg2>
      class debug_traits<impl::binder2of4<Quaternary,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder2of4<Quaternary,Arg2> &f)
            { return f.name(); }
       };
     template <class Quaternary, class Arg3>
      class debug_traits<impl::binder3of4<Quaternary,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder3of4<Quaternary,Arg3> &f)
            { return f.name(); }
       };
     template <class Quaternary, class Arg4>
      class debug_traits<impl::binder4of4<Quaternary,Arg4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder4of4<Quaternary,Arg4> &f)
            { return f.name(); }
       };

     template <class Quaternary, class Arg1, class Arg2>
       class debug_traits<impl::binder1and2of4<Quaternary,Arg1,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1and2of4<Quaternary,Arg1,Arg2> &f)
            { return f.name(); }
       };

     template <class Quaternary, class Arg1, class Arg2, class Arg3>
     class debug_traits<impl::binder1and2and3of4<Quaternary,Arg1,Arg2,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1and2and3of4<Quaternary,Arg1,Arg2,Arg3> &f)
            { return f.name(); }
       };

     template <class Quaternary, class Arg1, class Arg2, class Arg3, class Arg4>
       class debug_traits<impl::binder1to4of4<Quaternary,Arg1,Arg2,Arg3,Arg4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to4of4<Quaternary,Arg1,Arg2,Arg3,Arg4> &f)
            { return f.name(); }
       };

      template <>
      class debug_traits<impl::Curry4 >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Curry4 &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1of4>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1of4 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind2of4>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind2of4 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind3of4>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind3of4 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind4of4>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind4of4 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1and2of4>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1and2of4 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1and2and3of4>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1and2and3of4 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1to4of4>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to4of4 &f)
            { return f.name(); }
      };

     template <class Quintinary, class Arg1>
      class debug_traits<impl::binder1of5<Quintinary,Arg1> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1of5<Quintinary,Arg1> &f)
            { return f.name(); }
       };

     template <class Quintinary, class Arg2>
      class debug_traits<impl::binder2of5<Quintinary,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder2of5<Quintinary,Arg2> &f)
            { return f.name(); }
       };

     template <class Quintinary, class Arg1, class Arg2>
       class debug_traits<impl::binder1and2of5<Quintinary,Arg1,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1and2of5<Quintinary,Arg1,Arg2> &f)
            { return f.name(); }
       };

     template <class Quintinary,typename Binder,typename ArgA>
       class debug_traits<impl::binderAof5<Quintinary,Binder,ArgA> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderAof5<Quintinary,Binder,ArgA> &f)
            { return f.name(); }
       };

     template <class Quintinary,typename Pair>
       class debug_traits<impl::binderXof5<Quintinary,Pair> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXof5<Quintinary,Pair> &f)
            { return f.name(); }
       };

     template <class Quintinary,typename PairX,typename PairY>
       class debug_traits<impl::binderXandYof5<Quintinary,PairX,PairY> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXandYof5<Quintinary,PairX,PairY> &f)
            { return f.name(); }
       };

      template <class Quintinary, class Arg1, class Arg2, class Arg3>
	class debug_traits<impl::binder1to3of5<Quintinary,Arg1,Arg2,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to3of5<Quintinary,Arg1,Arg2,Arg3> &f)
            { return f.name(); }
       };

     template <class Quintinary,typename PairX,typename PairY,typename PairZ>
       class debug_traits<impl::binderXtoZof5<Quintinary,PairX,PairY,PairZ> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXtoZof5<Quintinary,PairX,PairY,PairZ> &f)
            { return f.name(); }
       };

     template <class Quintinary, class Arg1, class Arg2, class Arg3, class Arg4>
       class debug_traits<impl::binder1to4of5<Quintinary,Arg1,Arg2,Arg3,Arg4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to4of5<Quintinary,Arg1,Arg2,Arg3,Arg4> &f)
            { return f.name(); }
       };

     template <class Quintinary,typename PairW,typename PairX,
                               typename PairY,typename PairZ>
       class debug_traits<impl::binderWtoZof5<Quintinary,PairW,PairX,PairY,PairZ> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderWtoZof5<Quintinary,PairW,PairX,PairY,PairZ> &f)
            { return f.name(); }
       };

     template <class Quintinary, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
       class debug_traits<impl::binder1to5of5<Quintinary,Arg1,Arg2,Arg3,Arg4,Arg5> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to5of5<Quintinary,Arg1,Arg2,Arg3,Arg4,Arg5> &f)
            { return f.name(); }
       };

 
      template<>
      class debug_traits<impl::Bind1of5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1of5 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind2of5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind2of5 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::BindAof5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindAof5 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::BindXof5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXof5 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1and2of5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1and2of5 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::BindXandYof5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXandYof5 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1to3of5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to3of5 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::BindXtoZof5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXtoZof5 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1to4of5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to4of5 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::BindWtoZof5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindWtoZof5 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1to5of5>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to5of5 &f)
            { return f.name(); }
      };

      template <>
      class debug_traits<impl::Curry5 >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Curry5 &f)
            { return f.name(); }
       };

      template <>
      class debug_traits<impl::Curry6 >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Curry6 &f)
            { return f.name(); }
       };

      template <>
      class debug_traits<impl::Curry7 >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Curry7 &f)
            { return f.name(); }
       };

     template <class Sextinary, class Arg1>
      class debug_traits<impl::binder1of6<Sextinary,Arg1> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1of6<Sextinary,Arg1> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1of6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1of6 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::BindXof6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXof6 &f)
            { return f.name(); }
      };

      template <class Sextinary,typename PairX>
       class debug_traits<impl::binderXof6<Sextinary,PairX> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXof6<Sextinary,PairX> &f)
            { return f.name(); }
       };

     template <class Sextinary, class Arg1, class Arg2>
       class debug_traits<impl::binder1and2of6<Sextinary,Arg1,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1and2of6<Sextinary,Arg1,Arg2> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1and2of6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1and2of6 &f)
            { return f.name(); }
      };

     template <class Sextinary,typename PairX,typename PairY>
       class debug_traits<impl::binderXandYof6<Sextinary,PairX,PairY> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXandYof6<Sextinary,PairX,PairY> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindXandYof6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXandYof6 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1to3of6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to3of6 &f)
            { return f.name(); }
      };

      template <class Sextinary, class Arg1, class Arg2, class Arg3>
	class debug_traits<impl::binder1to3of6<Sextinary,Arg1,Arg2,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to3of6<Sextinary,Arg1,Arg2,Arg3> &f)
            { return f.name(); }
       };

     template <class Sextinary,typename PairX,typename PairY,typename PairZ>
       class debug_traits<impl::binderXtoZof6<Sextinary,PairX,PairY,PairZ> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXtoZof6<Sextinary,PairX,PairY,PairZ> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindXtoZof6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXtoZof6 &f)
            { return f.name(); }
      };

     template <class Sextinary, class Arg1, class Arg2, class Arg3, class Arg4>
       class debug_traits<impl::binder1to4of6<Sextinary,Arg1,Arg2,Arg3,Arg4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to4of6<Sextinary,Arg1,Arg2,Arg3,Arg4> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1to4of6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to4of6 &f)
            { return f.name(); }
      };

     template <class Sextinary,typename PairW,typename PairX,
                               typename PairY,typename PairZ>
       class debug_traits<impl::binderWtoZof6<Sextinary,PairW,PairX,PairY,PairZ> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderWtoZof6<Sextinary,PairW,PairX,PairY,PairZ> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindWtoZof6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindWtoZof6 &f)
            { return f.name(); }
      };

     template <class Sextinary, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
       class debug_traits<impl::binder1to5of6<Sextinary,Arg1,Arg2,Arg3,Arg4,Arg5> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to5of6<Sextinary,Arg1,Arg2,Arg3,Arg4,Arg5> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1to5of6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to5of6 &f)
            { return f.name(); }
      };

      template <class Sextinary,typename PairV,typename PairW,typename PairX,
                                typename PairY,typename PairZ>
	class debug_traits<impl::binderVtoZof6<Sextinary,PairV,PairW,PairX,
                                               PairY,PairZ> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderVtoZof6<Sextinary,PairV,
                               PairW,PairX,PairY,PairZ> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindVtoZof6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindVtoZof6 &f)
            { return f.name(); }
      };

     template <class Sextinary, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
       class debug_traits<impl::binder1to6of6<Sextinary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to6of6<Sextinary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1to6of6>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to6of6 &f)
            { return f.name(); }
      };

     template <class Septimary, class Arg1>
      class debug_traits<impl::binder1of7<Septimary,Arg1> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1of7<Septimary,Arg1> &f)
            { return f.name(); }
       };

     template <class Septimary, class Arg1, class Arg2>
       class debug_traits<impl::binder1and2of7<Septimary,Arg1,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1and2of7<Septimary,Arg1,Arg2> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1of7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1of7 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::BindXof7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXof7 &f)
            { return f.name(); }
      };

      template <class Septimary,typename PairX>
       class debug_traits<impl::binderXof7<Septimary,PairX> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXof7<Septimary,PairX> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1and2of7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1and2of7 &f)
            { return f.name(); }
      };

      template <class Septimary,typename PairX,typename PairY>
       class debug_traits<impl::binderXandYof7<Septimary,PairX,PairY> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXandYof7<Septimary,PairX,PairY> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindXandYof7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXandYof7 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1to3of7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to3of7 &f)
            { return f.name(); }
      };

      template <class Septimary, class Arg1, class Arg2, class Arg3>
	class debug_traits<impl::binder1to3of7<Septimary,Arg1,Arg2,Arg3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to3of7<Septimary,Arg1,Arg2,Arg3> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1to4of7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to4of7 &f)
            { return f.name(); }
      };

     template <class Septimary, class Arg1, class Arg2, class Arg3, class Arg4>
       class debug_traits<impl::binder1to4of7<Septimary,Arg1,Arg2,Arg3,Arg4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to4of7<Septimary,Arg1,Arg2,Arg3,Arg4> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindXtoZof7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXtoZof7 &f)
            { return f.name(); }
      };


     template <class Septimary,typename PairX,typename PairY,typename PairZ>
       class debug_traits<impl::binderXtoZof7<Septimary,PairX,PairY,PairZ> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXtoZof7<Septimary,PairX,PairY,PairZ> &f)
            { return f.name(); }
       };

     template <class Septimary,typename PairW,typename PairX,
                               typename PairY,typename PairZ>
       class debug_traits<impl::binderWtoZof7<Septimary,PairW,PairX,PairY,PairZ> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderWtoZof7<Septimary,PairW,PairX,PairY,PairZ> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindWtoZof7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindWtoZof7 &f)
            { return f.name(); }
      };

     template <class Septimary, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
       class debug_traits<impl::binder1to5of7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to5of7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1to5of7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to5of7 &f)
            { return f.name(); }
      };

      template <class Septimary,typename PairV,typename PairW,typename PairX,
                                typename PairY,typename PairZ>
	class debug_traits<impl::binderVtoZof7<Septimary,PairV,PairW,PairX,
                                               PairY,PairZ> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderVtoZof7<Septimary,PairV,
                               PairW,PairX,PairY,PairZ> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindVtoZof7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindVtoZof7 &f)
            { return f.name(); }
      };

      template <class Septimary,typename PairU,typename PairV,
                                typename PairW,typename PairX,
                                typename PairY,typename PairZ>
	class debug_traits<impl::binderUtoZof7<Septimary,PairU,PairV,
                                               PairW,PairX,
                                               PairY,PairZ> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderUtoZof7<Septimary,PairU,PairV,
                               PairW,PairX,PairY,PairZ> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindUtoZof7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindUtoZof7 &f)
            { return f.name(); }
      };

      template <class Septimary, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6>
	class debug_traits<impl::binder1to6of7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to6of7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1to6of7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to6of7 &f)
            { return f.name(); }
      };

      template<>
      class debug_traits<impl::Bind1to7of7>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1to7of7 &f)
            { return f.name(); }
      };

     template <class Septimary, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6, class Arg7>
       class debug_traits<impl::binder1to7of7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1to7of7<Septimary,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7> &f)
            { return f.name(); }
       };

     template <class Octavary, class Arg1>
      class debug_traits<impl::binder1of8<Octavary,Arg1> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1of8<Octavary,Arg1> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1of8>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1of8 &f)
            { return f.name(); }
      };

      template <class Octavary,typename PairX>
       class debug_traits<impl::binderXof8<Octavary,PairX> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXof8<Octavary,PairX> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindXof8>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXof8 &f)
            { return f.name(); }
      };

     template <class Octavary, class Arg1, class Arg2>
       class debug_traits<impl::binder1and2of8<Octavary,Arg1,Arg2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binder1and2of8<Octavary,Arg1,Arg2> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::Bind1and2of8>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::Bind1and2of8 &f)
            { return f.name(); }
      };

      template <class Octavary,typename PairX,typename PairY>
       class debug_traits<impl::binderXandYof8<Octavary,PairX,PairY> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXandYof8<Octavary,PairX,PairY> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindXandYof8>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXandYof8 &f)
            { return f.name(); }
      };

     template <class Octavary,typename PairX,typename PairY,typename PairZ>
       class debug_traits<impl::binderXtoZof8<Octavary,PairX,PairY,PairZ> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderXtoZof8<Octavary,PairX,PairY,PairZ> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindXtoZof8>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindXtoZof8 &f)
            { return f.name(); }
      };

     template <class Octavary,typename PairW,typename PairX,
                              typename PairY,typename PairZ>
       class debug_traits<impl::binderWtoZof8<Octavary,PairW,PairX,PairY,PairZ> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::binderWtoZof8<Octavary,PairW,PairX,PairY,PairZ> &f)
            { return f.name(); }
       };

      template<>
      class debug_traits<impl::BindWtoZof8>
      {
      public:
       static const bool has_name = true;
       static std::string name(const impl::BindWtoZof8 &f)
            { return f.name(); }
      };

#endif

   } //end namespace traits
#endif


#endif
