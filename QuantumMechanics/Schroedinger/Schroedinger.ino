/// Schroedinger.ino
/// Using the Numerov method.
/// Buffalo dept of physics.

#undef F
#include <vector>
#include <Streaming.h>


/// Tridiagonal may come later.
/// The references do not seem to use this method.

/// Define constants as 1 for scaling.
const double hbar = 1;
const double m = 1;
double omega = 1;

double V(double x) {
  return 0.5 * m * omega * omega * x * x;
}

double E;

double q(double x) {
  return 2 * m / (hbar * hbar) * (E - V(x));
}

int N = 100;
double x_left = -5;
double x_right = 5;
double h = (x_right - x_left) / N;
double phi_max = 0;
double phi_left_max = 0;

std::vector<double> phi_left(N+1);
std::vector<double> phi_right(N+1);
std::vector<double> phi(N+1);

double F(double E) {
  ::E = E;
  //Serial << "F (" << _FLOAT(E,4) << ")" << endl;

  int i_match = N;
  double x = x_right;
  while (V(x) > E) {
    --i_match;
    x -= h;
    if (i_match < 0) {
      Serial << "No match found " << endl;
      return 10000.;
    } //else {
      //Serial << "Match found at " << i_match << endl;
    //}
  }
  //Serial << "Match found at " << i_match << endl;

  // Numerov from the left
  phi_left[0] = 0;
  phi_left[1] = 1e-10;
  double c = h * h / 12;
  for (int i = 1; i <= i_match; i++) {
    x = x_left + i * h;
    phi_left[i+1]  = 2 * (1 - 5 * c *q(x)) * phi_left[i];
    phi_left[i+1] -= ( 1 + c * q(x - h)) * phi_left[i-1];
    phi_left[i+1] /= (1 + c * q(x + h) );
    if (phi_left[i+1] > phi_left_max) phi_left_max = phi_left[i+1];
  }

  // Numerov from the right
  phi[N]   = phi_right[N]   = 0;
  phi[N-1] = phi_right[N-1] = 1e-10;
  for (int i = N - 1 ; i >= i_match; i--) {
    x = x_right - i * h;
    phi_right[i-1]  = 2 * (1 - 5 * c *q(x)) * phi_right[i];
    phi_right[i-1] -= ( 1 + c * q(x + h)) * phi_right[i+1];
    phi[i-1] = phi_right[i-1] /= (1 + c * q(x - h) );
    if (phi[i-1] > phi_max) phi_max = phi[i-1];
  }
  // rescale phi_left values
  double scale = phi_right[i_match]  / phi_left[i_match];
  //Serial << "scale factor is " << _FLOAT(scale,6) << endl;
  for (int i = 1; i <= i_match; i++) {
    phi[i] = phi_left[i] *= scale;
    if (phi[i] > phi_max) phi_max = phi[i];
 }

  static int sign = 1;
  static int nodes = 0;

  int n = 0;
  for (int i = 1; i <= i_match; i++) {
    if (phi_left[i-1] * phi_left[i] < 0) ++n;
  }

  if ( n != nodes) {
    nodes = n;
    sign = -sign;
  }
  double right = phi_right[i_match-1] - phi_right[i_match+1];
  double left  = phi_left[i_match-1] - phi_left[i_match+1];
  double denom = ( 2 * h * phi_right[i_match]) ;
  double result =  sign * (  right - left ) / denom;
  //Serial << "maximum phi_left is " << _FLOAT(phi_left_max,6) << endl;
  //Serial << "maximum phi is " << _FLOAT(phi_max,6) << endl;
  //Serial << "(" << _FLOAT(left,4) << " - " << _FLOAT(right,4) << ") / " << _FLOAT(denom,4) << " " << _FLOAT(phi_right[i_match],8) <<  endl;
  return result;
}

void normalize() {
  double norm = 0;
  for (int i = 0; i < N; i++) {
    norm += phi[i] * phi[i];
  } 
  norm /= N;
  norm = sqrt(norm);
  for (int i = 0; i < N; i++) {
    phi[i] /= norm; // phi /= norm; does not compile.
  }
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
  double x2_old = x2;
  double xmold = x1;
  int iter = 0;
  int iter_max = 50;
  double f1 = F(x1);
  double fm; //f2
  // This has seriously reduced the number of function evaluations by reusing values already known.
  do {
    iter = iter + 1;
    //if (iter == 1) f1 = F(x1);  // This saves recomputing f1 when x1 has not changed
    //f2 = F(x2); Note this is not actually used.
    fm = F(xm);
    if (fm*f1 < 0 )
    {
      x2 = xm;
    } else {
      x1 = xm;
      f1 = fm; // and we know f1 as well.
    }
    xmold = xm;
    xm = (x1+x2)/2;
    //Serial << iter << " " << _FLOAT(xmold,4) << " " << _FLOAT(fm,4) << endl;
  } while (abs(xm - xmold) > accuracy && (iter < iter_max) );
  if (iter == iter_max) Serial << iter_max << "reached" << endl;
  if (abs(xm - x2_old ) < accuracy) { 
    //Serial << "top limit reached "  << _FLOAT(x2_old,4) << " " << _FLOAT(fm,4) << endl;
  } else {
    Serial << "root found at " << _FLOAT(xmold,4) << " " << _FLOAT(fm,4) << endl;
  }
  return xmold; 

}

// These have to be globals
double E_max, E_old;
int level;


void setup() {
  // put your setup code here, to run once:
  delay(5000);
  Serial.begin(115200);
  delay(5000);
   
  Serial << "Eignevalues for the Schroedinger Equation" << endl;
  Serial << "using the Numerov Algorithm" << endl;

  E_max = 6;
  level = 0;
  E_old = 0;
  E = 0.1;
  Serial << " Level      Energy " << endl;
}



void loop() {
  // put your main code here, to run repeatedly:
  double dE = 0.5 * (E - E_old);
  E_old = E;
  E += dE;
  if ( E < E_max) {
  do {
  //Serial << "Bisection iteration with Energy " << _FLOAT(E,4) << endl;  
  Bisect bisect;
  bisect.set_accuracy(0.0001);
  bisect.find_root(F,E,E+dE);
  } while (E < E_max);
  }
  Serial << E_max << " reached" << endl;
  delay (10000);
}
