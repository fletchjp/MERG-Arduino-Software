/// @file ArduinoRP2040BoostSpiritX3Custom.ino
/// @brief The employee example using custom error handling.
///
/// Taken from spirit/example/x3/employee and adapted using code from
/// https://stackoverflow.com/questions/61721421/customizing-the-full-error-message-for-expectation-failures-boostspiritx3
///
/// I now have the annotation of success working. The error message is not being called.
///
///


// 3rd party libraries
#include <Streaming.h>

#undef F
#undef min

#include <exception>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <cstdio>

#include "ArduinoCode.h"

#include <boost_spirit_home_x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
//#include <boost/fusion/sequence.hpp>
//#include <boost/fusion/include/sequence.hpp>

namespace x3 = boost::spirit::x3;

namespace ast
{
    struct name : std::string, x3::position_tagged {
        using std::string::string;
        using std::string::operator=;
    };
    struct person   : x3::position_tagged { ast::name first_name, last_name; };
    struct employee : x3::position_tagged { int age; person who; double salary; };
    using boost::fusion::operator<<;

    

    //using boost::fusion::operator<<;
    // I cannot use the fusion IO so I am instead doing this which works.
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

// We need to tell fusion about our employee struct
// to make it a first-class fusion citizen. This has to
// be in global scope.
BOOST_FUSION_ADAPT_STRUCT(ast::person, first_name, last_name)
BOOST_FUSION_ADAPT_STRUCT(ast::employee, age, who, salary)

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
    //  Our employee parser
    ///////////////////////////////////////////////////////////////////////////////
namespace parser
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    struct error_handler {
        template <typename It, typename E, typename Ctx>
        x3::error_handler_result on_error(It&, It const&, E const& x, Ctx const& ctx) {
            auto& handler = x3::get<custom::diagnostics_handler_tag>(ctx);
            handler(x.where(), "error: expecting: " + x.which());
            Serial << "x.which() : " <<  x.which() << endl;
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

void parse(std::string const& input) {
    using It = std::string::const_iterator;

    It iter = input.begin(), end = input.end();
    x3::position_cache<std::vector<It> > pos_cache(iter, end);
    custom::diagnostics_handler<It> diags { iter, end }; //std::clog };

    auto const parser =
        x3::with<parser::annotate_position>(std::ref(pos_cache)) [
            x3::with<custom::diagnostics_handler_tag>(diags) [
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
        Serial << "Parsing failed\n";
        ast.clear();
    }
}

//////////////////////////////////////////////////////////
static std::string const
    good_input = R"({ 23, "Amanda", "Stefanski", 1000.99 },
        { 35, "Angie", "Chilcote", 2000.99 }
    )", 
    bad_input = R"(
        { 23,
 'Amanda', "Stefanski", 1000.99 },
    )";
//////////////////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 10000));
  t2 = millis() - t1;
  Serial.print("Waited for ");
  Serial.print(t2);
  Serial.println(" millis");
  while (!delay_without_delaying(5000) ) { };
  Serial << "ArduinoRP2040BoostSpiritCustom ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit X3 operations" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Boost Spirit X3 Parser" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Employee structure example.\n";

  Serial << "/////////////////////////////////////////////////////////\n\n";
  Serial << "             An employee parser for Spirit...\n\n";
  Serial << "/////////////////////////////////////////////////////////\n\n";

  Serial << "With custom diagnostics only:" << endl;
  parse(good_input);

  Serial << "\n\n ----- Now with parse error:" << endl;
  parse(bad_input);

  Serial << "------------------------------" << endl;
  pinMode(LED_BUILTIN, OUTPUT);
}

//////////////////////////////////////////////////////////

int LEDstate = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if (delay_without_delaying(1000)) {
    LEDstate = !LEDstate;
    digitalWrite(LED_BUILTIN, LEDstate);
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    if (LEDstate) {
      Serial.println("Arduino blink ON");
    } else {
      Serial.println("Arduino blink OFF");
    }
  }

}
