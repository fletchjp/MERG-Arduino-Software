/// @file any_parser.hpp
/// @brief Arduino RP2040 code for Boost Spirit X3 any_parser example
///
/// Lazy example modified to NOT use the any_parser code. It defines lazy_rule instead.
///
/// This is based on an example: 
///
/// https://stackoverflow.com/questions/60171119/boost-spirit-x3-lazy-parser
///
/// I have modified the example to wrap the variant data type in a struct.
///
/// The lazy parser is now defined in lazy_parser.hpp
///
/// This is the problem dependent part of the code.

#ifndef ANY_PARSER_HPP
#define ANY_PARSER_HPP

#include <iomanip>
#include <boost_spirit_home_x3.hpp>

#include "lazy_parser.hpp"

namespace x3 = boost::spirit::x3;

/// Variant to cover different types of data to be found
typedef x3::variant<int, bool, double,std::string> Value;

/// @brief Value_struct putting a value into a struct
///
/// I have done this to be able to declare BOOST_FUSION_ADAPT_STRUCT
/// as otherwise the code fails attempting to use Value as a boost fusion view.
/// Boost Fusion does not support boost variant.
/// See the Changelog: https://www.boost.org/doc/libs/1_78_0/libs/fusion/doc/html/fusion/change_log.html
struct Value_struct {
  Value value;
};

BOOST_FUSION_ADAPT_STRUCT(Value_struct,value)

/// Stream output for a variant type provided operators exist for all the alternatives.
inline Print &operator <<(Print &stream, const Value_struct &arg)
{
   std::stringstream s;
   s << arg.value.get() << std::ends;
   stream.print(s.str().c_str());
   return stream;
}


/// @brief This is adapted from the example
///
/// I have now been able to use std::string with options using the rule quoted_string
/// The parser is now passed Value_struct and I do now get success on parsing.
void run_lazy_example()
{
    //namespace x3 = boost::spirit::x3;

    using namespace lazy_parser;
 
    //using Value = boost::variant<int, bool, double, std::string>;
    //using Value = boost::variant<int, bool, double>;
    using It    = std::string::const_iterator;
    using Rule  = lazy_rule<It, Value>;

    x3::symbols<Rule> options;

    auto const parser = x3::with<Rule>(Rule{}) [
        set_lazy<Rule>[options] >> ':' >> do_lazy<Rule>
    ];


   auto run_tests = [=] {
        for (std::string const& input_ : {
                "integer_value: 42",
                "quoted_string: \"hello world\"",
                "bool_value: true",
                "double_value: 3.1415926",
            })
        {

            Value_struct attr;
            It start_ = begin(input_);       
            It end_   = end(input_);
            //std::cout << std::setw(36) << std::quoted(input);
            // Serial << std::quoted(input_) << endl;
            // experiment
            if (x3::phrase_parse(start_, end_, parser, x3::space, attr)) {
                Serial << " -> success (" << attr << ")\n";
            } else {
                Serial << " -> failed " << attr.value.get().which() << "\n";
            }

        }

    
    };

    Serial << "Supporting only integer_value and quoted_string:\n";
    options.add("integer_value", x3::int_);
    options.add("quoted_string", quoted_string);

    run_tests();

    Serial << "\nAdded support for double_value and bool_value:\n";
    options.add("double_value", x3::double_);
    options.add("bool_value", x3::bool_);

    run_tests();

  }
#endif
