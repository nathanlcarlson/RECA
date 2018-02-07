#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <limits>
#include "state.hpp"

// Store info for cluster growth
class Cluster {

	private:
		// Holds cluster ids
		std::vector< unsigned int > m_cluster;
		// Current active cluster id
		unsigned int m_current;

	public:

		Cluster(int t_N) {
			// Initialize current ID to MAX UNSIGNED INT - 1
			m_current = std::numeric_limits<unsigned int>::max() - 1;

			// Resize to total number of nodes
			m_cluster.resize(t_N);

		}

		bool contains(int i) {

			// Check if node is in cluster by checking against the current ID
			return m_cluster[i] == m_current;

		}

		void add(int i) {

			// Add node to cluster by setting equal to the current cluster ID
			m_cluster[i] = m_current;

		}
		void clear() {

			// Decrement current, "removing" all nodes from cluster
			m_current -= 1;

			// Once current ID equals 1, reset it and fill with 0
			// Occurs after about 2^32 iterations
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
    int m_step = 0;

		std::shared_ptr<State> m_state;
		std::shared_ptr<State> m_replica;
		std::unique_ptr<Cluster> m_cluster;

		// TODO Implement this optimization
		void crotate_and_exchange(int i, std::shared_ptr<State> S_i, std::shared_ptr<State> S_j) {

			S_i->shift_one(i, -1*m_R);
			S_j->shift_one(i, m_R);

			double tmp = (*S_j)[i];
			(*S_j)[i] = (*S_i)[i];
			(*S_i)[i] = tmp;

		}

		// Check energy change and add to cluster, or not
		void q_swap(int i, int j, std::shared_ptr<State> S_i, std::shared_ptr<State> S_j) {
			double U_i = (*S_i)[j];
			double R_i = (*S_j)[j];

			double E_i = S_i->energy(i, j) + S_j->energy(i, j);
			crotate_and_exchange(j, S_i, S_j);
			double E_f = S_i->energy(i, j) + S_j->energy(i, j);
			m_step += 5;

			double P = 1 - exp((m_state->B) * (E_f - E_i));

			if ( rand0_1() < P ) {
				m_cluster->add(j);
				// Propigate from this new site
				propigate(j, S_i, S_j);
			}
			else {
				// Reset values to initial
				(*S_i)[j] = U_i;
				(*S_j)[j] = R_i;
			}
		}

		// Recursively called to grow cluster
		void propigate(int i, std::shared_ptr<State> S_i, std::shared_ptr<State> S_j) {
			for (auto neighbor = S_i->bonds()->begin(i); neighbor != S_i->bonds()->end(i); ++neighbor) {
				int j = *neighbor;
				if (!(m_cluster->contains(j))) {
					q_swap(i, j, S_i, S_j);
				}
			}
		}

	public:
		RECA(std::shared_ptr<State> t_state)
			: m_state(t_state)
		{
			m_L = m_state->size();

			m_replica = std::make_shared<State>( *m_state );
			m_replica->randomize_all();

			m_cluster = std::make_unique<Cluster>( m_L );
		}
		void evolve_state() {

			// Set rotation
			m_R = m_state->get_shift();

			// Seed site
			int i = randN(m_L);
			crotate_and_exchange(i, m_replica, m_state);
			m_cluster->add(i);

			// Propigate from seed site, building the cluster
			propigate(i, m_replica, m_state);
			m_cluster->clear();

		}
		int step() {
		  return m_step;
		}
};

class Metropolis {

	private:

		int m_L;
		int m_step = 0;
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
				m_step++;

			}

			// Make change
			m_state->randomize_one(i);

			// Calculate new energy
			for (auto neighbor = m_state->bonds()->begin(i); neighbor != m_state->bonds()->end(i); ++neighbor) {

				int j = *neighbor;
				E_f += m_state->energy(i, j);
				m_step++;

			}

			// Accept change?
			double P = exp((m_state->B) * (E_i - E_f));
			m_step++;

			if (P < 1 && rand0_1() < (1.0 - P) ) {

				// Reject change, back to initial
				(*m_state)[i] = initial;

			}
		}
		int step() {
		  return m_step;
		}

};
#endif
