#ifndef JJA_HPP
#define JJA_HPP

#include "state.hpp"

class JJA: public State{
  public:
      JJA(int t_L, int t_B, std::shared_ptr<Bonds> t_bonds, std::shared_ptr<Node> t_node)
        : State(t_L, t_B, t_bonds, t_node) {}

      double energy(int i, int j) {
        return -1*cos(m_v[i] - m_v[j] - m_bonds_map['A']->get(i,j));
      }
};
#endif