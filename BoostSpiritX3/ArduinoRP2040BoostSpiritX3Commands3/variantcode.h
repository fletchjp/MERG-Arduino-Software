/// @file variantCode.h
/// @brief Variant client ast code for Boost Spirit X3 Commands3
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

// 3rd party libraries
//#include <Streaming.h>

#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include "ArduinoCode.h"

namespace x3 = boost::spirit::x3;

namespace client { namespace ast
{

/// single line comment
struct SingleLineComment{};
/// whitespace
struct Whitespace       {};
/// event structure to store the results from parsing define statements
struct Event : x3::position_tagged           {
    Event(std::string const &name = "", int n = 0, int e = 0) : name(name), nn(n), en(e) {}
    std::string name;
    int nn; // Node number
    int en; // Event number
};
/// for define command - no longer used.
struct Define           {
    Define(std::string const &name = "", int n = 0, int e = 0) : name(name), nn(n), en(e) {}
    std::string name;
    int nn; // Node number
    int en; // Event number
};

/// to hold state off on.
enum class on_off_t { off, on };

/// to hold boolean operators and or not.
/// and or not are reserved by C++
enum class boolean_t { and_, or_, not_ };

/// to hold sequence operators before after.
enum class sequence_t { before, after };

/// to hold time unit alternatives
enum class time_unit_t { s, sec, ms };

/// State to store the state objects
struct State           {
    State(std::string const &name = "",on_off_t on_off = on_off_t::off) : name(name), on_off(on_off) { }
    std::string name;
    on_off_t on_off;
};

/// Received ($name) has nearly the same structure as State
struct Received           {
    Received(std::string const &name = "") : name(name) { }
    std::string name;
};

/// Item on/off$name is used in several contexts.
struct Item              {
    Item(on_off_t on_off = on_off_t::off, std::string const &name = "") : on_off(on_off), name(name) { }
    on_off_t on_off;
    std::string name;
};

/// Time for within or delay with time unit
struct Time {
  Time(int time=0,time_unit_t time_unit=time_unit_t::s) : time(time), time_unit(time_unit) {}
  int time;
  time_unit_t time_unit;
};

using TermVariant = x3::variant<State, Received>;

/// BooleanTerm needs to be a variant
struct BooleanTerm : TermVariant  {};

/// BooleanExpression can have multiple terms
struct BooleanExpression {
   std::vector<BooleanTerm> terms;  
};

using ActionVariant = x3::variant<Item>;

/// Action needs to be a variant
struct Action : ActionVariant  {}; 

/// Actions can have multiple terms
struct Actions {
   std::vector<Action> actions;
}; 

/// Simple example of when command
/// when state($name1) is off within 1sec send on$name2
/// Description from https://www.merg.org.uk/merg_wiki/doku.php?id=cbus:cancompute
/// Where <rule> is:
///   "when" <booleanExpression> "within" <time> <actions> ["then" <actions>] ";"
struct When /*: x3::position_tagged */ {
  When() {}
  BooleanExpression expression;
  Time time;
  Actions actions;
  /// not always needed
  Actions then_actions; 
};

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

/// Store parsed events
std::map<std::string,std::pair<int,int>> events;

/// Store an event - checking for duplicate
inline void store(const Event &arg)
{
    if (events.count(arg.name) > 0) {
       Serial << "Duplicate event name " /*<< arg.name.c_str()*/ << endl;
    } else {
       events.insert(std::make_pair(arg.name,std::make_pair(arg.nn,arg.en)));
    }
}

template <typename out>
inline out &operator <<(out &stream, const Event& arg)
{
    stream << std::string("event ") << arg.name << " " << arg.nn << " " << arg.en << std::ends;
    store(arg);
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

/// Store the result for person
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

/// Token structure to hold all the variant options. This is the heart of the parsing structure. New elements can be added by changing the definition of Variant 
struct Token : Variant, x3::position_tagged {
    using Variant::Variant;
    using Variant::operator=; // This is what was needed to get x3 variant working.
};

/// Print operator for a Token
/// This is able to output from all of the variants if they have an operator <<.
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
