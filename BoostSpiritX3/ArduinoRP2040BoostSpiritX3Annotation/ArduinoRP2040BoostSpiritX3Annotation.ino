// ArduinoRP2040BoostSpiritX3Annotation

// Taken from spirit/example/x3/annotation from Boost 1.77.0

/*
 * https://ostack.cn/?qa=302784/
That warning is telling you that there was a subtle ABI change
(actually a conformance fix) between 6 and 7.1,
such that libraries built with 6.x or earlier may not work properly
when called from code built with 7.x (and vice-versa).
As long as all your C++ code is built with GCC 7.1 or later, you can safely ignore this warning.
To disable it, pass -Wno-psabi to the compiler
 */

// 3rd party libraries
#include <Streaming.h>

#undef F
#undef min

#include <exception>
#include <stdexcept>
#include <cstdio>

#include "ArduinoCode.h"


#include <boost_config_warning_disable.hpp>
#include <boost_spirit_home_x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
//#include <boost/fusion/include/io.hpp>
//#include <boost/fusion/sequence.hpp>
//#include <boost/fusion/include/sequence.hpp>


namespace client { namespace ast
{

    ///////////////////////////////////////////////////////////////////////////
    //  Our AST (employee and person structs)
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct person : x3::position_tagged
    {
        person(
            std::string const& first_name = ""
          , std::string const& last_name = ""
        )
          : first_name(first_name)
          , last_name(last_name)
        {}

        std::string first_name, last_name;
    };

    struct employee : x3::position_tagged
    {
        int age;
        person who;
        double salary;
    };

    //using boost::fusion::operator<<;
    // I cannot use the fusion IO so I am instead doing this which works.
    inline Print &operator <<(Print &stream, const employee &emp)
    {
       stream.print("[");
       stream.print(emp.age);
       stream.print(",(");
       stream.print(emp.who.first_name.c_str());
       stream.print(",");
       stream.print(emp.who.last_name.c_str());
       stream.print("),");
       stream.print(emp.salary);       
       stream.print("]");
      return stream;
    }

}}

// We need to tell fusion about our employee struct
// to make it a first-class fusion citizen. This has to
// be in global scope.

BOOST_FUSION_ADAPT_STRUCT(client::ast::person,
    first_name, last_name
)

BOOST_FUSION_ADAPT_STRUCT(client::ast::employee,
    age, who, salary
)

namespace client
{
    namespace parser
    {
        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;

        ///////////////////////////////////////////////////////////////////////
        //  Our annotation handler
        ///////////////////////////////////////////////////////////////////////

        // tag used to get the position cache from the context
        struct position_cache_tag;

        struct annotate_position
        {
            template <typename T, typename Iterator, typename Context>
            inline void on_success(Iterator const& first, Iterator const& last
            , T& ast, Context const& context)
            {
                auto& position_cache = x3::get<position_cache_tag>(context).get();
                position_cache.annotate(ast, first, last);
            }
        };

    ///////////////////////////////////////////////////////////////////////////////
    //  Our employee parser
    ///////////////////////////////////////////////////////////////////////////////

        using x3::int_;
        //using x3::lit;
        using x3::double_;
        using x3::lexeme;
        using ascii::char_;

        struct quoted_string_class;
        struct person_class;
        struct employee_class;

        x3::rule<quoted_string_class, std::string> const quoted_string = "quoted_string";
        x3::rule<person_class, ast::person> const person = "person";
        x3::rule<employee_class, ast::employee> const employee = "employee";

        auto const quoted_string_def = lexeme['"' >> +(char_ - '"') >> '"'];
        auto const person_def = quoted_string >> ',' >> quoted_string;

        auto const employee_def =
               '{'
            >>  int_ >> ','
            >>  person >> ','
            >>  double_
            >>  '}'
            ;

        auto const employees = employee >> *(',' >> employee);

        BOOST_SPIRIT_DEFINE(quoted_string, person, employee);
 
        struct quoted_string_class {};
        struct person_class : annotate_position {};
        struct employee_class : annotate_position {};     
     }
}

///////////////////////////////////////////////////////////////////////////////
// Our main parse entry point
///////////////////////////////////////////////////////////////////////////////

using iterator_type = std::string::const_iterator;
using position_cache = boost::spirit::x3::position_cache<std::vector<iterator_type>>;

std::vector<client::ast::employee>
parse(std::string const& input, position_cache& positions)
{
    using boost::spirit::x3::ascii::space;

    std::vector<client::ast::employee> ast;
    iterator_type iter = input.begin();
    iterator_type const end = input.end();

    using boost::spirit::x3::with;

    // Our parser
    using client::parser::employees;
    using client::parser::position_cache_tag;

    auto const parser =
        // we pass our position_cache to the parser so we can access
        // it later in our on_sucess handlers
        with<position_cache_tag>(std::ref(positions))
        [
            employees
        ];

    bool r = phrase_parse(iter, end, parser, space, ast);

    if (r && iter == end)
    {
        //std::cout << boost::fusion::tuple_open('[');
        //std::cout << boost::fusion::tuple_close(']');
        //std::cout << boost::fusion::tuple_delimiter(", ");

        Serial << "-------------------------\n";
        Serial << "Parsing succeeded\n";

        for (auto const& emp : ast)
        {
            //Serial << "got a result " << endl;
            Serial << "got: " << emp << endl;
        }
        Serial << "\n-------------------------\n";

    }
    else
    {
        Serial << "-------------------------\n";
        Serial << "Parsing failed\n";
        Serial << "-------------------------\n";
        ast.clear();
    }
    return ast;
}

// Sample input:

std::string input = R"(
{
    23,
    "Amanda",
    "Stefanski",
    1000.99
},
{
    35,
    "Angie",
    "Chilcote",
    2000.99
},
{
    43,
    "Dannie",
    "Dillinger",
    3000.99
},
{
    22,
    "Dorene",
    "Dole",
    2500.99
},

{
    38,
    "Rossana",
    "Rafferty",
    5000.99
}
)";

//////////////////////////////////////////////////////////

// This comes from the cdc_multi example
// Helper: non-blocking "delay" alternative.
boolean delay_without_delaying(unsigned long time) {
  // return false if we're still "delaying", true if time ms has passed.
  // this should look a lot like "blink without delay"
  static unsigned long previousmillis = 0;
  unsigned long currentmillis = millis();
  if (currentmillis - previousmillis >= time) {
    previousmillis = currentmillis;
    return true;
  }
  return false;
}
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
  while (!delay_without_delaying(20000) ) { };
  Serial << "ArduinoRP2040BoostSpiritNumList1 ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Spirit X3 operations" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Boost Spirit X3 Parser" << endl;
  Serial << "------------------------------" << endl;
  Serial << "Annotated Employee structure example.\n";

  position_cache positions{input.begin(), input.end()};
  auto ast = parse(input, positions);

  // Get the source of the 2nd employee and print it
  auto pos = positions.position_of(ast[1]); // zero based of course!
  Serial << "Here's the 2nd employee:" << endl;
  //auto res = (pos.begin(), pos.end());
  //auto res1 = pos.begin();
  // Direct access to the vector which works and the pos version does not
  Serial << ast[1] << endl;
  //Serial << std::string(pos.begin(), pos.end()) << endl;

  //Serial << "/////////////////////////////////////////////////////////\n\n";
  //Serial << "             An employee parser for Spirit...\n\n";
  //Serial << "/////////////////////////////////////////////////////////\n\n";
  
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
