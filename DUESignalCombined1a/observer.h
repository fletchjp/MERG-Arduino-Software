// observer.h
// Code for the Observer Design Pattern
// For now I am using the simpler version of Observer and Subject

#ifndef OBSERVER_H
#define OBSERVER_H

#include <ArduinoSTL.h>

#include <list>
#include <vector>
#include <queue>
#include <map>


// Example code was missing this.
class Observer;
// and this.
class Event { };

enum class NotifyAction { Done, UnRegister };

typedef int EventNo;

class Subject
{
#ifdef USE_FCPP
   typedef vector< Fun0<void> > V;
   V fn_observers;
#endif
public:
    Subject() {}
    virtual ~Subject() {}
#ifdef USE_FCPP
   void attach( Fun0<void> o ) {
      fn_observers.push_back( o );
   }
   void notify() {
      for( V::iterator i=fn_observers.begin(); i!=fn_observers.end(); ++i ) {
         (*i)();
      }
   }
   virtual int get_state() const { return 0; }
#endif
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
//#ifndef USE_FCPP
    void notifyObservers(EventNo const &event_no);
//#endif
    // The function body is moved to the end.
    size_t numberOfObservers() const { return observers.size(); }
private:
    std::vector<Observer*> observers;
};

#ifdef USE_FCPP
// This now includes EventHandler.
class Observer 
{
   //Subject& subject;
public:
   Observer() {}
   void update() {
      //cout << "Update: new state is " << subject.get_state() << endl;
   }
    virtual ~Observer() {}
    // Change to use a reference
    //typedef int N;


//class EventHandler : public Observer
public:
//#ifdef USE_FCPP
//    EventHandler() {}
    typedef std::map<const EventNo,Fun0<int> > EventFun0;
    //typedef int N;
    NotifyAction onNotify(Subject& subject, EventNo const &event_no)
    {
      std::cout << "onNotify called with " << event_no << std::endl;
      //if (dynamic_cast<T*>(this))
      //{
      // Note that the subject is no longer needed.
         auto it = handlers.find(event_no);
         if (it != handlers.end())
         {
            it->second();
         }
      //}
      return NotifyAction::Done;
    }
protected:
    EventFun0 handlers;
};
#endif

//#ifndef USE_FCPP
// Function body moved here where Observer is defined.
    void Subject::notifyObservers(EventNo const &event_no)
    {
      std::cout << "notifyObservers called with " << event_no << std::endl;
      std::vector<Observer*> deadObservers;
      for (Observer* observer : observers)
      {
        observer->onNotify(*this, event_no);
      }
     } 
//#endif

#endif
