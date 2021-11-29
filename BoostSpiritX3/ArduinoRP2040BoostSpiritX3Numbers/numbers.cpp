/// @file numbers.cpp
/// @brief Simple example of parsing numbers
///
/// Source: https://www.py4u.net/discuss/106167 answer
///
/// Boost Spirit X3 AST not working with semantic actions when using separate rule definition and instantiation
///
///I am trying to use Boost Spirit X3 with semantic actions while parsing the structure to an AST.
///If I use a rule without separate definition and instantiation it works just fine.



#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <iostream>

namespace ast {
    struct ast_struct {
        int number;
        std::vector<int> numbers;
    };
}

BOOST_FUSION_ADAPT_STRUCT(ast::ast_struct, number, numbers)

namespace x3 = boost::spirit::x3;

namespace parsing {
    x3::rule<struct parser, ast::ast_struct> parser {"parser"};

    struct state_tag { };

    auto record_number = [](auto &ctx) { 
        unsigned& n = x3::get<state_tag>(ctx); // note: returns reference_wrapper<T>
        n = x3::_attr(ctx); 
    };

    auto parser_def = x3::rule<struct parser_def, ast::ast_struct> {} 
                   %= x3::int_[record_number] >> +(x3::omit[+x3::blank] >> x3::int_);

    BOOST_SPIRIT_DEFINE(parser)
}

void parse(const std::string &data) {
    using namespace std;

    ast::ast_struct ast;
    unsigned n = 0;
    auto parser = x3::with<parsing::state_tag>(ref(n)) [parsing::parser] >> x3::eoi;

    if (x3::parse(data.begin(), data.end(), parser, ast)) {
        cout << "n: " << n << ", ";
        copy(ast.numbers.begin(), ast.numbers.end(), ostream_iterator<int>(cout << ast.numbers.size() << " elements: ", " "));
        cout << "\n";
    } else
        cout << "Parse failed\n";
}

int main() {
    parse("3 1 2 3");
    parse("4 1 2 3 4");
}

/* Output
Prints

n: 3, 3 elements: 1 2 3 
n: 4, 4 elements: 1 2 3 4 

*/



