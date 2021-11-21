/// @file variantCode.h
/// @brief Arduino RP2040 code for Boost Spirit X3 Variant
///
/// This provides an example including operator<< for the whole variant.
///
/// It provides adaption for Boost 1.66.0 code to work with the Arduino RP2040.

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



#endif
