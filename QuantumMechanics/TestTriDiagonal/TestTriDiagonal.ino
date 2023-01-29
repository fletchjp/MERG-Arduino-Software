/// TestTriDiagonal
/// Experiment for Numerov method.
/// The references do not seem to use this method.

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

// Adapting wikipedia tridiagonal matrix
const size_t n = 10;

typedef struct {
  double A[n+2];
  double B[n+2];
  double C[n+2];
  double D[n+2];
} COEFFICIENTS;


void setup() {
  // put your setup code here, to run once:
  X x;
  X xx(x);
  //XwithConstructor xwith;  // This does not work as a constructor is declared.
  XwithConstructor xwith(1);
}

void loop() {
  // put your main code here, to run repeatedly:

}
