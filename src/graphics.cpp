#include <iostream>
#include <memory>
#include <math.h>
#include <GLFW/glfw3.h>

#include "utils.hpp"
#include "jja.hpp"
#include "ising.hpp"
#include "algorithm.hpp"

// Makes graphic
void display_state(State* state, int n, int n_states, int pos) {

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
			glVertex2f(x1, y1);
			glVertex2f(x1, y2);
			glVertex2f(x2, y1);

			glVertex2f(x1, y2);
			glVertex2f(x2, y1);
			glVertex2f(x2, y2);
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

	// N Replicas to use
	int n_states = atoi(argv[6]);

	//  Define states
	std::vector<State*>  state_pool;
	for(int i=0; i<n_states; ++i){
		state_pool.push_back(new JJA(n, beta));
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
	state_pool.clear();
	return 0;
}
