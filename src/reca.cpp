#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <array>
#include <math.h>
#include <time.h>

#include "utils.hpp"
#include "couplings.hpp"
#include "jja.hpp"
#include "algorithm.hpp"
#include "continuous.hpp"
#include "bonds.hpp"

// Used as an ID
#define A 'A'
#define PRI 0
#define ALL 1

// Define how to calculate bond energy between
double a_coupling_energy(node i, node j) {

	return (i.x * j.y - i.y * j.x);

}

int main(int argc, char **argv) {
	if (argc != 7) {
		std::cout << "\treca [width] [beta] [percent RECA] [Filename] [Seed] [N states]\n";
		return 1;
	}
	seedRand( atoi(argv[5]) );

	// Parameters
	int w = atoi(argv[1]);
	int n = w * w;
	double beta = atof(argv[2]);
	double freq = atof(argv[3])/100.0;
	std::cout << "Freq: " << freq << '\n';
	int n_states = atoi(argv[6]);

	// XY+A model
	//  Set-up bonds
	auto bonds_A = std::make_shared<StaticCouplings2D>(A, n, "aperiodic", a_coupling_energy);

	//  Define node values
	auto nodes_A = std::make_shared<Continuous>(2*M_PI);

	//  Define states
	std::vector<JJA*>  state_pool;
	for(int i=0; i<n_states; ++i){
		state_pool.push_back(new JJA(n, beta, bonds_A, nodes_A));
	}

	// Choices of algorithms
	auto reca = std::make_unique<RECA>( n );
	auto metro = std::make_unique<Metropolis>( );

	double c = 0;
  	double c_max = 1 << 15;
	double c_interval = 50;
	double t_interval = 0.005;
	double t_max = 30;
	double t = 0;
  	double Et, Mx, My, phi;

	std::vector<std::array<double, 2>> data;
	std::cout << "Initialization successful, beginning simulation...\n";
	// Gather metrics
	std::array<double, 2> c_data;
	clock_t end;
	clock_t begin = clock();
	while (t < t_max) {

		// c = (reca->step() + metro->step())/(double)n;
		Et = state_pool[PRI]->total_energy();
		// Mx = 0;
		// My = 0;
		// for(int i = 0; i < n; ++i){
		//   phi = 2*M_PI*(*state_pool[PRI])[i];
		//   Mx += cos(phi);
		//   My += sin(phi);
		// }
		end = clock();
		t = double(end - begin) / CLOCKS_PER_SEC;
		c_data[0] = t;
		c_data[1] = Et;
		// c_data[2] = Mx;
		// c_data[3] = My;
		data.push_back(c_data);
		// Evovle mixed
		// while((reca->step() + metro->step())/(double)n - c < c_interval){
		double t_evolve = 0;
		clock_t end_evolve;
		clock_t begin_evolve = clock();
		while(t_evolve < t_interval){
			if(rand0_1() < freq){
				int r1 = randN(n_states-1)+1;
				// int r2 = randN(n_states-1);
				// if(r2>=r1) r2++;
				reca->evolve_state(state_pool[r1], state_pool[PRI]);
			}
			else {
				for(int i=0; i<n_states; ++i){
					metro->evolve_state(state_pool[i]);
				}
			}
			end_evolve = clock();
			t_evolve = double(end_evolve - begin_evolve) / CLOCKS_PER_SEC;
		}
	}
	std::string file_base(argv[4]);
	std::string enr_file = file_base;
	std::ofstream oenr_file;
	oenr_file.open(enr_file);
	std::cout << "writing to file\n";
	for(int i=0; i<data.size(); ++i){
		c = data[i][0];
		Et = data[i][1];
		// Mx = data[i][2];
		// My = data[i][3];
		oenr_file << c << ' ' << Et << '\n';
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
