// CBUS_LCDBut
// This is a version of a code with a long name.
// LCDshieldButtonsSerialDFRobot.ino 
// LCD shield with buttons example code
// Adapted to run with CBUS
// 3rd party libraries
#include <Streaming.h>

////////////////////////////////////////////////////////////////////////////
// CBUS library header files


//////////////////////////////////////////////////////////////////////////////////////////////

// LCD shield with buttons example code

/*
Arduino 2x16 LCD - Detect Buttons
modified on 18 Feb 2019
by Saeed Hosseini @ Electropeak
https://electropeak.com/learn/
Using data from here:
https://wiki.dfrobot.com/Arduino_LCD_KeyPad_Shield__SKU__DFR0009_
I have also played with the values.
*/
#include <LiquidCrystal.h>
//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

int x;
int prevx = 0;
int range;
int prevrange = 0;
int y = 0;

// Serial IO
#define SERIAL_SPEED            19200   // Speed of the serial port.

void setup() {
  // put your setup code here, to run once:

  // Initialise
  Serial.begin(SERIAL_SPEED);             // Start Serial IO.
  Serial.println("CBUS LCDBut");
  //analogWrite(pin_d6,50);
 lcd.begin(16, 2);
 lcd.setCursor(0,0);
 lcd.print("CBUS LCDBut");
 lcd.setCursor(0,1);
 lcd.print("Press Key:");
}

void loop() {
  // put your main code here, to run repeatedly:
 x = analogRead (0);
 if (x < 175){         // was 50
  range = 1;
 } else if (x < 350){ // was 250
  range = 2;
 } else if (x < 500){ // unchanged
  range = 3;
 } else if (x < 800){ // was 650
  range = 4;
 } else if (x < 850){ // unchanged
  range = 5;
 } //else { range = 0; }
 if (range != prevrange) {
 Serial.print(range);
 Serial.print(" ");
 Serial.print(x);
 lcd.setCursor(10,1);
 switch (range) {
  case 1:
  {
   lcd.print ("Right ");
   //if (y == 0) {
   Serial.println(" Right");
     //y = 1;
   //}
   break;
  }
  case 2:
  {
   lcd.print ("Up    ");
   Serial.println(" Up");
   break;
  }
  case 3:
  {
   lcd.print ("Down  ");
   Serial.println(" Down");
   break;
  }
  case 4:
  {
   lcd.print ("Left  ");
   Serial.println(" Left ");
   break;
  }
  case 5:
  {
   lcd.print ("Select");
   Serial.println(" Select");
   break;
  }
  default:
  break;
 }
 prevrange = range;
 }

}
