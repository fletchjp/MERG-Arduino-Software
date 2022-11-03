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
    typedef int N;
    NotifyAction onNotify(Subject& subject, N const &event_no)
    {
      //if (dynamic_cast<T*>(this))
      //{
         auto it = handlers.find(event_no);
         if (it != handlers.end())
         {
            (it->second)(&subject);
         }
      //}
      return NotifyAction::Done;
    }
protected:
   /*
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
    */
    std::map<N, void (T::*)(Subject *)> handlers;
};


#define TURN_ON  0
#define TURN_OFF 1

// Now the usage is from Observer2.
// Avoiding the typeid by having each class be its own handler.  
class MyClass : public EventHandler<MyClass>
{
public:
   MyClass() {
      handlers[TURN_ON] = &MyClass::turnON; 
      handlers[TURN_OFF] = &MyClass::turnOFF; 
   }
   //virtual ~MyClass() { }
/*
   NotifyAction onNotify(Subject& subject, int const &event_no)
   {
         return NotifyAction::Done;
   }
   */
private:
   void turnON(Subject *subject)
   {
      std::cout << "Turn on" << std::endl;
   }
   void turnOFF(Subject *subject)
   {
      std::cout << "Turn off" << std::endl;
   }
};

void setup() {
  Serial.begin(115200); 
  printf("Hello Observer5 World\n");
  //printf("no working example yet\n");

  Subject aSubject;

  //MyClass myClass;

  EventHandler<MyClass> event_handler;
  
}

void loop() {
  
}
