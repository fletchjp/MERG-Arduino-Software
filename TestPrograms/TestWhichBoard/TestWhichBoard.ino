/// @file TestWhichBoard.ino
/// @brief Test which board the code is running on.
///
/// There are two libraries in the Arduino libraries which will help with doing this.
///
/// Board_Identity and MyMacros 
///
/// The second one extends the first one and will be used here.
/// Unfortunately it does not find NANO and MEGA although the first one does!
/// So I have made the use conditional on defining USE_MYMACROS.
/// I have found out that it is intended that the user adds the things they have!!
/// I have added NANO, MEGA1280 and MEGA2560 to the UNO section.
/// I need to back this up as it would be overwritten oif the library is updated!
/// Otherwise it uses only Board_Identify which does not support RP2040 types.
///
/// To avoid the library overhead, the code can be inspected to find the macros needed.

#define USE_MYMACROS
// Adding this line before the library will output compile time identification.
#define ARCH_IDENTIFY_WARNING
#define BOARD_IDENTIFY_WARNING
#include <Board_Identify.h>
#ifdef USE_MYMACROS
#include <MyMacros.h>
#endif
// 3rd party libraries
#include <Streaming.h>

const byte VER_MAJ  = 1;
const byte VER_MIN  = 0;
const byte VER_DETAIL = 0;

void heading()
{
  Serial << endl << endl << __FILE__ << endl;
  Serial << F("Ver: ") << VER_MAJ << F(".") << VER_MIN << F(".") << VER_DETAIL;
  Serial << F(" compiled on ") << __DATE__ << F(" at ") << __TIME__ << F(" using compiler ") << __cplusplus << endl;
}

#ifdef USE_MYMACROS
// Optional define a structure variable. Only required for access to struct data.
struct board myBoard;
#endif

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200); while(!Serial); Serial.println();
  heading();
  Serial.println("==== Setup ====");
#ifdef USE_MYMACROS
  Serial.print("Matching board define: ");
  printlnMatch(); 
  Serial.print("Arduino's 'BUILD_BOARD' property: ");
  Serial.println(BUILD_BOARD);
  delay(1000);
  // Adds the personal collection of boards and 100+ boards from Board Identify library
    // Use members in struct 'MyBoard' to access identity information from 'MyMacros.h'.
    Serial.println("==== MyMacros (Automatically pulls in from Board Identify) ===="); 
    Serial.print("Board Type: ");
    Serial.println(myBoard.type);  
    Serial.print("Board Make: ");
    Serial.println(myBoard.make);
    Serial.print("Board Model: ");
    Serial.println(myBoard.model);
    Serial.print("Board MCU: ");
  
    delay(3000);
#endif
  // Should be identical to above
    Serial.println("==== Board Identify (source board data) ====");
    Serial.print("Board Type: ");
    Serial.println(BoardIdentify::type);
    Serial.print("Board Make: ");
    Serial.println(BoardIdentify::make);
    Serial.print("Board Model: ");
    Serial.println(BoardIdentify::model);
    Serial.print("Board MCU: ");
    Serial.println(BoardIdentify::mcu);
    Serial.println();

    delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);
  //Serial << F("enter a number");
}
