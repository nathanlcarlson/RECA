#include <iostream>
#include <fstream>
#include <memory>
#include <math.h>
#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"

// The width of our 2D square and total number of nodes
<<<<<<< HEAD
int n = 1 << 6;
=======
int n = 1 << 5;
>>>>>>> df9d290e0abf494a84077ff5f6d3d53fa56c3fe7
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
void write_file(std::ofstream& outfile) {

	for(int i = 0; i < n_nodes; i++) {
		outfile << cos( (*my_state)[i]*2*M_PI );
	}
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

	int min_dt = 0;
	int max_dt = 100;
	int n_dt = 10;
	int t = 0;
	int t_i;
	double s;
	double s_t;
	double s_dt;
	std::vector<double> sum_dts;
	std::vector<int> n_dts;
	n_dts.resize(max_dt);
	sum_dts.resize(max_dt);
	std::fill(sum_dts.begin(), sum_dts.end(), 0);
	std::fill(n_dts.begin(), n_dts.end(), 0);
	//std::ofstream outfile;
	//outfile.open("test.data");

	while (true) {
		write_state(std::ref(outfile));
		// Step the state forward
		my_state->save();
		for(int dt=min_dt; dt <= t; dt++) {
   			s = 0;
   			for( int j=0; j < n_nodes; j++ ) {
   				s_t = my_state->get_history(t - dt , j);
   				s_dt = my_state->get_history(t , j);
   				s += cos(s_t*2*M_PI) * cos(s_dt*2*M_PI) + sin(s_t*2*M_PI) * sin(s_dt*2*M_PI);
   			}
   			n_dts[dt]++;
   			sum_dts[dt] += s;
   			if(n_dts[dt] == n_dt) {
   				min_dt++;
   				if(min_dt == max_dt) {
   					std::cout << sum_dts.size() <<'\n';
   					for (auto it=sum_dts.begin(); it!=sum_dts.end(); ++it)
						std::cout << ' ' << *it/n_nodes;
				 	std::cout << '\n';
				 	return 0;
   				}
   			}
   		}
   		my_metro->evolve_state();
   		t++;

	}

	return 0;
}
