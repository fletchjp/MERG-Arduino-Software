// Arduino STL Observer
// Bringing in previous code for the Observer Pattern
// Starting from observer2_callable.cpp NOT using the FC++ version.
// There is a lot of explanation there about what I was doing.

// This is based on two papers by Andrei Alexandrescu
// Policy Based Observer (I) and (II).

#include <ArduinoSTL.h>

#include <vector>
#include <algorithm>
#include <memory>
#include <string>

#include "observer.h"

void setup() {
  Serial.begin(115200); 
  printf("Hello World\n");
}

void loop() {
  
}
