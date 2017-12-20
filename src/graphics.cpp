#include <iostream>
#include <memory>
#include <math.h>
#include <GLFW/glfw3.h>
#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"
#include "node.hpp"

// Used as an ID
#define A 'A'
#define J 'J'

// May use different type of bonds, os typedef
typedef StaticCouplings2D Bonds;

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
void display_state(std::shared_ptr<State> state, int n, int n_states, int pos) {

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
	for (int i = -n / 2 + posx; i < n / 2 + posx; i++) {
		for (int j = -n / 2 + posy ; j < n / 2 + posy; j++) {

			glBegin(GL_TRIANGLES);
			// Get color
			auto color = state->getColor(c);

			glColor3f(color[0],
					      color[1],
					      color[2]);
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
	auto bonds_A = std::make_shared<Bonds>(A, n, a_coupling_energy);
	bonds_A->square2D(false);
	bonds_A->scale_all( 1.0/w );
	std::vector<std::shared_ptr<Bonds>> bondsA{bonds_A};
	//  Define node values
	auto nodes_A = std::make_shared<Node>(1.0);
	//  Define state
	auto jja_state = std::make_shared<State>(n, beta, jja_energy, bondsA, nodes_A);

	// Ising model
	//  Set-up bonds
	auto bonds_J = std::make_shared<Bonds>(J, n, ising_coupling_energy);
	bonds_J->square2D(true);
	std::vector<std::shared_ptr<Bonds>> bondsJ{bonds_J};
	//  Define node values
	std::vector<double> ising_nodes{-1.0, 1.0};
	auto nodes_J = std::make_shared<Node>(ising_nodes);
	//  Define state
	auto ising_state = std::make_shared<State>(n, beta, ising_energy, bondsJ, nodes_J);

	auto my_state = std::make_shared<State>( *jja_state );
	std::string state(argv[5]);
	if ( state == "ising") {
		my_state = ising_state;
	}

	// N Replicas to use
	int n_replicas = atoi(argv[6]);

	// Choices of algorithms
	auto my_reca = std::make_unique<RECA>( my_state, n_replicas );
	auto my_metro = std::make_unique<Metropolis>( my_state );

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
	int n_steps = 0;
	while (!glfwWindowShouldClose(window)) {

		// Step the state forward
		// Evovle mixed
		if(rand0_1() < freq){
			my_reca->evolve_state();
		}
		else {
			my_metro->evolve_state();
		}
		count--;
		n_steps++;
		if (count == 0) {
			glClear(GL_COLOR_BUFFER_BIT);
			display_state(my_state, w, n_replicas+1, n_replicas+1);
			for(int i = 0; i < n_replicas; i++){
				display_state(my_reca->get_replica(i), w, n_replicas+1, i+1);
			}
			glfwSwapBuffers(window);
			// Poll for and process events
			glfwPollEvents();
			count = interval;
			std::cout << n_steps << '\n';
			std::cout << "Press enter to continue\n";
      getchar();
		}
		// Poll for and process events
		glfwPollEvents();

	}

	glfwTerminate();

	return 0;
}
