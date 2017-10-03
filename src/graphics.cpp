#include <iostream>
#include <memory>
#include <math.h>
#include <GLFW/glfw3.h>
#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"

// The width of our 2D square and total number of nodes
int n = 1 << 6;
int n_nodes = n * n;
// Display parameters
double size = 0.8;
double w = size / n;
// Physical parameter of system
double beta = 30.0;

typedef StaticCouplings2D Bonds;
// Couplings used to calculate energy
Bonds *A = NULL;
Bonds *J = NULL;
State *my_state = NULL;

double a_coupling_energy(node i, node j) {

	return (i.x * j.y - i.y * j.x) / (double)n;

}

double j_coupling_energy(node i, node j) {

	return 1.0;

}

double energy(int i, int j) {

	return (*J)(i, j) * cos( 2 * M_PI * ( (*my_state)[i] - (*my_state)[j] - (*A)(i, j) ) );

}

void display_state(void) {

	glClear(GL_COLOR_BUFFER_BIT);
	int c = 0;
	for (int i = -n / 2; i < n / 2; i++) {

		for (int j = -n / 2; j < n / 2; j++) {

			glBegin(GL_TRIANGLES);
			// Get color
			glColor3f(hueToRGB((*my_state)[c] + 0.3333),
			          hueToRGB((*my_state)[c]),
			          hueToRGB((*my_state)[c] - 0.3333));
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
	else if (key == GLFW_KEY_RIGHT) {

		my_state->B += 0.1;

	}
	else if (key == GLFW_KEY_LEFT) {

		my_state->B -= 0.1;

	}
}

int main(int argc, char **argv) {

	A = new Bonds(n_nodes, a_coupling_energy);
	J = new Bonds(n_nodes, j_coupling_energy);
	my_state = new State(n_nodes, beta, energy);

	// Set up couplings
	A->square2D(false);
	J->square2D(false);

	// Choices of algorithms
	auto my_reca = std::unique_ptr<RECA<Bonds>>(new RECA<Bonds>( my_state, A ));
	auto my_metro = std::unique_ptr<Metropolis<Bonds>>(new Metropolis<Bonds>( my_state, A ));

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
		my_metro->evolve_state();
		count--;
		n_steps++;
		if (count == 0) {

			display_state();
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
