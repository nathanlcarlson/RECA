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

			if(m_current == 0) {
				m_current = std::numeric_limits<unsigned int>::max() - 1;
				std::fill(m_cluster.begin(), m_cluster.end(), -1);
			}
		}
};

template <class Couplings>
class RECA {

	private:

		int m_N;

		State *m_state;
		Couplings *m_couplings;
		Cluster *m_cluster;

		std::vector <double> m_replica;

		void swap(int j) {

			double tmp = m_replica[j];
			m_replica[j] = (*m_state)[j];
			(*m_state)[j] = tmp;

		}

		void q_swap(int i, int j) {

			double E_i = m_state->energy(i, j);
			swap(j);
			double E_f = m_state->energy(i, j);

			double P = 1 - exp((m_state->B) * (E_f - E_i));

			if ( rand0_1() < P ) {
				propigate(j);
			}
			else {
				swap(j);
			}
		}

		void propigate(int i) {

			for (auto neighbor = m_couplings->begin(i); neighbor != m_couplings->end(i); ++neighbor) {

				int j = *neighbor;

				if (!(m_cluster->contains(j))) {

					m_cluster->add(j);
					q_swap(i, j);

				}
			}
		}

	public:
		RECA(State *t_state, Couplings *t_couplings)
			: m_couplings( t_couplings )
		{

			m_state = t_state;
			m_N = m_state->size();

			m_replica.resize(m_N);
			for (int i = 0; i < m_N; i++) {

				m_replica[i] = rand0_1();

			}

			m_cluster = new Cluster(m_N);

		}
		void evolve_state() {

			m_state->shift_all();

			int i = randN(m_N);
			swap(i);
	    m_cluster->add(i);
			propigate(i);

			m_cluster->clear();

		}

		double total_energy() {

			double E = 0;

			for(int i = 0; i < m_N; i++) {

				for (auto neighbor = m_couplings->begin(i); neighbor != m_couplings->end(i); ++neighbor) {

					E += m_state->energy(i, *neighbor);

				}
			}

			return E;

		}
};

template <class Couplings>
class Metropolis {

	private:

		int m_L;
		State *m_state;
		Couplings *m_couplings;

	public:

		Metropolis(State *t_state, Couplings *t_couplings)
			: m_couplings( t_couplings )
		{
			m_state = t_state;
			m_L = m_state->size();
		}

		void evolve_state() {

			// Select site to propose change
			int i = randN(m_L);
			double initial = (*m_state)[i];
			double E_i = 0;
			double E_f = 0;

			// Calculate initial energy
			for (auto neighbor = m_couplings->begin(i); neighbor != m_couplings->end(i); ++neighbor) {

				int j = *neighbor;
				E_i += m_state->energy(i, j);

			}

			// Make change
			m_state->randomize_one(i);

			// Calculate new energy
			for (auto neighbor = m_couplings->begin(i); neighbor != m_couplings->end(i); ++neighbor) {

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

		double total_energy() {

			double E = 0;
			for(int i = 0; i < m_L; i++) {

				for (auto neighbor = m_couplings->begin(i); neighbor != m_couplings->end(i); ++neighbor) {

					E += m_state->energy(i, *neighbor);

				}
			}

			return E;

		}
};
#endif
