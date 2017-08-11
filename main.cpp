#include <iostream>

#include "utils.hpp"
#include "fields.hpp"
#include "couplings.hpp"

//using namespace utils;
using namespace fields;
using namespace couplings;

double a_coulping_energy(utils::node i,
                         utils::node j){

  //return 1.0;
  return i.x * j.y - i.y * j.x;
}
double j_coulping_energy(utils::node i,
                         utils::node j){

  return 1.0;
  //return i.x * j.y - i.y * j.x;
}
//Main program
int main(int argc, char **argv) {

  //For outliving declared scope
  //auto myfield = std::make_unique<GeneralField>(10, 11.1);
  // myfield[1] += 7.0;
  // double value = myfield[1];
  // std::cout << value;
  int w = 1<<4;
  int n_nodes = w*w;
  GeneralField myfield(n_nodes, 10.0);
  GeneralCouplings A(n_nodes, a_coulping_energy);
  GeneralCouplings J(n_nodes, j_coulping_energy);
  A.square2D(false);
  J.square2D(false);

  //mycouplings(0, 4) = 11.0;


  A.print();
  J.print();

  return 0;
}
