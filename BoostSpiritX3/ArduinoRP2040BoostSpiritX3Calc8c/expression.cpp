/// @file expression.cpp
/// @brief instantiation of the expression type
/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "expression_def.hpp"
#include "config.hpp"

namespace client { namespace parser
{
    using context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;
    BOOST_SPIRIT_INSTANTIATE(expression_type, iterator_type, context_type);
}}
