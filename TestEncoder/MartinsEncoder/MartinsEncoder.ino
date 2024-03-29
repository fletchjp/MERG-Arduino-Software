/// @file MartinsEncoder.ino
/// @brief Test for encoder based on Martin Da Costa's library code.
///
/// I am going to configure this for a UNO or NANO
///
/// Swap the pins to get the opposite action
#define SWAP_PINS 0

#include "EncoderMD.h"

boolean TurnDetected;

const int PinCLK=2;   // Generating interrupts using CLK signal
const int PinDT=3;    // Reading DT signal
const int PinSW=4;     // Reading Push Button switch
// Also connect +5V and ground.

#if SWAP_PINS
EncoderMD encoder(PinDT,PinCLK);
#else
EncoderMD encoder(PinCLK,PinDT);
#endif

int RotaryPosition=0;    // To store Stepper Motor Position

int PrevPosition;     // Previous Rotary position Value to check accuracy

void setupPCI()
{
  cli();
  PCICR  |= 0b00000100;  //Set Pin Change Interrupt on Register D
  PCMSK2 |= 0b00001100;  //Set pins 2 & 3 for interrupt
  sei();
}


void setup() {
  // put your setup code here, to run once:
  setupPCI();
  // These are done in the encoder constructor
  //pinMode(PinCLK,INPUT);
  //pinMode(PinDT,INPUT);  
  pinMode(PinSW,INPUT);
  digitalWrite(PinSW, HIGH); // Pull-Up resistor for switch
  Serial.begin (115200);
  Serial.println("MartinsEncoder test for a UNO or NANO");
  encoder.setLimits(0,100);
}


ISR(PCINT2_vect)  // Pin 2 & 3 interrupt vector
{
  encoder.encoderISR();
}

void loop() {
  // put your main code here, to run repeatedly:
  //small_stepper.setSpeed(700); //Max seems to be 700
  if (!(digitalRead(PinSW))) {   // check if button is pressed
    if (RotaryPosition == 0) {  // check if button was already pressed
        //Serial.println("XXX ");
       } else {
        //small_stepper.step(-(RotaryPosition*50));
        RotaryPosition=0; // Reset position to ZERO
        encoder.setPosition(RotaryPosition);
        Serial.print("X ");
        Serial.println(RotaryPosition);
        PrevPosition = RotaryPosition;
      }
      
  }

  // Runs if rotation was detected
  RotaryPosition = encoder.getPosition();
  TurnDetected = (RotaryPosition != PrevPosition);
  if (TurnDetected)  {
    PrevPosition = RotaryPosition; // Save previous position in variable
    Serial.println(RotaryPosition);
  }

}
