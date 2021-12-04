/// @file config.hpp
/// @brief configuration of the parser
///
/// This may need more changes to remove error handler references.
/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CALC8_CONFIG_HPP)
#define BOOST_SPIRIT_X3_CALC8_CONFIG_HPP
// 3rd party libraries
#include <Streaming.h>

#include <boost/spirit/home/x3.hpp>
#include "error_handler.hpp"
#include "custom.hpp"

namespace client { namespace parser
{
    typedef std::string::const_iterator iterator_type;
    typedef x3::phrase_parse_context<x3::ascii::space_type>::type phrase_context_type;
    typedef error_handler<iterator_type> error_handler_type;
#ifdef BOOST_SPIRIT_X3_NO_EXCEPTION
    typedef custom::diagnostics_handler<iterator_type> diagnostics_handler_type;
    typedef boost::spirit::x3::position_cache<std::vector<iterator_type>> position_cache_type;
    //typedef x3::phrase_parse_context<x3::ascii::space_type>::type context_type;
    //using context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;
#endif

    typedef x3::context<
#ifdef BOOST_SPIRIT_X3_NO_EXCEPTION
      //  x3::parse_pass_context_tag
      //, bool ,
        client::parser::position_cache_tag
      , std::reference_wrapper<position_cache_type>
//      ,  custom::diagnostics_handler_tag
//      , std::reference_wrapper<diagnostics_handler_type>
#else
        error_handler_tag
      , std::reference_wrapper<error_handler_type>
#endif
      , phrase_context_type>
    context_type;
/// To generate an error
/// int x = context_type{};
}}

#endif
