/// @file TestWarnings.ino
/// @brief Test code warnings.
///
/// There are two problems.
///
/// One is that the Arduino IDE normal setting turns off all the warnings.
///
/// The other is that the Arduino IDE uses -fpermissive which means that some errors become warnings.
/// 
/// As warnings are turned off there are then no warnings of some errors.

// 3rd party libraries
#include <Streaming.h>

void setup() {
  // put your setup code here, to run once:
  while(!Serial);
  Serial.begin (115200);
  Serial << endl << endl << F("> ** TestWarnings ** ") << __FILE__ << endl;
}

bool return_value_missing()
{
  
}

void return_value_not_expected()
{
  bool res = false;
  return res;
}

int return_value_wrong_type()
{
  bool res = false;
  return res;
}


void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);
  //Serial << F("enter a number");
  delay(1000);
  int cast = 0;
  //Serial >> cast;
  do {
    Serial << " " << cast << endl;
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
    cast++;
  } while (cast < 5);
  cast = 0;
}
