#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <math.h>

#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"
#include "node.hpp"

// Used as an ID
#define A 'A'

// May use different type of bonds, os typedef
typedef StaticCouplings2D Bonds;

// Define how to calculate bond energy between
double a_coupling_energy(node i, node j) {

	return (i.x * j.y - i.y * j.x);

}

// Define how to calculate energy between two nodes
double jja_energy(State* s, int i, int j) {

	return -1*cos( 2 * M_PI * ( (*s)[i] - (*s)[j] - s->bonds(A)->get(i, j) ) );

}

int main(int argc, char **argv) {


  if (argc != 5) {
    std::cout << "\treca [width] [beta] [percent RECA] [MC steps]\n";
    return 1;
  }

	seedRand( time(NULL) );
	//
	int w = atoi(argv[1]);
	int n = w * w;
	double beta = atof(argv[2]);
	double freq = atof(argv[3])/100.0;
	int t = 0;
  int t_stop = atoi(argv[4]);


	// XY+A model
	//  Set-up bonds
	auto bonds_A = std::make_shared<Bonds>(A, n, a_coupling_energy);
	bonds_A->square2D(false);
	std::vector<std::shared_ptr<Bonds>> bondsA{bonds_A};
	//  Define node values
	auto nodes_A = std::make_shared<Node>(1.0);
	//  Define state
	auto jja_state = std::make_shared<State>(n, beta, jja_energy, bondsA, nodes_A);


	// Choices of algorithms
	auto my_reca = std::make_unique<RECA>( jja_state );
	auto my_metro = std::make_unique<Metropolis>( jja_state );
	// Gather metrics
	while (t < t_stop) {

		// Save current t step in states history
		//my_state->save();

		// Evovle mixed
		if(rand0_1() < freq){
			my_reca->evolve_state();
		}
		else {
			my_metro->evolve_state();
		}

		// Advance time
		t++;

	}
	// TODO Replace this system for saving data with a more modular solution

	// std::vector<double> mean_sub;
	// double avg_energy = calc_avg_energy(my_state->energy_history(), t_stop);
	// for(const auto &energy : my_state->energy_history()) {
	// 	mean_sub.push_back(energy - avg_energy);
	// }
	//
	// autocorrelation(mean_sub);

	return 0;
}
