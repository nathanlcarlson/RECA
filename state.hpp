#ifndef STATE_HPP
#define STATE_HPP

#include <algorithm>
#include <functional>
#include "couplings.hpp"
#include "utils.hpp"
#include "algorithm.hpp"
class Algorithm;
class RECA;
class State{
  typedef std::function<double( int , int )> EnergyFunction_Ptr;
  friend class Algorithm;
  public:
    double m_B;

    State(int t_n, int t_B, EnergyFunction_Ptr t_f, int t_A);
    double &operator[] (int i);
    int size();
    void evolve_state();
    void print();

  private:
    EnergyFunction_Ptr m_energy;
    Algorithm *m_algorithm;
    std::vector<double> m_v;
};
#endif
