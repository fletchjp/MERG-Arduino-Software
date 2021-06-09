//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_RT_DOT_H
#define FCPP_RT_DOT_H

#include "config.h"
#include <functional>

#ifndef FCPP_NO_USE_NAMESPACE
namespace fcpp {
#endif

//////////////////////////////////////////////////////////////////////
// Icky helpers
//////////////////////////////////////////////////////////////////////
// These are strictly unnecessary, but they avoid a bug in the g++
// compiler and also make some things shorter to type.  
// RT<T, args> means "return type of T when passed argument types <args>"

////////////////////////////////////////////////////////////////////////
// This section should be removable when concepts have been implemented.
// I want to do this but not yet.
//#ifndef FCPP_CONCEPTS 

template <class T, class A1 = Void, class A2 = Void, class A3 = Void,
          class A4 = Void, class A5 = Void, class A6 = Void,
  class A7 = Void,class A8 = Void>
struct RT {
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7, A8>::ResultType ResultType;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7, A8>::Arg1Type Arg1Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7, A8>::Arg2Type Arg2Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7, A8>::Arg3Type Arg3Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7, A8>::Arg4Type Arg4Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7, A8>::Arg5Type Arg5Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7, A8>::Arg6Type Arg6Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7, A8>::Arg7Type Arg7Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7, A8>::Arg8Type Arg8Type;
};

template <class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
struct RT<T, A1, A2, A3, A4, A5, A6, A7, Void> {
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7>::ResultType ResultType;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7>::Arg1Type Arg1Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7>::Arg2Type Arg2Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7>::Arg3Type Arg3Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7>::Arg4Type Arg4Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7>::Arg5Type Arg5Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7>::Arg6Type Arg6Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6, A7>::Arg7Type Arg7Type;
};

template <class T, class A1, class A2, class A3, class A4, class A5, class A6>
struct RT<T, A1, A2, A3, A4, A5, A6, Void, Void> {
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6>::ResultType ResultType;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6>::Arg1Type Arg1Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6>::Arg2Type Arg2Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6>::Arg3Type Arg3Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6>::Arg4Type Arg4Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6>::Arg5Type Arg5Type;
typedef typename T::template Sig<A1, A2, A3, A4, A5, A6>::Arg6Type Arg6Type;
};

template <class T, class A1, class A2, class A3, class A4, class A5>
struct RT<T, A1, A2, A3, A4, A5, Void, Void, Void> {
   typedef typename T::template Sig<A1, A2, A3, A4, A5>::ResultType ResultType;
   typedef typename T::template Sig<A1, A2, A3, A4, A5>::Arg1Type Arg1Type;
   typedef typename T::template Sig<A1, A2, A3, A4, A5>::Arg2Type Arg2Type;
   typedef typename T::template Sig<A1, A2, A3, A4, A5>::Arg3Type Arg3Type;
   typedef typename T::template Sig<A1, A2, A3, A4, A5>::Arg4Type Arg4Type;
   typedef typename T::template Sig<A1, A2, A3, A4, A5>::Arg5Type Arg5Type;
};

template <class T, class A1, class A2, class A3, class A4>
struct RT<T, A1, A2, A3, A4, Void, Void, Void, Void> {
   typedef typename T::template Sig<A1, A2, A3, A4>::ResultType ResultType;
   typedef typename T::template Sig<A1, A2, A3, A4>::Arg1Type Arg1Type;
   typedef typename T::template Sig<A1, A2, A3, A4>::Arg2Type Arg2Type;
   typedef typename T::template Sig<A1, A2, A3, A4>::Arg3Type Arg3Type;
   typedef typename T::template Sig<A1, A2, A3, A4>::Arg4Type Arg4Type;
};

template <class T, class A1, class A2, class A3>
struct RT<T, A1, A2, A3, Void, Void, Void, Void, Void> {
   typedef typename T::template Sig<A1, A2, A3>::ResultType ResultType;
   typedef typename T::template Sig<A1, A2, A3>::Arg1Type Arg1Type;
   typedef typename T::template Sig<A1, A2, A3>::Arg2Type Arg2Type;
   typedef typename T::template Sig<A1, A2, A3>::Arg3Type Arg3Type;
};

template <class T, class A1, class A2>
struct RT<T, A1, A2, Void, Void, Void, Void, Void, Void> {
  //typedef typename SmartHasSig<FunctoidTraits<T>::has_sig>::Result Has_Sig;
   typedef typename T::template Sig<A1, A2>::ResultType ResultType;
   typedef typename T::template Sig<A1, A2>::Arg1Type Arg1Type;
   typedef typename T::template Sig<A1, A2>::Arg2Type Arg2Type;
};

template <class T, class A1>
struct RT<T, A1, Void, Void, Void, Void, Void, Void, Void> {
  //typedef typename SmartHasSig<FunctoidTraits<T>::has_sig>::Result Has_Sig;
   typedef typename T::template Sig<A1>::ResultType ResultType;
   typedef typename T::template Sig<A1>::Arg1Type Arg1Type;
};

template <class T>
struct RT<T, Void, Void, Void, Void, Void, Void, Void, Void> {
  // Not valid here until moved to a separate file run after signature.h
  //typedef typename SmartHasSig<FunctoidTraits<T>::has_sig>::Result Has_Sig;
   typedef typename T::template Sig<>::ResultType ResultType;
};


#ifdef FCPP_CUDA_COMPATIBLE
// This is used in the definition of Fun0 in place of RT.
template <class T>
struct RT0 {
  // Not valid here until moved to a separate file run after signature.h
  //typedef typename SmartHasSig<FunctoidTraits<T>::has_sig>::Result Has_Sig;
  typedef typename T::ResultType ResultType;
 };
#endif

//#endif // #ifndef FCPP_CONCEPTS


#ifndef FCPP_NO_USE_NAMESPACE
} // end namespace fcpp
#endif

#endif
