// argument_traits.h

#include <sstream>
#ifdef FCPP_CXX11
#include <tuple>
#endif

#ifndef FCPP_ARGUMENT_TRAITS_DOT_H
#define FCPP_ARGUMENT_TRAITS_DOT_H

namespace fcpp {

  // Advance declaration of new routine to help with access
    template <class T>
      std::string argument_traits_value (const T& t);

  namespace traits {

    // The specialised trait returning a null string is defined for
    // Full0, Full1, Full2 and Full3 objects in full.h
    // Now added also for Full4 and Full5.
    // The following were added in error to get testeither.cpp to compile.
    // The fact is that this functionality is provided in seq.h.
    // This needs to be sorted out to be in fcpp to be more generally
    // useful.
    // Also added for Fun0 in function.h - taken out of use.
    // std::pair specialization added here. - taken out of use.
    template <class T>
      class argument_traits {
    public:
      static std::string value(const T& t)
      {  
         std::ostringstream o;
         o << t;
         return o.str();
      }
    };

    template <>
    class argument_traits <bool> {
    public:
      static std::string value(const bool &b)
      {
        if (b) return std::string("true");
        else  return std::string("false");
      }
    };

    template<class S,class T>
    class argument_traits<std::pair<S,T> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const std::pair<S,T> &p)
         {
           std::string temp;
           temp += std::string("{ ");
           temp += argument_traits_value(p.first);
           temp += std::string(" , ");
           temp += argument_traits_value(p.second);
           temp += std::string(" }");
           return temp;
         }
       };

#ifdef FCPP_CXX11
    template<class S,class T>
    class argument_traits<std::tuple<S,T> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const std::tuple<S,T> &p)
         {
           std::string temp;
           temp += std::string("{ ");
           temp += argument_traits_value(std::get<0>(p) );
           temp += std::string(" , ");
           temp += argument_traits_value(std::get<1>(p) );
           temp += std::string(" }");
           return temp;
         }
       };


    template<class S,class T,class U>
    class argument_traits<std::tuple<S,T,U> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const std::tuple<S,T,U> &p)
         {
           std::string temp;
           temp += std::string("{ ");
           temp += argument_traits_value(std::get<0>(p) );
           temp += std::string(" , ");
           temp += argument_traits_value(std::get<1>(p) );
           temp += std::string(" , ");
           temp += argument_traits_value(std::get<2>(p) );
           temp += std::string(" }");
           return temp;
         }
       };

    template<class S,class T,class U, class V>
      class argument_traits<std::tuple<S,T,U,V> >
       {
       public:
         //static const bool has_name = true;
         static std::string value(const std::tuple<S,T,U,V> &p)
         {
           std::string temp;
           temp += std::string("{ ");
           temp += argument_traits_value(std::get<0>(p) );
           temp += std::string(" , ");
           temp += argument_traits_value(std::get<1>(p) );
           temp += std::string(" , ");
           temp += argument_traits_value(std::get<2>(p) );
           temp += std::string(" , ");
           temp += argument_traits_value(std::get<3>(p) );
           temp += std::string(" }");
           return temp;
         }
       };
#endif

  } // namespace traits

  // New routine to help with access
    template <class T>
    std::string argument_traits_value (const T& t)
    {
      return traits::argument_traits<T>::value(t);
    }

} // namespace fcpp

#endif

