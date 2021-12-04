/// @file statement.cpp
/// @brief instantiation for the statement type
/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
/// This switches off exceptions and provides a different way to get error information.
#define BOOST_SPIRIT_X3_NO_EXCEPTION

#include "statement_def.hpp"
#include "config.hpp"

namespace client { namespace parser
{
     BOOST_SPIRIT_INSTANTIATE(statement_type, iterator_type, context_type);
     //int x = context_type{};
}
}
