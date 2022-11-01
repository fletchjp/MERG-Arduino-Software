// composite.h
// First try at a composite header to go in a library
// Change vector to list when remove does work.
// Adding some things from refactoring.guru design

// This allows the setting of information on one parent.
// There are other differences.
// I have kept the print_ operations although this also allows for the return of a string.


#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <list>
#include <algorithm>
#include <string>

using namespace std;

class Component {
protected:
   Component *parent_;
public:
   virtual ~Component() { }
   void SetParent(Component *parent) {
    this->parent_ = parent;
   }
   Component *GetParent() const {
    return this->parent_;
   }
   virtual void Add(/* const */ Component* component) { }
   virtual void Remove(/* const */ Component* component) { }
   virtual bool IsComposite() const {
    return false;
   }
   virtual string Operation() const = 0;
   virtual void print_() const = 0;
};

class Composite : public Component {
  const string component_name;
protected:
  list<const Component*> children_;
public:
  explicit Composite(const string& n) : component_name(n) { }
  void print_() const override {
    cout << component_name << " ";
    for (auto c: children_) c->print_(); 
  }
  void Add(/* const */ Component* component) {
    this->children_.push_back(component);
    component->SetParent(this);
  }
  void Remove(/* const */ Component* component) {
    children_.remove(component);
    component->SetParent(nullptr);
  }
   virtual bool IsComposite() const {
    return true;
   }
  string Operation() const override {
    string result;
    for (const Component *c: children_) {
      if (c == children_.back() ) {
        result += c->Operation();
      } else {
        result += c->Operation() + "+";       
      }
    }
    return "Branch(" + result + ")";
  }
};

class Leaf : public Component {
private:
  const string leaf_name;
public:
  explicit Leaf(const string& n) : leaf_name(n) { }
  string Operation() const override {
    return leaf_name;
  }
  void print_() const override {
    cout << leaf_name << " ";
  }
};

#endif
