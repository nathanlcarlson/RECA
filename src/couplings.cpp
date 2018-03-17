#include "couplings.hpp"

StaticCouplings2D::StaticCouplings2D(int t_n)
  : m_L(t_n)
{
	m_map.resize(t_n);
    m_neighbors.resize(t_n);
    m_w = std::sqrt(m_L);
}
int StaticCouplings2D::x(int i){
    return i % m_w;
}
int StaticCouplings2D::y(int i){
    return i / m_w;
}
void StaticCouplings2D::square2DPeriodic(){
    int j, _y;
    for (int i = 0; i < m_L; i++) {
        _y = y(i);
        j = m_w * _y + mod(i + 1, m_w);
        m_map[i][j] = energy(i,j);

        j = m_w * _y + mod(i - 1, m_w);
        m_map[i][j] = energy(i,j);

        j = mod(i + m_w, m_L);
        m_map[i][j] = energy(i,j);

        j = mod(i - m_w, m_L);
        m_map[i][j] = energy(i,j);
    }
    save_couplings();
}
void StaticCouplings2D::square2DAperiodic() {
    int j, _y;
	for (int i = 0; i < m_L; i++) {
        _y = y(i);
		if ((i + 1) % m_w != 0) {
			j = m_w * _y + mod(i + 1, m_w);
            m_map[i][j] = energy(i,j);
		}
		if (i % m_w != 0) {
			j = m_w * _y + mod(i - 1, m_w);
            m_map[i][j] = energy(i,j);
		}
		if (i + m_w < m_L) {
			j = mod(i + m_w, m_L);
            m_map[i][j] = energy(i,j);
		}
		if (i - m_w >= 0) {
			j = mod(i - m_w, m_L);
            m_map[i][j] = energy(i,j);
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
StaticCouplings2D::const_iterator StaticCouplings2D::begin(int i) {

	return m_neighbors[i].begin();
}

StaticCouplings2D::const_iterator StaticCouplings2D::end(int i) {

	return m_neighbors[i].end();
}
