// Arduino STL Observer5
// Example from codereview.stackexchange.com
// Also see further down in the comments.
// I am going to make Observer2 the original version
// This is the modification.
// There are some things which are not going to work here:
// std::function and typeinfo.  Also unordered map is not available.
// The problems are in EventHandler.
// A look at method 2 - a workaround.


#include <ArduinoSTL.h>

#include <list>
#include <vector>
#include <queue>
#include <map>
// These are not useful.
//#include <functional>
//#include <typeinfo> // not typeindex

// Example code was missing this.
class Subject;
// and this.
class Event { };

enum class NotifyAction { Done, UnRegister };

class Observer 
{
public:
    virtual ~Observer() {}
    // Change to use a reference
    virtual NotifyAction onNotify(Subject& entity, Event const &event) = 0;
private:

};

class Subject
{
public:
    Subject() {}
    virtual ~Subject() {}
    // Return true if observer is added.
    // Changed to a reference.
    bool registerObserver(Observer& observer)
    {
      if (std::find(std::begin(observers),std::end(observers), &observer),std::end(observers))
      {
         return false;
      } else {
        observers.push_back(&observer);
        return true;
      }
    }
    void unregisterObserver(Observer& observer)
    {
       observers.erase(std::remove(std::begin(observers),std::end(observers), &observer),std::end(observers));
    }
    void notifyObservers(Event const &event)
    {
      std::vector<Observer*> deadObservers;
      for (Observer* observer : observers)
      {
          if (observer->onNotify(*this, event) == NotifyAction::UnRegister)
          {
            deadObservers.push_back(observer);
          }
      }
      auto newEnd = std::end(observers);
      for (Observer* dead : deadObservers) 
      {
        newEnd = std::remove(std::begin(observers), newEnd, dead);
      }
      observers.erase(newEnd, std::end(observers));
    }
private:
    std::vector<Observer*> observers;
};

template <typename T>
class EventHandler : public Observer
{
public:
/*
    NotifyAction onNotify(Subject& subject, Event const &event)
    {
       auto find = handlers.find(std::type_index(typeid(event)));
       if (find != handlers.end()) {
         find->second(subject, event);
       }
       return NotifyAction::Done;
    }
*/
private:  
/*
  std::map<std::type_index,std::function<void(Subject&, Event const &)>> handlers;
*/
};

// His example is incomplete and not relevant.

// What is useful is that he can use the map to store different actions by defining events.
// 
// _actions [EVENT_TYPE] = & member function to be called
//
// see the example code.


void setup() {
  Serial.begin(115200); 
  printf("Hello Observer2 World\n");
  printf("no working example yet\n");
}

void loop() {
  
}
