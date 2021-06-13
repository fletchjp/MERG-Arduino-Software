// structural

#include<ArduinoSTL.h>

#include <boost_1_51_0.h>
#define FCPP_ARDUINO_BOOST
#include <iostream>
// Need 5 parameter functoids for example.
#define FCPP152
#define FCPP_ENABLE_LAMBDA
#define FCPP_USE_PROMOTE
#define FCPP_DEBUG
#define FCPP_EXTRA_STRONG_CURRY
#define FCPP_PATTERN // define this to include pattern.h
#define FCPP_UNIFY
#include "prelude.h"
#include "functors.h"
//#include "comonad.h"

// Space here for new functoids!!
namespace fcpp {

// I am making this to help make IdentityM into an inferrable monad.
template <class T>
class BootStrap {
  OddList<T> rep;
 public:
 typedef T ElementType;
  BootStrap( AUniqueTypeForNothing ) {}
  BootStrap() { }
  BootStrap(const T& t) : rep(cons(t,NIL) ) {}
  T value() const { return head(rep); }
  bool is_error() const { return null(rep); }
  operator T() const { return head(rep); } // needed for transparency.
};


namespace impl {
   struct XBoot {
      template <class T> struct Sig : public FunType<T,BootStrap<T> > {};
   
      template <class T>
      typename Sig<T>::ResultType
      operator()( const T& x ) const {
         return BootStrap<T>( x );
      }
   };
}
typedef Full1<impl::XBoot> Boot;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Boot boot;
FCPP_MAYBE_NAMESPACE_CLOSE

  namespace impl {

    struct XAbstract {
#ifdef FCPP_DEBUG
        std::string name() const
        {
           return std::string("Abstract");
        }
#endif
      template <class T> struct Sig : public FunType<T,List<T> > {};

        template <class T>
        typename Sig<T>::ResultType operator()( const T& x ) const {
    return cons(x,NIL);
        }

     };
    
  }

typedef Full1<impl::XAbstract> Abstract;
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Abstract abstract;
FCPP_MAYBE_NAMESPACE_CLOSE

 #ifdef FCPP_DEBUG
    namespace traits {
    template<>
       class debug_traits<impl::XAbstract>
       {
       public:
  static const bool has_name = true;
  static std::string name(const impl::XAbstract &f) { return f.name(); }
       };
    }
 #endif

}

using namespace fcpp;


void setup() {
  // put your setup code here, to run once:
  // Early ideas.
  List<int> l12 = makeList(1,2);
  List<int> l123 = list_with(1,2,3);

  List<int> l = cons(0,l12);

  List<int> a = abstract(1);

  BootStrap<int> bs1(1);
  BootStrap<int> bs2 = boot(2);

}

void loop() {
  // put your main code here, to run repeatedly:

}
