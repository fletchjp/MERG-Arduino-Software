/// @file variantCode.h
/// @brief Arduino RP2040 code for Boost Spirit X3 Variant
///
/// I get tidier code after moving all the rules definitions into the namespace client::ast.
///
/// This provides an example including operator<< for the whole variant.
///
/// It provides adaption for Boost 1.66.0 code to work with the Arduino RP2040.

#ifndef VARIANT_CODE_H
#define VARIANT_CODE_H


#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include "ArduinoCode.h"

namespace x3 = boost::spirit::x3;

namespace client { namespace ast
{

/// single line comment
struct SingleLineComment{};
/// whitespace
struct Whitespace       {};
/// for define command
struct Define           {
    Define(std::string const &name = "") : name(name) {}
    std::string name;
};
/// for when command
struct When             {}; /// for when command

/// If an object does not have an output operator, one is needed. 
template <typename out>
inline out &operator <<(out &stream, const Whitespace)
{
    stream << std::string("whitespace") << std::ends;
    return stream;
}

inline Print &operator <<(Print &stream, Whitespace)
{
   return stream;
}

template <typename out>
inline out &operator <<(out &stream, const SingleLineComment)
{
    stream << std::string("singlelinecomment") << std::ends;
    return stream;
}

inline Print &operator <<(Print &stream, SingleLineComment)
{
   return stream;
}

template <typename out>
inline out &operator <<(out &stream, const Define)
{
    stream << std::string("define") << std::ends;
    return stream;
}


inline Print &operator <<(Print &stream, Define)
{
   return stream;
}

template <typename out>
inline out &operator <<(out &stream, const When)
{
    stream << std::string("when") << std::ends;
    return stream;
}

inline Print &operator <<(Print &stream, When)
{
   return stream;
}

/// person struct has first_name and last_name 
struct person : x3::position_tagged
{
       person(
            std::string const& first_name = ""
          , std::string const& last_name = ""
       )
          : first_name(first_name)
          , last_name(last_name)
       {}

       std::string first_name, last_name;
};


template <typename out>
inline out &operator <<(out &stream, const person &arg)
{
   //std::stringstream s;
   stream << arg.first_name << " " << arg.last_name << std::ends;
   //stream.print(s.str().c_str());
   return stream;
}

/*
inline Print &operator <<(Print &stream, const person &arg)
{
   std::stringstream s;
   s << arg.first_name << " " << arg.last_name << std::ends;
   stream.print(s.str().c_str());
   return stream;
}
*/

}}

using client::ast::SingleLineComment;
using client::ast::Whitespace;
using client::ast::Define;
using client::ast::When;
using client::ast::person;

/// I am now using X3 variant.
using Variant = x3::variant<SingleLineComment, Whitespace, Define, When , person>;

/// Stream output for a variant type provided operators exist for all the alternatives.
inline Print &operator <<(Print &stream, const Variant &arg)
{
   std::stringstream s;
   s << arg.get() << std::ends;
   stream.print(s.str().c_str());
   return stream;
}

/// Token structure to hold all the variant options.
struct Token : Variant, x3::position_tagged {
    using Variant::Variant;
    using Variant::operator=; // This is what was needed to get x3 variant working.
};

inline Print &operator <<(Print &stream, const Token &arg)
{
   std::stringstream s;
   s << arg.get() << std::ends;
   stream.print(s.str().c_str());
   return stream;
}


#endif
