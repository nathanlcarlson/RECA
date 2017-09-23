#include <iostream>
#include <math.h>
#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"


double a_coulping_energy(node i, node j);
double j_coulping_energy(node i, node j);
double energy(int i, int j);

// The width of our 2D square and total number of nodes
int n = 1 << 8;
int n_nodes = n * n;
int N_states = 200;
// Physical parameter of system
double beta = 30.0;

// Couplings used to calculate energy
StaticCouplings2D A(n_nodes, a_coulping_energy);
StaticCouplings2D J(n_nodes, j_coulping_energy);
State my_state(n_nodes, beta, energy, N_states);

// Choices of algorithms, use info from couplings
RECA <StaticCouplings2D> *my_reca = new RECA <StaticCouplings2D>( &my_state, &A );
Metropolis <StaticCouplings2D> *my_metro = new Metropolis <StaticCouplings2D>( &my_state, &A );

double a_coulping_energy(node i, node j)
{
	return (i.x * j.y - i.y * j.x) / (double)n;
}

double j_coulping_energy(node i, node j)
{
	return 1.0;
}

double energy(int i, int j)
{
	return J(i, j) * cos(2 * M_PI * (my_state[i] - my_state[j] - A(i, j)));
}

int main(int argc, char **argv)
{
	// Set up couplings
	A.square2D(false);
	J.square2D(false);


	while (true)
	{
		// Step the state forward
   		my_metro->evolve_state();

	}

	return 0;
}
