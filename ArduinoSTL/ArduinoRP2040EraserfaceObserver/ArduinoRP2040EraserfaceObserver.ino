//////////////////////////////////////////////////////////////////////
// Arduino RP2040 Eraserface Inherit
// This is an implementation of the example observer2_callable.cpp
//////////////////////////////////////////////////////////////////////
// Applying callable traits and eraserface to the observer pattern.
// I have now built interfaces for subject and observer
// in Andrei Alexandrescu's code.
// I have also built an interface for the subject in FC++ code.
// I am not entirely clear about the choices which work.
//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// These notes come from the earlier files.
// I want to bring together two different strands.

// One is the paper
// FC++: Functional Tools for Object-Oriented Tasks,
// Yannis Smaragdakis and Brian McNamara

// and the other is two papers by Andrei Alexandrescu
//  Policy Based Observer (I) and (II).

// The first strand suggests ways of using FC++ to simplify the second.

// I have already implemented some things from the first in objects.cpp
// but not the observer (page 6) because there was too little information 
// to build an example.

// The other two papers give enough information to put together an 
// example not using FC++ and I hope then to modify that.

// Why do this?  I think it could give a framework for an object based
// calculation framework for chemical engineering calculations.

// This now demonstrates both approaches to the problem in a hybrid way
// which is overkill and needs to be simplified.

// First then, what Andrei gives from (II) p.5 onwards.

#include <exception>
#include <stdexcept>

// Dummies to sort out compilation
namespace boost {

  void throw_exception( std::exception & e ) { }
  void throw_exception(const std::exception & e ) { }
  void throw_exception( std::runtime_error& e) { }
  void throw_exception(const std::runtime_error& e) { }

}
#undef F
#include <memory>
#include <boost_callable_traits.hpp>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <boost_function.hpp>
#include <eraserface.hpp>
#include "fcpp_prelude.h"

#include <Streaming.h>

using std::vector;

DEFINE_ERASERFACE(observer_interface,
    ((Update,      void(int) ))
    ((getLocal,    int() const))
);
// ====================================================================
// Code framework start.
// All of this is his except the headers and std:: in a couple of places.
// ====================================================================
// Code framework start.

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
  Serial << "Logic error called with " << something << endl;
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

// ====================================================================
// Example implementation
// ====================================================================

// I am going to have to invent this, as what is in the paper
// is sketchy, and gives only the typedefs.


template <class E, class Observer>
class AnObserver : public Observer {
  E local_;
public:
  AnObserver() : local_(E(0)) { }
  virtual void Update(E e) { local_ = e; }
  E getLocal() const { return local_; }
};

  typedef BaseSubject<int> MySubject;
  typedef ClosedNotify<BarebonesSubject<int> > MySubjectImpl;
  typedef MySubjectImpl::ObserverID  MySubjectImpl_p;

// I have now all working, only Attach and NotifyAll tested.
DEFINE_ERASERFACE(subject_interface,
      ((Attach,      bool(MySubjectImpl_p,int) ))
      ((Detach,      bool(MySubjectImpl_p,int) ))
      ((Detach,      void(MySubjectImpl_p) ))
      ((NotifyAll,   void(int) ))
);

// ====================================================================
// FC++ way of doing things (Page 6)
// ====================================================================

// This is used to keep the constructor happy.
int nullfun() { return 0; } 

fcpp::Fun0<int> nullfun0 = fcpp::ptr_to_fun(&nullfun);

template <class E>
class FcppSubject : public BarebonesSubject<E> {
  typedef typename  fcpp::Fun0<int> fun_type;
  fcpp::Fun0<int> f_ ;
  typedef std::pair<E, fun_type> fcpp_value_type;
  typedef std::vector<fcpp_value_type> fcpp_container;
  fcpp_container fcpp_data_;
public:
  FcppSubject() : f_(nullfun0) { }
  bool AttachFcpp(fcpp::Fun0<int> f, E e) {  
    f_ = f; fcpp_data_.push_back(std::make_pair(e,f)); return true; 
  }
  bool DetachFcpp(fcpp::Fun0<int> f) {  return false; } // Detach for all
  bool DetachFcpp(fcpp::Fun0<int> f, E e) {  return false; } // Detach for event
  void Notify() { f_(); }
  void NotifyAllFcpp() {    
    for (fcpp_iterator i = fcpp_data_.begin(); i != fcpp_data_.end(); ++i ) {
      (i->second)();
    }
  }
protected:
  typedef typename fcpp_container::iterator fcpp_iterator;
};

// This is the one to develop as it has no knowledge of the other structure
// (No inheritance from BarebonesSubject)
template <class E>
class FcppBareSubject {
  // At the moment this only stores one function to be called.
  // This needs to be made into a vector etc.
  typedef typename  fcpp::Fun0<int> fun_type;
  fcpp::Fun0<int> f_ ;
  typedef std::pair<E, fun_type> fcpp_value_type;
  //typedef std::vector<fun_type> fun_container;
  typedef std::vector<fcpp_value_type> fcpp_container;
  fcpp_container fcpp_data_;
public:
  typedef E Event;
  FcppBareSubject() : f_(nullfun0) { }
  bool AttachFcpp(fcpp::Fun0<int> f, E e) 
  {  
      f_ = f; 
      fcpp_data_.push_back(std::make_pair(e,f)); 
      return true;
  }
  bool DetachFcpp(fcpp::Fun0<int> f) // Detach for all matching f.
  { 
    bool found = false;
    // Note: Needs operator== defined for Fun objects - done for Fun0 only for now.
    if (f == nullfun0 ) return found;
    if (f == f_) { f_ = nullfun0; found = true; }
    for (fcpp_iterator i = fcpp_data_.begin(); i != fcpp_data_.end(); ) {
      if (!(i->second == f) ) ++i;
      else { i = fcpp_data_.erase(i); found = true; }
    } 
    return found; 
  } 
  bool DetachFcpp(fcpp::Fun0<int> f, E e)  // Detach for event
  {  
    const fcpp_value_type v = std::make_pair(e, f);
    const fcpp_iterator i = std::find(fcpp_data_.begin(), fcpp_data_.end(), v);
    if (i == fcpp_data_.end() ) return false;
    fcpp_data_.erase(i);
    return true;
  }
  void Notify() { f_(); }
  void NotifyAllFcpp() {    
    for (fcpp_iterator i = fcpp_data_.begin(); i != fcpp_data_.end(); ++i ) {
      (i->second)();
    }
  }
protected:
  //typedef typename fun_container::iterator  fun_iterator;
  typedef typename fcpp_container::iterator fcpp_iterator;
};

template <class Subject>
class ConcreteObserver {
public:
  typedef typename Subject::Event Event;
private:
  Subject& subject_;
  typedef fcpp::Fun0<int> fun_type;
  // This now collects the functoid for use when detaching.
  fun_type f_;
  bool attached_;
  typedef std::pair<Event, fun_type> fcpp_value_type;
  //typedef std::vector<fun_type> fun_container;
  //typedef std::vector<fcpp_value_type> fcpp_container;
  //fcpp_container fcpp_data_;
public:
  Event event_;
  ConcreteObserver (Subject &s, Event e) : subject_(s), event_(e) {
    // Store the attached function.    
    f_ = fcpp::curry( fcpp::ptr_to_fun( &ConcreteObserver::be_notified), this);
    // This is now stored in a vector. No action is taken to delete it.
    // fcpp_data_.push_back(std::make_pair(e,f_));
    attached_ = s.AttachFcpp( f_, e);
  }
  // At the moment an observer only observes one thing. Do I want more than this?
  // I am starting to develop that possibility. No don't do this.
  // Observers detach themselves by calling the subject to do this.
  bool AttachFcpp(Subject &s, Event e)
  {
    // Detach from current attachment.
    if (attached_) attached_ = !subject_.DetachFcpp(f_);
    //fcpp_data_.push_back(std::make_pair(e,f_));
    attached_ = s.AttachFcpp( f_, e); event_ = e;
    return attached_;
  } 
  bool DetachFcpp(Subject &s) 
  {
    attached_ = !s.DetachFcpp(f_);
    return !attached_;
  }
  bool DetachFcpp(Subject &s, Event e) 
  {
    attached_ = !s.DetachFcpp(f_,e);
    return !attached_;
  }
private:
  // Not to be called except from subject via functoid.
  int be_notified() {
    //event_ = e;
    if (attached_)
      Serial << "New state is " << event_ << endl;
    else 
      Serial << "Error: not attached" << endl;
    return event_;
  }
};

  typedef ClosedNotify<FcppSubject<int> > MyFcppSubjectImpl;
  typedef MyFcppSubjectImpl::Event Event;
  typedef MyFcppSubjectImpl::ObserverID  MyFcppSubjectImpl_p;

  typedef fcpp::Fun0<int> Observer_type;
  typedef FcppSubject<int> Subject_type;

// Now the FC++ version, which has to be different as the members
// are different, although they could be changed.
DEFINE_ERASERFACE(fcpp_subject_interface,
      ((AttachFcpp,      bool(Observer_type,Event) ))
      ((DetachFcpp,      bool(Observer_type,Event) ))
      ((DetachFcpp,      bool(Observer_type) ))
      ((Notify,          void() ))
      ((NotifyAllFcpp,   void() ))
);

typedef  FcppBareSubject<int> BareConcrete;

// I have now all working, only Attach and NotifyAll tested.
DEFINE_ERASERFACE(fcpp_observer_interface,
      ((AttachFcpp,      bool(BareConcrete&,Event) ))
      ((DetachFcpp,      bool(BareConcrete&,Event) ))
      ((DetachFcpp,      bool(BareConcrete&) ))
);

//////////////////////////////////////////////////////////

// This comes from the cdc_multi example
// Helper: non-blocking "delay" alternative.
boolean delay_without_delaying(unsigned long time) {
  // return false if we're still "delaying", true if time ms has passed.
  // this should look a lot like "blink without delay"
  static unsigned long previousmillis = 0;
  unsigned long currentmillis = millis();
  if (currentmillis - previousmillis >= time) {
    previousmillis = currentmillis;
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  unsigned long t1 = millis();
  unsigned long t2;
  while (!Serial && ((millis() - t1) <= 10000));
  t2 = millis() - t1;
  Serial.print("Waited for ");
  Serial.print(t2);
  Serial.println(" millis");
  while (!delay_without_delaying(5000) ) { };
  Serial << "Eraserface observer example" << endl;

  typedef BaseSubject<int> MySubject;
  typedef MySubject::Observer MyObserver;

  typedef ClosedNotify<BarebonesSubject<int> > MySubjectImpl;
  typedef AnObserver<int,MyObserver> anObserver;

  anObserver *ObsId1 = new anObserver;
  anObserver *ObsId2 = new anObserver;
  anObserver *ObsId3 = new anObserver;

  MySubjectImpl subject;

  observer_interface i_obs = *ObsId1;
  subject_interface  i_sub = subject;

  Serial << "============================================" << endl;
  Serial << "Test of Andrei Alexandrescu's Observer Model" << endl;
  Serial << "============================================" << endl;
  Serial << "ObsId1 before attachment " << ObsId1->getLocal() << endl;
  Serial << "ObsId2 before attachment " << ObsId2->getLocal() << endl;
  Serial << "ObsId3 before attachment " << ObsId3->getLocal() << endl;

  Serial << "ObsId1 using i_obs       " << i_obs.getLocal() << endl;

  //  subject.Attach(ObsId1,1);
  i_sub.Attach(ObsId1,1);
  subject.Attach(ObsId1,3);
  subject.Attach(ObsId2,2);
  subject.Attach(ObsId3,1);
  subject.Attach(ObsId3,2);
  Serial << "ObsId1 after attachment " << ObsId1->getLocal() << endl;
  Serial << "ObsId2 after attachment " << ObsId2->getLocal() << endl;
  Serial << "ObsId3 after attachment " << ObsId3->getLocal() << endl;
  Serial << "ObsId1 using i_obs      " << i_obs.getLocal() << endl;
  
  subject.NotifyAll(1);
  Serial << "ObsId1 after NotifyAll(1) " << ObsId1->getLocal() << endl;
  Serial << "ObsId2 after NotifyAll(1) " << ObsId2->getLocal() << endl;
  Serial << "ObsId3 after NotifyAll(1) " << ObsId3->getLocal() << endl;
  Serial << "ObsId1 using i_obs        " << i_obs.getLocal() << endl;
  subject.NotifyAll(2);
  Serial << "ObsId1 after NotifyAll(2) " << ObsId1->getLocal() << endl;
  Serial << "ObsId2 after NotifyAll(2) " << ObsId2->getLocal() << endl;
  Serial << "ObsId3 after NotifyAll(2) " << ObsId3->getLocal() << endl;
  i_sub.NotifyAll(3);
  Serial << "ObsId1 after NotifyAll(3) " << ObsId1->getLocal() << endl;
  Serial << "ObsId1 using i_obs        " << i_obs.getLocal() << endl;
  subject.NotifyAll(2);
  subject.Detach(ObsId3,1);
  subject.NotifyAll(1);
  Serial << "ObsId1 after Detach(ObsId3,1) & NotifyAll(1) " << ObsId1->getLocal() << endl;
  Serial << "ObsId2 after Detach(ObsId3,1) & NotifyAll(1) " << ObsId2->getLocal() << endl;
  Serial << "ObsId3 after Detach(ObsId3,1) & NotifyAll(1) " << ObsId3->getLocal() << endl;

  Serial << "============================================" << endl;
  Serial << "Now the FC++ way of doing things." << endl;
  Serial << "This way the Observer links to the subject " << endl;
  Serial << "which stores a pointer to the functoid to be called." << endl;
  Serial << "============================================" << endl;

  // This could be used to update e.g. all output streams with a common property
  // and also send individual information to different streams and stream types.
  typedef ClosedNotify<FcppSubject<int> > MyFcppSubjectImpl;
  typedef MyFcppSubjectImpl::Event Event;

  MyFcppSubjectImpl fcppsubject2, fcppsubject3;

  //fcpp_subject_interface  fcpp_i_sub = fcppsubject2;
 
  typedef  FcppBareSubject<int> BareConcrete;
  typedef  ConcreteObserver<MyFcppSubjectImpl> Concrete;
  Concrete concrete2(fcppsubject2,2);
  Concrete concrete4(fcppsubject2,4);
  Concrete concrete6(fcppsubject2,6);
  Concrete concrete3(fcppsubject3,3);

  // This FC++ subject has no knowledge of the other stuff.
  BareConcrete bareconcrete10,bareconcrete11;
  // The ConcreteObserver class is identical.
  ConcreteObserver<BareConcrete> barefcppobserver1(bareconcrete10,10);
  ConcreteObserver<BareConcrete> barefcppobserver2(bareconcrete11,11);
  ConcreteObserver<BareConcrete> barefcppobserver3(bareconcrete11,12);
  ConcreteObserver<BareConcrete> barefcppobserver4(bareconcrete11,12);
  ConcreteObserver<BareConcrete> barefcppobserver5(bareconcrete11,13);

  fcpp_subject_interface  fcpp_i_sub = bareconcrete10;
  fcpp_observer_interface fcpp_i_obs = barefcppobserver1;

  Serial << "fcppsubject2.NotifyAllFcpp(); notifies all linked to the object." << endl;
  Serial << "============================================" << endl;
  fcppsubject2.NotifyAllFcpp();
  Serial << "============================================" << endl;
  Serial << "fcppsubject3.Notify(); notifies only the most recent addition." << endl;
  fcppsubject3.Notify();
  Serial << "============================================" << endl;
  Serial << "fcpp_i_sub = bareconcrete10;" << endl;
  Serial << "fcpp_i_sub.Notify()" << endl;
  fcpp_i_sub.Notify();
  Serial << "============================================" << endl;
  Serial << "fcpp_i_sub = bareconcrete11;" << endl;
  fcpp_i_sub = bareconcrete11;
  fcpp_i_sub.NotifyAllFcpp();
  Serial << "============================================" << endl;
  Serial << "Tests of DetachFcpp and AttachFcpp cases." << endl;
  Serial << "============================================" << endl;
  if (barefcppobserver3.DetachFcpp(bareconcrete11) )
    Serial << "barefcppobserver3.DetachFcpp(bareconcrete11) done " << endl;
  bareconcrete11.NotifyAllFcpp();
  if (!barefcppobserver3.DetachFcpp(bareconcrete11) )
    Serial << "barefcppobserver3.DetachFcpp(bareconcrete11) not done (already detached)"
              << endl;
  Serial << "============================================" << endl;
  if (barefcppobserver4.DetachFcpp(bareconcrete11,12))
    Serial << "barefcppobserver4.DetachFcpp(bareconcrete11,12) done " << endl;
  if (!barefcppobserver2.DetachFcpp(bareconcrete11,12))
    Serial << "barefcppobserver2.DetachFcpp(bareconcrete11,12) not done (wrong event)" << endl;
  if (barefcppobserver4.AttachFcpp(bareconcrete11,13))
    Serial << "barefcppobserver4.AttachFcpp(bareconcrete11,13) done " << endl;
  if (barefcppobserver1.AttachFcpp(bareconcrete11,13))
    Serial << "barefcppobserver1.AttachFcpp(bareconcrete11,13) done (delete previous)"
              << endl;
  bareconcrete11.NotifyAllFcpp();
  Serial << "============================================" << endl;
  Serial << "fcpp_i_obs = barefcppobserver1;" << endl;
  fcpp_i_obs = barefcppobserver1;
  if (fcpp_i_obs.DetachFcpp(bareconcrete11,13))
    Serial << "fcpp_i_obs.DetachFcpp(bareconcrete11,13) done" << endl;
  Serial << "=====================================" << endl;
  Serial << "End of testing"  << endl;
  Serial << "=====================================" << endl;
  while (!delay_without_delaying(5000) ) { };
  pinMode(LED_BUILTIN, OUTPUT);

}

int LEDstate = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if (delay_without_delaying(1000)) {
    LEDstate = !LEDstate;
    digitalWrite(LED_BUILTIN, LEDstate);
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    if (LEDstate) {
      Serial.println("Arduino blink ON");
    } else {
      Serial.println("Arduino blink OFF");
    }
  }

}
