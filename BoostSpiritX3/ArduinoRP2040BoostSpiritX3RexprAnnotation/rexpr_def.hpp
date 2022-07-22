/// @file rexpr_def.hpp
/// @brief Rexpr parser definition
///
/// Error handling has been disabled.
///
/// Annotation has been removed temporarily.

/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_REXPR_REXPR_DEF_HPP)
#define BOOST_SPIRIT_X3_REXPR_REXPR_DEF_HPP

#include "ast.hpp"
#include "ast_adapted.hpp"
#include "error_handler.hpp"
#include "rexpr.hpp"
//#include "annotation.hpp"
#include "config.hpp"

#include <boost/spirit/home/x3.hpp>
//#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
//#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

namespace rexpr { namespace parser
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;

    using ascii::char_;
    using ascii::string;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    struct rexpr_value_class;
    struct rexpr_key_value_class;
    //struct rexpr_inner_class;
    struct rexpr_class;

    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    x3::rule<rexpr_value_class, ast::rexpr_value> const
        rexpr_value = "rexpr_value";

    x3::rule<rexpr_key_value_class, ast::rexpr_key_value> const
        rexpr_key_value = "rexpr_key_value";

/*    x3::rule<rexpr_inner_class, ast::rexpr> const
        rexpr_inner = "rexpr";

    rexpr_type const rexpr = "rexpr";
*/
    x3::rule<rexpr_class, ast::rexpr> const
        rexpr = "rexpr";

    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    auto const quoted_string =
        lexeme['"' >> *(char_ - '"') >> '"'];

    auto const rexpr_value_def =
        quoted_string | rexpr;

    auto const rexpr_key_value_def =
        quoted_string >> '=' >> rexpr_value;

    /*auto const rexpr_inner_def =
        '{' >> *rexpr_key_value >> '}';
    */
    auto const rexpr_def =
        '{' >> *rexpr_key_value >> '}';

    //auto const rexpr_def = rexpr_inner_def;

    BOOST_SPIRIT_DEFINE(rexpr_value, rexpr, rexpr_key_value);
    //BOOST_SPIRIT_DEFINE(rexpr_value, rexpr, rexpr_inner, rexpr_key_value);

    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;

        ///////////////////////////////////////////////////////////////////////
        //  Our annotation handler
        ///////////////////////////////////////////////////////////////////////

        // Correction from here for 'no matching function' error
        // https://stackoverflow.com/questions/57262850/boost-spirit-x3-not-compiling
        // except that it does not work. More thinking is needed.
        
        // tag used to get the position cache from the context
        struct position_cache_tag;
        // Added code not working
        /*
        using position_cache = boost::spirit::x3::position_cache<std::vector<iterator_type>>;
        */
        //using position_cache = boost::spirit::x3::position_cache;
        /*
        using simple_context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;

        using context_type = boost::spirit::x3::context<
            rexpr::parser::position_cache_tag,
            std::reference_wrapper<position_cache>, 
            simple_context_type
        >;
        */
        // Another discussion which may help.
        // https://stackoverflow.com/questions/65614720/boost-spirit-x3-tokenizer-with-annotation-does-not-work
        struct annotate_position
        {
            template <typename T, typename Iterator, typename Context>
            inline void on_success(Iterator const& first, Iterator const& last
            , T& ast, Context const& context)
            {
                auto& position_cache = x3::get<position_cache_tag>(context).get();
                position_cache.annotate(ast, first, last);
            }
        };

    // We want these to be annotated with the iterator position.
    struct rexpr_value_class /* : annotate_position /*_on_success*/ {};
    struct rexpr_key_value_class /* : x3::annotate_position /*_on_success*/ {};
    //struct rexpr_inner_class /* : x3::annotate_position /*_on_success*/ {};

    // We want error-handling only for the start (outermost) rexpr
    // rexpr is the same as rexpr_inner but without error-handling (see error_handler.hpp)
    //struct rexpr_class : x3::annotate_on_success, error_handler_base {};
    // I have taken out the error handling as it uses exceptions.
    struct rexpr_class /* : annotate_position /*_on_success , error_handler_base*/ {};
}}
/*
 * Moved to the main program as here the compiler complains it is defined twice!!
 * This may be because it is also called in rexpr.cpp.
namespace rexpr
{
    parser::rexpr_type const& rexpr()
    {
        return parser::rexpr;
    }
}
*/
#endif