#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <limits>
#include <stack>
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

		Cluster m_cluster;
		std::shared_ptr<State> S_i;
		std::shared_ptr<State> S_j;
		std::stack<int> m_stack;

		void crotate_and_exchange(int i) {
			S_i->shift_one(i, -1*m_R);
			S_j->shift_one(i, m_R);

			double tmp = (*S_j)[i];
			(*S_j)[i] = (*S_i)[i];
			(*S_i)[i] = tmp;
		}

		// Check energy change and add to cluster, or not
		void q_swap(int i, int j) {
			double U_i = (*S_i)[j];
			double R_i = (*S_j)[j];

			double E_i = S_i->energy(i, j) + S_j->energy(i, j);
			crotate_and_exchange(j);
			double E_f = S_i->energy(i, j) + S_j->energy(i, j);
			m_step += 5;

			double P = 1.0 - exp((S_i->B) * (E_f - E_i));

			if ( rand0_1() < P ) {
				m_cluster.add(j);
				m_stack.push(j);
				// Propigate from this new site
				//propigate(j);
			}
			else {
				// Reset values to initial
				(*S_i)[j] = U_i;
				(*S_j)[j] = R_i;
			}
		}

		// Recursively called to grow cluster
		void propigate(int i) {
			for (auto neighbor = S_i->bonds()->begin(i); neighbor != S_i->bonds()->end(i); ++neighbor) {
				int j = *neighbor;
				if (!(m_cluster.contains(j))) {
					q_swap(i, j);
				}
			}
		}

	public:
		RECA(int _L)
			: m_L(_L), m_cluster(_L)
		{}

		void evolve_state(const std::shared_ptr<State>& _S_i, const std::shared_ptr<State>& _S_j) {

			S_i = _S_i;
			S_j = _S_j;

			// Set rotation
			m_R = S_i->get_shift();

			// Seed site
			int i = randN(m_L);
			crotate_and_exchange(i);
			m_cluster.add(i);

			// Propigate from seed site, building the cluster
			//propigate(i);
			m_stack.push(i);
			while(!m_stack.empty()){
				i = m_stack.top();
				m_stack.pop();
				propigate(i);
			}
			// Reaches here when all propigation fails, or the entire state is added to the cluster
			m_cluster.clear();

		}
		int step() {
		  return m_step;
		}
};

class Metropolis {

	private:

		int m_step = 0;

	public:

		Metropolis()
		{
		}

		void evolve_state(const std::shared_ptr<State>& _state) {

			// Select site to propose change
			int i = randN(_state->size());
			double initial = (*_state)[i];
			double E_i = 0;
			double E_f = 0;

			// Calculate initial energy
			for (auto neighbor = _state->bonds()->begin(i); neighbor != _state->bonds()->end(i); ++neighbor) {

				int j = *neighbor;
				E_i += _state->energy(i, j);
				m_step++;

			}

			// Make change
			_state->randomize_one(i);

			// Calculate new energy
			for (auto neighbor = _state->bonds()->begin(i); neighbor != _state->bonds()->end(i); ++neighbor) {

				int j = *neighbor;
				E_f += _state->energy(i, j);
				m_step++;

			}

			// Accept change?
			double P = exp((_state->B) * (E_i - E_f));
			m_step++;

			if (P < 1 && rand0_1() < (1.0 - P) ) {

				// Reject change, back to initial
				(*_state)[i] = initial;

			}
		}
		int step() {
		  return m_step;
		}

};
#endif
