// traits.h

// New file to provide a model for traits.

// As a temporary measure take this from operator.h 
// and replace it there with a call to this file.

// Use of Loki to provide traits for arguments.
// This has been made conditional.
//#ifdef FCPP_USE_LOKI_TRAITS
//#include "loki/include/loki/TypeTraits.h"
//#endif
#ifdef FCPP_USE_BOOST_TRAITS
#include "boost/call_traits.hpp"
#endif

#ifndef FCPP_TRAITS_DOT_H
#define FCPP_TRAITS_DOT_H

// Provide here another implementation of the reference
// trait to free from either dependency.

namespace fcpp {
  namespace traits {

  } // namespace traits
} // namespace fcpp


#endif
