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

double rotate_y=0;
double rotate_x=0;

int n = 1<<8;
int n_nodes = n*n;
double size = 0.8;
double w = size/n;
double beta = 30.0;


StaticCouplings2D A(n_nodes, a_coulping_energy);
StaticCouplings2D J(n_nodes, j_coulping_energy);
RECA* my_reca = new RECA();
Metropolis* my_metro = new Metropolis();

//State<RECA> my_state(n_nodes, beta ,energy, my_reca);
State<Metropolis> my_state(n_nodes, beta ,energy, my_metro);

double a_coulping_energy(node i,
                         node j){
  return (i.x * j.y - i.y * j.x)/(double)n;
}
double j_coulping_energy(node i,
                         node j){
  return 1.0;
}
double energy(int i, int j) {

  return  J(i,j)*cos( 2*M_PI*(my_state[i] - my_state[j] - A(i,j) ) );
}

void display_state(void) {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  // Reset transformations
  glLoadIdentity();
  // Rotate when user changes rotate_x and rotate_y
  glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  glRotatef( rotate_y, 0.0, 1.0, 0.0 );
  int c = 0;
  for (int i = -n/2; i<n/2; i++){
    for (int j = -n/2; j<n/2; j++){
      glBegin(GL_TRIANGLES);
      glColor3f(hueToRGB(my_state[c] + 0.3333),
                hueToRGB(my_state[c]),
                hueToRGB(my_state[c] - 0.3333));
      c++;
      //Make Square
      glVertex3f( (1+2*i)*w-w, (1+2*j)*w-w, 0.0);
      glVertex3f( (1+2*i)*w-w, (1+2*j)*w+w, 0.0);
      glVertex3f( (1+2*i)*w+w, (1+2*j)*w-w, 0.0);
      glVertex3f( (1+2*i)*w-w, (1+2*j)*w+w, 0.0);
      glVertex3f( (1+2*i)*w+w, (1+2*j)*w-w, 0.0);
      glVertex3f( (1+2*i)*w+w, (1+2*j)*w+w, 0.0);
      glEnd();
    }
  }
}
void specialKeys(GLFWwindow* window, int key, int scancode, int action, int mods) {    //The current mouse coordinates)
  if (key == GLFW_KEY_ESCAPE )
    exit(0);

  else if (key == GLFW_KEY_RIGHT)
    my_state.m_B += 0.1;

  else if (key == GLFW_KEY_LEFT)
    my_state.m_B -= 0.1;

  else if (key == GLFW_KEY_UP)
    rotate_x += 5;

  else if (key == GLFW_KEY_DOWN)
    rotate_x -= 5;

}

//Main program
int main(int argc, char **argv) {

  //For outliving declared scope
  //auto myfield = std::make_unique<GeneralField>(10, 11.1);

  // Set up couplings
  A.square2D(false);
  J.square2D(false);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
      return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 640, "State", NULL, NULL);
  if (!window)
  {
      glfwTerminate();
      return -1;
  }
  //glfwSwapInterval(1);
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, specialKeys);
  /* Loop until the user closes the window */
  int count = 0;
  int n_steps = 1 << 12;
  while (!glfwWindowShouldClose(window))
  {
      /* Render here */
      my_state.evolve_state();
      count++;
      if (count == n_steps){
        display_state();
        glfwSwapBuffers(window);
        count = 0;
      }


      //std::cout << my_state.m_B << std::endl;
      /* Swap front and back buffers */

      /* Poll for and process events */
      glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
