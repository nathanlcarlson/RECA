#ifndef STATE_HPP
#define STATE_HPP

#include <algorithm>
#include <functional>
#include <map>
#include <time.h>
#include "utils.hpp"
#include "node.hpp"
#include "bonds.hpp"


class State {

	typedef std::function <double (State*, int, int)> EnergyFunction_Ptr;

	private:

		double m_L;

		EnergyFunction_Ptr m_energy;
		// TODO Simplify bonds
		std::shared_ptr<Bonds> m_bonds;
		std::map<char, std::shared_ptr<Bonds>> m_bonds_map;
		std::shared_ptr<Node> m_node;
		std::vector<double> m_v;

	public:

		double B;

		State(int t_L, int t_B,
			    EnergyFunction_Ptr t_f,
					std::shared_ptr<Bonds> t_bonds,
					std::shared_ptr<Node> t_node)
			: B(t_B), m_energy(t_f), m_L(t_L), m_bonds(t_bonds), m_node(t_node)
		{
			m_bonds_map[t_bonds->get_id()] = t_bonds;
			m_v.resize(t_L);
			randomize_all();
		}

		// Calculate energy between i and j
		double energy(int i, int j) {
			return m_energy(this, i, j);
		}

		// Shift all with help of Node class
		void shift_all() {
			for(int i = 0; i < m_L; ++i) {
				m_v[i] = m_node->shifted_value(m_v[i]);
			}
		}

		// Shift one site with help of Node class
		void shift_one(int i, double _s) {
			m_v[i] = m_node->shifted_value(m_v[i], _s);
		}
		void shift_one(int i) {
			m_v[i] = m_node->shifted_value(m_v[i]);
		}
		void shift_one() {
			shift_one(randN(m_L));
		}
		// Generate shift value to use later
		double get_shift() {
			return m_node->get_shift();
		}

		// Randomizes the whole state with help of Node class
		void randomize_all() {
			for(int i = 0; i < m_L; ++i) {
				m_v[i] = m_node->random_value();
			}
		}

		// Randomizes one with help of Node class
		int randomize_one(int i) {
			m_v[i] = m_node->random_value();
			return i;
		}
		int randomize_one(){
			return randomize_one(randN(m_L));
		}

		// Get color with help of Node class
		void getColor(int i, std::vector<double>& _rgb_out) {
			return m_node->getRGB(m_v[i], _rgb_out);
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
			return m_bonds;
		}

		// Calculate total energy
		double total_energy() {
			double E = 0;
			for(int i = 0; i < m_L; i++) {
				for (auto neighbor = m_bonds->begin(i); neighbor != m_bonds->end(i); ++neighbor) {
					if( i < *neighbor) {
						E += energy(i, *neighbor);
					}
				}
			}
			return E;
		}

		int size() {
			return m_v.size();
		}

};
#endif
