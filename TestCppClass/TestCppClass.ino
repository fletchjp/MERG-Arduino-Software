/// TestCppClass.ino Testing of C++ class rules for copying etc

/// Rule of Zero or Six

/* Default operations for class X
  default constructor  X()
  copy constructor X(const X&)
  copy assignment operator=(const X&)
  move constructor  X(X&&)
  move assignment operator=(X&&)
  destructor ~X()
*/

class X {
  public:

};

class XwithConstructor {
   int i_;
   public:
   XwithConstructor(int i) : i_(i) {}
};

void setup() {
  // put your setup code here, to run once:
  X x;
  //XwithConstructor xwith;  // This does not work as a constructor is declared.
  XwithConstructor xwith(1);
}

void loop() {
  // put your main code here, to run repeatedly:

}
