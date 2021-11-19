/// @file ArduinoRP2040BoostSpiritX3Comments.ino
/// @brief Boost Spirit X3 comments example with annotation
///
/// Taken from https://stackoverflow.com/questions/65614720/boost-spirit-x3-tokenizer-with-annotation-does-not-work

// 3rd party libraries
#include <Streaming.h>

#undef F
#undef min


#include <exception>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <variant>

#include "ArduinoCode.h"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
namespace x3 = boost::spirit::x3;

struct SingleLineComment{};
struct Whitespace       {};

using Variant = std::variant<SingleLineComment, Whitespace>;

struct Token : Variant, x3::position_tagged {
    using Variant::Variant;
};

namespace {
    struct position_cache_tag;
    namespace Parser {
        struct annotate_position {
            template <typename T, typename Iterator, typename Context>
                inline void on_success(Iterator first, Iterator last, T &ast, Context const &context) const {
                    auto &position_cache = x3::get<position_cache_tag>(context);
                    position_cache.annotate(ast, first, last);
                }
        };
               // unique on success hook classes
        template <typename> struct Hook {}; // no annotate_position mix-in
        template <> struct Hook<Token> : annotate_position   {};

        template <typename T>
        static auto constexpr as = [](auto p, char const* name = typeid(decltype(p)).name()) {
            return x3::rule<Hook<T>, T> {name} = p;
        };

        // rule definitions
        auto singleLineComment = as<SingleLineComment>("//" >> x3::omit[*(x3::char_ - x3::eol)]);
        auto whitespace        = as<Whitespace>       (x3::omit[+x3::ascii::space]);
        auto token             = as<Token>            (singleLineComment | whitespace, "token");
    }
}



void setup() {
  // put your setup code here, to run once:
    using It             = std::string::const_iterator;
    using position_cache = x3::position_cache<std::vector<It>>;

    std::string const content = R"(// first single line comment

// second single line comment

    )";
    position_cache positions{content.begin(), content.end()};

    auto parser = x3::with<position_cache_tag>(positions)[*Parser::token];

    std::vector<Token> tokens;
    if (parse(begin(content), end(content), parser >> x3::eoi, tokens)) {
        std::cout << "Found " << tokens.size() << " tokens" << std::endl;

        for (auto& token : tokens) {
            auto pos = positions.position_of(token);
            std::cout
                << (token.index() ? "space" : "comment") << "\t"
                << std::quoted(std::string_view(&*pos.begin(), pos.size()))
                << std::endl;
        }
    }
}
}

void loop() {
  // put your main code here, to run repeatedly:

}
