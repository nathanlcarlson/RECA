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
void display_state(std::shared_ptr<State> state, int n, double w) {

	glClear(GL_COLOR_BUFFER_BIT);
	int c = 0;

	for (int i = -n / 2; i < n / 2; i++) {

		for (int j = -n / 2; j < n / 2; j++) {

			glBegin(GL_TRIANGLES);
			// Get color
			auto color = state->getColor(c);

			glColor3f(color[0],
					      color[1],
					      color[2]);
			c++;
			// Make Square

			glVertex3f((1 + 2 * i) * w - w, (1 + 2 * j) * w - w, 0.0);
			glVertex3f((1 + 2 * i) * w - w, (1 + 2 * j) * w + w, 0.0);
			glVertex3f((1 + 2 * i) * w + w, (1 + 2 * j) * w - w, 0.0);
			glVertex3f((1 + 2 * i) * w - w, (1 + 2 * j) * w + w, 0.0);
			glVertex3f((1 + 2 * i) * w + w, (1 + 2 * j) * w - w, 0.0);
			glVertex3f((1 + 2 * i) * w + w, (1 + 2 * j) * w + w, 0.0);
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
	if (argc != 5) {
    std::cout << "\tgraphics [width] [beta] [percent RECA] [display interval]\n";
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


	// Choices of algorithms
	auto my_reca = std::make_unique<RECA>( jja_state );
	auto my_metro = std::make_unique<Metropolis>( jja_state );

	// Begin graphics setup
	GLFWwindow *window;

	// Initialize the library
	if (!glfwInit()) {

		return -1;

	}

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(640, 640, "State", NULL, NULL);

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

			display_state(jja_state, w, 0.9/w);
			glfwSwapBuffers(window);
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
