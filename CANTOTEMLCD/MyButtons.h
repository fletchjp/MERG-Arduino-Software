/// @file MyButtons.h
/// @brief Set up my own ranges for DfRobotInputAbstraction

#ifndef MY_BUTTONS_H
#define MY_BUTTONS_H

//const PROGMEM DfRobotAnalogRanges dfRobotAvrRanges { 0.0488F, 0.2441F, 0.4394F, 0.6347F, 0.8300F};

// These keys are not in the same order. LEFT UP DOWN RIGHT SELECT
const PROGMEM DfRobotAnalogRanges MyAvrRanges { 0.02F, 0.08F, 0.15F, 0.31F, 0.4F};

/// This works in a header file and not in the Arduino ino file.
IoAbstractionRef inputFromMyShield(uint8_t pin = A0, AnalogDevice* device = nullptr) {
    device = new ArduinoAnalogDevice();
    return new DfRobotInputAbstraction(&MyAvrRanges, pin, device);
}

#endif
