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
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

#include "lazy_parser.hpp"

namespace x3 = boost::spirit::x3;

namespace ast {

/// names for the variant values. These are used for parsing.
const char *names[] = {"integer_value","bool_value","double_value","quoted_string"};
/// Variant to cover different types of data to be found
typedef x3::variant<int, bool, double,std::string> Value;

/// @brief Value_struct declaring a wrapper struct around a value
///
/// I have done this to be able to declare BOOST_FUSION_ADAPT_STRUCT
/// as otherwise the code fails attempting to use Value as a boost fusion view.
/// Boost Fusion does not support boost variant.
/// See the Changelog: https://www.boost.org/doc/libs/1_78_0/libs/fusion/doc/html/fusion/change_log.html
struct Value_struct : x3::position_tagged {
  Value value;
};



/// Stream output for a variant type provided operators exist for all the alternatives.
inline Print &operator <<(Print &stream, const Value_struct &arg)
{
   std::stringstream s;
   s << arg.value.get() << std::ends;
   stream.print(s.str().c_str());
   return stream;
}

} // namespace ast

/// Vector to store parsed results.
std::vector<ast::Value_struct> results;
/// Vector to store parsed failures.
std::vector<ast::Value_struct> failures;

BOOST_FUSION_ADAPT_STRUCT(ast::Value_struct,value)

/// Parser namespace
namespace parser
{
    using namespace ast;
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;
    struct error_handler_tag;

/// error handler
    struct error_handler {
        template <typename It, typename E, typename Ctx>
        x3::error_handler_result on_error(It&, It const&, E const& x, Ctx const& ctx) {
            auto& handler = x3::get<custom::diagnostics_handler_tag>(ctx);
            handler(x.where(), "error: expecting: " + x.which());
            //Serial << "x.which() : " <<  x.which() << endl;
            return x3::error_handler_result::fail;
        }
    };

/// annotate position
    struct annotate_position {
        template <typename T, typename Iterator, typename Context>
        inline void on_success(const Iterator &first, const Iterator &last, T &ast, const Context &context)
        {
            auto &position_cache = x3::get<annotate_position>(context).get();
            position_cache.annotate(ast, first, last);
            //int x = Context{};
        }
    
    };

}

/// @brief This is adapted from the example
///
/// I have now been able to use std::string with options using the rule quoted_string
/// The parser is now passed Value_struct and I do now get success on parsing.
void run_lazy_example()
{
    using ast::names;
    using namespace lazy_parser;
 
    using It    = std::string::const_iterator;
    using Rule  = lazy_rule<It, ast::Value>;


/// symbol table to hold the rules which are declared 
    x3::symbols<Rule> options;

/// rule_parser defined for the options which have been declared adding expect
    auto const rule_parser = x3::with<Rule>(Rule{}) [
        x3::expect[set_lazy<Rule>[options]] >> ':' > do_lazy<Rule>
    ];

/// run_tests runs the parser over this sample data.
   auto run_tests = [=] {
        for (std::string const& input_ : {
                "integer_value: 42",
                "quoted_string: \"hello world\"",
                "bool_value: true",
                "double_value: 3.1415926",
            })
        {

            ast::Value_struct attr;
            It start_ = begin(input_);       
            It end_   = end(input_);
            x3::position_cache<std::vector<It> > pos_cache(start_, end_);
            custom::diagnostics_handler<It> diags {start_, end_ };
            parser::error_handler errors;

/// parser augmented with the position cache and the diagnostics handler.
            auto const parser =
            x3::with<parser::annotate_position>(std::ref(pos_cache)) [
                x3::with<custom::diagnostics_handler_tag>(diags) [
                   x3::with<parser::error_handler_tag>(errors) [
                      rule_parser
                   ]
                ]
            ];
            
            if (x3::phrase_parse(start_, end_, parser, x3::space, attr)) {
                Serial << " -> success (" << attr << ")\n";
                unsigned which = attr.value.get().which();
                Serial << names[which] << " : " << attr << endl;
                results.push_back(attr);
            } else {
                /// doing it this way there is no expectation failure.
                int wherep1 = boost::spirit::x3::where_was_I.size();
                int whatItp1 = boost::spirit::x3::what_iterator.size();
                /// I need to change the message to be the line where we are in the code.
                if (whatItp1 > 0) {
                    It iter = boost::spirit::x3::what_iterator[whatItp1-1];
                    //diags(iter,"error: expecting: " + *(iter));
                    // *(iter) gives a wrong location.
                }
                // start_ is still at the beginning.
                //diags(start_," is not defined");
                // This one causes a crash.
                //diags(pos_cache.position_of(start_).begin()," is not defined");
                //diags(pos_cache.position_of(attr)," is not defined");
                /// The value of the parsing type comes back as 0 for a failure.
                /// I will need to get the type information from the attribution.
                Serial << " -> failed " << attr.value.get().which() << "\n";
                Serial << "where_was_I.size() = " << wherep1 << endl;
                Serial << "what_iterator.size() = " << whatItp1 << endl;
                failures.push_back(attr);
            }

        }

    
    };

    Serial << "Supporting only integer_value and quoted_string:\n";
    options.add(names[0], x3::int_);
    options.add(names[3], quoted_string);

    run_tests();
    Serial << results.size() << " results stored" << endl;
    Serial << failures.size() << " failures stored" << endl;
    results.clear();
    failures.clear();
    Serial << "\nAdded support for double_value and bool_value:\n";
    options.add(names[2], x3::double_);
    options.add(names[1], x3::bool_);

    run_tests();
    Serial << results.size() << " results stored" << endl;
    Serial << failures.size() << " failures stored" << endl;

  }
#endif
