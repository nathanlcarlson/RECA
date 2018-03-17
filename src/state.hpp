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

	private:

		double m_L;

		Node& m_node;
		std::vector<double> m_v;

	public:

		double B;

		State(int t_L, int t_B, Node& t_node)
			: B(t_B), m_L(t_L), m_node(t_node)
		{
			m_v.resize(t_L);
		}

		// Get site i value
		double get(int i){
			return m_v[i];
		}
		// Get site i value
		double s(int i){
			return m_v[i];
		}
		virtual Bonds::const_iterator neighbors_begin(int i) = 0;
		virtual Bonds::const_iterator neighbors_end(int i) = 0;
		// Calculate energy between i and j
		virtual double energy(int i, int j) = 0;

		// Shift all with help of Node class
		void shift_all() {
			for(int i = 0; i < m_L; ++i) {
				m_v[i] = m_node.shifted_value(m_v[i]);
			}
		}

		// Shift one site with help of Node class
		void shift_one(int i, double _s) {
			m_v[i] = m_node.shifted_value(m_v[i], _s);
		}
		void shift_one(int i) {
			m_v[i] = m_node.shifted_value(m_v[i]);
		}
		void shift_one() {
			shift_one(randN(m_L));
		}
		// Generate shift value to use later
		double get_shift() {
			return m_node.get_shift();
		}

		// Randomizes the whole state with help of Node class
		void randomize_all() {
			for(int i = 0; i < m_L; ++i) {
				m_v[i] = m_node.random_value();
			}
		}

		// Randomizes one with help of Node class
		int randomize_one(int i) {
			m_v[i] = m_node.random_value();
			return i;
		}
		int randomize_one(){
			return randomize_one(randN(m_L));
		}

		// Get color with help of Node class
		void getColor(int i, std::vector<double>& _rgb_out) {
			return m_node.getRGB(m_v[i], _rgb_out);
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

		int size() {
			return m_v.size();
		}

};
#endif
