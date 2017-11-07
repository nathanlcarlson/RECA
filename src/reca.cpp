#include <iostream>
#include <fstream>
#include <memory>
#include <math.h>

#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"
#include "dbwriter.hpp"

#define A 'A'
#define J 'J'

typedef StaticCouplings2D Bonds;
typedef std::map<std::string, double> Args;

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
  if (argc != 6) {
    std::cout << "\treca [dbhostname] [width] [beta] [percent RECA] [MC steps]\n";
    return 1;
  }
	std::string hostalias(argv[1]);
	int n = atoi(argv[2]);
	int n_nodes = n * n;
	int n_states = 1;
	double beta = atof(argv[3]);
	double freq = atof(argv[4])/100.0;
	int t = 0;
  int t_stop = atoi(argv[5]);
	Args params;
	params["L"] = n_nodes;
	params["W"] = n;
	params["Beta"] = beta;
	params["Freq"] = freq*100;
	params["Steps"] = t_stop;

	Bonds* bonds_A = new Bonds(A, n_nodes, a_coupling_energy);
	Bonds* bonds_J = new Bonds(J, n_nodes, j_coupling_energy);

	State* my_state = new State(n_nodes, n_states, beta, energy, bonds_A, bonds_J);

	// Set up couplings
	bonds_A->square2D(false);
	//bonds_A->scale_all( 1.0/n );
	bonds_J->square2D(false);

	// Choices of algorithms
	RECA* my_reca = new RECA( my_state );
	Metropolis* my_metro = new Metropolis( my_state );


  std::vector<double> expr;
  expr.resize(t_stop);

	// Gather metrics
	while (t < t_stop) {

		// Save current t step in states history
		my_state->save();

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
	DBWriter<Args> db_writer(my_state, hostalias, "test", params);
	db_writer.write_all();
	return 0;
}
