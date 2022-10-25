# Signal3Aspect examples

These examples work with one three aspect signal.

These example needs the Adafruit_PWM_Servo_Driver_Library for the PWM pins.

They also need FastPCALight.h or SlowPCALight.h which are in the SignalControlPCA9685 library.

This is a separate library which needs the SignalControl library.

The examples with task management (TM in the name) also need TaskManagerIO and some need IOAbstraction as well.

## Signal3AspectCycleI2C

This is now tested.

## Signal3AspectCycleI2CTM

Adding task management and now tested. This fades off one LED and then on with the next LED.

## Signal3AspectSlowFadeCycleI2C

This is a demo which fades in and out on each LED in turn. It uses delays

## Signal3AspectSlowCycleI2C

This now runs using some aspects of state with explicit delays.

## Signal3AspectSlowCycleI2CTM

This runs a single task with delays replaced by yieldForMicros.

## Signal3AspectSlowCycleI2CTME

Adding task management executable. This should fades off one LED and another LED on at the same time.

It does so in separate tasks so is not well coodinated.

## Signal3AspectSlowCycleI2CTME2

Changing the task management executable. This now fades off one LED and another LED on in the same task.

## Signal3AspectWithButton

Awaiting a button for a test.

## Signal3AspectWithButtonI2C

Converted to use I2C and PC9685. Awaiting a button for a test.

## Signal3AspectWithButtonI2CTM

Adding task management and now tested except awaiting a button for a full test.

## Signal3AspectWith_IR_I2CTM

I have added an IR detector as well as the trigger for the light to go red. This code is now working.

## Signal3AspectSlowWith_IR_I2CTME2

This is the next challenge to add IR detection. This now fades off one LED and another LED on in the same task.

This is now working.

