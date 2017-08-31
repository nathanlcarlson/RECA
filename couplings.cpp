#include "couplings.hpp"


StaticCouplings2D::StaticCouplings2D(int t_n, CouplingEnergyFunction_Ptr t_f)
  : m_size(t_n), m_energy(t_f)
{
	m_map.resize(t_n);
}

double&StaticCouplings2D::operator()(int t_i, int t_j)
{
	return m_map[t_i][t_j];
}

void StaticCouplings2D::square2D(bool periodic)
{
	int w = std::sqrt(m_size);
	int j;
	int x;
	int y;

	if (periodic)
	{
		for (int i = 0; i < m_size; i++)
		{
			x = i % w;
			y = i / w;
			j = w * y + mod(i + 1, w);
			m_map[i][j] = m_energy(
			    make_node(x, y, 0),
			    make_node(j % w, j / w, 0));
			j = w * (y) + mod(i - 1, w);
			m_map[i][j] = m_energy(
			    make_node(x, y, 0),
			    make_node(j % w, j / w, 0));
			j = mod(i + w, m_size);
			m_map[i][j] = m_energy(
			    make_node(x, y, 0),
			    make_node(j % w, j / w, 0));
			j = mod(i - w, m_size);
			m_map[i][j] = m_energy(
			    make_node(x, y, 0),
			    make_node(j % w, j / w, 0));
		}
	}
	else
	{
		for (int i = 0; i < m_size; i++)
		{
			x = i % w;
			y = i / w;
			if ((i + 1) % w != 0)
			{
				j = w * y + mod(i + 1, w);
				m_map[i][j] = m_energy(
				    make_node(x, y, 0),
				    make_node(j % w, j / w, 0));
			}
			if (x != 0)
			{
				j = w * y + mod(i - 1, w);
				m_map[i][j] = m_energy(
				    make_node(x, y, 0),
				    make_node(j % w, j / w, 0));
			}
			if (i + w < m_size)
			{
				j = mod(i + w, m_size);
				m_map[i][j] = m_energy(
				    make_node(x, y, 0),
				    make_node(j % w, j / w, 0));
			}
			if (i - w >= 0)
			{
				j = mod(i - w, m_size);
				m_map[i][j] = m_energy(
				    make_node(x, y, 0),
				    make_node(j % w, j / w, 0));
			}
		}
	}
}

void StaticCouplings2D::print()
{
	int c = 0;

	for (CouplingsMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
	{
		for (auto& x : (*it))
		{
			std::cout << "( " << c << ", " << x.first << " ) => " << x.second << std::endl;
		}
		c++;
	}
}

Neighbors::iterator StaticCouplings2D::begin(int i)
{
	return m_map[i].begin();
}

Neighbors::iterator StaticCouplings2D::end(int i)
{
	return m_map[i].end();
}
