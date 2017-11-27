#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <limits>
#include "state.hpp"

class Cluster {

	private:

		std::vector< unsigned int > m_cluster;
		unsigned int m_current;

	public:

		Cluster(int t_N) {

			m_current = std::numeric_limits<unsigned int>::max() - 1;
			m_cluster.resize(t_N);

		}

		bool contains(int i) {

			return m_cluster[i] == m_current;

		}

		void add(int i) {

			m_cluster[i] = m_current;

		}
		void clear() {

			m_current -= 1;

			if(m_current == 1) {
				m_current = std::numeric_limits<unsigned int>::max() - 1;
				std::fill(m_cluster.begin(), m_cluster.end(), 0);
			}
		}
};

class RECA {

	private:
		double m_R;
		int m_L;

		std::shared_ptr<State> m_state;
		std::unique_ptr<State> m_replica;
		std::unique_ptr<Cluster> m_cluster;

		void swap(int j) {

			double tmp = (*m_replica)[j];
			(*m_replica)[j] = (*m_state)[j];
			(*m_state)[j] = tmp;

		}
		// TODO Implement this optimization
		// void rotate(int i) {
		//
		// 	(*m_replica)[i] += m_R;
		// 	if( (*m_replica)[i] >= 1.0 ) (*m_replica)[i] -= 1.0;
		// 	(*m_state)[i] -= m_R;
		// 	if( (*m_state)[i] < 0.0 ) (*m_state)[i] += 1.0;
		//
		// }
		void q_swap(int i, int j) {

			//double S_i = (*m_state)[j];
			//double U_i = (*m_replica)[j];

			double E_i = m_state->energy(i, j) + m_replica->energy(i, j);
			//rotate(j);
			swap(j);
			double E_f = m_state->energy(i, j) + m_replica->energy(i, j);;

			double P = 1 - exp((m_state->B) * (E_f - E_i));

			if ( rand0_1() < P ) {
				m_cluster->add(j);
				propigate(j);
			}
			else {
				swap(j);
			}
		}

		void propigate(int i) {

			for (auto neighbor = m_state->bonds()->begin(i); neighbor != m_state->bonds()->end(i); ++neighbor) {

				int j = *neighbor;

				if (!(m_cluster->contains(j))) {

					q_swap(i, j);

				}
			}
		}

	public:
		RECA(std::shared_ptr<State> t_state)
			: m_state(t_state)
		{
			m_L = m_state->size();
			m_replica = std::make_unique<State>( *m_state );
			m_cluster = std::make_unique<Cluster>( m_L );
		}
		void evolve_state() {

			//m_R = rand0_1();
			m_replica->shift_all();

			int i = randN(m_L);
			swap(i);
	    m_cluster->add(i);
			propigate(i);

			m_cluster->clear();

		}

		double total_energy() {

			double E = 0;

			for(int i = 0; i < m_L; i++) {

				for (auto neighbor = m_state->bonds()->begin(i); neighbor != m_state->bonds()->end(i); ++neighbor) {

					if( i < *neighbor) {

						E += m_state->energy(i, *neighbor);// + m_replica->energy(i, *neighbor);
					}
				}
			}

			return E;

		}
};

class Metropolis {

	private:

		int m_L;
		std::shared_ptr<State> m_state;

	public:

		Metropolis(std::shared_ptr<State> t_state)
			: m_state(t_state)
		{
			m_L = m_state->size();
		}

		void evolve_state() {

			// Select site to propose change
			int i = randN(m_L);
			double initial = (*m_state)[i];
			double E_i = 0;
			double E_f = 0;

			// Calculate initial energy
			for (auto neighbor = m_state->bonds()->begin(i); neighbor != m_state->bonds()->end(i); ++neighbor) {

				int j = *neighbor;
				E_i += m_state->energy(i, j);

			}

			// Make change
			m_state->randomize_one(i);

			// Calculate new energy
			for (auto neighbor = m_state->bonds()->begin(i); neighbor != m_state->bonds()->end(i); ++neighbor) {

				int j = *neighbor;
				E_f += m_state->energy(i, j);

			}

			// Accept change?
			double P = exp((m_state->B) * (E_i - E_f));

			if (P < 1 && rand0_1() < (1.0 - P) ) {

				// Reject change, back to initial
				(*m_state)[i] = initial;

			}
		}

};
#endif
