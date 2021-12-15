/// @file any_parser.hpp
/// @brief Arduino RP2040 code for Boost Spirit X3 any_parser example
///
/// This is based on an example: 
///
/// https://stackoverflow.com/questions/60171119/boost-spirit-x3-lazy-parser
///
/// I have modified the example to wrap the variant data type in a struct.
///
/// I have been unable to get this example to give the expected answer.
///
/// This is a version where I have brought in a lot of things learned in LazyParser6.
///
/// This still fails on every parse, which was the case with the previous version.
///
/// This may help:
///
/// https://stackoverflow.com/questions/43278723/spirit-x3-locally-defined-rule-definition-must-have-an-attribute-attached
///
/// and here: https://github.com/boostorg/spirit/issues/530
///
/// x3::rule<class Rule_Class, boost::variant<File, Directory>> parser;
/// auto const parser_def = lazy<FileParser> | lazy<DirectoryParser>;
/// BOOST_SPIRIT_DEFINE(parser)

///
/// I have an equivalent called LazyWithout which uses a different struct called lazy_rule.
///
/// It replaces any_parser within a similar structure and it does work.

#ifndef ANY_PARSER_HPP
#define ANY_PARSER_HPP

#include <iomanip>
#include <boost_spirit_home_x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

namespace x3 = boost::spirit::x3;

/// namespace named any_parser_or_something to avoid a name clash with any_parser.
namespace any_parser_or_something {
    template <typename T>
    struct as_type {
        template <typename...> struct Tag{};

        template <typename P>
        auto operator[](P p) const {
            return x3::rule<Tag<T, P>, T> {"as"} = x3::as_parser(p);
        }
    };

    template <typename Tag>
    struct set_context_type {
        template <typename P>
        auto operator[](P p) const {
            auto action = [](auto& ctx) {
                x3::get<Tag>(ctx) = x3::_attr(ctx);
            };
            return x3::omit [ p [ action ] ];
        }
    };

    template <typename Tag>
    struct lazy_type : x3::parser<lazy_type<Tag>> {
        using attribute_type = typename Tag::attribute_type; // TODO FIXME?

        template<typename It, typename Ctx, typename RCtx, typename Attr>
        bool parse(It& first, It last, Ctx& ctx, RCtx& rctx, Attr& attr) const {
            auto& subject = x3::get<Tag>(ctx);

            return x3::as_parser(subject)
                .parse(
                    first, last, 
                    std::forward<Ctx>(ctx),
                    std::forward<RCtx>(rctx),
                    attr);
        }
    };

    template <typename T> static const as_type<T>          as{};
    template <typename T> static const set_context_type<T> set_context{};
    template <typename T> static const lazy_type<T>        lazy{};
}

/// for the AST of the problem
namespace ast {

/// names for the variant values. These are used for parsing.
const char *names[] = {"integer_value","bool_value","double_value","quoted_string"};

/// @brief Variant to cover different types of data to be found
///
/// This is now defined following example in Rexpr.
struct Value : x3::variant<int, bool, double,std::string>
{
        using base_type::base_type;
        using base_type::operator=;  
};


/// @brief Value_struct putting a value into a struct
///
/// I have done this to be able to declare BOOST_FUSION_ADAPT_STRUCT
/// as otherwise the code fails attempting to use Value as a boost fusion view.
/// Boost Fusion does not support boost variant.
/// See the Changelog: https://www.boost.org/doc/libs/1_78_0/libs/fusion/doc/html/fusion/change_log.html
struct Value_struct : x3::position_tagged {
  Value value;
};

/// defined for use in specializations
struct Rule_tag;

/// Stream output for a variant type provided operators exist for all the alternatives.
inline Print &operator <<(Print &stream, const Value_struct &arg)
{
   std::stringstream s;
   s << arg.value.get() << std::ends;
   stream.print(s.str().c_str());
   return stream;
}

/// Stream output for a variant type provided operators exist for all the alternatives.
inline Print &operator <<(Print &stream, const Value &arg)
{
   std::stringstream s;
   s << arg.get() << std::ends;
   stream.print(s.str().c_str());
   return stream;
}

}  // namespace ast

std::vector<ast::Value_struct> ast_structs;
std::vector<ast::Value> ast_values;

/// Vector to store parsed results.
std::vector<ast::Value_struct> results;
//std::vector<ast::Value> results;

/// Vector to store parsed failures.
std::vector<ast::Value_struct> failures;
//std::vector<ast::Value> failures;

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

/// This is here to hold some definitions and specializations specific to the problem.
namespace any_parser_or_something {

   using It    = std::string::const_iterator;
   //typedef x3::any_parser<It,ast::Value_struct> Rule;
   using Rule = x3::any_parser<It,ast::Value_struct>;

/// do_lazy_type assuming that attr is a struct wrapping a variant value.
/// Specialisation once Rule is defined.
    using ast::Rule_tag;

/// quoted string made into a rule.
    struct quoted_string_class;
    x3::rule<quoted_string_class, std::string>        const quoted_string = "quoted_string";
    auto const quoted_string_def = x3::lexeme['"' >> +(x3::char_ - '"') >> '"'];
    BOOST_SPIRIT_DEFINE(quoted_string);

}

namespace special_rules
{
    using ast::names;
    using any_parser_or_something::Rule;
    using any_parser_or_something::quoted_string;
    using namespace any_parser_or_something;
//    using namespace lazy_parser;
 
    using It    = std::string::const_iterator;
    //using Rule  = lazy_rule<It, ast::Value>;
    typedef x3::any_parser<It, ast::Value_struct> Rule;

    struct inner_rule_class {};
    auto const inner_rule_def = lazy<Rule>;
    x3::rule<inner_rule_class> const inner_rule = "inner_rule";
    BOOST_SPIRIT_DEFINE(inner_rule)

    struct my_rule_class : parser::annotate_position {};
    x3::rule<my_rule_class, ast::Value_struct> const my_rule = "my_rule";

/// symbol table to hold the rules which are declared 
    x3::symbols<Rule> options;

/// This is following the X3 example for Roman Numbers.
/*
    struct options_ : x3::symbols<Rule>
    {
      options_()
      {
        add
           (ast::names[0], x3::int_)
           (ast::names[3], quoted_string)
           ;
      }
    } options;
*/
/// moved from the application end
/// I do not know why these compile later but not here.
/// It is because they need to be inside a function.
/// Where do I then execute this? - in run_lazy_example
/// It does not make any difference - the case still fails to find anything.
    x3::symbols<Rule> &add_options(x3::symbols<Rule> &options) {
       options.add(ast::names[0], x3::int_);
       options.add(ast::names[3], quoted_string);
       return options;
    }
    

    auto const my_rule_def
      = x3::expect[set_context<Rule>[options]] >> ':' > inner_rule;
      //= x3::expect[set_context<Rule>[options]] >> ':' > lazy<Rule>;

/// x3::rule<class Rule_Class, boost::variant<File, Directory>> parser;
/// auto const parser_def = lazy<FileParser> | lazy<DirectoryParser>;
/// BOOST_SPIRIT_DEFINE(parser)
/// Do I need this for the attribution?
/// It seems that while what is in my_rule_def is a valid rule it does not work in BOOST_SPIRIT_DEFINE.
    BOOST_SPIRIT_DEFINE(my_rule);

/// rule_parser defined for the options which have been declared adding expect
    auto const rule_parser = x3::with<Rule>(Rule{}) [
        x3::expect[set_context<Rule>[options]] >> ':' > inner_rule
        //x3::expect[set_context<Rule>[options]] >> ':' > lazy<Rule>
        //my_rule //now works with inner_rule. Still gives bad answers.
    ];

}

/// @brief This is adapted from the example
///
/// I have been unable to use std::string with options - it does not compile.
/// The parser is now passed Value_struct and I still do not get a success on parsing.
void run_lazy_example()
{
   namespace x3 = boost::spirit::x3;
   using namespace ast;
   using namespace special_rules;
   using It    = std::string::const_iterator;

   //using namespace any_parser_or_something;

   /// Add to options here. 
   options = add_options(options);

   auto run_tests = [=] {
        for (std::string const& input_ : {
                "integer_value: 42",
                "quoted_string: \"hello world\"",
                "bool_value: true",
                "double_value: 3.1415926",
            })
        {
            ast::Value_struct attr_struct;
            It start_ = begin(input_);       
            It end_   = end(input_);
            x3::position_cache<std::vector<It> > pos_cache(start_, end_);
            custom::diagnostics_handler<It> diags {start_, end_ };

/// parser augmented with the position cache and the diagnostics handler.
            auto const parser =
            x3::with<parser::annotate_position>(std::ref(pos_cache)) [
                x3::with<custom::diagnostics_handler_tag>(diags) [
                   //x3::with<parser::error_handler_tag>(errors) [
                      rule_parser
                      //my_rule //does not work here.
                   //]
                ]
            ];
            
            
            if (x3::phrase_parse(start_, end_, parser, x3::space, attr_struct)) {
                Serial << " -> success (" << attr_struct << ")\n";
                unsigned which = attr_struct.value.get().which();
                //unsigned which = attr.get().which();
                Serial << names[which] << " : " << attr_struct << endl;
                results.push_back(attr_struct);
            } else {
                /// doing it this way there is no expectation failure information.
                int wherep1 = boost::spirit::x3::where_was_I.size();
                int whatItp1 = boost::spirit::x3::what_iterator.size();
                Serial << " -> failed " << attr_struct.value.get().which() << "\n";
                Serial << "where_was_I.size() = " << wherep1 << endl;
                Serial << "what_iterator.size() = " << whatItp1 << endl;
                failures.push_back(attr_struct);
            }
        }

    
    };

    Serial << "Supporting only integer_value and quoted_string:\n";
    //options.add(names[0], x3::int_);
    //options.add(names[3], quoted_string);
/*  This does not compile so I have made it into a rule.
    options.add("quoted_string", as<std::string> 
    [
            // lexeme is actually redundant because we don't use surrounding skipper yet
            x3::lexeme [ '"' >> *('\\' >> x3::char_ | ~x3::char_('"')) >> '"' ]
        ]);
*/
//    Serial << "Rule: " << rule.get_info() << endl;
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
