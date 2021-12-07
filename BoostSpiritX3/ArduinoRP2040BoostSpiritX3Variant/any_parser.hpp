/// @file any_parser.hpp
/// @brief Arduino RP2040 code for Boost Spirit X3 any_parser simple example
///

#ifndef ANY_PARSER_HPP
#define ANY_PARSER_HPP

#include <boost_spirit_home_x3.hpp>

namespace x3 = boost::spirit::x3;

/// namespace named any_parser


/// somehow the pares here messes with the defintion of Variant_struct.
bool basic_test()
{ // basic tests
        using x3::lit;
        auto a = lit('a');
        auto b = lit('b');
        auto c = lit('c');

        {
            typedef std::string::const_iterator iterator_type;
            x3::any_parser<iterator_type> p =
                *(a | b | c);
            char const* in("abcabcacb");
            char const* last = in;
            while (*last) last++;
            return x3::parse(in, last, p);   
            //BOOST_TEST(test("abcabcacb", start));
        }
        return true;
}


#endif
