# CAN1602PINIOTM

Extending CAN1602PINIO using LiquidCrystalIO DrawingEvent.

This is now working

The updating of the screen is already being done through the SwitchInput class which provides switches.

This then calls the nominated routine logKeyPressed to do the update.

There is also update of the display in displayError and these may need to be coordinated.

These have now been integrated into a drawing event so that all screen updates are done in one task.
