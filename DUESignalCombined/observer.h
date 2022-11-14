// observer.h
// Code for the Observer Design Pattern

#ifndef OBSERVER_H
#define OBSERVER_H

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

typedef int EventNo;

class Observer 
{
public:
    virtual ~Observer() {}
    // Change to use a reference
    //typedef int N;
    virtual NotifyAction onNotify(Subject& entity, EventNo const &event_no) = 0;
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
      if (std::find(std::begin(observers),std::end(observers), &observer),std::end(observers) != std::end(observers))
      {
         std::cout << " already registered";
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
    void notifyObservers(EventNo const &event_no)
    {
      std::cout << "notifyObservers called with " << event_no << std::endl;
      std::vector<Observer*> deadObservers;
      for (Observer* observer : observers)
      {
        observer->onNotify(*this, event_no);
        /*  if (observer->onNotify(*this, event_no) == NotifyAction::UnRegister)
          {
            deadObservers.push_back(observer);
          }*/
      }/*
      auto newEnd = std::end(observers);
      for (Observer* dead : deadObservers) 
      {
        newEnd = std::remove(std::begin(observers), newEnd, dead);
      }
      observers.erase(newEnd, std::end(observers)); */
    } 
    size_t numberOfObservers() const { return observers.size(); }
private:
    std::vector<Observer*> observers;
};

// Adapting the EventHandler from Observer2
template <typename T>
class EventHandler : public Observer
{
public:
    typedef void (T::*MFP)(Subject *);
    //typedef int N;
    NotifyAction onNotify(Subject& subject, EventNo const &event_no)
    {
      std::cout << "onNotify called with " << event_no << std::endl;
      //if (dynamic_cast<T*>(this))
      //{
         auto it = handlers.find(event_no);
         if (it != handlers.end())
         {
            MFP item = it->second;
            ((static_cast<T*>(this))->*(item))(&subject);
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
    std::map<EventNo, MFP> handlers;
};

#endif
