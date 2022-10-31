// Arduino STL Composite
// Modified from modernescpp.com example.

#include <ArduinoSTL.h>

#include <vector>
#include <algorithm>
#include <string>

using namespace std;

vector<int> trial;

class Component {
public:
   virtual void print_() const = 0;
   virtual ~Component() { }  
};

class Composite : public Component {
  vector<const Component*> children;
  const string component_name;
public:
  explicit Composite(const string& n) : component_name(n) { }
  void print_() const override {
    cout << component_name << " ";
    for (auto c: children) c->print_(); 
  }
  void add(const Component* component) {
    children.push_back(component);
  }
  void remove(const Component* component) {
    // erase(children, component); Not available in C++11
    // There is children.erase(location) which needs to know where the component is.
    for (int i = 0; i < children.size(); i++) {
      if (children.at(i) == component) { children.erase(children.begin() + i); break; }
    } 
  }
};

class Leaf : public Component {
private:
  const string leaf_name;
public:
  explicit Leaf(const string& n) : leaf_name(n) { }
  void print_() const override {
    cout << leaf_name << " ";
  }
};

void setup() {
  Serial.begin(115200); 
  printf("STL Composite Pattern\n");
  for (int i = 0; i < 5; i++) trial.push_back(i);
  // range-based for loop
  for (auto i : trial) cout << i << " ";
  cout << endl;
  cout << "Test of Composite Pattern" << endl;

  Composite composite("composite");
  Composite composite1("composite1");
  composite.print_();
  Leaf leaf1("leaf1");
  Leaf leaf2("leaf2");
  Leaf leaf3("leaf3");
  leaf1.print_();
  cout << endl;
  composite1.add(&leaf1);
  composite1.add(&leaf2);

  composite.add(&composite1);
  composite.add(&leaf3);

  composite1.print_();
  cout << endl;

  composite.print_();
  cout << endl;
  
}

void loop() {
  
}
