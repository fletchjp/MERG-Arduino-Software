//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

// debug_helper.h

// This was written before the work on refactoring using debug functions
// in debug_traits.h which can be used here also.

// I have begun using the debug_traits functions which remove clutter.
// There may be a second level of operation to extract some common patterns.

#ifndef FCPP_DEBUG_TRAITS_DOT_H
#define FCPP_DEBUG_TRAITS_DOT_H

namespace fcpp {

  // The idea is to provide some helper functions which will
  // simplify the task of providing debug information within
  // the binders. At the moment code has to be provided in each
  // one which is often the same or very similar.

  // It may prove easier to use the helper functions instead.

  /*
   std::string name() const {
     std::string temp;
     if (fcpp::traits::debug_traits<Unary>::has_name) {
	 temp += std::string("binder1of1::")
	      + fcpp::traits::debug_traits<Unary>::name(f);
       if (fcpp::traits::debug_traits<Arg>::has_name) {
	 temp += std::string("::")
       	      +  fcpp::traits::debug_traits<Arg>::name(a);
       } else {
  	 temp += std::string("( ");
	 temp += fcpp::traits::argument_traits<Arg>::value(a);
         temp += std::string(" )");
       }
       return temp;
     } else
         return std::string("binder1of1");
     }
  */
  // Version of the same thing with debug_traits functoins:
  /*
   std::string name() const {
     std::string temp;
     if (debug_traits_has_name<Unary>()) {
	 temp += std::string("binder1of1::")
	      + debug_traits_name(f);
	 if (debug_traits_has_name<Arg>()) {
	   temp += std::string("::")
       	        +  debug_traits_name(a);
         } else {
  	   temp += std::string("( ");
	   temp += argument_traits_value(a);
           temp += std::string(" )");
         }
         return temp;
     } else
         return std::string("binder1of1");
     }
  */

  template <class Arg, int N> struct name_helper2;

  template <class Arg> struct name_helper2<Arg,1>
    {
      static inline std::string go(const Arg &x)
      {
         std::string temp;
  	 temp += std::string("( ");
	 temp += fcpp::traits::argument_traits<Arg>::value(x);
         temp += std::string(" , _ )");
         return temp;
      }
    };
  
  template <class Arg> struct name_helper2<Arg,2>
    {
      static inline std::string go(const Arg &x)
      {
         std::string temp;
  	 temp += std::string("( _ , ");
	 temp += fcpp::traits::argument_traits<Arg>::value(x);
         temp += std::string(" )");
         return temp;
      }
    };
 
  template <class Function, class Arg, int N>
  std::string name_helper(const std::string &namestring, const Arg& x) const {
     std::string temp;
     if (fcpp::traits::debug_traits<Function>::has_name) {
	 temp += namestring + std::string("::")
	      + fcpp::traits::debug_traits<Function>::name(f);
       if (fcpp::traits::debug_traits<Arg>::has_name) {
	 temp += std::string("::")
       	      +  fcpp::traits::debug_traits<Arg>::name(x);
       } else {
         temp += name_helper2<Arg,N>::go(x);
       }
       return temp;
     } else
         return namestring;
     }


  namespace traits {


  } // namespace traits
} // namespace fcpp

#endif
