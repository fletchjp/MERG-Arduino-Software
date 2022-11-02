// Arduino STL Observer
// Bringing in previous code for the Observer Pattern
// Starting from observer2_callable.cpp NOT using the FC++ version.
// There is a lot of explanation there about what I was doing.

// This is based on two papers by Andrei Alexandrescu
// Policy Based Observer (I) and (II).

#include <ArduinoSTL.h>

#include <vector>
#include <algorithm>
#include <memory>
#include <string>

#include "observer.h"

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


void setup() {
  Serial.begin(115200); 
  printf("Hello Observer World\n");

  typedef BaseSubject<int> MySubject;
  typedef MySubject::Observer MyObserver;
  typedef AnObserver<int,MyObserver> anObserver;

  anObserver *ObsId1 = new anObserver;
  anObserver *ObsId2 = new anObserver;
  anObserver *ObsId3 = new anObserver;

  MySubjectImpl subject;

  std::cout << "============================================" << std::endl;
  std::cout << "Test of Andrei Alexandrescu's Observer Model" << std::endl;
  std::cout << "============================================" << std::endl;
  std::cout << "ObsId1 before attachment " << ObsId1->getLocal() << std::endl;
  std::cout << "ObsId2 before attachment " << ObsId2->getLocal() << std::endl;
  std::cout << "ObsId3 before attachment " << ObsId3->getLocal() << std::endl;

  subject.Attach(ObsId1,1);
  subject.Attach(ObsId2,1);
  subject.Attach(ObsId2,2);
  subject.Attach(ObsId3,1);
  subject.Attach(ObsId3,2);
  subject.Attach(ObsId3,3);
  std::cout << "ObsId1 after attachment " << ObsId1->getLocal() << std::endl;
  std::cout << "ObsId2 after attachment " << ObsId2->getLocal() << std::endl;
  std::cout << "ObsId3 after attachment " << ObsId3->getLocal() << std::endl;
  
  subject.NotifyAll(1);
  std::cout << "ObsId1 after NotifyAll(1) " << ObsId1->getLocal() << std::endl;
  std::cout << "ObsId2 after NotifyAll(1) " << ObsId2->getLocal() << std::endl;
  std::cout << "ObsId3 after NotifyAll(1) " << ObsId3->getLocal() << std::endl;
  subject.NotifyAll(2);
  std::cout << "ObsId1 after NotifyAll(2) " << ObsId1->getLocal() << std::endl;
  std::cout << "ObsId2 after NotifyAll(2) " << ObsId2->getLocal() << std::endl;
  std::cout << "ObsId3 after NotifyAll(2) " << ObsId3->getLocal() << std::endl;
  subject.NotifyAll(3);
  std::cout << "ObsId1 after NotifyAll(3) " << ObsId1->getLocal() << std::endl;
  std::cout << "ObsId2 after NotifyAll(3) " << ObsId2->getLocal() << std::endl;
  std::cout << "ObsId3 after NotifyAll(3) " << ObsId3->getLocal() << std::endl;

  delete ObsId1;
  delete ObsId2;
  delete ObsId3;
  
  std::cout << "=====================================" << std::endl;
  std::cout << "End of testing"  << std::endl;
  std::cout << "=====================================" << std::endl;

 

}

void loop() {


}
