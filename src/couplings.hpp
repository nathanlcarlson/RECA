#ifndef COUPLINGS_HPP
#define COUPLINGS_HPP

#include <unordered_map>
#include <functional>
#include "utils.hpp"
#include "bonds.hpp"

class StaticCouplings2D : public Bonds{

	private:

		CouplingEnergyFunction_Ptr m_energy;
		CouplingsMap m_map;
		Neighbors m_neighbors;

		char m_id;
		int m_L;

		void save_couplings();
		void square2D(bool periodic);

	public:

		StaticCouplings2D(char t_id, int t_n, std::string periodic, CouplingEnergyFunction_Ptr t_f);
		char get_id();
		double get(int t_i, int t_j);
		void print();
		void scale_all( double factor );

		Neighborhood::iterator begin(int i);
		Neighborhood::iterator end(int i);

};

#endif
