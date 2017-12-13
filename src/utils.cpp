#include "utils.hpp"

std::mt19937 generator;

void seedRand( int s ) {

  generator.seed(s);

}
double rand0_1() {

  return std::uniform_real_distribution<double>(0.0, 1.0)(generator);

}
double randN( int n ) {

  return std::uniform_int_distribution<int>(0, n-1)(generator);

}
double hueToRGB( double t ) {

  if(t < 0.0) t += 1.0;
  if(t > 1.0) t -= 1.0;
  if(t < 0.1666) return 6.0 * t;
  if(t < 0.5) return 1.0;
  if(t < 0.6666) return (0.6666 - t) * 6.0;
  return 0.0;

}
int mod( int a, int b ) {

  int c = a % b;
  if (c < 0) c += b;
  return c;

}
std::vector<double>& autocorrelation(std::vector<double>& _s) {

  int N = _s.size();
  int fftN = 2*N;
  fftw_complex in[fftN], res[fftN]; /* double [2] */
  fftw_plan p, q;
  int i;

  p = fftw_plan_dft_1d(fftN, in, in, FFTW_FORWARD, FFTW_ESTIMATE);
  q = fftw_plan_dft_1d(fftN, res, res, FFTW_BACKWARD, FFTW_ESTIMATE);

  for (i = 0; i < N; i++) {
    in[i][0] = _s[i];
    in[i][1] = 0;
  }
  for (i = N; i < fftN; i++) {
    in[i][0] = 0;
    in[i][1] = 0;
  }

  fftw_execute(p);

  for (i = 0; i < fftN; i++) {

    res[i][0] = (in[i][0]*in[i][0] + in[i][1]*in[i][1]);
    res[i][1] = 0;

  }

  fftw_execute(q);

  for (i = 0; i < N; i++) {
    _s[i] = res[i][0]/fftN;
  }

  fftw_destroy_plan(p);
  fftw_destroy_plan(q);
  fftw_cleanup();
  return _s;
}

node make_node(int x, int y, int z) {

  node new_node;
  new_node.x = x;
  new_node.y = y;
  new_node.z = z;
  return new_node;

}
