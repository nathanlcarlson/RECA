#ifndef STATE_HPP
#define STATE_HPP

#include <algorithm>
#include <functional>
#include <map>
#include <time.h>
#include "utils.hpp"
#include "node.hpp"


class State {

	typedef std::function <double (State*, int, int)> EnergyFunction_Ptr;
	typedef StaticCouplings2D Bonds;

	private:

		double m_L;

		EnergyFunction_Ptr m_energy;
		// TODO Simplify bonds
		std::vector <std::shared_ptr<Bonds>> m_bonds;
		std::map <char, std::shared_ptr<Bonds>> m_bonds_map;
		std::shared_ptr <Node> m_node;
		std::vector <double> m_v;
		std::vector <double> m_energy_history;
		std::vector <std::vector <double>> m_history;

	public:

		double B;

		// TODO Simplify or create multiple  ructors
		template< typename TBonds >
		State(int t_L, int t_B,
			    EnergyFunction_Ptr t_f,
					TBonds t_bonds,
					std::shared_ptr<Node> t_node)
			: B(t_B), m_energy(t_f), m_L(t_L), m_bonds(t_bonds), m_node(t_node)
		{

			for(   auto& b: t_bonds ) {
				m_bonds_map[b->id] = b;
			}
			m_v.resize(t_L);
			randomize_all();

		}

		// Calculate energy between i and j
		double energy(  int i,   int j) {

			return m_energy(this, i, j);

		}

		// Shift all with help of Node class
		void shift_all() {

			m_node->set_shift();
			for(int i = 0; i < m_L; ++i) {
				m_v[i] = m_node->shifted_value(m_v[i]);
			}

		}

		// Randomizes the whole state with help of Node class
		void randomize_all() {

			for(int i = 0; i < m_L; ++i) {
				m_v[i] = m_node->random_value();
			}

		}

		// Randomizes one with help of Node class
		int randomize_one(int i = -1) {

			if(i == -1) {
				i = randN(m_v.size());
			}

			m_v[i] = m_node->random_value();

			return i;
		}

		// Get color with help of Node class
		std::vector<double> getColor(int i) {
			return m_node->getRGB(m_v[i]);
		}

		// TODO Replace this system for saving data with a more modular solution
		void save() {

			m_history.push_back(m_v);
			m_energy_history.push_back(total_energy());

		}

		void print() {

			std::cout << "State contains:";
			for ( auto& site: m_v) {
				std::cout << ' ' << site;
			}
			std::cout << '\n';

		}

		double&operator[](int i) {

			return m_v[i];

		}

		// Get bonds by ID
		std::shared_ptr<Bonds> bonds(char id) {

			return m_bonds_map[id];

		}

		// Get first bond in list
		std::shared_ptr<Bonds> bonds() {

			return m_bonds[0];

		}

		// Calculate total energy
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
		// TODO Replace this system for saving data with a more modular solution
		std::vector<std::vector<double>> history(){

			return m_history;
		}
		// TODO Replace this system for saving data with a more modular solution
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
