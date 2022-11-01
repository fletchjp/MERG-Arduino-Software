// composite.h
// First try at a composite header to go in a library

#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <vector>
#include <algorithm>
#include <string>

using namespace std;

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

#endif
