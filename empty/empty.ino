// empty

// An empty FC++ shell for new projects

// This will now work with FC++ as an Arduino library - just include "prelude.h" and it will find it.

#include<ArduinoSTL.h>

#include <iostream>
#include <vector>
#define FCPP_DEBUG
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;
using std::vector;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cout << "========================" << endl;
  cout << "ecoop1b Observer example" << endl;
  cout << "========================" << endl;
  cout << "========================" << endl;

}

void loop() {
  // put your main code here, to run repeatedly:

}
