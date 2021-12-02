/// @file custom.hpp
/// @brief Implementation code for custom diagnostics.
///
/// This code is the complete problem from the source except for the main program code.
///
/// The main change is to use my own output operator for the employee objects.
///
/// https://stackoverflow.com/questions/61721421/customizing-the-full-error-message-for-expectation-failures-boostspiritx3
///
/// At the moment the code works except for the call back to the on_error() function as the print added in it does not get executed.
///
/// The diagnostics handler does get called for the case with good input.
///
/// I have added more code for the error handler with the same result so I took it out again.
///
/// I have now been able to print out something from the failure case.


#ifndef CUSTOM_HPP
#define CUSTOM_HPP

namespace x3 = boost::spirit::x3;

namespace ast
{
    struct name : std::string, x3::position_tagged {
        using std::string::string;
        using std::string::operator=;
    };
    struct person   : x3::position_tagged { ast::name first_name, last_name; };
    struct employee : x3::position_tagged { int age; person who; double salary; };

    //using boost::fusion::operator<<;
    /// I cannot use the fusion IO so I am instead doing this which works.
    inline Print &operator <<(Print &stream, const employee &emp)
    {
       stream.print("(");
       stream.print(emp.age);
       stream.print(" (");
       stream.print(emp.who.first_name.c_str());
       stream.print(" ");
       stream.print(emp.who.last_name.c_str());
       stream.print(") ");
       stream.print(emp.salary);       
       stream.print(")");
      return stream;
    }

}

/// custom namespace for the diagnostics handler
namespace custom {
    struct diagnostics_handler_tag;

    template <typename It> struct diagnostics_handler {
        It _first, _last;
        //std::ostream& _os;

        void operator()(It err_first, std::string const& error_message) const {
            size_t line_no = 1;
            auto bol = _first;
            for (auto it = bol; it != err_first; ++it)
                if (*it == '\n') {
                    bol = it+1;
                    line_no += 1;
                }

            Serial  << "L:"<< line_no
                    << ":" << std::distance(bol, err_first)
                    << " " << error_message << "\n";
        }
    };

} // namespace custom

///////////////////////////////////////////////////////////////////////////////
///  Our employee parser
///////////////////////////////////////////////////////////////////////////////

/// Parser namespace
namespace parser
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;
    struct error_handler_tag;

/// error handler
    struct error_handler {
        template <typename It, typename E, typename Ctx>
        x3::error_handler_result on_error(It&, It const&, E const& x, Ctx const& ctx) {
            auto& handler = x3::get<custom::diagnostics_handler_tag>(ctx);
            handler(x.where(), "error: expecting: " + x.which());
            Serial << "x.which() : " <<  x.which() << endl;
            return x3::error_handler_result::fail;
        }
    };

/// annotate position
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
    x3::rule<class employee, ast::employee> const employee = "employee";

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

/// parse function
void parse(std::string const& input) {
    using It = std::string::const_iterator;

    It iter = input.begin(), end = input.end();
    x3::position_cache<std::vector<It> > pos_cache(iter, end);
    custom::diagnostics_handler<It> diags { iter, end }; //std::clog };
    //parser::error_handler errors;

    auto const parser =
        x3::with<parser::annotate_position>(std::ref(pos_cache)) [
            x3::with<custom::diagnostics_handler_tag>(diags) [
                 //x3::with<parser::error_handler_tag>(errors)
                 //[ parser::employees ]
                 parser::employees
            ]
        ];

    std::vector<ast::employee> ast;
    if (phrase_parse(iter, end, parser >> x3::eoi, x3::space, ast)) {
        Serial << "Parsing succeeded\n";

        for (auto const& emp : ast) {
            Serial << "got: " << emp << endl;

            diags(pos_cache.position_of(emp.who.last_name).begin(), "note: that's a nice last name");
            diags(pos_cache.position_of(emp.who).begin(), "warning: the whole person could be nice?");
        }
    } else {
        using E = boost::spirit::x3::expectation_failure<It>;
        Serial << "Parsing failed\n";
        Serial << "iter has " << *iter << endl;
        E e(iter,"test of expectation failure");
        diags(iter," is the location of the failure");
        
        // Found here: 
        // https://stackoverflow.com/questions/31553900/avoid-throwing-expectation-failure-when-expectation-parser-fails
        if (iter != end )
           Serial << "Remaining unparsed: " << std::string(iter,end) << endl;
        ast.clear();
    }
}


#endif
