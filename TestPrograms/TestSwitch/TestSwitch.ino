/// @file TestSwitch.ino
/// @brief Test ideas about switch statements.

// 3rd party libraries
#include <Streaming.h>

void setup() {
  // put your setup code here, to run once:
  while(!Serial);
  Serial.begin (115200);
  Serial << endl << endl << F("> ** TestSwitch ** ") << __FILE__ << endl;
}


void loop() {
  // put your main code here, to run repeatedly:
  Serial << F("enter a number");
  int cast = 0;
  Serial >> cast;
  // This will need to be input
  int value = 0;
  Serial << F("cast = ") << cast << endl;

  switch(cast) {

     case 0:  
        Serial << F("simple case") << endl;
        break;

     case 1:
        Serial << F("This case sets value") << endl;
        value = cast;
        break;

     case 2:
        //This is the problem - defining a variable within the switch.
        int another = cast;
        break;

     case 3:
        // Here -fpermissive allows the use of "another" in case 3 when it will not have been defined.
        Serial << another << endl;
        break;

     default:
        Serial << F("Default case: ") << cast << endl;
  }
  Serial << F("value = ") << value << endl;

}
