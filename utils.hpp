#ifndef UTILS_HPP
#define UTILS_HPP

#include <chrono>
#include <random>

std::mt19937 generator (std::chrono::system_clock::now().time_since_epoch().count());
void seedRand( int s = 0 );
double rand0_1();
double randN( int n );
double hueToRGB( double t );
int mod( int a, int b );
struct node{
  double spin;
  int x;
  int y;
  int z;
};
node make_node(int x, int y, int z);
#endif
