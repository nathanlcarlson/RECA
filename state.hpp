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
	double N;
	State(int t_L, int t_B, EnergyFunction_Ptr t_f, double t_N = 1.0)
		: B(t_B), m_energy(t_f)
	{
		N = t_N;
		m_v.resize(t_L);
		seedRand(100);
		if(t_N == 1.0)
		{
			std::generate( m_v.begin(), m_v.end(), rand0_1 );
		}
		else
		{

			for(int i = 0; i < t_L; i++){
				double r = randN(t_N)/t_N;
				std::cout << r << std::endl;
				m_v[i] = r;
			}
		}
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
