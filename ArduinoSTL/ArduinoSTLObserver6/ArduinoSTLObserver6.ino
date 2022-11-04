// Arduino STL Observer6
// Example from codereview.stackexchange.com
// Also see further down in the comments.
// I am going to make Observer2 the original version
// This is the modification.
// There are some things which are not going to work here:
// std::function and typeinfo.  Also unordered map is not available.
// The problems are in EventHandler.
// A look at method 2 - a workaround.
// Now that Observer5 is working I want to make the Observer code a header file.

#include <ArduinoSTL.h>

#include <list>
#include <vector>
#include <queue>
#include <map>

#include "observer.h"

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
  printf("Hello Observer6 World\n");
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
