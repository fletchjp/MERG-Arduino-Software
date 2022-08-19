//////////////////////////////////////////////////////////////////////
// Pico RP2040 Glucat
//////////////////////////////////////////////////////////////////////

#include <string>

#include "glucat.h"
#include "glucat/glucat_imp.h"

#include <Streaming.h>


//////////////////////////////////////////////////////////

void glucat_examples()
{
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
  Serial.println("after fcpp_examples");
}

 
void loop() {
  
}
