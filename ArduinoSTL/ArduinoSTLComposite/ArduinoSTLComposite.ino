// Arduino STL Composite

#include <ArduinoSTL.h>

#include <vector>
#include <algorithm>
#include <string>

using namespace std;

vector<int> trial;

class Component {
public:
   virtual void print() const = 0;
   virtual ~Component() { }  
};

class Composite : public Component {
  vector<const Component*> children;
  const string& component_name;
public:
  explicit Composite(const string& n) : component_name(n) { }
  void print() const override {
    cout << component_name << " ";
    for (auto c: children) c->print();
  }
  void add(const Component* component) {
    children.push_back(component);
  }
  void remove(const Component* component) {
    children.erase(component);
  }
};

void setup() {
  Serial.begin(115200); 
  printf("STL Composite Pattern\n");
  for (int i = 0; i < 5; i++) trial.push_back(i);
  // range-based for loop
  for (auto i : trial) cout << i << " ";
  cout << endl;
}

void loop() {
  
}
