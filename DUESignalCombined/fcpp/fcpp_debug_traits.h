// fcpp_debug_traits.h

// Traits for debugging of FC++.

#ifndef FCPP_DEBUG_TRAITS_DOT_H
#define FCPP_DEBUG_TRAITS_DOT_H

namespace fcpp {
  namespace traits {

    template <class F>
    class debug_traits
    {
    public:
       static const bool has_name = false;
       // Note that this requires the actual object.
       // Not normally called.
       static std::string name(const F& f) { return std::string("not named"); }
    };

  } // namespace traits
} // namespace fcpp

#endif
