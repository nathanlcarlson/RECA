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

class Wolff {
  public:
    Wolff()
    {}
    void evolve_state(){
      double R = rand0_1();
      int i = randN(m_N);
      (*m_state)[i] = R;
      propigate(i);
      m_traversed.clear();
    }
    void setState(State<Wolff>* t_state){
      m_state = t_state;
      m_N = m_state->size();
      m_w = std::sqrt(m_N);
    }
  private:
    State<Wolff>* m_state;
    std::vector<int> m_traversed;
    int m_N;
    int m_w;
    bool in_m_traversed(int i) {
      return std::find(m_traversed.begin(), m_traversed.end(), i) != m_traversed.end();
    }
    void q_swap(int i , int j){
      double e1 = m_state->energy(i, j);
      double init = (*m_state)[j];
      (*m_state)[j] = (*m_state)[i];
      if ((1 - exp( (m_state->m_B)*(m_state->energy(i, j) - e1) )) > rand0_1()){
        propigate(j);
      }
      else{
        (*m_state)[j] = init;
      }
    }
    void propigate(int i) {
      if( (i-m_w) >= 0 && !(in_m_traversed(i-m_w))){
        m_traversed.push_back(i-m_w);
        q_swap(i, i-m_w);
      }
      if( (i+m_w) < m_N && !(in_m_traversed(i+m_w))){
        m_traversed.push_back(i+m_w);
        q_swap(i, i+m_w);
      }
      if( ((i - 1) >= 0) && (((i - 1) % m_w) != (m_w-1)) && !(in_m_traversed(i-1)) ){
        m_traversed.push_back(i-1);
        q_swap(i, i-1);
      }
      if( ((i + 1) < m_N) && (((i + 1) % m_w) != 0) && !(in_m_traversed(i+1))){
        m_traversed.push_back(i+1);
        q_swap(i, i+1);
      }
    }
};

double rotate_y=0;
double rotate_x=0;

// The width of our 2D square and total number of nodes
int n = 1<<6;
int n_nodes = n*n;
// Display parameters
double size = 0.8;
double w = size/n;
// Physical parameter of system
double beta = 30.0;

StaticCouplings2D A(n_nodes, a_coulping_energy);
StaticCouplings2D J(n_nodes, j_coulping_energy);
RECA* my_reca = new RECA();
Metropolis* my_metro = new Metropolis();
Wolff* my_wolff = new Wolff();

//State<RECA> my_state(n_nodes, beta ,energy, my_reca);
//State<Metropolis> my_state(n_nodes, beta ,energy, my_metro);
State<Wolff> my_state(n_nodes, beta ,energy, my_wolff);

double a_coulping_energy(node i, node j){

  return (i.x * j.y - i.y * j.x)/(double)n;
}
double j_coulping_energy(node i, node j){

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

int main(int argc, char **argv) {

  //For outliving declared scope
  //auto myfield = std::make_unique<GeneralField>(10, 11.1);
  
  // Set up couplings  
  A.square2D(false);
  J.square2D(false);

  //glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LEQUAL);
  
  GLFWwindow* window;

  // Initialize the library
  if (!glfwInit())
      return -1;

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
  // Only render every 2^n steps
  int count = 0;
  int n_steps = 1 << 6;
  while (!glfwWindowShouldClose(window))
  {
      // Step the state forward
      my_state.evolve_state();

      count++;
      if (count == n_steps){
        display_state();
        // Swap front and back buffers
        glfwSwapBuffers(window);
        count = 0;
      }
      // Poll for and process events
      glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
