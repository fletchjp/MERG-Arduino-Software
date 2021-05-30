// promote.h

// New file to provide a model for the promotion.


#ifndef FCPP_PROMOTE_DOT_H
#define FCPP_PROMOTE_DOT_H

/* This is the code from 
 * "C++ Templates - The Complete Guide"
 * by David Vandevoorde and Nicolai M. Josuttis, Addison-Wesley, 2002
 */

#include "ifthenelse.hpp"  // Defines IfThenElse

#include "promote1.hpp" // Defines Promotion.
#include "promote2.hpp" // Specialised for (T,T) case.
#include "promote3.hpp" // Defines macro MK_PROMOTION
#include "promote4.hpp" // Provides some promotions on bool.

// Header to do Promotion traits with 3 4 or 5 types. 
// Extends the published work of David Vandevoorde and Nicolai M. Josuttis
#include "promote31.hpp"
// provides Promotion3, Promotion4 and Promotion 5.

// The header other header files need to be available 
// at compilation.

// J.P.Fletcher  13/2/2007

#endif
