/// Schroedinger.ino
/// Using the Numerov method.
/// Buffalo dept of physics.

#undef F
#include <vector>

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

int N = 500;
double x_left = -5;
double x_right = 5;
double h = (x_right - x_left) / N;

std::vector<double> phi_left(N+1);
std::vector<double> phi_right(N+1);
std::vector<double> phi(N+1);

double F(double E) {
  ::E = E;

  int i_match = N;
  double x = x_right;
  while (V(x) > E) {
    --i_match;
    x -= h;
    if (i_match < 0) {
      //Failure error message needed.
    }
  }

  // Numerov from the left
  phi_left[0] = 0;
  phi_left[1] = 1e-10;
  double c = h * h / 12;
  for (int i = 1; i <= i_match; i++) {
    x = x_left + i * h;
    phi_left[i+1]  = 2 * (1 - 5 * c *q(x)) * phi_left[i];
    phi_left[i+1] -= ( 1 + c * q(x - h)) * phi_left[i-1];
    phi_left[i+1] /= (1 + c * q(x + h) );
  }

  // Numerov from the right
  phi[N]   = phi_right[N]   = 0;
  phi[N-1] = phi_right[N-1] = 1e-10;
  for (int i = N; i >= i_match; i--) {
    x = x_right - i * h;
    phi_right[i-1]  = 2 * (1 - 5 * c *q(x)) * phi_right[i];
    phi_right[i-1] -= ( 1 + c * q(x + h)) * phi_right[i+1];
    phi[i-1] = phi_right[i-1] /= (1 + c * q(x - h) );
  }
  // rescale phi_left values
  double scale = phi_right[i_match]  / phi_left[i_match];
  for (int i = 1; i <= i_match; i++) {
    phi[i] = phi_left[i] *= scale;
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

  return sign * (  phi_right[i_match-1] - phi_right[i_match+1]
                  - phi_left[i_match-1] + phi_left[i_match+1] )
         / ( 2 * h * phi_right[i_match]) ; // for now so that it compiles.
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

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
