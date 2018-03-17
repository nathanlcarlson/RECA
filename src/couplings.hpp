#ifndef COUPLINGS_HPP
#define COUPLINGS_HPP

#include "utils.hpp"
#include "bonds.hpp"

class StaticCouplings2D : public Bonds{

	private:

		CouplingsMap m_map;
		Neighbors m_neighbors;

		int m_L;
		int m_w;

		void save_couplings();

	public:

		StaticCouplings2D(int _L);
		void square2DPeriodic();
		void square2DAperiodic();
		int x(int i);
		int y(int i);
		void print();
		void scale_all( double factor );

		double get(int i, int j);
		virtual double energy(int i, int j) = 0;

		const_iterator begin(int i);
		const_iterator end(int i);

};
#endif