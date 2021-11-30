/// @file customdiagnostics.cpp
/// @brief example with custom diagnostics
///
/// Source:
/// https://stackoverflow.com/questions/61721421/customizing-the-full-error-message-for-expectation-failures-boostspiritx3
///
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <iomanip>
#include <string>
namespace x3 = boost::spirit::x3;

namespace ast {
    struct name : std::string, x3::position_tagged {
        using std::string::string;
        using std::string::operator=;
    };
    struct person   : x3::position_tagged { ast::name first_name, last_name; };
    struct employee : x3::position_tagged { int age; person who; double salary; };
    using boost::fusion::operator<<;
}

BOOST_FUSION_ADAPT_STRUCT(ast::person, first_name, last_name)
BOOST_FUSION_ADAPT_STRUCT(ast::employee, age, who, salary)

namespace custom {
    struct diagnostics_handler_tag;

    template <typename It> struct diagnostics_handler {
        It _first, _last;
        std::ostream& _os;

        void operator()(It err_first, std::string const& error_message) const {
            size_t line_no = 1;
            auto bol = _first;
            for (auto it = bol; it != err_first; ++it)
                if (*it == '\n') {
                    bol = it+1;
                    line_no += 1;
                }

            _os << "L:"<< line_no
                << ":" << std::distance(bol, err_first)
                << " " << error_message << "\n";
        }
    };

} // namespace custom

namespace parser {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    struct error_handler {
        template <typename It, typename E, typename Ctx>
        x3::error_handler_result on_error(It&, It const&, E const& x, Ctx const& ctx) {
            auto& handler = x3::get<custom::diagnostics_handler_tag>(ctx);
            handler(x.where(), "error: expecting: " + x.which());
            return x3::error_handler_result::fail;
        }
    };
	
	struct annotate_position {
        template <typename T, typename Iterator, typename Context>
        inline void on_success(const Iterator &first, const Iterator &last, T &ast, const Context &context)
        {
            auto &position_cache = x3::get<annotate_position>(context).get();
            position_cache.annotate(ast, first, last);
        }
    };

    struct quoted_string_class : annotate_position {};
    struct person_class : annotate_position {};
    struct employee_class : error_handler, annotate_position {};

    x3::rule<quoted_string_class, ast::name>     const name = "name";
    x3::rule<person_class,        ast::person>   const person        = "person";
    x3::rule<employee_class,      ast::employee> const employee      = "employee";

    auto const name_def
        = x3::lexeme['"' >> +(x3::char_ - '"') >> '"']
        ;
    auto const person_def
        = name > ',' > name
        ;

    auto const employee_def
        = '{' > x3::int_ > ',' > person > ',' > x3::double_ > '}'
        ;

    BOOST_SPIRIT_DEFINE(name, person, employee)

    auto const employees = employee >> *(',' >> employee);
}

void parse(std::string const& input) {
    using It = std::string::const_iterator;

    It iter = input.begin(), end = input.end();
    x3::position_cache<std::vector<It> > pos_cache(iter, end);
    custom::diagnostics_handler<It> diags { iter, end, std::clog };

    auto const parser =
        x3::with<parser::annotate_position>(std::ref(pos_cache)) [
            x3::with<custom::diagnostics_handler_tag>(diags) [
                 parser::employees
            ]
        ];

    std::vector<ast::employee> ast;
    if (phrase_parse(iter, end, parser >> x3::eoi, x3::space, ast)) {
        std::cout << "Parsing succeeded\n";

        for (auto const& emp : ast) {
            std::cout << "got: " << emp << std::endl;

            diags(pos_cache.position_of(emp.who.last_name).begin(), "note: that's a nice last name");
            diags(pos_cache.position_of(emp.who).begin(), "warning: the whole person could be nice?");
        }
    } else {
        std::cout << "Parsing failed\n";
        ast.clear();
    }
}

static std::string const
    good_input = R"({ 23, "Amanda", "Stefanski", 1000.99 },
        { 35, "Angie", "Chilcote", 2000.99 }
    )", 
    bad_input = R"(
        { 23,
 'Amanda', "Stefanski", 1000.99 },
    )";

int main() {
    std::cout << "With custom diagnostics only:" << std::endl;
    parse(good_input);

    std::cout << "\n\n ----- Now with parse error:" << std::endl;
    parse(bad_input);
}


/* Expected output

With custom diagnostics only:
Parsing succeeded
got: (23 (Amanda Stefanski) 1000.99)
L:1:16 note: that's a nice last name
L:1:6 warning: the whole person could be nice?
got: (35 (Angie Chilcote) 2000.99)
L:2:23 note: that's a nice last name
L:2:14 warning: the whole person could be nice?


 ----- Now with parse error:
L:2:13 error: expecting: person
Parsing failed

*/