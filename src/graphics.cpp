#include <iostream>
#include <memory>
#include <math.h>
#include <GLFW/glfw3.h>
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

	return t->bonds(J)->get(i, j) * cos( 2 * M_PI * ( (*t)[i] - (*t)[j] - t->bonds(A)->get(i, j) ) );

}

void display_state(State* state, int n, int w) {

	glClear(GL_COLOR_BUFFER_BIT);
	int c = 0;
	for (int i = -n / 2; i < n / 2; i++) {

		for (int j = -n / 2; j < n / 2; j++) {

			glBegin(GL_TRIANGLES);
			// Get color
			//std::cout << (*state)[c] << '\n';
			glColor3f(hueToRGB((*state)[c] + 0.3333),
			          hueToRGB((*state)[c]),
			          hueToRGB((*state)[c] - 0.3333));
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

void specialKeys(GLFWwindow *window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE) {

		exit(0);

	}
}

int main(int argc, char **argv) {

	seedRand( time(NULL) );

	int n = 1 << 5;
	int n_nodes = n * n;
	int n_states = 1;
	// Display parameters
	double size = 0.8;
	double w = size / n;
	// Physical parameter of system
	double beta = 30.0;

	Bonds* bonds_A = new Bonds(A, n_nodes, a_coupling_energy);
	Bonds* bonds_J = new Bonds(J, n_nodes, j_coupling_energy);
	State* my_state = new State(n_nodes, n_states, beta, energy, bonds_A, bonds_J);

	// Set up couplings
	bonds_A->square2D(false);
	bonds_J->square2D(false);

	// Choices of algorithms
	RECA* my_reca = new RECA( my_state );
	Metropolis* my_metro = new Metropolis( my_state );

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
	int interval = 1 << 10;
	int count = interval;
	int n_steps = 0;
	while (!glfwWindowShouldClose(window)) {

		// Step the state forward
		//my_metro->evolve_state();
		// count--;
		// n_steps++;
		// if (count == 0) {
		//
		// 	display_state(my_state, n, w);
		// 	glfwSwapBuffers(window);
		// 	// count = interval;
		// 	// std::cout << n_steps << '\n';
		// 	// std::cout << "Press enter to continue\n";
    //   // getchar();
		// }
		display_state(my_state, n, w);
		glfwSwapBuffers(window);
		// Poll for and process events
		glfwPollEvents();

	}

	glfwTerminate();

	return 0;
}
