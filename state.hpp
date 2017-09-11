#ifndef STATE_HPP
#define STATE_HPP

#include <algorithm>
#include <functional>
#include "utils.hpp"


class State
{
typedef std::function <double (int, int)> EnergyFunction_Ptr;
public:
	double B;

	State(int t_n, int t_B, EnergyFunction_Ptr t_f)
		: B(t_B), m_energy(t_f)
	{
		m_v.resize(t_n);
		seedRand(100);
		std::generate(m_v.begin(), m_v.end(), rand0_1);
	}

	double energy(int i, int j)
	{
		return m_energy(i, j);
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

	int size()
	{
		return m_v.size();
	}

private:
	EnergyFunction_Ptr m_energy;
	std::vector <double> m_v;
};
#endif
