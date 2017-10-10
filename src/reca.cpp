#include <iostream>
#include <fstream>
#include <memory>
#include <math.h>
#include <cstdlib>
#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"

#define A 'A'
#define J 'J'

typedef StaticCouplings2D Bonds;

double a_coupling_energy(node i, node j) {

	return (i.x * j.y - i.y * j.x);

}

double j_coupling_energy(node i, node j) {

	return 1.0;

}

double energy(State* t, int i, int j) {

	//return t->bonds(J)->get(i, j) * cos( 2 * M_PI * ( (*t)[i] - (*t)[j] - t->bonds(A)->get(i, j) ) );
	return -1*cos( 2 * M_PI * ( (*t)[i] - (*t)[j] - t->bonds(A)->get(i, j) ) );

}

int main(int argc, char **argv) {

	seedRand( time(NULL) );

	int n = 1 << 5;
	int n_nodes = n * n;
	int n_states = 1;
	// Physical parameter of system
	double beta = 30.0;

	Bonds* bonds_A = new Bonds(A, n_nodes, a_coupling_energy);
	Bonds* bonds_J = new Bonds(J, n_nodes, j_coupling_energy);
	State* my_state = new State(n_nodes, n_states, beta, energy, bonds_A, bonds_J);
	State* control_state = new State(n_nodes, n_states, beta, energy, bonds_A, bonds_J);

	// Set up couplings
	bonds_A->square2D(false);
	bonds_A->scale_all( 1.0/n );
	bonds_J->square2D(false);

	// Choices of algorithms
	RECA* my_reca = new RECA( my_state );
	Metropolis* my_metro = new Metropolis( my_state );
	Metropolis* control_metro = new Metropolis( control_state );


	double freq = atof(argv[4])/100.0;

	int min_dt = 0;
	int max_dt = atoi(argv[2]);
	int n_dt = atoi(argv[3]);

	std::ofstream corfile;
	std::ofstream enrfile;
	std::string filename = argv[1];
	corfile.open(filename);
	enrfile.open(filename+".enr");

	corfile << "# Max dt " << max_dt
				  << ", N dt " << n_dt
					<< ", freq RECA " << freq
					<< "\n";

	enrfile << "# Beta " << beta
			<< ", freq RECA " << freq
			<< ", n_nodes " << n_nodes
			<< "\n";

	int t = 0;
	int t_start = 50000;

  double err;
	double control;
	double expr;

	std::vector<double> sum_dts;
	std::vector<int> n_dts;
	n_dts.resize(max_dt);
	sum_dts.resize(max_dt);
	std::fill(sum_dts.begin(), sum_dts.end(), 0);
	std::fill(n_dts.begin(), n_dts.end(), 0);


	// Evolve system without gathering any metrics
	while (t < t_start) {

		// Evolve mixed
		if(rand0_1() < freq){
			my_reca->evolve_state();
		}
		else {
			my_metro->evolve_state();
		}

		// Evolve control state
		control_metro->evolve_state();
		t++;
	}

	// Reset time
	t = 0;
	std::cout << "Begin gathering metrics\n";

	// Gather metrics
	while (true) {

		// Save current t step in states history
		my_state->save();

		// This for loop is a beast
		for(int dt=min_dt; (dt <= t && dt < max_dt); dt++) {

 			double s = 0;

 			for( int j=0; j < n_nodes; j++ ) {

 				double s_t = my_state->get_history(t - dt , j);
 				double s_dt = my_state->get_history(t , j);
 				s += cos(s_t*2*M_PI) * cos(s_dt*2*M_PI) + sin(s_t*2*M_PI) * sin(s_dt*2*M_PI);

 			}

 			n_dts[dt]++;
 			sum_dts[dt] += s;

 			if(n_dts[dt] == n_dt) {

 				min_dt = dt + 1;

 				if( min_dt == max_dt ) {

 					for (auto it=sum_dts.begin(); it!=sum_dts.end(); ++it)
						corfile << *it/(n_dt*n_nodes) << '\n';

					corfile.close();
					enrfile.close();
				 	return 0;

   			}
   		}
   	}

		// Evovle mixed
		if(rand0_1() < freq){
			my_reca->evolve_state();
		}
		else {
			my_metro->evolve_state();
		}

		// Evolve control state
		control_metro->evolve_state();

		// Get energies
		control = control_state->total_energy();
		expr = my_state->total_energy();

		enrfile << control << ' ' << expr <<'\n';

		// Advance time
		t++;

	}

	return 0;
}
