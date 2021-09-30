/**
 * CMRI module for detection and turnouts
 * ======================================
 * 
 * Set up for direct connection with USB lead from JMRI to Arduino
 * 
 * CMRI inputs and outputs in the 0000 range
 * 
 * CMRI library
 * https://github.com/madleech/ArduinoCMRI
 *
 * CMRI turnout control changed to. Two Bit, Pulsed Output.
 *
 * Two sensor feedback
 */

#include <CMRI.h>
#include <Servo.h>

CMRI cmri; // defaults to a SMINI with address 0. SMINI = 24 inputs, 48 outputs

/*--------------------------------------------------------------------*/

// defining Pin allocation

#define turnout1Pin 9
#define turnout2Pin 10

#define turnout1FrogPin 11
#define turnout2FrogPin 12

#define pushButtonRearPin 14
#define pushButtonFrontPin 15
#define block1Pin 3
#define block2Pin 4
#define block3Pin 5
#define block4Pin 6
#define block5Pin 7
#define block6Pin 8
/*--------------------------------------------------------------------*/


// define turnouts

#define turnoutMoveSpeed 8   // [ms] lower number is faster
unsigned long turnoutMoveDelay;
boolean turnoutIsClosed = true;   // 1 is Close, 0 is Thrown

Servo turnout1;
#define turnout1ClosedPosition 70      // angle of servo
#define turnout1ThrownPosition 110     // angle of servo

byte turnout1Position = turnout1ClosedPosition;  // byte ALERT!!!!!!!!!!!!!!!!!!
byte turnout1Target   = turnout1ClosedPosition;

Servo turnout2;
#define turnout2ClosedPosition 80      // angle of servo
#define turnout2ThrownPosition 120     // angle of servo

byte turnout2Position = turnout2ClosedPosition;  // byte ALERT!!!!!!!!!!!!!!!!!!
byte turnout2Target   = turnout2ClosedPosition;

// end of defining turnouts
/*--------------------------------------------------------------------*/

void setup() {
  Serial.begin(9600, SERIAL_8N2); // start talking at 9600bps
 
  // push buttons
  pinMode(pushButtonRearPin, INPUT_PULLUP);
  pinMode(pushButtonFrontPin, INPUT_PULLUP);

  // OUTPUTS setup
  // frogs
  pinMode(turnout1FrogPin, OUTPUT);
  pinMode(turnout2FrogPin, OUTPUT);
  
  digitalWrite(turnout1FrogPin, LOW);
  digitalWrite(turnout2FrogPin, HIGH);

  // servos
  turnout1.attach(turnout1Pin);
  turnout2.attach(turnout2Pin);

  turnout1.write(turnout1Position);
  turnout2.write(turnout2Position);
 
}

/*--------------------------------------------------------------------*/

void loop() {
  // build up a packet
  cmri.process();
/*--------------------------------------------------------------------*/ 
  // update inputs (invert digitalRead due to the pullups)
  
  cmri.set_bit(0, !digitalRead(block1Pin));
  cmri.set_bit(1, !digitalRead(block2Pin));
  cmri.set_bit(2, !digitalRead(block3Pin));
  cmri.set_bit(3, !digitalRead(block4Pin));
  cmri.set_bit(4, !digitalRead(block5Pin));
  cmri.set_bit(5, !digitalRead(block6Pin));
  
/*--------------------------------------------------------------------*/ 
  // check for "local" switches for either push botton switch
  if (digitalRead(pushButtonRearPin) == LOW || digitalRead(pushButtonFrontPin) == LOW){
    if (!turnoutIsClosed){
  	  turnout1Target = turnout1ClosedPosition;
	  cmri.set_bit(6, LOW);
	  }
    if (turnoutIsClosed){
  	  turnout1Target = turnout1ThrownPosition;
	  cmri.set_bit(7, LOW);
	  }
  }
/*--------------------------------------------------------------------*/     
  // update outputs from CMRI
  // check to see if CMRI is sending a turnout bit
  if (cmri.get_bit(0)){
	turnout1Target = turnout1ClosedPosition;
	cmri.set_bit(6, LOW);
  	}
  if (cmri.get_bit(1)){
	turnout1Target = turnout1ThrownPosition;
	cmri.set_bit(7, LOW);
  }  
/*--------------------------------------------------------------------*/
  // Check postion of turnout1 and set frogs and turnout2 accordingly
  if (turnoutIsClosed){
    if (turnout1Position == turnout1ThrownPosition){
      turnout2Target = turnout2ThrownPosition;
      if (turnout2Position == turnout2ThrownPosition){
        digitalWrite(turnout1FrogPin, HIGH);
        digitalWrite(turnout2FrogPin, LOW);
        turnoutIsClosed = false;
		cmri.set_bit(6, HIGH);
      }
    }
  }
  if (!turnoutIsClosed){
    if (turnout1Position == turnout1ClosedPosition) {
      turnout2Target = turnout2ClosedPosition;
      if (turnout2Position == turnout2ClosedPosition) {
       digitalWrite(turnout1FrogPin, LOW);
       digitalWrite(turnout2FrogPin, HIGH);
       turnoutIsClosed = true;
	   cmri.set_bit(7, HIGH);
      }
    }
  }
  
/*--------------------------------------------------------------------*/
  // move turnouts if state has changed
  
  // if the turnout1 not at the desired position
  if (turnout1Position != turnout1Target) {
    if (millis() > turnoutMoveDelay) {
      turnoutMoveDelay = millis() + turnoutMoveSpeed;
      if (turnout1Position < turnout1Target) turnout1Position++;
      if (turnout1Position > turnout1Target) turnout1Position--;
      turnout1.write(turnout1Position);
    }
  } // end of setting turnout1

  // if the turnout2 not at the desired position
  if (turnout2Position != turnout2Target) {
    if (millis() > turnoutMoveDelay) {
      turnoutMoveDelay = millis() + turnoutMoveSpeed;
      if (turnout2Position < turnout2Target) turnout2Position++;
      if (turnout2Position > turnout2Target) turnout2Position--;
      turnout2.write(turnout2Position);
    }
  } // end of setting turnout2

}
