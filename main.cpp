#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>
#include "utils.hpp"
#include "fields.hpp"
#include "couplings.hpp"
#include "state.hpp"


using namespace utils;
using namespace couplings;
using namespace state;

double a_coulping_energy(utils::node i, utils::node j);
double j_coulping_energy(utils::node i, utils::node j);
double energy(int i, int j);

double rotate_y=0;
double rotate_x=0;

int n = 1<<9;
int n_nodes = n*n;
double size = 0.8;
double w = size/n;
double beta = 1.0;
StaticCouplings2D A(n_nodes, a_coulping_energy);
StaticCouplings2D J(n_nodes, j_coulping_energy);
State my_state(n_nodes, beta ,energy);

double a_coulping_energy(utils::node i,
                         utils::node j){
  return (i.x * j.y - i.y * j.x)/(double)n;
}
double j_coulping_energy(utils::node i,
                         utils::node j){
  return 1.0;
}
double energy(int i, int j) {

  return  J(i,j)*cos( 2*M_PI*(my_state[i] - my_state[j] - A(i,j) ) );
}

void display_state(void) {
  int c = 0;
  glBegin(GL_TRIANGLES);
  for (int i = -n/2; i<n/2; i++){
    for (int j = -n/2; j<n/2; j++){
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
    }
  }
  glEnd();
}
void specialKeys(GLFWwindow* window, int key, int scancode, int action, int mods) {    //The current mouse coordinates)
  if (key == GLFW_KEY_ESCAPE )
    exit(0);

  else if (key == GLFW_KEY_RIGHT)
    my_state.m_B += 2.0;

  else if (key == GLFW_KEY_LEFT)
    my_state.m_B -= 2.0;

  else if (key == GLFW_KEY_UP)
    rotate_x += 5;

  else if (key == GLFW_KEY_DOWN)
    rotate_x -= 5;

}

//Main program
int main(int argc, char **argv) {

  //For outliving declared scope
  //auto myfield = std::make_unique<GeneralField>(10, 11.1);
  // myfield[1] += 7.0;
  // double value = myfield[1];
  // std::cout << value;
  //A(1,2) = 1.0;
  //A(2,1) = 3.0;
  //GeneralField myfield(n_nodes, 10.0);

  // Set up couplings
  A.square2D(false);
  J.square2D(false);

  //A.print();
  //J.print();

  // glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LEQUAL);

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

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, specialKeys);
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      // Reset transformations
      glLoadIdentity();
      // Rotate when user changes rotate_x and rotate_y
      glRotatef( rotate_x, 1.0, 0.0, 0.0 );
      glRotatef( rotate_y, 0.0, 1.0, 0.0 );

      my_state.evolve_state();

      display_state();
      std::cout << my_state.m_B << std::endl;
      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
