/// @file yet_another.cpp
/// @brief another way to do this
///
/// https://wandbox.org/permlink/Up1mD3FTVbE02tJG

//#define BOOST_SPIRIT_X3_DEBUG
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <iomanip>

namespace x3 = boost::spirit::x3;

struct quoted : std::string {};
struct bare   : std::string {};
using  two_i  = std::pair<int, int>;
using Value = boost::variant<quoted, bare, two_i>;

static inline std::ostream& operator<<(std::ostream& os, Value const& v) {
    struct {
        std::ostream& _os;
        void operator()(quoted const& v) const { _os << "quoted(" << std::quoted(v) << ")";             } 
        void operator()(bare const& v) const   { _os << "bare(" << v << ")";                            } 
        void operator()(two_i const& v) const  { _os << "two_i(" << v.first << ", " << v.second << ")"; } 
    } vis{os};

    boost::apply_visitor(vis, v);
    return os;
}

namespace square::peg {
    using namespace x3;

    struct with_error_handling {
        struct diags;

        template<typename It, typename Ctx>
            x3::error_handler_result on_error(It f, It l, expectation_failure<It> const& ef, Ctx const& ctx) const {
                std::string s(f,l);
                auto pos = std::distance(f, ef.where());

                std::ostringstream oss;
                oss << "Expecting " << ef.which() << " at "
                    << "\n\t" << s
                    << "\n\t" << std::setw(pos) << std::setfill('-') << "" << "^";

                x3::get<diags>(ctx).push_back(oss.str());

                return error_handler_result::fail;
            }
    };

    template <typename T = x3::unused_type> auto const as = [](auto p) {
        struct _ : with_error_handling {};
        return rule<_, T> {} = p;
    };

    const auto quoted_string = as<quoted>(lexeme['"' > *(print - '"') > '"']);
    const auto bare_string   = as<bare>(lexeme[alpha > *alnum] > ';');
    const auto two_ints      = as<two_i>(int_ > int_);

    const auto main          = quoted_string | bare_string | two_ints;
    using main_type = std::remove_cv_t<decltype(main)>;

    const auto entry_point   = skip(space)[ as<Value>(expect[main] > eoi) ];
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
    using D = square::peg::with_error_handling::diags;

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
        Value v;
        std::vector<std::string> diags;

        if (parse(iter, end, x3::with<D>(diags) [square::peg::entry_point], v)) {
            std::cout << "Result value: " << v;
        } else {
            std::cout << "Parsing failed";
        }

        std::cout << " with " << diags.size() << " diagnostics messages: \n";

        for(auto& msg: diags) {
            std::cout << " - " << msg << "\n";
        }
    }
}

