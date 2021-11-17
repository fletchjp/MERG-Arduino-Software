/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_REPR_PRINTER_HPP)
#define BOOST_SPIRIT_X3_REPR_PRINTER_HPP

#include "ast.hpp"

#include <ostream>

namespace rexpr { namespace ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  Print out the rexpr tree
    ///////////////////////////////////////////////////////////////////////////
    int const tabsize = 4;

    struct rexpr_printer
    {
        typedef void result_type;

        rexpr_printer(int indent = 0)
          : indent(indent) {}

        void operator()(rexpr const& ast) const
        {
            Serial << '{' << endl;
            for (auto const& entry : ast.entries)
            {
                tab(indent+tabsize);
                Serial << '"' << entry.first << "\" = ";
                boost::apply_visitor(rexpr_printer(indent+tabsize), entry.second);
            }
            tab(indent);
            Serial << '}' << endl;
        }

        void operator()(std::string const& text) const
        {
            Serial << '"' << text << '"' << endl;
        }

        void tab(int spaces) const
        {
            for (int i = 0; i < spaces; ++i)
                Serial << ' ';
        }

        //std::ostream& out;
        int indent;
    };
}}

#endif
