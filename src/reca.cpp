#include <iostream>
#include <memory>
#include <math.h>
#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"

// The width of our 2D square and total number of nodes
int n = 1 << 7;
int n_nodes = n * n;
// Physical parameter of system
double beta = 30.0;

typedef StaticCouplings2D Bonds;
// Couplings used to calculate energy
Bonds *A = NULL;
Bonds *J = NULL;
State *my_state = NULL;

double a_coupling_energy(node i, node j) {

	return (i.x * j.y - i.y * j.x) / (double)n;

}

double j_coupling_energy(node i, node j) {

	return 1.0;

}

double energy(int i, int j) {

	return (*J)(i, j) * cos( 2 * M_PI * ( (*my_state)[i] - (*my_state)[j] - (*A)(i, j) ) );

}

int main(int argc, char **argv) {

	A = new Bonds(n_nodes, a_coupling_energy);
	J = new Bonds(n_nodes, j_coupling_energy);
	my_state = new State(n_nodes, beta, energy);

	// Set up couplings
	A->square2D(false);
	J->square2D(false);

	// Choices of algorithms
	auto my_reca = std::unique_ptr<RECA<Bonds>>(new RECA<Bonds>( my_state, A ));
	auto my_metro = std::unique_ptr<Metropolis<Bonds>>(new Metropolis<Bonds>( my_state, A ));

	int interval = 1000;
	int count = interval;

	while (true) {

		// Step the state forward
   	my_reca->evolve_state();
		count--;
		if(count == 0) {

			count = interval;
			std::cout << my_reca->total_energy() << '\n';

		}
	}

	return 0;
}
