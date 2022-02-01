/// @file MyEncoder.cpp
/// @brief Library for using rotary encoders - initialization and default values.
///
/// Change so that encoderISR returns bool - true if there is a change.
/// This is to help with multiple encoders.

// -----
// 
// -----

#include "MyEncoder.h"
#include "Arduino.h"

/// @brief Structure for the state machine
///
/// The array holds the values -1 for the entries where a position was decremented,
/// a 1 for the entries where the position was incremented
/// and 0 in all the other (no change or not valid) cases.
const int8_t KNOBDIR[] = {
    0, -1, 1, 0,
    1, 0, 0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0};


// ----- Initialization and Default Values -----

MyEncoder::MyEncoder(byte pin1, byte pin2)
{
  // Remember Hardware Setup
  _pin1 = pin1;
  _pin2 = pin2;

  // Setup the input pins and turn on pullup resistor
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);

  // when not started in motion, the current state of the encoder should be 3
  byte sig1 = digitalRead(_pin1);
  byte sig2 = digitalRead(_pin2);
  _oldState = sig1 | (sig2 << 1);

  // start with position 0;
  _position = 0;
  _positionExt = 0;
} 

int MyEncoder::getPosition()
{
  return _positionExt;
} // getPosition()

void MyEncoder::setLimits(byte minPos, byte maxPos)
{
	if (maxPos > minPos)
	{
		_minPos = minPos;
		_maxPos = maxPos;
	}
	encoderISR();
}

void MyEncoder:: setWrap(bool wrap)
{
	_wrap = wrap;
}

void MyEncoder::setPosition(int newPosition)
{
	
    _position = ((newPosition << 2) | (_position & 0x03L)); // Multiply positionExt by 4 for changes between detents and set detent (0x03L)
    _positionExt = newPosition;

} 


bool MyEncoder::encoderISR(void)
{
  byte sig1 = digitalRead(_pin1);
  byte sig2 = digitalRead(_pin2);
  int8_t thisState = sig1 | (sig2 << 1);  // Assembles 2 bit grey code from pin inputs

  if (_oldState != thisState) {
    _position += KNOBDIR[thisState | (_oldState << 2)];  // Makes a 4 bit index to address the static array
    _oldState = thisState;


        _positionExt = _position >> 2;  // _position counts every change of state. Divide by 4 to get actual position

 
  	if (_positionExt < _minPos)  //limit minimum position
	{
		if (_wrap) // go to max position if wrap
		
		{
			setPosition(_maxPos); 
		}
		else  //clamp minimum position
		{
		    setPosition(_minPos);
		}
	}		
	else if (_positionExt > _maxPos)  //limit maximum position
	{
		if (_wrap) // go to minimum position if warp
		{
			setPosition(_minPos);
		}
		else  //clamp maximum position
		{
			setPosition(_maxPos);
		}
	}
  } else { return false; }
  return true;  
} 

// End
