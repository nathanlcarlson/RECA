#include "couplings.hpp"

StaticCouplings2D::StaticCouplings2D(char t_id, int t_n, std::string t_periodic, CouplingEnergyFunction_Ptr t_f)
  : m_energy(t_f), m_L(t_n), m_id(t_id)
{
	m_map.resize(t_n);
  m_neighbors.resize(t_n);
  if(t_periodic=="periodic"){
    square2D(true);
  }
  else if(t_periodic=="aperiodic"){
    square2D(false);
  }
}
char StaticCouplings2D::get_id(){
  return m_id;
}
void StaticCouplings2D::square2D(bool periodic) {

	int w = std::sqrt(m_L);
	int j, x, y;

	if (periodic) {
		for (int i = 0; i < m_L; i++) {
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
			j = mod(i + w, m_L);
			m_map[i][j] = m_energy(
			    make_node(x, y, 0),
			    make_node(j % w, j / w, 0));
			j = mod(i - w, m_L);
			m_map[i][j] = m_energy(
			    make_node(x, y, 0),
			    make_node(j % w, j / w, 0));
		}
	}
	else {
		for (int i = 0; i < m_L; i++) {
			x = i % w;
			y = i / w;
			if ((i + 1) % w != 0) {
				j = w * y + mod(i + 1, w);
				m_map[i][j] = m_energy(
				    make_node(x, y, 0),
				    make_node(j % w, j / w, 0));

			}
			if (x != 0) {
				j = w * y + mod(i - 1, w);
				m_map[i][j] = m_energy(
				    make_node(x, y, 0),
				    make_node(j % w, j / w, 0));

			}
			if (i + w < m_L) {
				j = mod(i + w, m_L);
				m_map[i][j] = m_energy(
				    make_node(x, y, 0),
				    make_node(j % w, j / w, 0));

			}
			if (i - w >= 0) {
				j = mod(i - w, m_L);
				m_map[i][j] = m_energy(
				    make_node(x, y, 0),
				    make_node(j % w, j / w, 0));

			}
		}
	}
  save_couplings();
}

double StaticCouplings2D::get(int t_i, int t_j) {

  return m_map[t_i][t_j];
}

void StaticCouplings2D::print() {

	int c = 0;
	for (CouplingsMap::iterator it = m_map.begin(); it != m_map.end(); ++it) {
		for (auto& x : (*it)) {
			std::cout << "( " << c << ", " << x.first << " ) => " << x.second << std::endl;
		}
		c++;
	}
}
void StaticCouplings2D::scale_all(double factor) {

  for (CouplingsMap::iterator it = m_map.begin(); it != m_map.end(); ++it) {
		for (auto& x : (*it)) {
			x.second = x.second*factor;
		}
	}
}
void StaticCouplings2D::save_couplings() {

  for(int i = 0; i < m_L; i++) {
    for (auto it = m_map[i].begin(); it != m_map[i].end(); ++it) {
      m_neighbors[i].push_back(it->first);
    }
    m_neighbors[i].shrink_to_fit();
  }
}
Neighborhood::iterator StaticCouplings2D::begin(int i) {

	return m_neighbors[i].begin();
}

Neighborhood::iterator StaticCouplings2D::end(int i) {

	return m_neighbors[i].end();
}
