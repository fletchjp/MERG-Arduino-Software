//////////////////////////////////////////////////////////////////////
// Arduino RP2040 Eraserface
//////////////////////////////////////////////////////////////////////

#include <memory>
#include <boost_callable_traits.hpp>
#include <eraserface.hpp>

#include <Streaming.h>

DEFINE_ERASERFACE(my_interface,
    ((a_func,       void(int) const))
    ((a_func,       void(long) const))
    ((another_func, int()))
    ((some_data,    const char*))
);

// two classes that implement my_interface
struct a_class {
    void a_func(int v) const {
        Serial << "a_class::void a_func(int v = " << v << ")" << endl;
    }

    void a_func(long v) const {
        Serial << "a_class::void a_func(long v = " << v << ")" << endl;
    }

    int another_func() {
        Serial << "a_class::another_func() = 3" << endl;
        return 3;
    }

    const char* some_data = "a_class's data";
};

struct another_class {

    int n;

    another_class() = default;
    
    another_class(int i) : n(i) {}

    // Notice a_func is implemented as a function template? No problem for eraserface.
    template<typename T>
    void a_func(T v) const {
        Serial <<
            "another_class::void a_func(T v = " << v << ")"
            "  [ T = " << typeid(T).name() << " ]" << endl;
    }


    int another_func() {
        Serial << "another_class::another_func() = " << n << endl;
        return n;
    }

    const char* some_data = "another_class's data";
};

// Both classes above can be assigned to the interface variable and their
// member functions can be called through it.

void print_data(my_interface obj) {
    Serial << obj.some_data() << endl;
}


void setup() {
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
