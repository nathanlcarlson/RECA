#include "utils.hpp"

void seedRand( int s ) {
  generator.seed (s ? s : std::chrono::system_clock::now().time_since_epoch().count());
}
double rand0_1() {
  return std::uniform_real_distribution<double> (0.0, 1.0) (generator);
}
double randN( int n ) {
  return std::uniform_int_distribution<int> (0, n-1)  (generator);
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

node make_node(int x, int y, int z){
  node new_node;
  new_node.x = x;
  new_node.y = y;
  new_node.z = z;
  return new_node;
}
