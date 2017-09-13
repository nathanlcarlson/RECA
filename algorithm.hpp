#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <limits>
#include "state.hpp"

class Cluster
{
public:
	Cluster(int t_N)
	{
		m_current = std::numeric_limits<unsigned int>::max() - 1;
		m_cluster.resize(t_N);
	}
	bool contains(int i)
	{
		return m_cluster[i] == m_current;
	}
	void add(int i)
	{
		m_cluster[i] = m_current;
	}
	void clear()
	{
		m_current -= 1;
		if(m_current == 0)
		{
			m_current = std::numeric_limits<unsigned int>::max() - 1;
			std::fill(m_cluster.begin(), m_cluster.end(), -1);
		}
	}
private:
	std::vector< unsigned int > m_cluster;
	unsigned int m_current;
};

template <class Couplings>
class RECA
{
public:
	RECA(State *t_state, Couplings *t_couplings)
		: m_couplings( t_couplings )
	{
		m_state = t_state;
		m_N = m_state->size();
		m_w = std::sqrt(m_N);
		m_replica.resize(m_N);
		for (int i = 0; i < m_N; i++)
		{
			m_replica[i] = rand0_1();//(*m_state)[i];
		}
		m_cluster = new Cluster(m_N);
	}
	void evolve_state()
	{
		double R = rand0_1();

		for (int i = 0; i < m_N; i++)
		{
			(*m_state)[i] += R;
			if ((*m_state)[i] >= 1.0)
			{
				(*m_state)[i] -= 1.0;
			}
		}
		int i = randN(m_N);
		swap(i);
    m_cluster->add(i);
		propigate(i);
		m_cluster->clear();
	}

	double total_energy()
	{
		double E = 0;
		for(int i = 0; i < m_N; i++)
		{
			for (auto it = m_couplings->begin(i); it != m_couplings->end(i); ++it)
			{
				E += m_state->energy(i, it->first);
			}
		}
		return E;
	}

private:
	State *m_state;
	Couplings *m_couplings;
	Cluster *m_cluster;
	std::vector <double> m_replica;
	int m_N;
	int m_w;

	void swap(int j)
	{
		double tmp = m_replica[j];
		m_replica[j] = (*m_state)[j];
		(*m_state)[j] = tmp;
	}

	void q_swap(int i, int j)
	{
		double e1 = m_state->energy(i, j);

		swap(j);
		if ((1 - exp((m_state->B) * (m_state->energy(i, j) - e1))) > rand0_1())
		{
			propigate(j);
		}
		else
		{
			swap(j);
		}
	}

	void propigate(int i)
	{
		for (auto it = m_couplings->begin(i); it != m_couplings->end(i); ++it)
		{
			int j = it->first;
			if (!(m_cluster->contains(j)))
			{
				m_cluster->add(j);
				q_swap(i, j);
			}
		}
	}
};
template <class Couplings>
class Metropolis
{
public:
	Metropolis(State *t_state, Couplings *t_couplings)
		: m_couplings( t_couplings )
	{
		m_state = t_state;
		m_N = m_state->size();
		m_w = std::sqrt(m_N);
	}
	void evolve_state()
	{
		double R;
		if(m_state->N == 1.0)
		{
			R = rand0_1();
		}
		else
		{
			R = randN( m_state->N )/( m_state->N );
		}
		int i = randN(m_N);
		double initial = (*m_state)[i];
		double E_i = 0;
		double E_f = 0;

		for (auto it = m_couplings->begin(i); it != m_couplings->end(i); ++it)
		{
			int j = it->first;
			E_i += m_state->energy(i, j);
		}

		(*m_state)[i] = R;

		for (auto it = m_couplings->begin(i); it != m_couplings->end(i); ++it)
		{
			int j = it->first;
			E_f += m_state->energy(i, j);
		}
		double p = exp(-1 * (m_state->B) * (E_f - E_i));

		if (p < 1 && (1.0 - p) > rand0_1())
		{
			(*m_state)[i] = initial;
		}
	}

	double total_energy()
	{
		double E = 0;
		for(int i = 0; i < m_N; i++)
		{
			for (auto it = m_couplings->begin(i); it != m_couplings->end(i); ++it)
			{
				E += m_state->energy(i, it->first);
			}
		}
		return E;
	}

private:
	State *m_state;
	Couplings *m_couplings;
	int m_N;
	int m_w;
};
// Note the Wolff algorithm doesn't work for this physical model and isn't properly implemented
// template <class Couplings>
// class Wolff
// {
// public:
// 	Wolff(Couplings *t_couplings)
// 		: m_couplings( t_couplings )
// 	{}
// 	void evolve_state()
// 	{
// 		double R = rand0_1();
// 		int i = randN(m_N);
//
// 		(*m_state)[i] = R;
// 		propigate(i);
// 		m_traversed.clear();
// 	}
//
// 	void setState(State <Wolff> *t_state)
// 	{
// 		m_state = t_state;
// 		m_N = m_state->size();
// 		m_w = std::sqrt(m_N);
// 	}
//
// private:
// 	State <Wolff> *m_state;
// 	Couplings *m_couplings;
// 	std::vector <int> m_traversed;
// 	int m_N;
// 	int m_w;
// 	bool in_m_traversed(int i)
// 	{
// 		return std::find(m_traversed.begin(), m_traversed.end(), i) != m_traversed.end();
// 	}
//
// 	void q_swap(int i, int j)
// 	{
// 		double e1 = m_state->energy(i, j);
// 		double init = (*m_state)[j];
//
// 		(*m_state)[j] = (*m_state)[i];
// 		if ((1 - exp((m_state->B) * (m_state->energy(i, j) - e1))) > rand0_1())
// 		{
// 			propigate(j);
// 		}
// 		else
// 		{
// 			(*m_state)[j] = init;
// 		}
// 	}
//
// 	void propigate(int i)
// 	{
// 		for (auto it = m_couplings->begin(i); it != m_couplings->end(i); ++it)
// 		{
// 			int j = it->first;
// 			if (!(in_m_traversed(j)))
// 			{
// 				m_traversed.push_back(j);
// 				q_swap(i, j);
// 			}
// 		}
// 	}
// 	double total_energy()
// 	{
// 		double E = 0;
// 		for(int i = 0; i < m_N; i++)
// 		{
// 			for (auto it = m_couplings->begin(i); it != m_couplings->end(i); ++it)
// 			{
// 				E += m_state->energy(i, it->first);
// 			}
// 		}
// 		return E;
// 	}
// };
#endif
