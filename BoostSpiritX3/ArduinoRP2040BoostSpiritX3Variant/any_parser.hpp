/// @file any_parser.hpp
/// @brief Arduino RP2040 code for Boost Spirit X3 any_parser simple example
///
/// test adapted from Boost X3 test of any_parser.

#ifndef ANY_PARSER_HPP
#define ANY_PARSER_HPP

#include <boost_spirit_home_x3.hpp>

namespace x3 = boost::spirit::x3;
using namespace boost::spirit::x3::ascii;
using boost::spirit::x3::any_parser;
using boost::spirit::x3::make_context;
using boost::spirit::x3::lit;
using boost::spirit::x3::unused_type;
using boost::spirit::x3::phrase_parse;
using boost::spirit::x3::skip_flag;
using boost::spirit::x3::skipper_tag;
using boost::spirit::x3::_attr;


/// simple test of any_parser
bool basic_test()
{ // basic tests
        using x3::lit;
        auto a = lit('a');
        auto b = lit('b');
        auto c = lit('c');

        {
            //typedef std::string::const_iterator iterator_type;
            typedef const char* iterator_type;
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
