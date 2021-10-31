// ArduinoDUEBoostMetaparseRational
// This now compiles using boost metaparse from ArduinoForBoost.
// I have removed <iostream> and replaced std::cout with Serial unconditionally.
// I also have to undef a definition of min.

#include <Streaming.h>

#undef F
#undef min
//#include <cstdio>
#include <string>
//#include <algorithm>
#include <boost_type_index.hpp>
#include <boost_metaparse.hpp>

// This will need to be implemented somehow.
template <class Num, class Denom>
struct rational {
  Num num;
  Denom denom;
  rational() {}
  rational(Num n, Denom d) : num(n), denom(d) { }
};

using namespace boost::metaparse;

typedef
  sequence_apply2<
    rational,

    token<int_>,
    last_of<lit_c<'/'>, token<int_>>
  >
  rational_grammar;

typedef build_parser<entire_input<rational_grammar>> rational_parser;

#define RATIONAL(s) \
  (rational_parser::apply<BOOST_METAPARSE_STRING(s)>::type())

using exp_parser1 = build_parser<int_>;

void setup() {
  Serial.begin (115200);
  Serial << endl << endl << "DUEBoostMetaparseRational ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Metaparse operations" << endl;
  
  // put your setup code here, to run once:
  auto one_third = RATIONAL("1/3");
  //std::string s = "1/3";
  auto something = boost::typeindex::type_id_with_cvr<BOOST_METAPARSE_STRING("1/3")>();
  // This needs a function in std::string which is not in the headers.
  // I am downloading the full source of Boost 1.66.0 to get it.
  //auto name = something.pretty_name();
  auto another = rational_parser::apply<BOOST_METAPARSE_STRING("1/3")>::type();
  auto what = exp_parser1::apply<boost::metaparse::string<'1'>>::type();
  
}
void loop() {
  // put your main code here, to run repeatedly:

}
