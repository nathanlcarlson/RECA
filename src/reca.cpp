#include <iostream>
#include <fstream>
#include <memory>
#include <math.h>
#include <cstdlib>
#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"

// The width of our 2D square and total number of nodes

int n = 1 << 5;
int n_nodes = n * n;
// Physical parameter of system
double beta = 30.0;

typedef StaticCouplings2D Bonds;
// Couplings used to calculate energy
Bonds *A = NULL;
Bonds *J = NULL;
State *my_state = NULL;
State *control_state = NULL;
double a_coupling_energy(node i, node j) {

	return (i.x * j.y - i.y * j.x);

}

double j_coupling_energy(node i, node j) {

	return 1.0;

}

double energy(int i, int j) {

	return -1*(*J)(i, j) * cos( 2 * M_PI * ( (*my_state)[i] - (*my_state)[j] - (*A)(i, j) ) );

}

double control_energy(int i, int j) {

	return -1*(*J)(i, j) * cos( 2 * M_PI * ( (*control_state)[i] - (*control_state)[j] - (*A)(i, j) ) );

}

int main(int argc, char **argv) {

	seedRand( time(NULL) );

	A = new Bonds(n_nodes, a_coupling_energy);
	J = new Bonds(n_nodes, j_coupling_energy);
	my_state = new State(n_nodes, beta, energy);
	control_state = new State(n_nodes, beta, control_energy);

	// Set up couplings
	A->square2D(false);
	J->square2D(false);

	// Choices of algorithms
	auto my_reca = std::unique_ptr<RECA<Bonds>>(new RECA<Bonds>( my_state, A ));
	auto my_metro = std::unique_ptr<Metropolis<Bonds>>(new Metropolis<Bonds>( my_state, A ));
	auto control_metro = std::unique_ptr<Metropolis<Bonds>>(new Metropolis<Bonds>( control_state, A ));


	double freq = atof(argv[4])/100.0;

	int min_dt = 0;
	int max_dt = atoi(argv[2]);
	int n_dt = atoi(argv[3]);

	std::ofstream corfile;
	std::ofstream enrfile;
	std::string filename = argv[1];
	corfile.open(filename);
	enrfile.open(filename+".enr");

	corfile << "#Max dt " << max_dt
				  << ", N dt " << n_dt
					<< ", freq RECA " << freq
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
		control = control_metro->total_energy();
		expr = my_reca->total_energy();

		enrfile << control << ' ' << expr <<'\n';

		// Advance time
		t++;

	}

	return 0;
}
