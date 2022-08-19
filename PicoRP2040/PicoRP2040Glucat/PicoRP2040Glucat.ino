//////////////////////////////////////////////////////////////////////
// Pico RP2040 Glucat
//////////////////////////////////////////////////////////////////////

#include <string>


//#define throw Serial<<

#include "glucat.h"
#include "glucat/glucat_imp.h"
#include "glucat/tuning.h"
typedef glucat::tuning<> Tune_P;

#include <Streaming.h>

using namespace glucat;

//////////////////////////////////////////////////////////

//template< class M_T , class T>
void do_test1_double()
{
    typedef double T;
    typedef framed_multi<T> number;
    //typedef Multivector_T number;
    Serial << "in do_test1() at start" << endl;
    T x, y;
    x = T(1.);
    y = sqrt(x);
    Serial << "in do_test1() after sqrt with y = " << y << endl;
    index_set<DEFAULT_LO,DEFAULT_HI> i1(1);
    index_set<DEFAULT_LO,DEFAULT_HI> i2(2);
    //number z1(i1,1.);
    number z2(i2);
    //Serial<< "in do_test1() after sqrt with i1 = " << i1 << endl;
    //Serial << "in do_test1() after sqrt with z1 = " << z1 << endl;
    //Serial << "in do_test1() after sqrt with z2 = " << z2 << endl;
    //number PO = 4.*z1 + 3.*z2;
    //number QO = 2.*z1 + 3.*z2;

}


void glucat_examples()
{
  Serial << "framed_multi<double>" << endl;
  Serial << endl;
  //do_test1< framed_multi<double>, double >();
  Serial << "matrix_multi<double>" << endl;
  //do_test1< matrix_multi<double>, double >();
}
 

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  while (!Serial) { }
  //::delay(5000);
  Serial.printf("Pico RP2040 Glucat operations\n");
#ifdef _GLUCAT_VERSION
  Serial << "Glucat Version " << _GLUCAT_VERSION << endl;
#endif
  glucat_examples();
  Serial.println("after glucat_examples");
}

 
void loop() {
  
}
