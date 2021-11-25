/// @file rulesCode.h
/// @brief rules client parser code for Boost Spirit X3 Commands3

#ifndef RULES_CODE_H
#define RULES_CODE_H

// 3rd party libraries
//#include <Streaming.h>

#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include "ArduinoCode.h"

//using client::ast::person;

/// namespace for the project changed from anonymous namespace to client to match client::ast.
namespace client {
    struct position_cache_tag;
    
    /// Parser namespace defines the parser structure for the problem.
    namespace Parser {

        /// annotate_position provides the on_success operator
        struct annotate_position {
            template <typename T, typename Iterator, typename Context>
                inline void on_success(Iterator first, Iterator last, T &ast, Context const &context) const {
                    auto &position_cache = x3::get<position_cache_tag>(context);
                    position_cache.annotate(ast, first, last);
                }
        };
        /// unique on success hook classes

        /// no annotate_position mix-in
        template <typename> struct Hook {};
        /// When there is a specialisation it is annotated.
        template <> struct Hook<Token> : annotate_position   {}; 

        /// definition of as lambda function used to generate the rules
        template <typename T>
        static auto constexpr as = [](auto p, char const* name = typeid(decltype(p)).name()) {
            return x3::rule<Hook<T>, T> {name} = p;
        };

        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;

        using boost::spirit::x3::lexeme;
        using boost::spirit::x3::omit;
        using boost::spirit::x3::skip;
        using boost::spirit::x3::ascii::space;
        using boost::spirit::x3::lit;
        using boost::spirit::x3::int_;
        //using boost::spirit::x3::add;
        using ascii::char_;
        using client::ast::on_off_t;
        
        struct event_name_class;
        struct event_class;
        struct state_class;
        struct received_class;
        struct item_class;
        struct quoted_string_class;
        struct Person_class;

        /// Parsing into a struct adapted from:
        /// https://stackoverflow.com/questions/37749344/parsing-into-structs-with-boolean-or-enum-members-with-boost-spirit-x3
        struct on_off_table : x3::symbols<on_off_t> {
        on_off_table() {
            add ("off"   , on_off_t::off)
                ("on" ,    on_off_t::on);
          }
        } const on_off;

        x3::rule<event_name_class, std::string>           const event_name = "event_name";
        x3::rule<event_class, client::ast::Event>         const event = "event";
        x3::rule<state_class, client::ast::State>         const state = "state";
        x3::rule<received_class, client::ast::Received>   const received = "received";
        x3::rule<item_class, client::ast::Item>           const item = "item";
        x3::rule<quoted_string_class, std::string>        const quoted_string = "quoted_string";
        x3::rule<Person_class, client::ast::Person>       const Person = "person";


        /// Adapted from https://stackoverflow.com/questions/59759810/how-do-you-get-a-string-out-of-a-boost-spirit-x3-lexeme-parser
        x3::rule<class identifier_rule_, std::string> const identifier_rule = "identifier_rule";
        auto const identifier_rule_def = x3::lexeme[(x3::alpha | x3::char_('$')) >> *(x3::alnum)];
        BOOST_SPIRIT_DEFINE(identifier_rule)

        /// The state rule parses an identifier and its state
        auto const state_def = lit("state(") >> identifier_rule >> ")" >> omit[+space] >> "is" >> omit[+space] >> on_off;
        /// The received rule parses an identifier and the state to be queried
        auto const received_def = lit("received(") >> identifier_rule >> ")" >> omit[+space] >> "is" >> omit[+space] >> on_off;
        /// The item rule parses on/off$name
        auto const item_def = on_off >> identifier_rule;
        /// The event rule parses an identifier and two numbers
        auto const event_def = lit("define") >> omit[+space] >> identifier_rule >> omit[+space] >> '=' 
                                             >> omit[+space] >> lit("NN:") >> int_
                                             >> omit[+space] >> lit("EN:") >> int_;
                                             //>> x3::omit[*(x3::char_ - x3::eol)];
        auto const quoted_string_def = lexeme['"' >> +(char_ - '"') >> '"'];
        /// The person rule uses omit[+space] to discard spaces after a keyword.
        auto const Person_def = lit("person") >> omit[+space] >> quoted_string >> ',' >> quoted_string;
         
        BOOST_SPIRIT_DEFINE(state,received,item,event,quoted_string,Person);
        /// rules defined like this cannot accept subrules such as quoted string.
        /// rule definition - singleLineComment
        auto singleLineComment = as<SingleLineComment>("//" >> x3::omit[*(x3::char_ - x3::eol)]);
        /// rule definition - Whitespace
        auto whitespace        = as<Whitespace>       (x3::omit[+x3::ascii::space]);
        /// rule definition - Define - for the moment just identify the keyword.
        auto define             = as<Define>            ("define" >> x3::omit[*(x3::char_ - x3::eol)]);
        /// rule definition - When - for the moment just identify the keyword.
        auto when              = as<When>             ("when" >> x3::omit[*(x3::char_ - x3::eol)]);
        /// rule definition - Token- this is the Variant for all the rules.
        auto token             = as<Token>            (singleLineComment | whitespace | event | when | Person, "token");
    }
}




#endif
