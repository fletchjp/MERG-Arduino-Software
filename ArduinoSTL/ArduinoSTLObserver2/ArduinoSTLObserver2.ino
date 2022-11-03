// Arduino STL Observer2
// Example from codereview.stackexchange.com
// Also see further down in the comments.
// I am going to make this one the original version
// There will be another (Observer3) for a modification.


#include <ArduinoSTL.h>

#include <list>
#include <queue>
#include <map>

// Example code was missing this.
class Subject;
// and this.
class Event { };

class Observer 
{
public:
    virtual ~Observer() {}
    virtual void onNotify(Subject *entity, Event event) = 0;
private:

};

class Subject
{
private:
    std::list<Observer*> _observers;
    std::queue<std::list<Observer*>::iterator> _eraseQueue;
public:
    Subject() {}
    virtual ~Subject() {}
    // Return true if observer is added.
    bool addObserver(Observer* observer)
    {
      if (std::find(_observers.begin(), _observers.end(), observer) ==  _observers.end())
      {
        _observers.push_back(observer);
        return true;
      } else return false;
    }
    void removeObserver(Observer* observer)
    {
      std::list<Observer*>::iterator it = std::find(_observers.begin(), _observers.end(), observer);
      if (it != _observers.end())
      {
        *it = NULL;
        _eraseQueue.push(it);
      }
    }
protected:
    void notify(Subject *entity, Event event)
    {
      for (std::list<Observer*>::iterator it = _observers.begin(); it != _observers.end(); it++)
      {
       if (*it != NULL)
         (*it)->onNotify(entity,event);
      }
      while (!_eraseQueue.empty()) {
        _observers.erase(_eraseQueue.front());
        _eraseQueue.pop();
      }
    }
    void notify(Subject *entity, Event event, Observer* observer)
    {
      if (observer != NULL)
         observer->onNotify(entity,event);
    }
    
};

template <typename T>
class EventHandler : public Observer
{
public:
    virtual ~EventHandler() {}
    virtual void onNotify(Subject *entity, Event event)
    {
      if (dynamic_cast<T*>(this))
      {
         auto it = _actions.find(event);
         if (it != _actions.end())
         {
             (dynamic_cast<T*>(this)->*(it->second))(entity);
         }
      }
    }
protected:
    template <typename U>
    U safe_cast(Subject* entity)
    {
        if (dynamic_cast<U>(entity))
        {
            return (dynamic_cast<U>(entity));
        } else {
          std::cout <<"Non matching entity" << std::endl;
        }
    }
    std::map<const Event, void (T::*)(Subject *)> _actions;
};

void setup() {
  Serial.begin(115200); 
  printf("Hello Observer2 World\n");
}

void loop() {
  
}
