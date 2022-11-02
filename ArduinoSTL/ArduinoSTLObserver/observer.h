// observer.h
// Observer Design Pattern Code.

// This is based on two papers by Andrei Alexandrescu
// Policy Based Observer (I) and (II).

#ifndef OBSERVER_H
#define OBSERVER_H
// ====================================================================
// Code framework start.
// All of this is his except the headers and std:: in a couple of places.
// ====================================================================

#include <vector>
#include <algorithm>
#include <memory>
#include <string>

template <class E>
class BaseSubject {
public:
  typedef E Event;
  struct Observer {
    virtual void Update(Event) = 0;
  };
  typedef Observer* ObserverID;
  enum {
    attachKillsAllIters = 1,
    detachKillsCurrentIter = 1,
    detachKillsAllIters = 1
  };
  virtual bool Attach(ObserverID, Event) = 0;
  virtual bool Detach(ObserverID, Event) = 0;
  virtual void Detach(ObserverID) = 0;
  virtual void NotifyAll(Event) = 0;
  virtual ~BaseSubject() {}
  Observer* ID2Observer(ObserverID id) { return id; }
};

template <class E>
class BarebonesSubject : BaseSubject<E> {
public:
  typedef typename BaseSubject<E>::Event Event;
  typedef typename BaseSubject<E>::Observer Observer;
  typedef typename BaseSubject<E>::ObserverID ObserverID;
  bool Attach(ObserverID id, Event e) {
    value_type v = std::make_pair(e, id);
    if (std::find(data_.begin(), data_.end(), v) != data_.end() ) {
      return false;
    }
    data_.push_back(v);
    return true;
  }

  virtual bool Detach(ObserverID id, Event e) {
    const value_type v = std::make_pair(e, id);
    const iterator i = std::find(data_.begin(), data_.end(), v);
    if (i == data_.end() ) return false;
    data_.erase(i);
    return true;
  }
  virtual void Detach(ObserverID id) {
    for (iterator i = data_.begin(); i != data_.end(); ) {
      if (i->second != id) ++i;
      else i = data_.erase(i);
    }
  }
  virtual void NotifyAll(Event e) {
    for (iterator i = data_.begin(); i != data_.end(); ++i ) {
      if (i->first != e) continue ;
      (i->second)->Update(e);
    }
  }
private:
  typedef std::pair<Event, ObserverID> value_type;
  typedef std::vector<value_type> container;
  container data_;
protected:
  typedef typename container::iterator iterator;
  iterator begin(Event);
  iterator end(Event);
};

// ====================================================================
// Code framework end.
// ====================================================================

// ====================================================================
// Example Policy implementation: 
// ====================================================================

// This is defined to get something going.
// This should not be called in a single threaded application.
void logic_error (const char * something)
{ 
  std::cout << "Logic error called with " << something << std::endl;
}

const char* attach_event = "attach event";
const char* detach_event = "detach event";
const char* detach_all = "detach all";

// ====================================================================
// ClosedNotify rejects calls to Attach and Detach during NotifyAll.
// ====================================================================
template <class Subject>
class ClosedNotify : public Subject {
public: 
  ClosedNotify() : closed_(false) { }
  typedef typename Subject::Event Event;
  typedef typename Subject::Observer Observer;
  typedef typename Subject::ObserverID ObserverID;
  bool Attach(ObserverID id, Event e) {
    if (closed_) /*throw*/ logic_error(attach_event);
    return Subject::Attach(id, e);
  }
  virtual bool Detach(ObserverID id, Event e) {
    if (closed_) /*throw*/ logic_error(detach_event);
    return Subject::Detach(id, e);
  }  
  virtual void Detach(ObserverID id) {
    if (closed_) /*throw*/ logic_error(detach_all);
    /*return*/ Subject::Detach(id);
  }
  virtual void NotifyAll(Event e) {
    closed_ = true;
    struct Local {
      ~Local() { *b_ = false; }
      bool * b_;
    } local = { &closed_ };
    Subject::NotifyAll(e);
  }
private:
  bool closed_;
};


#endif
