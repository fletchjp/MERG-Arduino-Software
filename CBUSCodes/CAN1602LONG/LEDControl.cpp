
#include "LEDControl.h"

//
/// class for individual LED with non-blocking control
//

LEDControl::LEDControl() {

  _state = LOW;
  _flash = false;
  _lastTime = 0UL;
}

//  set the pin for this LED

void LEDControl::setPin(byte pin) {

  _pin = pin;
  pinMode(_pin, OUTPUT);
}

// turn LED state on

void LEDControl::on(void) {

  _state = HIGH;
  _flash = false;
}

// turn LED state off

void LEDControl::off(void) {

  _state = LOW;
  _flash = false;
}

// blink LED

void LEDControl::flash(int period) {

  _flash = true;
  _period = period;
}

// actually operate the LED dependent upon its current state
// must be called frequently from loop() if the LED is set to blink or pulse

void LEDControl::run() {

  if (_flash) {

    // blinking
    if ((millis() - _lastTime) >= _period) {
      _state = !_state;
      _lastTime = millis();
    }
  }
/*  _write(_pin, _state);
}

// write to the physical pin

void LEDControl::_write(byte pin, bool state) {
*/
  // Serial << F("> mcu pin = ") << pin << F(", state = ") << state << endl;
  digitalWrite(_pin, _state);
}
