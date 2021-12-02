/// @file error_handling_eh.cpp
/// @brief this looks interesting
///
/// https://stackoverflow.com/questions/57048008/spirit-x3-is-this-error-handling-approach-useful

//#define BOOST_SPIRIT_X3_DEBUG
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <iomanip>

namespace x3 = boost::spirit::x3;

namespace square::peg {
    using namespace x3;

    struct with_error_handling {
        template<typename It, typename Ctx>
            x3::error_handler_result on_error(It f, It l, expectation_failure<It> const& ef, Ctx const&) const {
                std::string s(f,l);
                auto pos = std::distance(f, ef.where());

                std::cout << "Expecting " << ef.which() << " at "
                    << "\n\t" << s
                    << "\n\t" << std::setw(pos) << std::setfill('-') << "" << "^\n";

                return error_handler_result::fail;
            }
    };

    auto const eh = [](auto p) {
        struct _ : with_error_handling {};
        return rule<_> {} = p;
    };

    const auto quoted_string = eh(lexeme['"' > *(print - '"') > '"']);
    const auto bare_string   = eh(lexeme[alpha > *alnum] > ';');
    const auto two_ints      = eh(int_ > int_);

    const auto main          = quoted_string | bare_string | two_ints;
    using main_type = std::remove_cv_t<decltype(main)>;

    const auto entry_point   = skip(space)[ eh(expect[main] > eoi) ];
} // namespace square::peg

namespace boost::spirit::x3 {
    template <> struct get_info<int_type> {
        typedef std::string result_type;
        std::string operator()(int_type const&) const { return "integral number"; }
    };
    template <> struct get_info<square::peg::main_type> {
        typedef std::string result_type;
        std::string operator()(square::peg::main_type const&) const { return "quoted string, bare string or integer number pair"; }
    };
}

int main() {
    using It = std::string::const_iterator;

    for (std::string const input : { 
            "   -89 0038  ",
            "   \"-89 0038\"  ",
            "   something123123      ;",
            // undecidable
            "",
            // violate expecations, no successful parse
            "   -89 oops  ",   // not an integer
            "   \"-89 0038  ", // missing "
            "   bareword ",    // missing ;
            // trailing debris, successful "main"
            "   -89 3.14  ",   // followed by .14
        })
    {
        std::cout << "====== " << std::quoted(input) << "\n";

        It iter = input.begin(), end = input.end();
        if (parse(iter, end, square::peg::entry_point)) {
            std::cout << "Parsed successfully\n";
        } else {
            std::cout << "Parsing failed\n";
        }
    }
}
