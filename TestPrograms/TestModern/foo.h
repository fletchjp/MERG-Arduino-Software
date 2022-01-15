/// @file foo.h
/// @brief class Foo is defined here
///
/// Example of default initialization
///

#ifndef FOO_H
#define FOO_H

namespace foo {

  class Foo {
  public:
    Foo() /* : fooString(nullptr)*/ { }
    // comment out unused parameter names.
    Foo(const char* /*str*/) /* : fooString(nullptr) */ {  }
    Foo(const Foo& /*other*/) /* : fooString(nullptr) */ { }
  private:
    char* fooString = nullptr; // replaces all the initialisations
  };

}

#endif
