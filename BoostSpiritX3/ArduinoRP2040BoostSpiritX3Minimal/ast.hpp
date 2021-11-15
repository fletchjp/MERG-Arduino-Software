/// @file ast.hpp
/// @brief Our employee AST struct
///
/// This defines the struct to receive the parsed data.


/**=============================================================================
    Copyright (c) 2002-2018 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_MINIMAL_AST_HPP)
#define BOOST_SPIRIT_X3_MINIMAL_AST_HPP

//#include <boost/fusion/include/io.hpp>
// 3rd party libraries
#include <Streaming.h>

#undef F


#include <iostream>
#include <string>

namespace client { namespace ast
{
///////////////////////////////////////////////////////////////////////////
///  Our employee AST struct employee
///////////////////////////////////////////////////////////////////////////
    struct employee
    {
        int age;
        std::string forename;
        std::string surname;
        double salary;
    };

    /// I cannot use the fusion IO so I am instead doing this which works.
    inline Print &operator <<(Print &stream, const employee &emp)
    {
       stream.print("[");
       stream.print(emp.age);
       stream.print(",");
       stream.print(emp.surname.c_str());
       stream.print(",");
       stream.print(emp.forename.c_str());
       stream.print(",");
       stream.print(emp.salary);       
       stream.print("]");
      return stream;
    }

    //using boost::fusion::operator<<;
}}

#endif
