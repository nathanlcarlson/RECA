#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <random>
#include <array>
#include <vector>
#include <algorithm>
#include <GLFW/glfw3.h>

double rotate_y=0;
double rotate_x=0;
const float N = 32.0;
const int n = (int)N;
const int nsq = n*n;
const float size = 0.7;
const float w = size/N;
float B = 0.01;
std::mt19937 generator;
std::uniform_real_distribution<double> distribution(0.0, 1.0);

std::array< double, nsq > state;
std::array< double, nsq > replica;
std::vector< int > traversed;

struct rgb{
  double r;
  double g;
  double b;
} ;

bool in_traversed(int i) {
  return std::find(traversed.begin(), traversed.end(), i) != traversed.end();
}
double energy(int i, int j) {
  traversed.push_back(j);

  double E = cos(2.0*M_PI*(state[i] - state[j])) - cos(2.0*M_PI*(state[i] - replica[j]));
  printf("%f\n", E*B);

  return E;
}

void propigate(int i) {
  if( (i-n) >= 0 && !(in_traversed(i-n))){
    if( (1 - exp(B*energy(i, i-n))) >  distribution(generator)){
      double tmp = replica[i-n];
      replica[i-n] = state[i-n];
      state[i-n] = tmp;
      propigate(i-n);
    }
  }
  if( (i+n) < nsq && !(in_traversed(i+n))){
    if( (1 - exp(B*energy(i, i+n))) >  distribution(generator)){
      double tmp = replica[i+n];
      replica[i+n] = state[i+n];
      state[i+n] = tmp;
      propigate(i+n);
    }
  }
  if( ((i - 1) >= 0) && (((i - 1) % n) != (n-1)) && !(in_traversed(i-1)) ){
    if( (1 - exp(B*energy(i, i-1))) >  distribution(generator)){
      double tmp = replica[i-1];
      replica[i-1] = state[i-1];
      state[i-1] = tmp;
      propigate(i-1);
    }
  }
  if( ((i + 1) < nsq) && (((i + 1) % n) != 0) && !(in_traversed(i+1))){
    if( (1 - exp(B*energy(i, i+1))) >  distribution(generator)){
      double tmp = replica[i+1];
      replica[i+1] = state[i+1];
      state[i+1] = tmp;
      propigate(i+1);
    }
  }
}

void change_state(void) {

  // Alter
  double R = distribution(generator);
  for(int i=0; i<nsq; i++){
      replica[i] += R;
      if( replica[i] >= 1.0 )
        replica[i] -= 1.0;
  }
  //Seed site
  int i = round(distribution(generator)*(nsq-1));

  //Swap seed
  double tmp = replica[i];
  replica[i] = state[i];
  state[i] = tmp;

  propigate(i);
  traversed.clear();

}

double huetorgb(double t){

  if(t < 0.0) t += 1.0;
  if(t > 1.0) t -= 1.0;
  if(t < 0.16667) return 6.0 * t;
  if(t < 0.5) return 1.0;
  if(t < 0.6666) return (2.0/3.0 - t) * 6.0;
  return 0.0;
}
// Saturation = 1
// Lightness = 0.5
rgb hsltorgb(double h){
  rgb color;
  color.r = huetorgb(h + 0.3333);
  color.g = huetorgb(h);
  color.b = huetorgb(h - 0.3333);
  return color;
}
void display_state(void) {
  int c = 0;
  rgb color;
  glBegin(GL_TRIANGLES);
  for (int i = -n/2; i<n/2; i++){
    for (int j = -n/2; j<n/2; j++){
      color = hsltorgb(state[c]);
      glColor3f(color.r, color.g, color.b);
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
    B += 2.0;

  else if (key == GLFW_KEY_LEFT)
    B -= 2.0;

  else if (key == GLFW_KEY_UP)
    rotate_x += 5;

  else if (key == GLFW_KEY_DOWN)
    rotate_x -= 5;
}
//Main program
int main(int argc, char **argv) {

  //Initialize state
  for(int i=0; i<nsq; i++){
      state[i] = distribution(generator);
  }
  replica = state;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
      return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
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

      change_state();

      display_state();

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
