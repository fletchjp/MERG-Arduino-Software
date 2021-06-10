//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

// debug_traits.h

// renamed from

// fcpp_debug_traits.h

// Traits for debugging of FC++.

#include <sstream>

#ifndef FCPP_DEBUG_TRAITS_DOT_H
#define FCPP_DEBUG_TRAITS_DOT_H

namespace fcpp {

  namespace traits {

    // I need this to be separate as otherwise
    // I have to put something is all the other specializations
    // of debug_traits.
    template <class F>
    class local_debug_traits
    {
    public:
       static const bool has_local_name = false;
       // Note that this requires the actual object.
       // Not normally called.
       static std::string name(const F& f) { return std::string("not named"); }
   };

    template <class F>
    class debug_traits
    {
    public:
       static const bool has_name = false;
       // Note that this requires the actual object.
       // Not normally called.
       static std::string name(const F& f) { return std::string("not named"); }
       /*
       static const bool has_local_name = false;
	template <class Dummy>
	  struct local_debug_traits {
      public:
	   static const bool has_name = false;
	    static std::string name(Dummy d)
	    {
	      return std::string("Dummy");
	    }
	  };
       */
   };

  } // namespace traits

  // New routines to help with access
    template <class T>
    bool debug_traits_has_name()
    {
      return traits::debug_traits<T>::has_name;
    }

    template <class T>
    std::string debug_traits_name (const T& t)
    {
      return traits::debug_traits<T>::name(t);
    }

    template <class A,class B>
    bool debug_traits_has_local_name()
    {
      return traits::debug_traits<A>::has_local_name;
    }

    template <class A,class B>
    bool debug_traits_has_local_name_inner()
    {
      return traits::debug_traits<A>::template local_debug_traits<B>::has_name;
    }

    template <class A,class B>
      std::string debug_traits_local_name (const B& t)
    {
      return traits::debug_traits<A>::template local_debug_traits<B>::name(t);
    }

    template <class A,class B>
    std::string debug_local_name_or_nothing (const B& t)
    {
      if ( debug_traits_has_local_name<A,B>() )
	return debug_traits_local_name<A,B>(t);
      else return std::string("");
    }

    template <class T>
    std::string debug_name_or_nothing (const T& t)
    {
      if ( debug_traits_has_name<T>() )
         return debug_traits_name(t);
      else return std::string("");
    }

    template <class T>
    std::string debug_name_or_value (const T& t)
    {
      if ( debug_traits_has_name<T>() )
         return debug_traits_name(t);
      else return argument_traits_value(t);
    }

    template <class T>
      std::string debug_name_or_string(const T& t, const std::string &s)
    {
      if ( debug_traits_has_name<T>() )
         return debug_traits_name(t);
      else return s;
    }

    template <class T>
    std::string debug_traits_name_and_string (const T& t, const std::string &s)
    {
      return traits::debug_traits<T>::name(t) + s;
    }

    template <class T>
      std::string debug_string_and_name_or_string
      (const std::string &s1, const T& t, const std::string &s2)
    {
      if ( debug_traits_has_name<T>() )
         return s1 + debug_traits_name(t);
      else return s2;
    }

    template <class T>
      std::string debug_name_and_string_or_string
      (const T& t, const std::string &s1, const std::string &s2)
    {
      if ( debug_traits_has_name<T>() )
         return debug_traits_name(t) + s1;
      else return s2;
    }

    template <class T>
      std::string debug_string_name_and_string_or_string
      (const std::string &s1, const T& t, const std::string &s2, const std::string &s3)
    {
      if ( debug_traits_has_name<T>() )
         return s1 + debug_traits_name(t) + s2;
      else return s3;
    }

    // This is used in variadic_fcpp.h
    template <class F,class Arg>
      std::string debug_fname_f_and_arg
      (const std::string &fname, const F& f, const Arg& arg)
    {
     if ( debug_traits_has_name<F>() ) {
       if ( debug_traits_has_name<Arg>() ) {
	  return fname + std::string("::")+debug_traits_name(f)
       		  +debug_traits_name(arg);
       } else
	  return fname + std::string("::")+debug_traits_name(f);
   } else
        return fname;

    }

} // namespace fcpp

#endif
