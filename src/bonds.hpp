#ifndef BONDS_HPP
#define BONDS_HPP

#include <unordered_map>
#include <vector>

class Bonds {

	public:
		typedef std::unordered_map<int, double> Edge;
		typedef std::vector<Edge> CouplingsMap;
		typedef std::vector<int> Neighborhood;
		typedef std::vector<Neighborhood> Neighbors;
		typedef Neighborhood::const_iterator const_iterator;

		virtual double get(int i, int j) = 0;
		virtual const_iterator begin(int i) = 0;
		virtual const_iterator end(int i) = 0;

};

#endif
