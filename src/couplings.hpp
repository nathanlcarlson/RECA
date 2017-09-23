#ifndef COUPLINGS_HPP
#define COUPLINGS_HPP

#include <unordered_map>
#include <functional>
#include <iostream>
#include "utils.hpp"

typedef std::function <double (node, node)> CouplingEnergyFunction_Ptr;
typedef std::unordered_map <int, double> Edge;
typedef std::vector <Edge> CouplingsMap;
typedef std::vector <int> Neighborhood;
typedef std::vector <Neighborhood> Neighbors;
class StaticCouplings2D
{
public:
	StaticCouplings2D(int t_n, CouplingEnergyFunction_Ptr t_f);
	double&operator()(int t_i, int t_j);
	void square2D(bool periodic = true);
	void print();
	Neighborhood::iterator begin(int i);
	Neighborhood::iterator end(int i);

private:
	CouplingEnergyFunction_Ptr m_energy;
	CouplingsMap m_map;
	Neighbors m_neighbors;
	int m_size;
	void save_couplings();
};

#endif
