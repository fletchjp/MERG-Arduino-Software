//
// Copyright (c) 2000-2003 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_DETECT_PAIR_DOT_H
#define FCPP_DETECT_PAIR_DOT_H

//#include <boost/type_traits/is_integral.hpp>

// detect_pair.h
// New file to look at the problem of std::pair with C++11.

// This is taken from  
// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector

#define CREATE_MEMBER_DETECTOR(X)                                                   \
template<typename T> class Detect_##X {                                             \
    struct Fallback { int X; };                                                     \
    struct Derived : T, Fallback { };                                               \
                                                                                    \
    template<typename U, U> struct Check;                                           \
                                                                                    \
    typedef char ArrayOfOne[1];                                                     \
    typedef char ArrayOfTwo[2];                                                     \
                                                                                    \
    template<typename U> static ArrayOfOne & func(Check<int Fallback::*, &U::X> *); \
    template<typename U> static ArrayOfTwo & func(...);                             \
  public:                                                                           \
    typedef Detect_##X type;                                                        \
    enum { value = sizeof(func<Derived>(0)) == 2 };                                 \
};
 
CREATE_MEMBER_DETECTOR(first);
CREATE_MEMBER_DETECTOR(second);
#undef CREATE_MEMBER_DETECTOR

namespace fcpp {


template <typename T>
bool is_integral() {
  //return boost::is_integral<T>::value;
  return std::__is_integer<T>::value;  
}

template <typename T>
bool is_pair() {  
  return (Detect_first<T>::value && Detect_second<T>::value);
}

//namespace fcpp {
  namespace traits {

    template <class F,bool T>
    class pair_value_traits
    {
      public:
       static std::string value(const F& f) { return std::string(""); }
    };


    template <class T>
    class pair_traits
    {
    public:
      // This works as long as T is a class and not a POD e.g. int.
      static const bool is_pair =
        (Detect_first<T>::value && Detect_second<T>::value);
      // Inner trait is needed to sort out true and false cases.
      // true case is defined in seq.h
      static std::string value(const T& f) {
          return pair_value_traits<T,is_pair>::value(f);
      }
    };

    // So I have to specialize it for other types.
    template <>
    class pair_traits<int>
    {
    public:
       static const bool is_pair = false;
       template <class F>
       static std::string value(const F& f) { return std::string(""); }
    };

    template <>
    class pair_traits<unsigned int>
    {
    public:
       static const bool is_pair = false;
       template <class F>
       static std::string value(const F& f) { return std::string(""); }
    };

    template <>
    class pair_traits<long>
    {
    public:
       static const bool is_pair = false;
       template <class F>
       static std::string value(const F& f) { return std::string(""); }
    };

    template <>
    class pair_traits<unsigned long>
    {
    public:
       static const bool is_pair = false;
       template <class F>
       static std::string value(const F& f) { return std::string(""); }
    };

    template <>
    class pair_traits<bool>
    {
    public:
       static const bool is_pair = false;
       template <class F>
       static std::string value(const F& f) { return std::string(""); }
    };

  } // namespace traits

  // New routines to help with access
  template <class T>
  bool pair_traits_is_pair()
  {
    return traits::pair_traits<T>::is_pair;
  }

  template <class T>
  std::string pair_traits_value (const T& t)
  {
      return traits::pair_traits<T>::value(t);
  }

} // namespace fcpp


#endif
