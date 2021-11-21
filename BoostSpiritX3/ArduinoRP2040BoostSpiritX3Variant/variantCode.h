/// @file variantCode.h
/// @brief Arduino RP2040 code for Boost Spirit X3 Variant
///
/// This provides an example including operator<< for the whole variant.
///
/// It provides adaption for Boost 1.66.0 code to work with the Arduino RP2040.

#include <variant>
#include "ArduinoCode.h"

#ifndef VARIANT_CODE_H
#define VARIANT_CODE_H

namespace x3 = boost::spirit::x3;

struct none {};

/// If an object does not have an output operator, one is needed. 
template <typename out>
inline out &operator <<(out &stream, const none)
{
    stream << std::string("none") << std::ends;
    return stream;
}

inline Print &operator <<(Print &stream, const none)
{
  stream.print("none");
  return stream;
}

using variant = x3::variant<
        none
      , bool
      , std::string
      , int
      , double
    >;

struct ast;

struct ast : variant
{
    using variant::variant;
    using variant::operator=;

    ast(char const* s)
      : variant(std::string{s})
    {}

    ast& operator=(char const* s)
    {
        variant::operator=(std::string{s});
        return *this;
    }

};


/// Stream output for a variant type provided operators exist for all the alternatives.
inline Print &operator <<(Print &stream, const ast &arg)
{
   std::stringstream s;
   s << arg.get() << std::ends;
   stream.print(s.str().c_str());
   return stream;
}


// another code from 
// https://stackoverflow.com/questions/61392947/transitioning-boost-spirit-parser-from-boostvariant-to-stdvariant/61409717#61409717
struct Recurse;

struct Base : std::variant<std::string, boost::recursive_wrapper<Recurse> > {
    using BaseV = std::variant<std::string, boost::recursive_wrapper<Recurse> >;
    using BaseV::BaseV;
    using BaseV::operator=;

    struct adapted_variant_tag {};
    using types = boost::mpl::list<std::string, Recurse>;
};

struct Recurse {
    int _i;
    Base _base;
};

// traits code from the same source.
namespace boost::spirit::x3::traits {
    template<typename... t>
    struct is_variant<std::variant<t...> >
        : mpl::true_ {};

    template <typename attribute, typename... t>
    struct variant_has_substitute_impl<std::variant<t...>, attribute>
    {
        typedef std::variant<t...> variant_type;
        typedef typename mpl::transform<
              mpl::list<t...>
            , unwrap_recursive<mpl::_1>
            >::type types;
        typedef typename mpl::end<types>::type end;

        typedef typename mpl::find<types, attribute>::type iter_1;

        typedef typename
            mpl::eval_if<
                is_same<iter_1, end>,
                mpl::find_if<types, traits::is_substitute<mpl::_1, attribute>>,
                mpl::identity<iter_1>
            >::type
        iter;
 
        typedef mpl::not_<is_same<iter, end>> type;
    };

template <typename attribute, typename... t>
    struct variant_find_substitute<std::variant<t...>, attribute>
    {
        typedef std::variant<t...> variant_type;
        typedef typename mpl::transform<
              mpl::list<t...>
            , unwrap_recursive<mpl::_1>
            >::type types;

        typedef typename mpl::end<types>::type end;

        typedef typename mpl::find<types, attribute>::type iter_1;

        typedef typename
            mpl::eval_if<
                is_same<iter_1, end>,
                mpl::find_if<types, traits::is_substitute<mpl::_1, attribute> >,
                mpl::identity<iter_1>
            >::type
        iter;

        typedef typename
            mpl::eval_if<
                is_same<iter, end>,
                mpl::identity<attribute>,
                mpl::deref<iter>
            >::type
        type;
    };

    template <typename... t>
    struct variant_find_substitute<std::variant<t...>, std::variant<t...> >
        : mpl::identity<std::variant<t...> > {};
}



#endif
