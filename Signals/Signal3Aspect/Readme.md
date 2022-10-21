# Signal3Aspect examples

These examples work with one three aspect signal.

These example needs the Adafruit_PWM_Servo_Driver_Library for the PWM pins.

They also need FastPWMLight.h which is an addition to the SignalControl library.

NOTE: I have now renamed FastPWMLight.h as FastPCALight.h as that is what is different. There is a copy in the old name for now.

There is now also SlowPCALight.h which did not exist before.

## Signal3AspectCycleI2C

This is now tested.

## Signal3AspectCycleI2CTM

Adding task management and now tested.

## Signal3AspectSlowFadeCycleI2C

This is a demo which fades in and out on each LED in turn. It uses delays

## Signal3AspectSlowCycleI2C

This now runs using some aspects of state with explicit delays.

## Signal3AspectSlowCycleI2CTM

This runs a single task with delays replaced by yieldForMicros.

## Signal3AspectWithButton

Awaiting a button for a test.

## Signal3AspectWithButtonI2C

Converted to use I2C and PC9685. Awaiting a button for a test.

## Signal3AspectWithButtonI2CTM

Adding task management and now tested except awaiting a button for a full test.

## Signal3AspectWith_IR_I2CTM

I have added an IR detector as well as the trigger for the light to go red.
