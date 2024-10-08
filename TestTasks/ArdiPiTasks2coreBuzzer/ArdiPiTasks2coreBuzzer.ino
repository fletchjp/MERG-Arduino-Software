/// ArdiPiTasks2codeBuzzer.ino
/// Development for the ArdiPi
/// Some things being added from the taskManagement example as well. 

/**
 * @file SimpleTasks2coreBuzzer.ino
 * A very simple example of how to use task manager to schedule tasks to be done
 * extended to use two cores on the Pico
 *
 * In this example we demonstrate how to create tasks that execute at a point in time,
 * that repeat at a given interval, and tasks that are executed as soon as possible
 * by task manager. We also show how to cancel a running task.
 *
 */

// To use task manager we must include the library
//#include <Arduino.h>
#include "TaskManagerIO.h"

// RP2040 buzzer code for the ArdiPi which has a buzzer on pin 22.
// I did eventually find an example for this.
// https://github.com/earlephilhower/arduino-pico/blob/master/libraries/PWMAudio/examples/PlayRaw/PlayRaw.ino
// I could not anywhere else find out how to use the library.
#ifdef ARDUINO_ARCH_RP2040
#include <PWMAudio.h>

const int BUZZER_PIN = 22;

// There is no example of this in the documentation!
PWMAudio buzzer(BUZZER_PIN);
//buzzer.setFrequency(1865);
#endif
//
// A simple logging function that logs the time and the log line.
//
void logIt(const char* toLog) {
    Serial.print(millis());
    Serial.print(':');
    Serial.println(toLog);
}

//
// here we globally store the task ID of our repeating task, we need this to cancel it later.
//
int taskId;

//
// A task can either be a function that takes no parameters and returns void, a class that extends Executable or
// if you want to call with parameters either ExecWithParameter or ExecWith2Parameters
//
void twentySecondJob() {
    logIt("20 seconds one off task");
    logIt("stop 1 second task");
    taskManager.cancelTask(taskId);
    taskManager.scheduleOnce(10, [] {
#ifdef ARDUINO_ARCH_RP2040
        logIt("Running this on the second core of the Pico");
        logIt("with code for the ArdiPi buzzer");
        buzzer.begin(1865);
#endif
        logIt("Ten more seconds done finished.");
#ifdef ARDUINO_ARCH_RP2040
        //delay(1000); // Is there a better way to do this?
        taskManager.yieldForMicros(1000000);
        buzzer.end();
        logIt("buzzer finished.");
#endif
    }, TIME_SECONDS);
}

//
// In setup we prepare our tasks, this is what a usual task manager sketch looks like
//
void setup() {
    Serial.begin(115200);
    delay(5000);
#ifdef ARDUINO_ARCH_RP2040
    Serial.println("Running tests on a Pico");
#endif
    Serial.print("Block size = ");
    Serial.print(DEFAULT_TASK_SIZE);
    Serial.print(", blocks = ");
    Serial.println(DEFAULT_TASK_BLOCKS);

    // schedule a task to run at a fixed rate, every 1000 milliseconds.
    taskId = taskManager.scheduleFixedRate(1000, [] {
        logIt("Fixed rate, every second");
    });

#ifndef ARDUINO_ARCH_RP2040
    // schedule a task to run once in 20 seconds.
    taskManager.scheduleOnce(20, twentySecondJob, TIME_SECONDS);
#endif

    // schedule a task to be executed immediately as a taskManager task.
    taskManager.execute([] {
        logIt("To do as soon as possible");
    });
}


#ifdef ARDUINO_ARCH_RP2040
void setup1()
{
    // schedule a task to run once in 20 seconds.
    taskManager.scheduleOnce(20, twentySecondJob, TIME_SECONDS);
}
#endif
//
// All programs using TaskManager need to call taskManager.runLoop in the loop
// method, and should never use delay(..)
//
void loop() {
    // Optional:
    // If you wanted to go into a low power mode between tasks, you can use taskManager.microsToNextTask() to determine
    // how long you can sleep before the next execution. If you use interrupts, ensure the low power mode supports them.
    //auto delay = taskManager.microsToNextTask();
    //yourLowPowerMode.sleep(delay);

    taskManager.runLoop();
}
