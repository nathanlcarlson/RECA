#ifndef STATE_HPP
#define STATE_HPP

#include <algorithm>
#include <functional>
#include <map>
#include <time.h>
#include "utils.hpp"


class State {

	typedef std::function <double (State*, int, int)> EnergyFunction_Ptr;
	typedef StaticCouplings2D Bonds;

	private:

		double m_L;

		EnergyFunction_Ptr m_energy;
		std::vector <std::shared_ptr<Bonds>> m_bonds;
		std::map <char, std::shared_ptr<Bonds>> m_bonds_map;
		std::vector <double> m_v;
		std::vector <double> m_energy_history;
		std::vector <std::vector <double>> m_history;

	public:

		double B;

		template< typename TBonds >
		State(int t_L, int t_B, EnergyFunction_Ptr t_f, TBonds t_bonds)
			: B(t_B), m_energy(t_f), m_L(t_L), m_bonds(t_bonds)
		{

			for( const auto& b: t_bonds ) {
				m_bonds_map[b->id] = b;
			}
			m_v.resize(t_L);
			randomize_all();

		}

		double energy(const int i, const int j) {

			return m_energy(this, i, j);

		}

		void randomize_all() {

			std::generate( m_v.begin(), m_v.end(), rand0_1 );

		}

		int randomize_one(int i = -1) {

			if(i == -1) {
				i = randN(m_v.size());
			}

			m_v[i] = rand0_1();

			return i;
		}

		void save() {

			m_history.push_back(m_v);
			m_energy_history.push_back(total_energy());

		}

		void print() {

			std::cout << "State contains:";
			for (const auto& site: m_v) {
				std::cout << ' ' << site;
			}
			std::cout << '\n';

		}

		double&operator[](int i) {

			return m_v[i];

		}

		std::shared_ptr<Bonds> bonds(char id) {

			return m_bonds_map[id];

		}

		std::shared_ptr<Bonds> bonds() {

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

		std::vector<std::vector<double>> history(){

			return m_history;
		}

		std::vector<double>& energy_history(){

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
