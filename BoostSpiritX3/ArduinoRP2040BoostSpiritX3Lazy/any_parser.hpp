/// @file any_parser.hpp
/// @brief Arduino RP2040 code for Boost Spirit X3 any_parser example
///
/// This is based on an example: 
///
/// https://stackoverflow.com/questions/60171119/boost-spirit-x3-lazy-parser
///

#ifndef ANY_PARSER_HPP
#define ANY_PARSER_HPP

#include <boost_spirit_home_x3.hpp>

namespace x3 = boost::spirit::x3;

/// namespace named any_parser
namespace any_parser_or_something {
    template <typename T>
    struct as_type {
        template <typename...> struct Tag{};

        template <typename P>
        auto operator[](P p) const {
            return x3::rule<Tag<T, P>, T> {"as"} = x3::as_parser(p);
        }
    };

    template <typename Tag>
    struct set_context_type {
        template <typename P>
        auto operator[](P p) const {
            auto action = [](auto& ctx) {
                x3::get<Tag>(ctx) = x3::_attr(ctx);
            };
            return x3::omit [ p [ action ] ];
        }
    };

    template <typename Tag>
    struct lazy_type : x3::parser<lazy_type<Tag>> {
        using attribute_type = typename Tag::attribute_type; // TODO FIXME?

        template<typename It, typename Ctx, typename RCtx, typename Attr>
        bool parse(It& first, It last, Ctx& ctx, RCtx& rctx, Attr& attr) const {
            auto& subject = x3::get<Tag>(ctx);

            return x3::as_parser(subject)
                .parse(
                    first, last, 
                    std::forward<Ctx>(ctx),
                    std::forward<RCtx>(rctx),
                    attr.value);
        }
    };

    template <typename T> static const as_type<T>          as{};
    template <typename T> static const set_context_type<T> set_context{};
    template <typename T> static const lazy_type<T>        lazy{};
}

typedef x3::variant<int, bool, double> Value;
/// Value_struct putting a value into a struct
struct Value_struct {
  Value value;
};
BOOST_FUSION_ADAPT_STRUCT(Value_struct,value)

/// Stream output for a variant type provided operators exist for all the alternatives.
inline Print &operator <<(Print &stream, const Value_struct &arg)
{
   std::stringstream s;
   s << arg.value.get() << std::ends;
   stream.print(s.str().c_str());
   return stream;
}
/// somehow the parse here messes with the definition of Variant_struct.
/// The solution is to use the correct iterator_type.
bool basic_test()
    { // basic tests
        using x3::lit;
        auto a = lit('a');
        auto b = lit('b');
        auto c = lit('c');
        auto d = lit('d');

        {
            //typedef std::string::const_iterator iterator_type;
            typedef const char* iterator_type;
            x3::any_parser<iterator_type> p =
                *(a | b | c );
            char const* in("abcabcacdb");
            Serial << "test input is " << in << endl;
            char const* last = in;
            while (*last) last++;
            return x3::parse(in, last, p);   
            //BOOST_TEST(test("abcabcacb", start));
        }
        return false;
    }


void run_lazy_example()
{
    //namespace x3 = boost::spirit::x3;

    using namespace any_parser_or_something;

    if (basic_test() ) Serial << "any_parser basic_test() passes" << endl;
    else Serial << "any_parser basic_test() fails" << endl;
    
    //using Value = boost::variant<int, bool, double, std::string>;
    //using Value = boost::variant<int, bool, double>;
    using It    = std::string::const_iterator;
    using Rule  = x3::any_parser<It, Value>;

    x3::symbols<Rule> options;

    auto const parser = x3::with<Rule>(Rule{}) [
        set_context<Rule>[options] >> ':' >> lazy<Rule>
    ];



   auto run_tests = [=] {
        for (std::string const& input_ : {
                "integer_value: 42",
                //"quoted_string: \"hello world\"",
                "bool_value: true",
                "double_value: 3.1415926",
            })
        {

            Value_struct attr;
            It start_ = begin(input_);       
            It end_   = end(input_);
            //std::cout << std::setw(36) << std::quoted(input);
            // experiment
            //if (any_parser::lazy_type<Rule>::parse(begin(input_), end(input_), parser, x3::space, attr)) {
            // I cannot get this to compile - it produces a very long error.
            // I cannot see where phrase_parse is defined.
           if (x3::phrase_parse(start_, end_, parser, x3::space, attr)) {
                Serial << " -> success (" << attr << ")\n";
            } else {
                Serial << " -> failed " << attr.value.get().which() << "\n";
            }
  //error: no type named 'type' in 'struct boost::fusion::traits::is_view<boost::variant<int, bool, double> >'

        }

    
    };

    Serial << "Supporting only integer_value and quoted_string:\n";
    options.add("integer_value", x3::int_);
    options.add("double_value", x3::double_);
/*  This does not compile.
    options.add("quoted_string", as<std::string> 
    [
            // lexeme is actually redundant because we don't use surrounding skipper yet
            x3::lexeme [ '"' >> *('\\' >> x3::char_ | ~x3::char_('"')) >> '"' ]
        ]);
*/
run_tests();

    Serial << "\nAdded support for double_value and bool_value:\n";
    //options.add("double_value", x3::double_);
    options.add("bool_value", x3::bool_);

    run_tests();

  }
#endif
