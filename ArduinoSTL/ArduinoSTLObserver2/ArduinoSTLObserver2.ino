// Arduino STL Observer2
// Example from codereview.stackexchange.com


#include <ArduinoSTL.h>

#include <list>
#include <queue>

class Subject;

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
    void notify(Subject *entity, Event event, Observer* observer)
    {
      if (observer != NULL)
         observer->onNotify(entity,event);
    }
};

void setup() {
  Serial.begin(115200); 
  printf("Hello Observer2 World\n");
}

void loop() {
  
}
