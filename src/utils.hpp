#ifndef UTILS_HPP
#define UTILS_HPP

#include <random>
#include <iostream>

void seedRand( int s = 0 );
double rand0_1();
double randN( int n );
double hueToRGB( double t );
int mod( int a, int b );
#endif