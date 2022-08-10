
#define FCPP152
#define FCPP_ENABLE_LAMBDA
#define FCPP_EXTRA_STRONG_CURRY
#define FCPP_PATTERN // define this to include pattern.h
//#define FCPP_ARDUINO // Take out arguement traits - defined in prelude for Arduino etc
// All headers modified to work in the Arduino/Pico environment.
// Some exceptions guarding in pattern.h have had to be removed.
#define FCPP_UNIFY // Unification of Monads and Functors in functors.h
#include "fcpp_prelude.h"


#include <Streaming.h>

using namespace fcpp;

namespace fcpp {

template <class T>
Print &operator << ( Print &obj, const List<T> &arg);
/*{
    Serial << "[ ";
    for (auto i = arg.begin(); i != arg.end(); ++i)
    {
        Serial << *i << " " << endl;
    }
    Serial << "]";
    return obj; 
}
*/
}
