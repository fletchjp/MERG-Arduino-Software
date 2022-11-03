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

// Adapting the EventHandler from Observer2
template <typename T>
class EventHandler : public Observer
{
public:

    NotifyAction onNotify(Subject& subject, Event const &event)
    {
      if (dynamic_cast<T*>(this))
      {
         auto it = handlers.find(event);
         if (it != handlers.end())
         {
             (dynamic_cast<T*>(this)->*(it->second))(&subject);
         }
      }
      return NotifyAction::Done;
    }
protected:
    template <typename U>
    U safe_cast(Subject &subject)
    {
        if (dynamic_cast<U>(&subject))
        {
            return (dynamic_cast<U>(&subject));
        } else {
          std::cout <<"Non matching subject" << std::endl;
        }
    }
    std::map<const Event, void (T::*)(Subject *)> handlers;
};



void setup() {
  Serial.begin(115200); 
  printf("Hello Observer5 World\n");
  printf("no working example yet\n");
}

void loop() {
  
}
