#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <array>
#include <math.h>

#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"
#include "continuous.hpp"

// Used as an ID
#define A 'A'
#define PRI 0
#define ALL 1
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
  if (argc != 8) {
    std::cout << "\treca [width] [beta] [percent RECA] [All or Pri] [Filename] [Seed] [N states]\n";
    return 1;
  }
	seedRand( atoi(argv[6]) );

	// Parameters and outfile


	int w = atoi(argv[1]);
	int n = w * w;
	double beta = atof(argv[2]);
	double freq = atof(argv[3])/100.0;
	int all_or_pri = atoi(argv[4]);
	int n_states = atoi(argv[7]);

	// XY+A model
	//  Set-up bonds
	auto bonds_A = std::make_shared<Bonds>(A, n, a_coupling_energy);
	bonds_A->square2D(false);
	std::vector<std::shared_ptr<Bonds>> bondsA{bonds_A};

	//  Define node values
	auto nodes_A = std::make_shared<Continuous>(1.0);

	//  Define states
	std::vector<std::shared_ptr<State>>  state_pool;
	for(int i=0; i<n_states; ++i){
		state_pool.push_back(std::make_shared<State>(n, beta, jja_energy, bondsA, nodes_A));
	}

	// Choices of algorithms
	auto reca = std::make_unique<RECA>( n );
	auto metro = std::make_unique<Metropolis>( );

	double c = 0;
  double c_max = 1 << 15;
	double c_interval = 50;
  double Et, Mx, My, phi;

	std::vector<std::array<double, 4>> data;
	std::cout << "Initialization successful, beginning simulation...\n";
	// Gather metrics
	std::array<double, 4> c_data;
	while (c < c_max) {

		// Save current t step in states history
		//my_state->save();

		c = (reca->step() + metro->step())/(double)n;
		Et = state_pool[PRI]->total_energy();
		Mx = 0;
		My = 0;
		for(int i = 0; i < n; ++i){
		  phi = 2*M_PI*(*state_pool[PRI])[i];
		  Mx += cos(phi);
		  My += sin(phi);
		}
		c_data[0] = c;
		c_data[1] = Et;
		c_data[2] = Mx;
		c_data[3] = My;
		data.push_back(c_data);
		// Evovle mixed
		while((reca->step() + metro->step())/(double)n - c < c_interval){
			if(rand0_1() < freq){
				reca->evolve_state(state_pool[PRI], state_pool[1+randN(n_states-1)]);
			}
			else {
				if(all_or_pri == PRI || freq == 0){
					metro->evolve_state(state_pool[PRI]);
				}
				else if(all_or_pri == ALL){
					for(int i=0; i<n_states; ++i){
						metro->evolve_state(state_pool[i]);
					}
				}
				else{
					metro->evolve_state(state_pool[PRI]);
				}
			}
		}
	}
	std::string file_base(argv[5]);
	std::string enr_file = file_base;
	std::ofstream oenr_file;
	oenr_file.open(enr_file);
	std::cout << "writing to file\n";
	for(int i=0; i<data.size(); ++i){
		c = data[i][0];
		Et = data[i][1];
		Mx = data[i][2];
		My = data[i][3];

		oenr_file << c << ' '
							<< Et << ' '
							<< Mx << ' '
							<< My << '\n';
	}

	oenr_file.close();
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
