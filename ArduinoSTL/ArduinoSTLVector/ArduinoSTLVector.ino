// Arduino STL Vector

#include <ArduinoSTL.h>

#include <vector>

using namespace std;

vector<int> trial;

void setup() {
  Serial.begin(115200); 
  printf("STL Vector\n");
  for (int i = 0; i < 5; i++) trial.push_back(i);
}

void loop() {
  
}
