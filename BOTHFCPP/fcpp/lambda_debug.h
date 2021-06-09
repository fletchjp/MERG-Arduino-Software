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

// lambda_debug.h is to take all the argument_traits (if any) and 
// debug_traits definitions from lambda.h

#ifndef FCPP_LAMBDA_DEBUG_DOT_H
#define FCPP_LAMBDA_DEBUG_DOT_H

#ifdef FCPP_DEBUG
namespace fcpp {
  namespace traits {

      template <>
	class debug_traits<fcpp_lambda::exp::NIL >
      {
      public:
       static const bool has_name = true;
       static std::string name(const fcpp_lambda::exp::NIL &f)
        { return f.name(); }
      };

#ifdef FCPP_LAMBDA_AUTOCURRY
      template <>
	class debug_traits<fcpp_lambda::exp::AutoCurry >
      {
      public:
       static const bool has_name = true;
       static std::string name(const fcpp_lambda::exp::AutoCurry &f)
        { return f.name(); }
      };
#endif

      template <class T>
	class debug_traits<fcpp_lambda::exp::LambdaExp<T> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const fcpp_lambda::exp::LambdaExp<T> &f)
        { return f.name(); }
      };

      template <class T>
	class debug_traits<fcpp_lambda::exp::LambdaType<T> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const fcpp_lambda::exp::LambdaType<T> &f)
        { return f.name(); }
      };

      template <class T>
	class debug_traits<fcpp_lambda::exp::Value<T> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const fcpp_lambda::exp::Value<T> &f)
        { return f.name(); }
      };

      template <int i>
	class debug_traits<fcpp_lambda::exp::LambdaVar<i> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const fcpp_lambda::exp::LambdaVar<i> &f)
        { return f.name(); }
      };

      template <class Fun, class Args>
	class debug_traits<fcpp_lambda::exp::Call<Fun,Args> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const fcpp_lambda::exp::Call<Fun,Args> &f)
        { return f.name(); }
      };

      template <class TBBV>
      class debug_traits<fcpp_lambda::LambdaThingy<TBBV> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const fcpp_lambda::LambdaThingy<TBBV> &f)
        { return f.name(); }
      };

      template <>
      class debug_traits<fcpp_lambda::LambdaThingy<fcpp_lambda::NIL> >
      {
      public:
       static const bool has_name = true;
       static std::string name(const fcpp_lambda::LambdaThingy<fcpp_lambda::NIL> &f)
        { return f.name(); }
      };

      /*      template <class TBBV>
      class debug_traits<typename fcpp_lambda::LambdaThingy<TBBV>::Full >
      {
      public:
       static const bool has_name = true;
       static std::string name(const typename fcpp_lambda::LambdaThingy<TBBV>::Full &f)
        { return f.name(); }
      };
      */
 
      template <class LE>
      class debug_traits<fcpp_lambda::Lambda0<LE> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::Lambda0<LE> &f)
         { return f.name(); }
       };
     
       template <class LE,int i>
      class debug_traits<fcpp_lambda::Lambda1<LE,i> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::Lambda1<LE,i> &f)
         { return f.name(); }
       };

      template <class LE,int i,int j>
      class debug_traits<fcpp_lambda::Lambda2<LE,i,j> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::Lambda2<LE,i,j> &f) 
         { return f.name(); }
       };

      template <class LE,int i,int j, int k>
	class debug_traits<fcpp_lambda::Lambda3<LE,i,j,k> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::Lambda3<LE,i,j,k> &f)
         { return f.name(); }
       };

#ifdef FCPP152
      template <class LE,int i,int j, int k, int l>
	class debug_traits<fcpp_lambda::Lambda4<LE,i,j,k,l> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::Lambda4<LE,i,j,k,l> &f)
         { return f.name(); }
       };

      template <class LE,int i,int j, int k, int l, int m>
	class debug_traits<fcpp_lambda::Lambda5<LE,i,j,k,l,m> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::Lambda5<LE,i,j,k,l,m> &f)
         { return f.name(); }
       };

      template <class LE,int i,int j, int k, int l, int m, int n>
	class debug_traits<fcpp_lambda::Lambda6<LE,i,j,k,l,m,n> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::Lambda6<LE,i,j,k,l,m,n> &f)
         { return f.name(); }
       };

#ifdef FCPP_LAMBDA7
      template <class LE,int i,int j, int k, int l, int m, int n, int o>
	class debug_traits<fcpp_lambda::Lambda7<LE,i,j,k,l,m,n,o> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::Lambda7<LE,i,j,k,l,m,n,o> &f)
         { return f.name(); }
       };

      template <class LE,int i,int j, int k, int l, int m, int n, int o, int p>
	class debug_traits<fcpp_lambda::Lambda8<LE,i,j,k,l,m,n,o,p> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::Lambda8<LE,i,j,k,l,m,n,o,p> &f)
         { return f.name(); }
       };
#endif
#endif

      template <class RealET,class LE,class FV,class BE1,class B >
       class debug_traits<fcpp_lambda::exp::AddEnvLE<RealET,LE,FV,BE1,B> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::exp::AddEnvLE<RealET,LE,FV,BE1,B> &f)
         { return f.name(); }
       };

      template <class B,class LE>
	class debug_traits<fcpp_lambda::exp::BindingEnvExp<B,LE> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::exp::BindingEnvExp<B,LE> &f)
         { return f.name(); }
       };

      template <class LE, class VARS>
	class local_debug_traits<fcpp_lambda::exp::LambdaExpWithFreeVars<LE,VARS> >
       {
       public:
	static const bool has_local_name = true;
       };

      template <class LE, class VARS>
	class debug_traits<fcpp_lambda::exp::LambdaExpWithFreeVars<LE,VARS> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::exp::LambdaExpWithFreeVars<LE,VARS> &f)
         { return f.name(); }

      /* This won't work as normal as it is an internal class
         inside LambdaExpWithFreeVars.
         I have managed to make a local internal trait.
         Can I find a way to use it?
         The ideas are adapted from here:
http://stackoverflow.com/questions/19663640/syntax-for-specialization-of-nested-template-class
       */
	/*
	template <class Dummy>
	  struct local_debug_traits {
      public:
	   static const bool has_name = false;
	    static std::string name(Dummy d)
	    {
	      return std::string("Dummy");
	    }
	  };

	template <class LEa,class ET,int i>
	  class local_debug_traits<
          typename fcpp_lambda::exp::LambdaExpWithFreeVars<LE,VARS>::template
                                     Lambda1WithFreeVars<LEa,ET,i> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const
        typename fcpp_lambda::exp::LambdaExpWithFreeVars<LE,VARS>::template
                                   Lambda1WithFreeVars<LEa,ET,i> &f)
         { return f.name(); }
       };
	*/
       };

       template <class E,class T,class F,fcpp_lambda::IfKind ifkind>
       class debug_traits<fcpp_lambda::exp::IfLE<E,T,F,ifkind> >
       {
       public:
	static const bool has_name = true;
	static std::string name(const fcpp_lambda::exp::IfLE<E,T,F,ifkind> &f)
         { return f.name(); }
       };

  }  //  end namespace traits
} // end namespace fcpp

#endif

#endif
