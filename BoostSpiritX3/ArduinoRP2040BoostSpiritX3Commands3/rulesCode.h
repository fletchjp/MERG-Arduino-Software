/// @file rulesCode.h
/// @brief rules client parser code for Boost Spirit X3 Commands3
///
/// I have moved the rules out of the top level into this separate file.
///
/// I now find that I have enough tools and knowledge of Boost Spirit X3 that it is easier to build new items.
/// There is still a big task to do with storage of the results.
///
/// At the same time I have had a look at the CANCOMPUTE grammar and there is more to it than I remembered.
/// https://www.merg.org.uk/merg_wiki/doku.php?id=cbus:cancompute
///
/// Parsing from enums into a struct adapted from:
/// https://stackoverflow.com/questions/37749344/parsing-into-structs-with-boolean-or-enum-members-with-boost-spirit-x3


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

        /// @brief definition of as lambda function used to generate sone of the rules
        ///
        /// Rules defined using as cannot accept subrules such as quoted string.
        /// Those are defined using BOOST_SPIRIT_DEFINE.
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
        using client::ast::boolean_t;
        using client::ast::sequence_t;
        using client::ast::time_unit_t;
        
        struct event_name_class;
        struct event_class;
        struct state_class;
        struct received_class;
        struct item_class;
        struct time_class;
        struct when_class;
        struct quoted_string_class;
        struct Person_class;


        /// on_off table parsing on off into a struct.
        struct on_off_table : x3::symbols<on_off_t> {
        on_off_table() {
            add ("off", on_off_t::off)
                ("on" , on_off_t::on);
          }
        } const on_off;

        /// boolean table to parse and or not
        struct boolean_table : x3::symbols<boolean_t> {
        boolean_table() {
            add ("and",   boolean_t::and_)
                ("or" ,   boolean_t::or_)
                ("not",   boolean_t::not_);
          }
        } const and_or_not;

        /// sequence table to parse before after 
        struct sequence_table : x3::symbols<sequence_t> {
        sequence_table() {
            add ("before", sequence_t::before)
                ("after" , sequence_t::after);
          }
        } const sequence;

        /// time unit table to parse s sec ms
        struct time_unit_table : x3::symbols<time_unit_t> {
        time_unit_table() {
            add ("s"  ,   time_unit_t::s)
                ("sec",   time_unit_t::sec)
                ("ms" ,   time_unit_t::ms);
          }
        } const time_unit;

        x3::rule<event_name_class, std::string>           const event_name = "event_name";
        x3::rule<event_class, client::ast::Event>         const event = "event"; ///< $name = NN:nn EN:en 
        x3::rule<state_class, client::ast::State_>        const state_ = "state"; ///< state($name) is on/off   
        x3::rule<received_class, client::ast::Received>   const received = "received"; ///< received($name)
        x3::rule<item_class, client::ast::Item>           const item = "item";
        x3::rule<time_class, client::ast::Time>           const time = "time";
        x3::rule<when_class, client::ast::When>           const when = "when";       
        x3::rule<quoted_string_class, std::string>        const quoted_string = "quoted_string";
        x3::rule<Person_class, client::ast::Person>       const Person = "person";

        /// @brief identifier rule to parse a string without quote marks
        ///
        /// Adapted from https://stackoverflow.com/questions/59759810/how-do-you-get-a-string-out-of-a-boost-spirit-x3-lexeme-parser
        x3::rule<class identifier_rule_, std::string> const identifier_rule = "identifier_rule";
        auto const identifier_rule_def = x3::lexeme[(x3::alpha | x3::char_('$')) >> *(x3::alnum)];
        BOOST_SPIRIT_DEFINE(identifier_rule)

        /// The state rule parses an identifier and its state
        auto const state__def = lit("state(") >> identifier_rule >> ")" >> omit[+space] >> "is" >> omit[+space] >> on_off;
        /// The received rule parses an identifier
        auto const received_def = lit("received(") >> identifier_rule >> ")";
        /// The item rule parses on/off$name
        auto const item_def = on_off >> identifier_rule;
         /// The item rule parses on/off$name
        auto const time_def = "within" >> omit[+space] >> int_ >> time_unit;
        /// The event rule parses an identifier and two numbers
        auto const event_def = lit("define") >> omit[+space] >> identifier_rule >> omit[+space] >> '=' 
                                             >> omit[+space] >> lit("NN:") >> int_
                                             >> omit[+space] >> lit("EN:") >> int_;
        // Simple version of when rule with one state and one item.
        //auto const when_def = lit("when") >> omit[+space] >> state >> omit[+space] 
        //                     >> "within" >> omit[+space] >> time >> omit[+space] >> item;
        auto const quoted_string_def = lexeme['"' >> +(char_ - '"') >> '"'];
        /// The person rule uses omit[+space] to discard spaces after a keyword.
        auto const Person_def = lit("person") >> omit[+space] >> quoted_string >> ',' >> quoted_string;
         
        BOOST_SPIRIT_DEFINE(state_,received,item,time,event,quoted_string,Person);

        /// @brief Simple version of when rule with one state and one item.
        ///
        /// For some reason this rule did not parse when I set it up.
        /// I have separated out the state part and that does now work.
        /// I think the next stage is to get the other two components time and item working separately.
        auto const when_def = lit("when") >> omit[+space] >> state_ >> omit[+space] 
                                  >> time >> omit[+space] >> item;

        BOOST_SPIRIT_DEFINE(when);
        
        /// rule definition - singleLineComment
        auto singleLineComment = as<SingleLineComment>("//" >> x3::omit[*(x3::char_ - x3::eol)]);
        /// rule definition - Whitespace
        auto whitespace        = as<Whitespace>       (x3::omit[+x3::ascii::space]);
        /// rule definition - Define - for the moment just identify the keyword.
        auto define             = as<Define>            ("define" >> x3::omit[*(x3::char_ - x3::eol)]);
        // rule definition - When - for the moment just identify the keyword.
        //auto when              = as<When>             ("when" >> x3::omit[*(x3::char_ - x3::eol)]);
        /// rule definition - Token- this is the Variant for all the rules.
        auto token             = as<Token>            (singleLineComment | whitespace | event | state_ | Person | time | item | when, "token");
    }
}




#endif
