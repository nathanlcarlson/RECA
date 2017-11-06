#include <iostream>
#include <fstream>
#include <memory>
#include <math.h>
#include <cstdlib>
#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/gridfs/bucket.hpp>

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
  if (argc != 6) {
    std::cout << "\treca [filename] [width] [beta] [percent RECA] [MC steps]\n";
    return 1;
  }
	int n = atoi(argv[2]);
	int n_nodes = n * n;
	int n_states = 1;
	// Physical parameter of system
	double beta = atof(argv[3]);
	double freq = atof(argv[4])/100.0;
	int t = 0;
  int t_stop = atoi(argv[5]);

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

	std::cout << "Begin gathering metrics...\n";

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

		// Get energy
		expr[t] = my_state->total_energy();


		// Advance time
		t++;

	}
	// Write energy to
	bsoncxx::builder::stream::document energyBSON{};
	energyBSON << "beta" << beta;
	energyBSON << "freq" << freq*100;
	energyBSON << "n_nodes" << n_nodes;
	energyBSON << "t_steps" << t_stop;

	auto enr_array = energyBSON << "energy" << bsoncxx::builder::stream::open_array;
	for(int tau = 0; tau < t_stop; tau++) {

	  enr_array << expr[tau];

	}
	auto after_enr_array = enr_array << bsoncxx::builder::stream::close_array;
	bsoncxx::document::value energy_doc = after_enr_array << bsoncxx::builder::stream::finalize;

	bsoncxx::builder::stream::document metaBSON{};
	metaBSON << "beta" << beta;
	metaBSON << "freq" << freq*100;
	metaBSON << "n_nodes" << n_nodes;
	metaBSON << "t_steps" << t_stop;
	bsoncxx::document::value meta_doc = metaBSON << bsoncxx::builder::stream::finalize;


	mongocxx::instance instance{}; // This should be done only once.
	mongocxx::uri uri("mongo://mongo:27017");
	mongocxx::client conn(uri);
	auto db = conn["test"];
	auto collection = conn["test"]["data"];
	mongocxx::v_noabi::options::gridfs::upload options;
	options.metadata(meta_doc.view());
	auto bucket = db.gridfs_bucket();
	auto uploader = bucket.open_upload_stream( "sample_gridfs_file" ,
	 																					 options
																					 );
	union{
		double m_double;
		std::uint8_t m_byte;
	} conv;
	std::uint8_t bytes[n_nodes*t_stop];
	int i = 0;
	for(int tau = 0; tau < t_stop; tau++) {
		for(int j = 0; j < n_nodes; j++) {

			conv.m_double = 2*M_PI*my_state->get_history(tau, j);
			bytes[i] = conv.m_byte;
			i++;

		}
	}
	uploader.write(bytes, n_nodes*t_stop);
	auto conf_result = uploader.close();
	auto energy_result = collection.insert_one(energy_doc.view());
	//std::cout << bsoncxx::to_json(doc) << std::endl;
	return 0;
}
