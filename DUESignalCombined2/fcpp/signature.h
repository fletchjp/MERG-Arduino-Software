//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_SIGNATURE_DOT_H
#define FCPP_SIGNATURE_DOT_H

#include "config.h"
#include <functional>

#ifndef FCPP_NO_USE_NAMESPACE
namespace fcpp {
#endif

//////////////////////////////////////////////////////////////////////
// FCPP_CONCEPTS implemented as an option.
// The Sig definitions are being made conditional by putting them inside
// #ifndef FCPP_CONCEPTS
// #endif
// I have done this for all of CFunType except the one with zero arguments.
// Doing this messes something in list.h.
// I want to take all the RT out as well, but this causes problems.
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Inheritance detection

// Code based on Andrei Alexandrescu's article:
//    http://www.cuj.com/experts/1810/alexandr.html

template <class T, class U>
struct ConversionHelper {
   typedef char Small;
   struct Big { char dummy[2]; };
   static Small Test( const U* );
   static Big   Test(...);
   static const T* MakeT();
};

template <class T, class U>
struct Conversion {
   typedef ConversionHelper<T,U> H;
   static const int lhs = sizeof(H::Test((T*) H::MakeT()));
   static const int rhs = sizeof(typename H::Small);
   static const bool exists = (lhs==rhs);
   static const bool sameType = false;
};

template <class T>
struct Conversion<T, T> {
   static const bool exists = true;
   static const bool sameType = true;
};

template <class Derived, class Base>
struct Inherits {
   static const bool value = 
      Conversion<Derived,Base>::exists && 
      !Conversion<Base,void>::sameType;
};

//////////////////////////////////////////////////////////////////////
// Here are the classes with "nested typedefs" which just help us use
// our own type system; these classes are just inherited.
//
// Note that although the sigs support a large number of arguments, most 
// of the rest of the library only supports functions of 0-3 arguments.
//////////////////////////////////////////////////////////////////////

// Handy helper 'nothing' class.
struct Void {};

//#ifdef __VARIADIC_TEMPLATES
// Thinking outloud.
//#else
// This set names functoid arguments and results

// Increased the number of arguments to 9 for bindXof8 in curry.h
template <class A1, class A2 = Void, class A3 = Void, class A4 = Void,
  class A5 = Void, class A6 = Void, class A7 = Void, class A8 = Void,
  class A9 = Void, class R = Void>
struct FunType {
   typedef R ResultType;
   typedef A1 Arg1Type;
   typedef A2 Arg2Type;
   typedef A3 Arg3Type;
   typedef A4 Arg4Type;
   typedef A5 Arg5Type;
   typedef A6 Arg6Type;
   typedef A7 Arg7Type;
   typedef A8 Arg8Type;
   typedef A9 Arg9Type;
};

// Increased the number of arguments to 8 for bindXof7 in curry.h
template <class A1, class A2, class A3, class A4,
  class A5, class A6, class A7, class A8,class R>
  struct FunType<A1, A2, A3, A4, A5, A6, A7, A8, R> {
   typedef R ResultType;
   typedef A1 Arg1Type;
   typedef A2 Arg2Type;
   typedef A3 Arg3Type;
   typedef A4 Arg4Type;
   typedef A5 Arg5Type;
   typedef A6 Arg6Type;
   typedef A7 Arg7Type;
   typedef A8 Arg8Type;
};

template <class A1, class A2, class A3, class A4,
    class A5, class A6, class A7, class R>
  struct FunType<A1, A2, A3, A4, A5, A6, A7, R> {
   typedef R ResultType;
   typedef A1 Arg1Type;
   typedef A2 Arg2Type;
   typedef A3 Arg3Type;
   typedef A4 Arg4Type;
   typedef A5 Arg5Type;
   typedef A6 Arg6Type;
   typedef A7 Arg7Type;
};

//#endif

template <class A1, class A2, class A3, class A4,
                    class A5, class A6, class R>
  struct FunType<A1, A2, A3, A4, A5, A6, R, Void, Void> {
   typedef R ResultType;
   typedef A1 Arg1Type;
   typedef A2 Arg2Type;
   typedef A3 Arg3Type;
   typedef A4 Arg4Type;
   typedef A5 Arg5Type;
   typedef A6 Arg6Type;
};

template <class A1, class A2, class A3, class A4, class A5, class R>
  struct FunType<A1, A2, A3, A4, A5, R, Void, Void, Void> {
   typedef R ResultType;
   typedef A1 Arg1Type;
   typedef A2 Arg2Type;
   typedef A3 Arg3Type;
   typedef A4 Arg4Type;
   typedef A5 Arg5Type;
};

template <class A1, class A2, class A3, class A4, class R>
struct FunType<A1, A2, A3, A4, R, Void, Void, Void, Void> {
   typedef R ResultType;
   typedef A1 Arg1Type;
   typedef A2 Arg2Type;
   typedef A3 Arg3Type;
   typedef A4 Arg4Type;
};

template <class A1, class A2, class A3, class R>
struct FunType<A1, A2, A3, R, Void, Void, Void, Void, Void> {
   typedef R ResultType;
   typedef A1 Arg1Type;
   typedef A2 Arg2Type;
   typedef A3 Arg3Type;
};

template <class A1, class A2, class R>
struct FunType<A1, A2, R, Void, Void, Void, Void, Void, Void> {
  typedef R ResultType;
  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
};

template <class A1, class R>
struct FunType<A1, R, Void, Void, Void, Void, Void, Void, Void> {
  typedef R ResultType;
  typedef A1 Arg1Type;
};

template <class R>
struct FunType<R, Void, Void, Void, Void, Void, Void, Void, Void> {
  typedef R ResultType;
};

//////////////////////////////////////////////////////////////////////
// Concrete versions
//////////////////////////////////////////////////////////////////////
// This set is used for monomorphic direct functoids; the type names
// are inherited as-is, and also a template-Sig is defined so that
// monomorphic direct functoids can mix freely with polymorphic functoids 
// since the latter require a template-Sig member

// Brought over from function.hpp (Boost version)
// I have put it into CFunType as inherited (as in boost version)
// and also as a type (for monomorphic traits).
// I have to add the CFunType declaration to Fun2 etc as well as Fun2Impl
// for the types to be available.
struct i_am_monomorphic
{
};

template < class A1, class A2 = Void, class A3 = Void, class A4 = Void,
  class A5 = Void, class A6 = Void, class A7 = Void, class A8 = Void, class R = Void >
  struct CFunType : public FunType<A1,A2,A3,A4,A5,A6,A7,A8,R>,
                    public i_am_monomorphic
{
    typedef i_am_monomorphic monomorphic;
#ifndef FCPP_CONCEPTS
    template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
    struct Sig : public FunType<A1,A2,A3,A4,A5,A6,A7,A8,R> {};
#endif
   typedef A1 first_argument_type;  // imagined std:: octavary adaptable
   typedef A2 second_argument_type; // imagined std:: octavary adaptable
   typedef A3 third_argument_type;  // imagined std:: octavary adaptable
   typedef A4 fourth_argument_type; // imagined std:: octavary adaptable
   typedef A5 fifth_argument_type;  // imagined std:: octavary adaptable
   typedef A6 sixth_argument_type;  // imagined std:: octavary adaptable
   typedef A7 seventh_argument_type;  // imagined std:: octavary adaptable
   typedef A8 eighth_argument_type; // imagined std:: octavary adaptable
   typedef R result_type;  // Copied from signature.hpp fun_type.
};

template < class A1, class A2, class A3, class A4,
     class A5, class A6, class A7, class R>
  struct CFunType<A1, A2, A3, A4, A5, A6, A7, R>
  : public FunType<A1,A2,A3,A4,A5,A6,A7,R>,
                    public i_am_monomorphic
{
    typedef i_am_monomorphic monomorphic;
#ifndef FCPP_CONCEPTS
    template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
      struct Sig : public FunType<A1,A2,A3,A4,A5,A6,A7,R> {};
#endif
   typedef A1 first_argument_type;  // imagined std:: septimary adaptable
   typedef A2 second_argument_type; // imagined std:: septimary adaptable
   typedef A3 third_argument_type;  // imagined std:: septimary adaptable
   typedef A4 fourth_argument_type; // imagined std:: septimary adaptable
   typedef A5 fifth_argument_type;  // imagined std:: septimary adaptable
   typedef A6 sixth_argument_type;  // imagined std:: septimary adaptable
   typedef A7 seventh_argument_type;  // imagined std:: septimary adaptable
   typedef R result_type;  // Copied from signature.hpp fun_type.
};

template < class A1, class A2, class A3, class A4,
                     class A5, class A6, class R >
  struct CFunType<A1, A2, A3, A4, A5, A6, R, Void> :
   public FunType<A1,A2,A3,A4,A5,A6,R,Void> ,
   public i_am_monomorphic
{
   typedef i_am_monomorphic monomorphic;
#ifndef FCPP_CONCEPTS
   template <class P1, class P2, class P3, class P4, class P5, class P6>
   struct Sig : public FunType<A1,A2,A3,A4,A5,A6,R> {};
#endif
   typedef A1 first_argument_type;  // imagined std:: sextinary adaptable
   typedef A2 second_argument_type; // imagined std:: sextinary adaptable
   typedef A3 third_argument_type;  // imagined std:: sextinary adaptable
   typedef A4 fourth_argument_type; // imagined std:: sextinary adaptable
   typedef A5 fifth_argument_type;  // imagined std:: sextinary adaptable
   typedef A6 sixth_argument_type;  // imagined std:: sextinary adaptable
   typedef R result_type;  // Copied from signature.hpp fun_type.
};

template <class A1, class A2, class A3, class A4, class A5, class R>
  struct CFunType<A1, A2, A3, A4, A5, R, Void, Void> :
  public FunType<A1,A2,A3,A4,A5,R> ,
  public i_am_monomorphic
{
   typedef i_am_monomorphic monomorphic;
#ifndef FCPP_CONCEPTS
   template <class P1, class P2, class P3, class P4, class P5>
   struct Sig : public FunType<A1,A2,A3,A4,A5,R> {};
#endif
   typedef A1 first_argument_type;  // imagined std:: quintinary adaptable
   typedef A2 second_argument_type; // imagined std:: quintinary adaptable
   typedef A3 third_argument_type;  // imagined std:: quintinary adaptable
   typedef A4 fourth_argument_type; // imagined std:: quintinary adaptable
   typedef A5 fifth_argument_type;  // imagined std:: quintinary adaptable
   typedef R result_type;  // Copied from signature.hpp fun_type.
};

template <class A1, class A2, class A3, class A4, class R>
struct CFunType<A1, A2, A3, A4, R, Void, Void, Void> :
  public FunType<A1,A2,A3,A4,R>,
  public i_am_monomorphic
{
  typedef i_am_monomorphic monomorphic;
#ifndef FCPP_CONCEPTS
  template <class P1, class P2, class P3, class P4>
  struct Sig : public FunType<A1,A2,A3,A4,R> {};
#endif
  // Imported from signature.hpp
  typedef A1 first_argument_type;  // imagined std:: quaternary adaptable
  typedef A2 second_argument_type; // imagined std:: quaternary adaptable
  typedef A3 third_argument_type;  // imagined std:: quaternary adaptable
  typedef A4 fourth_argument_type; // imagined std:: quaternary adaptable
  typedef R result_type;  // Copied from signature.hpp fun_type.
};

template <class A1, class A2, class A3, class R>
struct CFunType<A1, A2, A3, R, Void, Void, Void, Void> :
  public FunType<A1,A2,A3,R>,
  public i_am_monomorphic
{
  typedef i_am_monomorphic monomorphic;
#ifndef FCPP_CONCEPTS
  template <class P1, class P2, class P3>
  struct Sig : public FunType<A1,A2,A3,R> {};
#endif
  // Imported from signature.hpp
  typedef A1 first_argument_type;  // imagined std:: ternary adaptable
  typedef A2 second_argument_type; // imagined std:: ternary adaptable
  typedef A3 third_argument_type;  // imagined std:: ternary adaptable
  typedef R result_type;  // Copied from signature.hpp fun_type.
};

template <class A1, class A2, class R>
struct CFunType<A1, A2, R, Void, Void, Void, Void, Void> :
  public FunType<A1,A2,R>, public std::binary_function<A1,A2,R>, 
  public i_am_monomorphic
{
  typedef i_am_monomorphic monomorphic;
#ifndef FCPP_CONCEPTS
  template <class P1, class P2>
  struct Sig : public FunType<A1,A2,R> {};
#endif
  // Imported from signature.hpp
  typedef A1 first_argument_type;   // std:: binary adaptable
  typedef A2 second_argument_type;  // std:: binary adaptable
  typedef R result_type;
};

template <class A1, class R>
struct CFunType<A1, R, Void, Void, Void, Void, Void, Void> :
  public FunType<A1,R>, public std::unary_function<A1,R>,
  public i_am_monomorphic
{
  typedef i_am_monomorphic monomorphic;
#ifndef FCPP_CONCEPTS
  template <class P1>
  struct Sig : public FunType<A1,R> {};
#endif
  // Imported from signature.hpp
  typedef A1 first_argument_type;   // for uniformity
  typedef A1 argument_type;  // std:: unary adaptable
  typedef R result_type;
};

struct CallableWithoutArguments {};
struct WrongNumberOfSigArgs {};

template <class R>
struct CFunType<R, Void, Void, Void, Void, Void, Void> :
  public CallableWithoutArguments, public FunType<R>,
  public i_am_monomorphic
{
  typedef i_am_monomorphic monomorphic;
  //#ifndef FCPP_CONCEPTS
  template <class Dummy1=Void, class Dummy2=Void>
  struct Sig : public FunType<WrongNumberOfSigArgs> {};
  //#endif
  template <class Dummy>
  struct Sig<Void,Dummy> : public FunType<R> {};
  typedef R result_type;
  typedef R ResultType;
};

// RT stuff moved to rt.h to be able to run it after smart.h

#ifndef FCPP_NO_USE_NAMESPACE
} // end namespace fcpp
#endif

#endif
