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
  

  return 0; // for now so that it compiles.
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
