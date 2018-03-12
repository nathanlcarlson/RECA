#ifndef BONDS_HPP
#define BONDS_HPP

#include <unordered_map>
#include <functional>
#include "utils.hpp"

typedef std::function<double (node, node)> CouplingEnergyFunction_Ptr;
typedef std::unordered_map<int, double> Edge;
typedef std::vector<Edge> CouplingsMap;
typedef std::vector<int> Neighborhood;
typedef std::vector<Neighborhood> Neighbors;

class Bonds {
	protected:
		char m_id;
		
	public:

		virtual char get_id() = 0;

		virtual double get(int t_i, int t_j) = 0;

		virtual Neighborhood::iterator begin(int i) = 0;
		virtual Neighborhood::iterator end(int i) = 0;

};

#endif
