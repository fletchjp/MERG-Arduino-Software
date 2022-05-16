// TaskMgrI2C20x4
// New code combining HelloI2C20x4 with ideas from TaskMgrIntegration 
// with a different problem.

// HelloI2C20x4
// Adapted for a 20x4 LCD
// Note that the I2C address is 0x27

/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display when the display is fitted
 with an I2C backpack. The LiquidCrystal library works with all LCD
 displays that are compatible with the Hitachi HD44780 driver. 

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi
modified by Dave Cherry in 2018 to demo I2C backpack support.
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <Arduino.h>
#include <LiquidCrystalIO.h>

// When using the I2C version, these two extra includes are always needed. Doing this reduces the memory slightly for
// users that are not using I2C.
#include <IoAbstractionWire.h>
#include <Wire.h>

// For most standard I2C backpacks one of the two helper functions below will create you a liquid crystal instance
// that's ready configured for I2C. Important Note: this method assumes a PCF8574 running at 100Khz. If otherwise
// use a custom configuration as you see in many other examples.

// If your backpack is wired RS,RW,EN then use this version
LiquidCrystalI2C_RS_EN(lcd, 0x27, false)

// If your backpack is wired EN,RW,RS then use this version instead of the above.
//LiquidCrystalI2C_EN_RS(lcd, 0x20, false)


**
 * Here we create an event that handles all the drawing for an application, in this case printing out readings
 * of a sensor when changed. It uses polling and immediate triggering to show both examples
 */
class DrawingEvent : public BaseEvent {
private:
    int heaterTemperature;
    bool heaterIsOn;
    volatile bool emergency; // if an event comes from an external interrupt the variable must be volatile.
    bool hasChanged;
public:
    /**
     * This is called by task manager every time the number of microseconds returned expires, if you trigger the
     * event it will run the exec(), if you complete the event, it will be removed from task manager.
     * @return the number of micros before calling again.
     */
    uint32_t timeOfNextCheck() override {
        setTriggered(hasChanged);
        return millisToMicros(500); // no point refreshing more often on an LCD, as its unreadable
    }

    /**
     * This is called when the event is triggered, it prints all the data onto the screen.
     */
    void exec() override {
        hasChanged = false;
        // Here we need to output to the display.
   }

    /**
     * This sets the latest temperature and heater status, but only marks the event changed,
     * so it will need to poll in order to trigger.
     * This prevents excessive screen updates.
     * @param temp the new temperature
     * @param on if the heater is on
     */
    void setLatestStatus(int temp, bool on) {
        //heaterTemperature = temp;
        //heaterIsOn = on;
        hasChanged = true;// we are happy to wait out the 500 millis
    }
    /**
     * Triggers an emergency that requires immediate update of the screen
     * @param isEmergency if there is an urgent notification
     */
    void triggerEmergency(bool isEmergency) {
        emergency = isEmergency;
        markTriggeredAndNotify(); // get on screen asap.
    }
};

// create an instance of the above class
DrawingEvent drawingEvent;

void setup() {
  // most backpacks have the backlight on pin 3.
  //lcd.configureBacklightPin(3);
  //lcd.backlight();
  
  // for i2c variants, this must be called first.
  Wire.begin();

  // set up the LCD's number of columns and rows, must be called.
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.print("hello over i2c!");

  //
  // when using this version of liquid crystal, it interacts (fairly) nicely with task manager. 
  // instead of doing stuff in loop, we can schedule things to be done. But just be aware than
  // only one task can draw to the display. Never draw to the display in two tasks.
  //
  // You don't have to use the library with task manager like this, it's an option.
  //
  taskManager.scheduleFixedRate(250, [] {
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    float secondsFraction =  millis() / 1000.0F;
    lcd.print(secondsFraction);
  });
}

void loop() {
    taskManager.runLoop();
}
