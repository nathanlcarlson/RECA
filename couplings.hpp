#ifndef COUPLINGS_HPP
#define COUPLINGS_HPP

#include <unordered_map>
#include <functional>
#include <iostream>
#include "utils.hpp"

typedef std::function<double( node, node )> CouplingEnergyFunction_Ptr;
typedef std::unordered_map<int, double> Neighbors;
typedef std::vector< Neighbors > CouplingsMap;
class StaticCouplings2D {

  public:

    StaticCouplings2D(int t_n, CouplingEnergyFunction_Ptr t_f);

    double &operator() (int t_i, int t_j);
    void square2D( bool periodic = true );
    void print();
    Neighbors::iterator begin(int i);
    Neighbors::iterator end(int i);
  private:
    CouplingEnergyFunction_Ptr m_energy;
    int m_size;
    CouplingsMap m_map;
};

#endif
