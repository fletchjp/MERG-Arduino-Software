/// @file without_any_parser.cpp
/// @brief for comparison with any_parser.cpp


#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <iomanip>

namespace x3 = boost::spirit::x3;

namespace {
    template <
         typename It = std::string::const_iterator,
         typename Attr = x3::unused_type >
    struct lazy_rule : x3::parser<lazy_rule<It, Attr>> {
        using base_type = x3::parser<lazy_rule<It, Attr>>;
        using attribute_type = Attr;

        lazy_rule() = default;

        template <typename P> lazy_rule(P p) {
            f = [p = x3::as_parser(std::move(p))](It& f, It l, Attr& attr) {
                return p.parse(f, l, x3::unused, x3::unused, attr);
            };
        }

        template<typename It_, typename Ctx, typename Attr_>
        bool parse(It_& first, It_ last, Ctx& ctx, x3::unused_type, Attr_& attr) const {
            x3::skip_over(first, last, ctx);
            return f
                ? f(first, last, attr)
                : false;
        }
      private:
        std::function<bool(It&, It, Attr&)> f;
    };

    template <typename T>
    struct as_type {
        template <typename...> struct Tag{};

        template <typename P>
        auto operator[](P p) const {
            return x3::rule<Tag<T, P>, T> {"as"} = x3::as_parser(p);
        }
    };

    template <typename Tag>
    struct set_lazy_type {
        template <typename P>
        auto operator[](P p) const {
            auto action = [](auto& ctx) {
                x3::get<Tag>(ctx) = x3::_attr(ctx);
            };
            return x3::omit [ p [ action ] ];
        }
    };

    template <typename Tag>
    struct do_lazy_type : x3::parser<do_lazy_type<Tag>> {
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

    template <typename T> static const as_type<T> as{};
    template <typename T> static const set_lazy_type<T> set_lazy{};
    template <typename T> static const do_lazy_type<T> do_lazy{};
}

int main() {
    std::cout << std::boolalpha << std::left;

    using Value = boost::variant<int, bool, double, std::string>;
    using It    = std::string::const_iterator;
    using Rule  = lazy_rule<It, Value>;

    x3::symbols<Rule> options;

    auto const parser = x3::with<Rule>(Rule{}) [
        set_lazy<Rule>[options] >> ':' >> do_lazy<Rule>
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
