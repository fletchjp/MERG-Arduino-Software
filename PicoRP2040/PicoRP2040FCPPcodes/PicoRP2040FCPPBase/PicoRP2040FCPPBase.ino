//////////////////////////////////////////////////////////////////////
// Pico RP2040 FC++ Base
// Base case for developing new code
//////////////////////////////////////////////////////////////////////

// ========================
// headers
// ========================

#include <string>
#include <vector>
#include <algorithm>

// ========================
// FC++ headers
// ========================

#define FCPP152
#define FCPP_ENABLE_LAMBDA
#define FCPP_EXTRA_STRONG_CURRY
#define FCPP_PATTERN // define this to include pattern.h
//#define FCPP_USE_EXCEPTIONS
#define FCPP_UNIFY // Unification of Monads and Functors in functors.h
#include "fcpp_prelude.h"
#include "fcpp/monoids.h"
#include "fcpp/functors.h"
#include "fcpp/comonad.h"
#include "fcpp/arrow.h"

#include <Streaming.h>

using namespace fcpp;

/// I have renamed String to StringL to avoid a name clash with the Arduino use of String.
/// This is used with the Parser monad and other codes.
typedef List<char> StringL;

namespace fcpp {
/// Space for new functoids.

}
//////////////////////////////////////////////////////////


void fcpp_examples()
{
  Serial.println("--------------------------");
  Serial.println("Some simple FC++ operations");
  Serial.print("plus(1,2) = "); Serial.println(plus(1,2));
  Serial.print("plus(1.5,2.3) = "); Serial.println(plus(1.5,2.3));
  Serial.print("plus(1)(2) = "); Serial.println(plus(1)(2));
  Serial.print("inc(1) = "); Serial.println(inc(1));
  Serial.println("--------------------------");

  Maybe<int> mx(2), my(3);
  Maybe<int> mz;
  if (mz.is_nothing()) Serial.println("mz is nothing");
  mz = just(plus(mx.value(),my.value()));
  if (mz.is_nothing()) { 
     Serial.println("mz is nothing");
  } else {
     Serial.print("mz has "); Serial.println(mz.value());
  }
  Serial.println("--------------------------");

}

// I have put these examples here to show how some things work and also to avoid some warnings
// that things are defined and not used.
auto bar() {
   LambdaVar<1> X;
   LambdaVar<2> Y;
   return lambda()[
      doM[ X <= list_with(1,2), Y <= list_with(3,4),
           unitM<ListM>()[ makePair[X,Y] ] ] ];
}

auto qux() {
   LambdaVar<1> X;
   LambdaVar<2> Y;
   return lambda()[ compM<ListM>()[ makePair[X,Y] | 
          X<=list_with(1,2), guard[true], Y<=list_with(3,4), 
          guard[equal[divides[Y,X],3] ] ]
      ];
}
auto whoa() {
   LambdaVar<1> even;
   LambdaVar<2> odd;
   LambdaVar<3> X;
   return lambda()[
      letrec[ even==lambda(X)[ if1[fcpp::equal[X,0],true,odd[fcpp::minus[X,1]]] ],
              odd ==lambda(X)[ if2[notEqual[X,0],even[fcpp::minus[X,1]],false] ]
      ].in[ even[3] ] ];
}

void lambda_examples()
{
   LambdaVar<1> F;
//   LambdaVar<2> G;
   //LambdaVar<3> H;
   LambdaVar<4> X;
   Serial << "lambda(F,X)[ if0[true,F[X,2],X] ](plus,1) = ";
   Serial << lambda(F,X)[ if0[true,F[X,2],X] ](plus,1) << endl;
  
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  while (!Serial) { }
  //::delay(5000);
  Serial.printf("Pico RP2040 FC++ operations\n");
#ifdef ARDUINO_PICO_VERSION_STR
  Serial.printf("ARDUINO_PICO_VERSION_STR ");
  Serial.println(ARDUINO_PICO_VERSION_STR);
#endif
  fcpp_examples();
  Serial.println("after fcpp_examples");
  Serial.println("--------------------------");
 }

int i = 0;
 
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(i);
  //i++;
}
