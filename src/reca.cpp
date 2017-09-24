#include <iostream>
#include <memory>
#include <math.h>
#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"


double a_coulping_energy(node i, node j);
double j_coulping_energy(node i, node j);
double energy(int i, int j);

// The width of our 2D square and total number of nodes
int n = 50;
int n_nodes = n * n;
// Physical parameter of system
double beta = 30.0;

typedef StaticCouplings2D Bonds;
// Couplings used to calculate energy
Bonds A(n_nodes, a_coulping_energy);
Bonds J(n_nodes, j_coulping_energy);
State my_state(n_nodes, beta, energy);

// Choices of algorithms, use info from couplings
auto my_reca = std::unique_ptr<RECA<Bonds>>(new RECA<Bonds>( &my_state, &A ));
auto my_metro = std::unique_ptr<Metropolis<Bonds>>(new Metropolis<Bonds>( &my_state, &A ));

double a_coulping_energy(node i, node j) {

	return (i.x * j.y - i.y * j.x) / (double)n;

}

double j_coulping_energy(node i, node j) {

	return 1.0;

}

double energy(int i, int j) {

	return J(i, j) * cos(2 * M_PI * (my_state[i] - my_state[j] - A(i, j)));

}
int main(int argc, char **argv) {

	// Set up couplings
	A.square2D(false);
	J.square2D(false);

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
