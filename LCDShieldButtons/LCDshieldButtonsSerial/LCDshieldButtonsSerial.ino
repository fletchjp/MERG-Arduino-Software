// LCD shield with buttons example code

/*
Arduino 2x16 LCD - Detect Buttons
modified on 18 Feb 2019
by Saeed Hosseini @ Electropeak
https://electropeak.com/learn/
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
  Serial.println("Electropeak.com");
  //analogWrite(pin_d6,50);
 lcd.begin(16, 2);
 lcd.setCursor(0,0);
 lcd.print("Electropeak.com");
 lcd.setCursor(0,1);
 lcd.print("Press Key:");
}

void loop() {
  // put your main code here, to run repeatedly:
 x = analogRead (0);
 if (x < 60){
  range = 1;
 } else if (x < 200) {
  range = 2;
 } else if (x < 400){
  range = 3;
 } else if (x < 600){
  range = 4;
 } else if (x < 800){
  range = 5;
 }
 if (range != prevrange) {
 Serial.print(range);
 lcd.setCursor(10,1);
 if (x < 60) {
   lcd.print ("Right ");
   //if (y == 0) {
   Serial.println(" Right");
     //y = 1;
   //}
 }
 else if (x < 200) {
   lcd.print ("Up    ");
   Serial.println(" Up");
 }
 else if (x < 400){
   lcd.print ("Down  ");
   Serial.println(" Down");
 }
 else if (x < 600){
   lcd.print ("Left  ");
   Serial.println(" Left ");
}
 else if (x < 800){
   lcd.print ("Select");
   Serial.println(" Select");
 }
 prevrange = range;
 }
}
