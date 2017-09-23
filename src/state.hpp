#ifndef STATE_HPP
#define STATE_HPP

#include <algorithm>
#include <functional>
#include <time.h>
#include "utils.hpp"


class State
{
typedef std::function <double (int, int)> EnergyFunction_Ptr;
public:
	double B;
	State(int t_L, int t_B, EnergyFunction_Ptr t_f, int t_N = 1)
		: B(t_B), m_energy(t_f), m_N(t_N)
	{
		m_v.resize(t_L);
		seedRand( time(NULL) );
		randomize_all();
	}

	double energy(int i, int j)
	{
		return m_energy(i, j);
	}
	void shift_all()
	{
		double r;
		if(m_N == 1)
		{
			r = rand0_1();
		}
		else
		{
			r = randN(m_N)/(double)m_N;
		}
		int n = m_v.size();
		for (int i = 0; i < n; i++)
		{
			m_v[i] += r;
			if (m_v[i] >= 1.0)
			{
				m_v[i] -= 1.0;
			}
		}
	}
	void randomize_all()
	{
		if(m_N == 1)
		{
			std::generate( m_v.begin(), m_v.end(), rand0_1 );
		}
		else
		{
			int n = m_v.size();
			for(int i = 0; i < n; i++){
				double r = randN(m_N)/(double)m_N;
				m_v[i] = r;
			}
		}
	}
	int randomize_one(int i = -1)
	{
		if(i == -1)
		{
			i = randN(m_v.size());
		}
		if(m_N == 1)
		{
			m_v[i] = rand0_1();
		}
		else
		{
			m_v[i] = randN(m_N)/(double)m_N;
		}
		return i;
	}
	void print()
	{
		std::cout << "State contains:";
		for (std::vector <double>::iterator it = m_v.begin(); it != m_v.end(); ++it)
		{
			std::cout << ' ' << *it;
		}
		std::cout << '\n';
	}

	double&operator[](int i)
	{
		return m_v[i];
	}
	void save_current()
	{
		m_saved = m_v;
	}
	double diff_saved()
	{
		int n = m_v.size();
		double d;
		double s = 0;
		for(int i = 0; i < n; i++)
		{
			d = fabs(m_v[i] - m_saved[i]);
			if(d > 0.5) d = 1 - d;
			s += d;
		}
		return s;
	}
	int size()
	{
		return m_v.size();
	}

private:
	EnergyFunction_Ptr m_energy;
	std::vector <double> m_v;
	std::vector <double> m_saved;
	double m_N;
};
#endif
