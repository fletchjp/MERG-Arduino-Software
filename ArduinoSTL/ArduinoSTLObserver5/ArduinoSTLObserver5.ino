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
  delay(2000);
  Serial.begin(115200); 
  while (!Serial) { }
  delay(2000);
  printf("Hello Observer5 World\n");
  //printf("no working example yet\n");

  Subject aSubject;

  MyClass myClass;

  //EventHandler<MyClass> event_handler;
  std::cout << "aSubject has " << aSubject.numberOfObservers() << " observers" << std::endl;
  std::cout << "Call to registerObserver";
  if (aSubject.registerObserver(myClass)) std::cout << " succeeded" << std::endl;
  else std::cout << " failed" << std::endl;
  std::cout << "aSubject has " << aSubject.numberOfObservers() << " observers" << std::endl;

  aSubject.notifyObservers(TURN_ON);
  
  aSubject.notifyObservers(TURN_OFF);

  std::cout << "End of tests" << std::endl;
  
}

void loop() {
  
}
