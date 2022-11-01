// composite.h
// First try at a composite header to go in a library
// Change vector to list when remove does work.

#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <list>
#include <algorithm>
#include <string>

using namespace std;

class Component {
public:
   virtual void print_() const = 0;
   virtual ~Component() { }  
};

class Composite : public Component {
  list<const Component*> children;
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
    children.remove(component);
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
