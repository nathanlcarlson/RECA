#ifndef ISING_HPP
#define ISING_HPP

#include "state.hpp"

class Ising: public State{
  public:
      Ising(int t_L, int t_B, std::shared_ptr<Bonds> t_bonds, std::shared_ptr<Node> t_node)
        : State(t_L, t_B, t_bonds, t_node) {}

      double energy(int i, int j) {
        return  -1.0 * m_v[i] * m_v[j];
      }
};
#endif