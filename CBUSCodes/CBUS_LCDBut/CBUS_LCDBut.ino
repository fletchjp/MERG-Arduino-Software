// CBUS_LCDBut
// This is a version of a code with a long name.
// LCDshieldButtonsSerialDFRobot.ino 
// LCD shield with buttons example code
// Adapted to run with CBUS
// 3rd party libraries
#include <Streaming.h>

////////////////////////////////////////////////////////////////////////////
// CBUS library header files
// CBUS library header files
#include <CBUS2515.h>            // CAN controller and CBUS class
#include "LEDControl.h"          // CBUS LEDs
#include <CBUSconfig.h>          // module configuration
#include <cbusdefs.h>            // MERG CBUS constants
#include <CBUSParams.h>

////////////DEFINE MODULE/////////////////////////////////////////////////
/// Use these values for the CBUS outputs
int button = -1;
int prevbutton = -1;

/// module name
unsigned char mname[7] = { 'L', 'C', 'D', 'B', 'u', 't', ' ' };

/// constants
const byte VER_MAJ = 1;         // code major version
const char VER_MIN = 'a';       // code minor version
const byte VER_BETA = 1;        // code beta sub-version
const byte MODULE_ID = 99;      // CBUS module type

const unsigned long CAN_OSC_FREQ = 8000000;     // Oscillator frequency on the CAN2515 board

#define NUM_LEDS 1              // How many LEDs are there?
#define NUM_SWITCHES 1          // How many switchs are there?

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
