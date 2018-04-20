#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <array>
#include <math.h>
#include <time.h>

#include "utils.hpp"
#include "jja.hpp"
#include "algorithm.hpp"

int main(int argc, char **argv) {
	if (argc != 8) {
		std::cout << "\treca [width] [beta] [percent RECA] [Filename] [Seed] [N states] [CPU Seconds]\n";
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
	//  Define states
	std::vector<JJA*>  state_pool;
	for(int i=0; i<n_states; ++i){
		state_pool.push_back(new JJA(n, beta));
	}

	// Choices of algorithms
	auto reca = std::make_unique<RECA>( n );
	auto metro = std::make_unique<Metropolis>( );

	double t_interval = 0.05;
	double t_max = atof(argv[7]);
	double t = 0;
  	double Et, Mx, My, phi;

	std::vector<double> t_data;
	std::string file_base(argv[4]);
	std::string enr_file = file_base;
	std::ofstream oenr_file;
	oenr_file.open(enr_file);

	std::cout << "Initialization successful, beginning simulation...\n";
	// Gather metrics

	double total_evolve = 0;
	while (total_evolve < t_max) {

		state_pool[randN(n_states)]->metrics(t_data);
		oenr_file << total_evolve << ' ';
		for ( auto& metric: t_data) {
			oenr_file << metric << ' ';
		}
		oenr_file << '\n';

		// Evolve mixed
		double t_evolve = 0;
		clock_t end_evolve;
		clock_t begin_evolve = clock();
		while(t_evolve < t_interval){
			if(rand0_1() < freq){
				int r1 = randN(n_states);
				int r2 = randN(n_states-1);
				if(r2>=r1) r2++;
				reca->evolve_state(state_pool[r1], state_pool[r2]);
			}
			else {
				for(int i=0; i<n_states; ++i){
					metro->evolve_state(state_pool[i]);
				}
			}
			end_evolve = clock();
			t_evolve = double(end_evolve - begin_evolve) / CLOCKS_PER_SEC;
		}
		total_evolve += t_evolve;
	}
	state_pool.clear();
	oenr_file.close();

	return 0;
}
