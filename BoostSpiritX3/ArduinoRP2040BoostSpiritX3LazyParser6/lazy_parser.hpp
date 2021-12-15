/// @file lazy_parser.hpp
/// @brief Arduino RP2040 code for Boost Spirit X3 lazy_parser code
///
/// Lazy example modified to NOT use the any_parser code. It defines lazy_rule instead.
///
/// This is based on an example: 
///
/// https://stackoverflow.com/questions/60171119/boost-spirit-x3-lazy-parser
///
/// I have modified the example to wrap the variant data type in a struct.
///
/// This is the code for the lazy parser independent of a particular problem.
///
/// See also https://stackoverflow.com/questions/67510759/boost-spirit-x3-parameterizing-parsers-with-other-parsers
///
/// https://stackoverflow.com/questions/66036568/mixing-non-terminal-rules-from-separeted-translation-unit
///
/// The reason is that, without BOOST_SPIRIT_DEFINE, 
/// the definition for a rule is stored in the context
/// and this is what causes the explosion in compile-times.
///
/// I have seen mention of any_rule which is not in the code for 3.0.9




#ifndef LAZY_PARSER_HPP
#define LAZY_PARSER_HPP
#define EXTRAS
#include <boost_spirit_home_x3.hpp>

namespace x3 = boost::spirit::x3;

/// lazy_parser namespace for lazy_rule
namespace lazy_parser {

/// @brief lazy_rule to set up the parser
///
/// Comparison with any_parser which has an extra default argument.
/// Some put in using EXTRAS
    template <
         typename It = std::string::const_iterator,
         typename Attr = x3::unused_type
#ifdef EXTRAS
       , typename Context = x3::subcontext<>
#endif
      >
    struct lazy_rule : x3::parser<lazy_rule<It, Attr
#ifdef EXTRAS
    , Context
#endif
    >> {
#ifdef EXTRAS
        using base_type = x3::parser<lazy_rule<It, Attr, Context>>;
#else
        using base_type = x3::parser<lazy_rule<It, Attr>>;
#endif
        using attribute_type = Attr;

        lazy_rule() = default;
        
///#ifdef EXTRAS
/// any_parser has this which does not seem to be used anywhere.
///        static bool const handles_container =
///            traits::is_container<Attr>::value;
///#endif

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

/// as_type from the original example, not currently used.
    template <typename T>
    struct as_type {
        template <typename...> struct Tag{};

        template <typename P>
        auto operator[](P p) const {
            return x3::rule<Tag<T, P>, T> {"as"} = x3::as_parser(p);
        }
    };



/// set_lazy_type sets up the configuration.
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

/// do_lazy_type assuming that attr is a struct wrapping a variant value.  
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

/// quoted string made into a rule.
    struct quoted_string_class;
    x3::rule<quoted_string_class, std::string>        const quoted_string = "quoted_string";
    auto const quoted_string_def = x3::lexeme['"' >> +(x3::char_ - '"') >> '"'];
    BOOST_SPIRIT_DEFINE(quoted_string);
      
    template <typename T> static const as_type<T>          as{};
    template <typename T> static const set_lazy_type<T>    set_lazy{};
    template <typename T> static const do_lazy_type<T>     do_lazy{};
}



#endif
