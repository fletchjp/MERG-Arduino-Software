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

// function_debug.h is to take all the argument_traits and
// debug_traits definitions from function.h

#ifndef FCPP_FUNCTION_DEBUG_DOT_H
#define FCPP_FUNCTION_DEBUG_DOT_H

namespace fcpp {
  namespace traits {

#ifdef FCPP_DEBUG

     ///////////////////////////////////////////////
     //                    Fun0                   //
     ///////////////////////////////////////////////

      template <class R>
      class debug_traits<Fun0<R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun0<R> &f) { return f.name(); }
      };

      template <class R>
      class debug_traits<Fun0Impl<R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun0Impl<R> &f) { return f.name(); }
      };

     template <class Rd, class Rs>
       class debug_traits<Fun0Converter<Rd,Rs> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun0Converter<Rd,Rs> &f)
           { return f.name(); }
      };

     template <class Rd, class Rs>
       class debug_traits<Fun0ExplicitConverter<Rd,Rs> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun0ExplicitConverter<Rd,Rs> &f)
         { return f.name(); }
      };

      template <class Gen>
      class debug_traits<Gen0<Gen> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Gen0<Gen> &f) { return f.name(); }
      };

     template <class A, class B>
	class debug_traits<Fun0Constructor<A,B> >
      {
      public:
       static const bool has_name = true;
       static std::string name(Fun0Constructor<A,B> const &f)
       { return f.name(); }
      };

     ///////////////////////////////////////////////
     //                    MakeN                  //
     ///////////////////////////////////////////////

     template <class T>
       class debug_traits<Make0<T> >
      {
      public:
       static const bool has_name = true;
       static std::string name(Make0<T> const &f)
       { return f.name(); }
      };

     template <class T, class A1>
       class debug_traits<Make1<T,A1> >
      {
      public:
       static const bool has_name = true;
       static std::string name(Make1<T,A1> const &f)
       { return f.name(); }
      };

     template <class T, class A1, class A2>
       class debug_traits<Make2<T,A1,A2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(Make2<T,A1,A2> const &f)
       { return f.name(); }
      };

     template <class T, class A1, class A2, class A3>
       class debug_traits<Make3<T,A1,A2,A3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(Make3<T,A1,A2,A3> const &f)
       { return f.name(); }
      };
#ifdef FCPP152
     template <class T, class A1, class A2, class A3, class A4>
       class debug_traits<Make4<T,A1,A2,A3,A4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(Make4<T,A1,A2,A3,A4> const &f)
       { return f.name(); }
      };
#endif

     ///////////////////////////////////////////////
     //                    Proxy                  //
     ///////////////////////////////////////////////

     template <class T>
	class debug_traits<LazyPtrProxy<T> >
      {
      public:
       static const bool has_name = true;
       static std::string name(LazyPtrProxy<T> const &f)
       { return f.name(); }
      };

     template <class F, class A1>
       class debug_traits<Proxy1<F,A1> >
      {
      public:
       static const bool has_name = true;
       static std::string name(Proxy1<F,A1> const &f)
       { return f.name(); }
      };
     template <class F, class A1, class A2>
       class debug_traits<Proxy2<F,A1,A2> >
      {
      public:
       static const bool has_name = true;
       static std::string name(Proxy2<F,A1,A2> const &f)
       { return f.name(); }
      };
     template <class F, class A1, class A2, class A3>
       class debug_traits<Proxy3<F,A1,A2,A3> >
      {
      public:
       static const bool has_name = true;
       static std::string name(Proxy3<F,A1,A2,A3> const &f)
       { return f.name(); }
      };
#ifdef FCPP152
     template <class F, class A1, class A2, class A3, class A4>
       class debug_traits<Proxy4<F,A1,A2,A3,A4> >
      {
      public:
       static const bool has_name = true;
       static std::string name(Proxy4<F,A1,A2,A3,A4> const &f)
       { return f.name(); }
      };
#endif

     ///////////////////////////////////////////////
     //                    Fun1                   //
     ///////////////////////////////////////////////

     template <class A,class R>
       class debug_traits<Fun1Impl<A,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun1Impl<A,R> &f) { return f.name(); }
      };

     template <class Arg1, class Result>
       class debug_traits<Fun1<Arg1,Result> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun1<Arg1,Result> &f) { return f.name(); }
      };

     template <class Gen>
       class debug_traits<Gen1<Gen> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Gen1<Gen> &f) { return f.name(); }
      };

     ///////////////////////////////////////////////
     //                    Fun2                   //
     ///////////////////////////////////////////////

     template <class A,class B,class R>
       class debug_traits<Fun2Impl<A,B,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun2Impl<A,B,R> &f) { return f.name(); }
      };

     template <class Arg1, class Arg2,class Result>
     class debug_traits<Fun2<Arg1,Arg2,Result> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun2<Arg1,Arg2,Result> &f) { return f.name(); }
      };

     template <class Gen>
       class debug_traits<Gen2<Gen> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Gen2<Gen> &f) { return f.name(); }
      };

     ///////////////////////////////////////////////
     //                    Fun3                   //
     ///////////////////////////////////////////////

     template <class A,class B,class C,class R>
       class debug_traits<Fun3Impl<A,B,C,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun3Impl<A,B,C,R> &f) { return f.name(); }
      };

   template <class Arg1, class Arg2,class Arg3,class Result>
     class debug_traits<Fun3<Arg1,Arg2,Arg3,Result> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun3<Arg1,Arg2,Arg3,Result> &f) { return f.name(); }
      };

   template <class Arg1, class Arg2,class Arg3,class Result>
     class debug_traits<Fun3Guts<Arg1,Arg2,Arg3,Result> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun3Guts<Arg1,Arg2,Arg3,Result> &f) { return f.name(); }
      };

     template <class Gen>
       class debug_traits<Gen3<Gen> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Gen3<Gen> &f) { return f.name(); }
      };

#ifdef FCPP152

     ///////////////////////////////////////////////
     //                    Fun4                   //
     ///////////////////////////////////////////////

     template <class A,class B,class C,class D,class R>
       class debug_traits<Fun4Impl<A,B,C,D,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun4Impl<A,B,C,D,R> &f) { return f.name(); }
      };

   template <class Arg1, class Arg2,class Arg3,class Arg4,class Result>
     class debug_traits<Fun4<Arg1,Arg2,Arg3,Arg4,Result> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun4<Arg1,Arg2,Arg3,Arg4,Result> &f) { return f.name(); }
      };

     template <class A,class B,class C,class D,class R>
       class debug_traits<Fun4Guts<A,B,C,D,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun4Guts<A,B,C,D,R> &f) { return f.name(); }
      };

     template <class Gen>
       class debug_traits<Gen4<Gen> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Gen4<Gen> &f) { return f.name(); }
      };

     ///////////////////////////////////////////////
     //                    Fun5                   //
     ///////////////////////////////////////////////

     template <class A,class B,class C,class D,class E,class R>
       class debug_traits<Fun5Impl<A,B,C,D,E,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun5Impl<A,B,C,D,E,R> &f) { return f.name(); }
      };

   template <class Arg1, class Arg2,class Arg3,class Arg4,class Arg5,class Result>
     class debug_traits<Fun5<Arg1,Arg2,Arg3,Arg4,Arg5,Result> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun5<Arg1,Arg2,Arg3,Arg4,Arg5,Result> &f) { return f.name(); }
      };

     template <class Gen>
       class debug_traits<Gen5<Gen> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Gen5<Gen> &f) { return f.name(); }
      };

     ///////////////////////////////////////////////
     //                    Fun6                   //
     ///////////////////////////////////////////////

     template <class A,class B,class C,class D,class E,class F,class R>
       class debug_traits<Fun6Impl<A,B,C,D,E,F,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun6Impl<A,B,C,D,E,F,R> &f) { return f.name(); }
      };

   template <class Arg1, class Arg2,class Arg3,class Arg4,class Arg5,class Arg6,class Result>
     class debug_traits<Fun6<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun6<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Result> &f) { return f.name(); }
      };

     template <class Gen>
       class debug_traits<Gen6<Gen> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Gen6<Gen> &f) { return f.name(); }
      };

     ///////////////////////////////////////////////
     //                    Fun7                   //
     ///////////////////////////////////////////////

     template <class A,class B,class C,class D,class E,class F,class G,class R>
       class debug_traits<Fun7Impl<A,B,C,D,E,F,G,R> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun7Impl<A,B,C,D,E,F,G,R> &f) { return f.name(); }
      };

   template <class Arg1, class Arg2,class Arg3,class Arg4,class Arg5,class Arg6,class Arg7,class Result>
     class debug_traits<Fun7<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Fun7<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Result> &f) { return f.name(); }
      };

     template <class Gen>
       class debug_traits<Gen7<Gen> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const Gen7<Gen> &f) { return f.name(); }
      };

#endif // FCPP152

#endif // FCPP_DEBUG

  }  //  end namespace traits
} // end namespace fcpp

#endif
