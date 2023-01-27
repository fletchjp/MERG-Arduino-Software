// SimpleRoot.ino

// Root finding for the Shroedinger Equation

#undef F
#include <Streaming.h>

double F(double x)
{
  return cos(x) - x;
}

class Bisect {
  double accuracy;
public:
  Bisect() : accuracy(0.01) { }
  void set_accuracy(double acc)  { accuracy = acc; }
  double get_accuracy() const { return accuracy; }
  double find_root(double F(double), double x, double x1);

};

double Bisect::find_root(double F(double), double x1, double x2)
{
  double xm = (x1 + x2)/2;
  double xmold = x1;
  int iter = 0;
  double f1, fm; //f2
  do {
    iter = iter + 1;
    f1 = F(x1);
    //f2 = F(x2); Note this is not actually used.
    fm = F(xm);
    if (fm*f1 < 0 )
    {
      x2 = xm;
    } else {
      x1 = xm;
    }
    xmold = xm;
    xm = (x1+x2)/2;
    Serial << iter << " " << _FLOAT(xmold,4) << " " << _FLOAT(fm,4) << endl;
  } while (abs(xm - xmold) > accuracy);
 
  return xm; 

}

void setup() {
  // put your setup code here, to run once:
  delay(5000);
  Serial.begin(115200);
  delay(5000);

  Serial << "Bisection root finding" << endl;

  Bisect bisect;
  bisect.set_accuracy(0.0001);
  bisect.find_root(F,0,1);

}

void loop() {
  // put your main code here, to run repeatedly:

}
