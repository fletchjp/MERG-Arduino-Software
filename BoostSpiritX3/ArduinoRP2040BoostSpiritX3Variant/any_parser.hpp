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
//using boost::spirit::x3::char_;

typedef char const* iterator_type;
typedef decltype(make_context<skipper_tag>(space)) context_type;

/// simple test of any_parser
bool basic_test()
{ // basic tests
        using x3::lit;
        auto a = lit('a');
        auto b = lit('b');
        auto c = lit('c');

        {
            any_parser<iterator_type> p =
                *(a | b | c);
            char const* in("abcabcacb");
            char const* last = in;
            while (*last) last++;
            return x3::parse(in, last, p);   
            //BOOST_TEST(test("abcabcacb", start));
        }
        return true;
}

/// use lambda function to get the output
bool another_test()
{ 
        std::string s;
        auto f = [&](auto& ctx){ s = _attr(ctx); };

        using x3::lit;
        auto a = lit('a');
        auto b = lit('b');
        auto c = lit('c');

        {
          // this does not compile although it is a copy from a test file.
          // It is the rhs of the parser which causes the problem.
             x3::any_parser<iterator_type, std::string> p = char_ >> *(',' >> char_);
          //  x3::any_parser<iterator_type, unused_type> p; //= char_ >> *(',' >> char_);
            char const* in("a,b,c,d,e,f");
            char const* last = in;
            while (*last) last++;
            bool res =  x3::parse(in, last, p[f]);
            Serial << "output : " << s << endl; // output is blank!
            return res;
        }
        return true;
}


#endif
