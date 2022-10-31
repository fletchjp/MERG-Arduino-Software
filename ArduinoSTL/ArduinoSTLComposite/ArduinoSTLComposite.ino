// Arduino STL Composite

#include <ArduinoSTL.h>

#include <vector>

using namespace std;

vector<int> trial;

void setup() {
  Serial.begin(115200); 
  printf("STL Vector\n");
  for (int i = 0; i < 5; i++) trial.push_back(i);
  // range-based for loop
  for (auto i : trial) cout << i << " ";
  cout << endl;
}

void loop() {
  
}
