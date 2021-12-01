/// @file another_error_handler.cpp
/// @brief collecting some further ideas.

/// There are a lot of different ideas and it is not clear to me how they all fit together.
/// I am going to try out some ideas.

/// https://stackoverflow.com/questions/57048008/spirit-x3-is-this-error-handling-approach-useful/57067207#57067207

/// We might not be content reporting just the one expectation and bailing out.
/// Indeed, you can report and continue parsing as there were just a regular mismatch: 
/// this is where on_error comes in.

/// Let's create a tag base:

struct with_error_handling {
    template<typename It, typename Ctx>
        x3::error_handler_result on_error(It f, It l, expectation_failure<It> const& ef, Ctx const&) const {
            std::string s(f,l);
            auto pos = std::distance(f, ef.where());

            std::cout << "Expecting " << ef.which() << " at "
                << "\n\t" << s
                << "\n\t" << std::setw(pos) << std::setfill('-') << "" << "^\n";

            return error_handler_result::fail;
        }
};

/// Now, all we have to do is derive our rule ID from with_error_handlingand BAM!,
/// we don't have to write any exception handlers, 
/// rules will simply "fail" with the appropriate diagnostics. 
/// What's more, some inputs can lead to multiple (hopefully helpful) diagnostics:

auto const eh = [](auto p) {
    struct _ : with_error_handling {};
    return rule<_> {} = p;
};

const auto quoted_string = eh(lexeme['"' > *(print - '"') > '"']);
const auto bare_string   = eh(lexeme[alpha > *alnum] > ';');
const auto two_ints      = eh(int_ > int_);

const auto main          = quoted_string | bare_string | two_ints;
using main_type = std::remove_cv_t<decltype(main)>;

const auto entry_point   = skip(space)[ eh(expect[main] > eoi) ];

///Now, main becomes just:

for (std::string const input : { 
        "   -89 0038  ",
        "   \"-89 0038\"  ",
        "   something123123      ;",
        // undecidable
        "",
        // violate expecations, no successful parse
        "   -89 oops  ",   // not an integer
        "   \"-89 0038  ", // missing "
        "   bareword ",    // missing ;
        // trailing debris, successful "main"
        "   -89 3.14  ",   // followed by .14
    })
{
    std::cout << "====== " << std::quoted(input) << "\n";

    It iter = input.begin(), end = input.end();
    if (parse(iter, end, square::peg::entry_point)) {
        std::cout << "Parsed successfully\n";
    } else {
        std::cout << "Parsing failed\n";
    }
}

/// Attribute Propagation, on_success

/// Parsers aren't very useful when they don't actually parse anything, 
/// so let's add some constructive value handling, also showcaseing on_success:

/// Defining some AST types to receive the attributes:

struct quoted : std::string {};
struct bare   : std::string {};
using  two_i  = std::pair<int, int>;
using Value = boost::variant<quoted, bare, two_i>;

/// Make sure we can print Values:

static inline std::ostream& operator<<(std::ostream& os, Value const& v) {
    struct {
        std::ostream& _os;
        void operator()(quoted const& v) const { _os << "quoted(" << std::quoted(v) << ")";             } 
        void operator()(bare const& v) const   { _os << "bare(" << v << ")";                            } 
        void operator()(two_i const& v) const  { _os << "two_i(" << v.first << ", " << v.second << ")"; } 
    } vis{os};

    boost::apply_visitor(vis, v);
    return os;
}

/// Now, use the old as<> trick to coerce attribute types, this time with error-handling:

/// As icing on the cake, let's demonstrate on_success in with_error_handling:
