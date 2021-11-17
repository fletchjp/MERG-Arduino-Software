/// @file variantCode.h
/// @brief Arduino RP2040 code for Boost Spirit X3 Variant
///
///
/// It provides adaption for Boost 1.66.0 code to work with the Arduino RP2040.


#ifndef VARIANT_CODE_H
#define VARIANT_CODE_H

namespace x3 = boost::spirit::x3;

struct none {};
/*
inline std::strstream &operator <<(std::strstream stream, const none)
{
    stream << std::string("none") << std::ends;
    return stream;
}
*/
inline Print &operator <<(Print &stream, const none)
{
  stream.print("none");
  return stream;
}


using variant = x3::variant<
      //  none,
        bool
      , std::string
      , int
      , double
    >;

struct ast;

struct ast : variant
{
    using variant::variant;
    using variant::operator=;

    ast(char const* s)
      : variant(std::string{s})
    {}

    ast& operator=(char const* s)
    {
        variant::operator=(std::string{s});
        return *this;
    }
    //typedef ast ast_type;


/*
// Stream output for a variant type.
   inline Print &operator <<(Print &stream)
   {
      std::strstream s;
      s << (*this).get() << std::ends;
      stream.print(s.str());
      return stream;
   }
*/
 //   friend Print &operator <<(Print &, const ast &);
};


//using ast::operator<<;

//typedef ast ast_type;


// Stream output for a variant type.
inline Print &operator <<(Print &stream, const ast &arg)
{
   std::strstream s;
   s << arg.get() << std::ends;
   stream.print(s.str());
   return stream;
}



#endif
