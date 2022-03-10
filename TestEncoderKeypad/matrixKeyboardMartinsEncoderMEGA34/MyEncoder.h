/// @file MyEncoder.h
/// @brief Library for using rotary encoders.
///
/// -----
/// MyEncoder.h - Library for using rotary encoders.
/// This class is implemented for use with the Arduino environment.
//
// 

#ifndef MyEncoder_h
#define MyEncoder_h

#include "Arduino.h"

/// @brief MyEncoder defined for two pins able to interrupt or pin change interrupt
class MyEncoder
{
public:
 
  // ----- Constructor -----
  MyEncoder(byte pin1, byte pin2);

  // retrieve the current position
  int getPosition();

  // adjust the current position
  void setPosition(int newPosition);
  
  void setLimits(byte minPos, byte maxPos);
  void setWrap(bool wrap);

  // function to be called by pin change interrupt.
  void encoderISR(void);

private:
  byte _pin1, _pin2; // Arduino pins used for the encoder.
  
//  LatchMode _mode; // Latch mode from initialization

  volatile int8_t _oldState;

  volatile int _position;        // Internal position (4 times _positionExt)
  volatile int _positionExt;     // External position
 // volatile int _positionExtPrev; // External position (used only for direction checking)
  byte _minPos = 0;
  byte _maxPos = 10;
  bool _wrap = 0;

};

#endif

// End
