/// @file custom.hpp
/// @brief custom diagnostics handler
///
/// Copied from example Custom3

#ifndef CUSTOM_HPP
#define CUSTOM_HPP

/// custom namespace for the diagnostics handler
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



#endif
