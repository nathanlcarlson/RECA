#ifndef STATE_HPP
#define STATE_HPP

#include <algorithm>
#include <functional>
#include <map>
#include <time.h>
#include "utils.hpp"


class State {

	typedef std::function <double (State*, int, int)> EnergyFunction_Ptr;

	private:

		double m_N;
		double m_L;

		EnergyFunction_Ptr m_energy;
		std::vector <StaticCouplings2D*> m_bonds;
		std::map <char, StaticCouplings2D*> m_bonds_map;
		std::vector <double> m_v;
		std::vector <double> m_energy_history;
		std::vector <std::vector <double>> m_history;

	public:

		double B;

		template< typename... Args>
		State(int t_L, int t_N, int t_B, EnergyFunction_Ptr t_f, Args ...t_bonds)
			: B(t_B), m_energy(t_f), m_N(t_N), m_L(t_L), m_bonds({t_bonds...})
		{

			for( auto it = m_bonds.begin(); it != m_bonds.end(); it++) {
				m_bonds_map[(*it)->id] = *it;
			}
			m_v.resize(t_L);
			randomize_all();

		}

		double energy(int i, int j) {

			return m_energy(this, i, j);

		}

		void shift_all() {

			double r;

			if(m_N == 1) {
				r = rand0_1();
			}
			else {
				r = randN(m_N)/(double)m_N;
			}

			int n = m_v.size();

			for (int i = 0; i < n; i++) {

				m_v[i] += r;
				if (m_v[i] >= 1.0) {

					m_v[i] -= 1.0;

				}
			}
		}

		void randomize_all() {

			if(m_N == 1) {
				std::generate( m_v.begin(), m_v.end(), rand0_1 );
			}
			else {
				int n = m_v.size();
				for(int i = 0; i < n; i++) {

					double r = randN(m_N)/(double)m_N;
					m_v[i] = r;

				}
			}
		}

		int randomize_one(int i = -1) {

			if(i == -1) {
				i = randN(m_v.size());
			}

			if(m_N == 1) {
				m_v[i] = rand0_1();
			}
			else {
				m_v[i] = randN(m_N)/(double)m_N;
			}

			return i;
		}

		void save() {

			m_history.push_back(m_v);
			m_energy_history.push_back(total_energy());

		}

		double get_history(int t, int i) {
			return m_history[t][i];
		}

		void print() {

			std::cout << "State contains:";
			for (std::vector <double>::iterator it = m_v.begin(); it != m_v.end(); ++it) {
				std::cout << ' ' << *it;
			}
			std::cout << '\n';

		}

		double&operator[](int i) {

			return m_v[i];

		}

		StaticCouplings2D* bonds(char id) {

			return m_bonds_map[id];

		}

		StaticCouplings2D* bonds() {

			return m_bonds[0];

		}

		double total_energy() {

			double E = 0;

			for(int i = 0; i < m_L; i++) {

				for (auto neighbor = m_bonds[0]->begin(i); neighbor != m_bonds[0]->end(i); ++neighbor) {

					if( i < *neighbor) {

						E += energy(i, *neighbor);
					}
				}
			}

			return E;

		}

		double calc_avg_energy(const int t_steps) {

			double s = 0;
			for(const auto &energy : m_energy_history) {
				s += energy;
			}
			s /= t_steps;
			return s;
		}

		std::vector<std::vector<double>> history(){

			return m_history;
		}

		std::vector<double> energy_history(){

			return m_energy_history;
		}

		int size() {

			return m_v.size();

		}

		int steps() {

			return m_history.size();
		}
};
#endif
