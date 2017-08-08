#include <iostream>

#include "utils.hpp"
#include "fields.hpp"
#include "graph.hpp"
#include "couplings.hpp"

using namespace utils;
using namespace fields;
using namespace couplings;

//Main program
int main(int argc, char **argv) {

  //For outliving declared scope
  //auto myfield = std::make_unique<GeneralField>(10, 11.1);
  // myfield[1] += 7.0;
  // double value = myfield[1];
  // std::cout << value;

  int n_nodes = 16*16;
  GeneralField myfield(n_nodes, 10.0);
  SquareCouplings mycouplings(n_nodes, 11.0, false);
  //mycouplings.couple(3, 4, 5.0);
  //mycouplings.couple(1, 2, 5.0);
  //mycouplings.square_uniform(10.0, false);
  mycouplings.print();

  return 0;
}
