/// @file lazy_parser.cpp
/// @brief example of any_parser in action
///
/// https://stackoverflow.com/questions/60171119/boost-spirit-x3-lazy-parser
///
/// any_parser uses subcontext

#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <iomanip>

namespace x3 = boost::spirit::x3;

namespace {
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
                    attr);
        }
    };

    template <typename T> static const as_type<T>          as{};
    template <typename T> static const set_context_type<T> set_context{};
    template <typename T> static const lazy_type<T>        lazy{};
}

int main() {
    std::cout << std::boolalpha << std::left;

    using Value = boost::variant<int, bool, double, std::string>;
    using It    = std::string::const_iterator;
    using Rule  = x3::any_parser<It, Value>;

    x3::symbols<Rule> options;

    auto const parser = x3::with<Rule>(Rule{}) [
        set_context<Rule>[options] >> ':' >> lazy<Rule>
    ];

    auto run_tests = [=] {
        for (std::string const& input : {
                "integer_value: 42",
                "quoted_string: \"hello world\"",
                "bool_value: true",
                "double_value: 3.1415926",
            })
        {
            Value attr;
            std::cout << std::setw(36) << std::quoted(input);
            if (phrase_parse(begin(input), end(input), parser, x3::space, attr)) {
                std::cout << " -> success (" << attr << ")\n";
            } else {
                std::cout << " -> failed\n";
            }
        }
    };


    std::cout << "Supporting only integer_value and quoted_string:\n";
    options.add("integer_value", x3::int_);
    options.add("quoted_string", as<std::string> [
            // lexeme is actually redundant because we don't use surrounding skipper yet
            x3::lexeme [ '"' >> *('\\' >> x3::char_ | ~x3::char_('"')) >> '"' ]
        ]);
    run_tests();

    std::cout << "\nAdded support for double_value and bool_value:\n";
    options.add("double_value", x3::double_);
    options.add("bool_value", x3::bool_);

    run_tests();
}
