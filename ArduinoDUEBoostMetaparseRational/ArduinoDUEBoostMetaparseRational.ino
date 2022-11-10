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
#include <boost_rational.hpp>
#include <boost_config.hpp>

// This will need to be implemented somehow.
// The docs did not bother to explain to use boost rational!!!
template <class Num, class Denom>
struct rational {
//  Num num;
//  Denom denom;
  typedef std::tuple<Num,Denom> tuple_type;
  typedef Num type1;
  typedef Denom type2;
  rational() {}
//  rational(Num n, Denom d) : num(n), denom(d) { }
  typedef rational type;

  // This explains why the example did not compile!!!

  static boost::rational<int> run()
  {
    return boost::rational<int>(Num::type::value, Denom::type::value);
  }


};

// Idea for template type extraction - see https://stackoverflow.com/questions/64795375/extracting-templates-from-templated-class
template <typename T>
struct extract_first {};
template <typename T>
struct extract_second {};

template <template <typename...> class ClassTemplate, typename... ClassTemplateParams>
struct extract_first<ClassTemplate<ClassTemplateParams...> > {
    using type = typename std::tuple_element<0, std::tuple<ClassTemplateParams...> >::type;
};
template <template <typename...> class ClassTemplate, typename... ClassTemplateParams>
struct extract_second<ClassTemplate<ClassTemplateParams...> > {
    using type = typename std::tuple_element<1, std::tuple<ClassTemplateParams...> >::type;
};


template <typename T1, typename T2>
struct Example { };

using namespace boost::metaparse;

typedef
  sequence_apply2<
    rational,

    token<int_>,
    one_of<
      last_of<lit_c<'/'>, token<int_> >,
      empty<boost::mpl::int_<1> >
    >
  >
  rational_grammar;

typedef build_parser<entire_input<rational_grammar>> rational_parser;

#define RATIONAL(s) \
  (rational_parser::apply<BOOST_METAPARSE_STRING(s)>::type::run())
#define RATIONAL_TYPE(s) \
  (rational_parser::apply<BOOST_METAPARSE_STRING(s)>::type)

using exp_parser1 = build_parser<int_>;

using boost::mpl::apply_wrap1;
using boost::mpl::_1;
using boost::mpl::_2;

void setup() {
  Serial.begin (115200);
  Serial << endl << endl << "DUEBoostMetaparseRational ** " << endl << __FILE__ << endl;
  Serial << "Some simple Boost Metaparse operations" << endl;
#if BOOST_METAPARSE_STD < 2011
  Serial << "Metaparse problem" << endl;
#else
  // I have corrected this to be more like the example.
  // However, this does not run because of a problem with boost::rational not having all the functions available. 
  //const boost::rational<int> r1 = RATIONAL("1/3");
  // This now works without the "run" to convert to boost::rational
  //auto r1 = RATIONAL("1/3");
  //extract_first<RATIONAL_TYPE("1/3")>::type::value;
  // I would like to extract the two types from r1 to get to the values.
  // I have not figured out how to do this.
  //auto r1_1 = typeid(r1)::type1::value;
  //Serial << r1.name() << endl;
  //boost::rational<int> x = RATIONAL("1/3");
#endif  
typedef Example<int,double> E1;
typedef extract_first<E1> type1;  
typedef extract_second<E1> type2;  
  // put your setup code here, to run once:
  //auto one_third = RATIONAL("1/3");
  //std::string s = "1/3";
  auto something = boost::typeindex::type_id_with_cvr<BOOST_METAPARSE_STRING("1/3")>();
  // This needs a function in std::string which is not in the headers.
  // I am downloading the full source of Boost 1.66.0 to get it.
  // No that is the wrong location - the std library must be too old.
  //auto name = something.pretty_name();
  auto name = something.raw_name();
  Serial << name << endl;
  //auto another = rational_parser::apply<BOOST_METAPARSE_STRING("1/3")>::type();
  //Serial << another << endl;
  auto what = exp_parser1::apply<boost::metaparse::string<'1'>>::type();
  // apply_wrap1 is boost mpl which explains why it is not in the metaparse manual.
  auto what2 = apply_wrap1<exp_parser1,boost::metaparse::string<'1'>>::type::value;
  Serial << "result of applywrap1 " << what2 << endl;
}
void loop() {
  // put your main code here, to run repeatedly:

}