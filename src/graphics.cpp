#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>
#include "utils.hpp"
#include "couplings.hpp"
#include "state.hpp"
#include "algorithm.hpp"


double a_coulping_energy(node i, node j);
double j_coulping_energy(node i, node j);
double energy(int i, int j);

// The width of our 2D square and total number of nodes
int n = 1 << 7;
int n_nodes = n * n;
int N_states = 200;
// Display parameters
double size = 0.8;
double w = size / n;
// Physical parameter of system
double beta = 30.0;

// Couplings used to calculate energy
StaticCouplings2D A(n_nodes, a_coulping_energy);
StaticCouplings2D J(n_nodes, j_coulping_energy);
State my_state(n_nodes, beta, energy, N_states);

// Choices of algorithms, use info from couplings
RECA <StaticCouplings2D> *my_reca = new RECA <StaticCouplings2D>( &my_state, &A );
Metropolis <StaticCouplings2D> *my_metro = new Metropolis <StaticCouplings2D>( &my_state, &A );

double a_coulping_energy(node i, node j)
{
	return (i.x * j.y - i.y * j.x) / (double)n;
}

double j_coulping_energy(node i, node j)
{
	return 1.0;
}

double energy(int i, int j)
{
	return J(i, j) * cos(2 * M_PI * (my_state[i] - my_state[j] - A(i, j)));
}

void display_state(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	int c = 0;
	for (int i = -n / 2; i < n / 2; i++)
	{
		for (int j = -n / 2; j < n / 2; j++)
		{
			glBegin(GL_TRIANGLES);
			glColor3f(hueToRGB(my_state[c] + 0.3333),
			          hueToRGB(my_state[c]),
			          hueToRGB(my_state[c] - 0.3333));
			c++;
			//Make Square
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

void specialKeys(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		exit(0);
	}
	else if (key == GLFW_KEY_RIGHT)
	{
		my_state.B += 0.1;
	}
	else if (key == GLFW_KEY_LEFT)
	{
		my_state.B -= 0.1;
	}
}

int main(int argc, char **argv)
{
	// Set up couplings
	A.square2D(false);
	J.square2D(false);

	GLFWwindow *window;

	// Initialize the library
	if (!glfwInit())
	{
		return -1;
	}

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(640, 640, "State", NULL, NULL);
	if (!window)
	{
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

	while (!glfwWindowShouldClose(window))
	{
		// Step the state forward
		if(rand()%100 < 25)
		{
			my_metro->evolve_state();
		}
		else
		{
			my_reca->evolve_state();		
		}
		count--;
		if (count == 0)
		{
			display_state();
			glfwSwapBuffers(window);
			count = interval;

		}
		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
