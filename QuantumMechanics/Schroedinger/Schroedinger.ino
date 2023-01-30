/// Schroedinger.ino
/// Using the Numerov method.
/// Buffalo dept of physics.

/// I have input this code and modified it in a number of ways.
/// The code now searches for roots in a set of limited ranges of E stored in arrays Elow and Ehigh.
/// Each range finds one root.
/// To find these the value of right_bcond_factor has to alternate between +1 and -1.
/// +1 for cases 0,2,4 etc
/// -1 for cases 1,3,5 etc.
/// This makes the sign of the value of phi_right[N-1] match the sign of the solution
/// as it nears the right boundary. Without this, the solution is not found at all.

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

const int N = 600;
const double accuracy = 0.000001;
int right_bcond_factor = -1;
double x_left = -5;
double x_right = 5;
double h = (x_right - x_left) / N;
double phi_max = 0;
double phi_left_max = 0;

std::vector<double> phi_left(N+1);
std::vector<double> phi_right(N+1);
std::vector<double> phi(N+1);

size_t i_match2 = 0;

double F(double E) {
  ::E = E;
  //Serial << "F (" << _FLOAT(E,4) << ")" << endl;

  size_t i_match = N;
  double x = x_right;
  while (V(x) > E) {
    --i_match;
    x -= h;
    if (i_match == 0) {
      Serial << "No match found " << endl;
      return 10000.;
    } //else {
      //Serial << "Match found at " << i_match << endl;
    //}
  }
  //Serial << "Match found at " << i_match << endl;
  i_match2 = i_match;
  // Numerov from the left
  phi_left[0] = 0;
  phi_left[1] = 1e-10;
  double c = h * h / 12;
  for (size_t i = 1; i <= i_match; i++) {
    x = x_left + i * h;
    phi_left[i+1]  = 2 * (1 - 5 * c *q(x)) * phi_left[i];
    phi_left[i+1] -= ( 1 + c * q(x - h)) * phi_left[i-1];
    phi_left[i+1] /= (1 + c * q(x + h) );
    if (phi_left[i+1] > phi_left_max) phi_left_max = phi_left[i+1];
  }

  // Numerov from the right
  phi[N]   = phi_right[N]   = 0;
  phi[N-1] = phi_right[N-1] = right_bcond_factor*1e-10;
  for (size_t i = N - 1 ; i >= i_match; i--) {
    x = x_right - i * h;
    phi_right[i-1]  = 2 * (1 - 5 * c *q(x)) * phi_right[i];
    phi_right[i-1] -= ( 1 + c * q(x + h)) * phi_right[i+1];
    phi[i-1] = phi_right[i-1] /= (1 + c * q(x - h) );
    if (phi[i-1] > phi_max) phi_max = phi[i-1];
  }
  // rescale phi_left values
  double scale = phi_right[i_match]  / phi_left[i_match];
  //Serial << "scale factor is " << _FLOAT(scale,6) << endl;
  for (size_t i = 0; i <= i_match; i++) {
    phi[i] = phi_left[i] *= scale;
    if (phi[i] > phi_max) phi_max = phi[i];
 }

  static int sign = 1;
  static int nodes = 0;

  int n = 0;
  for (size_t i = 1; i <= i_match; i++) {
    if (phi_left[i-1] * phi_left[i] < 0) ++n;
  }
  // This could be wrong if n has increased by an even number!
  if ( n != nodes) {
    if ( (n-nodes) %2 != 0) 
      sign = -sign;    
    nodes = n;
    
  }
  double right = phi_right[i_match-1] - phi_right[i_match+1];
  double left  = phi_left[i_match-1] - phi_left[i_match+1];
  double denom = ( 2 * h * phi_right[i_match]) ;
  double result = sign * (  right - left ) / denom;
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

const int N_ROOTS = 20;
double roots[N_ROOTS];
double funs[N_ROOTS];

int n_root = 0;


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
    if (n_root < N_ROOTS  && abs(fm) < 0.001) {
      roots[n_root] = xmold;
      funs[n_root] = fm;
      Serial << "   " <<  n_root << "   " << _FLOAT(xmold,4) << " " << _FLOAT(fm,4) << "  " <<  _FLOAT(roots[n_root]/ roots[0],4) << endl;
      n_root++;
      for (size_t i = 1; i < N; i += 25) {
        Serial << i << " ";
        if (i < i_match2) Serial <<  _FLOAT(phi_left[i]*1000,6) << endl;
        else Serial << _FLOAT(phi_right[i]*1000,6) << endl;
      } 
    }
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
   
  Serial << "Eigenvalues for the Schroedinger Equation" << endl;
  Serial << "using the Numerov Algorithm with grid size " << N << endl;
  Serial << "Bisection accuracy is " << _FLOAT(accuracy,6) << endl;
  E_max = 10;
  level = 0;
  E_old = 0;
  E = 0.1;
  Serial << "Level  Energy Function Ratio" << endl;
}

double Elow[] = {0.4, 1.4, 2.4, 3.4, 4.4, 5.4, 6.4, 7.4, 8.4, 9.4};
double Ehigh[] = {0.55, 1.55, 2.55, 3.51, 4.55, 5.55, 6.55, 7.55, 8.55, 9.55};
size_t ilow = 10;
int once = 0;

void loop() {
  // put your main code here, to run repeatedly:
  double dE = 0.5 * (E - E_old);
  Bisect bisect;
  bisect.set_accuracy(accuracy);

  if (once == 0) {
    for (size_t i = 0; i < ilow; i++) {
      Serial << i << " " << Elow[i] << " " << Ehigh[i] << " " << F(Elow[i]) << " " << F(Ehigh[i]) << endl;
      //if (i == 1) {
      //  size_t n = 10;
      //  double delE = (Ehigh[i] - Elow[i]) /n;
      //  for (size_t j = 0; j <= n; j++) {
      //    double Es = Elow[i] + j*delE;
      //    Serial << j << " " << Es << " " << F(Es) << endl;
      //  }
      // }
      // This modified the right boundary condition alternately so that all the values are found.
      right_bcond_factor *= -1;      
      E = bisect.find_root(F,Elow[i],Ehigh[i]);
    }
    once++;
  }
  E_old = E;
  E += dE;
  if ( E < E_max) {
    //do {
    //Serial << "Bisection iteration with Energy " << _FLOAT(E,4) << endl;  
    bisect.find_root(F,E,E + dE);
    //} while ((E < E_max) );
  } else {
    Serial << E_max << " reached" << endl;
    delay (10000);
  }
}
