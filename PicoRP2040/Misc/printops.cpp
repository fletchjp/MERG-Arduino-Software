
#include "printops.h"

namespace fcpp {

template <class T>
Print &operator << ( Print &obj, const List<T> &arg)
{
    Serial << "[ ";
    for (auto i = arg.begin(); i != arg.end(); ++i)
    {
        Serial << *i << " " << endl;
    }
    Serial << "]";
    return obj; 
}

}
