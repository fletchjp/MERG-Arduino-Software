/// @file variantCode.h
/// @brief Variant client ast code for Boost Spirit X3 Commands2
///
/// I get tidier code after moving all the rules definitions into the namespace client::ast.
/// I have now also moved Variant and Token in as well.
///
/// The next task is to parse the define command and get the data into a map keyed on the name.
/// I have already done something simular in the annotation series.
/// One way to approach this would be to duplicate the example there.
/// The question is how to extract data from some tokens but not others.
///
/// This provides an example including operator<< for the whole variant.

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

struct Event : x3::position_tagged           {
    Event(std::string const &name = "", int n = 0, int e = 0) : name(name), nn(n), en(e) {}
    std::string name;
    int nn; // Node number
    int en; // Event number
};
/// for define command
struct Define           {
    Define(std::string const &name = "", int n = 0, int e = 0) : name(name), nn(n), en(e) {}
    std::string name;
    int nn; // Node number
    int en; // Event number
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

std::map<std::string,std::pair<int,int>> events;

template <typename out>
inline out &operator <<(out &stream, const Event& arg)
{
    stream << std::string("event ") << arg.name << " " << arg.nn << " " << arg.en << std::ends;
    return stream;
}


inline Print &operator <<(Print &stream, Event)
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

/// Person struct has first_name and last_name 
struct Person : x3::position_tagged
{
       Person(
            std::string const& first_name = ""
          , std::string const& last_name = ""
       )
          : first_name(first_name)
          , last_name(last_name)
       {}

       std::string first_name, last_name;
};

/// to receive parsed results.
std::vector<Person> people; 

/// Store the result
/// I don't yet know how to overload the store function for different arg types
/// within Variant. I can call it from the overloaded operator<<
inline void store(const Person &arg)
{
    people.push_back(Person(arg.first_name,arg.last_name));
}

/// Output operator for a Person object
template <typename out>
inline out &operator <<(out &stream, const Person &arg)
{
   //std::stringstream s;
   store(arg);
   //people.push_back(Person(arg.first_name,arg.last_name));
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

/// I am now using X3 variant.
using Variant = x3::variant<SingleLineComment, Whitespace, Event, When , Person>;

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

/// Print operator for a Token
inline Print &operator <<(Print &stream, const Token &arg)
{
   std::stringstream s;
   s << arg.get() << std::ends;
   stream.print(s.str().c_str());
   return stream;
}


}}

using client::ast::SingleLineComment;
using client::ast::Whitespace;
using client::ast::Define;
using client::ast::Event;
using client::ast::When;
using client::ast::Person;
using client::ast::Variant;
using client::ast::Token;



#endif
