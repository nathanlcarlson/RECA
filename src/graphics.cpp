#include <iostream>
#include <memory>
#include <math.h>
#include <GLFW/glfw3.h>
#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"
#include "discrete.hpp"
#include "continuous.hpp"

// Used as an ID
#define A 'A'
#define J 'J'

// Define how to calculate bond energy between two nodes for jja
double a_coupling_energy(node i, node j) {

	return (i.x * j.y - i.y * j.x);

}

// Define how to calculate energy between two nodes for jja
double jja_energy(State* t, int i, int j) {

	return cos( 2 * M_PI * ( (*t)[i] - (*t)[j] - t->bonds(A)->get(i, j) ) );

}

// Ising
double ising_coupling_energy(node i, node j) {

	return 1.0;
}

double ising_energy(State* t, int i, int j) {

	return -1 * (*t)[i] * (*t)[j];
}
// Makes graphic
void display_state(const std::shared_ptr<State>& state, int n, int n_states, int pos) {

	int c = 0;
	double dmax = sqrt(n_states);
	int max = ceil(dmax);
	//std::cout << max/2 <<'\n';
	int posx;
	int posy;
	int f = (n + 4);
	if( max%2 == 0){
		posx = (int)f*(( (pos - 1) % max) - max/2 + 0.5);
		posy = (int)f*(( (pos - 1)/ max) - max/2 + 0.5);
	}
	else{
		posx = ( pos % max) - max/2;
		posy = ( (pos - 1)/ max) - max/2;
	}
	double w = 0.8/(n * max);
	std::vector<double> _rgb;
	_rgb.resize(3);
	for (int i = -n / 2 + posx; i < n / 2 + posx; i++) {
		for (int j = -n / 2 + posy ; j < n / 2 + posy; j++) {

			glBegin(GL_TRIANGLES);
			// Get color
			state->getColor(c, _rgb);

			glColor3f(_rgb[0], _rgb[1], _rgb[2]);
			c++;
			// Make Square
			double x1 = 2 * w * (i ) ;
			double x2 = 2 * w * (i + 1 ) ;
			double y1 = 2 * w * j ;
			double y2 = 2 * w * (j + 1) ;
			glVertex3f(x1, y1, 0.0);
			glVertex3f(x1, y2, 0.0);
			glVertex3f(x2, y1, 0.0);

			glVertex3f(x1, y2, 0.0);
			glVertex3f(x2, y1, 0.0);
			glVertex3f(x2, y2, 0.0);
			glEnd();

		}
	}
}

// Setup escape key, may set up other keys
void specialKeys(GLFWwindow *window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE) {

		exit(0);

	}
}

int main(int argc, char **argv) {

	// The needed parameters
	// TODO	Get parameters from file?
	if (argc != 7) {
    std::cout << "\tgraphics [width] [beta] [percent RECA] [display interval] [state_type] [n repilcas]\n";
    return 1;
  }

	// Seed with time
	seedRand( time(NULL) );

	int w = atoi(argv[1]);
	int n = w * w;
	double beta = atof(argv[2]);
	double freq = atof(argv[3])/100.0;

	// XY+A model
	//  Set-up bonds
	auto bonds_A = std::make_shared<StaticCouplings2D>(A, n, "aperiodic", a_coupling_energy);
	bonds_A->scale_all( 1.0/w );

	//  Define node values
	auto nodes_A = std::make_shared<Continuous>(1.0);
	//  Define state
	auto jja_state = std::make_shared<State>(n, beta, jja_energy, bonds_A, nodes_A);

	// Ising model
	//  Set-up bonds
	auto bonds_J = std::make_shared<StaticCouplings2D>(J, n, "periodic", ising_coupling_energy);


	//  Define node values
	std::vector<double> ising_nodes{-1.0, 1.0};
	auto nodes_J = std::make_shared<Discrete>(ising_nodes);
	//  Define state
	auto ising_state = std::make_shared<State>(n, beta, ising_energy, bonds_J, nodes_J);

	auto my_state = std::make_shared<State>( *jja_state );
	std::string state(argv[5]);
	if ( state == "ising") {
		my_state = ising_state;
	}

	// N Replicas to use
	int n_states = atoi(argv[6]);
	//  Define states
	std::vector<std::shared_ptr<State>>  state_pool;
	for(int i=0; i<n_states; ++i){
		state_pool.push_back(std::make_shared<State>(*my_state));
		state_pool[i]->randomize_all();
	}

	// Choices of algorithms
	auto my_reca = std::make_unique<RECA>( n );
	auto my_metro = std::make_unique<Metropolis>();

	// Begin graphics setup
	GLFWwindow *window;

	// Initialize the library
	if (!glfwInit()) {

		return -1;

	}

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(1000, 1000, "State", NULL, NULL);

	if (!window) {

		glfwTerminate();
		return -1;

	}
	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, specialKeys);

	// Loop until the user closes the window
	// Only render according to interval
	int interval = atoi(argv[4]);
	int count = interval;
	while (!glfwWindowShouldClose(window)) {

		// Step the state forward
		// Evovle mixed
		if(rand0_1() < freq){
			int r1 = randN(n_states);
			int r2 = randN(n_states-1);
			if(r2>=r1) r2++;
			my_reca->evolve_state(state_pool[r1], state_pool[r2]);
		}
		else {
			my_metro->evolve_state(my_state);
			for( auto& replica: state_pool){
				my_metro->evolve_state(replica);
			}
		}

		count--;
		if (count == 0) {
			glClear(GL_COLOR_BUFFER_BIT);
			for(int i = 0; i < n_states; i++){
				display_state(state_pool[i], w, n_states, i);
			}
			glfwSwapBuffers(window);

			// Poll for and process events
			glfwPollEvents();
			count = interval;
		}
		// Poll for and process events
		glfwPollEvents();

	}

	glfwTerminate();

	return 0;
}
