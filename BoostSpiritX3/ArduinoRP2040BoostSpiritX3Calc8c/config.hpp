/// @file config.hpp
/// @brief configuration of the parser
///
/// This is important. It is possible to include more than one with<> expression in the binder.
/// This means that the the context_type has to be adapted and only one with<> can be included at each stage.
///
/// There is an example here which I did not understand at first:
/// https://coderedirect.com/questions/389812/boost-spirit-x3-not-compiling
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
    typedef typename custom::diagnostics_handler_tag diagnostics_handler_tag;
    typedef typename custom::diagnostics_handler<iterator_type> diagnostics_handler_type;
    typedef typename boost::spirit::x3::position_cache<std::vector<iterator_type>> position_cache_type;
    //typedef x3::phrase_parse_context<x3::ascii::space_type>::type context_type;
    //using context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;
#endif

#ifdef BOOST_SPIRIT_X3_NO_EXCEPTION
    /// first_context_type handles the position cache
    typedef x3::context<
    	client::parser::position_cache_tag
      , std::reference_wrapper<position_cache_type>
      , phrase_context_type>
    first_context_type;
    /// context_type handles the diagnostics handler
    typedef x3::context<
        custom::diagnostics_handler_tag
      , diagnostics_handler_type   //std::reference_wrapper<diagnostics_handler_type>
      , first_context_type>
    context_type;
#else
    typedef x3::context<
        error_handler_tag
      , std::reference_wrapper<error_handler_type>
      , phrase_context_type>
    context_type;
#endif
/// To generate an error
/// int x = context_type{};
}}

#endif
