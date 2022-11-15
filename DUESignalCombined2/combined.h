// combined.h
// This is a version of composite.h which makes all the elements
// also compatible with the observer pattern.
// This is used instead of composite and observer

#ifndef COMBINED_H
#define COMBINED_H

#include "observer.h"

#include <list>
#include <algorithm>
#include <string>

using namespace std;

//template <typename T>
class Component : public Subject, public Observer  {
protected:
   list<Component*> parents_;
public:
    virtual ~Component() { }
   void RegisterParents();
   void SetParent(Component *parent) {
    this->parents_.push_back(parent);
   }
   void RemoveParent(Component *parent) {
    this->parents_.remove(parent);
   }
   list<Component*> GetParents() const {
    return this->parents_;
   }
   bool hasParents() const {
     return this->parents_.size() > 0;
   }
   size_t numberOfParents() const {
     return this->parents_.size();
   }
   string GetParentNames() const {
      string result;
      if (hasParents() ) {
         for (auto c: GetParents() ) {
           result += c->GetName() + " ";       
         }
      } else result = "no parents";
      return result;
   }
   virtual void Add(/* const */ Component* /*component*/) { }
   virtual void Remove(/* const */ Component* /*component*/) { }
   virtual bool IsComposite() const {
    return false;
   }
   virtual string Operation() const { return "nothing"; }
   virtual string GetName() const { return "no name"; }
   virtual void print_() const { };
};


   void Component::RegisterParents() {
//    This will not work at the moment as the parents are not guaranteed to be obervers.
      if (hasParents() ) {
        for (auto parent : parents_) {
           this->registerObserver(*parent);
        }
      }
   }

//template <typename T>
class Composite : public Component {
  const string component_name;
protected:
// Major problem here as I want to store children of type Component<U> where U may be different.
// I have to take out the Eventhandler<T> and do that at a higher level.
// I don't think that is possible.....
// This problem has been overcome using FC++.
  list<const Component*> children_;
public:
  explicit Composite(const string& n) : component_name(n) { }
  void print_() const {
    cout << component_name << " ";
    for (auto c: children_) c->print_(); 
  }
  //template<typename U>
  void Add(/* const */ Component* component) {
    this->children_.push_back(component);
    component->SetParent(this);
  }
  //template<typename U>
  void Remove(/* const */ Component* component) {
    component->RemoveParent(this);
    children_.remove(component);
  }
   virtual bool IsComposite() const {
    return true;
   }
  string GetName() const {
    return component_name;
  }
  string Operation() const {
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

//template <typename T>
class Leaf : public Component {
private:
  const string leaf_name;
public:
  explicit Leaf(const string& n) : leaf_name(n) { }
  string Operation() const override {
    return leaf_name;
  }
  string GetName() const {
    return leaf_name;
  }
  void print_() const override {
    cout << leaf_name << " ";
  }
  
};

#endif
