#ifndef COUPLINGS_HPP
#define COUPLINGS_HPP

#include <unordered_map>
#include <functional>
#include <iostream>
#include "utils.hpp"

typedef std::function<double( node, node )> CouplingEnergyFunction_Ptr;
class StaticCouplings2D {
  public:
    StaticCouplings2D(int t_n, CouplingEnergyFunction_Ptr t_f);
    double &operator() (int t_i, int t_j);
    void square2D( bool periodic = true );
    void print();
  private:
    CouplingEnergyFunction_Ptr m_energy;
    int m_size;
    std::vector< std::unordered_map<int, double> > m_map;
};

#endif
